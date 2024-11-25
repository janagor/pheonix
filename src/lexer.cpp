#include "lexer.hpp"
#include <string>
#include <stdexcept>
#include <cassert>
#include <cstdlib>
#include <cmath>

namespace lexer {

size_t LexerException::getLine() const { return line; }
size_t LexerException::getColumn() const { return column; }

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
    std::optional<Lexem> result;
    skipWhiteSpaces();

    result = tryEndOfFile();
    if (result.has_value()) return result.value();
    result = trySlashOrToken();
    if (result.has_value()) return result.value();
    result = tryParenthesis();
    if (result.has_value()) return result.value();
    result = tryTwoCharOperator();
    if (result.has_value()) return result.value();
    result = tryOneCharOperator();
    if (result.has_value()) return result.value();
    result = trySeparator();
    if (result.has_value()) return result.value();
    result = tryString();
    if (result.has_value()) return result.value();
    return tryLiteralOrNotAToken();
}

std::optional<Lexem> Lexer::tryEndOfFile() {
    switch (ch) {
    case EOF:
        return Lexem{token::Token(token::END_OF_FILE), line, column};
    default:
        return std::nullopt;
    }
}

std::optional<Lexem> Lexer::trySlashOrToken() {
    size_t sline = line;
    size_t scolumn = column;
    token::Token token;
    switch (ch) {
    case '/':
        switch (peek){
            case '/':
                token = handleOnelineCommentToken(sline, scolumn);
                return Lexem{token, sline, scolumn};
            case '*':
                token = handleMultilineCommentToken(sline, scolumn);
                return Lexem{token, sline, scolumn};
            default:
                break;
        }
        readChar();
        return Lexem{token::Token(token::SLASH), sline, scolumn};
    default:
        return std::nullopt;
    }
}

std::optional<Lexem> Lexer::tryParenthesis() {
    size_t sline = line;
    size_t scolumn = column;
    switch (ch) {
    case '(':
        readChar();
        return Lexem{token::Token(token::LPARENT), sline, scolumn};
    case ')':
        readChar();
        return Lexem{token::Token(token::RPARENT), sline, scolumn};
    case '{':
        readChar();
        return Lexem{token::Token(token::LBRACE), sline, scolumn};
    case '}':
        readChar();
        return Lexem{token::Token(token::RBRACE), sline, scolumn};
    case '[':
        readChar();
        return Lexem{token::Token(token::LBRACKET), sline, scolumn};
    case']':
        readChar();
        return Lexem{token::Token(token::RBRACKET), sline, scolumn};
    default:
        return std::nullopt;
    }
}

std::optional<Lexem> Lexer::tryTwoCharOperator() {
    size_t sline = line;
    size_t scolumn = column;
    switch (ch) {
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
    case '>':
        readChar();
        if (ch == '=') {
            readChar();
            return Lexem{token::Token(token::GEQ), sline, scolumn};
        }
        return Lexem{token::Token(token::GREATER), sline, scolumn};
    case '=':
        readChar();
        if (ch == '=') {
            readChar();
            return Lexem{token::Token(token::EQUALS), sline, scolumn};
        }
        return Lexem{token::Token(token::ASSIGN), sline, scolumn};
    case '!':
        readChar();
        if (ch == '=') {
            readChar();
            return Lexem{token::Token(token::NEQ), sline, scolumn};
        }
        return Lexem{token::Token(token::BANG), sline, scolumn};
    case '&':
        readChar();
        if (ch == '&') {
            readChar();
            return Lexem{token::Token(token::AND), sline, scolumn};
        }
        return Lexem{token::Token(token::NOT_A_TOKEN, "&"), sline, scolumn};
    case '|':
        readChar();
        if (ch == '|') {
            readChar();
            return Lexem{token::Token(token::OR), sline, scolumn};
        }
        return Lexem{token::Token(token::NOT_A_TOKEN, "|"), sline, scolumn};
    default:
        return std::nullopt;
    }
}

std::optional<Lexem> Lexer::tryOneCharOperator() {
    size_t sline = line;
    size_t scolumn = column;
    switch (ch) {
    case '-':
        readChar();
        return Lexem{token::Token(token::MINUS), sline, scolumn};
    case '+':
        readChar();
        return Lexem{token::Token(token::PLUS), sline, scolumn};
    case '*':
        readChar();
        return Lexem{token::Token(token::STAR), sline, scolumn};
    case '%':
        readChar();
        return Lexem{token::Token(token::PERCENT), sline, scolumn};
    case '$':
        readChar();
        return Lexem{token::Token(token::DOLAR), sline, scolumn};
    case '#':
        readChar();
        return Lexem{token::Token(token::HASH), sline, scolumn};
    default:
        return std::nullopt;
    }
}

std::optional<Lexem> Lexer::trySeparator() {
    size_t sline = line;
    size_t scolumn = column;
    switch (ch) {
    case ';':
        readChar();
        return Lexem{token::Token(token::SEMICOLON), sline, scolumn};
    case ',':
        readChar();
        return Lexem{token::Token(token::COMMA), sline, scolumn};
    default:
        return std::nullopt;
    }
}
std::optional<Lexem> Lexer::tryString() {
    size_t sline = line;
    size_t scolumn = column;
    token::Token token;
    if (ch == '"') {
        token = handleString(sline, scolumn);
        return Lexem{token, sline, scolumn};
    }
    return std::nullopt;
}

