//===-- M68kAsmPrinter.h - M68k LLVM Assembly Printer -----------*- C++ -*-===//
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

#ifndef M68KASMPRINTER_H
#define M68KASMPRINTER_H

#include "llvm/CodeGen/AsmPrinter.h"

namespace llvm {

class M68kAsmPrinter : public AsmPrinter {
public:
  M68kAsmPrinter(TargetMachine &TM, MCStreamer &Streamer);

  virtual const char *getPassName() const {
    return "M68k Assembly Printer";
  }

  virtual void EmitFunctionEntryLabel();
  virtual void EmitInstruction(const MachineInstr *MI);
  virtual void EmitEndOfAsmFile(Module &);
};

} // end namespace llvm

#endif
