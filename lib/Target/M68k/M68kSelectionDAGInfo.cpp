//===-- M68kSelectionDAGInfo.cpp - M68kSelectionDAG Info---------*- C++ -*-===//
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

#include "M68kTargetMachine.h"
#define DEBUG_TYPE "m68k-selectiondag-info"

using namespace llvm;

M68kSelectionDAGInfo::M68kSelectionDAGInfo(const M68kTargetMachine &TM)
  : TargetSelectionDAGInfo(TM) {
}
