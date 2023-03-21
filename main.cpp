#include "lexer.h"
#include "driver.hpp"
#include "parser.hpp"

extern ast::node_ptr<ast::Program> program;

template <class T, class U>
std::shared_ptr<T> dynamic_pointer_cast(const std::shared_ptr<U> &r) noexcept
{
    if (auto p = dynamic_cast<typename std::shared_ptr<T>::element_type *>(r.get()))
        return std::shared_ptr<T>{r, p};
    else
        return std::shared_ptr<T>{};
}
std::string types[] = {"integer", "real", "boolean", "array", "record"};
std::string comparisonOperations[] = {"=", "/=", ">=", ">", "<", "<="};
std::string binaryOperations[] = {"+", "-", "*", "/", "^", "%"};
std::string bitwiseOperations[] = {"and", "or", "xor"};

void parseValue(ast::node_ptr<ast::Expression>, int);
void parseIntLiteral(ast::node_ptr<ast::IntLiteral>, int);
void parseDoubleLiteral(ast::node_ptr<ast::DoubleLiteral>, int);
void parseBoolLiteral(ast::node_ptr<ast::BoolLiteral>, int);
void parseBinary(ast::node_ptr<ast::BinaryExpression>, int);
void parseBitwise(ast::node_ptr<ast::BitwiseExpression>, int);
void parseComparison(ast::node_ptr<ast::ComparisonExpression>, int);
void parseStatementList(std::vector<ast::node_ptr<ast::Statement>>, int);
void parseStatement(ast::node_ptr<ast::Statement>, int);
void parseModifiablePrimary(ast::node_ptr<ast::ModifiablePrimary>, int);
void parseRoutineCall(ast::node_ptr<ast::RoutineCall>, int);
void parseBinaryOperator(ast::BinaryOperatorEnum, int);
void parseBitwiseOperator(ast::BitwiseOperatorEnum, int);
void parseComparisonOperator(ast::ComparisonOperatorEnum, int);
void parseAssignment(ast::node_ptr<ast::Assignment>, int);
void parsePrint(ast::node_ptr<ast::Print>, int);
void parseIfStatement(ast::node_ptr<ast::IfStatement>, int);
void parseWhile(ast::node_ptr<ast::WhileLoop>, int);
void parseFor(ast::node_ptr<ast::ForLoop>, int);
void parseForEach(ast::node_ptr<ast::ForeachLoop>, int);
void parseReturn(ast::node_ptr<ast::Return>, int);
void parseNestedAccess(ast::node_ptr<ast::nestedAccess>, int);
void parseIdentifier(ast::node_ptr<ast::Identifier>, int);
void parseRecordType(ast::node_ptr<ast::RecordType>, int);
void parseArrayType(ast::node_ptr<ast::ArrayType>, int);
void parsePrimitiveType(ast::node_ptr<ast::Type>, int);
void parseBody(ast::node_ptr<ast::Body>, int);
void parseParameters(std::vector<ast::node_ptr<ast::VariableDeclaration>> varList, int);
void parseExpression(ast::node_ptr<ast::Expression> exp, int);
void parseType(ast::node_ptr<ast::Type>, int);
void parseRoutineDeclaration(ast::node_ptr<ast::RoutineDeclaration>, int);
void parseTypeDeclaration(std::string id, ast::node_ptr<ast::Type>, int);
void parseVariableDeclaration(ast::node_ptr<ast::VariableDeclaration>, int);
void parseRoutineDeclarationList(std::vector<ast::node_ptr<ast::RoutineDeclaration>>, int);
void parseTypeDeclarationsMap(std::map<std::string, ast::node_ptr<ast::Type>>, int);
void parseVariableDeclarationList(std::vector<ast::node_ptr<ast::VariableDeclaration>>, int);
void parseProgram(ast::node_ptr<ast::Program>, int);

void indent(int depth)
{
    for (int i = 0; i < depth; i++)
    {
        std::cout << "--";
    }
}

void parseProgram(ast::node_ptr<ast::Program> program, int depth)
{
    indent(depth);
    std::cout << "Program" << std::endl;
    parseVariableDeclarationList(program->variables, depth + 1);
    parseTypeDeclarationsMap(program->types, depth + 1);
    parseRoutineDeclarationList(program->routines, depth + 1);
}

void parseVariableDeclarationList(std::vector<ast::node_ptr<ast::VariableDeclaration>> varList, int depth)
{
    indent(depth);
    std::cout << " VariableDeclarationList" << std::endl;
    for (int i = 0; i < varList.size(); i++)
    {
        parseVariableDeclaration(varList[i], depth + 1);
    }
}

