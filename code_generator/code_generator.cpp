#include <codeGenerator.hpp>
namespace generator{
    void codeGenerator::visitProgram(ast::Program *node)
    {
        module = std::make_unique<llvm::Module>("Program", context);
        for (auto decl : node->decls)
        {
            if (decl)
            {
                decl->accept(this);
            }
        }
    };
    // TODO:
    void codeGenerator::visitTypeDecl(ast::TypeDecl *node)
    {
        if (node->type)
        {
            node->type->accept(this);
        }
    };
    void codeGenerator::visitRoutineDecl(ast::RoutineDecl *node)
    {
        std::vector<llvm::Type*> paramsVector;
        std::vector<std::string> paramNames;
        routine_vars_n = 0;
        if (node->params)
        {
            for (auto parameter : node->params->decls)
            {
                if (parameter)
                {
                    parameter->accept(this);
                    paramsVector.push_back(inferred_type);
                    paramNames.push_back(parameter->name);
                }
            }
        }
        llvm::ArrayRef<llvm::Type*> params(paramsVector);
        if (node->returnType)
        {
            node->returnType->accept(this);
        }
        llvm::Type* ret_type = inferred_type;
        llvm::FunctionType *funcType = llvm::FunctionType::get(ret_type, params, false);
        llvm::Function *func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, node->name, module.get());
        // add function to table
        //llvm::AllocaInst *x = builder->CreateAlloca(funcType, nullptr, node->name);
        //builder->CreateStore(func, x);
        // create basic block and set insertion point
        llvm::BasicBlock *bb = llvm::BasicBlock::Create(context, "entry", func);
        builder->SetInsertPoint(bb);
        // create function arguments
        if(node->params){
            for(int i = 0;i < node->params->decls.size(); i ++){
                std::cout << "I'm adding param " << node->params->decls[i]->name << '\n';
                routine_vars_n ++;
                llvm::Value *arg = (func->arg_begin()+i);
                arg->setName(node->params->decls[i]->name);
                // Create an alloca for this variable.
                llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(func, node->params->decls[i]->name);
                // Store the initial value into the alloca.
                builder->CreateStore(arg, Alloca);
                // Add arguments to variable symbol table.
                varDeclSymbolTable[node->params->decls[i]->name] = Alloca;
                varStack.push_back({node->params->decls[i]->name, Alloca});
            }
        }
        if (node->body)
        {
            node->body->accept(this);
        }
        // print LLVM IR code
        module->print(llvm::errs(), nullptr);
        remove_decls_from_scope();
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
        if(node->type){
            node->type->accept(this);
        }
        llvm::AllocaInst *x = builder->CreateAlloca(inferred_type, nullptr, node->name);
        if (node->init)
        {
            node->init->accept(this);
        }
        builder->CreateStore(inferred_value, x);
        varDeclSymbolTable[node->name] = x;
        varStack.push_back({node->name, x});
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
        routine_vars_n ++;
        std::cout << "I'm adding var " << node->name << '\n';
        if(node->type){
            node->type->accept(this);
        }
        llvm::AllocaInst *x = builder->CreateAlloca(inferred_type, nullptr, node->name);
        if (node->init)
        {
            node->init->accept(this);
        }
        builder->CreateStore(inferred_value, x);
        varDeclSymbolTable[node->name] = x;
        varStack.push_back({node->name, x});
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
        // return input argument
        builder->CreateRet(inferred_value);
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
        inferred_type = llvm::Type::getInt32Ty(context);
    };
    void codeGenerator::visitRealType(ast::RealType *node)
    {
        inferred_type = llvm::Type::getDoubleTy(context);
    }
    void codeGenerator::visitBooleanType(ast::BooleanType *node)
    {
        inferred_type = llvm::Type::getInt1Ty(context);
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
        llvm::Value *value1 = inferred_value;
        if (node->right)
        {
            node->right->accept(this);
        }
        llvm::Value *value2 = inferred_value;
        computeExpressionValue(value1, value2, node->op);
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
        std::cout<<"node value " << node->value << '\n';
        inferred_value = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), node->value);
        expected_type = new ast::IntegerType();
    };
    void codeGenerator::visitRealValue(ast::RealValue *node)
    {
        std::cout<<"node value " << node->value << '\n';
        inferred_value = llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), node->value);
        expected_type = new ast::RealType();
    };
    void codeGenerator::visitBooleanValue(ast::BooleanValue *node)
    {
        std::cout<<"node value " << node->value << '\n';
        inferred_value = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), node->value);
        expected_type = new ast::BooleanType();
    };

    void codeGenerator::visitVar(ast::Var *node)
    {
        llvm::AllocaInst* varAlloc = varDeclSymbolTable[node->name];
        inferred_value = builder->CreateLoad(varAlloc->getAllocatedType(), varAlloc, node->name.c_str());
        // you need to set expected type :)
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
    void codeGenerator::computeExpressionValue(llvm::Value* value1, llvm::Value* value2, BinaryOperator oper){
          if(dynamic_cast<ast::IntegerType*>(expected_type)){
                computeIntExprValue(value1, value2, oper);
          }
          else
          if(dynamic_cast<ast::RealType*>(expected_type)){
            computeRealExprValue(value1, value2, oper);
          }
          else{
            std::cout<<"ERROR: BOOLEAN EXPRESSION\n";
          }
    }
    void codeGenerator::computeIntExprValue(llvm::Value* value1, llvm::Value* value2, BinaryOperator oper){
          switch (oper){
            case BinaryOperator::Plus: 
                inferred_value = builder->CreateAdd(value1, value2, "result");
                break;
            case BinaryOperator::Minus:
                inferred_value = builder->CreateSub(value1, value2, "result");
                break;
            case BinaryOperator::Mul:
                inferred_value = builder->CreateMul(value1, value2, "result");
                break;
            case BinaryOperator::Div:
                inferred_value = builder->CreateSDiv(value1, value2, "result");
                break;
            case BinaryOperator::Mod:
                inferred_value = builder->CreateSRem(value1, value2, "result");
                break;
            default:
                std::cerr << "Undefiend binary expression!!\n";
                break;
        }
    }
    void codeGenerator::computeRealExprValue(llvm::Value* value1, llvm::Value* value2, BinaryOperator oper){
          switch (oper){
            case BinaryOperator::Plus: 
                inferred_value = builder->CreateFAdd(value1, value2, "result");
                break;
            case BinaryOperator::Minus:
                inferred_value = builder->CreateFSub(value1, value2, "result");
                break;
            case BinaryOperator::Mul:
                inferred_value = builder->CreateFMul(value1, value2, "result");
                break;
            case BinaryOperator::Div:
                inferred_value = builder->CreateFDiv(value1, value2, "result");
                break;
            case BinaryOperator::Mod:
                inferred_value = builder->CreateFRem(value1, value2, "result");
                break;
            default:
                std::cerr << "Undefiend binary expression!!\n";
                break;
        }
    }
    void codeGenerator::remove_decls_from_scope(){
            while(routine_vars_n -- ){
                if(varStack.size()){
                    std::string delVar = varStack[varStack.size()-1].first;
                    std::cout<<"I'm deleting var " << delVar << '\n';
                    varDeclSymbolTable.erase(delVar);
                    varStack.pop_back();
                    llvm::AllocaInst* shadowed_i = nullptr;
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
        }
} // namespace generator