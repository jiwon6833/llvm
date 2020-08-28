#include <iostream>

#include "AArch64.h"
#include "AArch64Subtarget.h"
#include "AArch64RegisterInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/GlobalValue.h"

#define DEBUG_TYPE "aarch64-mpt"

using namespace llvm;

namespace {
struct AArch64MPT : public MachineFunctionPass {

    //public:
    static char ID;
    const TargetInstrInfo *TII;
    const AArch64Subtarget *STI;
    int count = 0;

    AArch64MPT() : MachineFunctionPass(ID) {
        initializeAArch64MPTPass(*PassRegistry::getPassRegistry());
    }
        
    bool runOnMachineFunction(MachineFunction &Fn) override;
    StringRef getPassName() const override { return "AArch64 MPT"; }

};
char AArch64MPT::ID = 0;
} // end anonymous namespace

//char AArch64MPT::ID = 0;
INITIALIZE_PASS(AArch64MPT, DEBUG_TYPE, "AArch64 MPT", false, false)

FunctionPass *llvm::createAArch64MPT() {
    return new AArch64MPT();
}

static bool isLoad(const MachineInstr &MI) {
    unsigned Opcode = MI.getOpcode();
    switch(Opcode) {
        default:
            return false;
        case AArch64::LDPXi:
        case AArch64::LDPDi:
        case AArch64::LDRWpost:
        case AArch64::LDURQi:
        case AArch64::LDURXi:
        case AArch64::LDURDi:
        case AArch64::LDURWi:
        case AArch64::LDURSi:
        case AArch64::LDURSWi:
        case AArch64::LDURHi:
        case AArch64::LDURHHi:
        case AArch64::LDURSHXi:
        case AArch64::LDURSHWi:
        case AArch64::LDURBi:
        case AArch64::LDURBBi:
        case AArch64::LDURSBXi:
        case AArch64::LDURSBWi:
        case AArch64::LDPQi:
        case AArch64::LDNPQi:
        case AArch64::LDRQui:
        case AArch64::LDNPXi:
        case AArch64::LDNPDi:
        case AArch64::LDRXui:
        case AArch64::LDRDui:
        case AArch64::LDPWi:
        case AArch64::LDPSi:
        case AArch64::LDNPWi:
        case AArch64::LDNPSi:
        case AArch64::LDRWui:
        case AArch64::LDRSui:
        case AArch64::LDRSWui:
        case AArch64::LDRHui:
        case AArch64::LDRHHui:
        case AArch64::LDRBui:
        case AArch64::LDRBBui:
            return true;
    }
}

static bool isStore(const MachineInstr &MI) {
    unsigned Opcode = MI.getOpcode();
    switch(Opcode) {
    defualt:
        return false;
    case AArch64::STRWpost:
    case AArch64::STURQi:
    case AArch64::STURXi:
    case AArch64::STURDi:
    case AArch64::STURWi:
    case AArch64::STURSi:
    case AArch64::STURHi:
    case AArch64::STURHHi:
    case AArch64::STURBi:
    case AArch64::STURBBi:
    case AArch64::STPQi:
    case AArch64::STNPQi:
    case AArch64::STRQui:
    case AArch64::STPXi:
    case AArch64::STPDi:
    case AArch64::STNPXi:
    case AArch64::STNPDi:
    case AArch64::STRXui:
    case AArch64::STRDui:
    case AArch64::STPWi:
    case AArch64::STPSi:
    case AArch64::STNPWi:
    case AArch64::STNPSi:
    case AArch64::STRWui:
    case AArch64::STRSui:
    case AArch64::STRHui:
    case AArch64::STRHHui:
    case AArch64::STRBui:
    case AArch64::STRBBui:
        return true;
    }
}

void insertMPTInstr(MachineBasicBlock &MBB, MachineInstr *MIi, unsigned ptrReg, 
                    unsigned modReg, const MCInstrDesc &MCID, const DebugLoc &DL) {
    BuildMI(MBB, MIi, DL, MCID, ptrReg).addReg(modReg);
}

bool AArch64MPT::runOnMachineFunction(MachineFunction &MF) {
    
    dbgs() << "AArch64 MPT\n";

    bool found = false;

    STI = &MF.getSubtarget<AArch64Subtarget>();
    TII = STI->getInstrInfo();

    for (auto &MBB : MF) {
        for (auto MIi = MBB.instr_begin(); MIi != MBB.instr_end(); MIi++) {
            
            const auto MIOpcode = MIi->getOpcode();
            if (isStore(*MIi) || isLoad(*MIi)) {
                
                if (MIi->getOperand(0).isReg()) {
                    unsigned Reg = MIi->getOperand(0).getReg();
                    const auto &DL = MIi->getDebugLoc();
                    BuildMI(MBB, MIi, DL, TII->get(AArch64::MOVKXi), Reg).addReg(Reg).addImm(0).addImm(0);
                    found = true;
                }

                count++;
                dbgs() << count << " ";
                dbgs() << MIi->getOpcode() << " ";
                dbgs() << MIi->getNumOperands() << " ";

                if (MIi->getNumOperands() == 2) {
                    dbgs() << "| ";
                    auto op1 = MIi->getOperand(0);
                    auto op2 = MIi->getOperand(1);
                    if(op2.isImm())
                        dbgs() << "Immediate";
                    if(op2.isCImm())
                        dbgs() << "CImm";
                    if(op2.isFPImm())
                        dbgs() << "FPImm";
                    if(op2.isReg()) {
                        dbgs() << "Register";

                        //unsigned Reg = MIi->getOperand(0).getReg();
                        //const auto &DL = MIi->getDebugLoc();
                        //BuildMI(MBB, MIi, MIi->getDebugLoc(), TII->get(AArch64::ADDXri), Reg).addReg(Reg).addImm(0);
                        //BuildMI(MBB, MIi, DL, TII->get(AArch64::MOVKXi), Reg).addReg(Reg).addImm(0).addImm(0);
                        
                        //found = true;
                    }
                    if(op2.isMBB())
                        dbgs() << "MBB";
                    if(op2.isTargetIndex())
                        dbgs() << "TargetIndex";
                    if(op2.isJTI())
                        dbgs() << "JTI";
                    if(op2.isGlobal()) {
                        dbgs() << "Global ";
                        auto *GV = op2.getGlobal();
                        if(GV->isDefinitionExact()) {
                            dbgs() << "exact " << op2.getOffset();
                            //op2.dump();
                            //Constant* test = GV->
                            //dbgs() << "address space " << GV->getAddressSpace();
                            //GlobalObject *GO = GV->getBaseObject();

                        }
                    }
                    if(op2.isSymbol())
                        dbgs() << "Symbol";
                    if(op2.isBlockAddress())
                        dbgs() << "BlockAddress";
                }
                dbgs() << "\n";
            }
        }
    }
    return found;
}
