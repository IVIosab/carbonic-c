%require "3.0"
%{
#include <iostream>
#include <string>
#include "lexer.h"
#include <identifierTable.h>
using namespace std;
%}
%union{
    //define the AST here later on
    string stringLtrl;
    double doubleLtrl;
    long long int intLtrl;
}
%token TK_VAR TK_TYPE
%token TK_INT TK_DOUBLE TK_BOOL TK_CHAR TK_ARRAY TK_RECORD TK_ROUTINE TK_IS TK_NOT
%token TK_WHILE TK_LOOP TK_END TK_FOR TK_FOREACH TK_FROM TK_RETURN
%token TK_IF TK_THEN TK_ELSE TK_IN TK_REVERSE 
%token TK_IDENTIFIER TK_ASSIGN
%token TK_STRING_LITERAL TK_DOUBLE_LITERAL,TK_INTEGER_LITERAL TK_CHARACTER_LITERAL TK_TRUE TK_FALSE
%token TK_PRINT
%token TK_CEQ TK_CNEQ TK_CLT TK_CLE TK_CGT TK_CGE
%token TK_LPAREN TK_RPAREN TK_LBRACK TK_RBRACK TK_LSQBRK TK_RSQBRK 
%token TK_SQUOT TK_DOT TK_COMMA TK_COLON TK_SCOLON TK_DDOT
%token TK_PLUS TK_MINUS TK_MUL TK_DIV
%token TK_NEWLINE
%token TK_MOD TK_POW TK_AND TK_OR TK_XOR
%right '='
%left '+' '-'
%left '*' '/'
%right '^'

%type <stringLtrl> TK_IDENTIFIER TK_STRING_LITERAL
%type <intLtrl> TK_INTEGER_LITERAL
%type <doubleLtrl> TK_DOUBLE_LITERAL

%define api.namespace { carbonic_c }
%code requires
{
    #include <iostream>
    using namespace std;
    namespace carbonic_c {
        class Lexer;
    }

}

%start program
%%
//TODO: Grammar rules and actions
PROGRAM:
    SIMPLE_DECLERATION |
    ROUTINE_DECLERATION ;
SIMPLE_DECLERATION: 
    VARIABLE_DECLERATION |
    TYPE_DECLERATION;
ROUTINE_DECLERATION:
    TK_ROUTINE TK_IDENTIFIER TK_LPAREN PARAMETERS TK_RPAREN TK_COLON TYPE TK_IS BODY TK_END |
    TK_ROUTINE TK_IDENTIFIER TK_LPAREN PARAMETERS TK_RPAREN TK_IS BODY TK_END ;
PARAMETER: 
    TK_IDENTIFIER TK_IS TYPE {}|
    TK_IDENTIFIER TK_IS TYPE TK_COMMA PARAMETER{};
PARAMETERS:
    %empty |
    PARAMETER;
 BODY:
    VARIABLE_DECLERATION |
    STATEMENT;
STATEMENT:
    ;
VARIABLE_DECLERATION: 
    TK_VAR TK_IDENTIFIER TK_IS EXPRESSION {}|
    TK_VAR TK_IDENTIFIER TK_COLON TYPE TK_IS EXPRESSION {}; 
TYPE_DECLERATION: 
    TK_TYPE TK_IDENTIFIER TK_IS PRIMITIVE_TYPE TK_SCOLON {}; // primitive type or also previously defined types too?
TYPE:
    PRIMITIVE_TYPE | 
    ARRAY_TYPE |
    RECORD_TYPE |
    TK_IDENTIFIER {};
PRIMITIVE_TYPE:
    TK_INT {} |
    TK_DOUBLE {} |
    TK_BOOL {} |
    TK_CHAR {};
ARRAY_TYPE:
    TK_ARRAY TK_LSQBRK EXPRESSION TK_RSQBRK PRIMITIVE_TYPE {};
RECORD_TYPE:
    TK_RECORD TK_LBRACK VARIABLE_DECLERATIONS TK_RBRACK {};
EXPRESSION: {};
VARIABLE_DECLERATIONS: {};


%%
// C++ functions

// Called by yyparser when errors are faced.
void yyerror(string error_message){
    cout << "PARSING ERROR: " << error_message << '\n';
}
void yyparse(){
    //TODO: implement, it will get next token and be the driver to this parser.
}
int main(int argc, char* argv[]){
    extern FILE *yyin;
    argv++;
    argc--;
    yyin = fopen(argv[0], ”r” );
    yydebug = 1;
    errors = 0;
    yyparse ();
    return 0;
}

