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

        bool debug = false;
        std::ifstream infile;
        std::string outfile = "output.out";

        int parse_program();
        // The below section is commented for future usage while building the AST
        // int parse_args(int argc, char **argv);
        // int print_ast();
        void readFrom(std::istream *is);
        // void show_help();

    private:
        Lexer lexer;
        Parser parser;
    };
}

#endif