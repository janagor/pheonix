#include "../inc/lexer.hpp"
#include <string>

namespace lexer {

Lexem Lexer::nextLexem() {
    skipWhiteSpaces();
    char pk = 0;
    switch (ch) {
    case EOF:
        return Lexem{Token(END_OF_FILE), line, column};
        break;
    case '+':
        return Lexem{Token(PLUS), line, column};
        break;
    case '-':
        return Lexem{Token(MINUS), line, column};
        break;
    case '=':
        return Lexem{Token(EQUALS), line, column};
        break;
    case '*':
        return Lexem{Token(STAR), line, column};
        break;
    case '/':
        pk = istream_.peek();
        if (pk == '/') {
            int saved_line = line;
            int saved_column = column;
            std::string comment = getStringUntilNewLineEnd();
            return Lexem{Token(ONE_LINE_COMMENT, comment), saved_line, saved_column};
        }
        return Lexem{Token(SLASH), line, column};
        break;
    case '(':
        return Lexem{Token(LPARENT), line, column};
        break;
    case ')':
        return Lexem{Token(RPARENT), line, column};
        break;
    case '{':
        return Lexem{Token(LBRACES), line, column};
        break;
    case '}':
        return Lexem{Token(RBRACES), line, column};
        break;
    case '[':
        return Lexem{Token(LBRACKETS), line, column};
        break;
    case ']':
        return Lexem{Token(RBRACKETS), line, column};
        break;
    case '"':
        return Lexem{Token(DOUBLE_QUOTE), line, column};
        break;
    case '\'':
        return Lexem{Token(SINGLE_QUOTE), line, column};
        break;
    case ':':
        return Lexem{Token(COLON), line, column};
        break;
    case ';':
        return Lexem{Token(SEMICOLON), line, column};
        break;
    case ',':
        return Lexem{Token(COMMA), line, column};
        break;
    case '.':
        return Lexem{Token(DOT), line, column};
        break;
    default:
        return Lexem{Token(END_OF_FILE), line, column};
    }
}

void Lexer::readChar() {
    if (ch == '\n') {
        ++line;
        column = 0;
    }
    if (ch == EOF) {
        return;
    }
    ch = istream_.get();
    ++offset;
    ++column;

}

std::string Lexer::getStringUntilNewLineEnd() {
    std::string buffer = "";
    while (true) {
        switch (ch) {
        case '\n':
        case EOF:
            return buffer;
            break;
        default:
            buffer += ch;
            readChar();
        }
    }

}

void Lexer::skipWhiteSpaces() {
    while (true) {
        switch (ch) {
        case ' ':
        case '\t':
        case '\n':
            readChar();
            break;
        default:
            return;
        }
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
        readChar();
    }
}

} // namespace lexer
