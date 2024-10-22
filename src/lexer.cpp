#include "../inc/lexer.hpp"
#include <string>
#include <cassert>

namespace lexer {

Lexem Lexer::nextLexem() {
    skipWhiteSpaces();
    char pk = 0;
    int saved_line;
    int saved_column;
    std::string comment;
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
        switch (pk){
        case '/':
            saved_line = line;
            saved_column = column;
            comment = getStringUntilNewLineEnd();
            return Lexem{Token(ONE_LINE_COMMENT, comment), saved_line, saved_column};
            break;
        case '*':
            saved_line = line;
            saved_column = column;
            comment = getMultilineComment();
            return Lexem{Token(MULTILINE_COMMENT, comment), saved_line, saved_column};
        default:
                break;
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
        std::cout << "something else" << std::endl;
        return Lexem{Token(DOT), line, column};
        break;
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
std::string Lexer::getMultilineComment() {
    // TODO: what if comment is not ended properly
    // state 1: /
    std::string buffer = "";
    buffer += ch;
    assert(ch == '/');
    // state 2: *
    readChar();
    assert(ch == '*');
    buffer += ch;
    assert(buffer == "/*");
    // state 3: *
    readChar();
    while (true) {
        switch (ch) {
        case '*':
            buffer += ch;
            readChar();
            if (ch == '/') {
                buffer += ch;
                return buffer;
            }
            buffer += ch;
            readChar();
            break;
        default:
            buffer += ch;
            readChar();
        }
    }
    // state 4: /
}

} // namespace lexer
