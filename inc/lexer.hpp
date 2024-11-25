#pragma once

#include <sstream>
#include <vector>

#include "token.hpp"

namespace lexer {

static const int IDENTIFIER_MAX_SIZE = 1200;
static const int NUMERIC_MAX_SIZE = 1200;
static const int STRING_MAX_SIZE = 1200;
static const int COMMENT_MAX_SIZE = 1200;

struct Lexem {
    token::Token token;
    size_t line;
    size_t column;
    bool operator==(const Lexem& l) const;
};
std::ostream& operator<<(std::ostream& os, const Lexem& l);


struct Lexer {
private:
    void skipWhiteSpaces();
    void readChar();
    token::Token handleOnelineCommentToken();
    token::Token handleMultilineCommentToken();
    token::Token handleNumber();
    token::Token handleFloat(std::string& buffer);
    token::Token handleNumericUndefinedRepresentation(std::string& buffer);
    token::Token handleIdentifier();
    token::Token handleString();

    std::optional<Lexem> tryEndOfFile();
    std::optional<Lexem> trySlashOrToken();
    std::optional<Lexem> tryParenthesis();
    std::optional<Lexem> tryTwoCharOperator();
    std::optional<Lexem> tryOneCharOperator();
    std::optional<Lexem> trySeparator();
    std::optional<Lexem> tryString();
    Lexem tryLiteralOrNotAToken();


public:
    Lexer(std::istream& istream) : istream_(istream), offset(1), line(1), column(1){
        ch = istream_.get();
        peek = istream_.peek();
    }

    Lexem nextLexem();
private:
    std::istream& istream_;
    char ch;
    char peek;
    size_t offset;
    size_t line;
    size_t column;
};

} // namespace lexer;
