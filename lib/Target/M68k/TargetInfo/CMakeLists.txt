include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_llvm_library(LLVMM68kInfo
  M68kTargetInfo.cpp
  )

add_dependencies(LLVMM68kInfo M68kCommonTableGen)
