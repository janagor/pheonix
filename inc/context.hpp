#pragma once

#include "node.hpp"
#include <map>
#include <string>
#include <vector>

namespace pheonix::context {

struct Context {
public:
  Context() : context() { push_scope(); }
  void pop_scope();
  void push_scope();
  Primitive find(const std::string &ident);
  void insert_unique(const std::string &ident, const Primitive &value);
  void insert(const std::string &ident, const Primitive &value);
  Primitive &operator[](const std::string &ident) {
    for (auto &scope : context) {
      auto it = scope.find(ident);
      if (it != scope.end()) {
        return it->second;
      }
    }

    if (!context.empty()) {
      auto &current_scope = context.back();
      return current_scope[ident];
    }

    static Primitive default_value = std::monostate();
    return default_value;
  }
  Primitive &at(const std::string &ident) {
    for (auto &scope : context) {
      auto it = scope.find(ident);
      if (it != scope.end()) {
        return it->second;
      }
    }

    throw std::out_of_range("Identifier not found: " + ident);
  }

private:
  std::vector<std::map<std::string, Primitive>> context;
  // std::map<std::string, Primitive> context{};
};

} // namespace pheonix::context
