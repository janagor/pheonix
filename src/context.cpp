#include "context.hpp"

namespace pheonix::context {

void Context::push_scope() { context.push_back({}); }
void Context::pop_scope() {
  if (!context.empty()) {
    context.pop_back();
  }
}
Primitive Context::find(const std::string &ident) {
  for (auto &scope : context) {
    auto it = scope.find(ident);
    if (it != scope.end()) {
      return it->second;
    }
  }
  return std::monostate();
}
void Context::insert_unique(const std::string &ident, const Primitive &value) {
  if (!context.empty()) {
    auto &current_scope = context.back();
    if (current_scope.find(ident) == current_scope.end()) {
      current_scope[ident] = value;
    } else {
      std::cout << "Identifier already exists in the current scope.\n";
    }
  }
}
void Context::insert(const std::string &ident, const Primitive &value) {
  if (!context.empty()) {
    auto &current_scope = context.back();
    current_scope[ident] = value;
  }
}

} // namespace pheonix::context
