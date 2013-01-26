//===-- M68kISelLowering.cpp - M68k DAG Lowering ----------------*- C++ -*-===//
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

#include "M68kISelLowering.h"
#include "M68k.h"
#include "M68kTargetMachine.h"
#include "M68kRegisterInfo.h"
#include "M68kMacTargetObjectFile.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
using namespace llvm;

M68kTargetLowering::M68kTargetLowering(M68kTargetMachine &TM)
  : TargetLowering(TM, new M68kMacTargetLoweringObjectFile()) {
  // Setup register classes
  addRegisterClass(MVT::i8, &M68k::DR8RegClass);
  addRegisterClass(MVT::i16, &M68k::DR16RegClass);
  addRegisterClass(MVT::i32, &M68k::DR32RegClass);
  computeRegisterProperties();

  // We have no extending loads.
  for (unsigned ExtType = (unsigned)ISD::EXTLOAD;
       ExtType < (unsigned)ISD::LAST_LOADEXT_TYPE; ++ExtType) {
    // TODO(kwaters): What about i1 and float?
    setLoadExtAction(ExtType, MVT::i8, Expand);
    setLoadExtAction(ExtType, MVT::i16, Expand);
    setLoadExtAction(ExtType, MVT::i32, Expand);
  }

  // Easiest to match truncation stores with the truncating patterns.
  setTruncStoreAction(MVT::i16, MVT::i8, Expand);
  setTruncStoreAction(MVT::i32, MVT::i8, Expand);
  setTruncStoreAction(MVT::i32, MVT::i16, Expand);
}


//===----------------------------------------------------------------------===//
//  Calling Convention
//===----------------------------------------------------------------------===//

#include "M68kGenCallingConv.inc"

SDValue M68kTargetLowering::
LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                     bool isVarArg,
                     const SmallVectorImpl<ISD::InputArg> &Ins, DebugLoc dl,
                     SelectionDAG &DAG,
                     SmallVectorImpl<SDValue> &InVals) const {
  // Check for the right calling convention.
  switch (CallConv) {
  default:
    llvm_unreachable("Unsupported calling convention");
  case CallingConv::C:
  case CallingConv::Fast:
    break;
  }

  // TODO
  assert(!isVarArg && "VarArg unsupported");

  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo *MFI = MF.getFrameInfo();

  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(),
                 getTargetMachine(), ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, CC_M68k);

  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];

    int FI = MFI->CreateFixedObject(VA.getLocVT().getStoreSize(),
                                    VA.getLocMemOffset(), true);
    SDValue FIN = DAG.getFrameIndex(FI, VA.getLocVT());
    InVals.push_back(DAG.getLoad(VA.getValVT(), dl, Chain, FIN,
                                 MachinePointerInfo::getFixedStack(FI),
                                 false, false, false, 0));
  }

  return Chain;
}

SDValue
M68kTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                bool isVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutVals,
                                DebugLoc dl, SelectionDAG &DAG) const {
  // Check for the right calling convention.
  switch (CallConv) {
  default:
    llvm_unreachable("Unsupported calling convention");
  case CallingConv::C:
  case CallingConv::Fast:
    break;
  }

  // TODO
  assert(!isVarArg && "VarArg unsupported");

  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(),
                 getTargetMachine(), RVLocs, *DAG.getContext());
  CCInfo.AnalyzeReturn(Outs, RetCC_M68k);

  // Mark out registers as live outs.
  // TODO simplify
  if (DAG.getMachineFunction().getRegInfo().liveout_empty()) {
    for (unsigned i = 0; i != RVLocs.size(); ++i)
      if (RVLocs[i].isRegLoc())
        DAG.getMachineFunction().getRegInfo().addLiveOut(RVLocs[i].getLocReg());
  }

  SDValue Flag;

  // TODO struct returns through stack voodoo.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers");
    Chain = DAG.getCopyToReg(Chain, dl, VA.getLocReg(), OutVals[i], Flag);

    // Guarantee that all emitted copies are stuck together,
    // avoiding something bad.
    // TODO: what does this mean?  We only know how to return one value does
    // this matter?
    Flag = Chain.getValue(1);
  }

  if (Flag.getNode())
    return DAG.getNode(M68kISD::RET_FLAG, dl, MVT::Other, Chain, Flag);
  return DAG.getNode(M68kISD::RET_FLAG, dl, MVT::Other, Chain);
}
