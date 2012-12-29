//===-- M68kTargetMachine.cpp - Define TargetMachine for M68k----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "M68kTargetMachine.h"
#include "M68k.h"
#include "llvm/PassManager.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

namespace {
  class M68kPassConfig : public TargetPassConfig {
  public:
    M68kPassConfig(M68kTargetMachine *TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) { }

    M68kTargetMachine &getM68kTargetMachine() const {
      return getTM<M68kTargetMachine>();
    }

#if 0
    // TODO
    virtual bool addInstSelector();
#endif
  };
}

extern "C" void LLVMInitializeM68kTarget() {
  RegisterTargetMachine<M68kTargetMachine> X(TheM68kTarget);
}

void M68kTargetMachine::anchor() { }

M68kTargetMachine::
M68kTargetMachine(const Target &T, StringRef TT, StringRef CPU, StringRef FS,
                  const TargetOptions &Options, Reloc::Model RM,
                  CodeModel::Model CM, CodeGenOpt::Level OL)
  : LLVMTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL),
    Subtarget(TT, CPU, FS),
    DL("E-p:32:32-i8:8:8-i16:16:16-i32:16:16"),
    TLInfo(*this),
    InstrInfo(*this),
    FrameLowering(Subtarget),
    TSInfo(*this) {
}

TargetPassConfig *M68kTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new M68kPassConfig(this, PM);
}
