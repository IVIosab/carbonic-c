#include "driver.hpp"

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
}