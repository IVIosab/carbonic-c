#ifndef LEXER_H //wtf is this ?
#define LEXER_H //wtf is this ?

#ifndef yyFlexLexerOnce //wtf is this ?
// #undef yyFlexLexer
// #define yyFlexLexer carbonic_c_FlexLexer
#include <FlexLexer.h> //wtf is this ?
#endif

// Replaces `int yylex()` with `symbol_type get_next_token()` to be compatible with bison 3
#undef YY_DECL 
#define YY_DECL carbonic_c::Parser::symbol_type carbonic_c::Lexer::get_next_token() 

#include "parser.hpp" //wtf is this ?
#include <fstream> //wtf is this ?

namespace carbonic_c //wtf is this ?
{ 
    class Driver; //wtf is this ?

    class Lexer : public yyFlexLexer //wtf is this ?
    {
    public: //wtf is this ?
        Lexer(Driver &driver) : driver(driver) {} //wtf is this ?
        virtual ~Lexer() {} //wtf is this ?
        virtual carbonic_c::Parser::symbol_type get_next_token(); //wtf is this ?

    private:
        Driver &driver; //wtf is this ?
    };

}

#endif // LEXER_H