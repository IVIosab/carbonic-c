#ifndef AST_H
#define AST_H
// define the abstract syntax tree here
// we'll use the visitor pattern when parsing semantics
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#define first f
#define second s

namespace ast
{
    struct Node;
    struct Program;
    struct Type;
    struct Expression;
    struct UnaryExpression;
    struct BinaryExpression;
    struct BitwiseOperator;
    struct RelationalOperator;
    struct ArithmeticOperator;
    struct Identifier;
    struct Int;
    struct Double;
    struct Bool;
    struct Array;
    struct Record;
    struct IntLiteral;
    struct DoubleLiteral;
    struct BoolLiteral;
    struct Identifier;
    struct ModifiablePrimary;
    struct TypeDecleration;
    struct VariableDeclaration;
    struct RoutineDeclaration;
    struct Body;
    struct Statement;
    struct Return;
    struct Print;
    struct Assignment;
    struct IfStatement;
    struct WhileLoop;
    struct ForLoop;
    struct ForEachLoop;
    struct RoutineCall;
}

class Visitor
{
public:
    virtual void visit(ast::Program *program) = 0;
    virtual void visit(ast::Int *it) = 0;
    virtual void visit(ast::Double *it) = 0;
    virtual void visit(ast::Bool *it) = 0;
    virtual void visit(ast::Array *at) = 0;
    virtual void visit(ast::Record *rt) = 0;
    virtual void visit(ast::IntLiteral *il) = 0;
    virtual void visit(ast::DoubleLiteral *rl) = 0;
    virtual void visit(ast::BoolLiteral *bl) = 0;
    virtual void visit(ast::VariableDeclaration *vardecl) = 0;
    virtual void visit(ast::Identifier *id) = 0;
    virtual void visit(ast::UnaryExpression *exp) = 0;
    virtual void visit(ast::BinaryExpression *exp) = 0;
    virtual void visit(ast::RoutineDeclaration *routine) = 0;
    virtual void visit(ast::Body *body) = 0;
    virtual void visit(ast::Return *stmt) = 0;
    virtual void visit(ast::Print *stmt) = 0;
    virtual void visit(ast::Assignment *stmt) = 0;
    virtual void visit(ast::IfStatement *stmt) = 0;
    virtual void visit(ast::WhileLoop *stmt) = 0;
    virtual void visit(ast::ForLoop *stmt) = 0;
    virtual void visit(ast::RoutineCall *stmt) = 0;
};

namespace ast
{
    // Pointer to AST node
    template <typename Node>
    using node_ptr = std::shared_ptr<Node>;

    // Enumerations
    enum class TypeEnum
    {
        INT,
        DOUBLE,
        BOOL,
        CHAR,
        STRING,
        ARRAY,
        RECORD
    };
    enum class ArithmeticOperatorEnum
    {
        PLUS,
        MINUS,
        MUL,
        DIV,
        MOD,
        POW
    };
    enum class RelationalOperatorEnum
    {
        CEQ,
        CNEQ,
        CGE,
        CGT,
        CLE,
        CLT
    };
    enum class BitwiseOperatorEnum
    {
        AND,
        OR,
        XOR
    };
    enum class TypeEnum
    {
        INT,
        REAL,
        BOOL,
        ARRAY,
        RECORD
    };

    // Base class for AST nodes
    struct Node
    {
        virtual void accept(Visitor *v) = 0;
    };

    // A special node containing program variables, type aliases, and routines.
    struct Program : Node
    {
        std::vector<node_ptr<VariableDeclaration>> variables;
        std::map<std::string, node_ptr<Type>> types;
        std::vector<node_ptr<RoutineDeclaration>> routines;

        void accept(Visitor *v) override { v->visit(this); }
    };

    // Base class for Expressions
    struct Expression : Node
    {
        node_ptr<Type> dtype;
    };

