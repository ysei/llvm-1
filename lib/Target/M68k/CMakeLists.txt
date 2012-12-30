set(LLVM_TARGET_DEFINITIONS M68k.td)

tablegen(LLVM M68kGenCallingConv.inc -gen-callingconv)
tablegen(LLVM M68kGenDAGIsel.inc -gen-dag-isel)
tablegen(LLVM M68kGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM M68kGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM M68kGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(M68kCommonTableGen)

add_llvm_target(M68kCodeGen
  M68kFrameLowering.cpp
  M68kISelLowering.cpp
  M68kInstrInfo.cpp
  M68kRegisterInfo.cpp
  M68kSelectionDAGInfo.cpp
  M68kSubtarget.cpp
  M68kTargetMachine.cpp
  )

add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
