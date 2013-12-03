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

// Address registers are made up of 2 sub-sizes.
multiclass M68kAddrReg<bits<3> num, string defn, string n> {
  def W : M68kReg<num, n>;
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

defm A0 : M68kAddrReg<0, "A0", "a0">;
defm A1 : M68kAddrReg<1, "A1", "a1">;
defm A2 : M68kAddrReg<2, "A2", "a2">;
defm A3 : M68kAddrReg<3, "A3", "a3">;
defm A4 : M68kAddrReg<4, "A4", "a4">;
defm A5 : M68kAddrReg<5, "A5", "a5">;
defm A6 : M68kAddrReg<6, "A6", "a6">;
defm A7 : M68kAddrReg<7, "A7", "a7">;

def PC : M68kReg<0, "pc">;
def CCR : M68kReg<0, "ccr">;

//===----------------------------------------------------------------------===//
//  Register Classes
//===----------------------------------------------------------------------===//

def DR8  : RegisterClass<"M68k", [i8],   8, (add (sequence "D%uB", 0, 7))>;
def DR16 : RegisterClass<"M68k", [i16], 16, (add (sequence "D%uW", 0, 7))>;
def DR32 : RegisterClass<"M68k", [i32], 16, (add (sequence "D%uL", 0, 7))>;

def AR   : RegisterClass<"M68k", [i32], 16, (add (sequence "A%uL", 0, 7))>;
def AR16 : RegisterClass<"M68k", [i16], 16, (add (sequence "A%uW", 0, 7))>;

def R16 : RegisterClass<"M68k", [i16], 16, (add (sequence "D%uW", 0, 7),
                                                (sequence "A%uW", 0, 7))>;
def R32 : RegisterClass<"M68k", [i32], 16, (add (sequence "D%uL", 0, 7),
                                                (sequence "A%uL", 0, 7))>;

def CR : RegisterClass<"M68k", [i8], 8, (add CCR)> {
  let CopyCost = -1;
  let isAllocatable = 0;
}
