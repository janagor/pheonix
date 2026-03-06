#include <istream>
#include <memory>

#include "Pheonix/jit.hpp"
#include "Pheonix/node.hpp"
#include "Pheonix/types.hpp"

void test2() {
  pheonix::init();

  pheonix::node::Literal a{pheonix::Primitive(pheonix::types::Integer{1})};
  pheonix::node::Literal b{pheonix::Primitive(pheonix::types::Integer{2})};
  auto expression = pheonix::node::AdditiveExpression{
      std::make_unique<pheonix::node::Literal>(a),
      std::make_unique<pheonix::node::Literal>(b), "+"};
  pheonix::JIT jit{};
  jit.visitAdditiveExpression(expression);
}
int main() {
  test2();
  // pheonix::node::Literal numNode{
  //     pheonix::Primitive(pheonix::types::Integer{0})};
  // pheonix::JIT jit{};
  // jit.visitLiteral(numNode);
}
