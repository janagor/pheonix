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

std::string opToString(const token::TokenType &tok);

} // namespace pheonix::types
