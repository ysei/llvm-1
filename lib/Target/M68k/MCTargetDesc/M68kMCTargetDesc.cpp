//===-- M68kMCTargetDesc.cpp - M68k Target Descriptions----------*- C++ -*-===//
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

#include "M68kMCTargetDesc.h"
#include "M68kMCAsmInfo.h"
#include "M68kMCTargetDesc.h"
#include "InstPrinter/M68kInstPrinter.h"
#include "llvm/MC/MCCodeGenInfo.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_REGINFO_MC_DESC
#include "M68kGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "M68kGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "M68kGenSubtargetInfo.inc"

using namespace llvm;

static MCAsmInfo *createM68kMCAsmInfo(const MCRegisterInfo &MRI, StringRef TT) {
  Triple TheTriple(TT);
  MCAsmInfo *MAI = new M68kMCAsmInfo(TheTriple);
  // Initial frame state
  // TODO What does this do?

  MCCFIInstruction Inst = MCCFIInstruction::createDefCfa(
      0, MRI.getDwarfRegNum(M68k::A7, true), 4);
  MAI->addInitialFrameState(Inst);
  
  return MAI;
}

static MCInstrInfo *createM68kMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo;
  InitM68kMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createM68kMCRegisterInfo(StringRef TT) {
  MCRegisterInfo *X = new MCRegisterInfo;
  InitM68kMCRegisterInfo(X, M68k::PC);
  return X;
}

static MCSubtargetInfo *createM68kMCSubtargetInfo(StringRef TT, StringRef CPU,
                                                  StringRef FS) {
  MCSubtargetInfo *X = new MCSubtargetInfo;
  InitM68kMCSubtargetInfo(X, TT, CPU, FS);
  return X;
}

static MCInstPrinter *createM68kMCInstPrinter(const Target &T,
                                              unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI,
                                              const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI,
                                              const MCSubtargetInfo &STI) {
  return new M68kInstPrinter(MAI, MII, MRI);
}

extern "C" void LLVMInitializeM68kTargetMC() {
  RegisterMCAsmInfoFn X(TheM68kTarget, createM68kMCAsmInfo);

  TargetRegistry::RegisterMCInstrInfo(TheM68kTarget, createM68kMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(TheM68kTarget, createM68kMCRegisterInfo);
  TargetRegistry::RegisterMCSubtargetInfo(TheM68kTarget,
                                          createM68kMCSubtargetInfo);
  TargetRegistry::RegisterMCInstPrinter(TheM68kTarget,
                                        createM68kMCInstPrinter);
}
