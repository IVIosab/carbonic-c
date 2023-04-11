%option noyywrap c++
%option yyclass="Lexer"
%option yylineno
%{
#include <string>
#include <iostream>
#include "lexer.h"
#include "driver.hpp"
#include "parser.hpp"
using namespace std;
FlexLexer *lexer = new yyFlexLexer;

int lineno = 1 ;

void increase_no(){
    lineno++;
}
int getno(){
    return lineno;
}

%}

%%

[ \t]                    ;
[\n]                    {increase_no();}
"var"                   {return carbonic_c::Parser::make_TK_VAR();} 
"type"                  {return carbonic_c::Parser::make_TK_TYPE();}
"integer"               {return carbonic_c::Parser::make_TK_INTEGER();} 
"real"                  {return carbonic_c::Parser::make_TK_REAL();}
"boolean"               {return carbonic_c::Parser::make_TK_BOOLEAN();}
"array"                 {return carbonic_c::Parser::make_TK_ARRAY();}
"record"                {return carbonic_c::Parser::make_TK_RECORD();}
"routine"               {return carbonic_c::Parser::make_TK_ROUTINE();}
"return"                {return carbonic_c::Parser::make_TK_RETURN();}
"is"                    {return carbonic_c::Parser::make_TK_IS();}
"while"                 {return carbonic_c::Parser::make_TK_WHILE();}
"loop"		        	{return carbonic_c::Parser::make_TK_LOOP();}
"end"                   {return carbonic_c::Parser::make_TK_END();} 
"for"                   {return carbonic_c::Parser::make_TK_FOR();} 
"from"                  {return carbonic_c::Parser::make_TK_FROM();}
"if"                    {return carbonic_c::Parser::make_TK_IF();}
"then"                  {return carbonic_c::Parser::make_TK_THEN();}
"else"                  {return carbonic_c::Parser::make_TK_ELSE();}
"in"                    {return carbonic_c::Parser::make_TK_IN();}
"reverse"               {return carbonic_c::Parser::make_TK_REVERSE();}
"print"                 {return carbonic_c::Parser::make_TK_PRINT();}
":="                    {return carbonic_c::Parser::make_TK_ASSIGN();}
"="                     {return carbonic_c::Parser::make_TK_EQ();} 
"/="                    {return carbonic_c::Parser::make_TK_NEQ();}
"<"                     {return carbonic_c::Parser::make_TK_LT();} 
"<="                    {return carbonic_c::Parser::make_TK_LEQ();} 
">"                     {return carbonic_c::Parser::make_TK_GT();} 
">="                    {return carbonic_c::Parser::make_TK_GEQ();} 
"("                     {return carbonic_c::Parser::make_TK_LPAREN();}
")"                     {return carbonic_c::Parser::make_TK_RPAREN();}
"["                     {return carbonic_c::Parser::make_TK_LSQUARE();}
"]"                     {return carbonic_c::Parser::make_TK_RSQUARE();}
"."                     {return carbonic_c::Parser::make_TK_DOT();} 
","                     {return carbonic_c::Parser::make_TK_COMMA();}
":"                     {return carbonic_c::Parser::make_TK_COLON();}
";"                     {return carbonic_c::Parser::make_TK_SEMICOLON();}
".."                    {return carbonic_c::Parser::make_TK_DDOT();}
"+"                     {return carbonic_c::Parser::make_TK_PLUS();}
"-"                     {return carbonic_c::Parser::make_TK_MINUS();}
"*"                     {return carbonic_c::Parser::make_TK_MUL();} 
"/"                     {return carbonic_c::Parser::make_TK_DIV();}
"%"                     {return carbonic_c::Parser::make_TK_MOD();} 
"and"                   {return carbonic_c::Parser::make_TK_AND();} 
"or"                    {return carbonic_c::Parser::make_TK_OR();} 
"xor"                   {return carbonic_c::Parser::make_TK_XOR();}
[0-9]+\.[0-9]+          {return carbonic_c::Parser::make_TK_REAL_LITERAL(atof(yytext));} 
[0-9]+                  {return carbonic_c::Parser::make_TK_INTEGER_LITERAL(atoi(yytext));}
\/\/(.*)                {/*: will ignore and do nothing*/}
true                    {return carbonic_c::Parser::make_TK_BOOLEAN_LITERAL(true);}
false                   {return carbonic_c::Parser::make_TK_BOOLEAN_LITERAL(false);}
[a-zA-Z_][a-zA-Z0-9_]*  {return carbonic_c::Parser::make_TK_IDENTIFIER(yytext);}
.                       {cout<<"Undifined token "<< yytext<< endl <<"Line number: "<< getno() <<endl; exit(1);}  
%%
