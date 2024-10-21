#include "../inc/lexer.hpp"
#include <string>

namespace lexer {

Lexem Lexer::nextLexem() {
    switch (ch) {
    case EOF:
        return Lexem{Token(END_OF_FILE), line, column};
    case '+':
        return Lexem{Token(PLUS), line, column++};
    case '-':
        return Lexem{Token(MINUS), line, column++};
    case '=':
        return Lexem{Token(EQUALS), line, column++};
    case '*':
        return Lexem{Token(STAR), line, column++};
    case '/':
        return Lexem{Token(SLASH), line, column++};
    case '(':
        return Lexem{Token(LPARENT), line, column++};
    case ')':
        return Lexem{Token(RPARENT), line, column++};
    case '{':
        return Lexem{Token(LBRACES), line, column++};
    case '}':
        return Lexem{Token(RBRACES), line, column++};
    case '[':
        return Lexem{Token(LBRACKETS), line, column++};
    case ']':
        return Lexem{Token(RBRACKETS), line, column++};
    case '"':
        return Lexem{Token(DOUBLE_QUOTE), line, column++};
    case '\'':
        return Lexem{Token(SINGLE_QUOTE), line, column++};
    case ':':
        return Lexem{Token(COLON), line, column++};
    case ';':
        return Lexem{Token(SEMICOLON), line, column++};
    case ',':
        return Lexem{Token(COMMA), line, column++};
    case '.':
        return Lexem{Token(DOT), line, column++};
    default:
        return Lexem{Token(END_OF_FILE), line, column};
    }
}

void Lexer::readChar() {
    ch = istream_.get();
    ++offset;
}

std::vector<Lexem> Lexer::lexerize() {
    std::vector<Lexem> result;
    while (true) {
        Lexem l = nextLexem();
        result.emplace_back(l);
        if (l.token.tokenType == 0){
            return result;
        }
        readChar();
        

    }
}

} // namespace lexer
