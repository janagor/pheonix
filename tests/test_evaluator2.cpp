#include "ast_context.hpp"
#include "evaluator2.hpp"
#include "node2.hpp"

#include <iostream>
#include <variant>

void test1() {
  using namespace pheonix;

  ASTContext context{};
  Evaluator eval{};

  std::cout << "--- Test Evaluatora (PMR Arena) ---" << std::endl;

  Node emptyNode;
  std::cout << "Test 1 (Pusty): ";
  eval.eval(emptyNode);

  std::cout << "\nTest 2 (Literal 42):" << std::endl;
  Node *literalNode1 = context.make<Literal>(42);
  eval.eval(*literalNode1);

  std::cout << "\nTest 3 (Zmiana na Literal 100):" << std::endl;
  Node *literalNode2 = context.make<Literal>(100);
  eval.eval(*literalNode2);
}

void test2() {
  using namespace pheonix;

  ASTContext context{};
  Evaluator eval;

  Node *a = context.make<Literal>(42);
  Node *b = context.make<Literal>(10);

  Node *c = context.make<InfixExpression>(a, b);

  std::cout << "\nTest 4 (Addition(42+10)):" << std::endl;
  eval.eval(*c);
  std::cout << eval.result() << std::endl;
}

int main() {
  test1();
  test2();
  return 0;
}
