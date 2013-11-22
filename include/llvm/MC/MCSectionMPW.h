//===-- MCSectionMPW.h - MPW Machine Code Sections --------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the MCSectionMPW class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_MC_MCSECTIONMPW_H
#define LLVM_MV_MCSECTIONMPW_H

#include "llvm/ADT/StringRef.h"
#include "llvm/MC/MCSection.h"

namespace llvm {

class MCSectionMPW : public MCSection {
  /// SectionName - This is the name of the section.  Referenced memory is
  /// owned by MCContext.
  StringRef SectionName;

private:
  friend class MCContext;
  MCSectionMPW(StringRef Name, SectionKind K)
    : MCSection(SV_MPW, K), SectionName(Name) { }
  ~MCSectionMPW();

public:
  StringRef getSectionName() const { return SectionName; }

  // Base class overrides
  virtual void PrintSwitchToSection(const MCAsmInfo &MAI,
                                    raw_ostream &OS) const;

  virtual bool isBaseAddressKnownZero() const {
    // All sections are relocatable.
    return false;
  }
  virtual bool UseCodeAlign() const;
  virtual bool isVirtualSection() const;

  // Participate in llvm style RTTI.
  static bool classof(const MCSection *S) {
    return S->getVariant() == SV_MPW;
  }

};

} // end namespace llvm

#endif
