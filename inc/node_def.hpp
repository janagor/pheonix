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
    // std::unique_ptr<Program>,
    // std::unique_ptr<Parameter>,
    // std::unique_ptr<DeclarationArguments>,
    // std::unique_ptr<Block>,
    // std::unique_ptr<FunctionDeclaration>,
    // std::unique_ptr<VariableDeclaration>,
    // std::unique_ptr<WhileLoopStatement>,
    // std::unique_ptr<IfStatement>,
    // std::unique_ptr<ReturnStatement>,
    // std::unique_ptr<ExpressionStatement>,
    // std::unique_ptr<NullStatement>,
    // std::unique_ptr<AssignementExpression>,
    // std::unique_ptr<OrExpression>,
    // std::unique_ptr<AndExpression>,
    // std::unique_ptr<ComparisonExpression>,
    // std::unique_ptr<RelationalExpression>,
    std::unique_ptr<InfixExpression>,
    // std::unique_ptr<MultiplicativeExpression>,
    // std::unique_ptr<CompositiveExpression>,
    // std::unique_ptr<CastExpression>,
    // std::unique_ptr<PrefixExpression>,
    // std::unique_ptr<CallExpression>,
    // std::unique_ptr<DebugExpression>,
    // std::unique_ptr<CallArguments>,
    // std::unique_ptr<LambdaExpression>,
    // std::unique_ptr<Identifier>,
    // std::unique_ptr<ParentExpression>,
    std::unique_ptr<Literal>
    // std::unique_ptr<TypeSpecifier>,
    // std::unique_ptr<PrintFunction>
>;
// clang-format on

} // namespace pheonix::node2
