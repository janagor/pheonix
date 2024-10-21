#include "../inc/lexer.hpp"
#include <string>

namespace lexer {

Lexem Lexer::nextLexem() {
    switch (ch) {
    case EOF:
        return Lexem{Token(END_OF_FILE), line, column};
    default:
        return Lexem{Token(END_OF_FILE), line, column};
    }
}

std::vector<Lexem> Lexer::lexerize() {
    std::vector<Lexem> result;
    while (true) {
        Lexem l = nextLexem();
        result.emplace_back(l);
        if (l.token.tokenType == 0){
            return result;
        }

    }
}

} // namespace lexer
