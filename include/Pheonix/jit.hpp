#pragma once

#include "Pheonix/context.hpp"
#include "Pheonix/inst_visitor.hpp"
#include "Pheonix/node.hpp"
#include "Pheonix/object.hpp"
#include "Pheonix/operator_visitor.hpp"
#include "Pheonix/types.hpp"
#include "Pheonix/visitor.hpp"

#include <iostream>
#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/TargetSelect.h>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace pheonix {
inline void init() {
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
}

struct JIT : InstVisitor<JIT> {
public:
  JIT()
      : m_result(0),
        m_jit(llvm::ExitOnError()(llvm::orc::LLJITBuilder().create())),
        m_context(std::make_unique<llvm::LLVMContext>()),
        m_module(std::make_unique<llvm::Module>("module", *m_context)),
        m_builder(*m_context) {}
  void visitLiteral(pheonix::node::Literal &) {}
  void visitAdditiveExpression(pheonix::node::AdditiveExpression &) {

    llvm::FunctionType *FT = llvm::FunctionType::get(
        m_builder.getInt32Ty(),
        {m_builder.getInt32Ty(), m_builder.getInt32Ty()}, false);
    llvm::Function *F = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, "add", m_module.get());

    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*m_context, "entry", F);
    m_builder.SetInsertPoint(BB);
    llvm::Value *Sum = m_builder.CreateAdd(F->arg_begin(), F->arg_begin() + 1);
    m_builder.CreateRet(Sum);

    llvm::ExitOnError()(m_jit->addIRModule(llvm::orc::ThreadSafeModule(
        std::move(m_module), std::move(m_context))));

    auto AddSym = llvm::ExitOnError()(m_jit->lookup("add"));
    auto *AddFn = AddSym.toPtr<int (*)(int, int)>();

    int wynik = AddFn(10, 32);
    std::cout << "Wynik JIT (10 + 32): " << wynik << std::endl;
  }
  [[nodiscard]] int const &result() const noexcept { return m_result; }

private:
  int m_result;
  std::unique_ptr<llvm::orc::LLJIT> m_jit;
  std::unique_ptr<llvm::LLVMContext> m_context;
  std::unique_ptr<llvm::Module> m_module;
  llvm::IRBuilder<> m_builder;
};

} // namespace pheonix
