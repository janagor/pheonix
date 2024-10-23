#include "../inc/lexer.hpp"
#include <string>
#include <cassert>

namespace lexer {

std::optional<TokenType> searchForKeyword(std::string& word) {
    auto token = Keywords.find(word);
    if (token != Keywords.end()) {
        return token->second;
    }
    return {};

}

Lexem Lexer::nextLexem() {
    skipWhiteSpaces();
    char pk = 0;
    int saved_line;
    int saved_column;
    std::string comment;
    Token token;
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
            token = handleMultilineCommentToken();
            return Lexem{token, saved_line, saved_column};
            break;
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
        saved_line = line;
        saved_column = column;
        token = handleString();
        return Lexem{token, saved_line, saved_column};
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
    // LITERAL, // letter {letter | number | '_'};
        saved_line = line;
        saved_column = column;
        if (!isalnum(ch)) {
            return Lexem {Token(ERROR), saved_line, saved_column};
        }
        if (isdigit(ch)) {
            token = handleLiteral();
            return Lexem{token, saved_line, saved_column};
        }
        token = handleIdentifier();
        return Lexem{token, saved_line, saved_column};
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
        if (l.token.tokenType == END_OF_FILE){
            return result;
        }
        readChar();
    }
}
Token Lexer::handleMultilineCommentToken() {
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
                // state 4: /
                buffer += ch;
                return Token(MULTILINE_COMMENT, buffer);
            }
            if (ch == EOF) {
                return Token(UNFINISHED_COMMENT, buffer);
            }
            buffer += ch;
            readChar();
            break;
        case EOF:
            return Token(UNFINISHED_COMMENT, buffer);
            break;
        default:
            buffer += ch;
            readChar();
        }
    }
}

Token Lexer::handleIdentifier(){
    std::string buffer = "";
    buffer += ch;
    readChar();
    while (isalnum(ch) || ch=='_') {
        buffer += ch;
        readChar();
    }

    std::optional<TokenType> result =  searchForKeyword(buffer);
    if(result)
    return Token(*result);

    return Token(IDENTIFIER, buffer);
}

Token Lexer::handleLiteral(){
    std::string buffer = "";
    buffer += ch;
    readChar();
    while (isdigit(ch)) {
        buffer += ch;
        readChar();
    }
    if (!(isalnum(ch) || ch=='_')){
    return Token(LITERAL, buffer);
    }

    while (isalnum(ch) || ch=='_') {
        buffer += ch;
        readChar();
    }

    return Token(ERROR_LITERAL, buffer);
}

Token Lexer::handleString(){
    std::string buffer = "";
    readChar();
    while (ch != '"' && ch !=EOF) {
        if (ch == '\\') {
            readChar();
            switch (ch) {
            case '\\':
                buffer += R"(/)";
                break;
            case '"':
                buffer += R"(")";
                break;
            case '\n':
                buffer += R"(")";
                break;
            default:
                buffer += ch;
            }
            readChar();
            continue;

        }
        buffer += ch;
        readChar();
    }
    if (ch=='"'){
        readChar();
        return Token(STRING, buffer);
    }
    return Token(ERROR_STRING, buffer);
}

} // namespace lexer
