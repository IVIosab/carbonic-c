%{
#include <string>
#include <iostream>
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
    TK_IF,
    TK_THEN,
    TK_ELSE,
    TK_IN,
    TK_REVERSE,
    TK_IDENTIFIER,
    TK_DOUBLE_LITERAL,
    TK_INTEGER_LITERAL,
    TK_CHARACTER_LITERAL,
    TK_TRUE,
    TK_FALSE,
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

 FlexLexer *lexer = new yyFlexLexer;

%}
%option noyywrap c++

%%

[ \t\n]                 ;
"var"                   cout << "TK_VAR" << endl; 
"type"                  cout << "TK_TYPE" << endl;
"integer"               cout << "TK_INT" << endl; 
"real"                  cout << "TK_DOUBLE" << endl;
"boolean"               cout << "TK_BOOL" << endl;
"char"                  cout << "TK_CHAR" << endl;
"array"                 cout << "TK_ARRAY" << endl;
"record"                cout << "TK_RECORD" << endl;
"routine"               cout << "TK_ROUTINE" << endl;
"return"                cout << "TK_RETURN" << endl;
"is"                    cout << "TK_IS" << endl;
"not"                   cout << "TK_NOT" << endl; 
"while"                 cout << "TK_WHILE" << endl;
"loop"		        	cout << "TK_LOOP" << endl;
"end"                   cout << "TK_END" << endl; 
"for"                   cout << "TK_FOR" << endl; 
"if"                    cout << "TK_IF" << endl;
"then"                  cout << "TK_THEN" << endl;
"else"                  cout << "TK_ELSE" << endl;
"in"                    cout << "TK_IN" << endl;
"reverse"               cout << "TK_REVERSE" << endl;
"true"                  cout << "TK_TRUE" << endl;
"false"                 cout << "TK_FALSE" << endl;
":="                    cout << "TK_ASSIGN" << endl;
"/="                    cout << "TK_CNEQ" << endl;
"<="                    cout << "TK_CLE" << endl; 
">="                    cout << "TK_CGE" << endl; 
"="                     cout << "TK_CEQ" << endl; 
"<"                     cout << "TK_CLT" << endl; 
">"                     cout << "TK_CGT" << endl; 
"("                     cout << "TK_LPAREN" << endl;
")"                     cout << "TK_RPAREN" << endl;
"["                     cout << "TK_LSQBRK" << endl;
"]"                     cout << "TK_RSQBRK" << endl;
"'"                     cout << "TK_SQUOT" << endl;
".."                    cout << "TK_DDOT" << endl;
"."                     cout << "TK_DOT" << endl; 
","                     cout << "TK_COMMA" << endl;
":"                     cout << "TK_COLON" << endl;
";"                     cout << "TK_SCOLON" << endl;
"+"                     cout << "TK_PLUS" << endl;
"-"                     cout << "TK_MINUS" << endl;
"*"                     cout << "TK_MUL" << endl; 
"/"                     cout << "TK_DIV" << endl; 
"%"                     cout << "TK_MOD" << endl; 
"**"                    cout << "TK_POW" << endl; 
"and"                   cout << "TK_AND" << endl; 
"or"                    cout << "TK_OR" << endl; 
"xor"                   cout << "TK_XOR" << endl;
[a-zA-Z_][a-zA-Z0-9_]*  cout << "TK_IDENTIFIER" << endl; 
[0-9]+\.[0-9]+          cout << "TK_DOUBLE_LITERAL" << endl; 
[0-9]+                  cout << "TK_INTEGER_LITERAL" << endl; 
'.'                     cout << "TK_CHARACTER_LITERAL" << endl; 
.                       cout << "Unknown lexeme: " << yytext << endl;
%%

int main(int argc, char** argv) {
    while (lexer->yylex() != 0)
    return 0;
}

