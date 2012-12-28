//===-- M68kMCTargetDesc.cpp - M68k Target Descriptions----------*- C++ -*-===//
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

#include "M68kMCTargetDesc.h"
#include "llvm/MC/MCCodeGenInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCInstrInfo.h"

#define GET_REGINFO_MC_DESC
#include "M68kGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "M68kGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "M68kGenSubtargetInfo.inc"

using namespace llvm;

extern "C" void LLVMInitializeM68kTargetMC() {
}
