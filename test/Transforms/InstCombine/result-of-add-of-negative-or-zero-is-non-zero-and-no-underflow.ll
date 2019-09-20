; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt %s -instcombine -S | FileCheck %s

declare void @use8(i8)

declare void @use1(i1)
declare void @llvm.assume(i1)

define i1 @t0(i8 %base, i8 %offset) {
; CHECK-LABEL: @t0(
; CHECK-NEXT:    [[ADJUSTED:%.*]] = add i8 [[BASE:%.*]], [[OFFSET:%.*]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[NOT_NULL:%.*]] = icmp ne i8 [[ADJUSTED]], 0
; CHECK-NEXT:    [[NO_UNDERFLOW:%.*]] = icmp ule i8 [[ADJUSTED]], [[BASE]]
; CHECK-NEXT:    [[R:%.*]] = and i1 [[NOT_NULL]], [[NO_UNDERFLOW]]
; CHECK-NEXT:    ret i1 [[R]]
;
  %adjusted = add i8 %base, %offset
  call void @use8(i8 %adjusted)
  %not_null = icmp ne i8 %adjusted, 0
  %no_underflow = icmp ule i8 %adjusted, %base
  %r = and i1 %not_null, %no_underflow
  ret i1 %r
}

; We need to produce extra instruction, so one of icmp's must go away.
define i1 @t1_oneuse0(i8 %base, i8 %offset) {
; CHECK-LABEL: @t1_oneuse0(
; CHECK-NEXT:    [[ADJUSTED:%.*]] = add i8 [[BASE:%.*]], [[OFFSET:%.*]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[NOT_NULL:%.*]] = icmp ne i8 [[ADJUSTED]], 0
; CHECK-NEXT:    call void @use1(i1 [[NOT_NULL]])
; CHECK-NEXT:    [[NO_UNDERFLOW:%.*]] = icmp ule i8 [[ADJUSTED]], [[BASE]]
; CHECK-NEXT:    [[R:%.*]] = and i1 [[NOT_NULL]], [[NO_UNDERFLOW]]
; CHECK-NEXT:    ret i1 [[R]]
;
  %adjusted = add i8 %base, %offset
  call void @use8(i8 %adjusted)
  %not_null = icmp ne i8 %adjusted, 0
  call void @use1(i1 %not_null)
  %no_underflow = icmp ule i8 %adjusted, %base
  %r = and i1 %not_null, %no_underflow
  ret i1 %r
}
define i1 @t2_oneuse1(i8 %base, i8 %offset) {
; CHECK-LABEL: @t2_oneuse1(
; CHECK-NEXT:    [[ADJUSTED:%.*]] = add i8 [[BASE:%.*]], [[OFFSET:%.*]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[NOT_NULL:%.*]] = icmp ne i8 [[ADJUSTED]], 0
; CHECK-NEXT:    [[NO_UNDERFLOW:%.*]] = icmp ule i8 [[ADJUSTED]], [[BASE]]
; CHECK-NEXT:    call void @use1(i1 [[NO_UNDERFLOW]])
; CHECK-NEXT:    [[R:%.*]] = and i1 [[NOT_NULL]], [[NO_UNDERFLOW]]
; CHECK-NEXT:    ret i1 [[R]]
;
  %adjusted = add i8 %base, %offset
  call void @use8(i8 %adjusted)
  %not_null = icmp ne i8 %adjusted, 0
  %no_underflow = icmp ule i8 %adjusted, %base
  call void @use1(i1 %no_underflow)
  %r = and i1 %not_null, %no_underflow
  ret i1 %r
}
define i1 @n3_oneuse2_bad(i8 %base, i8 %offset) {
; CHECK-LABEL: @n3_oneuse2_bad(
; CHECK-NEXT:    [[ADJUSTED:%.*]] = add i8 [[BASE:%.*]], [[OFFSET:%.*]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[NOT_NULL:%.*]] = icmp ne i8 [[ADJUSTED]], 0
; CHECK-NEXT:    call void @use1(i1 [[NOT_NULL]])
; CHECK-NEXT:    [[NO_UNDERFLOW:%.*]] = icmp ule i8 [[ADJUSTED]], [[BASE]]
; CHECK-NEXT:    call void @use1(i1 [[NO_UNDERFLOW]])
; CHECK-NEXT:    [[R:%.*]] = and i1 [[NOT_NULL]], [[NO_UNDERFLOW]]
; CHECK-NEXT:    ret i1 [[R]]
;
  %adjusted = add i8 %base, %offset
  call void @use8(i8 %adjusted)
  %not_null = icmp ne i8 %adjusted, 0
  call void @use1(i1 %not_null)
  %no_underflow = icmp ule i8 %adjusted, %base
  call void @use1(i1 %no_underflow)
  %r = and i1 %not_null, %no_underflow
  ret i1 %r
}

define i1 @t4_commutativity0(i8 %base, i8 %offset) {
; CHECK-LABEL: @t4_commutativity0(
; CHECK-NEXT:    [[ADJUSTED:%.*]] = add i8 [[BASE:%.*]], [[OFFSET:%.*]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[NOT_NULL:%.*]] = icmp ne i8 [[ADJUSTED]], 0
; CHECK-NEXT:    [[NO_UNDERFLOW:%.*]] = icmp ule i8 [[ADJUSTED]], [[BASE]]
; CHECK-NEXT:    [[R:%.*]] = and i1 [[NO_UNDERFLOW]], [[NOT_NULL]]
; CHECK-NEXT:    ret i1 [[R]]
;
  %adjusted = add i8 %base, %offset
  call void @use8(i8 %adjusted)
  %not_null = icmp ne i8 %adjusted, 0
  %no_underflow = icmp ule i8 %adjusted, %base
  %r = and i1 %no_underflow, %not_null ; swapped
  ret i1 %r
}
define i1 @t5_commutativity1(i8 %base, i8 %offset) {
; CHECK-LABEL: @t5_commutativity1(
; CHECK-NEXT:    [[ADJUSTED:%.*]] = add i8 [[BASE:%.*]], [[OFFSET:%.*]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[NOT_NULL:%.*]] = icmp ne i8 [[ADJUSTED]], 0
; CHECK-NEXT:    [[NO_UNDERFLOW:%.*]] = icmp ule i8 [[ADJUSTED]], [[BASE]]
; CHECK-NEXT:    [[R:%.*]] = and i1 [[NOT_NULL]], [[NO_UNDERFLOW]]
; CHECK-NEXT:    ret i1 [[R]]
;
  %adjusted = add i8 %base, %offset
  call void @use8(i8 %adjusted)
  %not_null = icmp ne i8 %adjusted, 0
  %no_underflow = icmp uge i8 %base, %adjusted ; swapped
  %r = and i1 %not_null, %no_underflow
  ret i1 %r
}
define i1 @t6_commutativity3(i8 %base, i8 %offset) {
; CHECK-LABEL: @t6_commutativity3(
; CHECK-NEXT:    [[ADJUSTED:%.*]] = add i8 [[BASE:%.*]], [[OFFSET:%.*]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[NOT_NULL:%.*]] = icmp ne i8 [[ADJUSTED]], 0
; CHECK-NEXT:    [[NO_UNDERFLOW:%.*]] = icmp ule i8 [[ADJUSTED]], [[BASE]]
; CHECK-NEXT:    [[R:%.*]] = and i1 [[NO_UNDERFLOW]], [[NOT_NULL]]
; CHECK-NEXT:    ret i1 [[R]]
;
  %adjusted = add i8 %base, %offset
  call void @use8(i8 %adjusted)
  %not_null = icmp ne i8 %adjusted, 0
  %no_underflow = icmp uge i8 %base, %adjusted ; swapped
  %r = and i1 %no_underflow, %not_null ; swapped
  ret i1 %r
}

; We could have the opposite question, did we get null or overflow happened?
define i1 @t7(i8 %base, i8 %offset) {
; CHECK-LABEL: @t7(
; CHECK-NEXT:    [[ADJUSTED:%.*]] = add i8 [[BASE:%.*]], [[OFFSET:%.*]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[TMP1:%.*]] = add i8 [[ADJUSTED]], -1
; CHECK-NEXT:    [[TMP2:%.*]] = icmp uge i8 [[TMP1]], [[BASE]]
; CHECK-NEXT:    ret i1 [[TMP2]]
;
  %adjusted = add i8 %base, %offset
  call void @use8(i8 %adjusted)
  %not_null = icmp eq i8 %adjusted, 0
  %no_underflow = icmp ugt i8 %adjusted, %base
  %r = or i1 %not_null, %no_underflow
  ret i1 %r
}

; The comparison can be with any of the values being added.
define i1 @t8(i8 %base, i8 %offset) {
; CHECK-LABEL: @t8(
; CHECK-NEXT:    [[ADJUSTED:%.*]] = add i8 [[BASE:%.*]], [[OFFSET:%.*]]
; CHECK-NEXT:    call void @use8(i8 [[ADJUSTED]])
; CHECK-NEXT:    [[NOT_NULL:%.*]] = icmp ne i8 [[ADJUSTED]], 0
; CHECK-NEXT:    [[NO_UNDERFLOW:%.*]] = icmp ule i8 [[ADJUSTED]], [[OFFSET]]
; CHECK-NEXT:    [[R:%.*]] = and i1 [[NOT_NULL]], [[NO_UNDERFLOW]]
; CHECK-NEXT:    ret i1 [[R]]
;
  %adjusted = add i8 %base, %offset
  call void @use8(i8 %adjusted)
  %not_null = icmp ne i8 %adjusted, 0
  %no_underflow = icmp ule i8 %adjusted, %offset
  %r = and i1 %not_null, %no_underflow
  ret i1 %r
}
