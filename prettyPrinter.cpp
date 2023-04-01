#include "prettyPrinter.hpp"
#include "lexer.h"
using namespace ast;
using namespace std;

std::map<BinaryOperatorEnum, std::string> binary_to_str{
    {BinaryOperatorEnum::PLUS, "+"},
    {BinaryOperatorEnum::MINUS, "-"},
    {BinaryOperatorEnum::MUL, "*"},
    {BinaryOperatorEnum::POW, "**"},
    {BinaryOperatorEnum::DIV, "/"},
    {BinaryOperatorEnum::MOD, "%"}};

std::map<BitwiseOperatorEnum, std::string> logic_to_str{
    {BitwiseOperatorEnum::OR, "or"},
    {BitwiseOperatorEnum::XOR, "xor"},
    {BitwiseOperatorEnum::AND, "and"}};

std::map<ComparisonOperatorEnum, std::string> comparison_to_str{
    {ComparisonOperatorEnum::CEQ, "="},
    {ComparisonOperatorEnum::CNEQ, "/="},
    {ComparisonOperatorEnum::CLT, "<"},
    {ComparisonOperatorEnum::CLE, "<="},
    {ComparisonOperatorEnum::CGT, ">"},
    {ComparisonOperatorEnum::CGE, ">="}};

namespace prettyPrinter
{

    void codePrinter::indent()
    {
        for (int i = 0; i < depth; i++)
        {
            cout << "\t";
        }
    }
    void codePrinter::visit(ast::Program *node) // Done
    {
        for (auto type : node->types)
        {
            type->accept(this);
        }

        for (auto routine : node->routines)
        {
            routine->accept(this);
        }
    };
    void codePrinter::visit(ast::IntType *node) // Done
    {
        cout << "integer";
    };
    void codePrinter::visit(ast::DoubleType *node) // Done
    {
        cout << "real";
    };
    void codePrinter::visit(ast::BoolType *node) // Done
    {
        cout << "boolean";
    };
    void codePrinter::visit(ast::ArrayType *node) // TODO
    {
        node->size->accept(this);
        node->dtype->accept(this);
    };
    void codePrinter::visit(ast::RecordType *node) // TODO
    {
        for (auto field : node->fields)
        {
            field->accept(this);
        }
    };
    void codePrinter::visit(ast::IntLiteral *node) // Done
    {
        cout << node->value;
    };
    void codePrinter::visit(ast::DoubleLiteral *node) // Done
    {
        cout << node->value;
    };
    void codePrinter::visit(ast::BoolLiteral *node) // Done
    {
        if (node->value)
            cout << "true";
        else
            cout << "false";
    };
    void codePrinter::visit(ast::BinaryExpression *node) // Done
    {
        node->lhs->accept(this);
        cout << " " << binary_to_str[node->op] << " ";
        node->rhs->accept(this);
    };
    void codePrinter::visit(ast::BitwiseExpression *node) // Done
    {
        node->lhs->accept(this);
        cout << " " << logic_to_str[node->op] << " ";
        node->rhs->accept(this);
    };
    void codePrinter::visit(ast::ComparisonExpression *node) // Done
    {
        node->lhs->accept(this);
        cout << " " << comparison_to_str[node->op] << " ";
        node->rhs->accept(this);
    };
    void codePrinter::visit(ast::VariableDeclaration *node) // Done
    {
        indent();
        cout << "var " << node->name;
        if (node->dtype != nullptr)
        {
            cout << " : ";
            node->dtype->accept(this);
        }
        if (node->initial_value != nullptr)
        {
            cout << " is ";
            node->initial_value->accept(this);
        }
        cout << ";" << endl;
    };
    void codePrinter::visit(ast::TypeDeclaration *node) // Done
    {
        indent();
        cout << "type " << node->name << " is";
        if (node->dtype != nullptr)
        {
            node->dtype->accept(this);
        }
        cout << endl;
    };
    void codePrinter::visit(ast::RoutineDeclaration *node) // Done
    {
        indent();
        cout << "routine " << node->name << " (";
        for (auto parameter : node->params)
        {
            parameter->accept(this);
        }
        cout << ")";
        if (node->rtype != nullptr)
        {
            cout << " : ";
            node->rtype->accept(this);
        }
        cout << " is" << endl;
        depth++;
        node->body->accept(this);
        depth--;
        indent();
        cout << "end" << endl;
    };
    void codePrinter::visit(ast::RoutineCall *node) // Done
    {
        indent();
        cout << node->name << "(";
        for (size_t i = 0; i < node->args.size(); i++)
        {
            node->args[i]->accept(this);
            if (i != node->args.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << ");" << endl;
    };
    void codePrinter::visit(ast::Body *node) // Done
    {
        for (auto variableDecl : node->variables)
        {
            variableDecl->accept(this);
        }

        for (auto statement : node->statements)
        {
            statement->accept(this);
        }
    };
    void codePrinter::visit(ast::Assignment *node) // Done
    {
        indent();
        node->modifiablePrimary->accept(this);
        cout << " := ";
        node->expression->accept(this);
        cout << ";" << endl;
    };
    void codePrinter::visit(ast::Print *node) // Done
    {
        indent();
        cout << "print(";
        if (node->exp)
        {
            node->exp->accept(this);
        }
        cout << ");" << endl;
    };
    void codePrinter::visit(ast::Return *node) // Done
    {
        indent();
        cout << "return ";
        if (node->exp)
        {
            node->exp->accept(this);
        }
        cout << ";" << endl;
    };
    void codePrinter::visit(ast::ModifiablePrimary *node) // Done
    {
        cout << node->name;
    };
    void codePrinter::visit(ast::IfStatement *node) // Done
    {
        indent();
        cout << "if ";
        node->condition->accept(this);
        cout << " then" << endl;
        depth++;
        node->ifBody->accept(this);
        depth--;
        if (node->elseBody)
        {
            indent();
            cout << "else" << endl;
            depth++;
            node->elseBody->accept(this);
            depth--;
        }
        indent();
        cout << "end" << endl;
    };
    void codePrinter::visit(ast::WhileLoop *node) // Done
    {
        indent();
        cout << "while ";
        node->condition->accept(this);
        cout << " loop" << endl;
        depth++;
        node->loopBody->accept(this);
        depth--;
        indent();
        cout << "end" << endl;
    };
    void codePrinter::visit(ast::ForLoop *node) // Done
    {
        indent();
        cout << "for " << node->identifier << " in ";
        if (node->to < node->from)
        {
            cout << "reverse ";
            node->to->accept(this);
            cout << " .. ";
            node->from->accept(this);
        }
        else
        {
            node->from->accept(this);
            cout << " .. ";
            node->to->accept(this);
        }
        cout << " loop" << endl;
        depth++;
        node->loopBody->accept(this);
        depth--;
    };

}