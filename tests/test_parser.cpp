#include "../inc/parser.hpp"
#include <gtest/gtest.h>
#include <cassert>
#include <map>
using namespace std;
using namespace token;
using namespace lexer;
using namespace parser;

const map<string, string> TRIVIAL_CASES {
    //IntegerLiteral
    {
"1",
"(IntegerLiteral:\n\
    value=1)"
    },
    //TypeSpecifier
    /*
    {
"str",
"(TypeSpecifier:\n\
    value=str)"
    },
    {
"int",
"(TypeSpecifier:\n\
    value=int)"
    },
    {
"flt",
"(TypeSpecifier:\n\
    value=flt)"
    },
    {
"bol",
"(TypeSpecifier:\n\
    value=bol)"
    },
    */
    // CastExpression
    {
"1<-int",
"(CastExpression:\n\
    expression=(IntegerLiteral:\n\
        value=1),\n\
    type=(TypeSpecifier:\n\
        value=int))"
    },
    {
"1<-str",
"(CastExpression:\n\
    expression=(IntegerLiteral:\n\
        value=1),\n\
    type=(TypeSpecifier:\n\
        value=str))"
    },
    {
"1<-flt",
"(CastExpression:\n\
    expression=(IntegerLiteral:\n\
        value=1),\n\
    type=(TypeSpecifier:\n\
        value=flt))"
    },
    {
"1<-bol",
"(CastExpression:\n\
    expression=(IntegerLiteral:\n\
        value=1),\n\
    type=(TypeSpecifier:\n\
        value=bol))"
    },
    // AdditiveExpression
    {
"1+2",
"(AdditiveExpression:\n\
    left=(IntegerLiteral:\n\
        value=1),\n\
    operator=[+],\n\
    right=(IntegerLiteral:\n\
        value=2))"
    },
    {
"100-100",
"(AdditiveExpression:\n\
    left=(IntegerLiteral:\n\
        value=100),\n\
    operator=[-],\n\
    right=(IntegerLiteral:\n\
        value=100))"
    },
    // MultipliveExpression
    {
"9*123",
"(MultiplicativeExpression:\n\
    left=(IntegerLiteral:\n\
        value=9),\n\
    operator=[*],\n\
    right=(IntegerLiteral:\n\
        value=123))"
    },
    {
"9/123",
"(MultiplicativeExpression:\n\
    left=(IntegerLiteral:\n\
        value=9),\n\
    operator=[/],\n\
    right=(IntegerLiteral:\n\
        value=123))"
    },
    // RelationalExpression
    {
"9<123",
"(RelationalExpression:\n\
    left=(IntegerLiteral:\n\
        value=9),\n\
    operator=[<],\n\
    right=(IntegerLiteral:\n\
        value=123))"
    },
    {
"9<=123",
"(RelationalExpression:\n\
    left=(IntegerLiteral:\n\
        value=9),\n\
    operator=[<=],\n\
    right=(IntegerLiteral:\n\
        value=123))"
    },
    {
"9>123",
"(RelationalExpression:\n\
    left=(IntegerLiteral:\n\
        value=9),\n\
    operator=[>],\n\
    right=(IntegerLiteral:\n\
        value=123))"
    },
    {
"9>=123",
"(RelationalExpression:\n\
    left=(IntegerLiteral:\n\
        value=9),\n\
    operator=[>=],\n\
    right=(IntegerLiteral:\n\
        value=123))"
    },
    // ComparisonExpression
    {
"9==123",
"(ComparisonExpression:\n\
    left=(IntegerLiteral:\n\
        value=9),\n\
    operator=[==],\n\
    right=(IntegerLiteral:\n\
        value=123))"
    },
    {
"9!=123",
"(ComparisonExpression:\n\
    left=(IntegerLiteral:\n\
        value=9),\n\
    operator=[!=],\n\
    right=(IntegerLiteral:\n\
        value=123))"
    },
    // AndExpression
    {
"9&&123",
"(AndExpression:\n\
    left=(IntegerLiteral:\n\
        value=9),\n\
    operator=[&&],\n\
    right=(IntegerLiteral:\n\
        value=123))"
    },
    // OrExpression
    {
"9||123",
"(OrExpression:\n\
    left=(IntegerLiteral:\n\
        value=9),\n\
    operator=[||],\n\
    right=(IntegerLiteral:\n\
        value=123))"
    },
};
void testTrivial(const string& input, const string& expected) {
    istringstream in(input);
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    TreeGenVisitor visitor;
    output->accept(visitor);
    string received = visitor.getResult();
    EXPECT_EQ(expected, received);
}

TEST(TestParser, testTrivialCases) {
    for (const auto& [i, e] : TRIVIAL_CASES) {
        testTrivial(i, e);
    }
}

TEST(TestParser, testIntegerAddition) {
    string input = R"(1 + 2 * 3)";
    string expected = 
"(AdditiveExpression:\n\
    left=(IntegerLiteral:\n\
        value=1),\n\
    operator=[+],\n\
    right=(MultiplicativeExpression:\n\
        left=(IntegerLiteral:\n\
            value=2),\n\
        operator=[*],\n\
        right=(IntegerLiteral:\n\
            value=3)))";
    istringstream in(input);
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    TreeGenVisitor visitor;
    output->accept(visitor);
    string received = visitor.getResult();
    EXPECT_EQ(received, expected);
}

