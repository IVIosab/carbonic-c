#ifndef AST_H
#define AST_H

#include <iostream>
#include <memory>
#include <map>
#include <vector>

// Forward declarations
namespace ast
{
    struct Node;
    struct Program;
    struct Type;
    struct Expression;
    struct BinaryExpression;
    struct BinaryOperator;
    struct BitwiseExpression;
    struct ComparisonExpression;
    struct Identifier;
    struct IntType;
    struct DoubleType;
    struct BoolType;
    struct ArrayType;
    struct RecordType;
    struct IntLiteral;
    struct DoubleLiteral;
    struct BoolLiteral;
    // struct ExpressionList;
    struct SimpleDeclaration;
    struct VariableDeclaration;
    struct TypeDeclaration;
} // namespace ast

// Base class for code generator and anything that traverses AST.
class Visitor
{
public:
    virtual void visit(ast::Program *program) = 0;
    virtual void visit(ast::IntType *it) = 0;
    virtual void visit(ast::DoubleType *dt) = 0;
    virtual void visit(ast::BoolType *bt) = 0;
    virtual void visit(ast::ArrayType *at) = 0;
    virtual void visit(ast::RecordType *rt) = 0;
    virtual void visit(ast::IntLiteral *il) = 0;
    virtual void visit(ast::DoubleLiteral *il) = 0;
    virtual void visit(ast::BoolLiteral *il) = 0;
    // virtual void visit(ast::ExpressionList *el) = 0;
    virtual void visit(ast::BinaryExpression *binexp) = 0;
    virtual void visit(ast::BinaryOperator *binop) = 0;
    virtual void visit(ast::BitwiseExpression *bitexp) = 0;
    virtual void visit(ast::ComparisonExpression *comexp) = 0;
    virtual void visit(ast::VariableDeclaration *vardecl) = 0;
    virtual void visit(ast::TypeDeclaration *typedecl) = 0;
    virtual void visit(ast::Identifier *id) = 0;
};

namespace ast
{

    // Pointer to an AST node.
    template <typename Node>
    using node_ptr = std::shared_ptr<Node>;

    // Enumerations
    enum class TypeEnum
    {
        INT,
        DOUBLE,
        BOOL,
        ARRAY,
        RECORD
    };
    enum class BinaryOperatorEnum
    {
        PLUS,
        MINUS,
        MUL,
        DIV,
        POW,
        MOD
    };
    enum class BitwiseOperatorEnum
    {
        AND,
        OR,
        XOR
    };
    enum class ComparisonOperatorEnum
    {
        CEQ,
        CNEQ,
        CGE,
        CGT,
        CLT,
        CLE
    };
    // Base class for AST nodes
    struct Node
    {
        virtual void accept(Visitor *v) = 0;
    };

    // A special node containing program variables, type aliases, and routines.
    struct Program : Node
    {
        // std::vector<node_ptr<VariableDeclaration>> variables;
        // std::map<std::string, node_ptr<Type>> types;
        // std::vector<node_ptr<RoutineDeclaration>> routines;

        void accept(Visitor *v) override { v->visit(this); }
    };

    // Base class for Types
    struct Type : Node
    {
        virtual TypeEnum getType() { return TypeEnum::INT; };
        virtual void accept(Visitor *v) = 0;
    };

    // <Types>

    struct IntType : Type
    {
        IntType() {}
        TypeEnum getType() { return TypeEnum::INT; }

        void accept(Visitor *v) override { v->visit(this); }
    };
    struct BoolType : Type
    {
        BoolType() {}
        TypeEnum getType() { return TypeEnum::BOOL; }

        void accept(Visitor *v) override { v->visit(this); }
    };
    struct DoubleType : Type
    {
        DoubleType() {}
        TypeEnum getType() { return TypeEnum::DOUBLE; }

        void accept(Visitor *v) override { v->visit(this); }
    };
    struct ArrayType : Type
    {
        node_ptr<Expression> size;
        node_ptr<Type> dtype;

        ArrayType(node_ptr<Expression> size, node_ptr<Type> dtype)
        {
            this->size = size;
            this->dtype = dtype;
        }

        TypeEnum getType() { return TypeEnum::ARRAY; }

