#include "StrengthReduce.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include <cmath>

using namespace llvm;

namespace {
// Build a shift-add chain for x * C where C has few set bits
Value *buildShiftAddChain(IRBuilder<> &B, Value *X, const APInt &C) {
    SmallVector<Value *> Terms;
    for (unsigned Bit = 0; Bit < C.getBitWidth(); ++Bit) {
        if (C[Bit]) {
            if (Bit == 0) {
                Terms.push_back(X);
            } else {
                Terms.push_back(B.CreateShl(X, Bit));
            }
        }
    }
    Value *Acc = Terms[0];
    for (unsigned i = 1; i < Terms.size(); ++i) {
        Acc = B.CreateAdd(Acc, Terms[i]);
    }
    return Acc;
}
} // namespace

PreservedAnalyses StrengthReducePass::run(Function &F, FunctionAnalysisManager &) {
    bool Modified = false;

    for (auto &BB : F) {
        for (auto &I : BB) {
            if (auto *BO = dyn_cast<BinaryOperator>(&I)) {
                // Match integer multiplication by constant
                if (BO->getOpcode() == Instruction::Mul) {
                    Value *Op0 = BO->getOperand(0);
                    Value *Op1 = BO->getOperand(1);

                    if (auto *Const = dyn_cast<ConstantInt>(Op1)) {
                        const APInt &C = Const->getValue();
                        if (C.isNullValue() || C == 1) continue;

                        IRBuilder<> B(BO);
                        
                        // Power of two -> single shift
                        if (C.isPowerOf2()) {
                            unsigned ShiftAmt = C.logBase2();
                            Value *Shifted =
                                B.CreateShl(Op0, ConstantInt::get(Const->getType(), ShiftAmt),
                                            "mul_pow2");
                            BO->replaceAllUsesWith(Shifted);
                            Modified = true;
                        }
                        // Low popcount: use shift-add chain
                        else if (C.countPopulation() <= 3 && C.ult(256)) {
                            Value *Chain = buildShiftAddChain(B, Op0, C);
                            Chain->setName("mul_addchain");
                            BO->replaceAllUsesWith(Chain);
                            Modified = true;
                        }
                    }
                }
                // Match unsigned division by power of two
                else if (BO->getOpcode() == Instruction::UDiv) {
                    Value *Op0 = BO->getOperand(0);
                    if (auto *Const = dyn_cast<ConstantInt>(BO->getOperand(1))) {
                        const APInt &C = Const->getValue();

                        if (C.isPowerOf2()) {
                            unsigned ShiftAmt = C.logBase2();
                            IRBuilder<> B(BO);
                            Value *Shifted =
                                B.CreateLShr(Op0, ConstantInt::get(Const->getType(), ShiftAmt),
                                             "udiv_pow2");
                            BO->replaceAllUsesWith(Shifted);
                            Modified = true;
                        }
                    }
                }
            }
        }
    }

    return Modified ? PreservedAnalyses::none() : PreservedAnalyses::all();
}
