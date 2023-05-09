#ifndef ENUMS_H
#define ENUMS_H

#include <iostream>

enum Type
{
    Integer,
    Real,
    Boolean,
    Array,
    Record
};

enum BinaryOperator
{
    Plus,
    Minus,
    Mul,
    Div,
    Mod,
};

enum LogicOperator
{
    And,
    Or,
    Xor,
};

enum ComparisonOperator
{
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
};

#endif // ENUMS_H