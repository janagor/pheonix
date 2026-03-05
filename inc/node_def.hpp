#pragma once

#include <memory>
#include <variant>

namespace pheonix::node2 {
// struct Program;
// struct Parameter;
// struct DeclarationArguments;
// struct Block;
// struct FunctionDeclaration;
// struct VariableDeclaration;
// struct WhileLoopStatement;
// struct IfStatement;
// struct ReturnStatement;
// struct ExpressionStatement;
// struct NullStatement;
// struct AssignementExpression;
// struct OrExpression;
// struct AndExpression;
// struct ComparisonExpression;
// struct RelationalExpression;
struct InfixExpression;
// struct MultiplicativeExpression;
// struct CompositiveExpression;
// struct CastExpression;
// struct PrefixExpression;
// struct CallExpression;
// struct DebugExpression;
// struct CallArguments;
// struct LambdaExpression;
// struct Identifier;
// struct ParentExpression;
struct Literal;
// struct TypeSpecifier;
// struct PrintFunction;

// clang-format off
using Node = std::variant<
    std::monostate,
    // Program,
    // Parameter,
    // DeclarationArguments,
    // Block,
    // FunctionDeclaration,
    // VariableDeclaration,
    // WhileLoopStatement,
    // IfStatement,
    // ReturnStatement,
    // ExpressionStatement,
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
    // Identifier,
    // ParentExpression,
    Literal
    // TypeSpecifier,
    // PrintFunction
>;
// clang-format on

} // namespace pheonix::node2
