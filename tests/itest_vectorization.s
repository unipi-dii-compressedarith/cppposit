	.text
	.file	"itest_vectorization.cpp"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               // -- Begin function main
.LCPI0_0:
	.hword	0                       // 0x0
	.hword	1638                    // 0x666
	.hword	3276                    // 0xccc
	.hword	4915                    // 0x1333
	.text
	.globl	main
	.p2align	2
	.type	main,@function
main:                                   // @main
.Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception0
// %bb.0:
	sub	sp, sp, #80             // =80
	stp	x20, x19, [sp, #48]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #64]     // 16-byte Folded Spill
	add	x29, sp, #64            // =64
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	orr	w0, wzr, #0x18
	bl	_Znwm
	adrp	x8, _ZTVN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE+16
	adrp	x9, _ZL11svptrue_b16v
	adrp	x10, _ZL13svwhilelt_b16ii
	mov	x20, x0
	add	x8, x8, :lo12:_ZTVN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE+16
	add	x9, x9, :lo12:_ZL11svptrue_b16v
	add	x10, x10, :lo12:_ZL13svwhilelt_b16ii
	stp	x8, x9, [x0]
	str	x10, [x0, #16]
.Ltmp0:
	mov	w0, #10
	bl	_Znwm
.Ltmp1:
// %bb.1:
	adrp	x8, .LCPI0_0
	ldr	d0, [x8, :lo12:.LCPI0_0]
	mov	x19, x0
	orr	w9, wzr, #0x4000
	stp	xzr, xzr, [sp, #24]
	str	xzr, [sp, #40]
	stp	xzr, xzr, [sp, #8]
	strh	w9, [x0, #8]
	str	d0, [x0]
	str	xzr, [sp]
.Ltmp3:
	mov	w0, #10
	bl	_Znwm
.Ltmp4:
// %bb.2:
	ldr	x8, [x19]
	ldrh	w9, [x19, #8]
	add	x10, x0, #10            // =10
	str	x10, [sp, #16]
	str	x8, [x0]
	strh	w9, [x0, #8]
	stp	x0, x10, [sp]
.Ltmp6:
	mov	x1, sp
	add	x2, sp, #24             // =24
	mov	x0, x20
	bl	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE12vFastSigmoidESt6vectorIS4_SaIS4_EERS8_
.Ltmp7:
// %bb.3:
	ldr	x0, [sp]
	cbz	x0, .LBB0_5
// %bb.4:
	bl	_ZdlPv
.LBB0_5:
	ldr	x0, [sp, #24]
	cbz	x0, .LBB0_7
// %bb.6:
	bl	_ZdlPv
.LBB0_7:
	mov	x0, x19
	bl	_ZdlPv
	ldp	x29, x30, [sp, #64]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #48]     // 16-byte Folded Reload
	mov	w0, wzr
	add	sp, sp, #80             // =80
	ret
.LBB0_8:
.Ltmp8:
	ldr	x8, [sp]
	mov	x20, x0
	cbnz	x8, .LBB0_11
// %bb.9:
	ldr	x0, [sp, #24]
	cbnz	x0, .LBB0_13
.LBB0_10:
	mov	x0, x19
	bl	_ZdlPv
	mov	x0, x20
	bl	_Unwind_Resume
.LBB0_11:
	mov	x0, x8
	bl	_ZdlPv
	ldr	x0, [sp, #24]
	cbz	x0, .LBB0_10
	b	.LBB0_13
.LBB0_12:
.Ltmp5:
	mov	x20, x0
	ldr	x0, [sp, #24]
	cbz	x0, .LBB0_10
.LBB0_13:
	bl	_ZdlPv
	mov	x0, x19
	bl	_ZdlPv
	mov	x0, x20
	bl	_Unwind_Resume
.LBB0_14:
.Ltmp2:
	bl	_Unwind_Resume
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table0:
.Lexception0:
	.byte	255                     // @LPStart Encoding = omit
	.byte	255                     // @TType Encoding = omit
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end0-.Lcst_begin0
.Lcst_begin0:
	.uleb128 .Lfunc_begin0-.Lfunc_begin0 // >> Call Site 1 <<
	.uleb128 .Ltmp0-.Lfunc_begin0   //   Call between .Lfunc_begin0 and .Ltmp0
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp0-.Lfunc_begin0   // >> Call Site 2 <<
	.uleb128 .Ltmp1-.Ltmp0          //   Call between .Ltmp0 and .Ltmp1
	.uleb128 .Ltmp2-.Lfunc_begin0   //     jumps to .Ltmp2
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp3-.Lfunc_begin0   // >> Call Site 3 <<
	.uleb128 .Ltmp4-.Ltmp3          //   Call between .Ltmp3 and .Ltmp4
	.uleb128 .Ltmp5-.Lfunc_begin0   //     jumps to .Ltmp5
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp6-.Lfunc_begin0   // >> Call Site 4 <<
	.uleb128 .Ltmp7-.Ltmp6          //   Call between .Ltmp6 and .Ltmp7
	.uleb128 .Ltmp8-.Lfunc_begin0   //     jumps to .Ltmp8
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp7-.Lfunc_begin0   // >> Call Site 5 <<
	.uleb128 .Lfunc_end0-.Ltmp7     //   Call between .Ltmp7 and .Lfunc_end0
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end0:
	.p2align	2
                                        // -- End function
	.text
	.p2align	2               // -- Begin function _ZL11svptrue_b16v
	.type	_ZL11svptrue_b16v,@function
_ZL11svptrue_b16v:                      // @_ZL11svptrue_b16v
	.cfi_startproc
// %bb.0:
	ptrue	p0.h
	ret
.Lfunc_end1:
	.size	_ZL11svptrue_b16v, .Lfunc_end1-_ZL11svptrue_b16v
	.cfi_endproc
                                        // -- End function
	.p2align	2               // -- Begin function _ZL13svwhilelt_b16ii
	.type	_ZL13svwhilelt_b16ii,@function
_ZL13svwhilelt_b16ii:                   // @_ZL13svwhilelt_b16ii
	.cfi_startproc
// %bb.0:
	whilelt	p0.h, w0, w1
	ret
.Lfunc_end2:
	.size	_ZL13svwhilelt_b16ii, .Lfunc_end2-_ZL13svwhilelt_b16ii
	.cfi_endproc
                                        // -- End function
	.section	.text._ZN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED2Ev,"axG",@progbits,_ZN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED2Ev,comdat
	.weak	_ZN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED2Ev // -- Begin function _ZN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED2Ev
	.p2align	2
	.type	_ZN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED2Ev,@function
_ZN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED2Ev: // @_ZN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED2Ev
	.cfi_startproc
// %bb.0:
	ret
.Lfunc_end3:
	.size	_ZN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED2Ev, .Lfunc_end3-_ZN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED2Ev
	.cfi_endproc
                                        // -- End function
	.section	.text._ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED0Ev,"axG",@progbits,_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED0Ev,comdat
	.weak	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED0Ev // -- Begin function _ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED0Ev
	.p2align	2
	.type	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED0Ev,@function
_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED0Ev: // @_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED0Ev
	.cfi_startproc
// %bb.0:
	b	_ZdlPv
.Lfunc_end4:
	.size	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED0Ev, .Lfunc_end4-_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED0Ev
	.cfi_endproc
                                        // -- End function
	.section	.text._ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vAddESt6vectorIS4_SaIS4_EES8_RS8_,"axG",@progbits,_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vAddESt6vectorIS4_SaIS4_EES8_RS8_,comdat
	.weak	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vAddESt6vectorIS4_SaIS4_EES8_RS8_ // -- Begin function _ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vAddESt6vectorIS4_SaIS4_EES8_RS8_
	.p2align	2
	.type	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vAddESt6vectorIS4_SaIS4_EES8_RS8_,@function
_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vAddESt6vectorIS4_SaIS4_EES8_RS8_: // @_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vAddESt6vectorIS4_SaIS4_EES8_RS8_
	.cfi_startproc
// %bb.0:
	ret
.Lfunc_end5:
	.size	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vAddESt6vectorIS4_SaIS4_EES8_RS8_, .Lfunc_end5-_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vAddESt6vectorIS4_SaIS4_EES8_RS8_
	.cfi_endproc
                                        // -- End function
	.section	.text._ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vMulESt6vectorIS4_SaIS4_EES8_RS8_,"axG",@progbits,_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vMulESt6vectorIS4_SaIS4_EES8_RS8_,comdat
	.weak	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vMulESt6vectorIS4_SaIS4_EES8_RS8_ // -- Begin function _ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vMulESt6vectorIS4_SaIS4_EES8_RS8_
	.p2align	2
	.type	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vMulESt6vectorIS4_SaIS4_EES8_RS8_,@function
_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vMulESt6vectorIS4_SaIS4_EES8_RS8_: // @_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vMulESt6vectorIS4_SaIS4_EES8_RS8_
	.cfi_startproc
// %bb.0:
	ret
.Lfunc_end6:
	.size	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vMulESt6vectorIS4_SaIS4_EES8_RS8_, .Lfunc_end6-_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vMulESt6vectorIS4_SaIS4_EES8_RS8_
	.cfi_endproc
                                        // -- End function
	.section	.text._ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE7vMuladdESt6vectorIS4_SaIS4_EES8_RS8_,"axG",@progbits,_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE7vMuladdESt6vectorIS4_SaIS4_EES8_RS8_,comdat
	.weak	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE7vMuladdESt6vectorIS4_SaIS4_EES8_RS8_ // -- Begin function _ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE7vMuladdESt6vectorIS4_SaIS4_EES8_RS8_
	.p2align	2
	.type	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE7vMuladdESt6vectorIS4_SaIS4_EES8_RS8_,@function
_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE7vMuladdESt6vectorIS4_SaIS4_EES8_RS8_: // @_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE7vMuladdESt6vectorIS4_SaIS4_EES8_RS8_
	.cfi_startproc
// %bb.0:
	ret
.Lfunc_end7:
	.size	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE7vMuladdESt6vectorIS4_SaIS4_EES8_RS8_, .Lfunc_end7-_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE7vMuladdESt6vectorIS4_SaIS4_EES8_RS8_
	.cfi_endproc
                                        // -- End function
	.section	.text._ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vDotESt6vectorIS4_SaIS4_EES8_,"axG",@progbits,_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vDotESt6vectorIS4_SaIS4_EES8_,comdat
	.weak	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vDotESt6vectorIS4_SaIS4_EES8_ // -- Begin function _ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vDotESt6vectorIS4_SaIS4_EES8_
	.p2align	2
	.type	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vDotESt6vectorIS4_SaIS4_EES8_,@function
_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vDotESt6vectorIS4_SaIS4_EES8_: // @_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vDotESt6vectorIS4_SaIS4_EES8_
	.cfi_startproc
// %bb.0:
	mov	x0, xzr
	ret
.Lfunc_end8:
	.size	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vDotESt6vectorIS4_SaIS4_EES8_, .Lfunc_end8-_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vDotESt6vectorIS4_SaIS4_EES8_
	.cfi_endproc
                                        // -- End function
	.section	.text._ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE10vFastTwiceESt6vectorIS4_SaIS4_EERS8_,"axG",@progbits,_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE10vFastTwiceESt6vectorIS4_SaIS4_EERS8_,comdat
	.weak	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE10vFastTwiceESt6vectorIS4_SaIS4_EERS8_ // -- Begin function _ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE10vFastTwiceESt6vectorIS4_SaIS4_EERS8_
	.p2align	2
	.type	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE10vFastTwiceESt6vectorIS4_SaIS4_EERS8_,@function
_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE10vFastTwiceESt6vectorIS4_SaIS4_EERS8_: // @_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE10vFastTwiceESt6vectorIS4_SaIS4_EERS8_
.Lfunc_begin1:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception1
// %bb.0:
	str	x23, [sp, #-64]!        // 8-byte Folded Spill
	stp	x22, x21, [sp, #16]     // 16-byte Folded Spill
	stp	x20, x19, [sp, #32]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #48]     // 16-byte Folded Spill
	add	x29, sp, #48            // =48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -64
	ldp	x8, x10, [x1]
	subs	x0, x10, x8
	b.eq	.LBB9_7
// %bb.1:
	tbnz	x0, #63, .LBB9_29
// %bb.2:
	mov	x19, x1
	bl	_Znwm
	ldp	x8, x10, [x19]
	mov	x19, x0
	subs	x9, x10, x8
	b.eq	.LBB9_8
.LBB9_3:
	sub	x9, x9, #2              // =2
	lsr	x9, x9, #1
	add	x9, x9, #1              // =1
	lsl	x11, x9, #1
	add	x12, x8, x11
	cmp	x19, x12
	add	x22, x19, x11
	b.hs	.LBB9_9
// %bb.4:
	cmp	x8, x22
	b.hs	.LBB9_9
// %bb.5:
	mov	x9, x19
.LBB9_6:                                // =>This Inner Loop Header: Depth=1
	ldrh	w11, [x8], #2
	cmp	x10, x8
	strh	w11, [x9], #2
	b.ne	.LBB9_6
	b	.LBB9_11
.LBB9_7:
	mov	x19, xzr
	subs	x9, x10, x8
	b.ne	.LBB9_3
.LBB9_8:
	mov	x22, x19
	subs	x21, x22, x19
	b.ne	.LBB9_12
	b	.LBB9_16
.LBB9_9:
	mov	x10, xzr
	whilelo	p0.h, xzr, x9
.LBB9_10:                               // =>This Inner Loop Header: Depth=1
	mov	x11, x10
	incd	x11, all, mul #3
	index	z0.d, x11, #1
	mov	x11, x10
	incw	x11
	pfalse	p4.b
	index	z1.d, x11, #1
	mov	x11, x10
	zip2	p2.h, p0.h, p4.h
	zip1	p0.h, p0.h, p4.h
	incd	x11
	index	z2.d, x10, #1
	zip2	p1.s, p2.s, p4.s
	zip1	p2.s, p2.s, p4.s
	zip2	p3.s, p0.s, p4.s
	index	z3.d, x11, #1
	zip1	p4.s, p0.s, p4.s
	ld1sh	{ z4.d }, p1/z, [x8, z0.d, lsl #1]
	ld1sh	{ z5.d }, p2/z, [x8, z1.d, lsl #1]
	ld1sh	{ z6.d }, p3/z, [x8, z3.d, lsl #1]
	ld1sh	{ z7.d }, p4/z, [x8, z2.d, lsl #1]
	inch	x10
	uzp1	z4.s, z5.s, z4.s
	whilelo	p0.h, x10, x9
	uzp1	z5.s, z7.s, z6.s
	uzp1	z4.h, z5.h, z4.h
	uunpklo	z5.s, z4.h
	uunpkhi	z4.s, z4.h
	uunpklo	z6.d, z5.s
	uunpkhi	z5.d, z5.s
	st1h	{ z6.d }, p4, [x19, z2.d, lsl #1]
	uunpklo	z2.d, z4.s
	uunpkhi	z4.d, z4.s
	st1h	{ z5.d }, p3, [x19, z3.d, lsl #1]
	st1h	{ z2.d }, p2, [x19, z1.d, lsl #1]
	st1h	{ z4.d }, p1, [x19, z0.d, lsl #1]
	b.mi	.LBB9_10
.LBB9_11:
	subs	x21, x22, x19
	b.eq	.LBB9_16
.LBB9_12:
	tbnz	x21, #63, .LBB9_30
// %bb.13:
.Ltmp9:
	mov	x0, x21
	bl	_Znwm
.Ltmp10:
// %bb.14:
	rdvl	x8, #1
	mov	x20, x0
	cmp	x8, #17                 // =17
	b.hs	.LBB9_20
// %bb.15:
	mov	x0, x20
	mov	w1, wzr
	mov	x2, x21
	bl	memset
	mov	x23, x20
	cmp	x22, x19
	b.ne	.LBB9_23
	b	.LBB9_17
.LBB9_16:
	mov	x20, xzr
	mov	x23, xzr
	cmp	x22, x19
	b.ne	.LBB9_23
.LBB9_17:
	cbnz	x19, .LBB9_27
// %bb.18:
	cbz	x23, .LBB9_28
.LBB9_19:
	mov	x0, x20
	ldp	x29, x30, [sp, #48]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]     // 16-byte Folded Reload
	ldr	x23, [sp], #64          // 8-byte Folded Reload
	b	_ZdlPv
.LBB9_20:
	mov	x8, xzr
	whilelo	p0.b, xzr, x21
	mov	z0.b, #0                // =0x0
.LBB9_21:                               // =>This Inner Loop Header: Depth=1
	st1b	{ z0.b }, p0, [x20, x8]
	incp	x8, p0.b
	whilelo	p0.b, x8, x21
	b.mi	.LBB9_21
// %bb.22:
	mov	x23, x20
	cmp	x22, x19
	b.eq	.LBB9_17
.LBB9_23:
	asr	x8, x21, #1
	cmp	x8, #1                  // =1
	rdvl	x9, #1
	csinc	x8, x8, xzr, hi
	cmp	x9, #17                 // =17
	lsl	x2, x8, #1
	b.hs	.LBB9_25
// %bb.24:
	mov	x0, x20
	mov	x1, x19
	bl	memcpy
	mov	x0, x19
	bl	_ZdlPv
	cbnz	x23, .LBB9_19
	b	.LBB9_28
.LBB9_25:
	mov	x8, xzr
	whilelo	p0.b, xzr, x2
.LBB9_26:                               // =>This Inner Loop Header: Depth=1
	ld1b	{ z0.b }, p0/z, [x19, x8]
	st1b	{ z0.b }, p0, [x20, x8]
	incp	x8, p0.b
	whilelo	p0.b, x8, x2
	b.mi	.LBB9_26
.LBB9_27:
	mov	x0, x19
	bl	_ZdlPv
	cbnz	x23, .LBB9_19
.LBB9_28:
	ldp	x29, x30, [sp, #48]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]     // 16-byte Folded Reload
	ldr	x23, [sp], #64          // 8-byte Folded Reload
	ret
.LBB9_29:
	bl	_ZSt17__throw_bad_allocv
.LBB9_30:
.Ltmp11:
	bl	_ZSt17__throw_bad_allocv
.Ltmp12:
// %bb.31:
.LBB9_32:
.Ltmp13:
	mov	x20, x0
	cbz	x19, .LBB9_34
// %bb.33:
	mov	x0, x19
	bl	_ZdlPv
.LBB9_34:
	mov	x0, x20
	bl	_Unwind_Resume
.Lfunc_end9:
	.size	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE10vFastTwiceESt6vectorIS4_SaIS4_EERS8_, .Lfunc_end9-_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE10vFastTwiceESt6vectorIS4_SaIS4_EERS8_
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table9:
.Lexception1:
	.byte	255                     // @LPStart Encoding = omit
	.byte	255                     // @TType Encoding = omit
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end1-.Lcst_begin1
.Lcst_begin1:
	.uleb128 .Lfunc_begin1-.Lfunc_begin1 // >> Call Site 1 <<
	.uleb128 .Ltmp9-.Lfunc_begin1   //   Call between .Lfunc_begin1 and .Ltmp9
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp9-.Lfunc_begin1   // >> Call Site 2 <<
	.uleb128 .Ltmp10-.Ltmp9         //   Call between .Ltmp9 and .Ltmp10
	.uleb128 .Ltmp13-.Lfunc_begin1  //     jumps to .Ltmp13
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp10-.Lfunc_begin1  // >> Call Site 3 <<
	.uleb128 .Ltmp11-.Ltmp10        //   Call between .Ltmp10 and .Ltmp11
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp11-.Lfunc_begin1  // >> Call Site 4 <<
	.uleb128 .Ltmp12-.Ltmp11        //   Call between .Ltmp11 and .Ltmp12
	.uleb128 .Ltmp13-.Lfunc_begin1  //     jumps to .Ltmp13
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp12-.Lfunc_begin1  // >> Call Site 5 <<
	.uleb128 .Lfunc_end9-.Ltmp12    //   Call between .Ltmp12 and .Lfunc_end9
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end1:
	.p2align	2
                                        // -- End function
	.section	.text._ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastInvESt6vectorIS4_SaIS4_EERS8_,"axG",@progbits,_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastInvESt6vectorIS4_SaIS4_EERS8_,comdat
	.weak	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastInvESt6vectorIS4_SaIS4_EERS8_ // -- Begin function _ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastInvESt6vectorIS4_SaIS4_EERS8_
	.p2align	2
	.type	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastInvESt6vectorIS4_SaIS4_EERS8_,@function
_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastInvESt6vectorIS4_SaIS4_EERS8_: // @_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastInvESt6vectorIS4_SaIS4_EERS8_
	.cfi_startproc
// %bb.0:
	ret
.Lfunc_end10:
	.size	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastInvESt6vectorIS4_SaIS4_EERS8_, .Lfunc_end10-_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastInvESt6vectorIS4_SaIS4_EERS8_
	.cfi_endproc
                                        // -- End function
	.section	.text._ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastELUESt6vectorIS4_SaIS4_EERS8_,"axG",@progbits,_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastELUESt6vectorIS4_SaIS4_EERS8_,comdat
	.weak	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastELUESt6vectorIS4_SaIS4_EERS8_ // -- Begin function _ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastELUESt6vectorIS4_SaIS4_EERS8_
	.p2align	2
	.type	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastELUESt6vectorIS4_SaIS4_EERS8_,@function
_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastELUESt6vectorIS4_SaIS4_EERS8_: // @_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastELUESt6vectorIS4_SaIS4_EERS8_
	.cfi_startproc
// %bb.0:
	ret
.Lfunc_end11:
	.size	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastELUESt6vectorIS4_SaIS4_EERS8_, .Lfunc_end11-_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastELUESt6vectorIS4_SaIS4_EERS8_
	.cfi_endproc
                                        // -- End function
	.section	.text._ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE12vFastSigmoidESt6vectorIS4_SaIS4_EERS8_,"axG",@progbits,_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE12vFastSigmoidESt6vectorIS4_SaIS4_EERS8_,comdat
	.weak	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE12vFastSigmoidESt6vectorIS4_SaIS4_EERS8_ // -- Begin function _ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE12vFastSigmoidESt6vectorIS4_SaIS4_EERS8_
	.p2align	2
	.type	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE12vFastSigmoidESt6vectorIS4_SaIS4_EERS8_,@function
_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE12vFastSigmoidESt6vectorIS4_SaIS4_EERS8_: // @_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE12vFastSigmoidESt6vectorIS4_SaIS4_EERS8_
.Lfunc_begin2:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception2
// %bb.0:
	stp	x28, x27, [sp, #-96]!   // 16-byte Folded Spill
	stp	x26, x25, [sp, #16]     // 16-byte Folded Spill
	stp	x24, x23, [sp, #32]     // 16-byte Folded Spill
	stp	x22, x21, [sp, #48]     // 16-byte Folded Spill
	stp	x20, x19, [sp, #64]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #80]     // 16-byte Folded Spill
	add	x29, sp, #80            // =80
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	.cfi_offset w27, -88
	.cfi_offset w28, -96
	ldp	x9, x10, [x1]
	mov	x19, x2
	mov	x22, x1
	mov	x23, x0
	subs	x8, x10, x9
	b.eq	.LBB12_7
// %bb.1:
	tbnz	x8, #63, .LBB12_55
// %bb.2:
	mov	x0, x8
	bl	_Znwm
	ldp	x9, x10, [x22]
	mov	x24, x0
	subs	x8, x10, x9
	b.eq	.LBB12_8
.LBB12_3:
	sub	x8, x8, #2              // =2
	lsr	x8, x8, #1
	add	x8, x8, #1              // =1
	lsl	x11, x8, #1
	add	x12, x9, x11
	cmp	x24, x12
	add	x25, x24, x11
	b.hs	.LBB12_9
// %bb.4:
	cmp	x9, x25
	b.hs	.LBB12_9
// %bb.5:
	mov	x8, x24
.LBB12_6:                               // =>This Inner Loop Header: Depth=1
	ldrh	w11, [x9], #2
	cmp	x10, x9
	strh	w11, [x8], #2
	b.ne	.LBB12_6
	b	.LBB12_11
.LBB12_7:
	mov	x24, xzr
	subs	x8, x10, x9
	b.ne	.LBB12_3
.LBB12_8:
	mov	x25, x24
	subs	x21, x25, x24
	asr	x27, x21, #1
	b.ne	.LBB12_12
	b	.LBB12_16
.LBB12_9:
	mov	x10, xzr
	whilelo	p0.h, xzr, x8
.LBB12_10:                              // =>This Inner Loop Header: Depth=1
	mov	x11, x10
	incd	x11, all, mul #3
	index	z0.d, x11, #1
	mov	x11, x10
	incw	x11
	pfalse	p4.b
	index	z1.d, x11, #1
	mov	x11, x10
	zip2	p2.h, p0.h, p4.h
	zip1	p0.h, p0.h, p4.h
	incd	x11
	index	z2.d, x10, #1
	zip2	p1.s, p2.s, p4.s
	zip1	p2.s, p2.s, p4.s
	zip2	p3.s, p0.s, p4.s
	index	z3.d, x11, #1
	zip1	p4.s, p0.s, p4.s
	ld1sh	{ z4.d }, p1/z, [x9, z0.d, lsl #1]
	ld1sh	{ z5.d }, p2/z, [x9, z1.d, lsl #1]
	ld1sh	{ z6.d }, p3/z, [x9, z3.d, lsl #1]
	ld1sh	{ z7.d }, p4/z, [x9, z2.d, lsl #1]
	inch	x10
	uzp1	z4.s, z5.s, z4.s
	whilelo	p0.h, x10, x8
	uzp1	z5.s, z7.s, z6.s
	uzp1	z4.h, z5.h, z4.h
	uunpklo	z5.s, z4.h
	uunpkhi	z4.s, z4.h
	uunpklo	z6.d, z5.s
	uunpkhi	z5.d, z5.s
	st1h	{ z6.d }, p4, [x24, z2.d, lsl #1]
	uunpklo	z2.d, z4.s
	uunpkhi	z4.d, z4.s
	st1h	{ z5.d }, p3, [x24, z3.d, lsl #1]
	st1h	{ z2.d }, p2, [x24, z1.d, lsl #1]
	st1h	{ z4.d }, p1, [x24, z0.d, lsl #1]
	b.mi	.LBB12_10
.LBB12_11:
	subs	x21, x25, x24
	asr	x27, x21, #1
	b.eq	.LBB12_16
.LBB12_12:
	tbnz	x21, #63, .LBB12_56
// %bb.13:
.Ltmp14:
	mov	x0, x21
	bl	_Znwm
.Ltmp15:
// %bb.14:
	rdvl	x8, #1
	mov	x20, x0
	cmp	x8, #17                 // =17
	add	x26, x0, x27, lsl #1
	b.hs	.LBB12_18
// %bb.15:
	mov	x0, x20
	mov	w1, wzr
	mov	x2, x21
	bl	memset
	mov	x21, x20
	cmp	x25, x24
	b.ne	.LBB12_21
	b	.LBB12_17
.LBB12_16:
	mov	x20, xzr
	mov	x26, xzr
	cmp	x25, x24
	b.ne	.LBB12_21
.LBB12_17:
	cbnz	x24, .LBB12_25
	b	.LBB12_26
.LBB12_18:
	mov	x8, xzr
	whilelo	p0.b, xzr, x21
	mov	z0.b, #0                // =0x0
.LBB12_19:                              // =>This Inner Loop Header: Depth=1
	st1b	{ z0.b }, p0, [x20, x8]
	incp	x8, p0.b
	whilelo	p0.b, x8, x21
	b.mi	.LBB12_19
// %bb.20:
	mov	x21, x20
	cmp	x25, x24
	b.eq	.LBB12_17
.LBB12_21:
	cmp	x27, #1                 // =1
	rdvl	x8, #1
	csinc	x9, x27, xzr, hi
	cmp	x8, #17                 // =17
	lsl	x2, x9, #1
	b.hs	.LBB12_23
// %bb.22:
	mov	x0, x20
	mov	x1, x24
	bl	memcpy
	b	.LBB12_25
.LBB12_23:
	mov	x8, xzr
	whilelo	p0.b, xzr, x2
.LBB12_24:                              // =>This Inner Loop Header: Depth=1
	ld1b	{ z0.b }, p0/z, [x24, x8]
	st1b	{ z0.b }, p0, [x20, x8]
	incp	x8, p0.b
	whilelo	p0.b, x8, x2
	b.mi	.LBB12_24
.LBB12_25:
	mov	x0, x24
	bl	_ZdlPv
.LBB12_26:
	ldp	x24, x22, [x22]
	ldr	x8, [x23, #8]
.Ltmp19:
	blr	x8
.Ltmp20:
// %bb.27:
	sub	x8, x22, x24
	lsr	x8, x8, #1
	whilelt	p1.s, wzr, w8
	ptest	p0, p1.b
	cset	w9, mi
	cmp	w9, #1                  // =1
	b.ne	.LBB12_30
// %bb.28:
	mov	w9, wzr
	mov	z0.h, #16384            // =0x4000
.LBB12_29:                              // =>This Inner Loop Header: Depth=1
	sxtw	x10, w9
	ld1h	{ z1.h }, p1/z, [x21, x10, lsl #1]
	incw	x9
	asr	z1.h, p1/m, z1.h, #1
	movprfx	z1.h, p1/z, z1.h
	add	z1.h, p1/m, z1.h, z0.h
	asr	z1.h, p1/m, z1.h, #1
	st1h	{ z1.h }, p1, [x21, x10, lsl #1]
	whilelt	p1.s, w9, w8
	ptest	p0, p1.b
	cset	w10, mi
	tbnz	w10, #0, .LBB12_29
.LBB12_30:
	subs	x24, x26, x21
	b.eq	.LBB12_34
// %bb.31:
	tbnz	x24, #63, .LBB12_58
// %bb.32:
.Ltmp22:
	mov	x0, x24
	bl	_Znwm
.Ltmp23:
// %bb.33:
	mov	x1, x21
	mov	x2, x24
	mov	x22, x0
	bl	memmove
	mov	x23, x22
	b	.LBB12_35
.LBB12_34:
	mov	x22, xzr
	mov	x23, xzr
.LBB12_35:
	asr	x8, x24, #1
	add	x28, x22, x8, lsl #1
	subs	x25, x28, x22
	asr	x27, x25, #1
	b.eq	.LBB12_40
// %bb.36:
	tbnz	x25, #63, .LBB12_60
// %bb.37:
.Ltmp27:
	mov	x0, x25
	bl	_Znwm
.Ltmp28:
// %bb.38:
	rdvl	x8, #1
	mov	x24, x0
	cmp	x8, #17                 // =17
	add	x26, x0, x27, lsl #1
	b.hs	.LBB12_41
// %bb.39:
	mov	x0, x24
	mov	w1, wzr
	mov	x2, x25
	bl	memset
	mov	x25, x26
	cmp	x22, x28
	b.ne	.LBB12_44
	b	.LBB12_48
.LBB12_40:
	mov	x24, xzr
	lsl	x26, x27, #1
	cmp	x22, x28
	b.ne	.LBB12_44
	b	.LBB12_48
.LBB12_41:
	mov	x8, xzr
	whilelo	p0.b, xzr, x25
	mov	z0.b, #0                // =0x0
.LBB12_42:                              // =>This Inner Loop Header: Depth=1
	st1b	{ z0.b }, p0, [x24, x8]
	incp	x8, p0.b
	whilelo	p0.b, x8, x25
	b.mi	.LBB12_42
// %bb.43:
	mov	x25, x26
	cmp	x22, x28
	b.eq	.LBB12_48
.LBB12_44:
	cmp	x27, #1                 // =1
	rdvl	x8, #1
	csinc	x9, x27, xzr, hi
	cmp	x8, #17                 // =17
	lsl	x2, x9, #1
	b.hs	.LBB12_46
// %bb.45:
	mov	x0, x24
	mov	x1, x23
	bl	memcpy
	ldr	x0, [x19]
	stp	x24, x25, [x19]
	str	x26, [x19, #16]
	cbnz	x0, .LBB12_49
	b	.LBB12_50
.LBB12_46:
	mov	x8, xzr
	whilelo	p0.b, xzr, x2
.LBB12_47:                              // =>This Inner Loop Header: Depth=1
	ld1b	{ z0.b }, p0/z, [x23, x8]
	st1b	{ z0.b }, p0, [x24, x8]
	incp	x8, p0.b
	whilelo	p0.b, x8, x2
	b.mi	.LBB12_47
.LBB12_48:
	ldr	x0, [x19]
	stp	x24, x25, [x19]
	str	x26, [x19, #16]
	cbz	x0, .LBB12_50
.LBB12_49:
	bl	_ZdlPv
.LBB12_50:
	cbz	x22, .LBB12_52
// %bb.51:
	mov	x0, x23
	bl	_ZdlPv
.LBB12_52:
	cbz	x21, .LBB12_54
// %bb.53:
	mov	x0, x20
	ldp	x29, x30, [sp, #80]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #64]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #48]     // 16-byte Folded Reload
	ldp	x24, x23, [sp, #32]     // 16-byte Folded Reload
	ldp	x26, x25, [sp, #16]     // 16-byte Folded Reload
	ldp	x28, x27, [sp], #96     // 16-byte Folded Reload
	b	_ZdlPv
.LBB12_54:
	ldp	x29, x30, [sp, #80]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #64]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #48]     // 16-byte Folded Reload
	ldp	x24, x23, [sp, #32]     // 16-byte Folded Reload
	ldp	x26, x25, [sp, #16]     // 16-byte Folded Reload
	ldp	x28, x27, [sp], #96     // 16-byte Folded Reload
	ret
.LBB12_55:
	bl	_ZSt17__throw_bad_allocv
.LBB12_56:
.Ltmp16:
	bl	_ZSt17__throw_bad_allocv
.Ltmp17:
// %bb.57:
.LBB12_58:
.Ltmp24:
	bl	_ZSt17__throw_bad_allocv
.Ltmp25:
// %bb.59:
.LBB12_60:
.Ltmp29:
	bl	_ZSt17__throw_bad_allocv
.Ltmp30:
// %bb.61:
.LBB12_62:
.Ltmp31:
	mov	x19, x0
	cbz	x22, .LBB12_68
// %bb.63:
	mov	x0, x23
	bl	_ZdlPv
	cbnz	x21, .LBB12_69
	b	.LBB12_71
.LBB12_64:
.Ltmp26:
	mov	x19, x0
	cbnz	x21, .LBB12_69
	b	.LBB12_71
.LBB12_65:
.Ltmp18:
	mov	x19, x0
	cbz	x24, .LBB12_71
// %bb.66:
	mov	x0, x24
	b	.LBB12_70
.LBB12_67:
.Ltmp21:
	mov	x19, x0
.LBB12_68:
	cbz	x21, .LBB12_71
.LBB12_69:
	mov	x0, x20
.LBB12_70:
	bl	_ZdlPv
.LBB12_71:
	mov	x0, x19
	bl	_Unwind_Resume
.Lfunc_end12:
	.size	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE12vFastSigmoidESt6vectorIS4_SaIS4_EERS8_, .Lfunc_end12-_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE12vFastSigmoidESt6vectorIS4_SaIS4_EERS8_
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table12:
.Lexception2:
	.byte	255                     // @LPStart Encoding = omit
	.byte	255                     // @TType Encoding = omit
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end2-.Lcst_begin2
.Lcst_begin2:
	.uleb128 .Lfunc_begin2-.Lfunc_begin2 // >> Call Site 1 <<
	.uleb128 .Ltmp14-.Lfunc_begin2  //   Call between .Lfunc_begin2 and .Ltmp14
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp14-.Lfunc_begin2  // >> Call Site 2 <<
	.uleb128 .Ltmp15-.Ltmp14        //   Call between .Ltmp14 and .Ltmp15
	.uleb128 .Ltmp18-.Lfunc_begin2  //     jumps to .Ltmp18
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp15-.Lfunc_begin2  // >> Call Site 3 <<
	.uleb128 .Ltmp19-.Ltmp15        //   Call between .Ltmp15 and .Ltmp19
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp19-.Lfunc_begin2  // >> Call Site 4 <<
	.uleb128 .Ltmp20-.Ltmp19        //   Call between .Ltmp19 and .Ltmp20
	.uleb128 .Ltmp21-.Lfunc_begin2  //     jumps to .Ltmp21
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp22-.Lfunc_begin2  // >> Call Site 5 <<
	.uleb128 .Ltmp23-.Ltmp22        //   Call between .Ltmp22 and .Ltmp23
	.uleb128 .Ltmp26-.Lfunc_begin2  //     jumps to .Ltmp26
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp23-.Lfunc_begin2  // >> Call Site 6 <<
	.uleb128 .Ltmp27-.Ltmp23        //   Call between .Ltmp23 and .Ltmp27
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp27-.Lfunc_begin2  // >> Call Site 7 <<
	.uleb128 .Ltmp28-.Ltmp27        //   Call between .Ltmp27 and .Ltmp28
	.uleb128 .Ltmp31-.Lfunc_begin2  //     jumps to .Ltmp31
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp28-.Lfunc_begin2  // >> Call Site 8 <<
	.uleb128 .Ltmp16-.Ltmp28        //   Call between .Ltmp28 and .Ltmp16
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp16-.Lfunc_begin2  // >> Call Site 9 <<
	.uleb128 .Ltmp17-.Ltmp16        //   Call between .Ltmp16 and .Ltmp17
	.uleb128 .Ltmp18-.Lfunc_begin2  //     jumps to .Ltmp18
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp24-.Lfunc_begin2  // >> Call Site 10 <<
	.uleb128 .Ltmp25-.Ltmp24        //   Call between .Ltmp24 and .Ltmp25
	.uleb128 .Ltmp26-.Lfunc_begin2  //     jumps to .Ltmp26
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp29-.Lfunc_begin2  // >> Call Site 11 <<
	.uleb128 .Ltmp30-.Ltmp29        //   Call between .Ltmp29 and .Ltmp30
	.uleb128 .Ltmp31-.Lfunc_begin2  //     jumps to .Ltmp31
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp30-.Lfunc_begin2  // >> Call Site 12 <<
	.uleb128 .Lfunc_end12-.Ltmp30   //   Call between .Ltmp30 and .Lfunc_end12
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end2:
	.p2align	2
                                        // -- End function
	.section	.text._ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE9vFastTanhESt6vectorIS4_SaIS4_EERS8_,"axG",@progbits,_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE9vFastTanhESt6vectorIS4_SaIS4_EERS8_,comdat
	.weak	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE9vFastTanhESt6vectorIS4_SaIS4_EERS8_ // -- Begin function _ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE9vFastTanhESt6vectorIS4_SaIS4_EERS8_
	.p2align	2
	.type	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE9vFastTanhESt6vectorIS4_SaIS4_EERS8_,@function
_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE9vFastTanhESt6vectorIS4_SaIS4_EERS8_: // @_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE9vFastTanhESt6vectorIS4_SaIS4_EERS8_
	.cfi_startproc
// %bb.0:
	ret
.Lfunc_end13:
	.size	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE9vFastTanhESt6vectorIS4_SaIS4_EERS8_, .Lfunc_end13-_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE9vFastTanhESt6vectorIS4_SaIS4_EERS8_
	.cfi_endproc
                                        // -- End function
	.section	.text.startup,"ax",@progbits
	.p2align	2               // -- Begin function _GLOBAL__sub_I_itest_vectorization.cpp
	.type	_GLOBAL__sub_I_itest_vectorization.cpp,@function
_GLOBAL__sub_I_itest_vectorization.cpp: // @_GLOBAL__sub_I_itest_vectorization.cpp
	.cfi_startproc
// %bb.0:
	str	x19, [sp, #-32]!        // 8-byte Folded Spill
	stp	x29, x30, [sp, #16]     // 16-byte Folded Spill
	add	x29, sp, #16            // =16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -32
	adrp	x19, _ZStL8__ioinit
	add	x19, x19, :lo12:_ZStL8__ioinit
	mov	x0, x19
	bl	_ZNSt8ios_base4InitC1Ev
	ldp	x29, x30, [sp, #16]     // 16-byte Folded Reload
	adrp	x0, _ZNSt8ios_base4InitD1Ev
	adrp	x2, __dso_handle
	add	x0, x0, :lo12:_ZNSt8ios_base4InitD1Ev
	add	x2, x2, :lo12:__dso_handle
	mov	x1, x19
	ldr	x19, [sp], #32          // 8-byte Folded Reload
	b	__cxa_atexit
.Lfunc_end14:
	.size	_GLOBAL__sub_I_itest_vectorization.cpp, .Lfunc_end14-_GLOBAL__sub_I_itest_vectorization.cpp
	.cfi_endproc
                                        // -- End function
	.type	_ZStL8__ioinit,@object  // @_ZStL8__ioinit
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.hidden	__dso_handle
	.type	_ZTVN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,@object // @_ZTVN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.section	.rodata._ZTVN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,"aG",@progbits,_ZTVN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,comdat
	.weak	_ZTVN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.p2align	3
_ZTVN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE:
	.xword	0
	.xword	_ZTIN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.xword	_ZN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED2Ev
	.xword	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEED0Ev
	.xword	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vAddESt6vectorIS4_SaIS4_EES8_RS8_
	.xword	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vMulESt6vectorIS4_SaIS4_EES8_RS8_
	.xword	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE7vMuladdESt6vectorIS4_SaIS4_EES8_RS8_
	.xword	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE4vDotESt6vectorIS4_SaIS4_EES8_
	.xword	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE10vFastTwiceESt6vectorIS4_SaIS4_EERS8_
	.xword	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastInvESt6vectorIS4_SaIS4_EERS8_
	.xword	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE8vFastELUESt6vectorIS4_SaIS4_EERS8_
	.xword	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE12vFastSigmoidESt6vectorIS4_SaIS4_EERS8_
	.xword	_ZN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEE9vFastTanhESt6vectorIS4_SaIS4_EERS8_
	.size	_ZTVN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE, 104

	.type	_ZTSN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,@object // @_ZTSN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.section	.rodata._ZTSN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,"aG",@progbits,_ZTSN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,comdat
	.weak	_ZTSN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.p2align	4
_ZTSN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE:
	.asciz	"N5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE"
	.size	_ZTSN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE, 80

	.type	_ZTSN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,@object // @_ZTSN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.section	.rodata._ZTSN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,"aG",@progbits,_ZTSN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,comdat
	.weak	_ZTSN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.p2align	4
_ZTSN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE:
	.asciz	"N5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE"
	.size	_ZTSN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE, 88

	.type	_ZTIN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,@object // @_ZTIN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.section	.rodata._ZTIN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,"aG",@progbits,_ZTIN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,comdat
	.weak	_ZTIN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.p2align	3
_ZTIN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE:
	.xword	_ZTVN10__cxxabiv117__class_type_infoE+16
	.xword	_ZTSN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.size	_ZTIN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE, 16

	.type	_ZTIN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,@object // @_ZTIN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.section	.rodata._ZTIN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,"aG",@progbits,_ZTIN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE,comdat
	.weak	_ZTIN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.p2align	4
_ZTIN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE:
	.xword	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.xword	_ZTSN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.xword	_ZTIN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.size	_ZTIN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE, 24

	.section	.init_array,"aw",@init_array
	.p2align	3
	.xword	_GLOBAL__sub_I_itest_vectorization.cpp

	.ident	"Arm C/C++/Fortran Compiler version 19.2 (build number 155) (based on LLVM 7.1.0)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym __gxx_personality_v0
	.addrsig_sym _ZL11svptrue_b16v
	.addrsig_sym _ZL13svwhilelt_b16ii
	.addrsig_sym _GLOBAL__sub_I_itest_vectorization.cpp
	.addrsig_sym _Unwind_Resume
	.addrsig_sym _ZStL8__ioinit
	.addrsig_sym __dso_handle
	.addrsig_sym _ZTVN10__cxxabiv120__si_class_type_infoE
	.addrsig_sym _ZTSN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.addrsig_sym _ZTVN10__cxxabiv117__class_type_infoE
	.addrsig_sym _ZTSN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.addrsig_sym _ZTIN5posit10vectorized23PositVectorizedFrontendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
	.addrsig_sym _ZTIN5posit10vectorized15PositSVEBackendINS_5PositIsLi16ELi0EtLNS_9PositSpecE1EEEEE
