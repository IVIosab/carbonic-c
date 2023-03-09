#ifndef LEXER_H
#define LEXER_H

#ifndef yyFlexLexerOnce
// #undef yyFlexLexer
// #define yyFlexLexer carbonic_c_FlexLexer
#include <FlexLexer.h>
#endif

// Replaces `int yylex()` with `symbol_type get_next_token()` to be compatible with bison 3
#undef YY_DECL
#define YY_DECL carbonic_c::Parser::symbol_type carbonic_c::Lexer::get_next_token()

#include "parser.hpp"
#include <fstream>

// Define token types
enum token_type {
    TK_VAR,
    TK_TYPE,
    TK_INT,
    TK_DOUBLE,
    TK_BOOL,
    TK_CHAR,
    TK_STRING,
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

namespace carbonic_c
{
    class Driver;

    class Lexer : public yyFlexLexer
    {
    public:
        Lexer(Driver &driver) : driver(driver) {}
        virtual ~Lexer() {}
        virtual carbonic_c::Parser::symbol_type get_next_token();

    private:
        Driver &driver;
    };

}

#endif // LEXER_H