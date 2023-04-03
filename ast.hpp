#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include <algorithm>
#include <variant>
// Forward declarations
namespace ast
{
    struct Node;
    struct Program;
    struct Type;
    struct Expression;
    struct BinaryExpression;
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
    struct VariableDeclaration;
    struct TypeDeclaration;
    struct ModifiablePrimary;
    struct RoutineDeclaration;
    struct RoutineCall;
    struct Body;
    struct Statement;
    struct Assignment;
    struct Print;
    struct IfStatement;
    struct WhileLoop;
    struct ForLoop;
    struct Return;
    // struct ForeachLoop;
} // namespace ast

// Base class for code generator and anything that traverses AST.

namespace ast
{
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
        virtual void visit(ast::BinaryExpression *binexp) = 0;
        virtual void visit(ast::BitwiseExpression *bitexp) = 0;
        virtual void visit(ast::ComparisonExpression *comexp) = 0;
        virtual void visit(ast::VariableDeclaration *vardecl) = 0;
        virtual void visit(ast::TypeDeclaration *typedecl) = 0;
        virtual void visit(ast::RoutineDeclaration *routdecl) = 0;
        virtual void visit(ast::RoutineCall *routcall) = 0;
        virtual void visit(ast::Body *body) = 0;
        virtual void visit(ast::Assignment *assign) = 0;
        virtual void visit(ast::Print *stmt) = 0;
        virtual void visit(ast::Return *stmt) = 0;
        //        virtual void visit(ast::Identifier *id) = 0;
        virtual void visit(ast::ModifiablePrimary *mp) = 0;
        virtual void visit(ast::IfStatement *is) = 0;
        virtual void visit(ast::WhileLoop *wl) = 0;
        virtual void visit(ast::ForLoop *fl) = 0;
        // virtual void visit(ast::ForeachLoop *fel) = 0;
    };
}

namespace ast
{

    // Pointer to an AST node.
    template <typename Node>
    using node_ptr = std::shared_ptr<Node>;
    typedef std::variant<node_ptr<ast::Expression>, std::string> nestedAccess;

    // Enumerations
    enum TypeEnum
    {
        INT,
        DOUBLE,
        BOOL,
        ARRAY,
        RECORD
    };
    enum BinaryOperatorEnum
    {
        PLUS,
        MINUS,
        MUL,
        DIV,
        POW,
        MOD
    };
    enum BitwiseOperatorEnum
    {
        AND,
        OR,
        XOR
    };
    enum ComparisonOperatorEnum
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
        std::vector<node_ptr<TypeDeclaration>> types;
        std::vector<node_ptr<VariableDeclaration>> variables;
        std::vector<node_ptr<RoutineDeclaration>> routines;

