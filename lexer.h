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
enum class TokenType
{
    /*=========== Declarations Tokens ==============*/
    /*Variables*/
    TK_VAR,
    TK_IS,

    /*Type*/
    TK_TYPE,
    TK_INTEGER,
    TK_REAL,
    TK_BOOLEAN,
    TK_ARRAY,
    TK_RECORD,

    /*Routine*/
    TK_ROUTINE,

    /*============= Statement Tokens ===============*/
    /*Loops*/
    TK_WHILE,
    TK_LOOP,
    TK_END,
    TK_FOR,
    TK_FROM,
    TK_IN,
    TK_REVERSE,
    TK_DDOT,

    /*If*/
    TK_IF,
    TK_THEN,
    TK_ELSE,

    /*Return*/
    TK_RETURN,

    /*Assignment*/
    TK_ASSIGN,

    /*Print*/
    TK_PRINT,

    /*============ Expressions Tokens ==============*/
    /*Literals*/
    TK_REAL_LITERAL,
    TK_INTEGER_LITERAL,
    TK_BOOLEAN_LITERAL,

    /*Comparisons*/
    TK_EQ,
    TK_NEQ,
    TK_LT,
    TK_LEQ,
    TK_GT,
    TK_GEQ,

    /*Operators*/
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_MOD,
    TK_AND,
    TK_OR,
    TK_XOR,

    /*Variables*/
    TK_IDENTIFIER,
    TK_DOT,

    /*=============== Utility Tokens ===============*/
    TK_LPAREN,
    TK_RPAREN,
    TK_LSQUARE,
    TK_RSQUARE,
    TK_COMMA,
    TK_COLON,
    TK_SEMICOLON
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