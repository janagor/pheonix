#include "context.hpp"

namespace pheonix::context {

eval::Object &Context::operator[](const std::string &ident) {
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

  static eval::Object default_value = eval::Object();
  return default_value;
}

void Context::push_scope() { context.push_back({}); }

void Context::pop_scope() {
  if (!context.empty()) {
    context.pop_back();
  }
}

ContextIterator Context::end() { return context.back().end(); }

ContextIterator Context::find(const std::string &ident) {
  for (auto &scope : context) {
    auto it = scope.find(ident);
    if (it != scope.end()) {
      return it;
    }
  }
  return context.back().end();
}

ContextIterator Context::find_in_current_scope(const std::string &ident) {
  auto it = context.back().find(ident);
  if (it != context.back().end()) {
    return it;
  }
  return context.back().end();
}

void Context::insert_unique(const std::string &ident,
                            const eval::Object &value) {
  if (!context.empty()) {
    auto &current_scope = context.back();
    if (current_scope.find(ident) == current_scope.end()) {
      current_scope[ident] = value;
    } else {
      std::cout << "Identifier already exists in the current scope.\n";
    }
  }
}

void Context::insert(const std::string &ident, const eval::Object &value) {
  if (!context.empty()) {
    auto &current_scope = context.back();
    current_scope[ident] = value;
  }
}

eval::Object &Context::at(const std::string &ident) {
  for (auto &scope : context) {
    auto it = scope.find(ident);
    if (it != scope.end()) {
      return it->second;
    }
  }
  throw std::out_of_range("Identifier not found: " + ident);
}

} // namespace pheonix::context
