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
        for (auto variable : node->variables)
        {
            if(variable)
            {
                variable->accept(this);
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
        if (typeDeclSymbolTable.find(node->name) != typeDeclSymbolTable.end() ||
        varDeclSymbolTable.count(node->name) || routineDeclSymbolTable.count(node->name))
        {
            err_second_declaration(node->name);
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
        routine_vars_n = 0;
        if (routineDeclSymbolTable.find(node->name) != routineDeclSymbolTable.end() ||
        varDeclSymbolTable.count(node->name) || typeDeclSymbolTable.count(node->name))
        {
            err_second_declaration(node->name);
        }
        for (auto parameter : node->params)
        {
            if (parameter)
            {
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
        while(routine_vars_n -- ){
            if(varStack.size()){
                std::string delVar = varStack[varStack.size()-1].first;
                varDeclSymbolTable.erase(delVar);
                varStack.pop_back();
                ast::Type* shadowed_i = nullptr;
                for (auto i : varStack){
                    if (i.first == delVar){
                        shadowed_i = i.second;
                        break;
                    }
                }
                if(shadowed_i)
                    varDeclSymbolTable[delVar] = shadowed_i;
            }
        }
    };
    void Semantic::visit(ast::VariableDeclaration *node)
    {
        if (typeDeclSymbolTable.find(node->name) != typeDeclSymbolTable.end() 
                    || routineDeclSymbolTable.count(node->name))
        {
            err_second_declaration(node->name);
        }
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
        varStack.push_back({node->name, var_type});
        routine_vars_n ++;
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
    void Semantic::visit(ast::Assignment *node)
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
    void Semantic::visit(ast::Print *node)
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
    void Semantic::visit(ast::ModifiablePrimary *node)
    {
        if (varDeclSymbolTable.find(node->name) == varDeclSymbolTable.end())
        {
            err_undefined_obj(node->name);
        }
        auto current_type = varDeclSymbolTable[node->name];
        for (auto AV : node->accessValues)
        {
            if (std::holds_alternative<ast::node_ptr<ast::Expression>>(AV))
            {
                // check that we're trying to access an array
                std::string got_type = type_to_string(current_type);
                if (got_type.substr(0, 5) != "Array")
                {
                    err_expected_got(got_type, "Array");
                    break;
                }
                else
                {
                    auto access_value = std::get<ast::node_ptr<ast::Expression>>(AV);
                    access_value->accept(this);
                    typecheck_types(actual_type, new ast::IntType());
                    auto casted_type = static_cast<ast::ArrayType *>(current_type);
                    current_type = &(*casted_type->dtype);
                }
            }
            else
            {
                // check that we're trying to access a record and that this field exists
                std::string field_name = std::get<std::string>(AV);
                std::string got_type = type_to_string(current_type);
                if (got_type.substr(0, 6) != "Record")
                {
                    err_expected_got(got_type, "Record");
                    break;
                }
                else
                {
                    auto casted_type = static_cast<ast::RecordType *>(current_type);
                    bool found_field = false;
                    for (auto field : casted_type->fields)
                    {
                        if (field->name == field_name)
                        {
                            found_field = true;
                            current_type = &(*field->dtype);
                        }
                    }
                    if (!found_field)
                    {
                        err_undefined_obj(field_name);
                        break;
                    }
                }
            }
        }
        actual_type = current_type;
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
        varDeclSymbolTable[node->identifier] = new ast::IntType();
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
    // void Semantic::visit(ast::ForeachLoop *node) // TODO
    // {
    //     // TODO remove foreach loop from all places
    //     if (node->modifiablePrimary)
    //     {
    //         node->modifiablePrimary->accept(this);
    //     }
    //     if (node->loopBody)
    //     {
    //         node->loopBody->accept(this);
    //     }
    // };
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