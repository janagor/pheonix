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
    {R"("Kaczka";)", "Kaczka"},
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
    {"true;", true},
    {"false;", false},
    {"!true;", false},
    {"!false;", true},
    {"true && false;", false},
    {"true || false;", true},
    {"1<-str;", "1"},
    {"1.12<-str;", "1.120000"},
    {"true<-str;", "true"},
    {"12<-flt;", Float(12)},
    {"false<-flt;", Float(0.)},
    {"true<-flt;", Float(1.)},
    //
    {"false<-int;", Integer(0)},
    {"true<-int;", Integer(1)},
    //
    {"0<-bol;", false},
    {"1<-bol;", true},
    {"2<-bol;", true},
    {"let b = 1;let c = 2;let d = b;", Integer(1)},
    {"let mut b = 1;while(b<12){b = b+1;}b;", Integer(12)},
    {"let mut b = 1;if(true){b=12;}else{b=113;}b;", Integer(12)},
    {"let mut b = 1;if(false){b=12;}else{b=113;}b;", Integer(113)},

    {"let mut b = 1;fn v(mut c) {c = 2;} v(b);; b;", Integer(2)},
    {"let b = 1;fn a(c) {2;};12;a(b);", Integer(2)},
    {"let b = 1;fn a(c) {c+1;};12;a(b);", Integer(2)},
    {"let b = 1;\
      fn a(c,d) {return c+d;};12;a(1,b);",
     Integer(2)},
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
    {"let b = 1;\
      fn a(c) {\
        return c+1;\
      }\
      12;\
      let h = a|a;\
      let g = h(b);;;;\
      g;",
     Integer(3)},
    {"let b = 1;\
          fn a(c) {\
            return c+1;\
          }\
          12;\
          let h = a|a;\
          let g = [h](b);;;;\
          g;",
     Integer(3)},
    {"let mut a = 0;\
          \
          fn increment(mut a) {\
            a = a + 1;\
            return a;\
          }\
          fn add_one(a) { return a + 1; }\
          \
          if (a != 0) {\
            a = increment;\
          } else {\
            a = add_one;\
          }\
          let mut b = 12;\
          let c = a(b);",
     Integer(13)},
    {"\
      fn is_prime(num) {\
        fn is_prime_rec(n, divisor) {\
          if (n <= 1) {\
            return false;\
          } else {\
            if (divisor == 1) {\
              return true;\
            } else {\
              if (n % divisor == 0) {\
                return false;\
              } else {\
                return is_prime_rec(n, divisor - 1);\
              }\
            }\
          }\
        }\
        return is_prime_rec(num, num - 1);\
      }\
      is_prime(16);\
   ",
     false},
};
TEST(TestEvaluator, testArithmetic) {
  for (const auto &[i, p] : ARITHMETIC) {
    compareExpectedAndReceived(i, p);
  }
}
