#include <codeGenerator.hpp>
namespace generator{
    void codeGenerator::visitProgram(ast::Program *node)
    {
        for (auto decl : node->decls)
        {
            if (decl)
            {
                decl->accept(this);
            }
        }
    };
    void codeGenerator::visitTypeDecl(ast::TypeDecl *node)
    {
        if (node->type)
        {
            node->type->accept(this);
        }
    };
    void codeGenerator::visitRoutineDecl(ast::RoutineDecl *node)
    {
        module = std::make_unique<llvm::Module>("name", context);
        llvm::FunctionType *funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), {llvm::Type::getInt32Ty(context)}, false);
        llvm::Function *func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "return_input", module.get());
        // create basic block and set insertion point
        llvm::BasicBlock *bb = llvm::BasicBlock::Create(context, "entry", func);
        builder.SetInsertPoint(bb);
        // create function argument
        llvm::Value *arg = func->arg_begin();
        arg->setName("input");
        // return input argument
        builder.CreateRet(arg);
        // print LLVM IR code
        module->print(llvm::errs(), nullptr);
        
        if (node->params)
        {
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
            node->returnType->accept(this);
        }
        if (node->body)
        {
            node->body->accept(this);
        }
    };
    void codeGenerator::visitParameterDecl(ast::ParameterDecl *node)
    {
        if (node->type)
        {
            node->type->accept(this);
        }
    }
    void codeGenerator::visitGlobalVarDecl(ast::GlobalVarDecl *node)
    {

        if (node->init)
        {
            node->init->accept(this);
        }
    }
    void codeGenerator::visitBody(ast::Body *node)
    {
        for (auto entity : node->entities)
        {
            if (entity)
            {
                entity->accept(this);
            }
        }
    };
    void codeGenerator::visitLocalVarDecl(ast::LocalVarDecl *node)
    {
       
        if (node->init)
        {
            node->init->accept(this);
        }
    };
    void codeGenerator::visitAssignment(ast::Assignment *node)
    {
        if (node->expr)
        {
            node->expr->accept(this);
        }

    };
    void codeGenerator::visitRoutineCall(ast::RoutineCall *node)
    {
       if(node->args){
            node->args->accept(this);
       }
    };
    void codeGenerator::visitReturn(ast::Return *node)
    {
        if (node->expr)
        {
           node->expr->accept(this);
        }
    };

    void codeGenerator::visitIf(ast::If *node)
    {
        ast::Type *cond_type;
        if (node->condition)
        {
            node->condition->accept(this);
        }
        if (node->then)
        {
            node->then->accept(this);
        }
        if (node->else_)
        {
            node->else_->accept(this);
        }
    };
    void codeGenerator::visitWhileLoop(ast::WhileLoop *node)
    {
        ast::Type *cond_type;
        if (node->condition)
        {
            node->condition->accept(this);
        }
        if (node->body)
        {
            node->body->accept(this);
        }
    };
    void codeGenerator::visitForLoop(ast::ForLoop *node)
    {
        if (node->range)
        {
            node->range->accept(this);
        }
        if (node->body)
        {
            node->body->accept(this);
        }
    };
    void codeGenerator::visitRange(ast::Range *node)
    {
        if (node->from)
        {
            node->from->accept(this);
        }
        if (node->to)
        {
            node->to->accept(this);
        }
    }
    void codeGenerator::visitPrint(ast::Print *node)
    {
        if (node->expr)
        {
            node->expr->accept(this);
        }
    };

    void codeGenerator::visitIntegerType(ast::IntegerType *node)
    {
    };
    void codeGenerator::visitRealType(ast::RealType *node)
    {
    };
    void codeGenerator::visitBooleanType(ast::BooleanType *node)
    {
    };
    void codeGenerator::visitArrayType(ast::ArrayType *node)
    {
        if (node->size)
        {
            node->size->accept(this);
        }
        if (node->type)
        {
            node->type->accept(this);
        }
    };
    void codeGenerator::visitRecordType(ast::RecordType *node)
    {
        for (auto field : node->decls->vars)
        {
            if (field)
            {
                field->accept(this);
            }
        }
    };

    void codeGenerator::visitBinaryExpr(ast::BinaryExpr *node)
    {
        if (node->left)
        {
            node->left->accept(this);
        }
        if (node->right)
        {
            node->right->accept(this);
        }
    };
    void codeGenerator::visitLogicExpr(ast::LogicExpr *node)
    {
        if (node->left)
        {
            node->left->accept(this);
        }
        if (node->right)
        {
            node->right->accept(this);
        }
    };
    void codeGenerator::visitComparisonExpr(ast::ComparisonExpr *node)
    {
        if (node->left)
        {
            node->left->accept(this);
        }
        if (node->right)
        {
            node->right->accept(this);
        }
    };
    void codeGenerator::visitIntegerValue(ast::IntegerValue *node)
    {
    };
    void codeGenerator::visitRealValue(ast::RealValue *node)
    {
    };
    void codeGenerator::visitBooleanValue(ast::BooleanValue *node)
    {
    };

    void codeGenerator::visitVar(ast::Var *node)
    {
        if (node->accesses)
        {
            for (auto AV : node->accesses->accesses)
            {
                if (AV)
                    AV->accept(this);
            }
        }
    };
    void codeGenerator::visitArrayAccess(ast::ArrayAccess *node)
    {
        
    }
    void codeGenerator::visitRecordAccess(ast::RecordAccess *node)
    {
    }
    void codeGenerator::visitRoutineCallValue(ast::RoutineCallValue *node)
    {

    };
}