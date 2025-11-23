# LLVM Strength Reduction Pass

A custom LLVM optimization pass that performs strength reduction on integer operations.

## Implemented Transformations

| Pattern                              | Replacement           | Notes                      |
| ------------------------------------ | --------------------- | -------------------------- |
| `x * 2^k`                            | `x << k`              | Power-of-two multiply      |
| `x / 2^k` (unsigned)                 | `x >> k` (logical)    | Safe for unsigned division |
| `x * C` (low popcount, <=3 bits set) | Sum of shifted values | Reduces MUL latency        |

Each category increments an LLVM `STATISTIC` counter (enable with `LLVM_ENABLE_STATS`).

## Building

Prerequisites: LLVM (>= 15 recommended) built with shared libraries.

```bash
mkdir build && cd build
cmake -DLLVM_DIR=/path/to/llvm/lib/cmake/llvm ..
cmake --build . --config Release
```

## Usage

Generate IR and run via `opt`:

```bash
clang -O0 -S -emit-llvm tests/sample.c -o sample.ll
opt -load-pass-plugin=build/libStrengthReducePass.so -passes="function(strength-reduce)" sample.ll -o optimized.ll
```

### Why This Pass?

LLVM's existing InstCombine & other scalar passes already perform many
strength reductions. Re‑implementing a subset here serves to:

1. Demonstrate understanding of IR pattern matching & legality checks.
2. Show how to write a modern out‑of‑tree pass plugin for the new PM.
3. Provide a base for experimenting with cost models and custom heuristics.

### Future Extensions

- Signed division strength reduction (requires correct rounding semantics).
- Induction variable / loop stride analyses for transforming array indexing.
- Integration with cost model using TargetTransformInfo.
- Vector operation support & pattern matching in SIMD contexts.

### Safety & Correctness

- Transforms only apply when RHS is a constant to keep analysis local.
- Unsigned division chosen; signed division by power-of-two needs correction for negative values.
- Shift-add heuristic constrained to constants < 256 and popcount <= 3 to avoid instruction explosion.
