%require "3.0"
%define api.namespace { carbonic_c }
%code requires
{
    #include <iostream>
    using namespace std;
    namespace carbonic_c {
        class Lexer;
    }
}