//===-- M68kFrameLowering.h - M68k Frame Lowering----------------*- C++ -*-===//
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

#ifndef M68KFRAMELOWERING_H
#define M68KFRAMELOWERING_H

#include "llvm/Target/TargetFrameLowering.h"

namespace llvm {
  class M68kSubtarget;

  class M68kFrameLowering : public TargetFrameLowering {
  public:
    explicit M68kFrameLowering(const M68kSubtarget &ST);

    // Prologue and Epilogue emission.
    virtual void emitPrologue(MachineFunction &MF) const;
    virtual void emitEpilogue(MachineFunction &MF,
                              MachineBasicBlock &MBB) const;

    // Frame Pointer?
    virtual bool hasFP(const MachineFunction &MF) const;
  };
} // end namespace llvm

#endif