void parseTypeDeclarationsMap(std::map<std::string, ast::node_ptr<ast::Type>> typeMap, int depth)
{
    indent(depth);
    std::cout << " TypeDeclarationsMap" << std::endl;
    for (auto const &[key, val] : typeMap)
    {
        parseTypeDeclaration(key, val, depth + 1);
    }
}

void parseRoutineDeclarationList(std::vector<ast::node_ptr<ast::RoutineDeclaration>> routList, int depth)
{
    indent(depth);
    std::cout << " RoutineDeclarationList" << std::endl;
    for (int i = 0; i < routList.size(); i++)
    {
        parseRoutineDeclaration(routList[i], depth + 1);
    }
}

void parseVariableDeclaration(ast::node_ptr<ast::VariableDeclaration> variable, int depth)
{
    indent(depth);
    std::cout << " VariableDeclaration" << std::endl;
    std::string name = variable->name;
    indent(depth);
    std::cout << "-- " << name << std::endl;
    parseType(variable->dtype, depth + 1);
    parseExpression(variable->initial_value, depth + 1);
}

void parseTypeDeclaration(std::string id, ast::node_ptr<ast::Type> type, int depth)
{
    indent(depth);
    std::cout << " TypeDeclaration" << std::endl;
    std::string name = id;
    indent(depth);
    std::cout << "-- " << name << std::endl;
    parseType(type, depth + 1);
}

void parseRoutineDeclaration(ast::node_ptr<ast::RoutineDeclaration> routine, int depth)
{
    indent(depth);
    std::cout << " RoutineDeclaration" << std::endl;
    std::string name = routine->name;
    indent(depth);
    std::cout << "-- " << name << std::endl;
    parseParameters(routine->params, depth + 1);
    parseType(routine->rtype, depth + 1);
    parseBody(routine->body, depth + 1);
}

void parseType(ast::node_ptr<ast::Type> type, int depth)
{
    indent(depth);
    std::cout << " Type" << std::endl;
    std::string s = types[type->getType()];
    if (s == "array")
    {
        auto Ref0 = std::dynamic_pointer_cast<ast::ArrayType>(type);
        parseArrayType(Ref0, depth + 1);
    }
    else if (s == "record")
    {
        auto Ref0 = std::dynamic_pointer_cast<ast::RecordType>(type);
        parseRecordType(Ref0, depth + 1);
    }
    else
    {
        parsePrimitiveType(type, depth + 1);
    }
}

void parseExpression(ast::node_ptr<ast::Expression> exp, int depth)
{
    indent(depth);
    std::cout << " Expression" << std::endl;
    auto Ref4 = std::dynamic_pointer_cast<ast::ComparisonExpression>(exp);
    if (Ref4)
    {
        parseComparison(Ref4, depth + 1);
        return;
    }
    auto Ref5 = std::dynamic_pointer_cast<ast::BinaryExpression>(exp);
    if (Ref5)
    {
        parseBinary(Ref5, depth + 1);
        return;
    }
    auto Ref6 = std::dynamic_pointer_cast<ast::BitwiseExpression>(exp);
    if (Ref6)
    {
        parseBitwise(Ref6, depth + 1);
        return;
    }
    parseValue(exp, depth + 1);
}

void parseParameters(std::vector<ast::node_ptr<ast::VariableDeclaration>> varList, int depth)
{
    indent(depth);
    std::cout << " Parameters" << std::endl;
    for (int i = 0; i < varList.size(); i++)
    {
        parseVariableDeclaration(varList[i], depth + 1);
    }
}

void parseBody(ast::node_ptr<ast::Body> body, int depth)
{
    indent(depth);
    std::cout << " Body" << std::endl;
    parseVariableDeclarationList(body->variables, depth + 1);
    parseStatementList(body->statements, depth + 1);
}

void parsePrimitiveType(ast::node_ptr<ast::Type> type, int depth)
{
    indent(depth);
    std::cout << " PrimitiveType" << std::endl;
    indent(depth);
    std::cout << "-- " << types[type->getType()] << std::endl;
}

void parseArrayType(ast::node_ptr<ast::ArrayType> array, int depth)
{
    indent(depth);
    std::cout << " ArrayType" << std::endl;
    parseExpression(array->size, depth + 1);
    parseType(array->dtype, depth + 1);
}

void parseRecordType(ast::node_ptr<ast::RecordType> record, int depth)
{
    indent(depth);
    std::cout << " RecordType" << std::endl;
    std::string name = record->name;
    indent(depth);
    std::cout << "-- " << name << std::endl;
    parseVariableDeclarationList(record->fields, depth + 1);
}

