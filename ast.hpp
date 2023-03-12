// define the abstract syntax tree here
// we'll use the visitor pattern when parsing semantics
#include <iostream>
#include <vector>
#include <unordered_map>
#include <stack>
#define first f
#define second s

std::unordered_map <std::string, bool> VariableList;
// pair < variable name, scope number > 
// -> when a new scope is introduced, its id is (lastScopeId + 1).
// what to do with parallel scopes? do we pop before we push again?
std::stack <std::pair<std::string, int>> variableScope;
typedef std::vector<ExpressionNode> ExpressionList;
std::unordered_map<std::string, TypeNode> TypeList;
typedef std::vector<StatementNode> StatementList;

class Node{
    Node* right = NULL;
    Node* lfet  = NULL;
};
class SimpleDeclerationNode : Node{
    
};
// type inherits from type decleration?
class TypeDeclerationNode : SimpleDeclerationNode{
    //link new type to typeNode and store in type list
};
class TypeNode : TypeDeclerationNode{

};
class IntNode : TypeNode{

};
class ReelNode : TypeNode{

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
class VariableDeclerationNode : SimpleDeclerationNode{
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