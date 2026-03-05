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
  eval(emptyNode);

  Node literalNode = std::make_unique<Literal>(42);

  std::cout << "\nTest 2 (Literal 42):" << std::endl;
  eval(literalNode);

  literalNode = std::make_unique<Literal>(100);
  std::cout << "\nTest 3 (Zmiana na Literal 100):" << std::endl;
  eval(literalNode);
}

void test2() {
  using namespace pheonix::node2;
  using namespace pheonix::eval2;

  Evaluator eval;
  Node a = std::make_unique<Literal>(42);
  Node b = std::make_unique<Literal>(10);
  Node c = std::make_unique<InfixExpression>(a, b);

  std::cout << "\nTest 2 (Addition(42+10)):" << std::endl;
  eval(c);
}

int main() {
  test1();
  test2();
  return 0;
}
