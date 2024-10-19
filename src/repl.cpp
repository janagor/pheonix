#include "../inc/repl.hpp"

using namespace std;
namespace repl {
void kaczka(){};
void start(istream& is, ostream& os) {
    string statement;
    string line;

    while (true) {
        os << PROMPT;
        while (std::getline(is, line)) {
            for (char ch: line) {
                statement += ch;
                if (ch == ';') {
                    os << statement << endl;
                    statement.clear();
                } 
            } 
            if (line.back() == ';') {
                os << PROMPT;
                continue;
            }
            os << " | ";
            line.clear();
            }
        }
    }
}
