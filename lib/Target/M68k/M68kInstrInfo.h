//===-- M68kInstrInfo.h - M68k Instruction Information-----------*- C++ -*-===//
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

#ifndef M68KINSTRINFO_H
#define M68KINSTRINFO_H

#include "M68kRegisterInfo.h"
#include "llvm/Target/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "M68kGenInstrInfo.inc"

namespace llvm {
  class M68kTargetMachine;

  class M68kInstrInfo : public M68kGenInstrInfo {
    M68kTargetMachine &TM;
    M68kRegisterInfo RI;

  public:
    explicit M68kInstrInfo(M68kTargetMachine &TM);

    virtual const M68kRegisterInfo &getRegisterInfo() const;

    virtual void copyPhysReg(MachineBasicBlock &MBB,
                             MachineBasicBlock::iterator MI, DebugLoc DL,
                             unsigned DestReg, unsigned SrcReg,
                             bool KillSrc) const;

    virtual void storeRegToStackSlot(MachineBasicBlock &MBB,
                                     MachineBasicBlock::iterator MI,
                                     unsigned SrcReg, bool isKill,
                                     int FrameIndex,
                                     const TargetRegisterClass *RC,
                                     const TargetRegisterInfo *TRI) const;

  virtual void loadRegFromStackSlot(MachineBasicBlock &MBB,
                                    MachineBasicBlock::iterator MI,
                                    unsigned DestReg, int FrameIndex,
                                    const TargetRegisterClass *RC,
                                    const TargetRegisterInfo *TRI) const;

    virtual bool expandPostRAPseudo(MachineBasicBlock::iterator MI) const;

  private:
    bool expandSext(MachineBasicBlock::iterator MI) const;
    bool expandRol(MachineBasicBlock::iterator MI) const;
  };
} // end namespace llvm

#endif
