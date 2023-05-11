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
        size_t depth;

        std::unordered_map<std::string, ast::Type *> typeDeclSymbolTable;
        std::unordered_map<std::string, ast::Type *> varDeclSymbolTable;
        std::unordered_map<std::string, ast::RoutineDecl *> routineDeclTable;
        std::vector<std::pair<std::string, ast::Type *>> varStack;
        int routine_vars_n = 0;
        ast::Type *actual_type = nullptr;
        ast::Type *routine_return_type = nullptr;
        ast::Type *current_var_type = nullptr;
        void err_second_declaration(std::string name)
        {
            std::cout << "Error: second declaration of " << name << " is invalid.\n";
            exit(1);
        }
        void err_undefined_obj(std::string obj)
        {
            std::cout << "Error: Undefined object: " << obj << '\n';
            exit(1);
        }

        void err_expected_got(std::string expected, std::string got)
        {
            std::cout << "Error: Expected:\n"
                      << expected << ",\ngot: \n"
                      << got << '\n';
            exit(1);
        }
        void err_wrong_params_number(int expected, int got)
        {
            std::cout << "Error: Expected number of params: " << expected << " got: " << got << '\n';
            exit(1);
        }
        void warn_shadow(std::string obj)
        {
            std::cout << "Warning: Shadowing object: " << obj << '\n';
        }
        // Utility functions
        // Print expected/got type
        std::string type_to_string(ast::Type *type)
        {
            if (type == nullptr)
            {
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
                    type_string += decl->name;
                    type_string += ' ';
                    type_string += type_to_string(&(*decl->type));
                }
                type_string += "}";
                return type_string;
            }
            return "undefined";
        }
        // Typecheck types using the overloaded == operator
        void typecheck_types(ast::Type *type1, ast::Type *type2)
        {
            if ((*type1) == (*type2))
                return;
            std::string first = type_to_string(type1),
                        second = type_to_string(type2);
            err_expected_got(first, second);
        }
        void add_param_to_scope(Ident name)
        {
            ast::Type *var_type = actual_type;
            varDeclSymbolTable[name] = var_type;
            varStack.push_back({name, var_type});
            routine_vars_n++;
        }
        // Remove params from scope when exiting a routine declaration
        void remove_params_from_scope()
        {
            while (routine_vars_n--)
            {
                if (varStack.size())
                {
                    std::string delVar = varStack[varStack.size() - 1].first;
                    varDeclSymbolTable.erase(delVar);
                    varStack.pop_back();
                    ast::Type *shadowed_i = nullptr;
                    for (auto i : varStack)
                    {
                        if (i.first == delVar)
                        {
                            shadowed_i = i.second;
                            break;
                        }
                    }
                    if (shadowed_i)
                        varDeclSymbolTable[delVar] = shadowed_i;
                }
            }
        }
        // Check that arguments of routine call match params' types in function decl
        void routineCallCheck(ast::ParameterList *params, ast::ExprList *args)
        {
            for (int i = 0; i < args->exprs.size(); i++)
            {
                if (params->decls[i]->type && args->exprs[i])
                {
                    if (args->exprs[i])
                    {
                        actual_type = nullptr;
                        args->exprs[i]->accept(this);
                    }
                    if (actual_type)
                    {
                        typecheck_types(params->decls[i]->type, actual_type);
                    }
                }
            }
        }
    };
}