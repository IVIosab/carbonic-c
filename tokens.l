%{
#include <string>
#include "node.h" // TODO: add when we start using bison
#include "parser.hpp" //TODO: add for bison
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
extern "C" int yywrap() { }
// verify that we want to parse characters as [',CHAR,'] not as ['CHAR']
%}

%%

[ \t\n]                 ;
"var"                   return TOKEN(TK_VAR);
"type"                  return TOKEN(TK_TYPE);
"integer"               return TOKEN(TK_INT);
"real"                  return TOKEN(TK_DOUBLE);
"boolean"               return TOKEN(TK_BOOL);
"char"                  return TOKEN(TK_CHAR);
"array"                 return TOKEN(TK_ARRAY);
"record"                return TOKEN(TK_RECORD);
"routine"               return TOKEN(TK_ROUTINE);
"is"                    return TOKEN(TK_IS);
"not"                   return TOKEN(TK_NOT); // TODO: "print" return TOKEN(TK_PRINT);
"while"                 return TOKEN(TK_WHILE);
"loop"                  return TOKEN(TK_LOOP);
"end"                   return TOKEN(TK_END);
"for"                   return TOKEN(TK_FOR); // TODO: "for each" return TOKEN(FOR_EACH);
"if"                    return TOKEN(TK_IF);
"then"                  return TOKEN(TK_THEN);
"else"                  return TOKEN(TK_ELSE);
"in"                    return TOKEN(TK_IN);
"reverse"               return TOKEN(TK_REVERSE);

[a-zA-Z_][a-zA-Z0-9_]*  SAVE_TOKEN; return (TK_IDENTIFIER);
[0-9]+.[0-9]*           SAVE_TOKEN; return (TK_DOUBLE);
[0-9]+                  SAVE_TOKEN; return (TK_INTEGER);
[ -~]                   SAVE_TOKEN; return(TK_CHARACTER);
"true"                  return TOKEN(TK_TRUE);
"false"                 return TOKEN(TK_FALSE);
":="                    return TOKEN(TK_ASSIGN);
"="                     return TOKEN(TK_CEQ);
"/="                    return TOKEN(TK_CNEQ);
"<"                     return TOKEN(TK_CLT);
"<="                    return TOKEN(TK_CLE);
">"                     return TOKEN(TK_CGT);
">="                    return TOKEN(TK_CGE);
"("                     return TOKEN(TK_LPAREN);
")"                     return TOKEN(TK_RPAREN);
"{"                     return TOKEN(TK_LBRACK);
"}"                     return TOKEN(TK_RBRACK);
"["                     return TOKEN(TK_LSQBRK);
"]"                     return TOKEN(TK_RSQBRK);
"'"                     return TOKEN(TK_SQUOT);
"."                     return TOKEN(TK_DOT);
","                     return TOKEN(TK_COMMA);
":"                     return TOKEN(TK_COLON);
";"                     return TOKEN(TK_SCOLON);
".."                    return TOKEN(TK_DDOT);
"+"                     return TOKEN(TK_PLUS);
"-"                     return TOKEN(TK_MINUS);
"*"                     return TOKEN(TK_MUL);
"/"                     return TOKEN(TK_DIV);
"%"                     return TOKEN(TK_MOD);
"^"                     return TOKEN(TK_POW);
"and" return TOKEN(TK_AND);
"or" return TOKEN(TK_OR);
"XOR" return(TK_XOR);
.                       printf("Unknown token!n"); yyterminate();

%%