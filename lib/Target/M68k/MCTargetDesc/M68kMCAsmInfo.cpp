//===-- M68kMCAsmInfo.cpp - M68k Asm Info -----------------------*- C++ -*-===//
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

#include "M68kMCAsmInfo.h"
#include "llvm/ADT/StringRef.h"
using namespace llvm;

void M68kMCAsmInfo::anchor() { }

M68kMCAsmInfo::M68kMCAsmInfo(const Triple &Triple) {
  IsLittleEndian = false;
  MaxInstLength = 22;
  MinInstAlignment = 2;
  CommentString = ";";

  ZeroDirective = "DCB.B";
  AsciiDirective = "DC.B";
  AscizDirective = NULL;

  Data8bitsDirective = "DC.B";
  Data16bitsDirective = "DC.W";
  Data32bitsDirective = "DC.L";
  Data64bitsDirective = NULL;

  AlignDirective = "ALIGN";

  // Taken from NVPTX.  I wish we could get rid of it. TODO.
  GlobalDirective = "; GLOBAL ";
  HasSetDirective = false;
  HasDotTypeDotSizeDirective = false;
  HasSingleParameterDotFile = false;
}
