#pragma once

#include "node.hpp"
#include <map>
#include <string>
#include <vector>

namespace pheonix::context {

struct Context {
public:
  Context() : context() {}
  void pop_scope();
  void push_scope();
  void find(std::string ident);
  void insert_unique();
  void insert();

private:
  std::vector<std::map<std::string, types::Integer>> context{};
};

} // namespace pheonix::context
