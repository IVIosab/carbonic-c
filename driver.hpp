#ifndef DRIVER_H
#define DRIVER_H

#include <fstream>

#include "lexer.h"
#include "parser.hpp"

namespace carbonic_c //wtf is this ?
{
    class Driver //wtf is this ?
    {
    public: //wtf is this ?
        Driver(); //wtf is this ?

        friend class Parser; //wtf is this ?
        friend class Lexer; //wtf is this ?

        bool debug = false; //wtf is this ?
        std::ifstream infile; //wtf is this ?
        std::string outfile = "output.out"; //wtf is this ?

        int parse_program(); //wtf is this ?
        int parse_args(int argc, char **argv); //wtf is this ?
        int print_ast(); //wtf is this ?
        void readFrom(std::istream *is); //wtf is this ?
        void show_help(); //wtf is this ?

    private: //wtf is this ?
        Lexer lexer; //wtf is this ?
        Parser parser; //wtf is this ?
    };
} // namespace carbonic_c

#endif // DRIVER_H