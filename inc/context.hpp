#pragma once

#include "node.hpp"
#include "object.hpp"

#include <cassert>
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace pheonix::context {
using ContextObject =
    std::variant<eval::Object, std::reference_wrapper<eval::Object>>;
using ContextIterator = std::map<std::string, ContextObject>::iterator;

struct Context {
public:
  Context() : inCall(false), references({}), context() { push_scope(); }

  void pop_scope();
  void push_scope();

  ContextIterator end();
  ContextIterator find(const std::string &ident);
  ContextIterator find_in_current_scope(const std::string &ident);
  void insert_unique(const std::string &ident, const eval::Object &value);
  void insert(const std::string &ident, const eval::Object &value);
  eval::Object &at(const std::string &ident);

  inline void insertRef(const std::string &ident,
                        const std::string &referenced) {
    if (!context.empty()) {
      auto &current_scope = context.back();
      current_scope[ident] = std::ref((*this).at(referenced));
    }
  }

  bool inCall;
  std::map<std::string, std::string> references;

private:
  std::vector<std::map<std::string, ContextObject>> context;
};

} // namespace pheonix::context
