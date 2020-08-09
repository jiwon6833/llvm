#include "llvm/Transforms/MPT/MemoryIsolationPass.h"

char MemoryIsolationPass::ID = 0;
static RegisterPass<MemoryIsolationPass> P ("MemoryIsolationPass", "LD, ST instrumentation");

bool MemoryIsolationPass::runOnModule(Module & module) {
    
    errs() << "Memory Isolation Pass\n";
    return false;
}
