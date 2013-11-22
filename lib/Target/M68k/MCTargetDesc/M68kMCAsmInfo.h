//===-- M68kMCAsmInfo.h - M68k Asm Info -------------------------*- C++ -*-===//
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

#ifndef M68KMCASMINFO_H
#define M68KMCASMINFO_H

#include "llvm/MC/MCAsmInfo.h"

namespace llvm {
  class Triple;

  class M68kMCAsmInfo : public MCAsmInfo {
    virtual void anchor();
  public:
    M68kMCAsmInfo(const Triple &Triple);
  };
} // end namespace llvm

#endif
