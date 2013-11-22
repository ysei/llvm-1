//===-- M68kRegisterInfo.td - M68k Register Definitions-----*- tablegen -*-===//
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

//===----------------------------------------------------------------------===//
//  Declarations that describe the M68k register file
//===----------------------------------------------------------------------===//

class M68kReg<bits<3> num, string n> : Register<n> {
  field bits<3> Num = num;

  let Namespace = "M68k";
}

class M68kRegWithSubregs<bits<3> num, string n, list<Register> subregs>
  : RegisterWithSubRegs<n, subregs> {
  field bits<3> Num = num;

  let Namespace = "M68k";
}

let Namespace = "M68k" in {
  def sub_byte : SubRegIndex<8>;
  def sub_word : SubRegIndex<16>;
}

// Data registers are made up of 3 sub-sizes.
multiclass M68kDataReg<bits<3> num, string defn, string n> {
  def B : M68kReg<num, n>;
  def W : M68kRegWithSubregs<num, n, [!cast<Register>(defn # "B")]> {
    let SubRegIndices = [sub_byte];
  }
  def L : M68kRegWithSubregs<num, n, [!cast<Register>(defn # "W")]> {
    let SubRegIndices = [sub_word];
  }
}

//===----------------------------------------------------------------------===//
//  Registers
//===----------------------------------------------------------------------===//

defm D0 : M68kDataReg<0, "D0", "d0">;
defm D1 : M68kDataReg<1, "D1", "d1">;
defm D2 : M68kDataReg<2, "D2", "d2">;
defm D3 : M68kDataReg<3, "D3", "d3">;
defm D4 : M68kDataReg<4, "D4", "d4">;
defm D5 : M68kDataReg<5, "D5", "d5">;
defm D6 : M68kDataReg<6, "D6", "d6">;
defm D7 : M68kDataReg<7, "D7", "d7">;

def A0 : M68kReg<0, "a0">;
def A1 : M68kReg<1, "a1">;
def A2 : M68kReg<2, "a2">;
def A3 : M68kReg<3, "a3">;
def A4 : M68kReg<4, "a4">;
def A5 : M68kReg<5, "a5">;
def A6 : M68kReg<6, "a6">;
def A7 : M68kReg<7, "a7">;

def PC : M68kReg<0, "pc">;
def CCR : M68kReg<0, "ccr">;

//===----------------------------------------------------------------------===//
//  Register Classes
//===----------------------------------------------------------------------===//

def DR8  : RegisterClass<"M68k", [i8],   8, (add (sequence "D%uB", 0, 7))>;
def DR16 : RegisterClass<"M68k", [i16], 16, (add (sequence "D%uW", 0, 7))>;
def DR32 : RegisterClass<"M68k", [i32], 16, (add (sequence "D%uL", 0, 7))>;

def AR   : RegisterClass<"M68k", [i32], 16, (add (sequence "A%u", 0, 7))>;

def CR : RegisterClass<"M68k", [i8], 8, (add CCR)> {
  let CopyCost = -1;
  let isAllocatable = 0;
}
