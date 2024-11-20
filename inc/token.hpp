#pragma once

#include <string>
#include <variant>
#include <iostream>
#include <optional>
#include <map>

namespace token {

enum TokenType : int {
    END_OF_FILE = 0,

    ERROR_IDENTIFIER_TOO_LONG,
    ERROR_INTEGER_OUT_OF_BOUND,
    ERROR_STRING_OUT_OF_BOUND,
    ERROR_FLOAT_OUT_OF_BOUND,
    ERROR_ONE_LINE_COMMENT_OUT_OF_BOUND,
    ERROR_MULTILINE_COMMENT_OUT_OF_BOUND,
    ERROR_BACK_SLASH_STRING,
    ERROR_UNFINISHED_STRING,
    ERROR_NUMBER_UNDEFINED_REPRESENTATION,
    ERROR_UNFINISHED_COMMENT,

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

const std::map<TokenType, std::string> TokenTypeToLiteral {
    { END_OF_FILE,"END_OF_FILE" },

    { ERROR_INTEGER_OUT_OF_BOUND,"ERROR_INTEGER_OUT_OF_BOUND" },
    { ERROR_STRING_OUT_OF_BOUND,"ERROR_STRING_OUT_OF_BOUND" },
    { ERROR_FLOAT_OUT_OF_BOUND,"ERROR_FLOAT_OUT_OF_BOUND" },
    {
        ERROR_ONE_LINE_COMMENT_OUT_OF_BOUND,
        "ERROR_ONE_LINE_COMMENT_OUT_OF_BOUND"
    },
    {
        ERROR_MULTILINE_COMMENT_OUT_OF_BOUND,
        "ERROR_MULTILINE_COMMENT_OUT_OF_BOUND"
    },
    { ERROR_BACK_SLASH_STRING,"ERROR_BACK_SLASH_STRING" },
    { ERROR_UNFINISHED_STRING,"ERROR_UNFINISHED_STRING" },
    { ERROR_IDENTIFIER_TOO_LONG,"ERROR_IDENTIFIER_TOO_LONG" },
    {
        ERROR_NUMBER_UNDEFINED_REPRESENTATION,
        "ERROR_NUMBER_UNDEFINED_REPRESENTATION"
    },
    { ERROR_UNFINISHED_COMMENT,"ERROR_UNFINISHED_COMMENT" },

    { ONE_LINE_COMMENT,"ONE_LINE_COMMENT" },
    { MULTILINE_COMMENT,"MULTILINE_COMMENT" },

    { ASSIGN,"ASSIGN" },
    { BANG,"BANG" },
    { PLUS,"PLUS" },
    { MINUS,"MINUS" },
    { STAR,"STAR" },
    { SLASH,"SLASH" },
    { PERCENT,"PERCENT" },
    { DOUBLE_QUOTE,"DOUBLE_QUOTE" },
    { HASH,"HASH" },
    { AT,"AT" },
    { AMPERSAND,"AMPERSAND" },
    { PIPE,"PIPE" },
    { DOLAR,"DOLAR" },
    { SEMICOLON,"SEMICOLON" },
    { COMMA,"COMMA" },
    { DOT,"DOT" },

    { LPARENT,"LPARENT" },
    { RPARENT,"RPARENT" },
    { LBRACE,"LBRACE" },
    { RBRACE,"RBRACE" },
    { LBRACKET,"LBRACKET" },
    { RBRACKET,"RBRACKET" },
    { AND,"AND" },
    { OR,"OR" },
    { EQUALS,"EQUALS" },
    { NEQ,"NEQ" },
    { LEQ,"LEQ" },
    { GEQ,"GEQ" },
    { GREATER,"GREATER" },
    { LESS,"LESS" },
    { RARROW,"RARROW" },
    { LARROW,"LARROW" },

    { IDENTIFIER,"IDENTIFIER" },
    { STRING,"STRING" },
    { INTEGER,"INTEGER" },
    { FLOAT,"FLOAT" },

    { FN,"FN" },
    { LET,"LET" },
    { MUT,"MUT" },
    { RETURN,"RETURN" },
    { IF,"IF" },
    { ELSE,"ELSE" },
    { WHILE,"WHILE" },
    { INT,"INT" },
    { STR,"STR" },
    { FLT,"FLT" },
    { BOL,"BOL" },
    { TRUE, "TRUE" },
    { FALSE,"FALSE" },

    { NOT_A_KEYWORD, "NOT_A_KEYWORD"},
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
    Token() : tokenType(NOT_A_KEYWORD), value(std::nullopt) {}
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
