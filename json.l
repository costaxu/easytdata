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
/*int 这个规则是暂时的，很不完整*/
int [+-]*[0-9]+ 
/*num 这个规则是暂时的，很不完整*/
num [+-]*([0-9]|\.)* 
/*string 这个规则是暂时的，很不完整*/
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
    }/*要区分浮点数*/


{string}  {
    /*去掉前后引号的处理，存到TData里面不需要引号*/
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


//对字符串pc进行预处理：
//1 去掉开头和结尾的双引号                                                          
//2 对需要转义的字符进行转义
//返回字符串 
char * json2tdata_pre_process_string(char * pc)                                     
{
    //保存下字符串开始的地方
    char * pcstart = pc;

    //pc1作为写指针， pc作为读指针                                                  
    //不需要转义的情况下， 只需要把读指针的字符拷贝到写指针的位置                   
    //需要转义的情况下， 则需要把读指针位置的两个字符转义成一个字符，拷贝到写指针的位置  
    char * pc1 = pc ;

    //跳过字符串开始的双引号                                                        
    pc += 1;

    //当前字符是否被转义的标志                                                      
    int escape_flag = 0;

    while(*pc)                                                                      
    {
        escape_flag = 0;
        //如果当前字符为'\'                                                         
        if(*pc == '\\')
        {
            //读出下一个字符， 如果下一个字符可以转义， 则进行转义                  
            if( escape_map[*(pc+1)])                                                
            {
                escape_flag = 1;                                                    
                //如果需要转义, 则读指针向前移动两个字节， 写指针只移动一个字节     
                *pc1 ++ = escape_map[*(++pc)];                                      
                pc++;
            }
        }   
        //如果不需要转义，读写指针都向前移动 
        if(!escape_flag )    
        {
            *pc1++ = *pc++;
        }
    }

    //忽略结尾的双引号
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

//初始化转义字符列表
//比如 \n 转义成换行
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
