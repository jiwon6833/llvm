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
  //module.getOrInsertFunction("__mpt_init",FunctionType::get(VoidTy,args,false));

  //module.getOrInsertFunction("__mte_color_tag", TypeBuilder<void(char *, int), false>::get(module.getContext()));

  //module.getOrInsertFunction("__mte_check_tag", TypeBuilder<void(char *, int), false>::get(module.getContext()));
  
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
  int func_num = 0;
  int bb_num = 0;
  for(Module::iterator ff_begin = module.begin(), ff_end = module.end(); ff_begin != ff_end; ff_begin++){
    if(dyn_cast<Function>(ff_begin)->getName().find("llvm") == 0)
      continue;
    args.resize(0);
    func_num++;
    Constant* func_ptr = ConstantExpr::getBitCast(dyn_cast<Function>(ff_begin), VoidPtrTy);
    args.push_back(func_ptr);
    args.push_back(ConstantInt::get( module.getContext() , APInt(32, StringRef("1"), 10)));
    //CallInst::Create(m_mte_color_tag, args, "",InsertPos); Duplicated coloring operations

    for (auto &BBI : (dyn_cast<Function>(ff_begin))->getBasicBlockList()){
      BasicBlock* BB = dyn_cast<BasicBlock>(&BBI);
      BlockAddress * BA = BlockAddress::get((dyn_cast<Function>(ff_begin)), BB);
      if(!BA)
        continue;
      args.resize(0);
      
      Constant* b_addr = ConstantExpr::getBitCast(dyn_cast<Constant>(BA), VoidPtrTy);

      args.push_back(b_addr);
      args.push_back(ConstantInt::get( module.getContext() , APInt(32, StringRef("1"), 10)));
      CallInst::Create(m_mte_color_tag, args, "",InsertPos);
      bb_num++;
    }
  }
  dbgs() << "colored function : " << func_num << '\n';
  dbgs() << "colored BB : " << bb_num << '\n';
}
void MemoryIsolationPass::InsertColorCheck(Module & module){
  for(Module::iterator ff_begin = module.begin(), ff_end = module.end(); ff_begin != ff_end; ff_begin++){
    if(dyn_cast<Function>(ff_begin)->getName().find("llvm") == 0)
      continue;
    Function* func_ptr = dyn_cast<Function>(ff_begin);
    for(User* U : func_ptr->users()){
      CallInst * CI = dyn_cast<CallInst>(U);
      if(!CI)
        continue;

      Function* func = CI->getCalledFunction();
      if(func)
        if(!checkIfFunctionOfInterest(func))
          continue;
      IRBuilder<> builder(module.getContext());
      builder.SetInsertPoint(CI);

      SmallVector<Value*, 8> args;

      Constant* func_ptr = ConstantExpr::getBitCast(dyn_cast<Function>(CI->getCalledValue()), VoidPtrTy);
      args.push_back(func_ptr);
      args.push_back(ConstantInt::get( module.getContext() , APInt(32, StringRef("1"), 10)));

      CallInst::Create(m_mte_check_tag, args, "", CI);
    }
    
    for(auto &BBI : func_ptr->getBasicBlockList()){
      for(auto &InstI: (&BBI)->getInstList()){
//<<<<<<< Updated upstream
//=======
        //if(isa<IndirectBrInst>(InstI))
          //assert(1 & "indirect")//(&InstI)->dump();
//>>>>>>> Stashed changes
        if(!isa<BranchInst>(InstI))
          continue;
        BranchInst * BI = dyn_cast<BranchInst>(&InstI);

      }
    }// br check
  }
}
bool MemoryIsolationPass::runOnModule(Module & module) {
  InitializeMPT(module);
  InsertInitializeFunction(module);

  InsertColorCheck(module);
  errs() << "Memory Isolation Pass\n";
  return false;
}
