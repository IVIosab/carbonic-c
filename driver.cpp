#include "driver.hpp"

extern ast::node_ptr<ast::Program> program;
namespace carbonic_c
{

    Driver::Driver() : lexer(*this), parser(lexer, *this) {}

    int Driver::parse_program()
    {
        return parser.parse();
    }

    void Driver::readFrom(std::istream *is)
    {
        lexer.switch_streams(is, nullptr);
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