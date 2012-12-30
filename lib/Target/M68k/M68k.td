//===-- M68k.td - Describe the M68k Target Machine----------*- tablegen -*-===//
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

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// M68k supported processors.
//===----------------------------------------------------------------------===//

def : Processor<"m68k", NoItineraries, []>;

//===----------------------------------------------------------------------===//
// Register File, Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "M68kRegisterInfo.td"
include "M68kCallingConv.td"
include "M68kInstrInfo.td"

def M68kInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// Assembly printer
//===----------------------------------------------------------------------===//

def M68kAsmWriter : AsmWriter {
  string AsmWriterClassName = "InstPrinter";
  bit isMCAsmWriter = 1;
}

//===----------------------------------------------------------------------===//
// Target Declaration
//===----------------------------------------------------------------------===//

def M68k : Target {
  let InstructionSet = M68kInstrInfo;
  let AssemblyWriters = [M68kAsmWriter];
}
