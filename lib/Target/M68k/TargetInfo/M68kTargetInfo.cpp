//===-- M68kTargetInfo.cpp - M68k Target Implementation -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "M68k.h"
#include "llvm/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target llvm::TheM68kTarget;

extern "C" void LLVMInitializeM68kTargetInfo() {
  RegisterTarget<Triple::m68k, false> X(TheM68kTarget, "m68k", "M68k");
}
