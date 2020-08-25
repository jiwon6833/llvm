#include "AArch64.h"
#include "AArch64Subtarget.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

using namespace llvm;

#define DEBUG_TYPE "aarch64-testpass"

namespace llvm {
    void initializeAArch64TestPassPass(PassRegistry &);
}

#define AARCH64_TESTPASS_NAME "AArch64 test pass"

namespace {
    struct AArch64TestPass : public MachineFunctionPass {
        static char ID;
        AArch64TestPass() : MachineFunctionPass(ID) {
            initializeAArch64TestPassPass(*PassRegistry::getPassRegistry());
        }
        const TargetInstrInfo *TII;
        const AArch64Subtarget *STI;
        int count = 0;
        bool runOnMachineFunction(MachineFunction &Fn) override;
        //const char *getPassName() const override {
        //    return AARCH64_TESTPASS_NAME;
        //}
        StringRef getPassName() const override {
            return AARCH64_TESTPASS_NAME;
        }
    };
    char AArch64TestPass::ID = 0;
}

INITIALIZE_PASS(AArch64TestPass, "aarch64-testpass", AARCH64_TESTPASS_NAME, false, false)

// Returns true if inst. is a mem. op. that this pass is able to operate on
static bool isMemoryOp(const MachineInstr &MI) {
    unsigned Opcode = MI.getOpcode();
    
    // FIXME: probably missing some instructions...
    switch(Opcode) {

    //===------------------------------------------------------===//
    // Load instructions.
    //===------------------------------------------------------===//

    // pair(pre-indexed)
    case AArch64::LDRWpre:
    case AArch64::LDRXpre:
    case AArch64::LDPSpre:
    case AArch64::LDPDpre:
    case AArch64::LDPQpre:
    case AArch64::LDPSWpre:

    // pair(post-indexed)
    case AArch64::LDPWpost:
    case AArch64::LDPXpost:
    case AArch64::LDPSpost:
    case AArch64::LDPDpost:
    case AArch64::LDPQpost:
    case AArch64::LDPSWpost:

    // pair(no allocate)
    //case AArch64::LDNPW:
    //case AArch64::LDNPX:
    //case AArch64::LDNPS:
    //case AArch64::LDNPD:
    //case AArch64::LDNPQ:

    // integer
    //case AArch64::LDRBB:
    //case AArch64::LDRHH:
    //case AArch64::LDRW:
    //case AArch64::LDRX:

    // floating-point
    //case AArch64::LDRB:
    //case AArch64::LDRH:
    //case AArch64::LDRS:
    //case AArch64::LDRD:
    //case AArch64::LDRQ:

    // load sign-extended half-word
    //case AArch64::LDRSHW:
    //case AArch64::LDRSHX:

    // load sign-extended byte
    //case AArch64::LDRSBW:
    //case AArch64::LDRSBX:

    // load sign-extended word
    //case AArch64::LDRSW:

    // pre-fetch
    //case AArch64::PRFM:

    // unscaled immediate
    //case AArch64::LDURX:
    //case AArch64::LDURW:
    //case AArch64::LDURB:
    //case AArch64::LDURH:
    //case AArch64::LDURS:
    //case AArch64::LDURD:
    //case AArch64::LDURQ:
    //case AArch64::LDURHH:
    //case AArch64::LDURBB:

    // unscaled immediate, unprivileged
    //case AArch64::LDTRX:
    //case AArch64::LDTRW:
    //case AArch64::LDTRH:
    //case AArch64::LDTRB:

    //===------------------------------------------------------===//
    // Store instructions.
    //===------------------------------------------------------===//

    // Pair (indexed, offset)
    //case AArch64::STPW:
    //case AArch64::STPX:
    //case AArch64::STPS:
    //case AArch64::STPD:
    //case AArch64::STPQ:

    // Pair (pre-indexed)
    case AArch64::STPWpre:
    case AArch64::STPXpre:
    case AArch64::STPSpre:
    case AArch64::STPDpre:
    case AArch64::STPQpre:

    // Pair (post-indexed)
    case AArch64::STPWpost:
    case AArch64::STPXpost:
    case AArch64::STPSpost:
    case AArch64::STPDpost:
    case AArch64::STPQpost:

    // Pair (no allocate)
    //case AArch64::STNPW:
    //case AArch64::STNPX:
    //case AArch64::STNPS:
    //case AArch64::STNPD:
    //case AArch64::STNPQ:

    // Integer
    //case AArch64::STRBB:
    //case AArch64::STRHH:
    //case AArch64::STRW:
    //case AArch64::STRX:

    // Floating-point
    //case AArch64::STRB:
    //case AArch64::STRH:
    //case AArch64::STRS:
    //case AArch64::STRD:
    //case AArch64::STRQ:

    // unscaled immediate
    //case AArch64::STURX:
    //case AArch64::STURW:
    //case AArch64::STURB:
    //case AArch64::STURH:
    //case AArch64::STURS:
    //case AArch64::STURD:
    //case AArch64::STURQ:
    //case AArch64::STURHH:
    //case AArch64::STURBB:

    // ARMv8.4 weaker release consistency enhancements
    // LDAPR & STLR with Immediate offset instructions
    //case AArch64::STLURB:
    //case AArch64::STLURH:
    //case AArch64::STLURW:
    //case AArch64::STLURX:
    //case AArch64::LDAPURB:
    //case AArch64::LDAPURSBW:
    //case AArch64::LDAPURSBX:
    //case AArch64::LDAPURH:
    //case AArch64::LDAPURSHW:
    //case AArch64::LDAPURSHX:
    //case AArch64::LDAPUR:
    //case AArch64::LDAPURSW:
    //case AArch64::LDAPURX:

    // unscaled immediate, unprivileged
    //case AArch64::STTRW:
    //case AArch64::STTRX:
    //case AArch64::STTRH:
    //case AArch64::STTRB:

    // Load/store exclusive instructions
    case AArch64::LDARW:
    case AArch64::LDARX:
    case AArch64::LDARB:
    case AArch64::LDARH:
    case AArch64::LDAXRW:
    case AArch64::LDAXRX:
    case AArch64::LDAXRB:
    case AArch64::LDAXRH:
    case AArch64::LDXRW:
    case AArch64::LDXRX:
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
    default:
        return false;

    }
    return true;
}

bool AArch64TestPass::runOnMachineFunction(MachineFunction &Fn) {
    STI = &static_cast<const AArch64Subtarget &>(Fn.getSubtarget());
    TII = STI->getInstrInfo();

    bool Modified = false;
    for (MachineBasicBlock &MFI : Fn) {
        for (MachineInstr &MI : MFI) {
            if (!isMemoryOp(MI))
                continue;
            count++;
            dbgs() << "instrumented : " << count << "\n";
            
            unsigned Reg = MI.getOperand(0).getReg();
            BuildMI(MFI, MI, MI.getDebugLoc(), TII->get(AArch64::ADDG), Reg).addReg(Reg).addImm(0);   
        }
    }
    return Modified;
}

FunctionPass *llvm::createAArch64TestPassPass() {
    return new AArch64TestPass();
}
