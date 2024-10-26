#define BOOST_TEST_MODULE TestLexer
#include <boost/test/unit_test.hpp>
#include "../inc/lexer.hpp"
#include <cassert>
using namespace std;
using namespace lexer;

void compareLexemVectors(const vector<Lexem>& expected, const vector<Lexem>& received) {
    BOOST_CHECK_EQUAL(expected.size(), received.size());

    for (size_t i = 0; i < expected.size(); ++i) {
        BOOST_CHECK_EQUAL(expected[i], received[i]);
        // BOOST_CHECK_EQUAL(expected[i].token, received[i].token);
        //
        // BOOST_CHECK_EQUAL(expected[i].line, received[i].line);
        // BOOST_CHECK_EQUAL(expected[i].column, received[i].column);
    }
}

BOOST_AUTO_TEST_CASE(testEmptyInput) {
    string input = R"()";
    vector<Lexem> expected {
        {Token(END_OF_FILE), 1, 1},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testOperators) {
    string input = R"(+=*/-)";
    vector<Lexem> expected {
        {Token(PLUS), 1, 1},
        {Token(ASSIGN), 1, 2},
        {Token(STAR), 1, 3},
        {Token(SLASH), 1, 4},
        {Token(MINUS), 1, 5},
        {Token(END_OF_FILE), 1, 6},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testBracketsAndSigns) {
    string input = R"((){}[]'':;,.)";
    vector<Lexem> expected {
        {Token(LPARENT), 1, 1},
        {Token(RPARENT), 1, 2},
        {Token(LBRACE), 1, 3},
        {Token(RBRACE), 1, 4},
        {Token(LBRACKET), 1, 5},
        {Token(RBRACKET), 1, 6},
        {Token(SINGLE_QUOTE), 1, 7},
        {Token(SINGLE_QUOTE), 1, 8},
        {Token(COLON), 1, 9},
        {Token(SEMICOLON), 1, 10},
        {Token(COMMA), 1, 11},
        {Token(DOT), 1, 12},
        {Token(END_OF_FILE), 1, 13},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testOneLineComments) {
    string input =
"//abcdefg1123\n\
++//3kaczuszka";
    vector<Lexem> expected {
        {Token(ONE_LINE_COMMENT, "//abcdefg1123"), 1, 1},
        {Token(PLUS), 2, 1},
        {Token(PLUS), 2, 2},
        {Token(ONE_LINE_COMMENT, "//3kaczuszka"), 2, 3},
        {Token(END_OF_FILE), 2, 15},
    };
    istringstream in(input);
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
    istringstream in(input);
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
        {Token(ASSIGN), 4, 10},
        {Token(END_OF_FILE), 4, 11},
    };
    istringstream in(input);
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
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testIdentifiers) {
    string input =
R"(abcd123
abcd
++==a__12311
)";
    vector<Lexem> expected {
        {Token(IDENTIFIER, "abcd123"), 1, 1},
        {Token(IDENTIFIER, "abcd"), 2, 1},
        {Token(PLUS), 3, 1},
        {Token(PLUS), 3, 2},
        {Token(EQUALS), 3, 3},
        {Token(IDENTIFIER, "a__12311"), 3, 5},
        {Token(END_OF_FILE), 4, 1},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testIntegerLiteralsEasier) {
    string input =
R"(123
111112 1230)";
    vector<Lexem> expected {
        {Token(INTEGER, 123), 1, 1},
        {Token(INTEGER, 111112), 2, 1},
        {Token(INTEGER, 1230), 2, 8},
        {Token(END_OF_FILE), 2, 12},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}
BOOST_AUTO_TEST_CASE(testIntegerLiterals) {
    string input =
R"(123
abcd12 1230
**789 a__12311)";
    vector<Lexem> expected {
        {Token(INTEGER, 123), 1, 1},
        {Token(IDENTIFIER, "abcd12"), 2, 1},
        {Token(INTEGER, 1230), 2, 8},
        {Token(STAR), 3, 1},
        {Token(STAR), 3, 2},
        {Token(INTEGER, 789), 3, 3},
        {Token(IDENTIFIER, "a__12311"), 3, 7},
        {Token(END_OF_FILE), 3, 15},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testIntegerLiteralsError) {
    string input =
R"(123_
abcd12 1230
**789a a__12311)";
    vector<Lexem> expected {
        {Token(ERROR_NUMBER, "123_"), 1, 1},
        {Token(IDENTIFIER, "abcd12"), 2, 1},
        {Token(INTEGER, 1230), 2, 8},
        {Token(STAR), 3, 1},
        {Token(STAR), 3, 2},
        {Token(ERROR_NUMBER, "789a"), 3, 3},
        {Token(IDENTIFIER, "a__12311"), 3, 8},
        {Token(END_OF_FILE), 3, 16},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testKeywords) {
    string input =
"123 if while";
    vector<Lexem> expected {
        {Token(INTEGER, 123), 1, 1},
        {Token(IF), 1, 5},
        {Token(WHILE), 1, 8},
        {Token(END_OF_FILE), 1, 13},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testStringsWithoutSpecialCharacters) {
    string input =
R"("kaczuszka"

"cos    ")";
    vector<Lexem> expected {
        {Token(STRING, "kaczuszka"), 1, 1},
        {Token(STRING, "cos    "), 3, 1},
        {Token(END_OF_FILE), 3, 10},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}
BOOST_AUTO_TEST_CASE(testStringsWithoutSpecialCharactersWithNewLine) {
    string input =
R"("kaczuszka

cos    ")";
    vector<Lexem> expected {
        {Token(STRING, "kaczuszka\n\ncos    "), 1, 1},
        {Token(END_OF_FILE), 3, 9},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testStringsWithDoubleQuotesInside) {
    string input =
R"("kaczuszka mowi \"Hau, Hau!\"")";
    vector<Lexem> expected {
        {Token(STRING, "kaczuszka mowi \"Hau, Hau!\""), 1, 1},
        {Token(END_OF_FILE), 1, 31},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testStringsWithNewLineCharacterInside) {
    string input =
R"("kaczuszka mowi:
- hello!
- hello!")";
    vector<Lexem> expected {
        {Token(STRING,
"kaczuszka mowi:\n\
- hello!\n\
- hello!"), 1, 1},
        {Token(END_OF_FILE), 3, 10},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testFloat) {
    string input =
R"(1.12345)";
    vector<Lexem> expected {
        {Token(DOUBLE, 1.12345), 1, 1},
        {Token(END_OF_FILE), 1, 8},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testFloat2) {
    string input =
R"(1.12345 0.0)";
    vector<Lexem> expected {
        {Token(DOUBLE, 1.12345), 1, 1},
        {Token(DOUBLE, 0.0), 1, 9},
        {Token(END_OF_FILE), 1, 12},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testFloatWithoutNumbersAfterDot) {
    string input =
R"(1.12345 0.)";
    vector<Lexem> expected {
        {Token(DOUBLE, 1.12345), 1, 1},
        {Token(DOUBLE, 0.), 1, 9},
        {Token(END_OF_FILE), 1, 11},
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

BOOST_AUTO_TEST_CASE(testAll1) {
    string input =
R"(let kaczka = if (a == 21) { 12 } else { "kaczka" };
while(x < 12) { x = x + 0.; }
let mut b = 123->str;
)";
    vector<Lexem> expected {
        {Token(LET), 1, 1},
        {Token(IDENTIFIER, "kaczka"), 1, 5},
        {Token(ASSIGN), 1, 12},
        {Token(IF), 1, 14},
        {Token(LPARENT), 1, 17},
        {Token(IDENTIFIER, "a"), 1, 18},
        {Token(EQUALS), 1, 20},
        {Token(INTEGER, 21), 1, 23},
        {Token(RPARENT), 1, 25},

        {Token(LBRACE), 1, 27},
        {Token(INTEGER, 12), 1, 29},
        {Token(RBRACE), 1, 32},
        {Token(ELSE), 1, 34},
        {Token(LBRACE), 1, 39},
        {Token(STRING, "kaczka"), 1, 41},
        {Token(RBRACE), 1, 50},
        {Token(SEMICOLON), 1, 51},

        {Token(WHILE), 2, 1},
        {Token(LPARENT), 2, 6},
        {Token(IDENTIFIER, "x"), 2, 7},
        {Token(LESS), 2, 9},
        {Token(INTEGER, 12), 2, 11},
        {Token(RPARENT), 2, 13},
        {Token(LBRACE), 2, 15},
        {Token(IDENTIFIER, "x"), 2, 17},
        {Token(ASSIGN), 2, 19},
        {Token(IDENTIFIER, "x"), 2, 21},
        {Token(PLUS), 2, 23},
        {Token(DOUBLE, 0.), 2, 25},
        {Token(SEMICOLON), 2, 27},
        {Token(RBRACE), 2, 29},

        {Token(LET), 3, 1},
        {Token(MUT), 3, 5},
        {Token(IDENTIFIER, "b"), 3, 9},
        {Token(ASSIGN), 3, 11},
        {Token(INTEGER, 123), 3, 13},
        {Token(RARROW), 3, 16},
        {Token(STR), 3,18},
        {Token(SEMICOLON), 3, 21},

        {Token(END_OF_FILE), 4, 1}
    };
    istringstream in(input);
    Lexer l(in);
    vector<Lexem> result = l.lexerize();

    BOOST_CHECK_EQUAL(expected.size(), result.size());

    compareLexemVectors(expected, result);
}

