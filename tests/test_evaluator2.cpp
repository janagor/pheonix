#include "ast_context.hpp"
#include "evaluator2.hpp"
#include "node2.hpp"
#include "operator.hpp"
#include <gtest/gtest.h>
#include <variant>

using namespace pheonix;

class EvaluatorTest : public ::testing::Test {

protected:
  ASTContext context;
  Evaluator eval;
};

TEST_F(EvaluatorTest, EvaluatesEmptyNode) {
  Node emptyNode{};
  eval.eval(emptyNode);
  EXPECT_EQ(std::get<std::monostate>(eval.result()), std::monostate{});
}

TEST_F(EvaluatorTest, EvaluatesLiteralValues) {
  auto *literal42 = context.make<Literal>(42);
  eval.eval(*literal42);
  EXPECT_EQ(std::get<Int>(eval.result()), 42);
  auto *literal100 = context.make<Literal>(100);
  eval.eval(*literal100);
  EXPECT_EQ(std::get<Int>(eval.result()), 100);
}

TEST_F(EvaluatorTest, EvaluatesInfixAddition) {
  auto *a = context.make<Literal>(42);
  auto *b = context.make<Literal>(10);
  auto op = OperatorType::Add;
  auto *expr = context.make<InfixExpression>(op, a, b);
  eval.eval(*expr);
  EXPECT_EQ(std::get<Int>(eval.result()), 52);
}

TEST_F(EvaluatorTest, EvaluatesNestedExpressions) {
  auto *a = context.make<Literal>(10);
  auto *b_left = context.make<Literal>(2);
  auto *b_right = context.make<Literal>(3);
  auto *b = context.make<InfixExpression>(OperatorType::Add, b_left, b_right);
  auto *root = context.make<InfixExpression>(OperatorType::Add, a, b);
  eval.eval(*root);
  EXPECT_EQ(std::get<Int>(eval.result()), 15);
}

TEST_F(EvaluatorTest, EvaluatesBlock) {
  auto *stmt1 = context.make<Literal>(10);
  auto *stmt2 = context.make<Literal>(20);
  auto *stmt3 = context.make<InfixExpression>(OperatorType::Add, stmt1, stmt2);
  auto statements = std::vector<Node *>{stmt1, stmt2, stmt3};
  auto *block = context.make<Block>(std::move(statements));
  eval.eval(*block);
  EXPECT_EQ(std::get<Int>(eval.result()), 30);
}

TEST_F(EvaluatorTest, EvaluatesExpressionStatement) {
  auto *literal = context.make<Literal>(99);
  auto *exprStmt = context.make<ExpressionStatement>(literal);
  eval.eval(*exprStmt);
  EXPECT_EQ(std::get<Int>(eval.result()), 99);
}

TEST_F(EvaluatorTest, EvaluatesVariableDeclarationAndIdentifier) {
  auto *literal = context.make<Literal>(123);
  auto *varDecl = context.make<VariableDeclaration>("x", literal);

  eval.eval(*varDecl);

  auto *identifier = context.make<Identifier>("x");

  eval.result() = 0;

  eval.eval(*identifier);

  EXPECT_EQ(std::get<Int>(eval.result()), 123);
}

TEST_F(EvaluatorTest, EvaluatesVariablesInExpressions) {
  auto *val42 = context.make<Literal>(42);
  auto *varDecl = context.make<VariableDeclaration>("my_var", val42);
  eval.eval(*varDecl);

  auto *ident = context.make<Identifier>("my_var");
  auto *val10 = context.make<Literal>(10);
  auto *addition =
      context.make<InfixExpression>(OperatorType::Add, ident, val10);

  eval.eval(*addition);

  EXPECT_EQ(std::get<Int>(eval.result()), 52);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
