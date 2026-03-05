#include "evaluator2.hpp"
#include "node2.hpp"

#include <iostream>
#include <memory>
#include <variant>

void test1() {
  using namespace pheonix::node2;
  using namespace pheonix::eval2;

  std::cout << "--- Test Evaluatora (std::variant) ---" << std::endl;

  Node emptyNode;
  Evaluator eval;
  std::cout << "Test 1 (Pusty): ";
  eval.evaluate(emptyNode);

  Node literalNode = std::make_unique<Literal>(42);

  std::cout << "\nTest 2 (Literal 42):" << std::endl;
  eval.evaluate(literalNode);

  literalNode = std::make_unique<Literal>(100);
  std::cout << "\nTest 3 (Zmiana na Literal 100):" << std::endl;
  eval.evaluate(literalNode);
}

int main() {
  test1();
  return 0;
}
