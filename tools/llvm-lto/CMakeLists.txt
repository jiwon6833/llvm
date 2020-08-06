set(LLVM_LINK_COMPONENTS
  AllTargetsAsmParsers
  AllTargetsCodeGens
  AllTargetsDescs
  AllTargetsInfos
  BitReader
  BitWriter
  Core
  IRReader
  LTO
  MC
  Object
  Support
  Target
  MPT
  )

add_llvm_tool(llvm-lto
  llvm-lto.cpp

  DEPENDS
  intrinsics_gen
  )

