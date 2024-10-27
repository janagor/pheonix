#pragma once

#include <string>
#include <variant>
#include <iostream>
#include <optional>
#include <map>

namespace token {

enum TokenType : int {
    ERROR = 0,
    ERROR_NUMBER, // ex. 123a 123adfs
    ERROR_STRING,
    UNFINISHED_COMMENT,

    END_OF_FILE,

    PLUS,
    MINUS,
    STAR,
    SLASH,
    ASSIGN,

    EQUALS,
    LESS,
    LEQ,
    GREATER,
    GEQ,

    LPARENT,
    RPARENT,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,

    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    COLON,
    SEMICOLON,
    COMMA,
    DOT,
    RARROW,
    LARROW,

    ONE_LINE_COMMENT,
    MULTILINE_COMMENT,

    STRING,
    IDENTIFIER, // letter {letter | number | '_'};

    INTEGER,
    DOUBLE,
    LET,
    STRUCT,
    RETURN,
    WHILE,
    MUT,
    IF,
    ELSE,
    
    INT,
    STR,
    DBL,

    NOT_A_KEYWORD,
    /* */
};

static const std::map<std::string, TokenType> Keywords = {
    {"if", IF},
    {"else", ELSE},
    {"while", WHILE},

    {"let", LET},
    {"struct", STRUCT},
    {"return",RETURN },
    {"mut", MUT},

    {"int", INT},
    {"str", STR},
    {"dbl", DBL},
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
