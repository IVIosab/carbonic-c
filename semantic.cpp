#include "semantic.hpp"
#include "astPrinter.hpp"
#include "lexer.h"

namespace analyzer
{
    void Semantic::visitProgram(ast::Program *node)
    {
        for (auto decl : node->decls)
        {
            if (decl)
            {
                decl->accept(this);
            }
        }
    };
    void Semantic::visitTypeDecl(ast::TypeDecl *node)
    {
        if (typeDeclSymbolTable.count(node->name->name) ||
        varDeclSymbolTable.count(node->name->name) ||
        routineDeclTable.count(node->name->name))
        {
            err_second_declaration(node->name->name);
        }
        ast::Type *type;
        if (node->type)
        {
            actual_type = nullptr;
            node->type->accept(this);
        }
        type = actual_type;
        typeDeclSymbolTable[node->name->name] = type;
    };
    void Semantic::visitRoutineDecl(ast::RoutineDecl *node)
    {
        routine_vars_n = 0;
        routine_return_type = nullptr;
        ast::Type *ret_type = nullptr;
        if (routineDeclTable.count(node->name) ||
        varDeclSymbolTable.count(node->name) ||
        typeDeclSymbolTable.count(node->name))
        {
            err_second_declaration(node->name);
        }
        if(node->params){
            for (auto parameter : node->params->decls)
            {
                if (parameter)
                {
                    parameter->accept(this);
                }
            }
        }
        if (node->returnType)
        {
            actual_type = nullptr;
            node->returnType->accept(this);
        }
        ret_type = actual_type;
        if (node->body)
        {
            node->body->accept(this);
        }
        node->returnType = ret_type; 
        routineDeclTable[node->name] = node;
        if(routine_return_type && ret_type)
            typecheck_types(routine_return_type, ret_type);
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
    void Semantic::visitParameterDecl(ast::ParameterDecl* node){
        if (typeDeclSymbolTable.count(node->name) 
                    || routineDeclTable.count(node->name))
        {
            err_second_declaration(node->name);
        }
        if (varDeclSymbolTable.count(node->name))
        {
            warn_shadow(node->name);
        }
        if (node->type)
        {
            actual_type = nullptr;
            node->type->accept(this);
        }
        ast::Type *var_type = actual_type;
        varDeclSymbolTable[node->name] = var_type;
        varStack.push_back({node->name, var_type});
        routine_vars_n ++ ;
    }
    void Semantic::visitGlobalVarDecl(ast::GlobalVarDecl *node)
    {
        if (typeDeclSymbolTable.count(node->name) ||
         routineDeclTable.count(node->name)|| 
         varDeclSymbolTable.count(node->name))
        {
            err_second_declaration(node->name);
        }
        if (node->type)
        {
            actual_type = nullptr;
            node->type->accept(this);
        }
        ast::Type *var_type = actual_type;
        if (node->init)
        {
            node->init->accept(this);
        }
        ast::Type *expr_type = actual_type;
        if (node->type && node->init)
        {
            typecheck_types(var_type, expr_type);
        }
        else if (node->init)
        {
            var_type = expr_type;
        }
        varDeclSymbolTable[node->name] = var_type;
        varStack.push_back({node->name, var_type});
    };
    void Semantic::visitBody(ast::Body *node)
    {   
        for (auto entity : node->entities){
            if(entity){
                entity->accept(this);
            }
        }
    };
    void Semantic::visitLocalVarDecl(ast::LocalVarDecl *node)
    {
        if (typeDeclSymbolTable.count(node->name) 
                    || routineDeclTable.count(node->name))
        {
            err_second_declaration(node->name);
        }
        if (varDeclSymbolTable.count(node->name))
        {
            warn_shadow(node->name);
        }
        if (node->type)
        {
            actual_type = nullptr;
            node->type->accept(this);
        }
        ast::Type *var_type = actual_type;
        if (node->init)
        {
            node->init->accept(this);
        }
        ast::Type *expr_type = actual_type;
        if (var_type && expr_type)
        {
            typecheck_types(var_type, expr_type);
        }
        else if (expr_type)
        {
            var_type = expr_type;
        }
        varDeclSymbolTable[node->name] = var_type;
        varStack.push_back({node->name, var_type});
        routine_vars_n ++;
    };
    void Semantic::visitAssignment(ast::Assignment *node)
    {
        ast::Type *lhs_type;
        if (node->var)
        {
            node->var->accept(this);
        }
        lhs_type = actual_type;
        ast::Type *rhs_type;
        if (node->expr)
        {
            node->expr->accept(this);
        }
        rhs_type = actual_type;
        typecheck_types(lhs_type, rhs_type);
    };
    void Semantic::visitRoutineCall(ast::RoutineCall *node)
    {
        if (routineDeclTable.find(node->name) == routineDeclTable.end())
        {
            err_undefined_obj(node->name);
        }
        auto routine = routineDeclTable[node->name];
        int size1 = 0, size2 = 0;
        if(node->args){
            size1 = node->args->exprs.size();
        }
        if(routine->params){
            size2 = routine->params->decls.size();
        }
        if(size1 != size2){
            err_wrong_params_number(size2, size1);
        }
        if(node->args && routine->params){
            for (int i = 0; i < node->args->exprs.size(); i++){
                if(routine->params->decls[i]->type && node->args->exprs[i]){
                    if(node->args->exprs[i]){
                        actual_type = nullptr;
                        node->args->exprs[i]->accept(this);
                    }
                    if(actual_type){
                        typecheck_types(routine->params->decls[i]->type, actual_type);
                    }
                }
            }
        }
    };
    void Semantic::visitReturn(ast::Return *node)
    {
        if (node->expr)
        {
            if(node->expr){
                actual_type = nullptr;
                node->expr->accept(this);
            }
            if(actual_type){
                routine_return_type = actual_type;
            }
        }
    };

    void Semantic::visitIf(ast::If *node)
    {
        ast::Type *cond_type;
        if (node->condition)
        {
            node->condition->accept(this);
        }
        cond_type = actual_type;
        typecheck_types(new ast::BooleanType(), cond_type);
        if (node->then)
        {
            node->then->accept(this);
        }
        if (node->else_)
        {
            node->else_->accept(this);
        }
    };
    void Semantic::visitWhileLoop(ast::WhileLoop *node)
    {
        ast::Type *cond_type;
        if (node->condition)
        {
            node->condition->accept(this);
        }
        cond_type = actual_type;
        typecheck_types(new ast::BooleanType(), cond_type);
        if (node->body)
        {
            node->body->accept(this);
        }
    };
    void Semantic::visitForLoop(ast::ForLoop *node)
    {
        varDeclSymbolTable[node->name] = new ast::IntegerType();
        if(node->range){
            node->range->accept(this);
        }
        if (node->body)
        {
            node->body->accept(this);
        }
    };
    void Semantic::visitRange(ast::Range* node){
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
        typecheck_types(new ast::IntegerType(), fromType);
        typecheck_types(new ast::IntegerType(), toType);
    }
    void Semantic::visitPrint(ast::Print *node)
    {
        if (node->expr)
        {
             node->expr->accept(this);
        }
    };

    void Semantic::visitIntegerType(ast::IntegerType *node)
    {
        actual_type = node;
    };
    void Semantic::visitRealType(ast::RealType *node)
    {
        actual_type = node;
    };
    void Semantic::visitBooleanType(ast::BooleanType *node)
    {
        actual_type = node;
    };
    void Semantic::visitArrayType(ast::ArrayType *node)
    {
        if (node->size)
        {
            node->size->accept(this);
        }
        if (node->type)
        {
            node->type->accept(this);
        }
        actual_type = node;
    };
    void Semantic::visitRecordType(ast::RecordType *node)
    {
        for (auto field : node->decls->vars)
        {
            if (field)
            {
                field->accept(this);
            }
        }
        actual_type = node;
    };
    
    void Semantic::visitBinaryExpr(ast::BinaryExpr *node)
    {
        ast::Type *lhs_type;
        if (node->left)
        {
            node->left->accept(this);
        }
        lhs_type = actual_type;
        ast::Type *rhs_type;
        if (node->right)
        {
            node->right->accept(this);
        }
        rhs_type = actual_type;
        typecheck_types(lhs_type, rhs_type);
        actual_type = lhs_type;
    };
    void Semantic::visitLogicExpr(ast::LogicExpr *node)
    {
        ast::Type *lhs_type;
        if (node->left)
        {
            node->left->accept(this);
        }
        lhs_type = actual_type;
        ast::Type *rhs_type;
        if (node->right)
        {
            node->right->accept(this);
        }
        rhs_type = actual_type;
        typecheck_types(lhs_type, rhs_type);
        actual_type = lhs_type;
    };
    void Semantic::visitComparisonExpr(ast::ComparisonExpr *node)
    {
        ast::Type *lhs_type;
        if (node->left)
        {
            node->left->accept(this);
        }
        lhs_type = actual_type;
        ast::Type *rhs_type;
        if (node->right)
        {
            node->right->accept(this);
        }
        rhs_type = actual_type;
        typecheck_types(lhs_type, rhs_type);
        actual_type = new ast::BooleanType();
    };
    void Semantic::visitIntegerValue(ast::IntegerValue* node)
    {
        actual_type = new ast::IntegerType();
    };
    void Semantic::visitRealValue(ast::RealValue* node)
    {
        actual_type = new ast::RealType();
    };
    void Semantic::visitBooleanValue(ast::BooleanValue* node)
    {
        actual_type = new ast::BooleanType();
    };

    void Semantic::visitVar(ast::Var *node)
    {
        if (varDeclSymbolTable.find(node->name) == varDeclSymbolTable.end())
        {
            err_undefined_obj(node->name);
        }
        current_var_type = varDeclSymbolTable[node->name];
        if(node->accesses){
            for (auto AV : node->accesses->accesses)
            {
                if(AV)
                    AV->accept(this);
            }
        }
        actual_type = current_var_type;
    };
    void Semantic::visitArrayAccess(ast::ArrayAccess* node){
        // check that we're trying to access an array
        auto my_current_type = current_var_type;
        std::string got_type = type_to_string(my_current_type);
        if (got_type.substr(0, 5) != "Array")
        {
            err_expected_got("Array", got_type);
        }
        else
        {
            auto access_value = node->index;
            access_value->accept(this);
            typecheck_types(new ast::IntegerType(), actual_type);
            if(auto casted_type = dynamic_cast<ast::ArrayType *>(my_current_type)){
                current_var_type = casted_type->type;
            }
        }

    }
    void Semantic::visitRecordAccess(ast::RecordAccess* node){
        // check that we're trying to access a record and that this field exists
        auto my_current_type = current_var_type;
        std::string field_name = node->name;
        std::string got_type = type_to_string(my_current_type);
        if (got_type.substr(0, 6) != "Record")
        {
            err_expected_got("Record", got_type);
        }
        else
        {
            if(auto casted_type = dynamic_cast<ast::RecordType *>(my_current_type)){
                bool found_field = false;
                for (auto field : casted_type->decls->vars)
                {
                    if (field->name == field_name)
                    {
                        found_field = true;
                        current_var_type = field->type;
                    }
                }
                if (!found_field)
                {
                    err_undefined_obj(field_name);
                }
            }
        }
    }
     void Semantic::visitRoutineCallValue(ast::RoutineCallValue *node)
    {
        if (routineDeclTable.find(node->name) == routineDeclTable.end())
        {
            err_undefined_obj(node->name);
        }
        auto routine = routineDeclTable[node->name];
        int size1 = 0, size2 = 0;
        if(node->args){
            size1 = node->args->exprs.size();
        }
        if(routine->params){
            size2 = routine->params->decls.size();
        }
        if(size1 != size2){
            err_wrong_params_number(size2, size1);
        }
        if(node->args && routine->params){
            for (int i = 0; i < node->args->exprs.size(); i++){
                if(routine->params->decls[i]->type && node->args->exprs[i]){
                    if(node->args->exprs[i]){
                        actual_type = nullptr;
                        node->args->exprs[i]->accept(this);
                    }
                    if(actual_type){
                        typecheck_types(routine->params->decls[i]->type, actual_type);
                    }
                }
            }
        }
    };
}