#include "../inc/token.hpp"
// #include <cstdlib>

namespace token {

bool Token::operator==(const Token& t) const {
    return this->tokenType == t.tokenType && this->value == t.value;
}

std::ostream& operator<<(
    std::ostream& os,
    const std::optional<std::variant<int, double, std::string>>& opt
) {
    if (opt) {
        std::visit([&os](const auto& value) {
            os << value;
        }, *opt);
    } else {
        os << "[]";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& t) {
    os << "Token(\n\t\ttokenType:``" << TokenTypeToLiteral.at(t.tokenType) << "``, \n\t\tvalue:``" << t.value << "``,\n\t)";
    return os;
}

std::optional<TokenType> searchForKeyword(std::string& word) {
    auto token = Keywords.find(word);
    if (token != Keywords.end()) {
        return token->second;
    }
    return {};
}

} // namespace token
