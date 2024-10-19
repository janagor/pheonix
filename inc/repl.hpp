#pragma once
#include <iostream>
namespace repl {
constexpr const char* PROMPT = ">> ";
void start(std::istream& is, std::ostream& os);
void kaczka();
} // namespace repl;
