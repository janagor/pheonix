#include "repl.hpp"

#include <iostream>
#include <stack>
#include <string>

namespace pheonix::repl {

namespace {

bool _checkBrackets(const std::string &line, std::stack<char> &brackets) {
  for (char c : line) {
    switch (c) {
    case '(':
    case '[':
    case '{':
      brackets.push(c);
      break;
    case ')':
      if (brackets.empty() || brackets.top() != '(')
        return false;
      brackets.pop();
      break;
    case ']':
      if (brackets.empty() || brackets.top() != '[')
        return false;
      brackets.pop();
      break;
    case '}':
      if (brackets.empty() || brackets.top() != '{')
        return false;
      brackets.pop();
      break;
    }
  }
  return true;
}

bool _canCloseAllBrackets(std::stack<char> &brackets) {
  return brackets.empty();
}

} // namespace

void repl() {
  std::stringstream buffer;
  std::string line;
  std::stack<char> brackets;
  pheonix::eval::Evaluator evaluator;
  // pheonix::ast_view::ASTView evaluator;
  pheonix::eval::Evaluator prev_evaluator;
  std::unique_ptr<pheonix::node::Node> output;

  std::cout << PHEONIX << std::endl;

  while (true) {
    std::cout << ">>> ";
    std::getline(std::cin, line);

    buffer << line;

    if (_checkBrackets(line, brackets)) {
      if (_canCloseAllBrackets(brackets)) {
        pheonix::parser::Parser p(buffer);
        output = p.generateParsingTree();
        output->accept(evaluator);
        buffer.str("");
        buffer.clear();
        continue;
      } else {
        std::cout << "... ";
      }
    } else {
      std::cout
          << "Error: Nawiasy sa niepoprawnie zamkniete. Wprowadzaj dalej.\n";
      buffer.str("");
      buffer.clear();
      continue;
    }
  }
}

} // namespace pheonix::repl