        void accept(Visitor *v) override { v->visit(this); }
    };
    struct RecordType : Type
    {
        std::string name;
        std::vector<node_ptr<VariableDeclaration>> fields;

        RecordType(std::vector<node_ptr<VariableDeclaration>> fields)
        {
            this->fields = fields;
        }

        TypeEnum getType() { return TypeEnum::RECORD; }

        void accept(Visitor *v) override { v->visit(this); }
    };
    // </Types>
    // <Expressions>
    // Base class for Expressions
    struct Expression : Node
    {
        node_ptr<Type> dtype;
    };
    struct BinaryExpression : Expression
    {
        node_ptr<Expression> lhs, rhs;
        BinaryOperatorEnum op;

        BinaryExpression(node_ptr<Expression> lhs, BinaryOperatorEnum op, node_ptr<Expression> rhs)
        {
            this->lhs = lhs;
            this->rhs = rhs;
            this->op = op;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct BitwiseExpression : Expression
    {
        node_ptr<Expression> lhs, rhs;
        BitwiseOperatorEnum op;

        BitwiseExpression(node_ptr<Expression> lhs, BitwiseOperatorEnum op, node_ptr<Expression> rhs)
        {
            this->lhs = lhs;
            this->rhs = rhs;
            this->op = op;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct ComparisonExpression : Expression
    {
        node_ptr<Expression> lhs, rhs;
        ComparisonOperatorEnum op;

        ComparisonExpression(node_ptr<Expression> lhs, ComparisonOperatorEnum op, node_ptr<Expression> rhs)
        {
            this->lhs = lhs;
            this->rhs = rhs;
            this->op = op;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct IntLiteral : Expression
    {
        int64_t value;

        IntLiteral(int64_t value)
        {
            this->dtype = std::make_shared<IntType>();
            this->value = value;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct DoubleLiteral : Expression
    {
        double value;

        DoubleLiteral(double value)
        {
            this->dtype = std::make_shared<DoubleType>();
            this->value = value;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct BoolLiteral : Expression
    {
        bool value;

        BoolLiteral(bool value)
        {
            this->dtype = std::make_shared<BoolType>();
            this->value = value;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    // struct ExpressionList : Expression
    // {
    //     std::vector<node_ptr<Expression>> elements;

    //     ExpressionList(std::vector<node_ptr<Expression>> elements)
    //     {
    //         this->dtype = std::make_shared<ArrayType>();
    //         this->elements = elements;
    //     }

    //     void accept(Visitor *v) override { v->visit(this); }
    // };

    struct Identifier : Expression
    {
        std::string name;
        node_ptr<Expression> idx;

        // variable or record field access
        Identifier(std::string name)
        {
            this->name = name;
        }

        // array element access
        Identifier(std::string name, node_ptr<Expression> idx)
        {
            this->name = name;
            this->idx = idx;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    // </Expressions>
    // <Nodes>

    // <Simple Declaration>

    struct SimpleDeclaration : Node
    {
        virtual void accept(Visitor *v) = 0;
    };
    struct VariableDeclaration : SimpleDeclaration
    {
        std::string name;
        node_ptr<Type> dtype;
        node_ptr<Expression> initial_value;

        VariableDeclaration(std::string name, node_ptr<Type> dtype)
        {
            this->name = name;
            this->dtype = dtype;
            this->initial_value = nullptr;
        }

        VariableDeclaration(std::string name, node_ptr<Expression> initial_value)
        {
            this->name = name;
            this->dtype = initial_value->dtype;
            this->initial_value = initial_value;
        }

        VariableDeclaration(std::string name, node_ptr<Type> dtype, node_ptr<Expression> initial_value)
        {
            this->name = name;
            this->dtype = dtype;
            this->initial_value = initial_value;
        }

        void accept(Visitor *v) { v->visit(this); }
    };
    struct TypeDeclaration : SimpleDeclaration
    {
        std::string name;
        node_ptr<Type> dtype;

        TypeDeclaration(std::string name, node_ptr<Type> dtype)
        {
            this->name = name;
            this->dtype = dtype;
        }
        void accept(Visitor *v) { v->visit(this); }
    };
    // </Statements>
} // namespace ast

#endif // AST_H