#pragma once

#include "token_type.hpp"

#include <map>
#include <optional>
#include <stdexcept>
#include <string>

namespace pheonix::types {

std::optional<token::TokenType> stringToTokenType(std::string &word);

std::string tokenTypeToLiteral(token::TokenType t);
std::optional<token::TokenType> Keywords(const std::string &input);

enum class TypeName {
  ERROR = 0,
  STR,
  INT,
  FLT,
  BOL,
};

static const std::map<token::TokenType, TypeName> TokenToType{
    {token::TokenType::INT, TypeName::INT},
    {token::TokenType::STR, TypeName::STR},
    {token::TokenType::FLT, TypeName::FLT},
    {token::TokenType::BOL, TypeName::BOL}};

static const std::map<TypeName, std::string> TypeNameToString{
    {TypeName::INT, "int"},
    {TypeName::STR, "str"},
    {TypeName::FLT, "flt"},
    {TypeName::BOL, "bol"}};

std::string opToString(const token::TokenType &tok);

} // namespace pheonix::types
