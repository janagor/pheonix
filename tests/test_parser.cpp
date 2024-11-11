#define BOOST_TEST_MODULE TestParser
#include <boost/test/unit_test.hpp>
#include "../inc/parser.hpp"
#include <cassert>
using namespace std;
using namespace token;
using namespace lexer;
using namespace parser;

// void compareLexemVectors(const vector<Lexem>& expected, const vector<Lexem>& received) {
//     BOOST_CHECK_EQUAL(expected.size(), received.size());
//
//     for (size_t i = 0; i < expected.size(); ++i) {
//         BOOST_CHECK_EQUAL(expected[i], received[i]);
//     }
// }

// BOOST_AUTO_TEST_CASE(testInteger) {
//     string input = R"(12)";
//
//     istringstream in(input);
//     Parser p(in);
//     unique_ptr<Node> result = p.generateParsingTree();
//     // cout << result->toString() << endl;
//     BOOST_CHECK_EQUAL(12, 12);
// }

BOOST_AUTO_TEST_CASE(testIntegerAddition) {
    string input = R"(12 + 12 * 12)";

    istringstream in(input);
    Parser p(in);
    unique_ptr<Node> result = p.generateParsingTree();
    cout << result->toString() << endl;
    TreeGenVisitor visitor;
    result->accept(visitor);
    string r = visitor.getResult();
    cout << r << std::endl;
}

// BOOST_AUTO_TEST_CASE(testIntegerSubtraction) {
//     string input = R"(12 - 12)";
//
//     istringstream in(input);
//     Parser p(in);
//
//     unique_ptr<Node> result = p.generateParsingTree();
//     AdditiveExpression* resultT = dynamic_cast<AdditiveExpression*>(result.get());
//
//     BOOST_CHECK_EQUAL(resultT->left, 12);
//     BOOST_CHECK_EQUAL(resultT->right, 12);
//     BOOST_CHECK_EQUAL(resultT->op, MINUS);
//     delete resultT;
// }

// BOOST_AUTO_TEST_CASE(testIntegerMultiplication) {
//     string input = R"(12 * 12)";
//
//     istringstream in(input);
//     Parser p(in);
//
//     Node* result = p.generateParsingTree();
//     MultiplicativeExpression* resultT = dynamic_cast<MultiplicativeExpression*>(result);
//
//     BOOST_CHECK_EQUAL(resultT->left, 12);
//     BOOST_CHECK_EQUAL(resultT->right, 12);
//     BOOST_CHECK_EQUAL(resultT->op, STAR);
//     delete resultT;
// }
//
// BOOST_AUTO_TEST_CASE(testIntegerDevision) {
//     string input = R"(12 / 12)";
//
//     istringstream in(input);
//     Parser p(in);
//
//     Node* result = p.generateParsingTree();
//     MultiplicativeExpression* resultT = dynamic_cast<MultiplicativeExpression*>(result);
//
//     BOOST_CHECK_EQUAL(resultT->left, 12);
//     BOOST_CHECK_EQUAL(resultT->right, 12);
//     BOOST_CHECK_EQUAL(resultT->op, SLASH);
//     delete resultT;
// }
//
// BOOST_AUTO_TEST_CASE(testIntegerAdditionAndMultiplication) {
//     string input = R"(1 / 2 + 3)";
//
//     istringstream in(input);
//     Parser p(in);
//
//     Node* result = p.generateParsingTree();
//     MultiplicativeExpression* resultT = dynamic_cast<MultiplicativeExpression*>(result);
//
//     BOOST_CHECK_EQUAL(resultT->left, 12);
//     BOOST_CHECK_EQUAL(resultT->right, 12);
//     BOOST_CHECK_EQUAL(resultT->op, SLASH);
//     delete resultT;
// }
