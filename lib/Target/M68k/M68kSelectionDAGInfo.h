//===-- M68kSelectionDAGInfo.h - M68k SelectionDAG Info----------*- C++ -*-===//
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

#ifndef M68KSELECTIONDAGINFO_H
#define M68KSELECTIONDAGINFO_H

#include "llvm/Target/TargetSelectionDAGInfo.h"

namespace llvm {
  class M68kTargetMachine;

  class M68kSelectionDAGInfo : public TargetSelectionDAGInfo {
  public:
    explicit M68kSelectionDAGInfo(const M68kTargetMachine &TM);
  };
} // end namespace llvm

#endif
