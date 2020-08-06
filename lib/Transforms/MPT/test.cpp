#include "llvm/Transforms/MPT/test.h"

static cl::opt<bool>
test_test
("test_test",
  cl::desc("test"),
  cl::init(true));

char test::ID = 0;
static RegisterPass<test> P ("test", "test pass");

bool test::runOnModule(Module & module) {
  return true;
}
