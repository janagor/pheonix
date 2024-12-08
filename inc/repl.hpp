#pragma once
#include <iostream>

namespace pheonix::repl {

constexpr const char *PROMPT = ">> ";
void start(std::istream &is, std::ostream &os);
std::string dosth(const std::string &in);

} // namespace pheonix::repl
