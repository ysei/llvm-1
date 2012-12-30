//===-- M68kTargetMachine.h - Define TargetMachine for M68k------*- C++ -*-===//
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

#ifndef M68KTARGETMACHINE_H
#define M68KTARGETMACHINE_H

#include "M68kFrameLowering.h"
#include "M68kISelLowering.h"
#include "M68kInstrInfo.h"
#include "M68kSelectionDAGInfo.h"
#include "M68kSubtarget.h"
#include "llvm/DataLayout.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class M68kTargetMachine : public LLVMTargetMachine {
  M68kSubtarget Subtarget;
  const DataLayout DL;
  M68kTargetLowering TLInfo;
  M68kInstrInfo InstrInfo;
  M68kFrameLowering FrameLowering;
  M68kSelectionDAGInfo TSInfo;

  virtual void anchor();

public:
  M68kTargetMachine(const Target &T, StringRef TT, StringRef CPU, StringRef FS,
                    const TargetOptions &Options, Reloc::Model RM,
                    CodeModel::Model CM, CodeGenOpt::Level OL);

  virtual const DataLayout *getDataLayout() const { return &DL; }
  virtual const M68kTargetLowering *getTargetLowering() const {
    return &TLInfo;
  }
  virtual const M68kFrameLowering *getFrameLowering() const {
    return &FrameLowering;
  }
  virtual const M68kSubtarget *getSubtargetImpl() const { return &Subtarget; }
  virtual TargetPassConfig *createPassConfig(PassManagerBase &PM);
  virtual const M68kInstrInfo *getInstrInfo() const { return &InstrInfo; }
  virtual const M68kSelectionDAGInfo *getSelectionDAGInfo() const {
    return &TSInfo;
  }
  virtual const M68kRegisterInfo *getRegisterInfo() const {
    return &InstrInfo.getRegisterInfo();
  }
};

} // end namespace llvm

#endif
