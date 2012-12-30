//===-- M68kAsmPrinter.cpp - M68k LLVM Assembly Printer ---------*- C++ -*-===//
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

#define DEBUG_TYPE "m68k-asm-printer"
#include "M68kAsmPrinter.h"
#include "M68k.h"
#include "llvm/Support/TargetRegistry.h"

#include <stdio.h>

using namespace llvm;

M68kAsmPrinter::M68kAsmPrinter(TargetMachine &TM, MCStreamer &Streamer)
  : AsmPrinter(TM, Streamer) {
}

extern "C" void LLVMInitializeM68kAsmPrinter() {
  RegisterAsmPrinter<M68kAsmPrinter> X(TheM68kTarget);
}
