# LLVM Strength Reduction Pass

A custom LLVM optimization pass that performs strength reduction on integer operations.

## Building

```bash
mkdir build && cd build
cmake -DLLVM_DIR=/path/to/llvm/lib/cmake/llvm ..
make
```

## Usage

```bash
clang -O0 -S -emit-llvm tests/sample.c -o sample.ll
opt -load-pass-plugin=build/libStrengthReducePass.so \
    -passes="function(strength-reduce)" \
    sample.ll -o optimized.ll
```
