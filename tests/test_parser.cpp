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
"1;",
"(ExpressionStatement:\n\
    expression=(IntegerLiteral:\n\
        value=1))"
    },
    // CastExpression
    {
"1<-int;",
"(ExpressionStatement:\n\
    expression=(CastExpression:\n\
        expression=(IntegerLiteral:\n\
            value=1),\n\
        type=(TypeSpecifier:\n\
            value=int)))"
    },
    {
"1<-str;",
"(ExpressionStatement:\n\
    expression=(CastExpression:\n\
        expression=(IntegerLiteral:\n\
            value=1),\n\
        type=(TypeSpecifier:\n\
            value=str)))"
    },
    {
"1<-flt;",
"(ExpressionStatement:\n\
    expression=(CastExpression:\n\
        expression=(IntegerLiteral:\n\
            value=1),\n\
        type=(TypeSpecifier:\n\
            value=flt)))"
    },
    {
"1<-bol;",
"(ExpressionStatement:\n\
    expression=(CastExpression:\n\
        expression=(IntegerLiteral:\n\
            value=1),\n\
        type=(TypeSpecifier:\n\
            value=bol)))"
    },
    // PrefixExpression
    {
"-2;",
"(ExpressionStatement:\n\
    expression=(PrefixExpression:\n\
        operator=[-],\n\
        expression=(IntegerLiteral:\n\
            value=2)))"
    },
    {
"!2;",
"(ExpressionStatement:\n\
    expression=(PrefixExpression:\n\
        operator=[!],\n\
        expression=(IntegerLiteral:\n\
            value=2)))"
    },
    // AdditiveExpression
    {
"1+2;",
"(ExpressionStatement:\n\
    expression=(AdditiveExpression:\n\
        left=(IntegerLiteral:\n\
            value=1),\n\
        operator=[+],\n\
        right=(IntegerLiteral:\n\
            value=2)))"
    },
    {
"100-100;",
"(ExpressionStatement:\n\
    expression=(AdditiveExpression:\n\
        left=(IntegerLiteral:\n\
            value=100),\n\
        operator=[-],\n\
        right=(IntegerLiteral:\n\
            value=100)))"
    },
    // MultipliveExpression
    {
"9*123;",
"(ExpressionStatement:\n\
    expression=(MultiplicativeExpression:\n\
        left=(IntegerLiteral:\n\
            value=9),\n\
        operator=[*],\n\
        right=(IntegerLiteral:\n\
            value=123)))"
    },
    {
"9/123;",
"(ExpressionStatement:\n\
    expression=(MultiplicativeExpression:\n\
        left=(IntegerLiteral:\n\
            value=9),\n\
        operator=[/],\n\
        right=(IntegerLiteral:\n\
            value=123)))"
    },
    // RelationalExpression
    {
"9<123;",
"(ExpressionStatement:\n\
    expression=(RelationalExpression:\n\
        left=(IntegerLiteral:\n\
            value=9),\n\
        operator=[<],\n\
        right=(IntegerLiteral:\n\
            value=123)))"
    },
    {
"9<=123;",
"(ExpressionStatement:\n\
    expression=(RelationalExpression:\n\
        left=(IntegerLiteral:\n\
            value=9),\n\
        operator=[<=],\n\
        right=(IntegerLiteral:\n\
            value=123)))"
    },
    {
"9>123;",
"(ExpressionStatement:\n\
    expression=(RelationalExpression:\n\
        left=(IntegerLiteral:\n\
            value=9),\n\
        operator=[>],\n\
        right=(IntegerLiteral:\n\
            value=123)))"
    },
    {
"9>=123;",
"(ExpressionStatement:\n\
    expression=(RelationalExpression:\n\
        left=(IntegerLiteral:\n\
            value=9),\n\
        operator=[>=],\n\
        right=(IntegerLiteral:\n\
            value=123)))"
    },
    // ComparisonExpression
    {
"9==123;",
"(ExpressionStatement:\n\
    expression=(ComparisonExpression:\n\
        left=(IntegerLiteral:\n\
            value=9),\n\
        operator=[==],\n\
        right=(IntegerLiteral:\n\
            value=123)))"
    },
    {
"9!=123;",
"(ExpressionStatement:\n\
    expression=(ComparisonExpression:\n\
        left=(IntegerLiteral:\n\
            value=9),\n\
        operator=[!=],\n\
        right=(IntegerLiteral:\n\
            value=123)))"
    },
    // AndExpression
    {
"9&&123;",
"(ExpressionStatement:\n\
    expression=(AndExpression:\n\
        left=(IntegerLiteral:\n\
            value=9),\n\
        operator=[&&],\n\
        right=(IntegerLiteral:\n\
            value=123)))"
    },
    // OrExpression
    {
"9||123;",
"(ExpressionStatement:\n\
    expression=(OrExpression:\n\
        left=(IntegerLiteral:\n\
            value=9),\n\
        operator=[||],\n\
        right=(IntegerLiteral:\n\
            value=123)))"
    },
    // AssignementExpression
    {
"$a=123;",
"(ExpressionStatement:\n\
    expression=(AssignementExpression:\n\
        identifier=a,\n\
        expression=(IntegerLiteral:\n\
            value=123)))"
    },
    // VariableDeclaration
    {
"let a = 123;",
"(VariableDeclaration:\n\
    isMutable=false,\n\
    identifier=a,\n\
    expression=(ExpressionStatement:\n\
        expression=(IntegerLiteral:\n\
            value=123)))"
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

