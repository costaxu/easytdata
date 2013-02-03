%{
  	#include <iostream>
  	#include "stdio.h"
  	//#include "TData.h"
  	#include "easytdata.h"
//	#define YYDEBUG 1
	//#define YYSTYPE TData
	#define YYSTYPE _EasyTData*
	  	
	extern int json2tdata_lex();
	void json2tdata_error(const char*msg);
	
	
	//TData g_oJsonData; //结果存放点 
	_EasyTdata *g_oJsonData; //结果存放点 
	//TData * g_pCurrData; //当前节点
	
%}
%token INT NUM STRING IGNORE_CHAR L_BRACE R_BRACE L_BRACKET R_BRACKET COLON SEMICOLON COMMA IDENTIFIER TRUE FALSE NIL
%%

Json	: Value {g_oJsonData=$1;/*printf("=========\nResult ToJson():%s",g_oJsonData.ToJson().c_str());*/}

Object	: L_BRACE Pairs R_BRACE {$$=$2;}
		| L_BRACE R_BRACE {$$=ed_factory_map();}

Array	: L_BRACKET Elements R_BRACKET {$$=$2;}
		| L_BRACKET R_BRACKET {$$=ed_factory_vector();}

ID		: NUM {$$=$1;}
		| STRING {$$=$1;}
		| IDENTIFIER{$$=$1;}

Pair	: ID COLON Value 
     {
        //TData tmp;
        //tmp[$1.ToString()]=$3;
        //$$.AddData(tmp);
        $$ = ed_factory_pair($1, $3); 
        /*
            printf("\n**$1:%s,$3:%s,$$:%s**\n",$1.ToJson().c_str(),$3.ToJson().c_str(),$$.ToJson().c_str());
        */
    }

Pairs	: Pairs COMMA Pair {
            //$$.AddData($3);
            ed_map_add_pair($1,$3);
            $$ = $1;
        }
		| Pair {
            //$$=$1;/*printf("\n**$1:%s,$$:%s**\n",$1.ToJson().c_str(),$$.ToJson().c_str());*/        
            $$=ed_factory_map();
            ed_map_add_pair($$, $1);
        }


Value	: NUM {
            $$=$1;
        }
		| STRING {
            $$=$1;

        }
		| Object {$$=$1;}
		| Array {$$=$1;}
		| FALSE {
            //$$=false;
            $$=$1;
            }
		| TRUE {
            //$$=true;
            $$=$1;
            }
		| NIL {
            //$$=0;
            $$=$1;
            //$$ = ed_factory_none();
        }

Elements	: Elements COMMA Value {
            //$1.AddItem($3);
            //$$=$1;
            ed_vector_add($1,$3);
            $$ = $1;

        }
		| Value {
            //$$=TData::Array();
            //$$.AddItem($1);
            $$ = ed_factory_vector();
            ed_vector_add($$,$1);
        }



%%

