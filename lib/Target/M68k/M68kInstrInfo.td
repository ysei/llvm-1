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

//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//

def MOV32dd : M68kInst<(outs DR32:$dst), (ins DR32:$src),
                       "move.l\t$src, $dst",
                       [(set DR32:$dst, DR32:$src)]>;

def MOV16id : M68kInst<(outs DR16:$dst), (ins i16imm:$src),
                       "move.w\t#$src, $dst",
                       [(set DR16:$dst, imm:$src)]>;

def RTS : M68kInst<(outs), (ins), "rts", [(M68kretflag)]>;
