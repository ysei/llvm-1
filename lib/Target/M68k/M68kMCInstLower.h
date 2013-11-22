//===-- M68kMCInstLower.h - Convert M68k MI to MCInst -----------*- C++ -*-===//
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

#ifndef M68KMCINSTLOWER_H
#define M68KMCINSTLOWER_H

namespace llvm {

class MachineInstr;
class MCInst;

class M68kMCInstLower {
public:
  void lower(const MachineInstr *MI, MCInst &OutMI) const;
};

} // end namespace llvm

#endif
