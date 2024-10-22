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

BOOST_AUTO_TEST_CASE(testOneLineComments) {
    string input =
"//abcdefg    \t\t!@@#$\n\
++// kaczuszka";
    vector<Lexem> expected {
        {Token(ONE_LINE_COMMENT, "//abcdefg    \t\t!@@#$"), 1, 1},
        {Token(PLUS), 2, 1},
        {Token(PLUS), 2, 2},
        {Token(ONE_LINE_COMMENT, "// kaczuszka"), 2, 3},
        {Token(END_OF_FILE), 2, 15},
    };
    std::istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testOneLineComments2) {
    string input =
"//abcdefg    \t\t!@@#$\n//";
    vector<Lexem> expected {
        {Token(ONE_LINE_COMMENT, "//abcdefg    \t\t!@@#$"), 1, 1},
        {Token(ONE_LINE_COMMENT, "//"), 2, 1},
        {Token(END_OF_FILE), 2, 3},
    };
    std::istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testMultilineComments) {
    string input =
"/*abcdefll\n\
\n\
\n\
;'.,:1*/+=";
    vector<Lexem> expected {
        {Token(MULTILINE_COMMENT, "/*abcdefll\n\n\n;'.,:1*/"), 1, 1},
        {Token(PLUS), 4, 9},
        {Token(EQUALS), 4, 10},
        {Token(END_OF_FILE), 4, 11},
    };
    std::istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testMultilineComments2) {
    string input =
"/*abcdefll\n\
\n\
\n\
;'.,:1";
    vector<Lexem> expected {
        {Token(UNFINISHED_COMMENT, "/*abcdefll\n\n\n;'.,:1"), 1, 1},
        {Token(END_OF_FILE), 4, 7},
    };
    std::istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

