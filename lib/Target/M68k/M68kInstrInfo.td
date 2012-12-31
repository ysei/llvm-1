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

// TODO learn about operands, what are they exactly, why do they differ from
// patterns.  Theory: operand is for storing stuff in MCInst, pattern is for
// selection.
def simm16 : Operand<i32>;

// TODO match all types of memory based EAs (effictive addresses).
def mem : Operand<i32> {
  let MIOperandInfo = (ops AR, simm16);
  let PrintMethod = "printMemOperand";
}

// Pattern for immediates for "quick" instructions.
def immQ : ImmLeaf<i32, [{ return isInt<8>(Imm); }]>;

def addr : ComplexPattern<iPTR, 2, "SelectAddr", [frameindex],
                          [SDNPWantParent]>;

//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//

def MOV32dd : M68kInst<(outs DR32:$dst), (ins DR32:$src),
                       "move.l\t$src, $dst", []>;

def MOVQ32 : M68kInst<(outs DR32:$dst), (ins i32imm:$src),
                      "moveq\t$src, $dst",
                      [(set DR32:$dst, immQ:$src)]>;

def MOV32id : M68kInst<(outs DR32:$dst), (ins i32imm:$src),
                       "move.l\t$src, $dst",
                       [(set DR32:$dst, imm:$src)]>;

def MOV16md : M68kInst<(outs DR16:$dst), (ins mem:$src),
                       "move.w\t$src, $dst",
                       [(set DR16:$dst, (load addr:$src))]>;

def MOV16id : M68kInst<(outs DR16:$dst), (ins i16imm:$src),
                       "move.w\t$src, $dst",
                       [(set DR16:$dst, imm:$src)]>;

def MOV16dd : M68kInst<(outs DR16:$dst), (ins DR16:$src),
                      "move.w\t$src, $dst", []>;

let Constraints = "$srca = $dst" in {
def ADD16dd : M68kInst<(outs DR16:$dst), (ins DR16:$srca, DR16:$srcb),
                       "add.w\t$srcb, $dst",
                       [(set DR16:$dst, (add DR16:$srca, DR16:$srcb))]>;
def ADD16md : M68kInst<(outs DR16:$dst), (ins DR16:$srca, mem:$srcb),
                        "add.w\t$srcb, $dst",
                        [(set DR16:$dst, (add DR16:$srca,
                                              (load addr:$srcb)))]>;
// TODO limit this immediate to 16 bits.
def ADD16id : M68kInst<(outs DR16:$dst), (ins DR16:$srca, i16imm:$srcb),
                       "addi.w\t$srcb, $dst",
                       [(set DR16:$dst, (add DR16:$srca, imm:$srcb))]>;
}

// TODO look at how X86 does its reg->mem binops.

def RTS : M68kInst<(outs), (ins), "rts", [(M68kretflag)]> {
  let isReturn = 1;
  let isTerminator = 1;
}
