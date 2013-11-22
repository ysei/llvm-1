add_llvm_library(LLVMM68kDesc
  M68kMCTargetDesc.cpp
  M68kMCAsmInfo.cpp
  )

add_dependencies(LLVMM68kDesc M68kCommonTableGen)
