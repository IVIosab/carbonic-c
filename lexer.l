%option noyywrap c++
%option yyclass="Lexer"
%{
#include <string>
#include <iostream>
#include "lexer.h"
#include "driver.hpp"
#include "parser.hpp"
using namespace std;

// Define token types
enum token_type {
    TK_VAR,
    TK_TYPE,
    TK_INT,
    TK_DOUBLE,
    TK_BOOL,
    TK_CHAR,
    TK_ARRAY,
    TK_RECORD,
    TK_ROUTINE,
    TK_RETURN,
    TK_IS,
    TK_NOT,
    TK_WHILE,
    TK_LOOP,
    TK_END,
    TK_FOR,
    TK_FOREACH,
    TK_FROM,
    TK_IF,
    TK_THEN,
    TK_ELSE,
    TK_IN,
    TK_REVERSE,
    TK_STRING_LITERAL,
    TK_IDENTIFIER,
    TK_DOUBLE_LITERAL,
    TK_INTEGER_LITERAL,
    TK_CHARACTER_LITERAL,
    TK_TRUE,
    TK_FALSE,
    TK_PRINT,
    TK_ASSIGN,
    TK_CEQ,
    TK_CNEQ,
    TK_CLT,
    TK_CLE,
    TK_CGT,
    TK_CGE,
    TK_LPAREN,
    TK_RPAREN,
    TK_LBRACK,
    TK_RBRACK,
    TK_LSQBRK,
    TK_RSQBRK,
    TK_SQUOT,
    TK_DOT,
    TK_COMMA,
    TK_COLON,
    TK_SCOLON,
    TK_DDOT,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_NEWLINE,
    TK_MOD,
    TK_POW,
    TK_AND,
    TK_OR,
    TK_XOR
};

// Define token struct
struct Token {
    token_type type;
    string value;
};

// Function to create a new Token struct
Token newToken(token_type type, string value) {
    Token t;
    t.type = type;
    t.value = value;
    return t;
}

 //FlexLexer *lexer = new yyFlexLexer;

%}

%%

[ \t]                    ;
[\n]                    cout << endl;
"var"                   cout << "TK_VAR" << " "; 
"type"                  cout << "TK_TYPE" << " ";
"integer"               cout << "TK_INT" << " "; 
"real"                  cout << "TK_DOUBLE" << " ";
"boolean"               cout << "TK_BOOL" << " ";
"char"                  cout << "TK_CHAR" << " ";
"array"                 cout << "TK_ARRAY" << " ";
"record"                cout << "TK_RECORD" << " ";
"routine"               cout << "TK_ROUTINE" << " ";
"return"                cout << "TK_RETURN" << " ";
"is"                    cout << "TK_IS" << " ";
"not"                   cout << "TK_NOT" << " "; 
"while"                 cout << "TK_WHILE" << " ";
"loop"		        	cout << "TK_LOOP" << " ";
"end"                   cout << "TK_END" << " "; 
"for"                   cout << "TK_FOR" << " "; 
"foreach"               cout << "TK_FOREACH" << " ";
"from"                  cout << "TK_FROM" << " ";
"if"                    cout << "TK_IF" << " ";
"then"                  cout << "TK_THEN" << " ";
"else"                  cout << "TK_ELSE" << " ";
"in"                    cout << "TK_IN" << " ";
"reverse"               cout << "TK_REVERSE" << " ";
"true"                  cout << "TK_TRUE" << " ";
"false"                 cout << "TK_FALSE" << " ";
"print"                 cout << "TK_PRINT" << " ";
":="                    cout << "TK_ASSIGN" << " ";
"="                     cout << "TK_CEQ" << " "; 
"/="                    cout << "TK_CNEQ" << " ";
"<"                     cout << "TK_CLT" << " "; 
"<="                    cout << "TK_CLE" << " "; 
">"                     cout << "TK_CGT" << " "; 
">="                    cout << "TK_CGE" << " "; 
"("                     cout << "TK_LPAREN" << " ";
")"                     cout << "TK_RPAREN" << " ";
"["                     cout << "TK_LSQBRK" << " ";
"]"                     cout << "TK_RSQBRK" << " ";
"{"                     cout << "TK_LBRACK" << " ";
"}"                     cout << "TK_RBRACK" << " ";
"."                     cout << "TK_DOT" << " "; 
","                     cout << "TK_COMMA" << " ";
":"                     cout << "TK_COLON" << " ";
";"                     cout << "TK_SCOLON" << " ";
".."                    cout << "TK_DDOT" << " ";
"+"                     cout << "TK_PLUS" << " ";
"-"                     cout << "TK_MINUS" << " ";
"*"                     cout << "TK_MUL" << " "; 
"/"                     cout << "TK_DIV" << " ";
"\\n"                   cout << "TK_NEWLINE" << " "; 
"%"                     cout << "TK_MOD" << " "; 
"**"                    cout << "TK_POW" << " "; 
"and"                   cout << "TK_AND" << " "; 
"or"                    cout << "TK_OR" << " "; 
"xor"                   cout << "TK_XOR" << " ";
\"([^\\\"]|\\.)*\"      cout << "TK_STRING_LITERAL" << " "; /*TODO: new_token*/
\'([^\\\"]|\\.)*\'      cout << "TK_STRING_LITERAL" << " "; /*TODO: new_token*/
\/\/(.*)                cout << "TK_COMMENT" << " "; /*TODO: will ignore and do nothing*/
[a-zA-Z_][a-zA-Z0-9_]*  cout << "TK_IDENTIFIER" << " ";   /*TODO: new_token*/
[0-9]+\.[0-9]+          cout << "TK_DOUBLE_LITERAL" << " "; 
[0-9]+                  {cout << "TK_INTEGER_LITERAL" << " "; return carbonic_c::Parser::make_TK_INTEGER_LITERAL(atoi(yytext));} 
'.'                     cout << "TK_CHARACTER_LITERAL" << " "; 
.                       cout << "LEXICAL ERROR: INVALID PARAMETER FOR THE LEXEME -> " << yytext << " ";
%%