Lexem Lexer::tryLiteralOrNotAToken() {
    size_t sline = line;
    size_t scolumn = column;
    token::Token token;
    if (!isalnum(ch)) {
        throw LexerException("Not a token.", sline, scolumn);
    }
    if (isdigit(ch)) {
        token = handleNumber(sline, scolumn);
        return Lexem{token, sline, scolumn};
    }
    token = handleIdentifier(sline, scolumn);
    return Lexem{token, sline, scolumn};
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

token::Token Lexer::handleOnelineCommentToken(size_t row, size_t column) {
    std::string buffer = "";
    buffer += ch;
    readChar();
    while (buffer.size() <= COMMENT_MAX_SIZE) {
        switch (ch) {
        case '\r':
            if (peek == '\n') readChar();
            [[fallthrough]];
        case '\n':
        case EOF:
            readChar();
            return token::Token(token::ONE_LINE_COMMENT, buffer);
            break;
        default:
            buffer += ch;
            readChar();
        }
    }
    throw LexerException("Oneline comment too long.", row, column);
}

token::Token Lexer::handleMultilineCommentToken(size_t row, size_t column) {
    std::string buffer = "";
    assert(ch == '/');
    buffer += ch;
    readChar();
    assert(ch == '*');
    buffer += ch;
    assert(buffer == "/*");
    readChar();
    while (buffer.size() <= COMMENT_MAX_SIZE) {
        switch (ch) {
        case '*':
            buffer += ch;
            readChar();
            if (ch == '/') {
                buffer += ch;
                readChar();
                return token::Token(token::MULTILINE_COMMENT, buffer);
            }
            if (ch == EOF) {
                throw LexerException(
                    "Unfinished multiline comment.", row, column
                );
            }
            buffer += ch;
            readChar();
            break;
        case EOF:
            throw LexerException(
                "Unfinished multiline comment.", row, column
            );
        default:
            buffer += ch;
            readChar();
        }
    }
    throw LexerException("Multiline comment too long.", row, column);
}

token::Token Lexer::handleIdentifier(size_t row, size_t column){
    std::string buffer = "";
    buffer += ch;
    readChar();
    while ((isalnum(ch) || ch=='_') && buffer.size() <= IDENTIFIER_MAX_SIZE) {
        buffer += ch;
        readChar();
    }
    if(buffer.size() > IDENTIFIER_MAX_SIZE)
        throw LexerException("Identifier too long.", row, column);

    std::optional<token::TokenType> result = token::searchForKeyword(buffer);
    if(result)
        return token::Token(*result);

    return token::Token(token::IDENTIFIER, buffer);
}

token::Token Lexer::handleNumber(size_t row, size_t column){
    std::string buffer = "";
    buffer += ch;
    long integerPart = static_cast<long>(ch - '0');
    readChar();
    while (isdigit(ch) && integerPart <= std::numeric_limits<int>::max()) {
        integerPart = 10*integerPart + static_cast<long>(ch - '0');
        buffer += ch;
        readChar();
    }
    if (ch == '.') {
        readChar();
        return handleFloat(row, column, integerPart);
    }
    if (integerPart > std::numeric_limits<int>::max())
        throw LexerException("Integer literal out of range.", row, column);
    if (isalpha(ch))
        throw LexerException("Undefined value.", row, column);

    return token::Token(token::INTEGER, static_cast<int>(integerPart));
}

token::Token Lexer::handleFloat(size_t row, size_t column, long intPart){
    std::string buffer = "";
    int fractionalPart = 0;
    int length = 0;

    while (isdigit(ch) && fractionalPart <= std::numeric_limits<int>::max()) {
        fractionalPart = 10*fractionalPart + static_cast<long>(ch - '0');
        ++length;
        buffer += ch;
        readChar();
    }
    if (isalpha(ch))
        throw LexerException("Undefined value.", row, column);

    double result = static_cast<double>(intPart) +
    static_cast<double>(fractionalPart) * std::pow(10., static_cast<double>(-length));
    return token::Token(token::FLOAT, result);
}

token::Token Lexer::handleString(size_t line, size_t column){
    std::string buffer = "";
    assert(ch == '"');
    readChar();
    while (ch != '"' && ch !=EOF && buffer.size() <= STRING_MAX_SIZE) {
        if (ch == '\\') {
            readChar();
            switch (ch) {
            case '\\':
                buffer += R"(\)";
                break;
            case '"':
                buffer += R"(")";
                break;
            case 'r':
                buffer += '\r';
                break;
            case 'n':
                buffer += '\n';
                break;
            case 't':
                buffer += '\t';
                break;
            default:
                throw LexerException(
                    "Wrong usage of \\ character in string literal",
                    line, column
                );
            }
            readChar();
            continue;

        }
        buffer += ch;
        readChar();
    }

    if (ch=='"' && buffer.size() <= STRING_MAX_SIZE){
        readChar();
        return token::Token(token::STRING, buffer);
    }

    if (ch==EOF && buffer.size() <= STRING_MAX_SIZE){
        throw LexerException(
            "Unfinished string literal.",
            line, column
        );
    }

    throw LexerException(
        "String literal to long.", line, column
    );
}

} // namespace lexer
