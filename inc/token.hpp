#pragma once

#include <string>
#include <variant>
#include <iostream>
#include <optional>
#include <map>
#include "types.hpp"

namespace token {

enum TokenType : int {
    END_OF_FILE = 0,
    ONE_LINE_COMMENT,
    MULTILINE_COMMENT,
    ASSIGN,
    BANG,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,
    HASH,
    PIPE,
    DOLAR,
    SEMICOLON,
    COMMA,
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
    NOT_A_TOKEN,
};

std::optional<TokenType> searchForKeyword(std::string& word);

struct Token {
    Token() : tokenType(NOT_A_TOKEN), value(std::nullopt) {}
    Token(TokenType t) : tokenType(t), value(std::nullopt) {}
    Token(TokenType t, std::string val) : tokenType(t), value(val) {}
    Token(TokenType t, char val) : tokenType(t), value(std::string(1, val)) {}
    Token(TokenType t, const types::Integer& val) : tokenType(t), value(val) {}
    Token(TokenType t, int val) : tokenType(t), value(types::Integer(val)) {}
    Token(TokenType t, double val) : tokenType(t), value(types::Float(val)) {}
    Token(TokenType t, const types::Float& val) : tokenType(t), value(val) {}
    bool operator==(const Token& t) const;
    TokenType getTokenType() const;
    std::optional<std::variant<types::Integer, types::Float, std::string>> getValue() const;
private:
    TokenType tokenType;
    std::optional< std::variant<types::Integer, types::Float, std::string >> value;
};
std::ostream& operator<<(std::ostream& os, const std::optional<std::variant<types::Integer, types::Float, std::string>>& opt);
std::ostream& operator<<(std::ostream& os, const Token& t);


} // namespace token
