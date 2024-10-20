#include "../inc/repl.hpp"

using namespace std;
namespace repl {

string dosth(const string& in) {
    return "doing sth with {" + in + "}";
}

// NOTE: currently parsing statement which ends with ';' is ok, but casted
// functions will fail. Better probably searching for scopes.
void start(istream& is, ostream& os) {
    string statement;
    string line;
    while (true) {
        os << PROMPT;
        while (getline(is, line)) {
            for (char ch: line) {
                statement += ch;
                if (ch == ';') {
                    os << dosth(statement) << endl;
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

} // namespace repl
