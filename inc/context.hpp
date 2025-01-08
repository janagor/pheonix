#pragma once

#include "node.hpp"
#include "object.hpp"

#include <cassert>
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace pheonix::context {
using ContextIterator = std::map<std::string, eval::Object>::iterator;

struct Context {
public:
  Context() : inCall(false), context() { push_scope(); }

  void pop_scope();
  void push_scope();

  ContextIterator end();
  ContextIterator find(const std::string &ident);
  ContextIterator find_in_current_scope(const std::string &ident);
  void insert_unique(const std::string &ident, const eval::Object &value);
  void insert(const std::string &ident, const eval::Object &value);
  eval::Object &at(const std::string &ident);
  eval::Object &at_ref(const std::string &ident);

  inline void insertRef(const std::string &ident, const std::string &referenced,
                        bool mut = false) {
    if (!context.empty()) {
      auto &current_scope = context.back();
      current_scope[ident] = eval::Object(
          eval::ObjectValue(std::ref((*this).at(referenced))), mut);
    }
  }

  bool inCall;

private:
  std::vector<std::map<std::string, eval::Object>> context;
};

} // namespace pheonix::context
