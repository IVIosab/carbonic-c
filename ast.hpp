// define the abstract syntax tree here
// we'll use the visitor pattern when parsing semantics
#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#define first f
#define second s

// variable -> scope number
typedef std::unordered_map <std::string, int> VariableList;
// pair < variable name, scope number > 
// -> when a new scope is introduced, its id is (lastScopeId + 1).
// start - end
// treat it like an iterable stack
typedef std::vector <std::pair<VariableDeclerationNode, int>> VariableScope;
//typedef std::vector<ExpressionNode> ExpressionList;
typedef std::unordered_map<std::string, TypeNode> TypeList;
typedef std::vector <std::pair<TypeDeclerationNode, int>> TypeScope;
typedef std::vector<StatementNode> StatementList;
typedef std::vector<RoutineDeclerationNode> RoutineList;
class Node{
    Node* right = NULL;
    Node* left  = NULL;
};
/// This will hold the entire program tree and the lists of everything declared
class Program : Node{
    VariableList variableList;
    VariableScope variableScope;
    TypeList typeList;
    TypeScope typeScope;
    RoutineList routineList;
};
// type inherits from type decleration?
class TypeDeclerationNode : Node{
    TypeDeclerationNode(std::string typeAlias, TypeNode type){
        

    }
    //link new type to typeNode and store in type list
};
class TypeNode : TypeDeclerationNode{

};
class IntNode : TypeNode{

};
class RealNode : TypeNode{

};
class boolNode : TypeNode{

};
class arrayNode : TypeNode{

};
class recordNode : TypeNode{

};
class CharNode : TypeNode{

};
class StringNode : TypeNode{

};
class VariableDeclerationNode : Node{
    // add to variablelist
};
class RoutineDeclerationNode : Node{
    // statementList
};
class StatementNode : Node{

};
/// if we can't have global statements, add 'routine decleration' in between the following classes.
class AssignmentNode : StatementNode {

};
class printNode : StatementNode{

};
class IfStatementNode : StatementNode {

};
class WhileNode : StatementNode{

};
class ForNode : StatementNode{

};
class ForEachNode : StatementNode{

};
class ReturnNode : StatementNode{

};

class ExpressionNode : Node {

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