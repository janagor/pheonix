#pragma once

#include <string>
#include <variant>
#include <iostream>
#include <optional>
#include <map>

namespace token {

enum TokenType : int {
    ERROR = 0,
    ERROR_NUMBER,
    ERROR_STRING,
    UNFINISHED_COMMENT,

    END_OF_FILE,

    ONE_LINE_COMMENT,
    MULTILINE_COMMENT,

    ASSIGN,
    BASH,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    DOUBLE_QUOTE,
    HASH,
    AT,
    AMPERSAND,
    DOLAR,
    SEMICOLON,
    COMMA,
    DOT,

    LPARENT,
    RPARENT,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,
    AND,
    OR,
    EQUALS,
    NEQ,
    LEQ,
    GEQ,
    GREATER,
    LESS,
    RARROW,
    LARROW,

    IDENTIFIER,
    STRING,
    INTEGER,
    DOUBLE,

    FN,
    LET,
    MUT,
    RETURN,
    IF,
    ELSE,
    WHILE,
    INT,
    STR,
    DBL,
    BOL,
    TRUE,
    FALSE,

    NOT_A_KEYWORD,
    /* */
};

static const std::map<std::string, TokenType> Keywords = {
    {"fn", FN},
    {"let", LET},
    {"mut", MUT},
    {"return",RETURN},

    {"if", IF},
    {"else", ELSE},
    {"while", WHILE},

    {"int", INT},
    {"str", STR},
    {"bol", BOL},
    {"dbl", DBL},

    {"true", TRUE},
    {"false", FALSE},
};

std::optional<TokenType> searchForKeyword(std::string& word);

struct Token {
    Token() : tokenType(ERROR), value(std::nullopt) {}
    Token(TokenType t) : tokenType(t), value(std::nullopt) {}
    Token(TokenType t, std::string val) : tokenType(t), value(val) {}
    Token(TokenType t, int val) : tokenType(t), value(val) {}
    Token(TokenType t, double val) : tokenType(t), value(val) {}
    bool operator==(const Token& t) const;

    TokenType tokenType;
    std::optional< std::variant<int, double, std::string > > value;
};
std::ostream& operator<<(std::ostream& os, const std::optional<std::variant<int, double, std::string>>& opt);
std::ostream& operator<<(std::ostream& os, const Token& t);


} // namespace token
