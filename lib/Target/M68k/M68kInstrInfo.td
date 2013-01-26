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

// TODO match all types of memory based EAs (effective addresses).
def mem : Operand<i32> {
  let MIOperandInfo = (ops AR, simm16);
  let PrintMethod = "printMemOperand";
}

// Pattern for immediates for "quick" instructions.
def immQ : ImmLeaf<i32, [{ return isInt<8>(Imm); }]>;

def i32imm1_8 : ImmLeaf<i32, [{ return 1 <= Imm && Imm <= 8; }]>;
def i16imm1_8 : ImmLeaf<i16, [{ return 1 <= Imm && Imm <= 8; }]>;
def i8imm1_8  : ImmLeaf<i8,  [{ return 1 <= Imm && Imm <= 8; }]>;

def i32imm9_15  : ImmLeaf<i32, [{ return  9 <= Imm && Imm <= 15; }]>;
def i32imm24_31 : ImmLeaf<i32, [{ return 24 <= Imm && Imm <= 31; }]>;

def addr : ComplexPattern<iPTR, 2, "SelectAddr", [frameindex],
                          [SDNPWantParent]>;

class SizeInfo<ValueType vt, string asmflag, RegisterClass regclass,
               Operand immoperand, SDPatternOperator quickimm> {
  // VT - Value type this size represents.
  ValueType VT = vt;

  // AsmFlag - Assembler mnemonic flag.
  string AsmFlag = asmflag;

  // Register class for this size.
  RegisterClass RegClass = regclass;

  // ImmOperand - Immediate value for the type.
  Operand ImmOperand = immoperand;

  // QuickImm - Immediate operator for quick add/sub.
  SDPatternOperator QuickImm = quickimm;
}

def SizeL : SizeInfo<i32, ".l", DR32, i32imm, i32imm1_8>;
def SizeW : SizeInfo<i16, ".w", DR16, i16imm, i16imm1_8>;
def SizeB : SizeInfo<i8,  ".b", DR8,  i8imm,  i8imm1_8>;

//===----------------------------------------------------------------------===//
// MOVE (all flavors)
//===----------------------------------------------------------------------===//

// Register-Register moves (unselectable)

multiclass M68kMoveInst<ValueType vt, string size, RegisterClass reg,
                        Operand immop> {
  let AsmString = !strconcat("move.", size, "\t$src, $dst") in {
    def dd : M68kInst<(outs reg:$dst), (ins reg:$src), "", []>;
    def id : M68kInst<(outs reg:$dst), (ins immop:$src), "",
                      [(set reg:$dst, imm:$src)]>;
    def md : M68kInst<(outs reg:$dst), (ins mem:$src), "",
                      [(set reg:$dst, (load addr:$src))]>;

    def im : M68kInst<(outs), (ins mem:$dst, immop:$src), "",
                      [(store (vt imm:$src), addr:$dst)]>;
    def dm : M68kInst<(outs), (ins mem:$dst, reg:$src), "",
                      [(store reg:$src, addr:$dst)]>;
    def mm : M68kInst<(outs), (ins mem:$dst, mem:$src), "",
                      [(store (vt (load addr:$src)), addr:$dst)]>;

    // TODO(kwaters): am, ad flavors including truncation, I'm not sure it can
    // be selected automatically.
  }
}

def MOVEQ : M68kInst<(outs DR32:$dst), (ins i32imm:$src),
                     "moveq\t$src, $dst",
                     [(set DR32:$dst, immQ:$src)]>;

defm MOVEb : M68kMoveInst<i8, "b", DR8, i8imm>;
defm MOVEw : M68kMoveInst<i16, "w", DR16, i16imm>;
defm MOVEl : M68kMoveInst<i32, "l", DR32, i32imm>;

// A register moves for copyPhysReg
def MOVElaa : M68kInst<(outs AR:$dst), (ins AR:$src),
                       "movea.l\t$src, $dst", []>;
def MOVElad : M68kInst<(outs DR32:$dst), (ins AR:$src),
                       "move.l\t$src, $dst", []>;
def MOVElda : M68kInst<(outs AR:$dst), (ins DR32:$src),
                       "movea.l\t$src, $dst", []>;