void parseValue(ast::node_ptr<ast::Expression> exp, int depth)
{
    indent(depth);
    std::cout << " Value" << std::endl;
    auto Ref0 = std::dynamic_pointer_cast<ast::IntLiteral>(exp);
    if (Ref0)
    {
        parseIntLiteral(Ref0, depth + 1);
        return;
    }
    auto Ref1 = std::dynamic_pointer_cast<ast::BoolLiteral>(exp);
    if (Ref1)
    {
        parseBoolLiteral(Ref1, depth + 1);
        return;
    }
    auto Ref2 = std::dynamic_pointer_cast<ast::DoubleLiteral>(exp);
    if (Ref2)
    {
        parseDoubleLiteral(Ref2, depth + 1);
        return;
    }
    auto Ref3 = std::dynamic_pointer_cast<ast::ModifiablePrimary>(exp);
    if (Ref3)
    {
        parseModifiablePrimary(Ref3, depth + 1);
        return;
    }
}

void parseBinary(ast::node_ptr<ast::BinaryExpression> exp, int depth)
{
    indent(depth);
    std::cout << " BinaryExpression" << std::endl;
    parseExpression(exp->lhs, depth + 1);
    parseBinaryOperator(exp->op, depth + 1);
    parseExpression(exp->lhs, depth + 1);
}

void parseBitwise(ast::node_ptr<ast::BitwiseExpression> exp, int depth)
{
    indent(depth);
    std::cout << " BitwiseExpression" << std::endl;
    parseExpression(exp->lhs, depth + 1);
    parseBitwiseOperator(exp->op, depth + 1);
    parseExpression(exp->lhs, depth + 1);
}

void parseComparison(ast::node_ptr<ast::ComparisonExpression> exp, int depth)
{
    indent(depth);
    std::cout << " ComparisonExpression" << std::endl;
    parseExpression(exp->lhs, depth + 1);
    parseComparisonOperator(exp->op, depth + 1);
    parseExpression(exp->lhs, depth + 1);
}

void parseIntLiteral(ast::node_ptr<ast::IntLiteral> exp, int depth)
{
    indent(depth);
    std::cout << " IntegerLiteral" << std::endl;
    indent(depth);
    std::cout << "-- " << exp->value << std::endl;
}

void parseDoubleLiteral(ast::node_ptr<ast::DoubleLiteral> exp, int depth)
{
    indent(depth);
    std::cout << " DoubleLiteral" << std::endl;
    indent(depth);
    std::cout << "-- " << exp->value << std::endl;
}

void parseBoolLiteral(ast::node_ptr<ast::BoolLiteral> exp, int depth)
{
    indent(depth);
    std::cout << " BoolLiteral" << std::endl;
    indent(depth);
    std::cout << "-- " << exp->value << std::endl;
}

void parseStatementList(std::vector<ast::node_ptr<ast::Statement>> stmtList, int depth)
{
    indent(depth);
    std::cout << " StatementList" << std::endl;
    for (int i = 0; i < stmtList.size(); i++)
    {
        parseStatement(stmtList[i], depth + 1);
    }
}

void parseStatement(ast::node_ptr<ast::Statement> stmt, int depth)
{
    indent(depth);
    std::cout << " Statement" << std::endl;
    auto Ref0 = std::dynamic_pointer_cast<ast::IfStatement>(stmt);
    if (Ref0)
    {
        parseIfStatement(Ref0, depth + 1);
    }
    auto Ref1 = std::dynamic_pointer_cast<ast::ForLoop>(stmt);
    if (Ref1)
    {

        parseFor(Ref1, depth + 1);
    }
    auto Ref2 = std::dynamic_pointer_cast<ast::ForeachLoop>(stmt);
    if (Ref2)
    {
        parseForEach(Ref2, depth + 1);
    }
    auto Ref3 = std::dynamic_pointer_cast<ast::WhileLoop>(stmt);
    if (Ref3)
    {
        parseWhile(Ref3, depth + 1);
    }
    auto Ref4 = std::dynamic_pointer_cast<ast::Return>(stmt);
    if (Ref4)
    {
        parseReturn(Ref4, depth + 1);
    }
    auto Ref5 = std::dynamic_pointer_cast<ast::Print>(stmt);
    if (Ref5)
    {
        parsePrint(Ref5, depth + 1);
    }
    auto Ref6 = std::dynamic_pointer_cast<ast::Assignment>(stmt);
    if (Ref6)
    {
        parseAssignment(Ref6, depth + 1);
    }
}

