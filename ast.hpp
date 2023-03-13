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

class Node{
    std::vector< Node* > children;
};
/// This will hold the entire program tree and the lists of everything declared
class Program : Node{
    VariableList variableList;
    TypeMap typeMap;
    RoutineList routineList;
};
class TypeDeclerationNode : Node{
    TypeDeclerationNode(std::string typeAlias, TypeNode type){

    }
};
class TypeNode {
public:
    TypeNode(){}

};
class IntNode : TypeNode{
    IntNode(){}
};
class RealNode : TypeNode{
    RealNode(){}
};
class BoolNode : TypeNode{
    BoolNode(){}
};
class ArrayNode : TypeNode{
    ArrayNode(node_ptr<TypeNode> arrayType, node_ptr<ExpressionNode> size){
        
    }

};
class RecordNode : TypeNode{
    RecordNode(std::vector<node_ptr<VariableDeclerationNode>> recordVariables){

    }
};
class CharNode : TypeNode{
    CharNode(){}
};
class StringNode : TypeNode{
    StringNode(){}
};
class VariableDeclerationNode : Node{
    VariableDeclerationNode(node_ptr<TypeNode> variableType, node_ptr<IdentifierNode> identifier){

    }
};
class RoutineDeclerationNode : Node{
    StatementList statementList;
    
    RoutineDeclerationNode(std::vector<node_ptr<VariableDeclerationNode> > parameters, node_ptr<TypeNode> routineType, StatementList statementList){

    }
    RoutineDeclerationNode(std::vector<node_ptr<VariableDeclerationNode> > parameters, StatementList statementList){

    }
};
class StatementNode : Node{
public:
    StatementNode(){
    }
};
class AssignmentNode : StatementNode {
    AssignmentNode(node_ptr<IdentifierNode> identifier, node_ptr<ExpressionNode> expression){

    }
};
class PrintNode : StatementNode{
    PrintNode(std::string stringLiteral){

    }
    PrintNode(node_ptr<ExpressionNode> expression){

    }
};
class IfStatementNode : StatementNode {
    StatementList ifStatementList, elseStatementList;
    IfStatementNode(node_ptr<ExpressionNode> condition, StatementList ifStatementList, StatementList elseStatementList){

    }
};
class WhileNode : StatementNode{
    StatementList loopBody;
    WhileNode(node_ptr<ExpressionNode> condition, StatementList loopBody){

    }
};
class ForNode : StatementNode{
    ForNode(node_ptr<ExpressionNode> condition, StatementList loopBody){

    }
};
class ForEachNode : StatementNode{
    ForEachNode(node_ptr<IdentifierNode> identifier, node_ptr<ModifablePrimaryNode> modifablePrimary, StatementList loopBody){

    }
};
class ReturnNode : StatementNode{
    ReturnNode(node_ptr<ExpressionNode> returnValue){

    }
};

class ExpressionNode : Node{

};
class IntegerLiteralNode : ExpressionNode{

};
class DoubleLiteralNode : ExpressionNode{

};
class BoolLiteralNode : ExpressionNode{

};
class IdentifierNode : ExpressionNode{

};
class RoutineCallNode : ExpressionNode{

};
class ModifablePrimaryNode : ExpressionNode{

};
class operatorNode : ExpressionNode{

};
class arithmeticOperatorNode : operatorNode{

};
class relationalOperatorNode : operatorNode{

};
class logicalOperatorNode : operatorNode{

};

class NodeVisitor{

};
class Interpreter : NodeVisitor{

};