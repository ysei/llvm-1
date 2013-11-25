//===-- M68kMacTargetObjectFile.cpp - M68k MPW Object Info ------*- C++ -*-===//
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

#include "M68kMacTargetObjectFile.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCSectionMPW.h"
#include "llvm/Support/ELF.h"

using namespace llvm;

void M68kMacTargetLoweringObjectFile::Initialize(MCContext &Ctx,
                                                 const TargetMachine &TM) {
  // The base class screws up all of our sections, because it initializes us
  // as ELF.
  // TODO(kwaters): Move this into MC/MCObjectFileInfo.cpp.  I has some
  // desire to minimize changes to core code to make it easier to maintain
  // this branch/maximize upstreamability.
  TargetLoweringObjectFile::Initialize(Ctx, TM);

  // EH is not supported.  TODO: Are these reasonable defaults.

  // TODO: What is the Comm directive?
  CommDirectiveSupportsAlignment = false;
  SupportsWeakOmittedEHFrame = false;
  IsFunctionEHFrameSymbolPrivate = false;

  PersonalityEncoding = LSDAEncoding = FDEEncoding = FDECFIEncoding =
    TTypeEncoding = dwarf::DW_EH_PE_absptr;
  EHSectionType = 0;
  EHSectionFlags = 0;

  
  TextSection
    = Ctx.getMPWSection("main",
                        SectionKind::getText());

  // TODO(kwaters): How does data work?  What does data rel mean?
  DataSection
    = Ctx.getMPWSection("main_data",
                        SectionKind::getDataRel());

  // TODO(kwaters): Look at what the standard lib does.
  StaticCtorSection
    = Ctx.getMPWSection("ctor",
                        SectionKind::getDataRel());
  StaticDtorSection
    = Ctx.getMPWSection("dtor",
                        SectionKind::getDataRel());

  /// LSDASection - If exception handling is supported by the target, this is
  /// the section the Language Specific Data Area information is emitted to.
  LSDASection
    = Ctx.getMPWSection("exception",
                        SectionKind::getReadOnlyWithRel());

  // Don't want to invoke demand creation.
  EHFrameSection
    = Ctx.getMPWSection("ehframe",
                        SectionKind::getReadOnly());

  CompactUnwindSection = 0;
  DwarfAccelNamesSection = 0;
  DwarfAccelObjCSection = 0;
  DwarfAccelNamespaceSection = 0;
  DwarfAccelTypesSection = 0;

  // No support for debugging
  DwarfAbbrevSection = 0;
  DwarfInfoSection = 0;
  DwarfLineSection = 0;
  DwarfFrameSection = 0;
  DwarfPubTypesSection = 0;
  DwarfDebugInlineSection = 0;
  DwarfStrSection = 0;
  DwarfLocSection = 0;
  DwarfRangesSection = 0;
  DwarfMacroInfoSection = 0;

  // No support for TLS.
  TLSExtraDataSection = 0;
  TLSDataSection = 0;
  TLSBSSSection = 0;
}

const MCSection *M68kMacTargetLoweringObjectFile::
getExplicitSectionGlobal(const GlobalValue *GV, SectionKind Kind,
                         Mangler *Mang, const TargetMachine &TM) const {
  assert(GV->hasSection() && "GlobalValue missing section.");
  return getContext().getMPWSection(GV->getSection(), Kind);
}
