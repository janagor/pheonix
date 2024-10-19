#include "../inc/repl.hpp"

using namespace std;

namespace repl {

void kaczka(){};
void start(istream& is, ostream& os) {
    string statement;
    char input;
    while (1) {
        os << PROMPT;
        do {
            input = is.get();
            statement += input;
            if (input == '\n')
                os << " | ";
        }
        while (input != ';');

        input = is.get();
        statement += input;
        std::cout << statement << endl;
        statement.clear();
    }
}

} // namespace repl
