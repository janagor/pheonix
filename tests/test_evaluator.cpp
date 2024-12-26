#include "evaluator.hpp"
#include "parser.hpp"

#include <gtest/gtest.h>

using namespace std;
using namespace pheonix;
using namespace pheonix::exception;
using namespace pheonix::node;
using namespace pheonix::eval;
using namespace pheonix::types;
using namespace pheonix::lexer;
using namespace pheonix::parser;

void compareExpectedAndReceived(const string &input,
                                const ObjectValue &expected) {
  istringstream in(input);
  Parser p(in);
  unique_ptr<Node> output = p.generateParsingTree();
  Evaluator visitor;
  output->accept(visitor);
  ObjectValue received = visitor.getResult().value;
  EXPECT_EQ(expected, received);
}

void compareFunctions(const string &input, const ObjectValue &expected,
                      int size) {
  istringstream in(input);
  Parser p(in);
  unique_ptr<Node> output = p.generateParsingTree();
  Evaluator visitor;
  output->accept(visitor);
  auto received = visitor.getResult().value;
  EXPECT_EQ(expected, received);
  EXPECT_EQ(visitor.getResultVec().size(), size);
}

const map<string, ObjectValue> ARITHMETIC{
    // only integers
    {"2147483647;", Integer(2147483647)},
    {"-3;", Integer(-3)},
    {"1 + 2;", Integer(3)},
    {"1 - 2;", Integer(-1)},
    {"3 * 4;", Integer(12)},
    {"5 / 3;", Integer(1)},
    {"5 % 3;", Integer(2)},
    {"1 > 2;", false},
    {"1 < 2;", true},
    {"2 < 1;", false},
    {"1 >= 2;", false},
    {"1 <= 2;", true},
    {"1 == 2;", false},
    {"1 != 2;", true},
    // only floats
    {"2147483647.;", Float(2147483647.)},
    {"-3.;", Float(-3.)},
    {"1. + 2.;", Float(3.)},
    {"1. - 2.;", Float(-1.)},
    {"3. * 4.;", Float(12.)},
    {"5. / 3.;", Float(5. / 3.)},
    {"1. > 2.;", false},
    {"1. < 2.;", true},
    {"1. >= 2.;", false},
    {"1. <= 2.;", true},
    {"1. == 2.;", false},
    {"1. != 2.;", true},
    // only bools
    {"true;", true},
    {"false;", false},
    {"!true;", false},
    {"!false;", true},
    {"true && false;", false},
    {"true || false;", true},
    // assignement
    {"let b = 1;let c = 2;let d = b;", Integer(1)},
    {"let b = 1;while(b<12){b = b+1;}b;", Integer(12)},
    {"let b = 1;if(true){b=12;}else{b=113;}b;", Integer(12)},
    {"let b = 1;if(false){b=12;}else{b=113;}b;", Integer(113)},
};
TEST(TestEvaluator, testArithmetic) {
  for (const auto &[i, p] : ARITHMETIC) {
    compareExpectedAndReceived(i, p);
  }
}

const vector<std::tuple<string, ObjectValue, int>> FUNCTIONS{
    {"fn a(){}", Function(), 0},
    {"fn a(a,b,c,d){}", Function(), 4},
};

TEST(TestEvaluator, testFunctions) {
  for (const auto &[i, p, n] : FUNCTIONS) {
    compareFunctions(i, p, n);
  }
}
