#ifndef AST_H
#define AST_H

#include <iostream>
#include <map>
#include <vector>
#include "Enums.h"

// Forward declarations
namespace ast
{
    typedef long long Integer;
    typedef long double Real;
    typedef bool Boolean;
    typedef std::string Ident;

    class Program;
    class Decl; /*Abstract*/
    class RoutineDecl;
    class GlobalVarDecl;
    class Expr; /*Abstract*/
    class ExprList;
    class BinaryExpr;
    class LogicExpr;
    class ComparisonExpr;
    class Value; /*Abstract*/
    class IntegerValue;
    class RealValue;
    class BooleanValue;
    class RoutineCallValue;
    class Var;
    class TypeDecl;
    class Type;          /*Abstract*/
    class PrimitiveType; /*Abstract*/
    class UserType;      /*Abstract*/
    class TypeIdentifier;
    class IntegerType;
    class RealType;
    class BooleanType;
    class ArrayType;
    class RecordType;
    class ParameterDecl;
    class ParameterList;
    class Body;
    class BodyEntity; /*Abstract*/
    class LocalVarDecl;
    class LocalVarList;
    class Statement; /*Abstract*/
    class Assignment;
    class RoutineCall;
    class Return;
    class Print;
    class WhileLoop;
    class ForLoop;
    class Range;
    class If;
    class NestedAccess;
    class NestedAccessList;
    class ArrayAccess;
    class RecordAccess;

    class Visitor
    {
    public:
        virtual void visitProgram(Program *p) = 0;
        virtual void visitDecl(Decl *p) = 0;
        virtual void visitRoutineDecl(RoutineDecl *p) = 0;
        virtual void visitGlobalVarDecl(GlobalVarDecl *p) = 0;
        virtual void visitExpr(Expr *p) = 0;
        virtual void visitExprList(ExprList *p) = 0;
        virtual void visitBinaryExpr(BinaryExpr *p) = 0;
        virtual void visitLogicExpr(LogicExpr *p) = 0;
        virtual void visitComparisonExpr(ComparisonExpr *p) = 0;
        virtual void visitValue(Value *p) = 0;
        virtual void visitIntegerValue(IntegerValue *p) = 0;
        virtual void visitRealValue(RealValue *p) = 0;
        virtual void visitBooleanValue(BooleanValue *p) = 0;
        virtual void visitRoutineCallValue(RoutineCallValue *p) = 0;
        virtual void visitVar(Var *p) = 0;
        virtual void visitTypeDecl(TypeDecl *p) = 0;
        virtual void visitType(Type *p) = 0;
        virtual void visitPrimitiveType(PrimitiveType *p) = 0;
        virtual void visitUserType(UserType *p) = 0;
        virtual void visitTypeIdentifier(TypeIdentifier *p) = 0;
        virtual void visitIntegerType(IntegerType *p) = 0;
        virtual void visitRealType(RealType *p) = 0;
        virtual void visitBooleanType(BooleanType *p) = 0;
        virtual void visitArrayType(ArrayType *p) = 0;
        virtual void visitRecordType(RecordType *p) = 0;
        virtual void visitParameterDecl(ParameterDecl *p) = 0;
        virtual void visitParameterList(ParameterList *p) = 0;
        virtual void visitBody(Body *p) = 0;
        virtual void visitBodyEntity(BodyEntity *p) = 0;
        virtual void visitLocalVarDecl(LocalVarDecl *p) = 0;
        virtual void visitLocalVarList(LocalVarList *p) = 0;
        virtual void visitStatement(Statement *p) = 0;
        virtual void visitAssignment(Assignment *p) = 0;
        virtual void visitRoutineCall(RoutineCall *p) = 0;
        virtual void visitReturn(Return *p) = 0;
        virtual void visitPrint(Print *p) = 0;
        virtual void visitWhileLoop(WhileLoop *p) = 0;
        virtual void visitForLoop(ForLoop *p) = 0;
        virtual void visitRange(Range *p) = 0;
        virtual void visitIf(If *p) = 0;
        virtual void visitNestedAccess(NestedAccess *p) = 0;
        virtual void visitNestedAccessList(NestedAccessList *p) = 0;
        virtual void visitArrayAccess(ArrayAccess *p) = 0;
        virtual void visitRecordAccess(RecordAccess *p) = 0;

        virtual void visitInteger(Integer x) = 0;
        virtual void visitReal(Real x) = 0;
        virtual void visitBoolean(Boolean x) = 0;
        virtual void visitIdent(Ident x) = 0;
    };

    class Visitable
    {
    public:
        virtual void accept(Visitor *v) = 0;
    };

    class Program : public Visitable
    {
    public:
        std::vector<Decl *> decls = {};
        Program(std::vector<Decl *> decls) : decls(decls) {}
        void accept(Visitor *v) override { v->visitProgram(this); }
    };

