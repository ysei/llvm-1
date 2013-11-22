//===-- M68kMCInstLower.cpp - Convert M68k MI to MCInst ---------*- C++ -*-===//
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

#include "M68kMCInstLower.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/ErrorHandling.h"
using namespace llvm;

void M68kMCInstLower::lower(const MachineInstr *MI, MCInst &OutMI) const {
  OutMI.setOpcode(MI->getOpcode());

  for (MachineInstr::const_mop_iterator Op = MI->operands_begin(),
       E = MI->operands_end(); Op != E; ++Op) {
    switch (Op->getType()) {
    default:
      MI->dump();
      llvm_unreachable("unkown operand type");
      break;
    case MachineOperand::MO_Register:
      if (Op->isImplicit()) continue;
      OutMI.addOperand(MCOperand::CreateReg(Op->getReg()));
      break;
    case MachineOperand::MO_Immediate:
      OutMI.addOperand(MCOperand::CreateImm(Op->getImm()));
      break;
    }
  }
}
