#pragma once

#include <memory>
#include <variant>

namespace pheonix {
// class Program;
// class Parameter;
// class DeclarationArguments;
class Block;
// class FunctionDeclaration;
class VariableDeclaration;
// class WhileLoopStatement;
class IfExpression;
// class ReturnStatement;
class ExpressionStatement;
// class NullStatement;
// class AssignementExpression;
// class OrExpression;
// class AndExpression;
// class ComparisonExpression;
// class RelationalExpression;
class InfixExpression;
// class MultiplicativeExpression;
// class CompositiveExpression;
// class CastExpression;
// class PrefixExpression;
// class CallExpression;
// class DebugExpression;
// class CallArguments;
// class LambdaExpression;
class Identifier;
// class ParentExpression;
class Literal;
// class TypeSpecifier;
// class PrintFunction;

// clang-format off
using Node = std::variant<
    std::monostate,
    // Program,
    // Parameter,
    // DeclarationArguments,
    Block,
    // FunctionDeclaration,
    VariableDeclaration,
    // WhileLoopStatement,
    IfExpression,
    // ReturnStatement,
    ExpressionStatement,
    // NullStatement,
    // AssignementExpression,
    // OrExpression,
    // AndExpression,
    // ComparisonExpression,
    // RelationalExpression,
    InfixExpression,
    // MultiplicativeExpression,
    // CompositiveExpression,
    // CastExpression,
    // PrefixExpression,
    // CallExpression,
    // DebugExpression,
    // CallArguments,
    // LambdaExpression,
    Identifier,
    // ParentExpression,
    Literal
    // TypeSpecifier,
    // PrintFunction
>;
// clang-format on

} // namespace pheonix
