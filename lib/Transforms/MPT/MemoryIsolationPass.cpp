#include "llvm/Transforms/MPT/MemoryIsolationPass.h"

#include "llvm/IR/Argument.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/IRBuilder.h"

char MemoryIsolationPass::ID = 0;
static RegisterPass<MemoryIsolationPass> X ("MemoryIsolationPass", "LD, ST instrumentation", false, true);

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
                //errs() << "found instruction: " << insn->getOpcodeName() << "\n";
                if (StoreInst *sti = dyn_cast<StoreInst>(insn)) {
                  Value *val = sti->getPointerOperand();
                  IRBuilder<> load(insn);
                  LoadInst *test = load.CreateLoad(val, true);
                }
            }
            //
            //for (Instruction &I : BBI)
            //    errs() << I << "\n";
        }
        
    }

    return true;
}
