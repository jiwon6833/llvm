#include "AArch64.h"
#include "AArch64Subtarget.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

using namespace llvm;

#define DEBUG_TYPE "aarch64-mpt"
#define AARCH64_MPT_NAME "AArch64 mpt pass"

namespace {
//};
struct AArch64MPTPass : public MachineFunctionPass {
    static char ID;
    AArch64MPTPass() : MachineFunctionPass(ID) {
        initializeAArch64MPTPassPass(*PassRegistry::getPassRegistry());
    }

    const TargetInstrInfo *TII;
    const AArch64Subtarget *STI;
    int count = 0;

    bool runOnMachineFunction(MachineFunction &Fn) override;
    StringRef getPassName() const override { return AARCH64_MPT_NAME; }
};

char AArch64MPTPass::ID = 0;
} // end anonymous namespace

INITIALIZE_PASS(AArch64MPTPass, "aarch64-mpt", AARCH64_MPT_NAME, false, false)

static bool isMemoryOp(const MachineInstr &MI) {
    unsigned Opcode = MI.getOpcode();
    
    switch (Opcode) {

        case AArch64::LDRWpre:
        case AArch64::LDRXpre:
        case AArch64::LDPSpre:
        case AArch64::LDPDpre:
        case AArch64::LDPQpre:
        case AArch64::LDPSWpre:

        case AArch64::LDPWpost:
        case AArch64::LDPXpost:
        case AArch64::LDPSpost:
        case AArch64::LDPDpost:
        case AArch64::LDPQpost:
        case AArch64::LDPSWpost:

        case AArch64::STPWpre:
        case AArch64::STPXpre:
        case AArch64::STPSpre:
        case AArch64::STPDpre:
        case AArch64::STPQpre:

        case AArch64::STPWpost:
        case AArch64::STPXpost:
        case AArch64::STPSpost:
        case AArch64::STPDpost:
        case AArch64::STPQpost:

        case AArch64::LDARW:
        case AArch64::LDARX:
        case AArch64::LDARB:
        case AArch64::LDARH:
        case AArch64::LDAXRW:
        case AArch64::LDAXRX:
        case AArch64::LDAXRB:
        case AArch64::LDAXRH:
        case AArch64::LDXRW:
        case AArch64::LDXRB:
        case AArch64::LDXRH:
        case AArch64::STLRW:
        case AArch64::STLRX:
        case AArch64::STLRB:
        case AArch64::STLRH:
        case AArch64::STLXRW:
        case AArch64::STLXRX:
        case AArch64::STLXRB:
        case AArch64::STLXRH:
        case AArch64::STXRW:
        case AArch64::STXRX:
        case AArch64::STXRB:
        case AArch64::STXRH:
        case AArch64::LDAXPW:
        case AArch64::LDAXPX:
        case AArch64::LDXPW:
        case AArch64::LDXPX:
        case AArch64::STLXPW:
        case AArch64::STLXPX:
        case AArch64::STXPW:
        case AArch64::STXPX:
            break;

        // FIXME:
        // Additional(?) LD, STR instructions in LoadStoreOpt Pass

        default:
            return false;
    }
    return true;
}

bool AArch64MPTPass::runOnMachineFunction(MachineFunction &Fn) {
    
    STI = &static_cast<const AArch64Subtarget &>(Fn.getSubtarget());
    TII = STI->getInstrInfo();

    bool Modified = false;
    for (MachineBasicBlock &MFI : Fn) {
        for (MachineInstr &MI : MFI) {
            if (isMemoryOp(MI)) {
                //continue;
                count++;
                dbgs() << "instrumented : " << count << "\n";

                unsigned Reg = MI.getOperand(0).getReg();
                BuildMI(MFI, MI, MI.getDebugLoc(), TII->get(AArch64::ADDG), Reg).addReg(Reg).addImm(0);

                Modified = true;
            }
        }
    }
    return Modified;
}

FunctionPass *llvm::createAArch64MPTPass() {
    return new AArch64MPTPass();
}
