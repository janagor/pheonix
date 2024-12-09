#pragma once
#include <iostream>
#include <map>
#include <optional>

namespace pheonix::types {

// tokens
enum class TokenType {
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

std::optional<TokenType> searchForKeyword(std::string &word);

const std::map<TokenType, std::string> TokenTypeToLiteral;
// nodes typenames

enum TypeName : int {
  ERROR = 0,
  TYPE_STR,
  TYPE_INT,
  TYPE_FLT,
  TYPE_BOL,
};

static const std::map<TokenType, TypeName> TokenToType{
    {types::TokenType::INT, TypeName::TYPE_INT},
    {types::TokenType::STR, TypeName::TYPE_STR},
    {types::TokenType::FLT, TypeName::TYPE_FLT},
    {types::TokenType::BOL, TypeName::TYPE_BOL}};

static const std::map<TypeName, std::string> TypeNameToString{
    {TypeName::TYPE_INT, "int"},
    {TypeName::TYPE_STR, "str"},
    {TypeName::TYPE_FLT, "flt"},
    {TypeName::TYPE_BOL, "bol"}};

std::string opToString(const types::TokenType &tok);

struct Integer {
  Integer() : value(0) {}
  Integer(int v) : value(v) {}
  Integer(const Integer &other) : value(other.value) {}

  Integer &operator=(int val);
  Integer &operator=(const Integer &val);

  Integer &operator*=(int val);
  Integer &operator/=(int val);
  Integer &operator+=(int val);
  Integer &operator-=(int val);
  Integer &operator%=(int val);

  Integer &operator*=(const Integer &val);
  Integer &operator/=(const Integer &val);
  Integer &operator+=(const Integer &val);
  Integer &operator-=(const Integer &val);

  Integer operator*(int val) const;
  Integer operator/(int val) const;
  Integer operator+(int val) const;
  Integer operator-(int val) const;
  Integer operator%(int val) const;

  Integer operator*(const Integer &val) const;
  Integer operator/(const Integer &val) const;
  Integer operator+(const Integer &val) const;
  Integer operator-(const Integer &val) const;
  Integer operator%(const Integer &val) const;

  bool operator==(const Integer &other) const;
  bool operator!=(const Integer &other) const;
  bool operator<(const Integer &other) const;
  bool operator<=(const Integer &other) const;
  bool operator>(const Integer &other) const;
  bool operator>=(const Integer &other) const;

  int getValue() const;
  void setValue(long val);

  friend std::ostream &operator<<(std::ostream &os, const Integer &obj);

private:
  int value;
};

struct Float {
  Float() : value(0.0) {}
  Float(double v) : value(v) {}
  Float(const Float &other) : value(other.value) {}

  Float &operator=(double val);
  Float &operator=(const Float &val);

  Float &operator*=(double val);
  Float &operator/=(double val);
  Float &operator+=(double val);
  Float &operator-=(double val);

  Float &operator*=(const Float &val);
  Float &operator/=(const Float &val);
  Float &operator+=(const Float &val);
  Float &operator-=(const Float &val);

  Float operator*(double val) const;
  Float operator/(double val) const;
  Float operator+(double val) const;
  Float operator-(double val) const;

  Float operator*(const Float &val) const;
  Float operator/(const Float &val) const;
  Float operator+(const Float &val) const;
  Float operator-(const Float &val) const;

  bool operator==(const Float &other) const;
  bool operator!=(const Float &other) const;
  bool operator<(const Float &other) const;
  bool operator<=(const Float &other) const;
  bool operator>(const Float &other) const;
  bool operator>=(const Float &other) const;

  friend std::ostream &operator<<(std::ostream &os, const Float &obj);

  double getValue() const;
  void setValue(double val);

private:
  double value;
};

} // namespace pheonix::types
