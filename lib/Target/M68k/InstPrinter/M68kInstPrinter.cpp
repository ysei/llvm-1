//===-- M68kInstPrinter.cpp - Convert M68k MCInst to assembly ---*- C++ -*-===//
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

#define DEBUG_TYPE "asm-printer"
#include "M68kInstPrinter.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#include "M68kGenAsmWriter.inc"

M68kInstPrinter::M68kInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                                 const MCRegisterInfo &MRI)
  : MCInstPrinter(MAI, MII, MRI) {
}


void M68kInstPrinter::printInst(const MCInst *MI, raw_ostream &O,
                                StringRef Annot) {
  printInstruction(MI, O);
  printAnnotation(O, Annot);
}

void M68kInstPrinter::printRegName(raw_ostream &OS, unsigned RegNo) const {
  OS << getRegisterName(RegNo);
}

void M68kInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                   raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);

  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }
  if (Op.isImm()) {
    O << "#" << Op.getImm();
    return;
  }

  llvm_unreachable("Unknown operand type.");
}

void M68kInstPrinter::printMemOperand(const MCInst *MI, unsigned OpNo,
                                      raw_ostream &O) {
  const MCOperand &Base = MI->getOperand(OpNo);
  const MCOperand Disp = MI->getOperand(OpNo + 1);
  assert(Base.isReg() && Disp.isImm());

  if (uint64_t imm = Disp.getImm()) {
    O << imm;
  }
  O << "(";
  printRegName(O, Base.getReg());
  O << ")";
}
