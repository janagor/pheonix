#include "evaluator.hpp"
#include "parser.hpp"

#include <gtest/gtest.h>

using namespace std;
using namespace pheonix;
using namespace pheonix::exception;
using namespace pheonix::node;
using namespace pheonix::eval;
using namespace pheonix::types;
using namespace pheonix::context;
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
    {"let b = 1;fn v(c) {c = 2;} v(b);; b;", Integer(2)},
    {"let b = 1;fn a(c) {2;};12;a(b);", Integer(2)},
    {"let b = 1;fn a(c) {c+1;};12;a(b);", Integer(2)},
    {"let b = 1;\
      fn a(c) {\
        if (true) {\
          return 2;\
        }\
        return 0;\
      }\
      12;\
      let g = a(b);;;;\
      g;",
     Integer(2)},
};
TEST(TestEvaluator, testArithmetic) {
  for (const auto &[i, p] : ARITHMETIC) {
    compareExpectedAndReceived(i, p);
  }
}
const map<string, ObjectValue> A{
    {"let a = 11;\
      let b = #(x){return x*a;};\
      let c = b(a);\
      ;\
      c;",
     Integer(121)},
};
TEST(TestEvaluator, Aaaa) {
  for (const auto &[i, p] : A) {
    compareExpectedAndReceived(i, p);
  }
}

const vector<std::tuple<string, ObjectValue, int>> FUNCTIONS{
    {"#(){};", Function(), 0},
    {"#(a,b,c,d){};", Function(), 4},
};

TEST(TestEvaluator, testFunctions) {
  for (const auto &[i, p, n] : FUNCTIONS) {
    compareFunctions(i, p, n);
  }
}

TEST(TestEvaluator, Shit) {
  Context context;
  Function function(std::vector<string>{"a", "b"}, nullptr);
  Object ff = Object(function);
  context.insert("c", ff);
  context.insertRef("b", "c");
  std::get<Function>(context.at("c").value).args.push_back("aaaaa");
  EXPECT_EQ(std::get<Function>(context.at("c").value).args.size(), 3);
  EXPECT_EQ(std::get<Function>(context.at("b").value).args.size(), 3);
}
