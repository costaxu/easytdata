%{

#define YYSTYPE _EasyTData*
#include <iostream>
#include "stdio.h"
#include "easytdata.h"
#include "json.y.hpp"
int nNotNegIntCnt=0;
int nNumCnt=0;
int nStringCnt=0;
extern int json2tdata_parse();

bool JsonParse(const char * pszJson);
extern _EasyTData* g_oJsonData;

static char escape_map[256] ;
char * json2tdata_pre_process_string(char * pc);
void json2tdata_init_escape_map();
%}
/*int �����������ʱ�ģ��ܲ�����*/
int [+-]*[0-9]+ 
/*num �����������ʱ�ģ��ܲ�����*/
num [+-]*([0-9]|\.)* 
/*string �����������ʱ�ģ��ܲ�����*/
string \"(\\.|[^\\"])*\"
ignore_char [ \t\r\n]
identifier [a-zA-Z_][a-zA-Z0-9_]*

%%
{identifier} {
        if(strcmp(yytext,"true")==0)
        {
            json2tdata_lval=ed_factory_bool(true);
            return TRUE;
        }
        else if(strcmp(yytext,"false")==0)
        {
            json2tdata_lval=ed_factory_bool(false);
            return FALSE;
        }
        else if(strcmp(yytext,"null")==0)
        {
            json2tdata_lval=ed_factory_none();
            return NIL;
        }
        else
        {
            json2tdata_lval=ed_factory_string(yytext);
            return IDENTIFIER;
        }
}

{num}  {
        json2tdata_lval=ed_factory_int(atoi(yytext));
        return NUM;
    }/*Ҫ���ָ�����*/


{string}  {
    /*ȥ��ǰ�����ŵĴ������浽TData���治��Ҫ����*/
    /**/
    json2tdata_pre_process_string(yytext);
    json2tdata_lval=ed_factory_string(yytext);
    return STRING;
}
"{" {return L_BRACE;}
"}" {return R_BRACE;}
"[" {return L_BRACKET;}
"]" {return R_BRACKET;}
":" {return COLON;}
";" {return SEMICOLON;}
"," {return COMMA;}
{ignore_char}
%%


//���ַ���pc����Ԥ������
//1 ȥ����ͷ�ͽ�β��˫����                                                          
//2 ����Ҫת����ַ�����ת��
//�����ַ��� 
char * json2tdata_pre_process_string(char * pc)                                     
{
    //�������ַ�����ʼ�ĵط�
    char * pcstart = pc;

    //pc1��Ϊдָ�룬 pc��Ϊ��ָ��                                                  
    //����Ҫת�������£� ֻ��Ҫ�Ѷ�ָ����ַ�������дָ���λ��                   
    //��Ҫת�������£� ����Ҫ�Ѷ�ָ��λ�õ������ַ�ת���һ���ַ���������дָ���λ��  
    char * pc1 = pc ;

    //�����ַ�����ʼ��˫����                                                        
    pc += 1;

    //��ǰ�ַ��Ƿ�ת��ı�־                                                      
    int escape_flag = 0;

    while(*pc)                                                                      
    {
        escape_flag = 0;
        //�����ǰ�ַ�Ϊ'\'                                                         
        if(*pc == '\\')
        {
            //������һ���ַ��� �����һ���ַ�����ת�壬 �����ת��                  
            if( escape_map[*(pc+1)])                                                
            {
                escape_flag = 1;                                                    
                //�����Ҫת��, ���ָ����ǰ�ƶ������ֽڣ� дָ��ֻ�ƶ�һ���ֽ�     
                *pc1 ++ = escape_map[*(++pc)];                                      
                pc++;
            }
        }   
        //�������Ҫת�壬��дָ�붼��ǰ�ƶ� 
        if(!escape_flag )    
        {
            *pc1++ = *pc++;
        }
    }

    //���Խ�β��˫����
    *(pc1-1) = 0;
    return pcstart;
}



int yywrap()
{
    return 1;
}

void usage()
{
    printf(
    "usage: parse json to an object\n"
    "json filename\n"
    "json file format: \n"
    "each line in file is an json object\n"
    
    );
}

bool JsonParse(const char * pszJson)
{
    g_oJsonData = 0;
    json2tdata_init_escape_map();
    yy_scan_string(pszJson);
    return json2tdata_parse();
}

void json2tdata_error(const char*msg)
{
    std::cerr << msg << std::endl;
}

//��ʼ��ת���ַ��б�
//���� \n ת��ɻ���
void json2tdata_init_escape_map()
{
    bzero(escape_map, sizeof(escape_map));
    escape_map['"'] = '"';
    escape_map['\\']= '\\';
    escape_map['/'] = '/';
    escape_map['b'] = '\b';
    escape_map['f'] = '\f';
    escape_map['n'] = '\n';
    escape_map['r'] = '\r';
    escape_map['t'] = '\t';
}

int main( int argc, char** argv)
{
    if(argc == 1)
    {
        usage();
        return 0;
    }

    char * sFileName =argv[1];
    FILE * f = fopen(sFileName, "r");
    if(f==0)
    {
        usage();
        return 0;
    }
    char sBuffer[1024]={0};
    while(fgets(sBuffer, sizeof(sBuffer), f))
    {
        printf("  Buffer: %s", sBuffer);
        JsonParse(sBuffer);
        printf("ed_print: ");
        ed_print(g_oJsonData);
        printf("\n\n");
    }
    return 0;

}