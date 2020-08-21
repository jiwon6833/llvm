#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Operator.h"
#include "llvm/ADT/FoldingSet.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/GetElementPtrTypeIterator.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/STLExtras.h"
#include <algorithm>
#include <cstdarg>

#include "llvm/IR/CallingConv.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstIterator.h"
#include "llvm-c/Target.h"
#include "llvm-c/TargetMachine.h"
#include "llvm/IR/Dominators.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/CFG.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/IRBuilder.h"

using namespace llvm;

namespace {
  struct Nomad : public ModulePass {
    static char ID;
    Nomad() : ModulePass(ID) {}
    bool runOnModule(Module & module) {
      
      Module *M = &module;

      for (auto curFref = M->getFunctionList().begin(), endFref = M->getFunctionList().end(); curFref != endFref; ++curFref) {
        for (auto &BBI : curFref->getBasicBlockList()) {
          BasicBlock *BB = &BBI;
          for (auto &InstI : BB->getInstList()) {
            Instruction *insn = &InstI;
            if (StoreInst *sti = dyn_cast<StoreInst>(insn)) {
              Value *val = sti->getPointerOperand();
              IRBuilder<> load(insn);
              LoadInst *test = load.CreateLoad(val);
            }
          }
        }
      }
      return true;
    }
  };
}

char Nomad::ID = 0;
static RegisterPass<Nomad> X("nomad", "nomad pass", false, true);
static RegisterStandardPasses Y(PassManagerBuilder::EP_FullLinkTimeOptimizationLast, [](const PassManagerBuilder &Builder, legacy::PassManagerBase &PM) {PM.add(new Nomad());});
