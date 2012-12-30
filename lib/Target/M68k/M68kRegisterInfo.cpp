//===-- M68kRegisterInfo.cpp - M68k Register Information --------*- C++ -*-===//
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

#include "M68kRegisterInfo.h"
#include "llvm/ADT/BitVector.h"

#define GET_REGINFO_TARGET_DESC
#include "M68kGenRegisterInfo.inc"

using namespace llvm;

M68kRegisterInfo::M68kRegisterInfo(M68kTargetMachine &TM,
                                   const TargetInstrInfo &TII)
  : M68kGenRegisterInfo(M68k::PC) {
}

const uint16_t *
M68kRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_SaveList;
}

const uint32_t *M68kRegisterInfo::getCallPreservedMask(CallingConv::ID) const {
  return CSR_RegMask;
}

void M68kRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                           int SPAdj, RegScavenger *RS) const {
  // TODO
}

BitVector M68kRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  Reserved.set(M68k::A5);
  Reserved.set(M68k::A6);
  Reserved.set(M68k::A7);
  return Reserved;
}

unsigned M68kRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return M68k::A6;
}
