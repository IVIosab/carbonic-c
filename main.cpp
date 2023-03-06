#include "lexer.h"
#include "driver.hpp"
#include "parser.hpp"

int main(int argc, char **argv)
{
    carbonic_c::Driver driver;
    // while (lexer->yylex() != 0)
    // driver.parse_args(argc, argv);
    int x = driver.parse_program();

    return 0;
}