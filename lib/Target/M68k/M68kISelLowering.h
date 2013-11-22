//===-- M68kISelLowering.h - M68k DAG Lowering-------------------*- C++ -*-===//
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

#ifndef M68KISELLOWERING_H
#define M68KISELLOWERING_H

#include "M68k.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/Target/TargetLowering.h"

namespace llvm {
  namespace M68kISD {
    enum {
      FIRST_NUMBER = ISD::BUILTIN_OP_END,

      // Return with a flag operand.
      // TODO what is this flag operand for?
      RET_FLAG
    };
  }

  class M68kTargetMachine;

  class M68kTargetLowering : public TargetLowering {
  public:
    explicit M68kTargetLowering(M68kTargetMachine &TM);

    virtual SDValue
    LowerFormalArguments(SDValue /*Chain*/, CallingConv::ID /*CallConv*/,
                         bool /*isVarArg*/,
                         const SmallVectorImpl<ISD::InputArg> &/*Ins*/,
                         SDLoc /*dl*/, SelectionDAG &/*DAG*/,
                         SmallVectorImpl<SDValue> &/*InVals*/) const;

    virtual SDValue
    LowerReturn(SDValue /*Chain*/, CallingConv::ID /*CallConv*/,
                bool /*isVarArg*/,
                const SmallVectorImpl<ISD::OutputArg> &/*Outs*/,
                const SmallVectorImpl<SDValue> &/*OutVals*/,
                SDLoc /*dl*/, SelectionDAG &/*DAG*/) const;
  };
} // end namespace llvm

#endif
