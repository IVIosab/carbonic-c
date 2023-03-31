#include "ast.hpp"
#include <memory>
#include <unordered_map>

namespace analyzer
{

    template <typename T>
    using sPtr = std::shared_ptr<T>;

    class Semantic : public ast::Visitor
    {
    public:
        Semantic(size_t depth = 0) : depth(depth) {}
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
//        void visit(ast::Identifier *node) override;
        void visit(ast::ModifiablePrimary *node) override;
        void visit(ast::IfStatement *node) override;
        void visit(ast::WhileLoop *node) override;
        void visit(ast::ForLoop *node) override;
        void visit(ast::ForeachLoop *node) override;

    private:
        size_t depth;

        std::unordered_map<std::string, ast::Type *> typeDeclSymbolTable;
        std::unordered_map<std::string, ast::Type *> varDeclSymbolTable;
        std::unordered_map<std::string, ast::Type *> routineDeclSymbolTable;
        std::vector< std::pair<std::string, ast::Type*> > varStack;
        ast::Type *actual_type = nullptr;

        void err_undefined_obj(std::string obj)
        {
            std::cout << "Error: Undefined object: " << obj << std::endl;
            exit(0);
        }

        void err_expected_got(std::string got, std::string expected)
        {
            std::cout << "Error: Expected: " << expected << ", got: " << got << std::endl;
            exit(0);
        }

        void warn_shadow(std::string obj)
        {
            std::cout << "Warning: Shadowing object: " << obj << std::endl;
        }

        void typecheck_types(ast::Type *type1, ast::Type *type2)
        {
            std::string first = "", second = "";

            auto ti1 = dynamic_cast<ast::IntType *>(type1);
            if (ti1)
                first = "Int";
            auto td1 = dynamic_cast<ast::DoubleType *>(type1);
            if (td1)
                first = "Double";
            auto tb1 = dynamic_cast<ast::BoolType *>(type1);
            if (tb1)
                first = "Bool";

            auto ti2 = dynamic_cast<ast::IntType *>(type2);
            if (ti2)
                second = "Int";
            auto td2 = dynamic_cast<ast::DoubleType *>(type2);
            if (td2)
                second = "Double";
            auto tb2 = dynamic_cast<ast::BoolType *>(type2);
            if (tb2)
                second = "Bool";

            if (first != second)
                err_expected_got(first, second);
        }

        void testing()
        {
            ast::Type *test = new ast::IntType();
            auto t = dynamic_cast<ast::IntType *>(test);
            if (t)
            {
                std::cout << "GOOOD" << std::endl;
            }
        }
    };
}