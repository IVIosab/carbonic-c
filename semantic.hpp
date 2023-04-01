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
        // void visit(ast::ForeachLoop *node) override;

    private:
        size_t depth;

        std::unordered_map<std::string, ast::Type *> typeDeclSymbolTable;
        std::unordered_map<std::string, ast::Type *> varDeclSymbolTable;
        std::unordered_map<std::string, ast::Type *> routineDeclSymbolTable;
        std::vector<std::pair<std::string, ast::Type *>> varStack;
        int routine_vars_n = 0;
        ast::Type *actual_type = nullptr;
        void err_second_declaration(std::string name){
            std::cout << "Error: second declaration of " << name << " is invalid.";
            exit(0);
        }
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
        std::string type_to_string(ast::Type *type)
        {
            if (auto type_int = dynamic_cast<ast::IntType *>(type))
                return "Int";
            if (auto type_double = dynamic_cast<ast::DoubleType *>(type))
                return "Double";
            if (auto type_bool = dynamic_cast<ast::BoolType *>(type))
                return "Bool";
            if (auto type_array = dynamic_cast<ast::ArrayType *>(type))
            {
                return "Array of " + type_to_string(&(*type_array->dtype));
            }
            if (auto type_record = dynamic_cast<ast::RecordType *>(type))
            {
                std::string type_string = "Record {";
                for (auto field : type_record->fields)
                {
                    type_string += '\n';
                    type_string += type_to_string(&(*field->dtype));
                }
                type_string += "}";
                return type_string;
            }
            return "undefined";
        }
        void typecheck_types(ast::Type *type1, ast::Type *type2)
        {
            std::string first = type_to_string(type1),
                        second = type_to_string(type2);
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