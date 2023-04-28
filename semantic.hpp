#include "ast.hpp"
#include <memory>
#include <unordered_map>
#include "Enums.h"
namespace analyzer
{
    typedef std::string Ident;
    class Semantic : public ast::Visitor
    {
    public:
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
        size_t depth;

        std::unordered_map<std::string, ast::Type *> typeDeclSymbolTable;
        std::unordered_map<std::string, ast::Type *> varDeclSymbolTable;
        std::unordered_map<std::string, ast::RoutineDecl *> routineDeclTable;
        std::vector<std::pair<std::string, ast::Type *>> varStack;
        int routine_vars_n = 0;
        ast::Type *actual_type = nullptr;
        ast::Type* routine_return_type = nullptr;
        ast::Type* current_var_type = nullptr;
        void err_second_declaration(std::string name){
            std::cout << "Error: second declaration of " << name << " is invalid.\n";
            exit(0);
        }
        void err_undefined_obj(std::string obj)
        {
            std::cout << "Error: Undefined object: " << obj << '\n';
            exit(0);
        }

        void err_expected_got(std::string expected, std::string got)
        {
            std::cout << "Error: Expected: " << expected << ", got: " << got << '\n';
            exit(0);
        }
        void err_wrong_params_number(int expected, int got){
            std::cout << "Error: Expected number of params: " << expected << " got: " << got << '\n';
            exit(0);
        }
        void warn_shadow(std::string obj)
        {
            std::cout << "Warning: Shadowing object: " << obj << '\n';
        }
        std::string type_to_string(ast::Type *type)
        {
            if (type == nullptr){
                return "nullptr";
            }
            if (auto type_int = dynamic_cast<ast::IntegerType *>(type))
                return "Integer";
            if (auto type_double = dynamic_cast<ast::RealType *>(type))
                return "Real";
            if (auto type_bool = dynamic_cast<ast::BooleanType *>(type))
                return "Boolean";
            if (auto type_array = dynamic_cast<ast::ArrayType *>(type))
            {
                return "Array of " + type_to_string(&(*type_array->type));
            }
            if (auto type_record = dynamic_cast<ast::RecordType *>(type))
            {
                std::string type_string = "Record {";
                for (auto decl : type_record->decls->vars)
                {
                    type_string += '\n';
                    type_string += type_to_string(&(*decl->type));
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
        // ast::Type* TypePointerToShared(ast::Type* type){
        //     if (auto type_int = dynamic_cast<ast::IntegerType *>(type))
        //         return new ast::IntegerType(*type_int);
        //     if (auto type_double = dynamic_cast<ast::RealType *>(type))
        //         return new ast::RealType(*type_double);
        //     if (auto type_bool = dynamic_cast<ast::BooleanType *>(type))
        //         return new ast::BooleanType(*type_bool);
        //     if (auto type_array = dynamic_cast<ast::ArrayType *>(type))
        //         return new ast::ArrayType(*type_array);
        //     if (auto type_record = dynamic_cast<ast::RecordType *>(type))
        //        return new ast::RecordType(*type_record);
        //     return nullptr;
        // }
        void testing()
        {
            ast::Type *test = new ast::IntegerType();
            auto t = dynamic_cast<ast::IntegerType *>(test);
            if (t)
            {
                std::cout << "GOOOD" << std::endl;
            }
        }
    };
}