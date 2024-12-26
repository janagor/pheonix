#pragma once

#include "node.hpp"
#include "object.hpp"

#include <cassert>
#include <map>
#include <string>
#include <vector>

namespace pheonix::context {
using ContextIterator = std::map<std::string, eval::Object>::iterator;

struct Context {
public:
  Context() : context() { push_scope(); }

  eval::Object &operator[](const std::string &ident);

  void pop_scope();
  void push_scope();

  ContextIterator end();
  ContextIterator find(const std::string &ident);
  ContextIterator find_in_current_scope(const std::string &ident);
  void insert_unique(const std::string &ident, const eval::Object &value);
  void insert(const std::string &ident, const eval::Object &value);
  eval::Object &at(const std::string &ident);

private:
  std::vector<std::map<std::string, eval::Object>> context;
};

} // namespace pheonix::context
