#include "../inc/lexer.hpp"
#include <string>
#include <cassert>
#include <cstdlib>

namespace lexer {

bool Token::operator==(const Token& t) const {
    return this->tokenType == t.tokenType && this->value == t.value;
}

std::ostream& operator<<(std::ostream& os, const std::optional<std::variant<int, double, std::string>>& opt) {
    if (opt) {
        std::visit([&os](const auto& value) {
            os << value;
        }, *opt);
    } else {
        os << "!empty!";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& t) {
    os << "Token(tokenType:" << t.tokenType << "value:" << t.value << ")";
    return os;
}

bool Lexem::operator==(const Lexem& l) const {
    return this->token==l.token && this->line==l.line && this->column==l.column;
}

std::ostream& operator<<(std::ostream& os, const Lexem& l) {
    os << "Lexem(token:" << l.token << ", line:" << l.line << ", column:" << l.column << ")";
    return os;
}
Lexem Lexer::nextLexem() {
    skipWhiteSpaces();
    int sline = line;
    int scolumn = column;
    std::string comment;
    Token token;

    switch (ch) {
    case EOF:
        return Lexem{Token(END_OF_FILE), sline, scolumn};
        break;
    case '+':
        readChar();
        return Lexem{Token(PLUS), sline, scolumn};
        break;
    case '-':
        readChar();
        if (ch == '>') {
            readChar();
            return Lexem{Token(RARROW), sline, scolumn};
        }
        return Lexem{Token(MINUS), sline, scolumn};
        break;

    case '<':
        readChar();
        if (ch == '=') {
            readChar();
            return Lexem{Token(LEQ), sline, scolumn};
        }
        if (ch == '-') {
            readChar();
            return Lexem{Token(LARROW), sline, scolumn};
        }
        return Lexem{Token(LESS), sline, scolumn};
        break;
    case '>':
        readChar();
        if (ch == '=') {
            readChar();
            return Lexem{Token(GEQ), sline, scolumn};
        }
        return Lexem{Token(GREATER), sline, scolumn};
        break;

    case '=':
        readChar();
        if (ch == '=') {
            readChar();
            return Lexem{Token(EQUALS), sline, scolumn};
        }
        return Lexem{Token(ASSIGN), sline, scolumn};
        break;

    case '*':
        readChar();
        return Lexem{Token(STAR), sline, scolumn};
        break;
    case '/':
        switch (peek){
        case '/':
            token = handleOnelineCommentToken();
            return Lexem{token, sline, scolumn};
            break;
        case '*':
            token = handleMultilineCommentToken();
            return Lexem{token, sline, scolumn};
            break;
        default:
            break;
        }
            readChar();
            return Lexem{Token(SLASH), sline, scolumn};
        break;

    case '(':
        readChar();
        return Lexem{Token(LPARENT), sline, scolumn};
        break;
    case ')':
        readChar();
        return Lexem{Token(RPARENT), sline, scolumn};
        break;
    case '{':
        readChar();
        return Lexem{Token(LBRACE), sline, scolumn};
        break;
    case '}':
        readChar();
        return Lexem{Token(RBRACE), sline, scolumn};
        break;
    case '[':
        readChar();
        return Lexem{Token(LBRACKET), sline, scolumn};
        break;
    case ']':
        readChar();
        return Lexem{Token(RBRACKET), sline, scolumn};
        break;

    case '"':
        token = handleString();
        return Lexem{token, sline, scolumn};
        break;
    case '\'':
        readChar();
        return Lexem{Token(SINGLE_QUOTE), sline, scolumn};
        break;
    case ':':
        readChar();
        return Lexem{Token(COLON), sline, scolumn};
        break;
    case ';':
        readChar();
        return Lexem{Token(SEMICOLON), sline, scolumn};
        break;
    case ',':
        readChar();
        return Lexem{Token(COMMA), sline, scolumn};
        break;
    case '.':
        readChar();
        return Lexem{Token(DOT), sline, scolumn};
        break;

    default:
        if (!isalnum(ch)) {
            readChar();
            return Lexem {Token(ERROR), sline, scolumn};
        }
        if (isdigit(ch)) {
            token = handleNumber();
            return Lexem{token, sline, scolumn};
        }
        token = handleIdentifier();
        return Lexem{token, sline, scolumn};
        break;
    }
}

void Lexer::readChar() {
    if (ch == '\n') {
        ++line;
        column = 0;
    }
    if (ch == EOF) {
    peek = ch;
        return;
    }
    ch = istream_.get();
    peek = istream_.peek();
    ++offset;
    ++column;

}

Token Lexer::handleOnelineCommentToken() {
    std::string buffer = "";
    buffer += ch;
    readChar();
    while (true) {
        switch (ch) {
        case '\n':
        case EOF:
            readChar();
            return Token(ONE_LINE_COMMENT, buffer);
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
    }
}
Token Lexer::handleMultilineCommentToken() {
    // state 1: /
    std::string buffer = "";
    assert(ch == '/');
    buffer += ch;
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
                readChar();
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

    std::optional<TokenType> result = searchForKeyword(buffer);
    if(result)
        return Token(*result);

    return Token(IDENTIFIER, buffer);
}

Token Lexer::handleNumber(){
    std::string buffer = "";
    bool doubleFlag = false;
    buffer += ch;
    readChar();
    while (isdigit(ch)) {
        buffer += ch;
        readChar();
    }
    if (ch == '.') {
        doubleFlag = true;
        buffer += ch;
        readChar();
        while (isdigit(ch)) {
            buffer += ch;
            readChar();
        }
    }
    if (!(isalpha(ch) || ch == '_')){
        if (doubleFlag)
            return Token(DOUBLE, stod(buffer));
        return Token(INTEGER, stoi(buffer));
    }

    // for better error information
    while (isalnum(ch) || ch == '_') {
        buffer += ch;
        readChar();
    }

    return Token(ERROR_NUMBER, buffer);
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
            case 'n':
                buffer += '\n';
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

std::optional<TokenType> searchForKeyword(std::string& word) {
    auto token = Keywords.find(word);
    if (token != Keywords.end()) {
        return token->second;
    }
    return {};
}

} // namespace lexer
