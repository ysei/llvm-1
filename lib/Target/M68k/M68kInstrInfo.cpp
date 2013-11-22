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
    Opcode = M68k::MOVEbdd;
  else if (M68k::DR16RegClass.contains(DestReg, SrcReg))
    Opcode = M68k::MOVEwdd;
  else if (M68k::DR32RegClass.contains(DestReg)) {
    if (M68k::DR32RegClass.contains(SrcReg))
      Opcode = M68k::MOVEldd;
    else if (M68k::ARRegClass.contains(SrcReg))
      Opcode = M68k::MOVElad;
  } else if (M68k::ARRegClass.contains(DestReg)) {
    if (M68k::DR32RegClass.contains(SrcReg))
      Opcode = M68k::MOVElda;
    else if (M68k::ARRegClass.contains(SrcReg))
      Opcode = M68k::MOVElaa;
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

void M68kInstrInfo::
storeRegToStackSlot(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
                    unsigned SrcReg, bool isKill, int FrameIndex,
                    const TargetRegisterClass *RC,
                    const TargetRegisterInfo *TRI) const {
  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  unsigned Opcode = 0;
  if (RC == &M68k::DR32RegClass) {
    Opcode = M68k::MOVEldm;
  } else if (RC == &M68k::DR16RegClass) {
    Opcode = M68k::MOVEwdm;
  } else if (RC == &M68k::DR8RegClass) {
    Opcode = M68k::MOVEbdm;
  } else {
    assert(RC == &M68k::ARRegClass && "Called with wrong RegClass.");
    Opcode = M68k::MOVElam;
  }

  BuildMI(MBB, MI, DL, get(Opcode))
    .addFrameIndex(FrameIndex).addImm(0)
    .addReg(SrcReg, getKillRegState(isKill));
}

void M68kInstrInfo::
loadRegFromStackSlot(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
                     unsigned DestReg, int FrameIndex,
                     const TargetRegisterClass *RC,
                     const TargetRegisterInfo *TRI) const {
  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  unsigned Opcode = 0;
  if (RC == &M68k::DR32RegClass) {
    Opcode = M68k::MOVElmd;
  } else if (RC == &M68k::DR16RegClass) {
    Opcode = M68k::MOVEwmd;
  } else if (RC == &M68k::DR8RegClass) {
    Opcode = M68k::MOVEbmd;
  } else {
    assert(RC == &M68k::ARRegClass && "Called with wrong RegClass.");
    Opcode = M68k::MOVElma;
  }

  BuildMI(MBB, MI, DL, get(Opcode), DestReg)
    .addFrameIndex(FrameIndex).addImm(0);
}

bool M68kInstrInfo::expandSext(MachineBasicBlock::iterator MI) const {
  unsigned SubReg;
  unsigned MoveOp;
  unsigned ExtOp;

  switch (MI->getOpcode()) {
  default: llvm_unreachable("Bad sign extension");
  case M68k::EXTw_PSEUDO:
    SubReg = M68k::sub_byte;
    MoveOp = M68k::MOVEbdd;
    ExtOp = M68k::EXTw;
    break;
  case M68k::EXTBl_PSEUDO:
    SubReg = M68k::sub_byte;
    MoveOp = M68k::MOVEbdd;
    ExtOp = M68k::EXTl;
    break;
  case M68k::EXTl_PSEUDO:
    SubReg = M68k::sub_word;
    MoveOp = M68k::MOVEwdd;
    ExtOp = M68k::EXTl;
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

  if (MI->getOpcode() == M68k::EXTBl_PSEUDO) {
    // TODO(kwaters): If this is 68020+ use EXTB.L.
    // M68000 doesn't have the EXTB.L instruction, so cascade EXT.W and
    // EXT.L.
    unsigned Middle = RI.getSubReg(Dst, M68k::sub_word);
    BuildMI(*MI->getParent(), MI, MI->getDebugLoc(), get(M68k::EXTw), Middle)
      .addReg(Src, RegState::Implicit | RegState::Kill);
    MI->getOperand(1).setReg(Middle);
  }

  MI->getOperand(1).setImplicit();
  MI->setDesc(get(ExtOp));
  return true;
}

bool M68kInstrInfo::expandRol(MachineBasicBlock::iterator MI) const {
  unsigned Size;
  unsigned Opcode;

  switch (MI->getOpcode()) {
  default: llvm_unreachable("Bad ROL");
  case M68k::ROLlid_PSEUDO:
    Size = 32;
    Opcode = M68k::RORlid;
    break;
  case M68k::ROLwid_PSEUDO:
    Size = 16;
    Opcode = M68k::RORwid;
    break;
  }

  // Change from ROL to ROR.
  assert(MI->getNumOperands() == 3 && "Wrong operand count");
  MI->setDesc(get(Opcode));
  MachineOperand &Op = MI->getOperand(2);
  assert(Op.isImm() && "Wrong operand type");
  Op.setImm(Size - Op.getImm());
  return true;
}

bool M68kInstrInfo::expandPostRAPseudo(MachineBasicBlock::iterator MI) const {
  switch (MI->getOpcode()) {
  default: break;
  case M68k::EXTw_PSEUDO:
  case M68k::EXTBl_PSEUDO:
  case M68k::EXTl_PSEUDO:
    return expandSext(MI);
  case M68k::ROLlid_PSEUDO:
  case M68k::ROLwid_PSEUDO:
    return expandRol(MI);
  }

  return false;
}
