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
#include "M68kFrameLowering.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFrameInfo.h"

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
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS) const {
  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction &MF = *MBB.getParent();
  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  unsigned FrameReg = getFrameRegister(MF);

  uint64_t StackSize = MF.getFrameInfo()->getStackSize();
  int64_t SpOffset = MF.getFrameInfo()->getObjectOffset(FrameIndex);
  
  if (FrameReg == M68k::A7) {
    SpOffset += 4;
  } else {
    SpOffset += 8;
  }
  
  int64_t Offset = SpOffset + (int64_t)StackSize +
                   MI.getOperand(FIOperandNum + 1).getImm();

  // TODO This doesn't get the correct offset.
  MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
}

BitVector M68kRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  Reserved.set(M68k::A5);
  Reserved.set(M68k::A6);
  Reserved.set(M68k::A7);
  return Reserved;
}

unsigned M68kRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const M68kFrameLowering *TFI =
  static_cast<const M68kFrameLowering*>(MF.getTarget().getFrameLowering());
  if (TFI->hasFP(MF)) {
    return M68k::A6;
  } else {
    return M68k::A7;
  }
}
