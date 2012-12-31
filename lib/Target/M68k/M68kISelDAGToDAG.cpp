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
  // Tablegenerated selector
  #include "M68kGenDAGISel.inc"
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

FunctionPass *llvm::createM68kISelDag(M68kTargetMachine &TM) {
  return new M68kDAGToDAGISel(TM);
}
