#include "lexer.h"
#include "driver.hpp"
#include "parser.hpp"

extern ast::node_ptr<ast::Program> program;

void parseVariableDeclarationList(std::vector<ast::node_ptr<ast::VariableDeclaration>> v)
{
}

void parseTypeDeclarationsMap(std::map<std::string, ast::node_ptr<ast::Type>> m)
{
}

void parseRoutineDeclaration(ast::node_ptr<ast::RoutineDeclaration> r)
{
}

void parseVariableDeclaration(ast::node_ptr<ast::VariableDeclaration> v)
{
}

void parseRoutineDeclaration(ast::node_ptr<ast::RoutineDeclaration> r)
{
}

void parseType(ast::node_ptr<ast::Type> t)
{
}

void parseExpression(ast::node_ptr<ast::Expression> e)
{
}

void parseBody(ast::node_ptr<ast::Body> b)
{
}

void parseStatements(std::vector<ast::node_ptr<ast::Statement>> s)
{
}

int main(int argc, char **argv)
{
    carbonic_c::Driver driver;
    // while (lexer->yylex() != 0)
    // driver.parse_args(argc, argv);
    int x = driver.parse_program();

    std::cout << program->routines[0]->name << std::endl;
    std::cout << program->routines[0]->params.size() << std::endl;
    std::cout << program->routines[0]->body->variables.size() << std::endl;
    std::cout << program->routines[0]->body->variables[0]->name << std::endl;
    std::cout << program->routines[0]->body->variables[0]->initial_value << std::endl;

    return 0;
}