//===-- M68kFrameLowering.cpp - M68k Frame Lowering--------------*- C++ -*-===//
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

#include "M68kFrameLowering.h"

using namespace llvm;

M68kFrameLowering::M68kFrameLowering(const M68kSubtarget &ST)
  : TargetFrameLowering(StackGrowsDown, 2, 0) {
}

void M68kFrameLowering::emitPrologue(MachineFunction &MF) const {
  // TODO
}

void M68kFrameLowering::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
  // TODO
}

bool M68kFrameLowering::hasFP(const MachineFunction &MF) const {
  // TOOD: Always use frame pointer for now
  return true;
}
