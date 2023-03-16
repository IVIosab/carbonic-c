// define the abstract syntax tree here
// we'll use the visitor pattern when parsing semantics
#include <iostream>
#include <vector>
#include <unordered_map>
#define first f
#define second s

/// variable list connected to scope. each scope has start and end, new scope is previous scope +1
typedef std::vector <std::pair<VariableDeclerationNode, int>> VariableList;
typedef std::unordered_map<std::string, TypeNode> TypeMap;
typedef std::vector<StatementNode> StatementList;
typedef std::vector<RoutineDeclerationNode> RoutineList;
template <typename Node> using node_ptr = std::shared_ptr<Node>;

enum class typeEnum{
    INT, REAL, BOOL, CHAR, STRING, ARRAY, RECORD
};
enum class arithmeticOperatorEnum {
    PLUS, MINUS, MUL, DIV, POW
};
enum class relationalOperatorEnum{
    EQ, NOTEQ, GEQ, GT, LEQ, LT
};

class Node{
    std::vector< Node* > children;
};
/// This will hold the entire program tree and the lists of everything declared
class Program : Node{
    VariableList variableList;
    TypeMap typeMap;
    RoutineList routineList;
};
// you may need to override = operator for some classes
class TypeDeclerationNode : Node{
    std::string typeAlias;
    TypeNode type;
    TypeDeclerationNode(std::string typeAlias, TypeNode type){
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
class RealNode : TypeNode{
public:
    RealNode(){}
    typeEnum getType(){
        return typeEnum::REAL;
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
    ArrayNode(node_ptr<TypeNode> arrayType, node_ptr<ExpressionNode> size){
        this->arrayType = arrayType;
        this->size = size;
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
class VariableDeclerationNode : Node{
protected:
    node_ptr<TypeNode> variableType;
    node_ptr<IdentifierNode> idedntifier;
public:
    VariableDeclerationNode(node_ptr<TypeNode> variableType, node_ptr<IdentifierNode> identifier){
        this->variableType = variableType;
        this->idedntifier = identifier;
    }
};
class RoutineDeclerationNode : Node{
protected:
    StatementList statementList;
    node_ptr<TypeNode> routineType;
    std::vector<node_ptr<VariableDeclerationNode>> parameters;
public:
    RoutineDeclerationNode(std::vector<node_ptr<VariableDeclerationNode> > parameters, node_ptr<TypeNode> routineType, StatementList statementList){
        this->parameters = parameters;
        this->routineType = routineType;
        this->statementList = statementList;
    }
    RoutineDeclerationNode(std::vector<node_ptr<VariableDeclerationNode> > parameters, StatementList statementList){
        this->parameters = parameters;
        this->routineType = inferType(statementList);
        this->statementList = statementList;
    }
    node_ptr<TypeNode> inferType(StatementList StatementList){
        // TODO: implement type inferring
        std::shared_ptr<TypeNode> type = std::make_shared<IntNode>();
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
    node_ptr<IdentifierNode> identifier;
    node_ptr<ExpressionNode> expression;
public:
    AssignmentNode(node_ptr<IdentifierNode> identifier, node_ptr<ExpressionNode> expression){
        this->identifier = identifier;
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
    ForEachNode(node_ptr<IdentifierNode> identifier, node_ptr<ModifablePrimaryNode> modifablePrimary, StatementList loopBody){
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
class RoutineCallNode : ExpressionNode{
protected:
    node_ptr<RoutineDeclerationNode> routine;
    std::vector<node_ptr<VariableDeclerationNode>> parameters;
public:
    RoutineCallNode(node_ptr<RoutineDeclerationNode> routine, std::vector<node_ptr<VariableDeclerationNode>> parameters){
        this->routine = routine;
        this->parameters = parameters;
    }
};
// TODO: THIS IS WRONG. 
// for the accessValue we need a list of variant type
// list value be an identifier(for record) or an expression for array
class ModifiablePrimaryNode : ExpressionNode{
protected:
    node_ptr<IdentifierNode> identifier;
    node_ptr<ExpressionNode> accessValue;
public:
    ModifiablePrimaryNode(node_ptr<IdentifierNode> identifier, node_ptr<ExpressionNode> accessValue){
        this->identifier = identifier;
        this->accessValue = accessValue;
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
        this->right = op;
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

class NodeVisitor{

};
class Interpreter : NodeVisitor{

};