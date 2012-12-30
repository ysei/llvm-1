//===-- M68kInstrInfo.cpp - M68k Instruction Information --------*- C++ -*-===//
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

#include "M68kInstrInfo.h"

#define GET_INSTRINFO_CTOR
#include "M68kGenInstrInfo.inc"

using namespace llvm;

M68kInstrInfo::M68kInstrInfo(M68kTargetMachine &TM)
    : TM(TM),
      RI(TM, *this) {
}

const M68kRegisterInfo &M68kInstrInfo::getRegisterInfo() const {
    return RI;
}
