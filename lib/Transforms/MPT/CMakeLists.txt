add_llvm_library(LLVMmpt
    MemoryIsolationPass.cpp

    ADDITIONAL_HEADER_DIRS
    ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms
    ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/MPT
    )

add_dependencies(LLVMmpt intrinsics_gen)
