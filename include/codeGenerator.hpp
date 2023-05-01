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
namespace generator{
    class codeGenerator : public ast::Visitor 
    {
    public:
        codeGenerator() : builder(context) {}
        void visitProgram(ast::Program *p);
        void visitDecl(ast::Decl *p){}
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
        void visitExprList(ast::ExprList *p){}
        void visitBinaryExpr(ast::BinaryExpr *p);
        void visitLogicExpr(ast::LogicExpr *p);
        void visitComparisonExpr(ast::ComparisonExpr *p);
        void visitValue(ast::Value *p) {}
        void visitIntegerValue(ast::IntegerValue *p);
        void visitRealValue(ast::RealValue *p);
        void visitBooleanValue(ast::BooleanValue *p);
        void visitRoutineCallValue(ast::RoutineCallValue *p);
        
        void visitType(ast::Type *p){}
        void visitPrimitiveType(ast::PrimitiveType *p){}
        void visitUserType(ast::UserType *p){}
        void visitTypeIdentifier(ast::TypeIdentifier *p){} //-> doesn't get visited
        void visitIntegerType(ast::IntegerType *p);
        void visitRealType(ast::RealType *p);
        void visitBooleanType(ast::BooleanType *p);
        void visitArrayType(ast::ArrayType *p);
        void visitRecordType(ast::RecordType *p);
        void visitInteger(ast::Integer x){} //  -> doesn't get visited
        void visitReal(ast::Real x){} // -> doesn't get visited
        void visitBoolean(ast::Boolean x){} //  -> doesn't get visited
        void visitIdent(Ident x){} //  -> doesn't get visited
        
        void visitVar(ast::Var *p);
        void visitNestedAccess(ast::NestedAccess *p) {}
        void visitNestedAccessList(ast::NestedAccessList *p){}
        void visitArrayAccess(ast::ArrayAccess *p);
        void visitRecordAccess(ast::RecordAccess *p);
        
        void visitParameterDecl(ast::ParameterDecl *p); //  -> doesn't get visited
        void visitParameterList(ast::ParameterList *p){}
    private:
        llvm::LLVMContext context;
        llvm::IRBuilder<> builder;
        std::unique_ptr<llvm::Module> module;
        std::map<std::string, llvm::Value*> m_namedValues;
        llvm::TargetMachine* m_targetMachine;
        llvm::Type* realtime_type = nullptr;
        // potentially add getting realtime type function/or realtime_type variable
    };
} // namespace generator
