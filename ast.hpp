// define the abstract syntax tree here
// we'll use the visitor pattern when parsing semantics
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <variant>
#define first f
#define second s
class Node;
class Program;
class TypeDeclerationNode;
class TypeNode;
class IntNode;
class DoubleNode;
class BoolNode;
class ArrayNode;
class RecordNode;
class VariableDeclerationNode;
class RoutineDeclerationNode;
class IdentifierNode;
class ModifiablePrimaryNode;
class ExpressionNode;
class StatementNode;
class BitwiseOperatorNode;
class RelationalOperatorNode;
class ArithmeticOperatorNode;

template <typename Node> using node_ptr = std::shared_ptr<Node>;
/// potentially add scopes
typedef std::vector<VariableDeclerationNode> VariableList;
typedef std::unordered_map<std::string, TypeNode> TypeMap;
typedef std::vector<StatementNode> StatementList;
typedef std::vector<RoutineDeclerationNode> RoutineList;
typedef std::variant<node_ptr<ExpressionNode>(), node_ptr<IdentifierNode>()> nestedAccess;

typedef node_ptr<Node> node;
typedef node_ptr<RoutineDeclerationNode> routineDeclerationNode;
typedef node_ptr<VariableDeclerationNode> variableDeclerationNode;
typedef node_ptr<ExpressionNode>   expressionNode; 
typedef node_ptr<IdentifierNode> identifierNode;
typedef std::variant<node_ptr<ExpressionNode>, node_ptr<IdentifierNode>,
node_ptr<ArithmeticOperatorNode>, node_ptr<RelationalOperatorNode>, 
node_ptr<BitwiseOperatorNode> > expressionType;

enum class typeEnum{
    INT, DOUBLE, BOOL, CHAR, STRING, ARRAY, RECORD
};
enum class arithmeticOperatorEnum {
    PLUS, MINUS, MUL, DIV, MOD, POW
};
enum class relationalOperatorEnum{
    CEQ, CNEQ, CGE, CGT, CLE, CLT
};
enum class bitwiseOperatorEnum{
    AND, OR, XOR
};

