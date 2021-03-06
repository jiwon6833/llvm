#ifndef TEST_H
#define TEST_H

//#include "llvm/Pass.h"
//#include "llvm/IR/Function.h"
//#include "llvm/Support/raw_ostream.h"
//#include "llvm/ADT/Statistic.h"

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


#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
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
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/CFG.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

class test: public ModulePass{

  private:
    bool runOnModule(Module &);

  public:
    static char ID;
  test(): ModulePass(ID) {
    }
    //const char* getPassName() const { return "test"; }
    StringRef getPassName() const {return "test";}
};

#endif
