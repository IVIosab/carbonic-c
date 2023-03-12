// define the abstract syntax tree here
// we'll use the visitor pattern when parsing semantics
#include <iostream>
#include <vector>
typedef std::vector<ExpressionNode> ExpressionList;
typedef std::vector<SimpleDeclerationNode> VariableList;
typedef std::vector<StatementNode> StatementList;

class Node{

};
class SimpleDeclerationNode : Node{

};
class TypeDeclerationNode : SimpleDeclerationNode{

};
class VariableDeclerationNode : SimpleDeclerationNode{

};
// type inherits from type decleration?
class RoutineDeclerationNode : Node{

};
class StatementNode : Node{

};
/// if we can't have global statements, add 'routine decleration' in between.
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
class RoutineCall : StatementNode{

};
// return where?
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