def MOVElam : M68kInst<(outs), (ins mem:$dst, AR:$src),
                       "move.l\t$src, $dst", []>;
def MOVElma : M68kInst<(outs AR:$dst), (ins mem:$src),
                       "move.l\t$src, $dst", []>;

// TODO(kwaters): rename MOVEAld to MOVElad, etc.
// TODO(kwaters): MOVEAlm MOVEAli
// TODO(kwaters): MOVEM
// TODO(kwaters): MOVEP

def LEA : M68kInst<(outs AR:$dst), (ins mem:$src2), "lea\t$src2, $dst",
                   [(set AR:$dst, addr:$src2)]>;

//===----------------------------------------------------------------------===//

let Defs = [CCR] in {
let Constraints = "$src1 = $dst" in {
class BinOp_DD<SizeInfo sz, string mnemonic, SDNode op>
  : M68kInst<(outs sz.RegClass:$dst),
             (ins sz.RegClass:$src1, sz.RegClass:$src2),
             !strconcat(mnemonic, sz.AsmFlag, "\t$src2, $dst"),
             [(set sz.RegClass:$dst,
                   (op sz.RegClass:$src1, sz.RegClass:$src2)),
              (implicit CCR)]>;

class BinOp_MD<SizeInfo sz, string mnemonic, SDNode op>
  : M68kInst<(outs sz.RegClass:$dst), (ins sz.RegClass:$src1, mem:$src2),
             !strconcat(mnemonic, sz.AsmFlag, "\t$src2, $dst"),
             [(set sz.RegClass:$dst,
                   (op sz.RegClass:$src1, (load addr:$src2))),
              (implicit CCR)]>;

class BinOp_ID<SizeInfo sz, string mnemonic, SDNode op>
  : M68kInst<(outs sz.RegClass:$dst),
             (ins sz.RegClass:$src1, sz.ImmOperand:$src2),
             !strconcat(mnemonic, "i", sz.AsmFlag, "\t$src2, $dst"),
             [(set sz.RegClass:$dst, (op sz.RegClass:$src1, imm:$src2)),
              (implicit CCR)]>;

class BinOp_QD<SizeInfo sz, string mnemonic, SDNode op>
  : M68kInst<(outs sz.RegClass:$dst),
             (ins sz.RegClass:$src1, sz.ImmOperand:$src2),
             !strconcat(mnemonic, "q", sz.AsmFlag, "\t$src2, $dst"),
             [(set sz.RegClass:$dst,
                   (op sz.RegClass:$src1, sz.QuickImm:$src2)),
              (implicit CCR)]>;

// TODO(kwaters): Register class of $src2 doesn't matter, it's clipped to 6
// bits.
class ShiftOp_DD<SizeInfo sz, string mnemonic, SDNode op>
  : M68kInst<(outs sz.RegClass:$dst),
             (ins sz.RegClass:$src1, DR32:$src2),
             !strconcat(mnemonic, sz.AsmFlag, "\t$src2, $dst"),
             [(set sz.RegClass:$dst, (op sz.RegClass:$src1, DR32:$src2)),
              (implicit CCR)]>;

class ShiftOp_ID<SizeInfo sz, string mnemonic, SDNode op>
  : M68kInst<(outs sz.RegClass:$dst), (ins sz.RegClass:$src1, i32imm:$src2),
             !strconcat(mnemonic, sz.AsmFlag, "\t$src2, $dst"),
             [(set sz.RegClass:$dst, (op sz.RegClass:$src1, i32imm1_8:$src2)),
              (implicit CCR)]>;
} // Constraints = "$src1 = $dst"

let Constraints = "$src = $dst" in {
class UnaryOp_D<SizeInfo sz, string mnemonic, SDPatternOperator op>
  : M68kInst<(outs sz.RegClass:$dst), (ins sz.RegClass:$src),
             !strconcat(mnemonic, sz.AsmFlag, "\t$dst"),
             [(set sz.RegClass:$dst, (op sz.RegClass:$src)),
              (implicit CCR)]>;
} // Constraints = "$src = $dst"

class BinOp_DM<SizeInfo sz, string mnemonic, SDNode op>
  : M68kInst<(outs), (ins mem:$dst, sz.RegClass:$src),
             !strconcat(mnemonic, sz.AsmFlag, "\t$src, $dst"),
             [(store (op (sz.VT (load addr:$dst)), sz.RegClass:$src),
                     addr:$dst),
              (implicit CCR)]>;

class BinOp_IM<SizeInfo sz, string mnemonic, SDNode op>
  : M68kInst<(outs), (ins mem:$dst, sz.ImmOperand:$src),
             !strconcat(mnemonic, "i", sz.AsmFlag, "\t$src, $dst"),
             [(store (op (sz.VT (load addr:$dst)), imm:$src), addr:$dst),
              (implicit CCR)]>;

class BinOp_QM<SizeInfo sz, string mnemonic, SDNode op>
  : M68kInst<(outs), (ins mem:$dst, sz.ImmOperand:$src),
             !strconcat(mnemonic, "q", sz.AsmFlag, "\t$src, $dst"),
             [(store (op (sz.VT (load addr:$dst)), sz.QuickImm:$src),
                     addr:$dst),
              (implicit CCR)]>;

class ShiftOp_M<SizeInfo sz, string mnemonic, SDNode op>
  : M68kInst<(outs), (ins mem:$dst),
             !strconcat(mnemonic, sz.AsmFlag, "\t$dst"),
             [(store (op (sz.VT (load addr:$dst)), (i32 1)), addr:$dst),
              (implicit CCR)]>;

class UnaryOp_M<SizeInfo sz, string mnemonic, SDPatternOperator op>
  : M68kInst<(outs), (ins mem:$dst),
             !strconcat(mnemonic, sz.AsmFlag, "\t$dst"),
             [(store (op (sz.VT (load addr:$dst))), addr:$dst),
              (implicit CCR)]>;
} // Defs = [CCR]

