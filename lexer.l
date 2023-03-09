%option noyywrap c++
%option yyclass="Lexer"
%{
#include <string>
#include <iostream>
#include "lexer.h"
#include "driver.hpp"
#include "parser.hpp"
using namespace std;
%}

%%

[ \t]                    ;
[\n]                    {cout << endl;}
"var"                   {cout << "TK_VAR" << " "; return carbonic_c::Parser::make_TK_VAR();} 
"type"                  {cout << "TK_TYPE" << " "; return carbonic_c::Parser::make_TK_TYPE();}
"integer"               {cout << "TK_INT" << " "; return carbonic_c::Parser::make_TK_INT();} 
"real"                  {cout << "TK_DOUBLE" << " "; return carbonic_c::Parser::make_TK_DOUBLE();}
"boolean"               {cout << "TK_BOOL" << " "; return carbonic_c::Parser::make_TK_BOOL();}
"char"                  {cout << "TK_CHAR" << " "; return carbonic_c::Parser::make_TK_CHAR();}
"string"                {cout << "TK_STRING" << " "; return carbonic_c::Parser::make_TK_STRING();}
"array"                 {cout << "TK_ARRAY" << " "; return carbonic_c::Parser::make_TK_ARRAY();}
"record"                {cout << "TK_RECORD" << " "; return carbonic_c::Parser::make_TK_RECORD();}
"routine"               {cout << "TK_ROUTINE" << " "; return carbonic_c::Parser::make_TK_ROUTINE();}
"return"                {cout << "TK_RETURN" << " "; return carbonic_c::Parser::make_TK_RETURN();}
"is"                    {cout << "TK_IS" << " "; return carbonic_c::Parser::make_TK_IS();}
"not"                   {cout << "TK_NOT" << " "; return carbonic_c::Parser::make_TK_NOT();} 
"while"                 {cout << "TK_WHILE" << " "; return carbonic_c::Parser::make_TK_WHILE();}
"loop"		        	{cout << "TK_LOOP" << " "; return carbonic_c::Parser::make_TK_LOOP();}
"end"                   {cout << "TK_END" << " "; return carbonic_c::Parser::make_TK_END();} 
"for"                   {cout << "TK_FOR" << " "; return carbonic_c::Parser::make_TK_FOR();} 
"foreach"               {cout << "TK_FOREACH" << " "; return carbonic_c::Parser::make_TK_FOREACH();}
"from"                  {cout << "TK_FROM" << " "; return carbonic_c::Parser::make_TK_FROM();}
"if"                    {cout << "TK_IF" << " "; return carbonic_c::Parser::make_TK_IF();}
"then"                  {cout << "TK_THEN" << " "; return carbonic_c::Parser::make_TK_THEN();}
"else"                  {cout << "TK_ELSE" << " "; return carbonic_c::Parser::make_TK_ELSE();}
"in"                    {cout << "TK_IN" << " "; return carbonic_c::Parser::make_TK_IN();}
"reverse"               {cout << "TK_REVERSE" << " "; return carbonic_c::Parser::make_TK_REVERSE();}
"print"                 {cout << "TK_PRINT" << " "; return carbonic_c::Parser::make_TK_PRINT();}
"newline"               {cout << "TK_NEWLINE" << " "; return carbonic_c::Parser::make_TK_NEWLINE();} 
true                    {cout << "TK_TRUE" << " "; return carbonic_c::Parser::make_TK_TRUE(true);}
false                   {cout << "TK_FALSE" << " "; return carbonic_c::Parser::make_TK_FALSE(false);}
":="                    {cout << "TK_ASSIGN" << " "; return carbonic_c::Parser::make_TK_ASSIGN();}
"="                     {cout << "TK_CEQ" << " "; return carbonic_c::Parser::make_TK_CEQ();} 
"/="                    {cout << "TK_CNEQ" << " "; return carbonic_c::Parser::make_TK_CNEQ();}
"<"                     {cout << "TK_CLT" << " "; return carbonic_c::Parser::make_TK_CLT();} 
"<="                    {cout << "TK_CLE" << " "; return carbonic_c::Parser::make_TK_CLE();} 
">"                     {cout << "TK_CGT" << " "; return carbonic_c::Parser::make_TK_CGT();} 
">="                    {cout << "TK_CGE" << " "; return carbonic_c::Parser::make_TK_CGE();} 
"("                     {cout << "TK_LPAREN" << " "; return carbonic_c::Parser::make_TK_LPAREN();}
")"                     {cout << "TK_RPAREN" << " "; return carbonic_c::Parser::make_TK_RPAREN();}
"["                     {cout << "TK_LSQBRK" << " "; return carbonic_c::Parser::make_TK_LSQBRK();}
"]"                     {cout << "TK_RSQBRK" << " "; return carbonic_c::Parser::make_TK_RSQBRK();}
"{"                     {cout << "TK_LBRACK" << " "; return carbonic_c::Parser::make_TK_LBRACK();}
"}"                     {cout << "TK_RBRACK" << " "; return carbonic_c::Parser::make_TK_RBRACK();}
"."                     {cout << "TK_DOT" << " "; return carbonic_c::Parser::make_TK_DOT();} 
","                     {cout << "TK_COMMA" << " "; return carbonic_c::Parser::make_TK_COMMA();}
":"                     {cout << "TK_COLON" << " "; return carbonic_c::Parser::make_TK_COLON();}
";"                     {cout << "TK_SCOLON" << " "; return carbonic_c::Parser::make_TK_SCOLON();}
".."                    {cout << "TK_DDOT" << " "; return carbonic_c::Parser::make_TK_DDOT();}
"+"                     {cout << "TK_PLUS" << " "; return carbonic_c::Parser::make_TK_PLUS();}
"-"                     {cout << "TK_MINUS" << " "; return carbonic_c::Parser::make_TK_MINUS();}
"*"                     {cout << "TK_MUL" << " "; return carbonic_c::Parser::make_TK_MUL();} 
"/"                     {cout << "TK_DIV" << " "; return carbonic_c::Parser::make_TK_DIV();}
"%"                     {cout << "TK_MOD" << " "; return carbonic_c::Parser::make_TK_MOD();} 
"**"                    {cout << "TK_POW" << " "; return carbonic_c::Parser::make_TK_POW();} 
"and"                   {cout << "TK_AND" << " "; return carbonic_c::Parser::make_TK_AND();} 
"or"                    {cout << "TK_OR" << " "; return carbonic_c::Parser::make_TK_OR();} 
"xor"                   {cout << "TK_XOR" << " "; return carbonic_c::Parser::make_TK_XOR();}
'.'                     {cout << "TK_CHARACTER_LITERAL" << " "; return carbonic_c::Parser::make_TK_CHARACTER_LITERAL(*yytext);}
\"([^\\\"]|\\.)*\"      {cout << "TK_STRING_LITERAL" << " "; return carbonic_c::Parser::make_TK_STRING_LITERAL(yytext);} /*TODO: new_token*/
\/\/(.*)                cout << "TK_COMMENT" << " "; /*TODO: will ignore and do nothing*/
[a-zA-Z_][a-zA-Z0-9_]*  {cout << "TK_IDENTIFIER" << " "; return carbonic_c::Parser::make_TK_IDENTIFIER();}   /*TODO: new_token*/
[0-9]+\.[0-9]+          {cout << "TK_DOUBLE_LITERAL" << " "; return carbonic_c::Parser::make_TK_DOUBLE_LITERAL(atof(yytext));} 
[0-9]+                  {cout << "TK_INTEGER_LITERAL" << " "; return carbonic_c::Parser::make_TK_INTEGER_LITERAL(atoi(yytext));}  
.                       cout << "LEXICAL ERROR: INVALID PARAMETER FOR THE LEXEME -> " << yytext << " ";
%%


