; RUN: opt -load-pass-plugin=%b/../build/libStrengthReducePass.so -passes="function(strength-reduce)" %s -S | FileCheck %s

define i32 @mul_pow2(i32 %x) {
; CHECK-LABEL: @mul_pow2
; CHECK: shl i32 %x, 3
entry:
  %mul = mul i32 %x, 8
  ret i32 %mul
}

define i32 @mul_chain(i32 %x) {
; CHECK-LABEL: @mul_chain
; CHECK: add i32
; CHECK: shl i32 %x, 2
entry:
  %mul = mul i32 %x, 5
  ret i32 %mul
}

define i32 @udiv_pow2(i32 %x) {
; CHECK-LABEL: @udiv_pow2
; CHECK: lshr i32 %x, 2
entry:
  %div = udiv i32 %x, 4
  ret i32 %div
}