    // Base class for Types
    struct Type : Node
    {
        virtual TypeEnum getType() { return TypeEnum::INT; };
        virtual void accept(Visitor *v) = 0;
    };

    // Base class for Statements
    struct Statement : virtual Node
    {
        void accept(Visitor *v) override = 0;
    };

    // <Types>
    struct Int : Type
    {
        Int() {}
        TypeEnum getType() { return TypeEnum::INT; }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct Double : Type
    {
        Double() {}
        TypeEnum getType() { return TypeEnum::DOUBLE; }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct Bool : Type
    {
        Bool() {}
        TypeEnum getType() { return TypeEnum::BOOL; }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct Array : Type
    {
        node_ptr<Expression> size;
        node_ptr<Type> dtype;

        Array(node_ptr<Expression> size, node_ptr<Type> dtype)
        {
            this->size = size;
            this->dtype = dtype;
        }

        TypeEnum getType() { return TypeEnum::ARRAY; }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct Record : Type
    {
        std::string name; // set by llvm vardecl or typedecl
        std::vector<node_ptr<VariableDeclaration>> fields;

        Record(std::vector<node_ptr<VariableDeclaration>> fields)
        {
            this->fields = fields;
        }

        TypeEnum getType() { return TypeEnum::RECORD; }

        void accept(Visitor *v) override { v->visit(this); }
    };

    // </Types>
    // <Expressions>
    struct UnaryExpression : Expression
    {
        node_ptr<Expression> operand;
        OperatorEnum op;

        UnaryExpression(OperatorEnum op, node_ptr<Expression> operand)
        {
            this->operand = operand;
            this->op = op;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct BinaryExpression : Expression
    {
        node_ptr<Expression> lhs, rhs;
        OperatorEnum op;

        BinaryExpression(node_ptr<Expression> lhs, OperatorEnum op, node_ptr<Expression> rhs)
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
            this->dtype = std::make_shared<Int>();
            this->value = value;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct DoubleLiteral : Expression
    {
        double value;

        DoubleLiteral(double value)
        {
            this->dtype = std::make_shared<Double>();
            this->value = value;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct BoolLiteral : Expression
    {
        bool value;

        BoolLiteral(bool value)
        {
            this->dtype = std::make_shared<Bool>();
            this->value = value;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

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

    struct Body : Node
    {
        std::vector<node_ptr<Statement>> statements;
        std::vector<node_ptr<VariableDeclaration>> variables;
        // std::vector<node_ptr<TypeDeclaration>> types;

        Body(std::vector<node_ptr<VariableDeclaration>> variables, std::vector<node_ptr<Statement>> statements)
        {
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

        RoutineDeclaration(std::string name, std::vector<node_ptr<VariableDeclaration>> params, node_ptr<Body> body, node_ptr<Type> rtype)
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

    // </Nodes>
    // <Statements>
    struct Return : Statement
    {
        node_ptr<Expression> exp;

        Return() {}

        Return(node_ptr<Expression> exp)
        {
            this->exp = exp;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct Print : Statement
    {
        node_ptr<Expression> exp;
        node_ptr<std::string> str;
        bool endl;

        Print(node_ptr<Expression> exp, bool endl = false)
        {
            this->exp = exp;
            this->endl = endl;
        }

        Print(node_ptr<std::string> str, bool endl = false)
        {
            this->str = str;
            this->endl = endl;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct Assignment : Statement
    {
        node_ptr<Identifier> id;
        node_ptr<Expression> exp;

        Assignment(node_ptr<Identifier> id, node_ptr<Expression> exp)
        {
            this->id = id;
            this->exp = exp;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct IfStatement : Statement
    {
        node_ptr<Expression> cond;
        node_ptr<Body> then_body, else_body;

        IfStatement(node_ptr<Expression> cond, node_ptr<Body> then_body)
        {
            this->cond = cond;
            this->then_body = then_body;
        }

        IfStatement(node_ptr<Expression> cond, node_ptr<Body> then_body, node_ptr<Body> else_body)
        {
            this->cond = cond;
            this->then_body = then_body;
            this->else_body = else_body;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct WhileLoop : Statement
    {
        node_ptr<Expression> cond;
        node_ptr<Body> body;

        WhileLoop(node_ptr<Expression> cond, node_ptr<Body> body)
        {
            this->cond = cond;
            this->body = body;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct ForLoop : Statement
    {
        node_ptr<VariableDeclaration> loop_var;
        node_ptr<Expression> cond;
        node_ptr<Body> body;
        node_ptr<Assignment> action;

        ForLoop(node_ptr<VariableDeclaration> loop_var, node_ptr<Expression> cond, node_ptr<Body> body, node_ptr<Assignment> action)
        {
            this->loop_var = loop_var;
            this->cond = cond;
            this->body = body;
            this->action = action;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    struct RoutineCall : Statement, Expression
    {
        node_ptr<RoutineDeclaration> routine;
        std::vector<node_ptr<Expression>> args;

        RoutineCall(node_ptr<RoutineDeclaration> routine, std::vector<node_ptr<Expression>> args)
        {
            this->routine = routine;
            this->args = args;
        }

        void accept(Visitor *v) override { v->visit(this); }
    };

    // /// potentially add scopes
    // typedef std::vector<VariableDeclerationNode> VariableList;
    // typedef std::unordered_map<std::string, TypeNode> TypeMap;
    // typedef std::vector<StatementNode> StatementList;
    // typedef std::vector<RoutineDeclerationNode> RoutineList;
    // typedef std::variant<node_ptr<ExpressionNode>(), node_ptr<IdentifierNode>()> nestedAccess;

    // typedef node_ptr<Node> node;
    // typedef node_ptr<RoutineDeclerationNode> routineDeclerationNode;
    // typedef node_ptr<VariableDeclerationNode> variableDeclerationNode;
    // typedef node_ptr<ExpressionNode> expressionNode;
    // typedef node_ptr<IdentifierNode> identifierNode;
    // typedef std::variant<node_ptr<ExpressionNode>, node_ptr<IdentifierNode>,
    //                      node_ptr<ArithmeticOperatorNode>, node_ptr<RelationalOperatorNode>,
    //                      node_ptr<BitwiseOperatorNode>>
    //     expressionType;

    // class Node
    // {
    //     std::vector<std::shared_ptr<Node>> children();
    // };
    // /// This will hold the entire program tree and the lists of everything declared
    // class Program : Node
    // {
    //     VariableList variableList;
    //     TypeMap typeMap; // I may not be able to use that
    //     RoutineList routineList;
    // };
    // // you may need to override = operator for some classes
    // class TypeDeclerationNode : Node
    // {
    //     std::string typeAlias;
    //     node_ptr<TypeNode> type;
    //     TypeDeclerationNode(std::string typeAlias, node_ptr<TypeNode> type)
    //     {
    //         this->typeAlias = typeAlias;
    //         this->type = type;
    //     }
    // };
    // class TypeNode
    // {
    // public:
    //     TypeNode() {}
    //     virtual typeEnum getType();
    // };
    // class IntNode : TypeNode
    // {
    // public:
    //     IntNode() {}
    //     typeEnum getType()
    //     {
    //         return typeEnum::INT;
    //     }
    // };
    // class DoubleNode : TypeNode
    // {
    // public:
    //     DoubleNode() {}
    //     typeEnum getType()
    //     {
    //         return typeEnum::DOUBLE;
    //     }
    // };
    // class BoolNode : TypeNode
    // {
    // public:
    //     BoolNode() {}
    //     typeEnum getType()
    //     {
    //         return typeEnum::BOOL;
    //     }
    // };
    // class ArrayNode : TypeNode
    // {
    // protected:
    //     node_ptr<TypeNode> arrayType;
    //     node_ptr<ExpressionNode> size;

    // public:
    //     ArrayNode(node_ptr<ExpressionNode> size, node_ptr<TypeNode> arrayType)
    //     {
    //         this->size = size;
    //         this->arrayType = arrayType;
    //     }
    //     typeEnum getType()
    //     {
    //         return typeEnum::ARRAY;
    //     }
    // };
    // class RecordNode : TypeNode
    // {
    // protected:
    //     std::vector<node_ptr<VariableDeclerationNode>> recordVariables;

    // public:
    //     RecordNode(std::vector<node_ptr<VariableDeclerationNode>> recordVariables)
    //     {
    //         this->recordVariables = recordVariables;
    //     }
    //     typeEnum getType()
    //     {
    //         return typeEnum::RECORD;
    //     }
    // };
    // class CharNode : TypeNode
    // {
    // public:
    //     CharNode() {}
    //     typeEnum getType()
    //     {
    //         return typeEnum::CHAR;
    //     }
    // };
    // class StringNode : TypeNode
    // {
    // public:
    //     StringNode() {}
    //     typeEnum getType()
    //     {
    //         return typeEnum::STRING;
    //     }
    // };
    // // TODO: do we infer types here?
    // class VariableDeclerationNode : Node
    // {
    // protected:
    //     node_ptr<TypeNode> variableType;
    //     node_ptr<IdentifierNode> idedntifier;
    //     node_ptr<ExpressionNode> value;

    // public:
    //     VariableDeclerationNode(node_ptr<IdentifierNode> identifier, node_ptr<TypeNode> variableType, node_ptr<ExpressionNode> expression)
    //     {
    //         this->idedntifier = identifier;
    //         this->variableType = variableType;
    //         this->value = expression;
    //     }
    //     VariableDeclerationNode(node_ptr<IdentifierNode> identifier, node_ptr<TypeNode> variableType)
    //     {
    //         this->idedntifier = identifier;
    //         this->variableType = variableType;
    //     }
    //     VariableDeclerationNode(node_ptr<IdentifierNode> identifier, node_ptr<ExpressionNode> expression)
    //     {
    //         this->idedntifier = identifier;
    //         this->value = expression;
    //     }
    //     node_ptr<TypeNode> inferType(node_ptr<ExpressionNode> expression)
    //     {
    //         // expression should potentially have type inference so you only need to imlement it once.
    //     }
    // };
    // class RoutineDeclerationNode : Node
    // {
    // protected:
    //     std::string identifier;
    //     StatementList statementList;
    //     node_ptr<TypeNode> routineType;
    //     std::vector<node_ptr<VariableDeclerationNode>> parameters;

    // public:
    //     RoutineDeclerationNode(std::string identifier, std::vector<node_ptr<VariableDeclerationNode>> parameters,
    //                            node_ptr<TypeNode> routineType, StatementList statementList)
    //     {
    //         this->identifier = identifier;
    //         this->parameters = parameters;
    //         this->routineType = routineType;
    //         this->statementList = statementList;
    //     }
    //     RoutineDeclerationNode(std::string identifier,
    //                            std::vector<node_ptr<VariableDeclerationNode>> parameters, StatementList statementList)
    //     {
    //         this->identifier = identifier;
    //         this->parameters = parameters;
    //         this->routineType = inferType(statementList);
    //         this->statementList = statementList;
    //     }
    //     node_ptr<TypeNode> inferType(StatementList StatementList)
    //     {
    //         // TODO: implement type inferring
    //         std::shared_ptr<TypeNode> type = std::make_shared<TypeNode>(IntNode());
    //         return type;
    //     }
    // };
    // // TODO: I forgot what I wanted to add here...
    // class StatementNode : Node
    // {
    // public:
    //     StatementNode()
    //     {
    //     }
    // };
    // class AssignmentNode : StatementNode
    // {
    // protected:
    //     node_ptr<ModifiablePrimaryNode> modifiablePrimary;
    //     node_ptr<ExpressionNode> expression;

    // public:
    //     AssignmentNode(node_ptr<ModifiablePrimaryNode> modifiablePrimary, node_ptr<ExpressionNode> expression)
    //     {
    //         this->modifiablePrimary = modifiablePrimary;
    //         this->expression = expression;
    //     }
    // };
    // class PrintNode : StatementNode
    // {
    // protected:
    //     std::string outputString;
    //     node_ptr<ExpressionNode> outputExpression;
    //     node_ptr<IdentifierNode> Identifier;
    //     bool NEWLINE;

    // public:
    //     PrintNode(std::string outputString)
    //     {
    //         this->outputString = outputString;
    //         this->NEWLINE = false;
    //     }
    //     PrintNode(node_ptr<ExpressionNode> outputExpression)
    //     {
    //         this->outputExpression = outputExpression;
    //         this->NEWLINE = false;
    //     }
    //     PrintNode(node_ptr<IdentifierNode> identifier)
    //     {
    //         this->Identifier = identifier;
    //         this->NEWLINE = false;
    //     }
    //     PrintNode(bool NEWLINE)
    //     {
    //         this->NEWLINE = true;
    //     }
    // };
    // class IfStatementNode : StatementNode
    // {
    // protected:
    //     node_ptr<ExpressionNode> condition;
    //     StatementList ifStatementList, elseStatementList;

    // public:
    //     IfStatementNode(node_ptr<ExpressionNode> condition, StatementList ifStatementList, StatementList elseStatementList)
    //     {
    //         this->condition = condition;
    //         this->ifStatementList = ifStatementList;
    //         this->elseStatementList = elseStatementList;
    //     }
    // };
    // class WhileNode : StatementNode
    // {
    // protected:
    //     node_ptr<ExpressionNode> condition;
    //     StatementList loopBody;

    // public:
    //     WhileNode(node_ptr<ExpressionNode> condition, StatementList loopBody)
    //     {
    //         this->condition = condition;
    //         this->loopBody = loopBody;
    //     }
    // };
    // class ForNode : StatementNode
    // {
    // protected:
    //     node_ptr<IdentifierNode> identifier;
    //     node_ptr<ExpressionNode> startRange, endRange;
    //     StatementList loopBody;

    // public:
    //     ForNode(node_ptr<IdentifierNode> identifier, node_ptr<ExpressionNode> startRange,
    //             node_ptr<ExpressionNode> endRange, StatementList loopBody)
    //     {
    //         this->identifier = identifier;
    //         this->startRange = startRange;
    //         this->endRange = endRange;
    //         this->loopBody = loopBody;
    //     }
    // };
    // class ForEachNode : StatementNode
    // {
    // protected:
    //     node_ptr<IdentifierNode> identifier;
    //     node_ptr<ModifiablePrimaryNode> modifiablePrimary;
    //     StatementList loopBody;

    // public:
    //     ForEachNode(node_ptr<IdentifierNode> identifier, node_ptr<ModifiablePrimaryNode> modifablePrimary, StatementList loopBody)
    //     {
    //         this->identifier = identifier;
    //         this->modifiablePrimary = modifablePrimary;
    //         this->loopBody = loopBody;
    //     }
    // };
    // class ReturnNode : StatementNode
    // {
    // protected:
    //     node_ptr<ExpressionNode> returnValue;

    // public:
    //     ReturnNode(node_ptr<ExpressionNode> returnValue)
    //     {
    //         this->returnValue = returnValue;
    //     }
    // };
    // class ExpressionNode : Node
    // {
    // protected:
    //     node_ptr<TypeNode> type;

    // public:
    //     ExpressionNode()
    //     {
    //         // should we infer type here?
    //     }
    //     ExpressionNode(node_ptr<TypeNode> type)
    //     {
    //         this->type = type;
    //     }
    // };
    // class IntegerLiteralNode : ExpressionNode
    // {
    // protected:
    //     int64_t value;

    // public:
    //     IntegerLiteralNode(int64_t value)
    //     {
    //         this->value = value;
    //     }
    // };
    // class DoubleLiteralNode : ExpressionNode
    // {
    // protected:
    //     long double value;

    // public:
    //     DoubleLiteralNode(long double value)
    //     {
    //         this->value = value;
    //     }
    // };
    // class BoolLiteralNode : ExpressionNode
    // {
    // protected:
    //     bool value;

    // public:
    //     BoolLiteralNode(bool value)
    //     {
    //         this->value = value;
    //     }
    // };
    // // The following classes are only for variable declerations.
    // class CharLiteralNode : ExpressionNode
    // {
    // protected:
    //     char value;

    // public:
    //     CharLiteralNode(char value)
    //     {
    //         this->value = value;
    //     }
    // };
    // class StringLiteralNode : ExpressionNode
    // {
    // protected:
    //     std::string value;

    // public:
    //     StringLiteralNode(std::string value)
    //     {
    //         this->value = value;
    //     }
    // };
    // class IdentifierNode : ExpressionNode
    // {
    // protected:
    //     std::string identifier;
    //     node_ptr<ExpressionNode> accessValue;

    // public:
    //     IdentifierNode(std::string identifier)
    //     {
    //         this->identifier = identifier;
    //     }
    //     IdentifierNode(std::string identifier, node_ptr<ExpressionNode> accessValue)
    //     {
    //         this->identifier = identifier;
    //         this->accessValue = accessValue;
    //     }
    // };
    // // Look into
    // class RoutineCallNode : ExpressionNode
    // {
    // protected:
    //     std::string routineName;
    //     std::vector<node_ptr<VariableDeclerationNode>> parameters;

    // public:
    //     RoutineCallNode(std::string routineName, std::vector<node_ptr<VariableDeclerationNode>> parameters)
    //     {
    //         this->routineName = routineName;
    //         this->parameters = parameters;
    //     }
    // };
    // class ModifiablePrimaryNode : ExpressionNode
    // {
    // protected:
    //     node_ptr<IdentifierNode> identifier;
    //     std::vector<nestedAccess> accessValues;

    // public:
    //     ModifiablePrimaryNode(node_ptr<IdentifierNode> identifier, std::vector<nestedAccess> accessValues)
    //     {
    //         this->identifier = identifier;
    //         this->accessValues = accessValues;
    //     }
    // };
    // class ArithmeticOperatorNode : ExpressionNode
    // {
    // protected:
    //     node_ptr<ExpressionNode> left, right;
    //     arithmeticOperatorEnum op;

    // public:
    //     ArithmeticOperatorNode(node_ptr<ExpressionNode> left, arithmeticOperatorEnum op, node_ptr<ExpressionNode> right)
    //     {
    //         this->left = left;
    //         this->op = op;
    //         this->right = right;
    //     }
    // };
    // class RelationalOperatorNode : ExpressionNode
    // {
    // protected:
    //     node_ptr<ExpressionNode> left, right;
    //     relationalOperatorEnum op;
    //     RelationalOperatorNode(node_ptr<ExpressionNode> left, relationalOperatorEnum op, node_ptr<ExpressionNode> right)
    //     {
    //         this->left = left;
    //         this->op = op;
    //         this->right = right;
    //     }
    // };
    // class BitwiseOperatorNode : ExpressionNode
    // {
    // protected:
    //     node_ptr<ExpressionNode> left, right;
    //     bitwiseOperatorEnum op;

    // public:
    //     BitwiseOperatorNode(node_ptr<ExpressionNode> left, bitwiseOperatorEnum op, node_ptr<ExpressionNode> right)
    //     {
    //         this->left = left;
    //         this->op = op;
    //         this->right = right;
    //     }
    // };
    // class NodeVisitor
    // {
    // };
    // class Interpreter : NodeVisitor
    // {
    // };
}

#endif // AST_H