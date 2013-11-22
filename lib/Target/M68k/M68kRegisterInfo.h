//===-- M68kRegisterInfo.h - M68k Register Information ----------*- C++ -*-===//
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

#ifndef M68KREGISTERINFO_H
#define M68KREGISTERINFO_H

#include "M68k.h"
#include "llvm/Target/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "M68kGenRegisterInfo.inc"

namespace llvm {
class M68kTargetMachine;

class M68kRegisterInfo : public M68kGenRegisterInfo {
public:
  M68kRegisterInfo(M68kTargetMachine &TM, const TargetInstrInfo &TII);

  // Code generation virtual methods...
  virtual const uint16_t *
    getCalleeSavedRegs(const MachineFunction *MF = 0) const;
  virtual const uint32_t *getCallPreservedMask(CallingConv::ID) const;
  virtual BitVector getReservedRegs(const MachineFunction &MF) const;

  virtual void eliminateFrameIndex(MachineBasicBlock::iterator MI,
                                   int SPAdj, unsigned FIOperandNum,
                                   RegScavenger *RS = NULL) const;
  virtual unsigned getFrameRegister(const MachineFunction &MF) const;
};

} // end namespace llvm

#endif
