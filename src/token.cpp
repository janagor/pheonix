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
        os << "!empty!";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& t) {
    os << "Token(tokenType:" << t.tokenType << "value:" << t.value << ")";
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
