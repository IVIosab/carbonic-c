#include "astPrinter.hpp"
#include "lexer.h"
#include "Enums.h"
using namespace std;

std::map<BinaryOperator, std::string> bin_to_str{
    {BinaryOperator::Plus, "+"},
    {BinaryOperator::Minus, "-"},
    {BinaryOperator::Mul, "*"},
    {BinaryOperator::Div, "/"},
    {BinaryOperator::Mod, "%"}};

std::map<LogicOperator, std::string> bit_to_str{
    {LogicOperator::Or, "or"},
    {LogicOperator::Xor, "xor"},
    {LogicOperator::And, "and"}};

std::map<ComparisonOperator, std::string> cmp_to_str{
    {ComparisonOperator::Equal, "="},
    {ComparisonOperator::NotEqual, "/="},
    {ComparisonOperator::Less, "<"},
    {ComparisonOperator::LessEqual, "<="},
    {ComparisonOperator::Greater, ">"},
    {ComparisonOperator::GreaterEqual, ">="}};

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

    void AstPrinter::visitProgram(ast::Program *p)
    {
        indent();
        cout << "Program" << endl;
        depth++;
        for (auto &decl : p->decls)
        {
            if (decl != nullptr)
                decl->accept(this);
        }
        depth--;
    }

    void AstPrinter::visitDecl(ast::Decl *p) {}

    void AstPrinter::visitRoutineDecl(ast::RoutineDecl *p)
    {
        indent();
        cout << "RoutineDecl" << endl;
        depth++;
        visitIdent(p->name);
        if (p->params != nullptr)
            p->params->accept(this);
        if (p->returnType != nullptr)
            p->returnType->accept(this);
        if (p->body != nullptr)
            p->body->accept(this);
        depth--;
    }

    void AstPrinter::visitGlobalVarDecl(ast::GlobalVarDecl *p)
    {
        indent();
        cout << "GlobalVarDecl" << endl;
        depth++;
        visitIdent(p->name);
        if (p->type != nullptr)
            p->type->accept(this);
        if (p->init != nullptr)
            p->init->accept(this);
        depth--;
    }

    void AstPrinter::visitExpr(ast::Expr *p) {}

    void AstPrinter::visitExprList(ast::ExprList *p)
    {
        indent();
        cout << "ExprList" << endl;
        depth++;
        for (auto &expr : p->exprs)
        {
            if (expr != nullptr)
                expr->accept(this);
        }
        depth--;
    }

    void AstPrinter::visitBinaryExpr(ast::BinaryExpr *p)
    {
        indent();
        cout << "BinaryExpr" << endl;
        depth++;
        p->left->accept(this);
        indent();
        cout << bin_to_str[p->op] << endl;
        p->right->accept(this);
        depth--;
    }

    void AstPrinter::visitLogicExpr(ast::LogicExpr *p)
    {
        indent();
        cout << "LogicExpr" << endl;
        depth++;
        p->left->accept(this);
        indent();
        cout << bit_to_str[p->op] << endl;
        p->right->accept(this);
        depth--;
    }

    void AstPrinter::visitComparisonExpr(ast::ComparisonExpr *p)
    {
        indent();
        cout << "ComparisonExpr" << endl;
        depth++;
        p->left->accept(this);
        indent();
        cout << cmp_to_str[p->op] << endl;
        p->right->accept(this);
        depth--;
    }

    void AstPrinter::visitValue(ast::Value *p) {}

    void AstPrinter::visitIntegerValue(ast::IntegerValue *p)
    {
        indent();
        cout << "IntegerValue" << endl;
        depth++;
        indent();
        cout << p->value << endl;
        depth--;
    }

    void AstPrinter::visitRealValue(ast::RealValue *p)
    {
        indent();
        cout << "RealValue" << endl;
        depth++;
        indent();
        cout << p->value << endl;
        depth--;
    }

    void AstPrinter::visitBooleanValue(ast::BooleanValue *p)
    {
        indent();
        cout << "BooleanValue" << endl;
        depth++;
        indent();
        cout << p->value << endl;
        depth--;
    }

    void AstPrinter::visitRoutineCallValue(ast::RoutineCallValue *p)
    {
        indent();
        cout << "RoutineCallValue" << endl;
        depth++;
        visitIdent(p->name);
        if (p->args != nullptr)
            p->args->accept(this);
        depth--;
    }

    void AstPrinter::visitVar(ast::Var *p) {}

    void AstPrinter::visitTypeDecl(ast::TypeDecl *p)
    {
        indent();
        cout << "TypeDecl" << endl;
        depth++;
        if (p->name != nullptr)
            p->name->accept(this);
        if (p->type != nullptr)
            p->type->accept(this);
        depth--;
    }

    void AstPrinter::visitType(ast::Type *p) {}

    void AstPrinter::visitPrimitiveType(ast::PrimitiveType *p) {}

    void AstPrinter::visitUserType(ast::UserType *p) {}

    void AstPrinter::visitTypeIdentifier(ast::TypeIdentifier *p)
    {
        indent();
        cout << "TypeIdentifier" << endl;
        depth++;
        visitIdent(p->name);
        depth--;
    }

    void AstPrinter::visitIntegerType(ast::IntegerType *p)
    {
        indent();
        cout << "IntegerType" << endl;
    }

    void AstPrinter::visitRealType(ast::RealType *p)
    {
        indent();
        cout << "RealType" << endl;
    }

    void AstPrinter::visitBooleanType(ast::BooleanType *p)
    {
        indent();
        cout << "BooleanType" << endl;
    }

    void AstPrinter::visitArrayType(ast::ArrayType *p)
    {
        indent();
        cout << "ArrayType" << endl;
        depth++;
        if (p->type != nullptr)
            p->type->accept(this);
        if (p->size != nullptr)
            p->size->accept(this);
        depth--;
    }

    void AstPrinter::visitRecordType(ast::RecordType *p)
    {
        indent();
        cout << "RecordType" << endl;
        depth++;
        if (p->decls != nullptr)
            p->decls->accept(this);
        depth--;
    }

    void AstPrinter::visitParameterDecl(ast::ParameterDecl *p)
    {
        indent();
        cout << "ParameterDecl" << endl;
        depth++;
        visitIdent(p->name);
        if (p->type != nullptr)
            p->type->accept(this);
        depth--;
    }

    void AstPrinter::visitParameterList(ast::ParameterList *p)
    {
        indent();
        cout << "ParameterList" << endl;
        depth++;
        for (auto &decl : p->decls)
        {
            if (decl != nullptr)
                decl->accept(this);
        }
        depth--;
    }

    void AstPrinter::visitBody(ast::Body *p)
    {
        indent();
        cout << "Body" << endl;
        depth++;
        for (auto &entity : p->entities)
        {
            if (entity != nullptr)
                entity->accept(this);
        }
        depth--;
    }

    void AstPrinter::visitBodyEntity(ast::BodyEntity *p) {}

    void AstPrinter::visitLocalVarDecl(ast::LocalVarDecl *p)
    {
        indent();
        cout << "LocalVarDecl" << endl;
        depth++;
        visitIdent(p->name);
        if (p->type != nullptr)
            p->type->accept(this);
        if (p->init != nullptr)
            p->init->accept(this);
        depth--;
    }

    void AstPrinter::visitLocalVarList(ast::LocalVarList *p)
    {
        indent();
        cout << "LocalVarList" << endl;
        depth++;
        for (auto &var : p->vars)
        {
            if (var != nullptr)
                var->accept(this);
        }
        depth--;
    }

    void AstPrinter::visitStatement(ast::Statement *p) {}

    void AstPrinter::visitAssignment(ast::Assignment *p)
    {
        indent();
        cout << "Assignment" << endl;
        depth++;
        if (p->var != nullptr)
            p->var->accept(this);
        if (p->expr != nullptr)
            p->expr->accept(this);
        depth--;
    }

    void AstPrinter::visitRoutineCall(ast::RoutineCall *p)
    {
        indent();
        cout << "RoutineCall" << endl;
        depth++;
        visitIdent(p->name);
        if (p->args != nullptr)
            p->args->accept(this);
        depth--;
    }

    void AstPrinter::visitReturn(ast::Return *p)
    {
        indent();
        cout << "Return" << endl;
        depth++;
        if (p->expr != nullptr)
            p->expr->accept(this);
        depth--;
    }

    void AstPrinter::visitPrint(ast::Print *p)
    {
        indent();
        cout << "Print" << endl;
        depth++;
        if (p->expr != nullptr)
            p->expr->accept(this);
        depth--;
    }

    void AstPrinter::visitWhileLoop(ast::WhileLoop *p)
    {
        indent();
        cout << "WhileLoop" << endl;
        depth++;
        if (p->condition != nullptr)
            p->condition->accept(this);
        if (p->body != nullptr)
            p->body->accept(this);
        depth--;
    }

    void AstPrinter::visitForLoop(ast::ForLoop *p)
    {
        indent();
        cout << "ForLoop" << endl;
        depth++;
        visitIdent(p->name);
        if (p->range != nullptr)
            p->range->accept(this);
        if (p->body != nullptr)
            p->body->accept(this);
        depth--;
    }

    void AstPrinter::visitRange(ast::Range *p)
    {
        indent();
        cout << "Range" << endl;
        depth++;
        if (p->from != nullptr)
            p->from->accept(this);
        if (p->to != nullptr)
            p->to->accept(this);
        depth--;
    }

    void AstPrinter::visitIf(ast::If *p)
    {
        indent();
        cout << "If" << endl;
        depth++;
        if (p->condition != nullptr)
            p->condition->accept(this);
        if (p->then != nullptr)
            p->then->accept(this);
        if (p->else_ != nullptr)
            p->else_->accept(this);
        depth--;
    }

    void AstPrinter::visitNestedAccess(ast::NestedAccess *p) {}

    void AstPrinter::visitNestedAccessList(ast::NestedAccessList *p)
    {
        indent();
        cout << "NestedAccessList" << endl;
        depth++;
        for (auto &access : p->accesses)
        {
            if (access != nullptr)
                access->accept(this);
        }
        depth--;
    }

    void AstPrinter::visitArrayAccess(ast::ArrayAccess *p)
    {
        indent();
        cout << "ArrayAccess" << endl;
        depth++;
        if (p->index != nullptr)
            p->index->accept(this);
        depth--;
    }

    void AstPrinter::visitRecordAccess(ast::RecordAccess *p)
    {
        indent();
        cout << "RecordAccess" << endl;
        depth++;
        visitIdent(p->name);
        depth--;
    }

    void AstPrinter::visitInteger(ast::Integer x)
    {
        indent();
        cout << "Integer" << endl;
        depth++;
        indent();
        cout << x << endl;
        depth--;
    }

    void AstPrinter::visitReal(ast::Real x)
    {
        indent();
        cout << "Real" << endl;
        depth++;
        indent();
        cout << x << endl;
        depth--;
    }

    void AstPrinter::visitBoolean(ast::Boolean x)
    {
        indent();
        cout << "Boolean" << endl;
        depth++;
        indent();
        cout << x << endl;
        depth--;
    }

    void AstPrinter::visitIdent(ast::Ident x)
    {
        indent();
        cout << "Ident" << endl;
        depth++;
        indent();
        cout << x << endl;
        depth--;
    }

}