class Node{
    std::vector<std::shared_ptr<Node>> children();
};
/// This will hold the entire program tree and the lists of everything declared
class Program : Node{
    VariableList variableList;
    TypeMap typeMap; // I may not be able to use that
    RoutineList routineList;
};
// you may need to override = operator for some classes
class TypeDeclerationNode : Node{
    std::string typeAlias;
    node_ptr<TypeNode> type;
    TypeDeclerationNode(std::string typeAlias, node_ptr<TypeNode> type){
        this->typeAlias = typeAlias;
        this->type = type;
    }
};
class TypeNode {
public:
    TypeNode(){}
    virtual typeEnum getType();
};
class IntNode : TypeNode{
public:
    IntNode(){}
    typeEnum getType(){
        return typeEnum::INT;
    }
};
class DoubleNode : TypeNode{
public:
    DoubleNode(){}
    typeEnum getType(){
        return typeEnum::DOUBLE;
    }
};
class BoolNode : TypeNode{
public:    
    BoolNode(){}
    typeEnum getType(){
        return typeEnum::BOOL;
    }
};
class ArrayNode : TypeNode{
protected:
    node_ptr<TypeNode> arrayType;
    node_ptr<ExpressionNode> size;
public:
    ArrayNode(node_ptr<ExpressionNode> size, node_ptr<TypeNode> arrayType){
        this->size = size;
        this->arrayType = arrayType;
    }
    typeEnum getType(){
        return typeEnum::ARRAY;
    }
};
class RecordNode : TypeNode{
protected:
    std::vector<node_ptr<VariableDeclerationNode>> recordVariables;
public:
    RecordNode(std::vector<node_ptr<VariableDeclerationNode>> recordVariables){
        this->recordVariables = recordVariables;
    }
    typeEnum getType(){
        return typeEnum::RECORD;
    }
};
class CharNode : TypeNode{
public:
    CharNode(){}
    typeEnum getType(){
        return typeEnum::CHAR;
    }
};
class StringNode : TypeNode{
public:
    StringNode(){}
    typeEnum getType(){
        return typeEnum::STRING;
    }
};
// TODO: do we infer types here?
class VariableDeclerationNode : Node{
protected:
    node_ptr<TypeNode> variableType;
    node_ptr<IdentifierNode> idedntifier;
    node_ptr<ExpressionNode> value;
public:
    VariableDeclerationNode(node_ptr<IdentifierNode> identifier, node_ptr<TypeNode> variableType, node_ptr<ExpressionNode> expression){
        this->idedntifier = identifier;
        this->variableType = variableType;
        this->value = expression;
    }
    VariableDeclerationNode(node_ptr<IdentifierNode> identifier, node_ptr<TypeNode> variableType){
        this->idedntifier = identifier;
        this->variableType = variableType;
    }
    VariableDeclerationNode(node_ptr<IdentifierNode> identifier, node_ptr<ExpressionNode> expression){
       this->idedntifier = identifier;
       this->value = expression;
    }
    node_ptr<TypeNode> inferType(node_ptr<ExpressionNode> expression){
        // expression should potentially have type inference so you only need to imlement it once.    
    }
};
class RoutineDeclerationNode : Node{
protected:
    std::string identifier;
    StatementList statementList;
    node_ptr<TypeNode> routineType;
    std::vector<node_ptr<VariableDeclerationNode>> parameters;
public:
    RoutineDeclerationNode( std::string identifier, std::vector<node_ptr<VariableDeclerationNode> > parameters,
     node_ptr<TypeNode> routineType, StatementList statementList){
        this->identifier = identifier;
        this->parameters = parameters;
        this->routineType = routineType;
        this->statementList = statementList;
    }
    RoutineDeclerationNode(std::string identifier,
    std::vector<node_ptr<VariableDeclerationNode> > parameters, StatementList statementList){
        this->identifier = identifier;
        this->parameters = parameters;
        this->routineType = inferType(statementList);
        this->statementList = statementList;
    }
    node_ptr<TypeNode> inferType(StatementList StatementList){
        // TODO: implement type inferring
        std::shared_ptr<TypeNode> type = std::make_shared<TypeNode>(IntNode());
        return type;
    }
};
// TODO: I forgot what I wanted to add here...
class StatementNode : Node{
public:
    StatementNode(){
    }
};
class AssignmentNode : StatementNode {
protected:
    node_ptr<ModifiablePrimaryNode> modifiablePrimary;
    node_ptr<ExpressionNode> expression;
public:
    AssignmentNode(node_ptr<ModifiablePrimaryNode> modifiablePrimary, node_ptr<ExpressionNode> expression){
        this->modifiablePrimary = modifiablePrimary;
        this->expression = expression;
    }
};
class PrintNode : StatementNode{
protected:
    std::string outputString;
    node_ptr<ExpressionNode> outputExpression;
    node_ptr<IdentifierNode> Identifier;
    bool NEWLINE;
public:
    PrintNode(std::string outputString){
        this->outputString = outputString;
        this->NEWLINE = false;
    }
    PrintNode(node_ptr<ExpressionNode> outputExpression){
        this->outputExpression = outputExpression;
        this->NEWLINE = false;
    }
    PrintNode(node_ptr<IdentifierNode> identifier){
        this->Identifier = identifier;
        this->NEWLINE = false;
    }
    PrintNode(bool NEWLINE){
        this->NEWLINE = true;
    }
};
class IfStatementNode : StatementNode {
protected:
    node_ptr<ExpressionNode> condition;
    StatementList ifStatementList, elseStatementList;
public:
    IfStatementNode(node_ptr<ExpressionNode> condition, StatementList ifStatementList, StatementList elseStatementList){
        this->condition = condition;
        this->ifStatementList = ifStatementList;
        this->elseStatementList = elseStatementList;
    }
};
class WhileNode : StatementNode{
protected:
    node_ptr<ExpressionNode> condition;
    StatementList loopBody;
public:
    WhileNode(node_ptr<ExpressionNode> condition, StatementList loopBody){
        this->condition = condition;
        this->loopBody = loopBody;
    }
};
class ForNode : StatementNode{
protected:
    node_ptr<IdentifierNode> identifier;
    node_ptr<ExpressionNode> startRange, endRange;
    StatementList loopBody;
public:
    ForNode(node_ptr<IdentifierNode> identifier, node_ptr<ExpressionNode> startRange,
    node_ptr<ExpressionNode> endRange, StatementList loopBody){
        this->identifier = identifier;
        this->startRange = startRange;
        this->endRange = endRange;
        this->loopBody = loopBody;
    }
};
class ForEachNode : StatementNode{
protected:
    node_ptr<IdentifierNode> identifier;
    node_ptr<ModifiablePrimaryNode> modifiablePrimary;
    StatementList loopBody;
public:
    ForEachNode(node_ptr<IdentifierNode> identifier, node_ptr<ModifiablePrimaryNode> modifablePrimary, StatementList loopBody){
        this->identifier = identifier;
        this->modifiablePrimary = modifablePrimary;
        this->loopBody = loopBody;
    }
};
class ReturnNode : StatementNode{
protected:
    node_ptr<ExpressionNode> returnValue;
public:
    ReturnNode(node_ptr<ExpressionNode> returnValue){
        this->returnValue = returnValue;
    }
};
class ExpressionNode : Node{
protected:
    node_ptr<TypeNode> type;
public:
    ExpressionNode(){
        // should we infer type here?
    }
    ExpressionNode(node_ptr<TypeNode> type){
        this->type = type;
    }
};
class IntegerLiteralNode : ExpressionNode{
protected:
    int64_t value;
public:
    IntegerLiteralNode(int64_t value){
        this->value = value;
    }
};
class DoubleLiteralNode : ExpressionNode{
protected:
    long double value;
public:
    DoubleLiteralNode(long double value){
        this->value = value;
    }
};
class BoolLiteralNode : ExpressionNode{
protected:
    bool value;
public:
    BoolLiteralNode(bool value){
        this->value = value;
    }
};
// The following classes are only for variable declerations.
class CharLiteralNode : ExpressionNode{
protected:
    char value;
public:
    CharLiteralNode(char value){
        this->value = value;
    }
};
class StringLiteralNode : ExpressionNode{
protected:
    std::string value;
public:
    StringLiteralNode(std::string value){
        this->value = value;
    }
};
class IdentifierNode : ExpressionNode{
protected:
    std::string identifier;
    node_ptr<ExpressionNode> accessValue;
public:
    IdentifierNode(std::string identifier){
        this->identifier = identifier;
    }
    IdentifierNode(std::string identifier, node_ptr<ExpressionNode> accessValue){
        this->identifier = identifier;
        this->accessValue = accessValue;
    }
};
// Look into
class RoutineCallNode : ExpressionNode{
protected:
    std::string routineName;
    std::vector<node_ptr<VariableDeclerationNode>> parameters;
public:
    RoutineCallNode(std::string routineName, std::vector<node_ptr<VariableDeclerationNode>> parameters){
        this->routineName = routineName;
        this->parameters = parameters;
    }
};
class ModifiablePrimaryNode : ExpressionNode{
protected:
    node_ptr<IdentifierNode> identifier;
    std::vector<nestedAccess> accessValues;
public:
    ModifiablePrimaryNode(node_ptr<IdentifierNode> identifier, std::vector<nestedAccess> accessValues){
        this->identifier = identifier;
        this->accessValues = accessValues;
    }
};
class ArithmeticOperatorNode : ExpressionNode{
protected:
    node_ptr<ExpressionNode> left, right;
    arithmeticOperatorEnum op;
public:
    ArithmeticOperatorNode(node_ptr<ExpressionNode> left, arithmeticOperatorEnum op, node_ptr<ExpressionNode> right){
        this->left = left;
        this->op = op;
        this->right = right;
    }
};
class RelationalOperatorNode : ExpressionNode{
protected:
    node_ptr<ExpressionNode> left, right;
    relationalOperatorEnum op;
    RelationalOperatorNode(node_ptr<ExpressionNode> left, relationalOperatorEnum op, node_ptr<ExpressionNode> right){
        this->left = left;
        this->op = op;
        this->right = right;
    }
};
class BitwiseOperatorNode : ExpressionNode{
protected:
    node_ptr<ExpressionNode> left, right;
    bitwiseOperatorEnum op;
public:
    BitwiseOperatorNode(node_ptr<ExpressionNode> left, bitwiseOperatorEnum op, node_ptr<ExpressionNode> right){
        this->left = left;
        this->op = op;
        this->right = right;
    }
};
class NodeVisitor{

};
class Interpreter : NodeVisitor{

};