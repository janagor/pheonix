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

TEST(TestEvaluator, Literal) {
  string input = "1;";
  istringstream in(input);
  Parser p(in);
  unique_ptr<Node> output = p.generateParsingTree();
  Evaluator visitor;
  output->accept(visitor);
  auto received = visitor.getResult();
  int res = std::get<Integer>(received).getValue();
  EXPECT_EQ(1, res);
}
