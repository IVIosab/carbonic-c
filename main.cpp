#include "lexer.h"
#include "driver.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "astPrinter.hpp"
#include "semantic.hpp"
#include "llvm/IR/IRBuilder.h"
extern ast::Program *program;

namespace generator
{
    class CodeGenerator : public ast::Visitor
    {
    public:
        llvm::IRBuilder<> m_builder;
    };
}

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
    return 0;
}