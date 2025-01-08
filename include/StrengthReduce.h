#ifndef LLVM_STRENGTH_REDUCE_H#ifndef LLVM_STRENGTH_REDUCE_H

#define LLVM_STRENGTH_REDUCE_H#define LLVM_STRENGTH_REDUCE_H



#include "llvm/IR/PassManager.h"#include "llvm/IR/PassManager.h"



namespace llvm {namespace llvm {



class StrengthReducePass : public PassInfoMixin<StrengthReducePass> {class StrengthReducePass : public PassInfoMixin<StrengthReducePass> {

public:public:

    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

};};



} // namespace llvm}



#endif#endif

