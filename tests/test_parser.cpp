#define BOOST_TEST_MODULE TestParser
#include <boost/test/unit_test.hpp>
#include "../inc/parser.hpp"
#include <cassert>
using namespace std;
using namespace token;
using namespace lexer;
using namespace parser;

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

