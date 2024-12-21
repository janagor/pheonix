#include "evaluator.hpp"
#include "parser.hpp"

#include <gtest/gtest.h>

using namespace std;
using namespace pheonix::exception;
using namespace pheonix::node;
using namespace pheonix::eval;
using namespace pheonix::types;
using namespace pheonix::lexer;
using namespace pheonix::parser;

// helper
void compareExpectedAndReceived(const string &input,
                                const Primitive &expected) {
  istringstream in(input);
  Parser p(in);
  unique_ptr<Node> output = p.generateParsingTree();
  Evaluator visitor;
  output->accept(visitor);
  auto received = visitor.getResult();
  EXPECT_EQ(expected, received);
}

const map<string, Primitive> ARITHMETIC{
    {"1;", Integer(1)},      {"1 + 2;", Integer(3)}, {"1 - 2;", Integer(-1)},
    {"3 * 4;", Integer(12)}, {"5 / 3;", Integer(1)}, {"5 % 3;", Integer(2)},
    {"-3;", Integer(-3)},
};
TEST(TestEvaluator, testArithmetic) {
  for (const auto &[i, p] : ARITHMETIC) {
    compareExpectedAndReceived(i, p);
  }
}
