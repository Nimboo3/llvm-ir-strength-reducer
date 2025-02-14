#!/usr/bin/env bash
set -euo pipefail

SRC=${1:-tests/sample.c}
BUILD_DIR=${BUILD_DIR:-build}
PASS_LIB="$BUILD_DIR/libStrengthReducePass.so"
CLANG=${CLANG:-clang}
OPT=${OPT:-opt}

$CLANG -O0 -S -emit-llvm "$SRC" -o /tmp/orig.ll
$OPT -stats -load-pass-plugin=$PASS_LIB -passes="function(strength-reduce)" /tmp/orig.ll -o /tmp/opt.ll 2> /tmp/stats.txt || true

ORIG_COUNT=$(grep -c '^[[:space:]]*[a-zA-Z]' /tmp/orig.ll || true)
OPT_COUNT=$(grep -c '^[[:space:]]*[a-zA-Z]' /tmp/opt.ll || true)

echo "Instruction counts (rough heuristic):"
printf "  Original:   %s\n" "$ORIG_COUNT"
printf "  Optimized:  %s\n" "$OPT_COUNT"

echo "LLVM pass statistics:" && cat /tmp/stats.txt | grep StrengthReducePass || echo "(No stats output)"
