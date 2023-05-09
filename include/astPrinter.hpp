#include "ast.hpp"
#include <memory>

namespace analyzer
{
    class AstPrinter : public ast::Visitor
    {
    public:
        AstPrinter(size_t depth = 0) : depth(depth) {}
        void visitProgram(ast::Program *p) override;
        void visitDecl(ast::Decl *p) override;
        void visitRoutineDecl(ast::RoutineDecl *p) override;
        void visitGlobalVarDecl(ast::GlobalVarDecl *p) override;
        void visitExpr(ast::Expr *p) override;
        void visitExprList(ast::ExprList *p) override;
        void visitBinaryExpr(ast::BinaryExpr *p) override;
        void visitLogicExpr(ast::LogicExpr *p) override;
        void visitComparisonExpr(ast::ComparisonExpr *p) override;
        void visitValue(ast::Value *p) override;
        void visitIntegerValue(ast::IntegerValue *p) override;
        void visitRealValue(ast::RealValue *p) override;
        void visitBooleanValue(ast::BooleanValue *p) override;
        void visitRoutineCallValue(ast::RoutineCallValue *p) override;
        void visitVar(ast::Var *p) override;
        void visitTypeDecl(ast::TypeDecl *p) override;
        void visitType(ast::Type *p) override;
        void visitPrimitiveType(ast::PrimitiveType *p) override;
        void visitUserType(ast::UserType *p) override;
        void visitTypeIdentifier(ast::TypeIdentifier *p) override;
        void visitIntegerType(ast::IntegerType *p) override;
        void visitRealType(ast::RealType *p) override;
        void visitBooleanType(ast::BooleanType *p) override;
        void visitArrayType(ast::ArrayType *p) override;
        void visitRecordType(ast::RecordType *p) override;
        void visitParameterDecl(ast::ParameterDecl *p) override;
        void visitParameterList(ast::ParameterList *p) override;
        void visitBody(ast::Body *p) override;
        void visitBodyEntity(ast::BodyEntity *p) override;
        void visitLocalVarDecl(ast::LocalVarDecl *p) override;
        void visitLocalVarList(ast::LocalVarList *p) override;
        void visitStatement(ast::Statement *p) override;
        void visitAssignment(ast::Assignment *p) override;
        void visitRoutineCall(ast::RoutineCall *p) override;
        void visitReturn(ast::Return *p) override;
        void visitPrint(ast::Print *p) override;
        void visitWhileLoop(ast::WhileLoop *p) override;
        void visitForLoop(ast::ForLoop *p) override;
        void visitRange(ast::Range *p) override;
        void visitIf(ast::If *p) override;
        void visitNestedAccess(ast::NestedAccess *p) override;
        void visitNestedAccessList(ast::NestedAccessList *p) override;
        void visitArrayAccess(ast::ArrayAccess *p) override;
        void visitRecordAccess(ast::RecordAccess *p) override;

        void visitInteger(ast::Integer x) override;
        void visitReal(ast::Real x) override;
        void visitBoolean(ast::Boolean x) override;
        void visitIdent(ast::Ident x) override;

    private:
        size_t depth;
        void indent();
    };
}