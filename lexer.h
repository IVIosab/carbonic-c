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