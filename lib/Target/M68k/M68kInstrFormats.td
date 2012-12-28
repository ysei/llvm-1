//===-- M68kInstrFormats.td - M68k Instruction Formats------*- tablegen -*-===//
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

class M68kInst<dag outs, dag ins, string asmstr, list<dag> pattern>
  : Instruction {
  let Namespace = "M68k";

  let OutOperandList = outs;
  let InOperandList = ins;

  let AsmString = asmstr;

  let Pattern = pattern;
}
