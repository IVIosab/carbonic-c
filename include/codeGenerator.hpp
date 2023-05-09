#include "ast.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#pragma GCC diagnostic pop
#include <fstream>
typedef std::string Ident;
namespace generator
{
    class codeGenerator : public ast::Visitor
    {
    public:
        // codeGenerator() {}
        void visitProgram(ast::Program *p);
        void visitDecl(ast::Decl *p) {}
        void visitTypeDecl(ast::TypeDecl *p);
        void visitGlobalVarDecl(ast::GlobalVarDecl *p);
        void visitRoutineDecl(ast::RoutineDecl *p);

        void visitBody(ast::Body *p);
        void visitBodyEntity(ast::BodyEntity *p){};
        void visitLocalVarDecl(ast::LocalVarDecl *p);
        void visitLocalVarList(ast::LocalVarList *p){};
        void visitStatement(ast::Statement *p){};
        void visitAssignment(ast::Assignment *p);
        void visitRoutineCall(ast::RoutineCall *p);
        void visitReturn(ast::Return *p);
        void visitPrint(ast::Print *p);
        void visitIf(ast::If *p);
        void visitWhileLoop(ast::WhileLoop *p);
        void visitForLoop(ast::ForLoop *p);
        void visitRange(ast::Range *p);

        void visitExpr(ast::Expr *p) {}
        void visitExprList(ast::ExprList *p) {}
        void visitBinaryExpr(ast::BinaryExpr *p);
        void visitLogicExpr(ast::LogicExpr *p);
        void visitComparisonExpr(ast::ComparisonExpr *p);
        void visitValue(ast::Value *p) {}
        void visitIntegerValue(ast::IntegerValue *p);
        void visitRealValue(ast::RealValue *p);
        void visitBooleanValue(ast::BooleanValue *p);
        void visitRoutineCallValue(ast::RoutineCallValue *p);

        void visitType(ast::Type *p) {}
        void visitPrimitiveType(ast::PrimitiveType *p) {}
        void visitUserType(ast::UserType *p) {}
        void visitTypeIdentifier(ast::TypeIdentifier *p) {} //-> doesn't get visited
        void visitIntegerType(ast::IntegerType *p);
        void visitRealType(ast::RealType *p);
        void visitBooleanType(ast::BooleanType *p);
        void visitArrayType(ast::ArrayType *p);
        void visitRecordType(ast::RecordType *p);
        void visitInteger(ast::Integer x) {} //  -> doesn't get visited
        void visitReal(ast::Real x) {}       // -> doesn't get visited
        void visitBoolean(ast::Boolean x) {} //  -> doesn't get visited
        void visitIdent(Ident x) {}          //  -> doesn't get visited

        void visitVar(ast::Var *p);
        void visitNestedAccess(ast::NestedAccess *p) {}
        void visitNestedAccessList(ast::NestedAccessList *p) {}
        void visitArrayAccess(ast::ArrayAccess *p);
        void visitRecordAccess(ast::RecordAccess *p);

        void visitParameterDecl(ast::ParameterDecl *p); //  -> doesn't get visited
        void visitParameterList(ast::ParameterList *p) {}

    private:
        llvm::LLVMContext context;
        std::unique_ptr<llvm::Module> module;
        std::unique_ptr<llvm::IRBuilder<>> builder =
            std::unique_ptr<llvm::IRBuilder<>>(new llvm::IRBuilder<>(context));
        llvm::TargetMachine *m_targetMachine;
        llvm::Type *inferred_type = nullptr;
        llvm::Value *inferred_value = nullptr;
        ast::Type *expected_type = nullptr;

        llvm::FunctionCallee printf;
        llvm::Value *intFmtStr = nullptr;
        llvm::Value *doubleFmtStr = nullptr;

        int routine_vars_n = 0;
        std::string curr_local_name;
        std::string curr_access_name = "";

        std::unordered_map<std::string, llvm::AllocaInst *> varAllocSymbolTable;
        std::unordered_map<std::string, ast::Type *> varType;
        std::unordered_map<std::string, std::unordered_map<std::string, std::pair<llvm::Type *, llvm::Value *>>> recordMemembers;
        std::unordered_map<std::string, std::vector<llvm::Value *>> nameToRecordValues;
        std::vector<std::pair<std::string, llvm::AllocaInst *>>
            varStack;
        std::vector<std::pair<std::string, ast::Type *>> varTypeStack;
        std::unordered_map<std::string, llvm::Function *> funTable;

        void computeBinaryExprValue(llvm::Value *value1, llvm::Value *value2, BinaryOperator oper);
        void computeBinaryIntExprValue(llvm::Value *value1, llvm::Value *value2, BinaryOperator oper);
        void computeBinaryRealExprValue(llvm::Value *value1, llvm::Value *value2, BinaryOperator oper);
        void computeLogicExprValue(llvm::Value *value1, llvm::Value *value2, LogicOperator oper);
        void computeCompExprValue(llvm::Value *value1, llvm::Value *value2, ComparisonOperator oper);
        void computeCompIntExprValue(llvm::Value *value1, llvm::Value *value2, ComparisonOperator oper);
        void computeCompRealExprValue(llvm::Value *value1, llvm::Value *value2, ComparisonOperator oper);
        // Remove params from scope when exiting a routine declaration
        void remove_decls_from_scope();
        // reference: https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl07.html
        llvm::AllocaInst *CreateEntryBlockAlloca(llvm::Function *TheFunction, std::string &VarName)
        {
            llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
            return TmpB.CreateAlloca(llvm::Type::getDoubleTy(this->context), nullptr, VarName);
        }
    };
} // namespace generator