    class Decl : public Visitable
    {
    public:
        virtual void accept(Visitor *v) = 0;
    };

    class RoutineDecl : public Decl
    {
    public:
        Ident name;
        ParameterList *params = nullptr;
        Type *returnType = nullptr;
        Body *body = nullptr;
        RoutineDecl(Ident name, ParameterList *params, Type *returnType, Body *body) : name(name), params(params), returnType(returnType), body(body) {}
        void accept(Visitor *v) override { v->visitRoutineDecl(this); }
    };

    class GlobalVarDecl : public Decl
    {
    public:
        Ident name;
        Type *type = nullptr;
        Expr *init = nullptr;
        GlobalVarDecl(Ident name, Type *type, Expr *init) : name(name), type(type), init(init) {}
        void accept(Visitor *v) override { v->visitGlobalVarDecl(this); }
    };

    class TypeDecl : public Decl
    {
    public:
        TypeIdentifier *name = nullptr;
        Type *type = nullptr;
        TypeDecl(TypeIdentifier *name, Type *type) : name(name), type(type) {}
        void accept(Visitor *v) override { v->visitTypeDecl(this); }
    };

    class Expr : public Visitable
    {
    public:
        virtual void accept(Visitor *v) = 0;
    };

    class ExprList : public Visitable
    {
    public:
        std::vector<Expr *> exprs = {};
        ExprList(std::vector<Expr *> exprs) : exprs(exprs) {}
        void accept(Visitor *v) override { v->visitExprList(this); }
    };

    class BinaryExpr : public Expr
    {
    public:
        BinaryOperator op;
        Expr *left = nullptr;
        Expr *right = nullptr;
        BinaryExpr(BinaryOperator op, Expr *left, Expr *right) : op(op), left(left), right(right) {}
        void accept(Visitor *v) override { v->visitBinaryExpr(this); }
    };

    class LogicExpr : public Expr
    {
    public:
        LogicOperator op;
        Expr *left = nullptr;
        Expr *right = nullptr;
        LogicExpr(LogicOperator op, Expr *left, Expr *right) : op(op), left(left), right(right) {}
        void accept(Visitor *v) override { v->visitLogicExpr(this); }
    };

    class ComparisonExpr : public Expr
    {
    public:
        ComparisonOperator op;
        Expr *left = nullptr;
        Expr *right = nullptr;
        ComparisonExpr(ComparisonOperator op, Expr *left, Expr *right) : op(op), left(left), right(right) {}
        void accept(Visitor *v) override { v->visitComparisonExpr(this); }
    };

    class Value : public Expr
    {
    public:
        virtual void accept(Visitor *v) = 0;
    };

    class IntegerValue : public Value
    {
    public:
        Integer value;
        IntegerValue(Integer value) : value(value) {}
        void accept(Visitor *v) override { v->visitIntegerValue(this); }
    };

    class RealValue : public Value
    {
    public:
        Real value;
        RealValue(Real value) : value(value) {}
        void accept(Visitor *v) override { v->visitRealValue(this); }
    };

    class BooleanValue : public Value
    {
    public:
        Boolean value;
        BooleanValue(Boolean value) : value(value) {}
        void accept(Visitor *v) override { v->visitBooleanValue(this); }
    };

    class RoutineCallValue : public Value
    {
    public:
        Ident name;
        ExprList *args = nullptr;
        RoutineCallValue(Ident name, ExprList *args) : name(name), args(args) {}
        void accept(Visitor *v) override { v->visitRoutineCallValue(this); }
    };

    class Var : public Value
    {
    public:
        Ident name;
        NestedAccessList *accesses;
        Var(Ident name, NestedAccessList *accesses) : name(name), accesses(accesses) {}
        void accept(Visitor *v) override { v->visitVar(this); }
    };

    class Type : public Visitable
    {
    public:
        virtual void accept(Visitor *v) = 0;
    };

    class PrimitiveType : public Type
    {
    public:
        virtual void accept(Visitor *v) = 0;
    };

    class UserType : public Type
    {
    public:
        virtual void accept(Visitor *v) = 0;
    };

    class TypeIdentifier : public Type
    {
    public:
        Ident name;
        TypeIdentifier(Ident name) : name(name) {}
        void accept(Visitor *v) override { v->visitTypeIdentifier(this); }
    };

    class IntegerType : public PrimitiveType
    {
    public:
        void accept(Visitor *v) override { v->visitIntegerType(this); }
    };

    class RealType : public PrimitiveType
    {
    public:
        void accept(Visitor *v) override { v->visitRealType(this); }
    };

    class BooleanType : public PrimitiveType
    {
    public:
        void accept(Visitor *v) override { v->visitBooleanType(this); }
    };

    class ArrayType : public UserType
    {
    public:
        Type *type = nullptr;
        Expr *size = nullptr;
        ArrayType(Type *type, Expr *size) : type(type), size(size) {}
        void accept(Visitor *v) override { v->visitArrayType(this); }
    };

