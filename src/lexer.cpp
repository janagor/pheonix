#include "../inc/lexer.hpp"
#include <string>
#include <cassert>
#include <cstdlib>

namespace lexer {

bool Lexem::operator==(const Lexem& l) const {
    return this->token==l.token &&
        this->line==l.line &&
        this->column==l.column;
}

std::ostream& operator<<(std::ostream& os, const Lexem& l) {
    os << "Lexem(  token: `" << l.token 
        << "`,\n\tline: `" << l.line
        << "`,\n\tcolumn: `" << l.column << "`,\n),";
    return os;
}
Lexem Lexer::nextLexem() {
    skipWhiteSpaces();
    size_t sline = line;
    size_t scolumn = column;
    std::string comment;
    token::Token token;

    switch (ch) {
    case EOF:
        return Lexem{token::Token(token::END_OF_FILE), sline, scolumn};
        break;
    case '-':
        readChar();
        if (ch == '>') {
            readChar();
            return Lexem{token::Token(token::RARROW), sline, scolumn};
        }
        return Lexem{token::Token(token::MINUS), sline, scolumn};
        break;
    case '<':
        readChar();
        if (ch == '=') {
            readChar();
            return Lexem{token::Token(token::LEQ), sline, scolumn};
        }
        if (ch == '-') {
            readChar();
            return Lexem{token::Token(token::LARROW), sline, scolumn};
        }
        return Lexem{token::Token(token::LESS), sline, scolumn};
        break;

    case '>':
        readChar();
        if (ch == '=') {
            readChar();
            return Lexem{token::Token(token::GEQ), sline, scolumn};
        }
        return Lexem{token::Token(token::GREATER), sline, scolumn};
        break;

    case '=':
        readChar();
        if (ch == '=') {
            readChar();
            return Lexem{token::Token(token::EQUALS), sline, scolumn};
        }
        return Lexem{token::Token(token::ASSIGN), sline, scolumn};
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
            return Lexem{token::Token(token::SLASH), sline, scolumn};
        break;

    case '(':
        readChar();
        return Lexem{token::Token(token::LPARENT), sline, scolumn};
        break;
    case ')':
        readChar();
        return Lexem{token::Token(token::RPARENT), sline, scolumn};
        break;
    case '{':
        readChar();
        return Lexem{token::Token(token::LBRACE), sline, scolumn};
        break;
    case '}':
        readChar();
        return Lexem{token::Token(token::RBRACE), sline, scolumn};
        break;
    case '[':
        readChar();
        return Lexem{token::Token(token::LBRACKET), sline, scolumn};
        break;
    case ']':
        readChar();
        return Lexem{token::Token(token::RBRACKET), sline, scolumn};
        break;
    case '+':
        readChar();
        return Lexem{token::Token(token::PLUS), sline, scolumn};
        break;
    case '!':
        readChar();
        if (ch == '=') {
            readChar();
            return Lexem{token::Token(token::NEQ), sline, scolumn};
        }
        return Lexem{token::Token(token::BANG), sline, scolumn};
        break;

    case '*':
        readChar();
        return Lexem{token::Token(token::STAR), sline, scolumn};
        break;
    case '%':
        readChar();
        return Lexem{token::Token(token::PERCENT), sline, scolumn};
        break;
    case '"':
        token = handleString();
        return Lexem{token, sline, scolumn};
        break;
    case '@':
        readChar();
        return Lexem{token::Token(token::AT), sline, scolumn};
        break;
    case '$':
        readChar();
        return Lexem{token::Token(token::DOLAR), sline, scolumn};
        break;
    case '#':
        readChar();
        return Lexem{token::Token(token::HASH), sline, scolumn};
        break;
    case '&':
        readChar();
        if (ch == '&') {
            readChar();
            return Lexem{token::Token(token::AND), sline, scolumn};
        }
        return Lexem{token::Token(token::AMPERSAND), sline, scolumn};
        break;
    case '|':
        readChar();
        if (ch == '|') {
            readChar();
            return Lexem{token::Token(token::OR), sline, scolumn};
        }
        return Lexem{token::Token(token::PIPE), sline, scolumn};
        break;

    case ';':
        readChar();
        return Lexem{token::Token(token::SEMICOLON), sline, scolumn};
        break;
    case ',':
        readChar();
        return Lexem{token::Token(token::COMMA), sline, scolumn};
        break;
    case '.':
        readChar();
        return Lexem{token::Token(token::DOT), sline, scolumn};
        break;

    default:
        if (!isalnum(ch)) {
            readChar();
            return Lexem {token::Token(token::ERROR), sline, scolumn};
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
        // Linux
        ++line;
        column = 0;
    }
    if (ch == '\r') {
        // MacOS or Windows
        if (peek == '\n') {
            // Windows
            ++offset;
            ch = istream_.get();
            peek = istream_.peek();
        }
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

token::Token Lexer::handleOnelineCommentToken() {
    std::string buffer = "";
    buffer += ch;
    readChar();
    while (true) {
        switch (ch) {
        case '\n':
        case '\r':
        case EOF:
            readChar();
            return token::Token(token::ONE_LINE_COMMENT, buffer);
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
        case '\r':
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
        if (l.token.tokenType == token::END_OF_FILE){
            return result;
        }
    }
}
token::Token Lexer::handleMultilineCommentToken() {
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
                return token::Token(token::MULTILINE_COMMENT, buffer);
            }
            if (ch == EOF) {
                return token::Token(token::UNFINISHED_COMMENT, buffer);
            }
            buffer += ch;
            readChar();
            break;
        case EOF:
            return token::Token(token::UNFINISHED_COMMENT, buffer);
            break;
        default:
            buffer += ch;
            readChar();
        }
    }
}

token::Token Lexer::handleIdentifier(){
    std::string buffer = "";
    buffer += ch;
    readChar();
    while ((isalnum(ch) || ch=='_') && buffer.size() < IDENTIFIER_MAX_SIZE) {
        buffer += ch;
        readChar();
    }
    // NOTE: if identifier size may be greater then IDENTIFIER_MAX_SIZE
    // skipping all the next chars
    bool overflow_encoutered = false;
    while (isalnum(ch) || ch=='_') {
        overflow_encoutered = true;
        readChar();
    }
    if(overflow_encoutered)
        return token::Token(token::ERROR_IDENTIFIER_TOO_LONG, buffer);

    std::optional<token::TokenType> result = token::searchForKeyword(buffer);
    if(result)
        return token::Token(*result);

    return token::Token(token::IDENTIFIER, buffer);
}

token::Token Lexer::handleNumber(){
    std::string buffer = "";
    buffer += ch;
    readChar();

    while (isdigit(ch) && buffer.size() < NUMERIC_MAX_SIZE) {
        buffer += ch;
        readChar();
    }
    if (ch == '.')
        return handleFloat(buffer);

    if (isalpha(ch) || ch == '_')
        return handleNumericUndefinedRepresentation(buffer);

    if (buffer.size() == NUMERIC_MAX_SIZE) {
        while (isdigit(ch)) {
            readChar();
        }
        if (ch == '.')
            return handleFloat(buffer);

        if (isalnum(ch) || ch == '_')
            return handleNumericUndefinedRepresentation(buffer);
        return token::Token(token::ERROR_INTEGER_OUT_OF_BOUND, buffer);
    }

    try {
        // NOTE: it is expected that std::numeric_limit::<int>lowest() is not
        // achievable
        return token::Token(token::INTEGER, stoi(buffer));
    } catch (const std::out_of_range& e) {
        return token::Token(token::ERROR_INTEGER_OUT_OF_BOUND, buffer);
    }
}

token::Token Lexer::handleFloat(std::string& buffer){
    if (buffer.size() < NUMERIC_MAX_SIZE) {
        buffer += ch;
        readChar();
    } else {
        readChar();
    }

    while (isdigit(ch) && buffer.size() < NUMERIC_MAX_SIZE) {
        buffer += ch;
        readChar();
    }
    if (isalpha(ch) || ch == '_')
        return handleNumericUndefinedRepresentation(buffer);

    if (buffer.size() == NUMERIC_MAX_SIZE) {
        while (isdigit(ch)) {
            readChar();
        }
        if (isalpha(ch) || ch == '_')
            return handleNumericUndefinedRepresentation(buffer);
        return token::Token(token::ERROR_FLOAT_OUT_OF_BOUND, buffer);
    }

    try {
        return token::Token(token::FLOAT, stod(buffer));
    } catch (const std::out_of_range& e) {
        return token::Token(token::ERROR_FLOAT_OUT_OF_BOUND, buffer);
    }
}

token::Token Lexer::handleNumericUndefinedRepresentation(std::string& buffer){
    while ((isalnum(ch) || ch == '_') && buffer.size() < NUMERIC_MAX_SIZE) {
        buffer += ch;
        readChar();
    }
    if (buffer.size() == NUMERIC_MAX_SIZE) {
        while (isalnum(ch) || ch == '_')
            readChar();
    }
    return token::Token(token::ERROR_NUMBER_UNDEFINED_REPRESENTATION, buffer);
}

token::Token Lexer::handleString(){
    std::string buffer = "";
    readChar();
    while (ch != '"' && ch !=EOF) {
        if (ch == '\\') {
            readChar();
            switch (ch) {
            case '\\':
                buffer += R"(\)";
                break;
            case '"':
                buffer += R"(")";
                break;
            case 'n':
                buffer += '\n';
                break;
            case 't':
                buffer += '\t';
                break;
            default:
                return token::Token(
                    token::ERROR_BACK_SLASH_STRING, "\"" + buffer + "\\"
                );
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
        return token::Token(token::STRING, buffer);
    }
    return token::Token(token::ERROR_UNFINISHED_STRING, "\"" + buffer);
}

} // namespace lexer
