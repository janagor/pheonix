#define BOOST_TEST_MODULE TestLexer
#include <boost/test/unit_test.hpp>
#include "../inc/lexer.hpp"

using namespace std;
namespace lexer {

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

} // namespace lexer