    class RecordType : public UserType
    {
    public:
        LocalVarList *decls;
        RecordType(LocalVarList *decls) : decls(decls) {}
        void accept(Visitor *v) override { v->visitRecordType(this); }
    };

    class ParameterDecl : public Decl
    {
    public:
        Ident name;
        Type *type = nullptr;
        ParameterDecl(Ident name, Type *type) : name(name), type(type) {}
        void accept(Visitor *v) override { v->visitParameterDecl(this); }
    };

    class ParameterList : public Visitable
    {
    public:
        std::vector<ParameterDecl *> decls = {};
        ParameterList(std::vector<ParameterDecl *> decls) : decls(decls) {}
        void accept(Visitor *v) override { v->visitParameterList(this); }
    };

    class Body : public Visitable
    {
    public:
        std::vector<BodyEntity *> entities = {};
        Body(std::vector<BodyEntity *> entities) : entities(entities) {}
        void accept(Visitor *v) override { v->visitBody(this); }
    };

    class BodyEntity : public Visitable
    {
    public:
        virtual void accept(Visitor *v) = 0;
    };

    class LocalVarDecl : public BodyEntity
    {
    public:
        Ident name;
        Type *type = nullptr;
        Expr *init = nullptr;
        LocalVarDecl(Ident name, Type *type, Expr *init) : name(name), type(type), init(init) {}
        void accept(Visitor *v) override { v->visitLocalVarDecl(this); }
    };

    class LocalVarList : public Visitable
    {
    public:
        std::vector<LocalVarDecl *> vars = {};
        LocalVarList(std::vector<LocalVarDecl *> vars) : vars(vars) {}
        void accept(Visitor *v) override { v->visitLocalVarList(this); }
    };

    class Statement : public BodyEntity
    {
    public:
        virtual void accept(Visitor *v) = 0;
    };

    class Assignment : public Statement
    {
    public:
        Var *var = nullptr;
        Expr *expr = nullptr;
        Assignment(Var *var, Expr *expr) : var(var), expr(expr) {}
        void accept(Visitor *v) override { v->visitAssignment(this); }
    };

    class RoutineCall : public Statement
    {
    public:
        Ident name;
        ExprList *args = nullptr;
        RoutineCall(Ident name, ExprList *args) : name(name), args(args) {}
        void accept(Visitor *v) override { v->visitRoutineCall(this); }
    };

    class Return : public Statement
    {
    public:
        Expr *expr = nullptr;
        Return(Expr *expr) : expr(expr) {}
        void accept(Visitor *v) override { v->visitReturn(this); }
    };

    class Print : public Statement
    {
    public:
        Expr *expr = nullptr;
        Print(Expr *expr) : expr(expr) {}
        void accept(Visitor *v) override { v->visitPrint(this); }
    };

    class WhileLoop : public Statement
    {
    public:
        Expr *condition = nullptr;
        Body *body = nullptr;
        WhileLoop(Expr *condition, Body *body) : condition(condition), body(body) {}
        void accept(Visitor *v) override { v->visitWhileLoop(this); }
    };

    class ForLoop : public Statement
    {
    public:
        Ident name;
        Range *range = nullptr;
        Body *body = nullptr;
        ForLoop(Ident name, Range *range, Body *body) : name(name), range(range), body(body) {}

        void accept(Visitor *v) override { v->visitForLoop(this); }
    };

    class Range : public Visitable
    {
    public:
        Expr *from = nullptr;
        Expr *to = nullptr;
        bool reverse = false;
        Range(Expr *from, Expr *to, bool reverse) : from(from), to(to), reverse(reverse) {}
        void accept(Visitor *v) override { v->visitRange(this); }
    };

    class If : public Statement
    {
    public:
        Expr *condition = nullptr;
        Body *then = nullptr;
        Body *else_ = nullptr;
        If(Expr *condition, Body *then, Body *else_) : condition(condition), then(then), else_(else_) {}
        void accept(Visitor *v) override { v->visitIf(this); }
    };

    class NestedAccess : public Visitable
    {
    public:
        virtual void accept(Visitor *v) = 0;
    };

    class NestedAccessList : public Visitable
    {
    public:
        std::vector<NestedAccess *> accesses = {};
        NestedAccessList(std::vector<NestedAccess *> accesses) : accesses(accesses) {}
        void accept(Visitor *v) override { v->visitNestedAccessList(this); }
    };

    class ArrayAccess : public NestedAccess
    {
    public:
        Expr *index = nullptr;
        ArrayAccess(Expr *index) : index(index) {}
        void accept(Visitor *v) override { v->visitArrayAccess(this); }
    };

    class RecordAccess : public NestedAccess
    {
    public:
        Ident name;
        RecordAccess(Ident name) : name(name) {}
        void accept(Visitor *v) override { v->visitRecordAccess(this); }
    };

} // namespace ast

#endif // AST_H