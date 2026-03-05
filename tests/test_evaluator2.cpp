#include "ast_context.hpp"
#include "evaluator2.hpp"
#include "node2.hpp"
#include "operator.hpp"
#include <gtest/gtest.h>

using namespace pheonix;

class EvaluatorTest : public ::testing::Test {
protected:
  ASTContext context;
  Evaluator eval;
};

TEST_F(EvaluatorTest, EvaluatesEmptyNode) {
  Node emptyNode{};
  eval.eval(emptyNode);
  EXPECT_EQ(eval.result(), 0);
}

TEST_F(EvaluatorTest, EvaluatesLiteralValues) {
  auto *literal42 = context.make<Literal>(42);
  eval.eval(*literal42);
  EXPECT_EQ(eval.result(), 42);

  auto *literal100 = context.make<Literal>(100);
  eval.eval(*literal100);
  EXPECT_EQ(eval.result(), 100);
}

TEST_F(EvaluatorTest, EvaluatesInfixAddition) {
  auto *a = context.make<Literal>(42);
  auto *b = context.make<Literal>(10);
  auto op = OperatorType::Add;

  auto *expr = context.make<InfixExpression>(op, a, b);

  eval.eval(*expr);
  EXPECT_EQ(eval.result(), 52);
}

TEST_F(EvaluatorTest, EvaluatesNestedExpressions) {
  auto *a = context.make<Literal>(10);

  auto *b_left = context.make<Literal>(2);
  auto *b_right = context.make<Literal>(3);
  auto *b = context.make<InfixExpression>(OperatorType::Add, b_left, b_right);

  auto *root = context.make<InfixExpression>(OperatorType::Add, a, b);

  eval.eval(*root);
  EXPECT_EQ(eval.result(), 15);
}

TEST_F(EvaluatorTest, EvaluatesBlock) {
  auto *stmt1 = context.make<Literal>(10);
  auto *stmt2 = context.make<Literal>(20);
  
  auto *stmt3 = context.make<InfixExpression>(OperatorType::Add, stmt1, stmt2);

  std::vector<Node *> statements = {stmt1, stmt2, stmt3};

  auto *block = context.make<Block>(std::move(statements));

  eval.eval(*block);

  EXPECT_EQ(eval.result(), 30);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