multiclass BinOp<string mnemonic, SDNode op> {
  def ldd : BinOp_DD<SizeL, mnemonic, op>;
  def wdd : BinOp_DD<SizeW, mnemonic, op>;
  def bdd : BinOp_DD<SizeB, mnemonic, op>;
  def lmd : BinOp_MD<SizeL, mnemonic, op>;
  def wmd : BinOp_MD<SizeW, mnemonic, op>;
  def bmd : BinOp_MD<SizeB, mnemonic, op>;
  def lid : BinOp_ID<SizeL, mnemonic, op>;
  def wid : BinOp_ID<SizeW, mnemonic, op>;
  def bid : BinOp_ID<SizeB, mnemonic, op>;
  def ldm : BinOp_DM<SizeL, mnemonic, op>;
  def wdm : BinOp_DM<SizeW, mnemonic, op>;
  def bdm : BinOp_DM<SizeB, mnemonic, op>;
  def lim : BinOp_IM<SizeL, mnemonic, op>;
  def wim : BinOp_IM<SizeW, mnemonic, op>;
  def bim : BinOp_IM<SizeB, mnemonic, op>;
}

multiclass BinOpAdd<string mnemonic, SDNode op> : BinOp<mnemonic, op> {
  def lqd : BinOp_QD<SizeL, mnemonic, op>;
  def wqd : BinOp_QD<SizeW, mnemonic, op>;
  def bqd : BinOp_QD<SizeB, mnemonic, op>;
  def lqm : BinOp_QM<SizeL, mnemonic, op>;
  def wqm : BinOp_QM<SizeW, mnemonic, op>;
  def bqm : BinOp_QM<SizeB, mnemonic, op>;

  // TODO(kwaters): ADDA and SUBA opcodes, forms AA DA IA MA.  These forms
  // can't be selected, because they conflict with the D forms.  However,
  // it might be possible to pair them with the D forms.  In the "word" forms
  // they include the sign extension of the source before adding/subtracting.

  // TODO(kwaters): ADDQ #-1 can be converted to SUBQ #1.  Does it make sense
  // to do this conversion during lowering or during instruction selection?
  // How about as a post selection peep-hole?
}

multiclass ShiftOp<string mnemonic, SDNode op> {
  def ldd : ShiftOp_DD<SizeL, mnemonic, op>;
  def wdd : ShiftOp_DD<SizeW, mnemonic, op>;
  def bdd : ShiftOp_DD<SizeB, mnemonic, op>;
  def lid : ShiftOp_ID<SizeL, mnemonic, op>;
  def wid : ShiftOp_ID<SizeW, mnemonic, op>;
  def bid : ShiftOp_ID<SizeB, mnemonic, op>;
  def lm  : ShiftOp_M< SizeL, mnemonic, op>;
  def wm  : ShiftOp_M< SizeW, mnemonic, op>;
  def bm  : ShiftOp_M< SizeB, mnemonic, op>;
}

