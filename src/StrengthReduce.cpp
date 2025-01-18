#include "StrengthReduce.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include <cmath>

using namespace llvm;

PreservedAnalyses StrengthReducePass::run(Function &F, FunctionAnalysisManager &) {
    bool Modified = false;

    for (auto &BB : F) {
        for (auto &I : BB) {
            if (auto *BO = dyn_cast<BinaryOperator>(&I)) {
                // Match integer multiplication by constant
                if (BO->getOpcode() == Instruction::Mul) {
                    Value *Op0 = BO->getOperand(0);
                    Value *Op1 = BO->getOperand(1);

                    // Check if second operand is a constant power of two
                    if (auto *Const = dyn_cast<ConstantInt>(Op1)) {
                        uint64_t Val = Const->getZExtValue();

                        // Power of two check: n & (n - 1) == 0
                        if (Val > 0 && (Val & (Val - 1)) == 0) {
                            unsigned ShiftAmt = log2(Val);

                            IRBuilder<> B(BO);
                            Value *Shifted =
                                B.CreateShl(Op0, ConstantInt::get(Const->getType(), ShiftAmt),
                                            "mul_pow2");

                            BO->replaceAllUsesWith(Shifted);
                            Modified = true;
                        }
                    }
                }
                // Match unsigned division by power of two
                else if (BO->getOpcode() == Instruction::UDiv) {
                    Value *Op0 = BO->getOperand(0);
                    if (auto *Const = dyn_cast<ConstantInt>(BO->getOperand(1))) {
                        uint64_t Val = Const->getZExtValue();

                        if (Val > 0 && (Val & (Val - 1)) == 0) {
                            unsigned ShiftAmt = log2(Val);

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
