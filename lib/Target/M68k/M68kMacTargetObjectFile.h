//===-- M68kMacTargetObjectFile.h - M68k MPW Object Info --------*- C++ -*-===//
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

#ifndef M68KMACTARGETOBJECTFILE_H
#define M68KMACTARGETOBJECTFILE_H

#include "llvm/ADT/StringMap.h"
#include "llvm/Target/TargetLoweringObjectFile.h"

namespace llvm {

class MCContext;
class MCSectionM68kMac;

class M68kMacTargetLoweringObjectFile : public TargetLoweringObjectFile {
public:
  // Setup
  virtual void Initialize(MCContext &Ctx, const TargetMachine &TM);

  // Create a section for marked globals.
  virtual const MCSection *
  getExplicitSectionGlobal(const GlobalValue *GV, SectionKind Kind,
                           Mangler *Mang, const TargetMachine &TM) const;
};

} // end namespace llvm

#endif
