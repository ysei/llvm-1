//===-- M68kSubtarget.cpp - M68k Subtarget Information-----------*- C++ -*-===//
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

#include "M68kSubtarget.h"
#include "M68k.h"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "M68kGenSubtargetInfo.inc"

using namespace llvm;

M68kSubtarget::M68kSubtarget(const std::string &TT, const std::string &CPU,
                             const std::string &FS) :
  M68kGenSubtargetInfo(TT, CPU, FS) {
  // TODO(kwaters): Instruction Itineraries
}
