#include "prettyPrinter.hpp"
#include "lexer.h"
#include "Enums.h"
using namespace std;

std::map<BinaryOperator, std::string> binary_to_str{
    {BinaryOperator::Plus, " + "},
    {BinaryOperator::Minus, " - "},
    {BinaryOperator::Mul, " * "},
    {BinaryOperator::Div, " / "},
    {BinaryOperator::Mod, " % "}};

std::map<LogicOperator, std::string> logic_to_str{
    {LogicOperator::Or, " or "},
    {LogicOperator::Xor, " xor "},
    {LogicOperator::And, " and "}};

std::map<ComparisonOperator, std::string> compare_to_str{
    {ComparisonOperator::Equal, " = "},
    {ComparisonOperator::NotEqual, " /= "},
    {ComparisonOperator::Less, " < "},
    {ComparisonOperator::LessEqual, " <= "},
    {ComparisonOperator::Greater, " > "},
    {ComparisonOperator::GreaterEqual, " >= "}};

namespace prettyPrinter
{

    void codePrinter::indent()
    {
        for (int i = 0; i < depth; i++)
        {
            cout << "\t";
        }
    }

    void codePrinter::visitProgram(ast::Program *p)
    {
        for (auto &decl : p->decls)
        {
            if (decl != nullptr)
                decl->accept(this);
        }
    }

    void codePrinter::visitDecl(ast::Decl *p) {}

    void codePrinter::visitRoutineDecl(ast::RoutineDecl *p)
    {
        cout << "routine ";
        visitIdent(p->name);
        cout << "(";
        if (p->params != nullptr)
        {
            p->params->accept(this);
        }
        cout << ")";
        if (p->returnType != nullptr)
        {
            cout << " : ";
            p->returnType->accept(this);
        }
        if (p->body != nullptr)
        {
            cout << " is" << endl;
            depth++;
            p->body->accept(this);
            depth--;
        }
        indent();
        cout << "end" << endl;
    }

    void codePrinter::visitGlobalVarDecl(ast::GlobalVarDecl *p)
    {
        cout << "var ";
        visitIdent(p->name);
        if (p->type != nullptr)
        {
            cout << " : ";
            p->type->accept(this);
        }
        if (p->init != nullptr)
        {
            cout << " is ";
            p->init->accept(this);
        }
        cout << ";" << endl;
    }

    void codePrinter::visitExpr(ast::Expr *p) {}

    void codePrinter::visitExprList(ast::ExprList *p)
    {
        bool flag = false;
        for (auto &expr : p->exprs)
        {
            if (flag)
                cout << ", ";
            flag = true;
            if (expr != nullptr)
                expr->accept(this);
        }
    }

    void codePrinter::visitBinaryExpr(ast::BinaryExpr *p)
    {
        cout << "(";
        p->left->accept(this);
        cout << binary_to_str[p->op];
        p->right->accept(this);
        cout << ")";
    }

    void codePrinter::visitLogicExpr(ast::LogicExpr *p)
    {
        cout << "(";
        p->left->accept(this);
        cout << logic_to_str[p->op];
        p->right->accept(this);
        cout << ")";
    }

    void codePrinter::visitComparisonExpr(ast::ComparisonExpr *p)
    {
        cout << "(";
        p->left->accept(this);
        cout << compare_to_str[p->op];
        p->right->accept(this);
        cout << ")";
    }

    void codePrinter::visitValue(ast::Value *p) {}

    void codePrinter::visitIntegerValue(ast::IntegerValue *p)
    {
        cout << p->value;
    }

    void codePrinter::visitRealValue(ast::RealValue *p)
    {
        cout << p->value;
    }

    void codePrinter::visitBooleanValue(ast::BooleanValue *p)
    {
        if (p->value)
            cout << "true";
        else
            cout << "false";
    }

    void codePrinter::visitRoutineCallValue(ast::RoutineCallValue *p)
    {
        visitIdent(p->name);
        cout << "(";
        if (p->args != nullptr)
        {
            p->args->accept(this);
        }
        cout << ")";
    }

    void codePrinter::visitVar(ast::Var *p)
    {
        visitIdent(p->name);
        if (p->accesses != nullptr)
        {
            p->accesses->accept(this);
        }
    }

    void codePrinter::visitTypeDecl(ast::TypeDecl *p)
    {
        cout << "type ";
        if (p->name != nullptr)
            p->name->accept(this);
        cout << " is ";
        if (p->type != nullptr)
            p->type->accept(this);
        cout << ";" << endl;
    }

    void codePrinter::visitType(ast::Type *p) {}

    void codePrinter::visitPrimitiveType(ast::PrimitiveType *p) {}

    void codePrinter::visitUserType(ast::UserType *p) {}

    void codePrinter::visitTypeIdentifier(ast::TypeIdentifier *p)
    {
        visitIdent(p->name);
    }

    void codePrinter::visitIntegerType(ast::IntegerType *p)
    {
        cout << "integer";
    }

    void codePrinter::visitRealType(ast::RealType *p)
    {
        cout << "real";
    }

    void codePrinter::visitBooleanType(ast::BooleanType *p)
    {
        cout << "boolean";
    }

