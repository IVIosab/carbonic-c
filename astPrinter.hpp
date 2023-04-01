#include "ast.hpp"
#include <memory>

namespace analyzer
{

    template <typename T>
    using sPtr = std::shared_ptr<T>;

    class AstPrinter : public ast::Visitor
    {
    public:
        AstPrinter(size_t depth = 0) : depth(depth) {}
        void visit(ast::Program *node) override;
        void visit(ast::IntType *node) override;
        void visit(ast::DoubleType *node) override;
        void visit(ast::BoolType *node) override;
        void visit(ast::ArrayType *node) override;
        void visit(ast::RecordType *node) override;
        void visit(ast::IntLiteral *node) override;
        void visit(ast::DoubleLiteral *node) override;
        void visit(ast::BoolLiteral *node) override;
        void visit(ast::BinaryExpression *node) override;
        void visit(ast::BitwiseExpression *node) override;
        void visit(ast::ComparisonExpression *node) override;
        void visit(ast::VariableDeclaration *node) override;
        void visit(ast::TypeDeclaration *node) override;
        void visit(ast::RoutineDeclaration *node) override;
        void visit(ast::RoutineCall *node) override;
        void visit(ast::Body *node) override;
        void visit(ast::Assignment *node) override;
        void visit(ast::Print *node) override;
        void visit(ast::Return *node) override;
        void visit(ast::ModifiablePrimary *node) override;
        void visit(ast::IfStatement *node) override;
        void visit(ast::WhileLoop *node) override;
        void visit(ast::ForLoop *node) override;
        // void visit(ast::ForeachLoop *node) override;

    private:
        size_t depth;
        void indent();
    };
}