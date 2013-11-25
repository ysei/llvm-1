//===-- MCSectionMPW.cpp - MPW Machine Code Sections ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/MC/MCSectionMPW.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

MCSectionMPW::~MCSectionMPW() { }

void MCSectionMPW::PrintSwitchToSection(const MCAsmInfo &MAI,
                                        raw_ostream &OS,
                                        const MCExpr *Subsection) const {
  OS << "\tSEG\t'" << SectionName << "'\n";
}

bool MCSectionMPW::UseCodeAlign() const {
  // TODO(kwaters): Should be true for code segments?
  return false;
}

bool MCSectionMPW::isVirtualSection() const {
  return false;
}
