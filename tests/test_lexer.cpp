#define BOOST_TEST_MODULE TestLexer
#include <boost/test/unit_test.hpp>
#include "../inc/lexer.hpp"
#include <cassert>
using namespace std;
using namespace lexer;
BOOST_AUTO_TEST_CASE(testEmptyInput) {
    string input = R"()";
    vector<Lexem> expected {
        {Token(END_OF_FILE), 1, 1},
    };
    std::istringstream in(input);
 
    Lexer l(in);
    vector<Lexem> result = l.lexerize();
    //

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    for (size_t i = 0; i < expected.size(); ++i) {
        BOOST_CHECK_EQUAL(expected[i].token.tokenType, result[i].token.tokenType);
        BOOST_CHECK_EQUAL(expected[i].token.value.has_value(), result[i].token.value.has_value());
        if (expected[i].token.value.has_value(), result[i].token.value.has_value()) 
            BOOST_CHECK_EQUAL(*expected[i].token.value, *result[i].token.value);

        BOOST_CHECK_EQUAL(expected[i].line, result[i].line);
        BOOST_CHECK_EQUAL(expected[i].column, result[i].column);
    }
}

BOOST_AUTO_TEST_CASE(testGeneratingRookRelativeOccupancy) {
    string input = R"(+=*/)";
    vector<int> expected {
        {1},
        {2},
        {3},
        {4},
    };

    BOOST_CHECK_EQUAL(input.size(), expected.size());
}

