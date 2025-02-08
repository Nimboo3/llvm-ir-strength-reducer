#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR=${BUILD_DIR:-build}
PASS_LIB="$BUILD_DIR/libStrengthReducePass.so"

if [[ ! -f "$PASS_LIB" ]]; then
  echo "Pass library not found at $PASS_LIB. Build first." >&2
  exit 1
fi

LLVM_BIN_PREFIX=${LLVM_BIN_PREFIX:-}
OPT_BIN=${OPT_BIN:-${LLVM_BIN_PREFIX}opt}
FILECHECK_BIN=${FILECHECK_BIN:-${LLVM_BIN_PREFIX}FileCheck}

$OPT_BIN -load-pass-plugin=$PASS_LIB -passes="function(strength-reduce)" tests/strength_reduction.ll -S | $FILECHECK_BIN tests/strength_reduction.ll

echo "All FileCheck tests passed."
