#include "llvm/Support/raw_ostream.h"
#include "lexer.h"
#include "driver.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "astPrinter.hpp"
#include "semantic.hpp"
#include "codeGenerator.hpp"
extern ast::Program *program;

int main(int argc, char **argv)
{
    carbonic_c::Driver driver;
    // while (lexer->yylex() != 0)
    // driver.parse_args(argc, argv);
    int x = driver.parse_program();

    std::cout << std::endl;
    // analyzer::AstPrinter printer;
    // program->accept(&printer);
    analyzer::Semantic analyzer;
    program->accept(&analyzer);
    // prettyPrinter::codePrinter printer;
    // program->accept(&printer);

    generator::codeGenerator code_generator;
    program->accept(&code_generator);
    return 0;
}