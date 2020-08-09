#include "llvm/Transforms/MPT/MemoryIsolationPass.h"




char MemoryIsolationPass::ID = 0;
static RegisterPass<MemoryIsolationPass> P ("MemoryIsolationPass", "LD, ST instrumentation");

                                   

void MemoryIsolationPass::InitializeMPT(Module & module){
  
  VoidTy = Type::getVoidTy(module.getContext());
  VoidPtrTy = PointerType::getUnqual(Type::getInt8Ty(module.getContext()));
  SizeTy = Type::getInt64Ty(module.getContext());
  Int32Ty = Type::getInt32Ty(module.getContext());
  Int8PtrTy = Type::getInt8PtrTy(module.getContext());

  SmallVector<Type*,8> args;  
  module.getOrInsertFunction("__mpt_init",FunctionType::get(VoidTy,args,false));

  args.push_back(Int32Ty);
  args.push_back(Int32Ty);
  module.getOrInsertFunction("__mte_color_tag", TypeBuilder<void(char *, int), false>::get(module.getContext()));

  args.resize(0);
  module.getOrInsertFunction("__mte_check_tag", FunctionType::get(VoidTy,args,false));
  
  m_initialize_mpt = module.getFunction("__mpt_init");
  assert(m_initialize_mpt &&
  "m_initialize_mpt function type null?");
  
  m_mte_check_tag = module.getFunction("__mte_check_tag");
  
  assert(m_mte_check_tag &&
         "m_mte_check_check function type null?"); 

  m_mte_color_tag = module.getFunction("__mte_color_tag");
  assert(m_mte_color_tag &&
         "m_mte_color_tag function type null?"); 



  return;
}
bool MemoryIsolationPass::isFuncDefMPT(StringRef func_name){
  //TODO : coloring expected functions
  if(m_func_def_mpt.getNumItems() == 0){
    m_func_def_mpt["__mte_color_tag"] = true;
    m_func_def_mpt["__mte_check_tag"] = true;
    m_func_def_mpt["__mpt_init"] = true;
  }

  if(m_func_def_mpt.count(func_name) > 0){
    return true;
  }
  return false;
}
bool MemoryIsolationPass::checkIfFunctionOfInterest(Function* func){
  if(func->isDeclaration())
    return false;
  if(isFuncDefMPT(func->getName()))
    return false;

  return true;
}
void MemoryIsolationPass::InsertInitializeFunction(Module& module){
  //insert initialize function
  Function* main_func = module.getFunction("main");
  if(!main_func)
    return;

  Instruction * InsertPos;
  BasicBlock::iterator It = main_func->getEntryBlock().getFirstInsertionPt();
  while (isa<AllocaInst>(*It) || isa<DbgInfoIntrinsic>(*It) || isa<BitCastInst>(*It))
    ++It;
  InsertPos = &*It;

  IRBuilder<> builder(module.getContext());
  builder.SetInsertPoint(InsertPos);

  ConstantInt* tag_num  = ConstantInt::get( module.getContext() , APInt(32, StringRef("1"), 10));

  SmallVector<Value*, 8> args;
  CallInst::Create(m_initialize_mpt, args,"", InsertPos);
}
bool MemoryIsolationPass::runOnModule(Module & module) {
  InitializeMPT(module);
  InsertInitializeFunction(module);
  //find function address
  for(Module::iterator ff_begin = module.begin(), ff_end = module.end(); ff_begin != ff_end; ++ff_begin){
    Function * func_ptr = dyn_cast<Function>(ff_begin);
  }
  errs() << "Memory Isolation Pass\n";
  return false;
}
