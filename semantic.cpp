#include "semantic.hpp"
#include "astPrinter.hpp"
#include "lexer.h"

namespace analyzer
{
    void Semantic::visit(ast::Program *node)
    {
        for (auto type : node->types)
        {
            if (type)
            {
                type->accept(this);
            }
        }

        for (auto routine : node->routines)
        {
            if (routine)
            {
                routine->accept(this);
            }
        }
    };
    void Semantic::visit(ast::TypeDeclaration *node)
    {
        // I don't think this is shadowing, this is an error, type declarations are all global.
        if (typeDeclSymbolTable.find(node->name) != typeDeclSymbolTable.end())
        {
            warn_shadow(node->name);
        }
        ast::Type *type;
        if (node->dtype)
        {
            node->dtype->accept(this);
        }
        type = actual_type;
        typeDeclSymbolTable[node->name] = type;
    };
    void Semantic::visit(ast::RoutineDeclaration *node)
    {
        varDeclSymbolTable.clear();
        // I don't think this is shadowing, this is an error, routines are all global.
        if (routineDeclSymbolTable.find(node->name) != routineDeclSymbolTable.end())
        {
            warn_shadow(node->name);
        }
        for (auto parameter : node->params)
        {
            if (parameter)
            {
                // add parameter to identifier table
                parameter->accept(this);
            }
        }
        ast::Type *ret_type;
        if (node->rtype)
        {
            node->rtype->accept(this);
        }
        ret_type = actual_type;
        if (node->body)
        {
            node->body->accept(this);
        }
        routineDeclSymbolTable[node->name] = ret_type;
    };
    void Semantic::visit(ast::VariableDeclaration *node)
    {
        if (varDeclSymbolTable.find(node->name) != varDeclSymbolTable.end())
        {
            warn_shadow(node->name);
        }
        if (node->dtype)
        {
            node->dtype->accept(this);
        }
        ast::Type *var_type = actual_type;
        if (node->initial_value)
        {
            node->initial_value->accept(this);
        }
        ast::Type *expr_type = actual_type;
        if (node->dtype && node->initial_value)
        {
            typecheck_types(var_type, expr_type);
        }
        else if (node->initial_value)
        {
            var_type = expr_type;
        }
        varDeclSymbolTable[node->name] = var_type;
    };
    void Semantic::visit(ast::Body *node)
    {
        for (auto variableDecl : node->variables)
        {
            if (variableDecl)
            {
                variableDecl->accept(this);
            }
        }

        for (auto statement : node->statements)
        {
            if (statement)
            {
                statement->accept(this);
            }
        }
    };
    void Semantic::visit(ast::IntType *node)
    {
        actual_type = node;
    };
    void Semantic::visit(ast::DoubleType *node)
    {
        actual_type = node;
    };
    void Semantic::visit(ast::BoolType *node)
    {
        actual_type = node;
    };
    void Semantic::visit(ast::ArrayType *node)
    {
        if (node->size)
        {
            node->size->accept(this);
        }
        if (node->dtype)
        {
            node->dtype->accept(this);
        }
        actual_type = node;
    };
    void Semantic::visit(ast::RecordType *node)
    {
        for (auto field : node->fields)
        {
            if (field)
            {
                field->accept(this);
            }
        }
        actual_type = node;
    };
    void Semantic::visit(ast::IntLiteral *node)
    {
        actual_type = new ast::IntType();
    };
    void Semantic::visit(ast::DoubleLiteral *node)
    {
        actual_type = new ast::DoubleType();
    };
    void Semantic::visit(ast::BoolLiteral *node)
    {
        actual_type = new ast::BoolType();
    };
    void Semantic::visit(ast::BinaryExpression *node)
    {
        ast::Type *lhs_type;
        if (node->lhs)
        {
            node->lhs->accept(this);
        }
        lhs_type = actual_type;
        ast::Type *rhs_type;
        if (node->rhs)
        {
            node->rhs->accept(this);
        }
        rhs_type = actual_type;
        typecheck_types(lhs_type, rhs_type);
        actual_type = lhs_type;
    };
    void Semantic::visit(ast::BitwiseExpression *node)
    {
        ast::Type *lhs_type;
        if (node->lhs)
        {
            node->lhs->accept(this);
        }
        lhs_type = actual_type;
        ast::Type *rhs_type;
        if (node->rhs)
        {
            node->rhs->accept(this);
        }
        rhs_type = actual_type;
        typecheck_types(lhs_type, rhs_type);
        actual_type = lhs_type;
    };
    void Semantic::visit(ast::ComparisonExpression *node)
    {
        ast::Type *lhs_type;
        if (node->lhs)
        {
            node->lhs->accept(this);
        }
        lhs_type = actual_type;
        ast::Type *rhs_type;
        if (node->rhs)
        {
            node->rhs->accept(this);
        }
        rhs_type = actual_type;
        typecheck_types(lhs_type, rhs_type);
        actual_type = new ast::BoolType();
    };
    void Semantic::visit(ast::Assignment *node) // TODO
    {
        ast::Type *lhs_type;
        if (node->modifiablePrimary)
        {
            node->modifiablePrimary->accept(this);
        }
        lhs_type = actual_type;
        ast::Type *rhs_type;
        if (node->expression)
        {
            node->expression->accept(this);
        }
        rhs_type = actual_type;
        typecheck_types(lhs_type, rhs_type);
    };
    void Semantic::visit(ast::Print *node) // DONE
    {
        if (node->exp)
        {
            node->exp->accept(this);
        }
    };
    void Semantic::visit(ast::Return *node) // TODO
    {
        // TODO type check exp with return type of current routine if exists
        if (node->exp)
        {
            node->exp->accept(this);
        }
    };
    // void Semantic::visit(ast::Identifier *node)
    // {
    //     if (node->idx)
    //     {
    //         node->idx->accept(this);
    //     }
    // };
    void Semantic::visit(ast::ModifiablePrimary *node)
    {
        if (varDeclSymbolTable.find(node->name) == varDeclSymbolTable.end())
        {
            err_undefined_obj(node->name);
        }
        auto currentType = varDeclSymbolTable[node->name];
        for(auto AV : node->accessValues){
            if(std::holds_alternative<ast::node_ptr<ast::Expression>>(AV)){
                // check that we're trying to access an array
                std::string got_type = type_to_string(currentType);
                if(got_type.substr(0,5) != "Array" ){
                    err_expected_got(got_type, "Array");
                    break;
                }
                else{
                    auto castedType = static_cast<ast::ArrayType*>(currentType);
                    currentType = &(*castedType->dtype);
                }
            }
            else{
                // check that we're trying to access a record and that this field exists
                std::string fieldName = std::get<std::string>(AV);
                std::string got_type = type_to_string(currentType);
                if(got_type.substr(0,6) != "Record" ){
                    err_expected_got(got_type, "Record");
                    break;
                }
                else{
                    auto castedType = static_cast<ast::RecordType*>(currentType);
                    bool found_field = false;
                    for (auto field : castedType->fields){
                        if(field->name == fieldName){
                            found_field = true;
                            currentType = &(*field->dtype);
                        }
                    }
                    if (!found_field){
                        err_undefined_obj(fieldName);
                        break;
                    }
                }
            }
        }
        actual_type = currentType;
    };
    void Semantic::visit(ast::IfStatement *node)
    {
        ast::Type *cond_type;
        if (node->condition)
        {
            node->condition->accept(this);
        }
        cond_type = actual_type;
        typecheck_types(cond_type, new ast::BoolType());
        if (node->ifBody)
        {
            node->ifBody->accept(this);
        }
        if (node->elseBody)
        {
            node->elseBody->accept(this);
        }
    };
    void Semantic::visit(ast::WhileLoop *node)
    {
        ast::Type *cond_type;
        if (node->condition)
        {
            node->condition->accept(this);
        }
        cond_type = actual_type;
        typecheck_types(cond_type, new ast::BoolType());
        if (node->loopBody)
        {
            node->loopBody->accept(this);
        }
    };
    void Semantic::visit(ast::ForLoop *node)
    {
        ast::Type *fromType;
        if (node->from)
        {
            node->from->accept(this);
        }
        fromType = actual_type;
        ast::Type *toType;
        if (node->to)
        {
            node->to->accept(this);
        }
        toType = actual_type;
        typecheck_types(fromType, new ast::IntType());
        typecheck_types(toType, new ast::IntType());
        if (node->loopBody)
        {
            node->loopBody->accept(this);
        }
    };
    void Semantic::visit(ast::ForeachLoop *node) // TODO
    {
        // TODO remove foreach loop from all places
        if (node->modifiablePrimary)
        {
            node->modifiablePrimary->accept(this);
        }
        if (node->loopBody)
        {
            node->loopBody->accept(this);
        }
    };
    void Semantic::visit(ast::RoutineCall *node) // TODO
    {
        // TODO Check for wrong number of arguments
        // TODO Check for wrong type of arguments
        if (routineDeclSymbolTable.find(node->name) == routineDeclSymbolTable.end())
        {
            err_undefined_obj(node->name);
        }
        for (auto arg : node->args)
        {
            if (arg)
            {
                arg->accept(this);
            }
        }
    };

}