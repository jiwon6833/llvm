#include "llvm/Transforms/MPT/MemoryIsolationPass.h"

char MemoryIsolationPass::ID = 0;
static RegisterPass<MemoryIsolationPass> P ("MemoryIsolationPass", "LD, ST instrumentation");

bool MemoryIsolationPass::runOnModule(Module & module) {
    
    errs() << "Memory Isolation Pass\n";
    Module *M = &module;

    for (auto curFref = M->getFunctionList().begin(), endFref = M->getFunctionList().end(); curFref != endFref; ++curFref) {
        errs() << "found function: " << curFref->getName() << "\n";

        for (auto &BBI : curFref->getBasicBlockList()) {
            //errs() << "test basicblock\n";
            BasicBlock *BB = &BBI;

            for (auto &InstI : BB->getInstList()) {
                Instruction *insn = &InstI;
                errs() << "found instruction: " << insn->getOpcodeName() << "\n";
            }
            //
            //for (Instruction &I : BBI)
            //    errs() << I << "\n";
        }
        
    }

    return false;
}