void parseModifiablePrimary(ast::node_ptr<ast::ModifiablePrimary> mp, int depth)
{
    indent(depth);
    std::cout << " ModifiablePrimary" << std::endl;
    parseIdentifier(mp->identifier, depth + 1);
    // parseNestedAccess(mp->accessValues, depth + 1);
    //
}

void parseRoutineCall(ast::node_ptr<ast::RoutineCall> call, int depth)
{
    indent(depth);
    std::cout << " RoutineCall" << std::endl;
    std::string name = call->name;
    indent(depth);
    std::cout << "-- " << name << std::endl;
    for (int i = 0; i < (call->args).size(); i++)
    {
        parseExpression((call->args)[i], depth + 1);
    } // questionable
}

void parseBinaryOperator(ast::BinaryOperatorEnum op, int depth)
{
    indent(depth);
    std::cout << " BinaryOperator" << std::endl;
    indent(depth);
    std::cout << "-- " << binaryOperations[op] << std::endl;
}

void parseBitwiseOperator(ast::BitwiseOperatorEnum op, int depth)
{
    indent(depth);
    std::cout << " BitwiseOperator" << std::endl;
    std::cout << "-- " << bitwiseOperations[op] << std::endl;
}

void parseComparisonOperator(ast::ComparisonOperatorEnum op, int depth)
{
    indent(depth);
    std::cout << " ComparisonOperator" << std::endl;
    std::cout << "-- " << comparisonOperations[op] << std::endl;
}

void parseAssignment(ast::node_ptr<ast::Assignment> ass, int depth)
{
    indent(depth);
    std::cout << " Assignment" << std::endl;
    parseModifiablePrimary(ass->modifiablePrimary, depth + 1);
    parseExpression(ass->expression, depth + 1);
}

void parsePrint(ast::node_ptr<ast::Print> print, int depth)
{
    indent(depth);
    std::cout << " Print" << std::endl;
    parseExpression(print->exp, depth + 1);
    bool endl = print->endl;
}

void parseIfStatement(ast::node_ptr<ast::IfStatement> ifstmt, int depth)
{
    indent(depth);
    std::cout << " IfStatement" << std::endl;
    parseExpression(ifstmt->condition, depth + 1);
    parseBody(ifstmt->ifBody, depth + 1);
    parseBody(ifstmt->elseBody, depth + 1);
}

void parseWhile(ast::node_ptr<ast::WhileLoop> whileloop, int depth)
{
    indent(depth);
    std::cout << " WhileLoop" << std::endl;
    parseExpression(whileloop->condition, depth + 1);
    parseBody(whileloop->loopBody, depth + 1);
}

void parseFor(ast::node_ptr<ast::ForLoop> forloop, int depth)
{
    indent(depth);
    std::cout << " ForLoop" << std::endl;
    parseVariableDeclaration(forloop->identifier, depth + 1);
    parseExpression(forloop->condition, depth + 1);
    parseExpression(forloop->action, depth + 1);
    parseBody(forloop->loopBody, depth + 1);
}

void parseForEach(ast::node_ptr<ast::ForeachLoop> foreach, int depth)
{
    indent(depth);
    std::cout << " ForEachLoop" << std::endl;
    parseIdentifier(foreach->identifier, depth + 1);
    parseModifiablePrimary(foreach->modifiablePrimary, depth + 1);
    parseBody(foreach->loopBody, depth + 1);
}

void parseReturn(ast::node_ptr<ast::Return> returnstmt, int depth)
{
    indent(depth);
    std::cout << " Return" << std::endl;
    parseExpression(returnstmt->exp, depth + 1);
}

void parseNestedAccess(std::vector<ast::nestedAccess> nestedaccess, int depth)
{
    indent(depth);
    std::cout << " NestedAccess" << std::endl;
}

void parseIdentifier(ast::node_ptr<ast::Identifier> id, int depth)
{
    indent(depth);
    std::cout << " Identifier" << std::endl;
    std::string name = id->name;
    indent(depth);
    std::cout << "-- " << name << std::endl;
    parseExpression(id->idx, depth + 1);
}

int main(int argc, char **argv)
{
    carbonic_c::Driver driver;
    // while (lexer->yylex() != 0)
    // driver.parse_args(argc, argv);
    int x = driver.parse_program();

    parseProgram(program, 0);
    // std::cout << program->routines[0]->name << std::endl;
    // std::cout << program->routines[0]->params.size() << std::endl;
    // std::cout << program->routines[0]->body->variables.size() << std::endl;
    // std::cout << program->routines[0]->body->variables[0]->name << std::endl;
    // std::cout << program->routines[0]->body->variables[0]->initial_value << std::endl;

    return 0;
}