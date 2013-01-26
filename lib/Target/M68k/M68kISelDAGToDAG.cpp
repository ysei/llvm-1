//===-- M68kISelDAGToDAG.cpp - M68k Instruction Selector --------*- C++ -*-===//
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

#define DEBUG_TYPE "m68k-isel"
#include "M68k.h"
#include "M68kTargetMachine.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

#include <stdio.h>

using namespace llvm;

namespace {
class M68kDAGToDAGISel : public SelectionDAGISel {
  M68kTargetMachine &TM;
public:
  explicit M68kDAGToDAGISel(M68kTargetMachine &TM)
    : SelectionDAGISel(TM),
      TM(TM) { }

  virtual bool runOnMachineFunction(MachineFunction &MF) {
    bool res = SelectionDAGISel::runOnMachineFunction(MF);
    return res;
  }

  virtual SDNode *Select(SDNode *Node);

private:
  uint64_t invertValue(const ConstantSDNode *N) const {
    const uint64_t mask = (1 << N->getConstantIntValue()->getBitWidth()) - 1;
    return N->getZExtValue() ^ mask;
  }

  // Tablegenerated selector
  #include "M68kGenDAGISel.inc"

  // Complex Patterns.
  bool SelectAddr(SDNode *Parent, SDValue N, SDValue &Base, SDValue &Offset);
};
} // end anonymous namespace

SDNode *M68kDAGToDAGISel::Select(SDNode *Node) {
  // const unsigned Opcode = Node->getOpcode();

  if (Node->isMachineOpcode()) {
    return NULL;
  }

  // Default selection
  SDNode *ResNode = SelectCode(Node);

  return ResNode;
}

bool M68kDAGToDAGISel::SelectAddr(SDNode *Parent, SDValue Addr, SDValue &Base,
                                  SDValue &Offset) {
  DEBUG(dbgs() << "SelectAddr\n");
  DEBUG(Addr->dump());
  EVT ValTy = Addr.getValueType();

  // Frame index matcher
  if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), ValTy);
    Offset = CurDAG->getTargetConstant(0, MVT::i16);
    return true;
  }

  // TODO other memory matching
  Base = Addr;
  Offset = CurDAG->getTargetConstant(0, MVT::i16);
  return true;
}

FunctionPass *llvm::createM68kISelDag(M68kTargetMachine &TM) {
  return new M68kDAGToDAGISel(TM);
}
