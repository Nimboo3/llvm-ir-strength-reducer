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
            // Match integer multiplication by constant
            if (auto *Mul = dyn_cast<BinaryOperator>(&I)) {
                if (Mul->getOpcode() == Instruction::Mul) {
                    Value *Op0 = Mul->getOperand(0);
                    Value *Op1 = Mul->getOperand(1);

                    // Check if second operand is a constant power of two
                    if (auto *Const = dyn_cast<ConstantInt>(Op1)) {
                        uint64_t Val = Const->getZExtValue();

                        // Power of two check: n & (n - 1) == 0
                        if (Val > 0 && (Val & (Val - 1)) == 0) {
                            unsigned ShiftAmt = log2(Val);

                            IRBuilder<> B(Mul);
                            Value *Shifted =
                                B.CreateShl(Op0, ConstantInt::get(Const->getType(), ShiftAmt),
                                            "mul_pow2");

                            Mul->replaceAllUsesWith(Shifted);
                            Modified = true;
                        }
                    }
                }
            }
        }
    }

    return Modified ? PreservedAnalyses::none() : PreservedAnalyses::all();
}
