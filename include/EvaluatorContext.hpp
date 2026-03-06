#pragma once

#include <Value.hpp>
#include <ValueDef.hpp>

#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace pheonix {

class GlobalEnvironment {

public:
  void define(std::string const &name, Value value) {
    m_variables[name] = std::move(value);
  }

  [[nodiscard]] bool set(std::string const &name, Value value) {
    if (auto it = m_variables.find(name); it != m_variables.end()) {
      it->second = std::move(value);
      return true;
    }
    return false;
  }

  [[nodiscard]] std::optional<Value> get(std::string const &name) const {
    if (auto it = m_variables.find(name); it != m_variables.end()) {
      return it->second;
    }
    return std::nullopt;
  }

private:
  std::unordered_map<std::string, Value> m_variables;
};

using Environment = std::unordered_map<std::string, Value>;

struct CallFrame {

  std::vector<Environment> m_blockScopes;
  CallFrame() { m_blockScopes.emplace_back(); }
};

class EvaluatorContext {

public:
  explicit EvaluatorContext(GlobalEnvironment &globals) : m_globalEnv(globals) {
    m_callStack.emplace_back();
  }

  void pushCallFrame() { m_callStack.emplace_back(); }
  void popCallFrame() { m_callStack.pop_back(); }

  void pushBlockScope() { m_callStack.back().m_blockScopes.emplace_back(); }
  void popBlockScope() { m_callStack.back().m_blockScopes.pop_back(); }

  void defineVariable(std::string const &name, Value value) {
    m_callStack.back().m_blockScopes.back()[name] = std::move(value);
  }

  [[nodiscard]] std::optional<Value>
  getVariable(std::string const &name) const {
    const auto &current_frame = m_callStack.back();
    for (auto it = current_frame.m_blockScopes.rbegin();
         it != current_frame.m_blockScopes.rend(); ++it) {
      if (auto found = it->find(name); found != it->end()) {
        return found->second;
      }
    }

    if (auto global_val = m_globalEnv.get(name)) {
      return *global_val;
    }
    return std::nullopt;
  }

  [[nodiscard]] bool setVariable(std::string const &name, Value value) {
    auto &current_frame = m_callStack.back();

    for (auto it = current_frame.m_blockScopes.rbegin();
         it != current_frame.m_blockScopes.rend(); ++it) {
      if (auto found = it->find(name); found != it->end()) {
        found->second = std::move(value);
        return true;
      }
    }
    if (m_globalEnv.set(name, std::move(value))) {
      return true;
    }
    return false;
  }

private:
  GlobalEnvironment &m_globalEnv;
  std::vector<CallFrame> m_callStack;
};

struct BlockScopeGuard {
  EvaluatorContext &ctx;
  explicit BlockScopeGuard(EvaluatorContext &c) : ctx(c) {
    ctx.pushBlockScope();
  }
  ~BlockScopeGuard() { ctx.popBlockScope(); }
  BlockScopeGuard(const BlockScopeGuard &) = delete;
  BlockScopeGuard &operator=(const BlockScopeGuard &) = delete;
};

} // namespace pheonix