multiclass UnaryOp<string mnemonic, SDPatternOperator op> {
  def ld : UnaryOp_D<SizeL, mnemonic, op>;
  def wd : UnaryOp_D<SizeW, mnemonic, op>;
  def bd : UnaryOp_D<SizeB, mnemonic, op>;
  def lm : UnaryOp_M<SizeL, mnemonic, op>;
  def wm : UnaryOp_M<SizeW, mnemonic, op>;
  def bm : UnaryOp_M<SizeB, mnemonic, op>;
}

defm NEG : UnaryOp<"neg", ineg>;
defm NOT : UnaryOp<"not", not>;

defm AND : BinOp<"and", and>;
defm EOR : BinOp<"eor", xor>;
defm OR : BinOp<"or", or>;

defm ADD : BinOpAdd<"add", add>;
defm SUB : BinOpAdd<"sub", sub>;

defm LSL : ShiftOp<"lsl", shl>;
defm LSR : ShiftOp<"lsr", srl>;
defm ASR : ShiftOp<"asr", sra>;
defm ROL : ShiftOp<"rol", rotl>;
defm ROR : ShiftOp<"ror", rotr>;
// TODO(kwaters): ASL, ROXL, ROXR exists as instructions, but aren't intresting
// for selection.

let Defs = [CCR], Constraints = "$dst = $src" in
def SWAPl : M68kInst<(outs DR32:$dst), (ins DR32:$src),
                     "swap.w\t$dst",
                     [(set DR32:$dst, (rotl DR32:$src, (i32 16))),
                      (implicit CCR)]>;

// Shift instructions truncate the shift amount to 6 bits before shifting, so
// we can fold a zeroextend into them.
multiclass ShiftZext<SDNode op, RegisterClass regclass, Instruction inst> {
    def : Pat<(op regclass:$dst, (zext DR16:$src)),
              (inst regclass:$dst, (INSERT_SUBREG (i32 (IMPLICIT_DEF)),
                                                  DR16:$src, sub_word))>;
    def : Pat<(op regclass:$dst, (i32 (zext DR8:$src))),
              (inst regclass:$dst, (INSERT_SUBREG (i32 (IMPLICIT_DEF)),
                                                  DR8:$src, sub_byte))>;
}
defm : ShiftZext<shl,  DR16, LSLwdd>;
defm : ShiftZext<shl,  DR8,  LSLbdd>;
defm : ShiftZext<srl,  DR16, LSRwdd>;
defm : ShiftZext<srl,  DR8,  LSRbdd>;
defm : ShiftZext<sra,  DR16, ASRwdd>;
defm : ShiftZext<sra,  DR8,  ASRbdd>;
defm : ShiftZext<rotl, DR16, ROLwdd>;
defm : ShiftZext<rotl, DR8,  ROLbdd>;
defm : ShiftZext<rotr, DR16, RORwdd>;
defm : ShiftZext<rotr, DR8,  RORbdd>;

// Psuedo instructions for converting from ROL #imm, Dn to ROR #imm, Dn.
let Constraints = "$src1 = $dst", isPseudo = 1 in {
def ROLlid_PSEUDO
  : M68kInst<(outs DR32:$dst), (ins DR32:$src1, i32imm:$src2), "",
             [(set DR32:$dst, (rotl DR32:$src1, i32imm24_31:$src2))]>;
def ROLwid_PSEUDO
  : M68kInst<(outs DR16:$dst), (ins DR16:$src1, i32imm:$src2), "",
             [(set DR16:$dst, (rotl DR16:$src1, i32imm9_15:$src2))]>;
}

// TODO(kwaters): bit manipulating instructions.

//===----------------------------------------------------------------------===//
// Extended adds and subtracts
//===----------------------------------------------------------------------===//

