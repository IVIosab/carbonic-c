#include "astPrinter.hpp"
#include "lexer.h"
using namespace ast;
using namespace std;

std::map<BinaryOperatorEnum, std::string> bin_to_str{
    {BinaryOperatorEnum::PLUS, "+"},
    {BinaryOperatorEnum::MINUS, "-"},
    {BinaryOperatorEnum::MUL, "*"},
    {BinaryOperatorEnum::POW, "**"},
    {BinaryOperatorEnum::DIV, "/"},
    {BinaryOperatorEnum::MOD, "%"}};

std::map<BitwiseOperatorEnum, std::string> bit_to_str{
    {BitwiseOperatorEnum::OR, "or"},
    {BitwiseOperatorEnum::XOR, "xor"},
    {BitwiseOperatorEnum::AND, "and"}};

std::map<ComparisonOperatorEnum, std::string> cmp_to_str{
    {ComparisonOperatorEnum::CEQ, "="},
    {ComparisonOperatorEnum::CNEQ, "/="},
    {ComparisonOperatorEnum::CLT, "<"},
    {ComparisonOperatorEnum::CLE, "<="},
    {ComparisonOperatorEnum::CGT, ">"},
    {ComparisonOperatorEnum::CGE, ">="}};

namespace analyzer
{

    void AstPrinter::indent()
    {
        for (int i = 0; i < depth; i++)
        {
            cout << "|";
        }
        cout << "- ";
    }
    void AstPrinter::visit(ast::Program *node)
    {
        depth++;
        indent();
        cout << "Program" << endl;
        for (auto type : node->types)
        {
            type.second->accept(this);
        }

        for (auto variableDecl : node->variables)
        {
            variableDecl->accept(this);
        }

        for (auto routine : node->routines)
        {
            routine->accept(this);
        }
        depth--;
    };
    void AstPrinter::visit(ast::IntType *node)
    {
        depth++;
        indent();
        cout << "IntType" << endl;
        depth--;
    };
    void AstPrinter::visit(ast::DoubleType *node)
    {
        depth++;
        indent();
        cout << "DoubleType" << endl;
        depth--;
    };
    void AstPrinter::visit(ast::BoolType *node)
    {
        depth++;
        indent();
        cout << "BoolType" << endl;
        depth--;
    };
    void AstPrinter::visit(ast::ArrayType *node)
    {
        depth++;
        indent();
        cout << "ArrayType" << endl;

        if (node->size != nullptr)
        {
            node->size->accept(this);
        }

        node->dtype->accept(this);

        depth--;
    };
    void AstPrinter::visit(ast::RecordType *node)
    {
        depth++;
        indent();
        cout << "RecordType" << endl;
        for (auto field : node->fields)
        {
            field->accept(this);
        }
        depth--;
    };
    void AstPrinter::visit(ast::IntLiteral *node)
    {
        depth++;
        indent();
        cout << "IntLiteral (" << node->value << ")" << endl;
        depth--;
    };
    void AstPrinter::visit(ast::DoubleLiteral *node)
    {
        depth++;
        indent();
        cout << "DoubleLiteral (" << node->value << ")" << endl;
        depth--;
    };
    void AstPrinter::visit(ast::BoolLiteral *node)
    {
        depth++;
        indent();
        cout << "BoolLiteral (" << node->value << ")" << endl;
        depth--;
    };
    void AstPrinter::visit(ast::BinaryExpression *node)
    {
        depth++;
        indent();
        cout << "BinaryExpression (" << bin_to_str[node->op] << ")" << endl;
        node->lhs->accept(this);
        node->rhs->accept(this);
        depth--;
    };
    void AstPrinter::visit(ast::BitwiseExpression *node)
    {
        depth++;
        indent();
        cout << "BitwiseExpression (" << bit_to_str[node->op] << ")" << endl;
        node->lhs->accept(this);
        node->rhs->accept(this);
        depth--;
    };
    void AstPrinter::visit(ast::ComparisonExpression *node)
    {
        depth++;
        indent();
        cout << "ComparisonExpression (" << cmp_to_str[node->op] << ")" << endl;
        node->lhs->accept(this);
        node->rhs->accept(this);
        depth--;
    };
    void AstPrinter::visit(ast::VariableDeclaration *node)
    {
        depth++;
        indent();
        cout << "VariableDeclaration (" << node->name << ")" << endl;
        if (node->dtype != nullptr)
        {
            node->dtype->accept(this);
        }

        if (node->initial_value != nullptr)
        {
            node->initial_value->accept(this);
        }
        depth--;
    };
    void AstPrinter::visit(ast::TypeDeclaration *node)
    {
        depth++;
        indent();
        cout << "TypeDeclaration (" << node->name << ")" << endl;
        if (node->dtype != nullptr)
        {
            node->dtype->accept(this);
        }
        depth--;
    };
    void AstPrinter::visit(ast::RoutineDeclaration *node)
    {
        depth++;
        indent();
        cout << "RoutineDeclaration (" << node->name << ")" << endl;
        for (auto parameter : node->params)
        {
            parameter->accept(this);
        }

        node->body->accept(this);

        if (node->rtype != nullptr)
        {
            node->rtype->accept(this);
        }

        depth--;
    };
    void AstPrinter::visit(ast::RoutineCall *node)
    {
        depth++;
        indent();
        cout << "VariableDeclaration (" << node->name << ")" << endl;
        for (size_t i = 0; i < node->args.size(); i++)
        {
            node->args[i]->accept(this);
        }
        depth--;
    };
    void AstPrinter::visit(ast::Body *node)
    {
        depth++;
        indent();
        cout << "Body" << endl;
        for (auto variableDecl : node->variables)
        {
            variableDecl->accept(this);
        }

        for (auto statement : node->statements)
        {
            statement->accept(this);
        }
        depth--;
    };
    void AstPrinter::visit(ast::Assignment *node)
    {
        depth++;
        indent();
        cout << "Assignment" << endl;
        node->modifiablePrimary->accept(this);
        node->expression->accept(this);
        depth--;
    };
    void AstPrinter::visit(ast::Print *node)
    {
        depth++;
        indent();
        cout << "Print" << endl;
        if (node->exp)
        {
            node->exp->accept(this);
        }
        depth--;
    };
    void AstPrinter::visit(ast::Return *node)
    {
        depth++;
        indent();
        cout << "Return" << endl;
        if (node->exp)
        {
            node->exp->accept(this);
        }
        depth--;
    };
    void AstPrinter::visit(ast::Identifier *node)
    {
        depth++;
        // name
        node->idx->accept(this);
        if (node->idx)
        {
            node->idx->accept(this);
        }
        depth--;
    };
    void AstPrinter::visit(ast::ModifiablePrimary *node)
    {
        depth++;
        indent();
        cout << "ModifiablePrimary" << endl;
        node->identifier->accept(this);
        depth--;
    };
    void AstPrinter::visit(ast::IfStatement *node)
    {
        depth++;
        indent();
        cout << "IfStatement" << endl;
        node->condition->accept(this); // TODO
        node->ifBody->accept(this);
        if (node->elseBody)
        {
            node->elseBody->accept(this);
        }
        depth--;
    };
    void AstPrinter::visit(ast::WhileLoop *node)
    {
        depth++;
        indent();
        cout << "WhileLoop" << endl;
        node->condition->accept(this);
        node->loopBody->accept(this);
        depth--;
    };
    void AstPrinter::visit(ast::ForLoop *node)
    {
        depth++;
        indent();
        cout << "ForLoop" << endl;
        node->identifier->accept(this);
        node->condition->accept(this);
        node->action->accept(this);
        node->loopBody->accept(this);

        depth--;
    };
    void AstPrinter::visit(ast::ForeachLoop *node)
    {
        depth++;
        indent();
        cout << "ForeachLoop" << endl;
        node->identifier->accept(this);
        node->loopBody->accept(this);
        node->modifiablePrimary->accept(this);
        depth--;
    };

}