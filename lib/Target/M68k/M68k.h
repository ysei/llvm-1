//===-- M68k.h - Top-level interface for M68k--------------------*- C++ -*-===//
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

#ifndef TARGET_M68K_H
#define TARGET_M68K_H

#include "MCTargetDesc/M68kMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class M68kTargetMachine;

FunctionPass *createM68kISelDag(M68kTargetMachine &TM);

} // end namespace llvm
#endif
