%require "3.0"
%{
#include <iostream>
#include <string>
#include "lexer.h"
using namespace std;
%}
%union{
    //define the AST here later on
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

/*
TODO: uncomment when you define types later on
%type <string> TK_IDENTIFIER TK_STRING_LITERAL
%type <long long int> TK_INTEGER_LITERAL
%type <double> TK_DOUBLE_LITERAL
*/
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

