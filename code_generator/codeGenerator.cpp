#include <codeGenerator.hpp>
namespace generator
{
    // done
    void codeGenerator::visitProgram(ast::Program *node)
    {
        module = std::make_unique<llvm::Module>("Program", context);
        printf = module->getOrInsertFunction("printf",
                                             llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(context), llvm::PointerType::get(llvm::Type::getInt8Ty(context), 0), true /* this is var arg func type*/));
        for (auto decl : node->decls)
        {
            if (decl)
            {
                decl->accept(this);
            }
        }
        // print LLVM IR code
        module->print(llvm::errs(), nullptr);
    };
    // todo:
    void codeGenerator::visitTypeDecl(ast::TypeDecl *node)
    {
        if (node->type)
        {
            node->type->accept(this);
        }
    };
    // done
    void codeGenerator::visitRoutineDecl(ast::RoutineDecl *node)
    {
        std::vector<llvm::Type *> paramsVector;
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
        llvm::ArrayRef<llvm::Type *> params(paramsVector);
        if (node->returnType)
        {
            node->returnType->accept(this);
        }
        llvm::Type *ret_type = inferred_type;
        llvm::FunctionType *funcType = llvm::FunctionType::get(ret_type, params, false);
        llvm::Function *func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, -1, node->name, module.get());
        // add function to table
        funTable[node->name] = func;
        // create basic block and set insertion point
        llvm::BasicBlock *bb = llvm::BasicBlock::Create(context, "entry", func);
        builder->SetInsertPoint(bb);
        // create function arguments
        if (node->params)
        {
            for (int i = 0; i < node->params->decls.size(); i++)
            {
                routine_vars_n++;
                llvm::Value *arg = (func->arg_begin() + i);
                arg->setName(paramNames[i]);
                //  Create an alloca for this variable.
                node->params->decls[i]->accept(this);
                llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(func, paramNames[i]);
                //  Store the initial value into the alloca.
                builder->CreateStore(arg, Alloca);
                //  Add arguments to variable symbol table.
                varAllocSymbolTable[paramNames[i]] = Alloca;
                varType[paramNames[i]] = node->params->decls[i]->type;
                varStack.push_back({paramNames[i], Alloca});
                varTypeStack.push_back({paramNames[i], node->params->decls[i]->type});
            }
        }
        if (node->body)
        {
            node->body->accept(this);
        }
        while (routine_vars_n--)
        {
            if (varStack.size())
                removeDeclFromScope(varStack[varStack.size() - 1].first);
        }
    };
    void codeGenerator::visitParameterDecl(ast::ParameterDecl *node)
    {
        if (node->type)
        {
            node->type->accept(this);
        }
    }
    // TODO:
    void codeGenerator::visitGlobalVarDecl(ast::GlobalVarDecl *node)
    {
        if (node->type)
        {
            node->type->accept(this);
        }
        if (node->init)
        {
            node->init->accept(this);
        }
        varType[node->name] = node->type;
        llvm::GlobalVariable *globalVar = new llvm::GlobalVariable(*module, inferred_type, false, llvm::GlobalValue::ExternalLinkage, llvm::dyn_cast<llvm::Constant>(inferred_value), node->name);
    }
    // done
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
        routine_vars_n++;
        curr_local_name = node->name;
        if (node->type)
        {
            node->type->accept(this);
        }
        llvm::AllocaInst *x = builder->CreateAlloca(inferred_type, nullptr, node->name);
        if (node->init)
        {
            node->init->accept(this);
            builder->CreateStore(inferred_value, x);
        }
        varAllocSymbolTable[node->name] = x;
        varType[node->name] = node->type;
        varStack.push_back({node->name, x});
        varTypeStack.push_back({node->name, node->type});
    };
    void codeGenerator::visitAssignment(ast::Assignment *node)
    {

        if (node->expr)
        {
            node->expr->accept(this);
        }
        if (node->var->accesses)
        {

            if (node->var->accesses->accesses.size())
            {

                if (auto recAccess = dynamic_cast<ast::RecordAccess *>(node->var->accesses->accesses[0]))
                {

                    recordMembers[node->var->name][recAccess->name].second = inferred_value;
                    return;
                }

                auto arrAccess = dynamic_cast<ast::ArrayAccess *>(node->var->accesses->accesses[0]);
                llvm::Value *rhs = inferred_value;
                arrAccess->index->accept(this);
                int index_int = 0;
                llvm::ConstantInt *myConstantInt = llvm::dyn_cast<llvm::ConstantInt>(inferred_value);
                if (myConstantInt)
                {
                    int64_t IntegerValue = myConstantInt->getSExtValue();
                    index_int = IntegerValue;
                }
                else 
                {
                    std::cerr<<"Error: Unable to resolve array index to a constant int\n";
                }
                nameToArray[node->var->name][index_int] = rhs;
                return;
            }

            llvm::AllocaInst *curr_alloca = varAllocSymbolTable[node->var->name];
            builder->CreateStore(inferred_value, curr_alloca);
        }
    };
    void codeGenerator::visitRoutineCall(ast::RoutineCall *node)
    {
        llvm::Function *func = funTable[node->name];
        std::vector<llvm::Value *> args;
        if (node->args)
        {
            for (int i = 0; i < node->args->exprs.size(); i++)
            {
                node->args->exprs[i]->accept(this);
                args.push_back(inferred_value);
            }
            llvm::ArrayRef argsRef(args);
            llvm::CallInst *call = builder->CreateCall(func, argsRef, func->getName());
        }
        else
        {
            llvm::CallInst *call = builder->CreateCall(func, {}, func->getName());
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
        llvm::Function *func = builder->GetInsertBlock()->getParent();
        llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(context, "then", func);
        llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(context, "else", func);
        llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(context, "end", func);
        if (node->condition)
        {
            node->condition->accept(this);
        }

        llvm::Value *cmp = inferred_value;

        builder->CreateCondBr(cmp, thenBlock, elseBlock);
        if (node->then)
        {
            builder->SetInsertPoint(thenBlock);
            node->then->accept(this);
            builder->CreateBr(endBlock);
            builder->GetInsertBlock();
        }
        builder->SetInsertPoint(elseBlock);
        if (node->else_)
        {
            node->else_->accept(this);
        }
        builder->CreateBr(endBlock);
        builder->GetInsertBlock();
        builder->SetInsertPoint(endBlock);
    };
    void codeGenerator::visitWhileLoop(ast::WhileLoop *node)
    {
        llvm::Function *func = builder->GetInsertBlock()->getParent();
        llvm::BasicBlock *loopCondBB = llvm::BasicBlock::Create(context, "loopCond", func);
        llvm::BasicBlock *loopBodyBB = llvm::BasicBlock::Create(context, "loopBody", func);
        llvm::BasicBlock *loopExitBB = llvm::BasicBlock::Create(context, "loopExit", func);

        builder->CreateBr(loopCondBB);
        builder->SetInsertPoint(loopCondBB);
        if (node->condition)
        {
            node->condition->accept(this);
        }
        llvm::Value *loopCond = inferred_value;
        builder->CreateCondBr(loopCond, loopBodyBB, loopExitBB);

        builder->SetInsertPoint(loopBodyBB);
        if (node->body)
        {
            node->body->accept(this);
        }

        builder->CreateBr(loopCondBB);
        builder->SetInsertPoint(loopExitBB);
    };
    void codeGenerator::visitForLoop(ast::ForLoop *node)
    {
        llvm::Function *func = builder->GetInsertBlock()->getParent();
        llvm::BasicBlock *loopCondBB = llvm::BasicBlock::Create(context, "loopCond", func);
        llvm::BasicBlock *loopBodyBB = llvm::BasicBlock::Create(context, "loopBody", func);
        llvm::BasicBlock *loopExitBB = llvm::BasicBlock::Create(context, "loopExit", func);
        llvm::BasicBlock *loopInc = llvm::BasicBlock::Create(context, "loopInc", func);
        ast::Type *intType = new ast::IntegerType();
        llvm::AllocaInst *iInst = builder->CreateAlloca(builder->getInt32Ty(), nullptr, node->name);
        varAllocSymbolTable[node->name] = iInst;
        varType[node->name] = intType;
        varStack.push_back({node->name, iInst});
        varTypeStack.push_back({node->name, intType});
        llvm::Value *i = iInst;
        llvm::Value *cond = nullptr;
        if (!node->range->reverse)
        {
            node->range->from->accept(this);
            builder->CreateStore(inferred_value, i);
            builder->CreateBr(loopCondBB);
            builder->SetInsertPoint(loopCondBB);
            node->range->to->accept(this);
            cond = builder->CreateICmpSLT(builder->CreateLoad(iInst->getAllocatedType(), i, node->name.c_str()), inferred_value, "loopCond");
        }
        else
        {
            node->range->to->accept(this);
            builder->CreateStore(inferred_value, i);
            builder->CreateBr(loopCondBB);
            builder->SetInsertPoint(loopCondBB);
            node->range->from->accept(this);
            cond = builder->CreateICmpSGT(builder->CreateLoad(iInst->getAllocatedType(), i, node->name.c_str()), inferred_value, "loopCondReverse");
        }

        builder->CreateCondBr(cond, loopBodyBB, loopExitBB);
        builder->SetInsertPoint(loopBodyBB);
        if (node->body)
        {
            node->body->accept(this);
        }
        builder->CreateBr(loopInc);
        builder->SetInsertPoint(loopInc);
        llvm::Value *bodyI = builder->CreateLoad(iInst->getAllocatedType(), i, node->name.c_str());
        llvm::Value *iRes = nullptr;
        if (!node->range->reverse)
        {
            iRes = builder->CreateAdd(bodyI, builder->getInt32(1), "bodyRes");
        }
        else
        {
            iRes = builder->CreateSub(bodyI, builder->getInt32(1), "bodyRes");
        }
        builder->CreateStore(iRes, i);

        builder->CreateBr(loopCondBB);
        builder->SetInsertPoint(loopExitBB);
        // remove i declaration from scope;
        removeDeclFromScope(node->name);
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

        llvm::Value *formatStr;
        auto type = inferred_value->getType();
        if (type->isIntegerTy())
        {
            if (intFmtStr == nullptr)
            {
                intFmtStr = builder->CreateGlobalStringPtr("%d\n", "intFmtString");
            }
            formatStr = intFmtStr;
        }
        else if (type->isDoubleTy())
        {
            if (doubleFmtStr == nullptr)
            {
                doubleFmtStr = builder->CreateGlobalStringPtr("%f\n", "doubleFmtString");
            }
            formatStr = doubleFmtStr;
        }
        else
        {
            // PANIC
            std::cout << "Panicking -- Unkown expression type" << std::endl;
            type->print(llvm::outs(), true);
            std::cout << std::endl;
            throw "Unknown inferred expression type";
        }
        builder->CreateCall(printf, {formatStr, inferred_value}, "printfCall");
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
        std::string array_name = curr_local_name;
        if (node->size)
        {
            node->size->accept(this);
        }
        if (node->type)
        {
            node->type->accept(this);
        }
        int size = 0;
        llvm::ConstantInt *myConstantInt = llvm::dyn_cast<llvm::ConstantInt>(inferred_value);
        if (myConstantInt)
        {
            int64_t IntegerValue = myConstantInt->getSExtValue();
            size = IntegerValue;
        }
        else
        {
            std::cerr << "Error: Unable to resolve array size to a constant int\n";
        }
        inferred_type = llvm::ArrayType::get(inferred_type, size);
        nameToArray[array_name] = {};
    };
    void codeGenerator::visitRecordType(ast::RecordType *node)
    {
        std::vector<llvm::Type *> recordFields;
        std::vector<llvm::Value *> recValues;
        std::string recName = curr_local_name;
        for (auto field : node->decls->vars)
        {
            llvm::Value *inf;
            if (field)
            {
                field->accept(this);
                recordFields.push_back(inferred_type);
                recordMembers[recName][field->name].first = inferred_type;
                recordMembers[recName][field->name].second = inferred_value;
            }
        }

        llvm::StructType *rec = llvm::StructType::create(context, recordFields, curr_local_name);
        nameToRecordValues[recName] = recValues;
        llvm::StructType *recordType = llvm::StructType::create(recordFields, "recordFields");

        inferred_type = recordType;
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
        computeBinaryExprValue(value1, value2, node->op);
    };
    void codeGenerator::visitLogicExpr(ast::LogicExpr *node)
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
        computeLogicExprValue(value1, value2, node->op);
    };
    void codeGenerator::visitComparisonExpr(ast::ComparisonExpr *node)
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
        computeCompExprValue(value1, value2, node->op);
    };
    void codeGenerator::visitIntegerValue(ast::IntegerValue *node)
    {
        inferred_value = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), node->value);
        expected_type = new ast::IntegerType();
    };
    void codeGenerator::visitRealValue(ast::RealValue *node)
    {
        inferred_value = llvm::ConstantFP::get(llvm::Type::getDoubleTy(context), node->value);
        expected_type = new ast::RealType();
    };
    void codeGenerator::visitBooleanValue(ast::BooleanValue *node)
    {
        inferred_value = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), node->value);
        expected_type = new ast::BooleanType();
    };
    void codeGenerator::visitVar(ast::Var *node)
    {
        llvm::AllocaInst *varAlloc = varAllocSymbolTable[node->name];

        if (!varAlloc)
        {
            llvm::GlobalVariable *var = module->getNamedGlobal(node->name);
            inferred_value = builder->CreateLoad(var->getValueType(), var, node->name.c_str());
        }

        else
        {
            inferred_value = builder->CreateLoad(varAlloc->getAllocatedType(), varAlloc, node->name.c_str());
        }
        expected_type = varType[node->name];
        if (node->accesses)
        {
            curr_access_name = node->name;
            for (auto AV : node->accesses->accesses)
            {
                if (AV)
                    AV->accept(this);
            }
        }
    };
    void codeGenerator::visitArrayAccess(ast::ArrayAccess *node)
    {
        std::string arr_access_name = curr_access_name;
        auto arr_type = dynamic_cast<ast::ArrayType *>(expected_type);
        expected_type = arr_type->type;
        llvm::Value *array = inferred_value;
        if (node->index)
        {
            node->index->accept(this);
        }
        int index_int = 0;
        llvm::ConstantInt *myConstantInt = llvm::dyn_cast<llvm::ConstantInt>(inferred_value);
        if (myConstantInt)
        {
            int64_t IntegerValue = myConstantInt->getSExtValue();
            index_int = IntegerValue;
        }
        else
        {
            std::cerr << "Error: Unable to resolve array index to a constant int\n";
        }       
        llvm::Value *elementValue = builder->CreateExtractValue(array, index_int);
        inferred_value = nameToArray[arr_access_name][index_int];
    }
    void codeGenerator::visitRecordAccess(ast::RecordAccess *node)
    {
        auto record_type = dynamic_cast<ast::RecordType *>(expected_type);
        llvm::Value *recordValue = inferred_value;
        unsigned int index = 0;
        for (int i = 0; i < record_type->decls->vars.size(); i++)
        {
            if (record_type->decls->vars[i]->name == node->name)
            {
                index = i;
                break;
            }
        }
        llvm::Value *fieldValue = builder->CreateExtractValue(recordValue, {index});
        inferred_value = recordMembers[curr_access_name][node->name].second;
    }
    void codeGenerator::visitRoutineCallValue(ast::RoutineCallValue *node)
    {
        llvm::Function *func = funTable[node->name];
        std::vector<llvm::Value *> args;
        if (node->args)
        {
            for (int i = 0; i < node->args->exprs.size(); i++)
            {
                node->args->exprs[i]->accept(this);
                args.push_back(inferred_value);
            }
            llvm::ArrayRef argsRef(args);
            llvm::CallInst *call = builder->CreateCall(func, argsRef, func->getName());
            inferred_value = call;
        }
        else
        {
            llvm::CallInst *call = builder->CreateCall(func, {}, func->getName());
            inferred_value = call;
        }
    };
    // Utility functions
    void codeGenerator::computeBinaryExprValue(llvm::Value *value1, llvm::Value *value2, BinaryOperator oper)
    {
        if (dynamic_cast<ast::IntegerType *>(expected_type))
        {
            computeBinaryIntExprValue(value1, value2, oper);
        }
        else if (dynamic_cast<ast::RealType *>(expected_type))
        {
            computeBinaryRealExprValue(value1, value2, oper);
        }
        else
        {
            std::cerr << "ERROR: BOOLEAN EXPRESSION in Binary expression!!\n";
        }
    }
    void codeGenerator::computeBinaryIntExprValue(llvm::Value *value1, llvm::Value *value2, BinaryOperator oper)
    {
        switch (oper)
        {
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
            std::cerr << "Error: Undefiend binary expression!!\n";
            break;
        }
    }
    void codeGenerator::computeBinaryRealExprValue(llvm::Value *value1, llvm::Value *value2, BinaryOperator oper)
    {
        switch (oper)
        {
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
    void codeGenerator::computeLogicExprValue(llvm::Value *value1, llvm::Value *value2, LogicOperator oper)
    {
        switch (oper)
        {
        case LogicOperator::And:
            inferred_value = builder->CreateAnd(value1, value2, "result");
            break;
        case LogicOperator::Or:
            inferred_value = builder->CreateOr(value1, value2, "result");
            break;
        case LogicOperator::Xor:
            inferred_value = builder->CreateXor(value1, value2, "result");
            break;
        default:
            std::cerr << "Error: Undefined logic expression!!\n";
            break;
        }
    }
    void codeGenerator::computeCompExprValue(llvm::Value *value1, llvm::Value *value2, ComparisonOperator oper)
    {
        if (dynamic_cast<ast::IntegerType *>(expected_type))
        {
            computeCompIntExprValue(value1, value2, oper);
        }
        else if (dynamic_cast<ast::RealType *>(expected_type))
        {
            computeCompRealExprValue(value1, value2, oper);
        }
        else
        {
            std::cerr << "ERROR: BOOLEAN EXPRESSION in Binary expression!!\n";
        }
    }
    void codeGenerator::computeCompIntExprValue(llvm::Value *value1, llvm::Value *value2, ComparisonOperator oper)
    {
        switch (oper)
        {
        case ComparisonOperator::Equal:
            inferred_value = builder->CreateICmpEQ(value1, value2, "result");
            break;
        case ComparisonOperator::Greater:
            inferred_value = builder->CreateICmpSGT(value1, value2, "result");
            break;
        case ComparisonOperator::GreaterEqual:
            inferred_value = builder->CreateICmpSGE(value1, value2, "result");
            break;
        case ComparisonOperator::Less:
            inferred_value = builder->CreateICmpSLT(value1, value2, "result");
            break;
        case ComparisonOperator::LessEqual:
            inferred_value = builder->CreateICmpSLE(value1, value2, "result");
            break;
        case ComparisonOperator::NotEqual:
            inferred_value = builder->CreateICmpNE(value1, value2, "result");
            break;
        default:
            std::cerr << "Undefined Comparison expression!!\n";
            break;
        }
    }
    void codeGenerator::computeCompRealExprValue(llvm::Value *value1, llvm::Value *value2, ComparisonOperator oper)
    {
        switch (oper)
        {
        case ComparisonOperator::Equal:
            inferred_value = builder->CreateFCmpOEQ(value1, value2, "result");
            break;
        case ComparisonOperator::Greater:
            inferred_value = builder->CreateFCmpOGT(value1, value2, "result");
            break;
        case ComparisonOperator::GreaterEqual:
            inferred_value = builder->CreateFCmpOGE(value1, value2, "result");
            break;
        case ComparisonOperator::Less:
            inferred_value = builder->CreateFCmpOLT(value1, value2, "result");
            break;
        case ComparisonOperator::LessEqual:
            inferred_value = builder->CreateFCmpOLE(value1, value2, "result");
            break;
        case ComparisonOperator::NotEqual:
            inferred_value = builder->CreateFCmpONE(value1, value2, "result");
            break;
        default:
            std::cerr << "Undefined Comparison expression!!\n";
            break;
        }
    }
    void codeGenerator::removeDeclFromScope(std::string delVar)
    {

            if (varStack.size())
            {
                varAllocSymbolTable.erase(delVar);
                varType.erase(delVar);
                varStack.pop_back();
                varTypeStack.pop_back();
                llvm::AllocaInst *shadowed_i = nullptr;
                for (auto i : varStack)
                {
                    if (i.first == delVar)
                    {
                        shadowed_i = i.second;
                        break;
                    }
                }
                if (shadowed_i)
                    varAllocSymbolTable[delVar] = shadowed_i;
                ast::Type *shadowed_type = nullptr;
                for (auto i : varTypeStack)
                {
                    if (i.first == delVar)
                    {
                        shadowed_type = i.second;
                        break;
                    }
                }
                if (shadowed_type)
                {
                    varType[delVar] = shadowed_type;
                }
            }
    }

    llvm::AllocaInst *codeGenerator::CreateEntryBlockAlloca(llvm::Function *TheFunction, std::string &VarName)
    {
        llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
        return TmpB.CreateAlloca(inferred_type, nullptr, VarName);
    }
} // namespace generator