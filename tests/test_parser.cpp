#define BOOST_TEST_MODULE TestParser
#include <boost/test/unit_test.hpp>
#include "../inc/parser.hpp"
#include <cassert>
#include <map>
using namespace std;
using namespace token;
using namespace lexer;
using namespace parser;

const map<string, string> trivialCases {
    {
"1",
"(IntegerLiteral:\n\
    value=1)"
    },
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
};

void testTrivial(const string& input, const string& expected) {
    istringstream in(input);
    Parser p(in);
    unique_ptr<Node> output = p.generateParsingTree();
    TreeGenVisitor visitor;
    output->accept(visitor);
    string received = visitor.getResult();
    BOOST_CHECK_EQUAL(expected, received);
}

BOOST_AUTO_TEST_CASE(testTrivialCases) {
    for (const auto& [i, e] : trivialCases) {
        testTrivial(i, e);
    }
}

BOOST_AUTO_TEST_CASE(testIntegerAddition) {
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
    BOOST_CHECK_EQUAL(received, expected);
}

