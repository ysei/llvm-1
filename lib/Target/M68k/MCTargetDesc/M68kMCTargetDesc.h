//===-- M68kMCTargetDesc.h - M68k Target Descriptions------------*- C++ -*-===//
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

#ifndef M68KMCTARGETDESC_H
#define M68KMCTARGETDESC_H

namespace llvm {
  class Target;

  extern Target TheM68kTarget;
}  // End llvm namespace

#define GET_REGINFO_ENUM
#include "M68kGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "M68kGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "M68kGenSubtargetInfo.inc"

#endif
