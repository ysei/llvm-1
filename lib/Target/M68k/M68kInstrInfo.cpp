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
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

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
  unsigned Opcode = 0;
  if (M68k::DR8RegClass.contains(DestReg, SrcReg))
    Opcode = M68k::MOV8dd;
  else if (M68k::DR16RegClass.contains(DestReg, SrcReg))
    Opcode = M68k::MOV16dd;
  else if (M68k::DR32RegClass.contains(DestReg)) {
    if (M68k::DR32RegClass.contains(SrcReg))
      Opcode = M68k::MOV32dd;
    else if (M68k::ARRegClass.contains(SrcReg))
      Opcode = M68k::MOV32ad;
  } else if (M68k::ARRegClass.contains(DestReg)) {
    if (M68k::DR32RegClass.contains(SrcReg))
      Opcode = M68k::MOVEA32d;
    else if (M68k::ARRegClass.contains(SrcReg))
      Opcode = M68k::MOVEA32a;
  }

  // TODO(kwaters): status register

  if (!Opcode) {
    DEBUG(dbgs() << "Cannot copy " << RI.getName(SrcReg) << " to "
                 << RI.getName(DestReg) << '\n');
    llvm_unreachable("Failed to copy pysreg");
  }

  BuildMI(MBB, MI, DL, get(Opcode), DestReg)
    .addReg(SrcReg, getKillRegState(KillSrc));
}

bool M68kInstrInfo::expandSext(MachineBasicBlock::iterator MI) const {
  unsigned SubReg;
  unsigned MoveOp;
  unsigned ExtOp;

  switch (MI->getOpcode()) {
  default: llvm_unreachable("Bad sign extension");
  case M68k::EXT16_PSEUDO:
    SubReg = M68k::sub_byte;
    MoveOp = M68k::MOV8dd;
    ExtOp = M68k::EXT16;
    break;
  case M68k::EXTB32_PSEUDO:
    SubReg = M68k::sub_byte;
    MoveOp = M68k::MOV8dd;
    ExtOp = M68k::EXT32;
    break;
  case M68k::EXT32_PSEUDO:
    SubReg = M68k::sub_word;
    MoveOp = M68k::MOV16dd;
    ExtOp = M68k::EXT32;
    break;
  }

  unsigned Src = MI->getOperand(1).getReg();
  unsigned Dst = MI->getOperand(0).getReg();

  if (!RI.isSuperRegister(Src, Dst)) {
    // Src and Dst are actually different physical registers, copy from src
    // to dst before sign extending.

    unsigned Middle = RI.getSubReg(Dst, SubReg);
    MI->getOperand(1).setReg(Middle);
    MI->getOperand(1).setIsKill();
    BuildMI(*MI->getParent(), MI, MI->getDebugLoc(), get(MoveOp),
            Middle).addReg(Src);
  }
  assert(MI->getOperand(1).isKill() && "Overwritiing non-killed register.");

  if (MI->getOpcode() == M68k::EXTB32_PSEUDO) {
    // TODO(kwaters): If this is 68020+ use EXTB.L.
    // M68000 doesn't have the EXTB.L instruction, so cascade EXT.W and
    // EXT.L.
    unsigned Middle = RI.getSubReg(Dst, M68k::sub_word);
    BuildMI(*MI->getParent(), MI, MI->getDebugLoc(), get(M68k::EXT16), Middle)
      .addReg(Src, RegState::Implicit | RegState::Kill);
    MI->getOperand(1).setReg(Middle);
  }

  MI->getOperand(1).setImplicit();
  MI->setDesc(get(ExtOp));
  return true;
}

bool M68kInstrInfo::expandPostRAPseudo(MachineBasicBlock::iterator MI) const {
  switch (MI->getOpcode()) {
  default: break;
  case M68k::EXT16_PSEUDO:
  case M68k::EXTB32_PSEUDO:
  case M68k::EXT32_PSEUDO:
    return expandSext(MI);
  }

  return false;
}