    void codePrinter::visitArrayType(ast::ArrayType *p)
    {
        cout << "array ";
        if (p->size != nullptr)
        {
            cout << "[";
            p->size->accept(this);
            cout << "] ";
        }
        if (p->type != nullptr)
        {
            p->type->accept(this);
        }
    }

    void codePrinter::visitRecordType(ast::RecordType *p)
    {
        cout << "record" << endl;
        if (p->decls != nullptr)
        {
            depth++;
            p->decls->accept(this);
            depth--;
        }
        cout << "end";
    }

    void codePrinter::visitParameterDecl(ast::ParameterDecl *p)
    {
        visitIdent(p->name);
        cout << " : ";
        if (p->type != nullptr)
            p->type->accept(this);
    }

    void codePrinter::visitParameterList(ast::ParameterList *p)
    {
        bool flag = false;
        for (auto &decl : p->decls)
        {
            if (flag)
                cout << ", ";
            flag = true;
            if (decl != nullptr)
                decl->accept(this);
        }
    }

    void codePrinter::visitBody(ast::Body *p)
    {
        for (auto &entity : p->entities)
        {
            if (entity != nullptr)
                entity->accept(this);
        }
    }

    void codePrinter::visitBodyEntity(ast::BodyEntity *p) {}

    void codePrinter::visitLocalVarDecl(ast::LocalVarDecl *p)
    {
        indent();
        cout << "var ";
        visitIdent(p->name);
        if (p->type != nullptr)
        {
            cout << " : ";
            p->type->accept(this);
        }
        if (p->init != nullptr)
        {
            cout << " is ";
            p->init->accept(this);
        }
        cout << ";" << endl;
    }

    void codePrinter::visitLocalVarList(ast::LocalVarList *p)
    {
        for (auto &var : p->vars)
        {
            if (var != nullptr)
                var->accept(this);
        }
    }

    void codePrinter::visitStatement(ast::Statement *p) {}

    void codePrinter::visitAssignment(ast::Assignment *p)
    {
        indent();
        if (p->var != nullptr)
            p->var->accept(this);
        cout << " := ";
        if (p->expr != nullptr)
            p->expr->accept(this);
        cout << ";" << endl;
    }

    void codePrinter::visitRoutineCall(ast::RoutineCall *p)
    {
        indent();
        visitIdent(p->name);
        cout << "(";
        if (p->args != nullptr)
            p->args->accept(this);
        cout << ");" << endl;
    }

    void codePrinter::visitReturn(ast::Return *p)
    {
        indent();
        cout << "return ";
        if (p->expr != nullptr)
            p->expr->accept(this);
        cout << ";" << endl;
    }

    void codePrinter::visitPrint(ast::Print *p)
    {
        indent();
        cout << "print(";
        if (p->expr != nullptr)
            p->expr->accept(this);
        cout << ");" << endl;
    }

    void codePrinter::visitWhileLoop(ast::WhileLoop *p)
    {
        indent();
        cout << "while ";
        if (p->condition != nullptr)
            p->condition->accept(this);
        cout << " loop" << endl;
        if (p->body != nullptr)
        {
            depth++;
            p->body->accept(this);
            depth--;
        }
        indent();
        cout << "end" << endl;
    }

    void codePrinter::visitForLoop(ast::ForLoop *p)
    {
        indent();
        cout << "for ";
        visitIdent(p->name);
        cout << " in ";
        if (p->range != nullptr)
            p->range->accept(this);
        cout << " loop" << endl;
        if (p->body != nullptr)
        {
            depth++;
            p->body->accept(this);
            depth--;
        }
        indent();
        cout << "end" << endl;
    }

    void codePrinter::visitRange(ast::Range *p)
    {
        if (p->from != nullptr)
            p->from->accept(this);
        cout << " .. ";
        if (p->to != nullptr)
            p->to->accept(this);
    }

    void codePrinter::visitIf(ast::If *p)
    {
        indent();
        cout << "if ";
        if (p->condition != nullptr)
            p->condition->accept(this);
        cout << " then" << endl;
        if (p->then != nullptr)
        {
            depth++;
            p->then->accept(this);
            depth--;
        }
        if (p->else_ != nullptr)
        {
            indent();
            cout << "else" << endl;
            depth++;
            p->else_->accept(this);
            depth--;
        }
        indent();
        cout << "end" << endl;
    }

    void codePrinter::visitNestedAccess(ast::NestedAccess *p) {}

    void codePrinter::visitNestedAccessList(ast::NestedAccessList *p)
    {
        for (auto &access : p->accesses)
        {
            if (access != nullptr)
                access->accept(this);
        }
    }

    void codePrinter::visitArrayAccess(ast::ArrayAccess *p)
    {
        if (p->index != nullptr)
        {
            cout << "[";
            p->index->accept(this);
            cout << "]";
        }
    }

    void codePrinter::visitRecordAccess(ast::RecordAccess *p)
    {
        cout << ".";
        visitIdent(p->name);
    }

    void codePrinter::visitInteger(ast::Integer x)
    {
        cout << x;
    }

    void codePrinter::visitReal(ast::Real x)
    {
        cout << x;
    }

    void codePrinter::visitBoolean(ast::Boolean x)
    {
        cout << x;
    }

    void codePrinter::visitIdent(ast::Ident x)
    {
        cout << x;
    }

}