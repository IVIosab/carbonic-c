#ifndef DRIVER_H
#define DRIVER_H

#include "lexer.h"
#include "parser.hpp"

namespace carbonic_c
{
    class Driver
    {
    public:
        Driver();

        std::ifstream infile;
        std::string outfile = "output.out";

        int parse_program();
        void readFrom(std::istream *is);

    private:
        Lexer lexer;
        Parser parser;
    };
}

#endif