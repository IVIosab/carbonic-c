#include "driver.hpp"

namespace carbonic_c //wtf is this ?
{

    Driver::Driver() : lexer(*this), parser(lexer, *this) {} //wtf is this ?

    int Driver::parse_program() //wtf is this ?
    {
        return parser.parse(); //wtf is this ?
    }

    void Driver::readFrom(std::istream *is) //wtf is this ?
    {
        lexer.switch_streams(is, nullptr); //wtf is this ?
    }

    // int Driver::parse_args(int argc, char **argv)
    // {
    //     for (int i = 1; i < argc; i++)
    //     {
    //         std::string arg = argv[i];
    //         if (arg == "--help" || arg == "-h")
    //         {
    //             show_help();
    //         }
    //         else if (arg == "-d" || arg == "--debug")
    //         {
    //             debug = true;
    //         }
    //         else if (arg == "-o" || arg == "--outfile")
    //         {
    //             outfile = argv[++i];
    //             continue;
    //         }
    //         else
    //         {
    //             infile.open(arg);
    //             if (!infile.good())
    //             {
    //                 std::cout << "Error: no such file: " << arg << '\n';
    //                 return 1;
    //             }
    //             readFrom(&infile);
    //         }
    //     }
    //     if (!infile.is_open())
    //     {
    //         show_help();
    //     }
    //     return 0;
    // }

}