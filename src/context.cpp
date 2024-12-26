#include "context.hpp"

namespace pheonix::context {

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
      if (std::holds_alternative<eval::Object>(it->second)) {
        return std::get<eval::Object>(it->second);
      }
      if (std::holds_alternative<std::reference_wrapper<eval::Object>>(
              it->second)) {
        return std::get<std::reference_wrapper<eval::Object>>(it->second).get();
      }
    }
  }
  throw std::out_of_range("Identifier not found: " + ident);
}

} // namespace pheonix::context
