#define BOOST_TEST_MODULE TestLexer
#include <boost/test/unit_test.hpp>
#include "../inc/lexer.hpp"
#include <cassert>
using namespace std;
using namespace lexer;

void compareLexemVectors(const vector<Lexem>& expected, const vector<Lexem>& received) {
    BOOST_CHECK_EQUAL(expected.size(), received.size());

    for (size_t i = 0; i < expected.size(); ++i) {
        BOOST_CHECK_EQUAL(expected[i].token.tokenType, received[i].token.tokenType);
        BOOST_CHECK_EQUAL(expected[i].token.value.has_value(), received[i].token.value.has_value());
        if (expected[i].token.value.has_value(), received[i].token.value.has_value()) 
            BOOST_CHECK_EQUAL(*expected[i].token.value, *received[i].token.value);

        BOOST_CHECK_EQUAL(expected[i].line, received[i].line);
        BOOST_CHECK_EQUAL(expected[i].column, received[i].column);
    }
}

BOOST_AUTO_TEST_CASE(testEmptyInput) {
    string input = R"()";
    vector<Lexem> expected {
        {Token(END_OF_FILE), 1, 1},
    };
    std::istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testOperators) {
    string input = R"(+=*/-)";
    vector<Lexem> expected {
        {Token(PLUS), 1, 1},
        {Token(EQUALS), 1, 2},
        {Token(STAR), 1, 3},
        {Token(SLASH), 1, 4},
        {Token(MINUS), 1, 5},
        {Token(END_OF_FILE), 1, 6},
    };
    std::istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testBracketsAndSigns) {
    string input = R"((){}[]"':;,.)";
    vector<Lexem> expected {
        {Token(LPARENT), 1, 1},
        {Token(RPARENT), 1, 2},
        {Token(LBRACES), 1, 3},
        {Token(RBRACES), 1, 4},
        {Token(LBRACKETS), 1, 5},
        {Token(RBRACKETS), 1, 6},
        {Token(DOUBLE_QUOTE), 1, 7},
        {Token(SINGLE_QUOTE), 1, 8},
        {Token(COLON), 1, 9},
        {Token(SEMICOLON), 1, 10},
        {Token(COMMA), 1, 11},
        {Token(DOT), 1, 12},
        {Token(END_OF_FILE), 1, 13},
    };
    std::istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

