#define BOOST_TEST_MODULE TestParser
#include <boost/test/unit_test.hpp>
#include "../inc/parser.hpp"
#include <cassert>
using namespace std;
using namespace lexer;
using namespace parser;

// void compareLexemVectors(const vector<Lexem>& expected, const vector<Lexem>& received) {
//     BOOST_CHECK_EQUAL(expected.size(), received.size());
//
//     for (size_t i = 0; i < expected.size(); ++i) {
//         BOOST_CHECK_EQUAL(expected[i], received[i]);
//     }
// }

BOOST_AUTO_TEST_CASE(testInteger) {
    string input = R"(12)";

    istringstream in(input);
    Parser p(in);

    Node* result = p.generateParsingTree();
    IntegerLiteral* resultT = dynamic_cast<IntegerLiteral*>(result);

    BOOST_CHECK_EQUAL(resultT->value, 12);
    delete resultT;
}

BOOST_AUTO_TEST_CASE(testIntegerAddition) {
    string input = R"(12 + 12)";

    istringstream in(input);
    Parser p(in);

    Node* result = p.generateParsingTree();
    AdditiveExpression* resultT = dynamic_cast<AdditiveExpression*>(result);

    BOOST_CHECK_EQUAL(resultT->left, 12);
    BOOST_CHECK_EQUAL(resultT->right, 12);
    BOOST_CHECK_EQUAL(resultT->op, lexer::PLUS);
    delete resultT;
}

BOOST_AUTO_TEST_CASE(testIntegerSubtraction) {
    string input = R"(12 - 12)";

    istringstream in(input);
    Parser p(in);

    Node* result = p.generateParsingTree();
    AdditiveExpression* resultT = dynamic_cast<AdditiveExpression*>(result);

    BOOST_CHECK_EQUAL(resultT->left, 12);
    BOOST_CHECK_EQUAL(resultT->right, 12);
    BOOST_CHECK_EQUAL(resultT->op, lexer::MINUS);
    delete resultT;
}
