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
#include "M68kRegisterInfo.h"
#include "M68kTargetMachine.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"

using namespace llvm;

M68kFrameLowering::M68kFrameLowering(const M68kSubtarget &ST)
  : TargetFrameLowering(StackGrowsDown, 2, 0) {
}

void M68kFrameLowering::emitPrologue(MachineFunction &MF) const {
  MachineFrameInfo *MFI = MF.getFrameInfo();
  // Get the number of bytes to allocate from the FrameInfo
  int NumBytes = (int) MFI->getStackSize();
  if (!hasFP(MF)) {
    return;
  }
  MachineBasicBlock &MBB = MF.front();
  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc dl = MBB.begin()->getDebugLoc();
  const M68kInstrInfo &TII =
  *static_cast<const M68kInstrInfo*>(MF.getTarget().getInstrInfo());
  const M68kRegisterInfo *RegInfo =
  static_cast<const M68kRegisterInfo*>(MF.getTarget().getRegisterInfo());
  BuildMI(MBB, MBBI, dl, TII.get(M68k::LINKai), RegInfo->getFrameRegister(MF)).addImm(NumBytes);
}

void M68kFrameLowering::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
  if (!hasFP(MF)) {
    return;
  }
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  DebugLoc dl = MBB.begin()->getDebugLoc();
  const M68kInstrInfo &TII =
  *static_cast<const M68kInstrInfo*>(MF.getTarget().getInstrInfo());
  const M68kRegisterInfo *RegInfo =
  static_cast<const M68kRegisterInfo*>(MF.getTarget().getRegisterInfo());
  BuildMI(MBB, MBBI, dl, TII.get(M68k::UNLKa), RegInfo->getFrameRegister(MF));
}

bool M68kFrameLowering::hasFP(const MachineFunction &MF) const {
  const MachineFrameInfo *MFI = MF.getFrameInfo();
  const MachineModuleInfo &MMI = MF.getMMI();
  
  return (MF.getTarget().Options.DisableFramePointerElim(MF) ||
          MFI->hasVarSizedObjects() ||
          MFI->isFrameAddressTaken() ||
          MMI.callsUnwindInit() || MMI.callsEHReturn());
}