        void accept(Visitor *v) override { v->visit(this); }
    };

    // Base class for Types
    struct Type : Node
    {
        virtual TypeEnum getType() { return ast::TypeEnum::INT; }
        virtual void accept(Visitor *v) = 0;
        // friend std::ostream& operator<<(std::ostream& os, const node_ptr<Type> value);
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

    // struct Identifier : Expression
    // {
    //     std::string name;
    //     node_ptr<Expression> idx;

    //     // variable or record field access
    //     Identifier(std::string name)
    //     {
    //         this->name = name;
    //         this->idx = nullptr;
    //     }

    //     // array element access
    //     Identifier(std::string name, node_ptr<Expression> idx)
    //     {
    //         this->name = name;
    //         this->idx = idx;
    //     }

    //     void accept(Visitor *v) override { v->visit(this); }
    // };

    // </Expressions>
    // <Nodes>

    // <Simple Declaration>

    struct VariableDeclaration : Node
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
            this->dtype = nullptr;
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
    struct TypeDeclaration : Node
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
    struct ModifiablePrimary : Expression
    {
        std::string name;
        std::vector<nestedAccess> accessValues;

        ModifiablePrimary(std::string name, std::vector<nestedAccess> accessValues)
        {
            this->name = name;
            std::reverse(accessValues.begin(), accessValues.end());
            this->accessValues = accessValues;
        }

        // ModifiablePrimary(std::string identifier)
        // {
        //      this->identifier = identifier;
        //      this->accessValues = {};
        // }
        // ModifiablePrimary(std::string name, node_ptr<Expression> expression)
        // {
        //     this->name = name;
        //     this->expression = expression;
        //     this->argname = "";
        // }

        // ModifiablePrimary(std::string name, std::string argname)
        // {
        //     this->name = name;
        //     this->argname = argname;
        // }

        void accept(Visitor *v) override { v->visit(this); }
    };
    // Base class for Statements
    struct Statement : virtual Node
    {
        void accept(Visitor *v) override = 0;
    };
    struct Assignment : Statement
    {
        node_ptr<ModifiablePrimary> modifiablePrimary;
        node_ptr<Expression> expression;
        Assignment(node_ptr<ModifiablePrimary> modifiablePrimary, node_ptr<Expression> expression)
        {
            this->modifiablePrimary = modifiablePrimary;
            this->expression = expression;
        }
        void accept(Visitor *v) override { v->visit(this); }
    };
    struct Print : Statement
    {
        node_ptr<Expression> exp;
        bool endl;

        Print(node_ptr<Expression> exp)
        {
            this->exp = exp;
            this->endl = false;
        }
        Print(bool endl)
        {
            this->exp = nullptr;
            this->endl = endl;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };
    struct IfStatement : Statement
    {
        node_ptr<Expression> condition;
        node_ptr<Body> ifBody, elseBody;
        IfStatement(node_ptr<Expression> condition, node_ptr<Body> ifBody)
        {
            this->condition = condition;
            this->ifBody = ifBody;
            this->elseBody = nullptr;
        }
        IfStatement(node_ptr<Expression> condition, node_ptr<Body> ifBody, node_ptr<Body> elseBody)
        {
            this->condition = condition;
            this->ifBody = ifBody;
            this->elseBody = elseBody;
        }
        void accept(Visitor *v) override { v->visit(this); }
    };
    struct WhileLoop : Statement
    {
        node_ptr<Expression> condition;
        node_ptr<Body> loopBody;
        WhileLoop(node_ptr<Expression> condition, node_ptr<Body> loopBody)
        {
            this->condition = condition;
            this->loopBody = loopBody;
        }
        void accept(Visitor *v) override { v->visit(this); }
    };
    struct ForLoop : Statement
    {
        std::string identifier;
        node_ptr<Expression> from, to;
        node_ptr<Body> loopBody;
        ForLoop(std::string identifier, node_ptr<Expression> from, node_ptr<Expression> to, node_ptr<Body> loopBody)
        {
            this->identifier = identifier;
            this->from = from;
            this->to = to;
            this->loopBody = loopBody;
        }
        void accept(Visitor *v) override { v->visit(this); }
    };
    // struct ForeachLoop : Statement
    // {
    //     std::string identifier;
    //     node_ptr<ModifiablePrimary> modifiablePrimary;
    //     node_ptr<Body> loopBody;
    //     ForeachLoop(std::string identifier, node_ptr<ModifiablePrimary> modifablePrimary, node_ptr<Body> loopBody)
    //     {
    //         this->identifier = identifier;
    //         this->modifiablePrimary = modifablePrimary;
    //         this->loopBody = loopBody;
    //     }
    //     void accept(Visitor *v) override { v->visit(this); }
    // };
    struct Return : Statement
    {
        node_ptr<Expression> exp;

        Return()
        {
            this->exp = nullptr;
        }

        Return(node_ptr<Expression> exp)
        {
            this->exp = exp;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };
    struct Body : Node
    {
        std::vector<node_ptr<VariableDeclaration>> variables;
        std::vector<node_ptr<Statement>> statements;

        Body(std::vector<node_ptr<VariableDeclaration>> variables, std::vector<node_ptr<Statement>> statements)
        {
            std::reverse(variables.begin(), variables.end());
            std::reverse(variables.begin(), variables.end());
            this->variables = variables;
            this->statements = statements;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };
    struct RoutineDeclaration : Node
    {
        std::string name;
        std::vector<node_ptr<VariableDeclaration>> params;
        node_ptr<Type> rtype;
        node_ptr<Body> body;

        RoutineDeclaration(std::string name, std::vector<node_ptr<VariableDeclaration>> params, node_ptr<Type> rtype, node_ptr<Body> body)
        {
            this->name = name;
            this->params = params;
            this->rtype = rtype;
            this->body = body;
        }

        RoutineDeclaration(std::string name, std::vector<node_ptr<VariableDeclaration>> params, node_ptr<Body> body)
        {
            this->name = name;
            this->params = params;
            this->body = body;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct RoutineCall : Statement, Expression
    {
        // node_ptr<RoutineDeclaration> routine;
        std::string name;
        std::vector<node_ptr<Expression>> args;

        RoutineCall(std::string name, std::vector<node_ptr<Expression>> args)
        {
            this->name = name;
            this->args = args;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };
    // </Statements>
} // namespace ast

#endif // AST_H