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
    ERROR_BACK_SLASH_STRING,
    ERROR_UNFINISHED_STRING,
    ERROR_IDENTIFIER_TOO_LONG,
    ERROR_INTEGER_OUT_OF_BOUND,
    ERROR_FLOAT_OUT_OF_BOUND,
    ERROR_NUMBER_UNDEFINED_REPRESENTATION,
    UNFINISHED_COMMENT,

    END_OF_FILE,

    ONE_LINE_COMMENT,
    MULTILINE_COMMENT,

    ASSIGN,
    BANG,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,
    DOUBLE_QUOTE,
    HASH,
    AT,
    AMPERSAND,
    PIPE,
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
    FLOAT,

    FN,
    LET,
    MUT,
    RETURN,
    IF,
    ELSE,
    WHILE,
    INT,
    STR,
    FLT,
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
    {"flt", FLT},

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
