//===-- M68kInstPrinter.h - Convert M68k MCInst to assembly -----*- C++ -*-===//
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

#ifndef M68KINSTPRINTER_H
#define M68KINSTPRINTER_H

#include "llvm/MC/MCInstPrinter.h"

namespace llvm {

class TargetMachine;

class M68kInstPrinter : public MCInstPrinter {
public:
  M68kInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                  const MCRegisterInfo &MRI);

  virtual void printInst(const MCInst *MI, raw_ostream &O, StringRef Annot);
  virtual void printRegName(raw_ostream &OS, unsigned RegNo) const;

  // tblgen
  void printInstruction(const MCInst *MI, raw_ostream &O);
  static const char *getRegisterName(unsigned RegNo);

private:
  void printOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);
  void printMemOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);
};

} // end namespace llvm

#endif
