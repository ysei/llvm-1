//===-- M68kInstrInfo.td - M68k Instruction Definition------*- tablegen -*-===//
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

include "M68kInstrFormats.td"


//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//

def M68kretflag : SDNode<"M68kISD::RET_FLAG", SDTNone,
                         [SDNPHasChain, SDNPOptInGlue]>;

// Pattern for immediates for "quick" instructions.
def immQ : ImmLeaf<i32, [{ return isInt<8>(Imm); }]>;

//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//

def MOV32dd : M68kInst<(outs DR32:$dst), (ins DR32:$src),
                       "move.l\t$src, $dst", []>;

def MOVQ32 : M68kInst<(outs DR32:$dst), (ins i8imm:$src),
                      "moveq\t$src, $dst",
                      [(set DR32:$dst, immQ:$src)]>;

def MOV32id : M68kInst<(outs DR32:$dst), (ins i32imm:$src),
                       "move.l\t$src, $dst",
                       [(set DR32:$dst, imm:$src)]>;

def MOV16id : M68kInst<(outs DR16:$dst), (ins i16imm:$src),
                       "move.w\t$src, $dst",
                       [(set DR16:$dst, imm:$src)]>;

def RTS : M68kInst<(outs), (ins), "rts", [(M68kretflag)]> {
  let isReturn = 1;
  let isTerminator = 1;
}
