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

#include "llvm/CodeGen/MachineInstrBuilder.h"

using namespace llvm;

M68kInstrInfo::M68kInstrInfo(M68kTargetMachine &TM)
    : TM(TM),
      RI(TM, *this) {
}

const M68kRegisterInfo &M68kInstrInfo::getRegisterInfo() const {
    return RI;
}

void M68kInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator MI, DebugLoc DL,
                                unsigned DestReg, unsigned SrcReg,
                                bool KillSrc) const {
  if (M68k::DR16RegClass.contains(DestReg, SrcReg)) {
    // TODO what is all this extra stuff
    BuildMI(MBB, MI, DL, get(M68k::MOV16dd), DestReg)
      .addReg(SrcReg, getKillRegState(KillSrc));
    return;
  }

  // TODO
  llvm_unreachable("HACK");
}