// We're only really intrested in selecting ADDX and SUBX in the long size.
// Otherwise we shouldn't be doing an extended add.
let Uses = [CCR], Defs = [CCR], Constraints = "$src1 = $dst" in {
def ADDXldd : M68kInst<(outs DR32:$dst), (ins DR32:$src1, DR32:$src2),
                       "addx.l\t$src2, $dst",
                       [(set DR32:$dst, (adde DR32:$src1, DR32:$src2)),
                        (implicit CCR)]>;

def SUBXldd : M68kInst<(outs DR32:$dst), (ins DR32:$src1, DR32:$src2),
                       "subx.l\t$src2, $dst",
                       [(set DR32:$dst, (sube DR32:$src1, DR32:$src2)),
                        (implicit CCR)]>;

// Other sizes are not selected.
def ADDXwdd : M68kInst<(outs DR16:$dst), (ins DR16:$src1, DR16:$src2),
                       "addx.w\t$src2, $dst", []>;
def ADDXbdd : M68kInst<(outs DR8:$dst), (ins DR8:$src1, DR8:$src2),
                       "addx.b\t$src2, $dst", []>;
def SUBXwdd : M68kInst<(outs DR16:$dst), (ins DR16:$src1, DR16:$src2),
                       "subx.w\t$src2, $dst", []>;
def SUBXbdd : M68kInst<(outs DR8:$dst), (ins DR8:$src1, DR8:$src2),
                       "subx.b\t$src2, $dst", []>;

// TODO(kwaters): -(An), -(An) forms.
}

// addc/subc match patterns.
// TODO(kwaters): Do we want all of the forms?  What can we do to simplify
// them?
def : Pat<(addc DR32:$src1, imm:$src2), (ADDlid DR32:$src1, imm:$src2)>;
def : Pat<(addc DR32:$src1, DR32:$src2), (ADDldd DR32:$src1, DR32:$src2)>;
def : Pat<(addc DR32:$src, (load addr:$src2)), (ADDlmd DR32:$src, addr:$src2)>;
def : Pat<(subc DR32:$src1, DR32:$src2), (SUBldd DR32:$src1, DR32:$src2)>;
def : Pat<(subc DR32:$src, (load addr:$src2)), (SUBlmd DR32:$src, addr:$src2)>;

//===----------------------------------------------------------------------===//
// Extension
//===----------------------------------------------------------------------===//

def : Pat<(i16 (zext DR8:$src)),
          (INSERT_SUBREG (EXTRACT_SUBREG (MOVEQ 0), sub_word),
                         DR8:$src, sub_byte)>;
def : Pat<(i32 (zext DR8:$src)),
          (INSERT_SUBREG (MOVEQ 0), DR8:$src, sub_byte)>;
def : Pat<(i32 (zext DR16:$src)),
          (INSERT_SUBREG (MOVEQ 0), DR16:$src, sub_word)>;

// Model sign extension as pseudo instructions with two operands.
// TODO(kwaters): Is this the best way to model a one-operand instruction?  Do
// we have any gaurentee that the register allocator will tend to put the src
// and dst in the same physical register?
let isPseudo = 1 in {
def EXTw_PSEUDO : M68kInst<(outs DR16:$dst), (ins DR8:$src),
                           "PSEUDO ext.w\t$src, $dst",
                           [(set DR16:$dst, (sext DR8:$src))]>;
def EXTBl_PSEUDO : M68kInst<(outs DR32:$dst), (ins DR8:$src),
                            "PSEUDO extb.l\t$src, $dst",
                            [(set DR32:$dst, (sext DR8:$src))]>;
def EXTl_PSEUDO : M68kInst<(outs DR32:$dst), (ins DR16:$src),
                           "PSEUDO ext.l\t$src, $dst",
                           [(set DR32:$dst, (sext DR16:$src))]>;
}

// Acutal sign extending opcodes.
def EXTl : M68kInst<(outs DR32:$dst), (ins), "ext.l\t$dst", []>;
def EXTw : M68kInst<(outs DR16:$dst), (ins), "ext.w\t$dst", []>;

// TODO(kwaters): anyext

//===----------------------------------------------------------------------===//
// Flow control
//===----------------------------------------------------------------------===//

def RTS : M68kInst<(outs), (ins), "rts", [(M68kretflag)]> {
  let isReturn = 1;
  let isTerminator = 1;
}
