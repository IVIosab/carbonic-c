#include "lexer.h"
#include "driver.hpp"
#include "parser.hpp"
extern ast::node_ptr<ast::Program> program;
extern ast::TypeEnum enu;
std::ostream& operator<<(std::ostream& out, ast::node_ptr<ast::Expression> value);
std::ostream& operator<<(std::ostream& out, const ast::node_ptr<ast::VariableDeclaration> value);
std::ostream& operator<<(std::ostream& out, const std::shared_ptr<ast::IntLiteral> vl){
            return out << vl->value;
}
std::ostream& operator<<(std::ostream& out, const std::shared_ptr<ast::BoolLiteral> vl){
            return out << vl->value;
}
std::ostream& operator<<(std::ostream& out, const std::shared_ptr<ast::DoubleLiteral> vl){
            return out << vl->value;
}
std::ostream& operator<<(std::ostream& out, const std::shared_ptr<ast::Identifier> vl){
            return out << vl->name;
}

std::ostream& operator<<(std::ostream& out, const std::shared_ptr<ast::ModifiablePrimary> vl){
            // TODO: print the vector of nested access too.
            return out << vl->identifier;
}
std::ostream& operator<<(std::ostream& out, const std::shared_ptr<ast::ComparisonExpression> vl){
        std::string comparisonOperations [] = {"=", "/=", ">=", ">", "<", "<="};
        auto l = vl->lhs;
        auto r = vl->rhs;
        auto o = comparisonOperations[vl->op];
        auto type = "";
        return out << l <<" "<< o << " " << r;
}
std::ostream& operator<<(std::ostream& out, const std::shared_ptr<ast::BinaryExpression> vl){
        std::string binaryOperations[] = {"+","-","*","/","^","%"};
        auto l = vl->lhs;
        auto r = vl->rhs;
        auto o = binaryOperations[vl->op];
        return out << l <<" "<< o << " " << r;
}
std::ostream& operator<<(std::ostream& out, const std::shared_ptr<ast::BitwiseExpression> vl){
        std::string bitwiseOperations[] = {"and", "or", "xor"};
        auto l = vl->lhs;
        auto r = vl->rhs;
        auto o = bitwiseOperations[vl->op];
        return out << l <<" "<< o << " " << r;
}
template< class T, class U >
std::shared_ptr<T> dynamic_pointer_cast( const std::shared_ptr<U>& r ) noexcept
{
    if (auto p = dynamic_cast<typename std::shared_ptr<T>::element_type*>(r.get()))
        return std::shared_ptr<T>{r, p};
    else
        return std::shared_ptr<T>{};
}
std::ostream& operator<<(std::ostream& out, ast::node_ptr<ast::Expression> value){
    auto Ref0 = std::dynamic_pointer_cast<ast::IntLiteral>(value);
    if(Ref0)
        return out << Ref0;
    auto Ref1 = std::dynamic_pointer_cast<ast::BoolLiteral>(value);
    if(Ref1)
        return out << Ref1;
    auto Ref2 = std::dynamic_pointer_cast<ast::DoubleLiteral>(value);
    if(Ref2){
        return out << Ref2;
    }
    auto Ref3 = std::dynamic_pointer_cast<ast::ModifiablePrimary>(value);
    if(Ref3){
        return out << Ref3;
    }
    auto Ref4 = std::dynamic_pointer_cast<ast::ComparisonExpression>(value);
    if(Ref4){
        return out << Ref4;
    }
    auto Ref5 = std::dynamic_pointer_cast<ast::BinaryExpression>(value);
    if(Ref5){
        return out << Ref5;
    }
    auto Ref6 = std::dynamic_pointer_cast<ast::BitwiseExpression>(value);
    if(Ref6){
        return out << Ref6;
    }
    return out << -1;
}
std::ostream& operator<<(std::ostream& out, ast::node_ptr<ast::Type>& value){
    std::string types[] = {"integer", "real", "boolean", "array", "record"};
    auto enumValue = value->getType();
    std::string s = types[enumValue];
    if (s == "array"){
        auto Ref0 = std::dynamic_pointer_cast<ast::ArrayType>(value);
        return out << s <<" "<< Ref0->dtype << " " << Ref0->size;
    }
    else 
    if(s== "record"){
        std::cout<<"record {\n";
        auto Ref0 = std::dynamic_pointer_cast<ast::RecordType>(value);
        for(int i = 0; i < Ref0->fields.size(); i++){
            std::cout << Ref0->fields[i] << '\n';
        }
        std::cout<<"}";
        return out << '\n';
    }
    return out << s;
}
std::ostream& operator<<(std::ostream& out, const ast::node_ptr<ast::VariableDeclaration> value){
    return out << value->dtype << " " << value->name <<" = "<<  value->initial_value;
}
std::ostream& operator<<(std::ostream& out, const ast::node_ptr<ast::TypeDeclaration> value){
    return out << value->dtype << " " << value->name;
}

int main(int argc, char **argv)
{
    carbonic_c::Driver driver;
    // while (lexer->yylex() != 0)
    // driver.parse_args(argc, argv);
    int x = driver.parse_program();
    
    std::cout<<program->routines.size() << std::endl;
    std::cout<<program->variables.size() << std::endl;
    for (int i =0; i < program->variables.size(); i++){
        std::cout<<program->variables[i]<< '\n';
    }
    std::cout << program->routines[0]->body->variables[0] << '\n';
    return 0;
}