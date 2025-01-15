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
  pheonix::eval::Evaluator prev_evaluator;
  std::unique_ptr<pheonix::node::Node> output;
  bool in_scope = false;

  std::cout << PHEONIX << std::endl;

  while (true) {
    if (!in_scope)
      std::cout << ">>> ";
    std::getline(std::cin, line);

    buffer << line;

    if (_checkBrackets(line, brackets)) {
      if (_canCloseAllBrackets(brackets)) {
        pheonix::parser::Parser p(buffer);
        output = p.generateParsingTree();
        prev_evaluator.setContext(evaluator.getContext());
        output->accept(evaluator);
        buffer.str("");
        buffer.clear();
        in_scope = false;
        continue;
      } else {
        std::cout << "... ";
        in_scope = true;
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
