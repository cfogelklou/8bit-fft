
#include "user_config.h"
#include <ip2k/ip2000_asm.h>
#include <ip2k/ip2022_asm.h>	

;-----------------------------------------------------------------
;u16_t add_u16_u16 (u16_t a, u16_t b ); // passing two parameters
.section .pram, "ax" ; locate in flash
.global _add_u16_u16
.func add_u16_u16, _add_u16_u16
_add_u16_u16:
	; --- Prologue: Save the current stack frame
	push $fe
	push $fd
	; Load the new stack frame
	mov w,SPL
	mov $fe,w
	mov w,SPH
	mov $fd,w
	; --- End Prologue	
	; Assembly function implementation.
	; Perform the desired assembly operation
	; on the passed parameters and return the result.
	; ----
	mov w,4(SP) ; Add low byte of u16_t
	add w,6(SP) ; to low byte of b
	mov $81,w ; and store in low byte of return value
	mov w,#0 ; Add carry
	addc w,3(SP) ; to high byte of a
	add w,5(SP); and add high byte b
	mov $80,w ; and move to high byte of return value
	; ----
	; --- Epilogue: Restore the saved stack frame
	pop $fd
	pop $fe
	; Adjust the stack to account for the parameters passed
	mov w,#4 ; add 4 (2 for u16_t a + 2 for u16_t b)
	add SPL,w
	; --- End Epilogue
	ret
.endfunc

;-----------------------------------------------------------------
;u16_t sub_c16_c16 (c16_t a, c16_t b ); // passing two parameters
.section .pram, "ax" ; locate in flash
.global _sub_c16_c16
.func sub_c16_c16, _sub_c16_c16
_sub_c16_c16:
	; --- Prologue: Save the current stack frame
	push $fe
	push $fd
	; Load the new stack frame
	mov w,SPL
	mov $fe,w
	mov w,SPH
	mov $fd,w
	; --- End Prologue	
	; Assembly function implementation.
	; Perform the desired assembly operation
	; on the passed parameters and return the result.
	; ----
	
	mov w,8(SP); Sub low byte of b.real
	sub w,4(SP) ; From low byte of a.real
	mov $81,w ; and store in low byte of return value.real
	mov w,7(SP) ; Sub high byte of b.real
	subc w,3(SP) ; from high byte of a.real
	mov $80,w ; and move to high byte of return value.real

	mov w,10(SP); Sub low byte of b.imag
	sub w,6(SP) ; From low byte of a.imag
	mov $83,w ; and store in low byte of return value.imag
	mov w,9(SP) ; Sub high byte of b.imag
	subc w,5(SP) ; from high byte of a.imag
	mov $82,w ; and move to high byte of return value.imag
	
	; ----
	; --- Epilogue: Restore the saved stack frame
	pop $fd
	pop $fe
	; Adjust the stack to account for the parameters passed
	mov w,#8 ; add 8 (4 for c16_t a + 4 for c16_t b)
	add SPL,w
	; --- End Epilogue
	ret
.endfunc

;-----------------------------------------------------------------
;c16_t add_c16_c16 (c16_t a, c16_t b ); // passing two parameters
.section .pram, "ax" ; locate in flash
.global _add_c16_c16
.func add_c16_c16, _add_c16_c16
_add_c16_c16:
	; --- Prologue: Save the current stack frame
	push $fe
	push $fd
	; Load the new stack frame
	mov w,SPL
	mov $fe,w
	mov w,SPH
	mov $fd,w
	; --- End Prologue	
	; Assembly function implementation.
	; Perform the desired assembly operation
	; on the passed parameters and return the result.
	; ----
	mov w,4(SP) ; Add low byte of a.real
	add w,8(SP) ; to low byte of b.real
	mov $81,w ; and store in low byte of return value.real
	mov w,#0 ; Add carry
	addc w,3(SP) ; to high byte of a.real
	add w,7(SP); and add high byte b.real
	mov $80,w ; and move to high byte of return value.real
	mov w,6(SP) ; Add low byte of a.imag
	add w,10(SP) ; to low byte of b.imag
	mov $83,w ; and store in low byte of return value.imag
	mov w,#0 ; Add carry
	addc w,5(SP) ; to high byte of a.imag
	add w,9(SP); and add high byte b.imag
	mov $82,w ; and move to high byte of return value.imag
	; ----
	; --- Epilogue: Restore the saved stack frame
	pop $fd
	pop $fe
	; Adjust the stack to account for the parameters passed
	mov w,#8 ; add 4 (2 for u16_t a + 2 for u16_t b)
	add SPL,w
	; --- End Epilogue
	ret
.endfunc

;-----------------------------------------------------------------
;u32_t mul_u16_u16 (u16_t a, u16_t b ); // passing two parameters
.section .pram, "ax" ; locate in flash
.global _mul_u16_u16
.func mul_u16_u16, _mul_u16_u16
_mul_u16_u16:
	; --- Prologue: Save the current stack frame
	push $fe
	push $fd
	; Load the new stack frame
	mov w,SPL
	mov $fe,w
	mov w,SPH
	mov $fd,w
	; --- End Prologue	
	; Assembly function implementation.
	; Perform the desired assembly operation
	; on the passed parameters and return the result.
	; ----
	; a = 3(sp) b = 4(sp) c = 5(sp) d = 6(sp)
	; $83 = bd_l, $82 = bd_h
	mov w,4(SP) ; Multiply low byte of u16_t a
	mulu w,6(SP) ; with low byte of b
	mov $83,w
	mov w,mulh
	mov $82,w
	; $82 += ad_l, $81 = ad_h + bd_c
	mov w, 3(SP)
	mulu w, 6(SP)
	add $82,w
	clr $81
	mov w,mulh
	addc $81,w
	; $82 += cb_l, $81 += cb_h + cb_c
	mov w, 4(SP)
	mulu w, 5(SP)
	add $82,w
	mov w,mulh
	addc $81,w
	clr $80
	snb STATUS,C
	inc $80
	; $81 += ac_l, $80 = ac_h + ac_c
	mov w, 3(SP)
	mulu w, 5(SP)
	add $81,w
	clr $80
	mov w,mulh
	addc $80,w
	; ----
	; --- Epilogue: Restore the saved stack frame
	pop $fd
	pop $fe
	; Adjust the stack to account for the parameters passed
	mov w,#4 ; add 4 (2 for u16_t a + 2 for u16_t b)
	add SPL,w
	; --- End Epilogue
	ret
.endfunc


#define ROUNDING 1
;-----------------------------------------------------------------
;c16_t as_mul_c16_c16 (c16_t a, c16_t b); // passing two parameters
; return.real = (a.real*b.real - a.imag*b.imag)>>16
; return.imag = (a.real*b.imag + a.imag*b.real)>>16
.section .pram, "ax" ; locate in flash
.global _mul_c16_c16
.func mul_c16_c16, _mul_c16_c16
_mul_c16_c16:
	; --- Prologue: Save the current stack frame
	push $fe
	push $fd
	; Load the new stack frame
	mov w,SPL
	mov $fe,w
	mov w,SPH
	mov $fd,w			
	; --- End Prologue	
; save signs of all multiplies
	clr _fft_sign
	mov w, 3(SP) ; MSB of a.real
	xor w, 7(SP) ; MSB of b.real
	and w, #%10000000
	sz
	setb _fft_sign, 4
	mov w, 5(SP) ;MSB of a.imag
	xor w, 9(SP) ;MSB of b.imag
	and w, #%10000000 
	snz		; snz since this is negative (j*j = -1)
	setb _fft_sign, 5 
	mov w, 3(SP) ;MSB of a.real
	xor w, 9(SP) ;MSB of b.imag
	and w, #%10000000
	sz
	setb _fft_sign, 6
	mov w, 5(SP);MSB of a.imag
	xor w, 7(SP);MSB of b.real
	and w, #%10000000
	sz
	setb _fft_sign, 7
; make all inputs positive
a_real_sign:
	sb 3(SP),7
	jmp a_imag_sign
	not 4(SP)
	not 3(SP)
	inc 4(SP)
	snz
	inc 3(SP)
a_imag_sign:
	sb 5(SP),7
	jmp b_real_sign
	not 6(SP)
	not 5(SP)
	inc 6(SP)
	snz
	inc 5(SP)
b_real_sign:
	sb 7(SP),7
	jmp b_imag_sign
	not 8(SP)
	not 7(SP)
	inc 8(SP)
	snz
	inc 7(SP)
b_imag_sign:
	sb 9(SP),7
	jmp do_areal_breal
	not 10(SP)
	not 9(SP)
	inc 10(SP)
	snz
	inc 9(SP)

	; ----
	
do_areal_breal:
	mov w, 3(SP); mov w, arh
	mulu w, 7(SP); mul w, brh
	mov ipl, w	; mov ipl, w
	mov w, mulh	; mov iph, mulh
	mov iph, w
	mov w, 3(SP)	; mov w, arh
	mulu w, 8(SP)	; mul w, brl
	mov $86, w	; mov _fft_a_l, w
	mov w, mulh	; add ipl, mulh
	add ipl, w
	mov w, 4(SP)	; mov w, arl
	mulu w, 7(SP)	; mulu w, brh
	add $86, w	; add _fft_a_l, w
	mov w, mulh	; add ipl, mulh
	addc ipl, w
	mov w, 4(SP)	; mov w, arl
	mulu w, 8(SP)	; mulu w, brl
	mov $87,w
	mov w, mulh	; add _fft_a_l, mulh
	add $86, w
	snb STATUS, C	; snc
	inc ipl	; inc ipl


	; do sign
	sb _fft_sign, 4
	jmp do_aimag_bimag
	not $86
	not $87
	not ipl
	not iph
	inc $87
	snz
	inc $86
	snz
	inc ipl
do_aimag_bimag:	
	; move to return loc
	mov w, iph
	mov $80, w
	mov w, ipl
	mov $81, w
		
	; Next multiply... a.imag * b.imag
	mov w, 5(SP); mov w, aih
	mulu w, 9(SP); mul w, bih
	mov ipl, w	; mov ipl, w
	mov w, mulh	; mov iph, mulh
	mov iph, w
	mov w, 5(SP)	; mov w, aih
	mulu w, 10(SP)	; mul w, bil
	mov $88, w	; mov 86, w
	mov w, mulh	; add ipl, mulh
	add ipl, w
	mov w, 6(SP)	; mov w, ail
	mulu w, 9(SP)	; mulu w, bih
	add $88, w	; add 86, w
	mov w, mulh	; add ipl, mulh
	addc ipl, w
	mov w, 6(SP)	; mov w, ail
	mulu w, 10(SP)	; mulu w, bil
	mov $89,w
	mov w, mulh	; add 86, mulh
	add $88, w
	snb STATUS, C	; snc
	inc ipl	; inc ipl

	; do sign
	sb _fft_sign, 5
	jmp do_areal_bimag
	not $88
	not $89
	not ipl
	not iph
	inc $89
	snz
	inc $88
	snz
	inc ipl

	
do_areal_bimag:	

	; propagate the carry up to the ipl registers
	mov w, $87 ; add the two least significant bytes
	add w, $89
	mov w, $88 ; add the next two bytes with carry
	addc $86, w
	mov w, ipl ; add the next two bytes with carry
	addc $81, w
	mov w, iph ; add to the upper byte with carry
	addc $80, w
#ifdef ROUNDING
	sb $86,7
	jmp no_round1
	inc $81
	snz
	inc $80
no_round1:
#endif
	mov w, 3(SP); mov w, arh
	mulu w, 9(SP); mul w, bih
	mov ipl, w	; mov ipl, w
	mov w, mulh	; mov iph, mulh
	mov iph, w
	mov w, 3(SP)	; mov w, arh
	mulu w, 10(SP)	; mul w, bil
	mov $86, w	; mov 86, w
	mov w, mulh	; add ipl, mulh
	add ipl, w
	mov w, 4(SP)	; mov w, arl
	mulu w, 9(SP)	; mulu w, bih
	add $86, w	; add 86, w
	mov w, mulh	; add ipl, mulh
	addc ipl, w
	mov w, 4(SP)	; mov w, arl
	mulu w, 10(SP)	; mulu w, bil
	mov $87, w
	mov w, mulh	; add 86, mulh
	add $86, w
	snb STATUS, C	; snc
	inc ipl	; inc ipl

	; do sign
	sb _fft_sign, 6
	jmp do_aimag_breal
	not $87
	not $86
	not ipl
	not iph
	inc $87
	snz
	inc $86
	snz
	inc ipl
do_aimag_breal:	
	; move to return loc
	mov w, iph
	mov $82, w
	mov w, ipl
	mov $83, w
	
	; Next multiply... a.imag * b.real
	mov w, 5(SP); mov w, aih
	mulu w, 7(SP); mul w, brh
	mov ipl, w	; mov ipl, w
	mov w, mulh	; mov iph, mulh
	mov iph, w
	mov w, 5(SP)	; mov w, aih
	mulu w, 8(SP)	; mul w, brl
	mov $88, w	; mov 86, w
	mov w, mulh	; add ipl, mulh
	add ipl, w
	mov w, 6(SP)	; mov w, ail
	mulu w, 7(SP)	; mulu w, brh
	add $88, w	; add 86, w
	mov w, mulh	; add ipl, mulh
	addc ipl, w
	mov w, 6(SP)	; mov w, ail
	mulu w, 8(SP)	; mulu w, brl
	mov $89, w
	mov w, mulh	; add 86, mulh
	add $88, w
	snb STATUS, C	; snc
	inc ipl	; inc ipl

	; do sign
	sb _fft_sign, 7
	jmp mul_c16_c16_done
	not $89
	not $88
	not ipl
	not iph
	inc $89
	snz
	inc $88
	snz
	inc ipl
mul_c16_c16_done:	
	; add to return loc
	; propagate the carry up to the ipl registers
	mov w, $87 ; add the two least significant bytes
	add w, $89
	mov w, $88 ; add the next two bytes with carry
	addc $86,w
	mov w, ipl ; add the next two bytes with carry
	addc $83, w
	mov w, iph ; add to the upper byte with carry
	addc $82, w
#ifdef ROUNDING
	sb $86,7
	jmp no_round2
	inc $83
	snz
	inc $82
no_round2:
#endif	
	; --- Epilogue: Restore the saved stack frame
	pop $fd
	pop $fe
	; Adjust the stack to account for the parameters passed
	mov w,#8 ; add 8 (4 for c16_t a + 4 for c16_t b)
	add SPL,w
	; --- End Epilogue
	ret
.endfunc

;-----------------------------------------------------------------
;s32_t as_mul_s16_s16 (void); // passing two parameters
.section .pram, "ax" ; locate in flash
.global _as_mul_s16_s16
.func as_mul_s16_s16, _as_mul_s16_s16
_as_mul_s16_s16:
	mov w, _fft_a_h
	xor w, _fft_b_h
	mov _fft_sign, w
	sb _fft_a_h, 7
	jmp b_sign
	not _fft_a_l
	not _fft_a_h
	inc _fft_a_l
	snz
	inc _fft_a_h
b_sign:
	sb _fft_b_h, 7
	jmp multiply
	not _fft_b_l
	not _fft_b_h
	inc _fft_b_l
	snz
	inc _fft_b_h
multiply:
	; ----
	; a_h = 3(sp) a_l = 4(sp) b_h = 5(sp) b_l = 6(sp)
	; $83 = bd_l, $82 = bd_h
	mov w,_fft_a_l ; Multiply low byte of u16_t a
	mulu w,_fft_b_l ; with low byte of b
	mov $83,w
	mov w,mulh
	mov $82,w
	; $82 += ad_l, $81 = ad_h + bd_c
	mov w, _fft_a_h
	mulu w, _fft_b_l
	add $82,w
	clr $81
	mov w,mulh
	addc $81,w
	; $82 += cb_l, $81 += cb_h + cb_c
	mov w, _fft_a_l;4(SP)
	mulu w, _fft_b_h;5(SP)
	add $82,w
	mov w,mulh
	addc $81,w
	clr $80
	snb STATUS,C
	inc $80
	; $81 += ac_l, $80 = ac_h + ac_c
	mov w, _fft_a_h;3(SP)
	mulu w, _fft_b_h;5(SP)
	add $81,w
	clr $80
	mov w,mulh
	addc $80,w
result_sign:
	sb _fft_sign, 7
	ret
	not $80
	not $81
	not $82
	not $83
	incsz $83
	ret
	incsz $82
	ret
	incsz $81
	ret
	inc $80
	ret
.endfunc

;-----------------------------------------------------------------
;c16_t get_fft_result (u16_t index);
.section .text, "ax" ; locate in flash
.global _get_fft_result
.func _get_fft_result, _get_fft_result
_get_fft_result:
	; --- Prologue: Save the current stack frame
	push $fe
	push $fd
	; Load the new stack frame
	mov w,SPL
	mov $fe,w
	mov w,SPH
	mov $fd,w
	; --- End Prologue	

	mov w, 4(sp)
	and w, #((MAXFFTSIZE-1))
	mov addrl,w
	mov w, 3(sp)
	and w, #((MAXFFTSIZE-1)/256)
	mov addrh,w
	clrb STATUS, C
	rl addrl
	rl addrh
	mov w, #%hi8insn(_fft_results)
	add addrh,w
	mov w, #%lo8insn(_fft_results)
	add addrl, w
	rl addrl;
	rl addrh;
	mov w,#PRAM_ADDRX
	mov addrx, w
	ireadi ; get real data (blocking - reading RAM)
	mov w,DATAL ; get data retrieved from iread.
	mov $81,w
	mov w,DATAH
	mov $80,w
	iread ; get imaginary data (blocking - reading RAM)
	mov w,DATAL
	mov $83,w
	mov w,DATAH
	mov $82,w
	; ----
	; --- Epilogue: Restore the saved stack frame
	pop $fd
	pop $fe
	; Adjust the stack to account for the parameters passed
	mov w,#2 ; add 2 (2 for u16_t)
	add SPL,w
	; --- End Epilogue
	
	ret
.endfunc
;-----------------------------------------------------------------
;void set_fft_result (u16_t index, c16_t data);
.section .text, "ax" ; locate in flash
.global _set_fft_result
.func _set_fft_result, _set_fft_result
_set_fft_result:
	; --- Prologue: Save the current stack frame
	push $fe
	push $fd
	; Load the new stack frame
	mov w,SPL
	mov $fe,w
	mov w,SPH
	mov $fd,w
	; --- End Prologue	

	mov w, 4(sp)
	and w, #((MAXFFTSIZE-1))
	mov addrl,w
	mov w, 3(sp)
	and w,#((MAXFFTSIZE-1)/256)
	mov addrh,w
	clrb STATUS, C
	rl addrl
	rl addrh
	mov w, #%hi8insn(_fft_results)
	add addrh,w
	mov w, #%lo8insn(_fft_results)
	add addrl, w
	rl addrl;
	rl addrh;
	mov w,#PRAM_ADDRX
	mov addrx, w
	mov w,5(sp)
	mov DATAH,w
	mov w,6(sp)
	mov DATAL,w
	iwritei ; set real data (blocking - reading RAM)
	mov w,7(sp) ; get data retrieved from iread.
	mov DATAH,w
	mov w,8(sp)
	mov DATAL,w
	iwrite ; set imaginary data (blocking - reading RAM)
	; ----
	; --- Epilogue: Restore the saved stack frame
	pop $fd
	pop $fe
	; Adjust the stack to account for the parameters passed
	mov w,#6 ; add 6 (2 for u16_t, 4 for c16_t)
	add SPL,w
	; --- End Epilogue
	
	ret
.endfunc


;-----------------------------------------------------------------
;	as_copy_scrambled();
;void as_copy_scrambled(void); // passing one parameter
.section .text, "ax" ; locate in flash
.global _as_copy_scrambled
.func as_copy_scrambled, _as_copy_scrambled
_as_copy_scrambled:
	; --- Prologue: Save the current stack frame
	push $fe
	push $fd
	; Load the new stack frame
	mov w,SPL
	mov $fe,w
	mov w,SPH
	mov $fd,w
	; --- End Prologue
	
	clr _fft_count1_h
	clr _fft_count1_l
	mov w, _fft_dp1_lo	
	mov ipl, w
	mov w, _fft_dp1_hi
	mov iph, w
	mov w,#PRAM_ADDRX
	mov addrx, w
	
cs_loop1:
	; bit_reverse count1 into (bits) bits.
	mov w, _fft_N_l
	mov _fft_N_mask_l,w
	mov w, _fft_N_h
	mov _fft_N_mask_h,w
	clr _fft_index_l
	clr _fft_index_h
	mov w, _fft_count1_l
	mov _fft_temp1, w
	mov w, _fft_count1_h
cs_loop2:
	rr wreg
	rr _fft_temp1
	rl _fft_index_l
	rl _fft_index_h
	rr _fft_N_mask_h
	rr _fft_N_mask_l
	sb _fft_N_mask_l, 0
	jmp cs_loop2
cs_index_done:
; now have correct write index in _fft_index	
	; address the new location in pram
	clrb STATUS, C
	rl _fft_index_l
	rl _fft_index_h

	mov w, _fft_index_l
	add w, #%lo8insn(_fft_results)
	mov ADDRL, w
	mov w, _fft_index_h
	mov w, #%hi8insn(_fft_results)
	addc w, _fft_index_h
	mov ADDRH,w
	setb STATUS, C
	rl ADDRL;
	rl ADDRH;
	mov w, (IP)
	mov DATAH, w
	inc IPL
	mov w, (IP)
	mov DATAL, w
	iwritei 
	inc IPL
	mov w, #0
	mov DATAL, w
	mov DATAH, w
	iwrite 
	inc _fft_count1_l
	snz
	inc _fft_count1_h
	mov w, _fft_N_l
	xor w, _fft_count1_l
	sz
	jmp cs_loop1
	mov w, _fft_N_h
	xor w, _fft_count1_h
	sz
	jmp cs_loop1
	; ----
	; --- Epilogue: Restore the saved stack frame
	pop $fd
	pop $fe
	; Adjust the stack to account for the parameters passed
	mov w,#0 ; add 6 (2 for u16_t, 4 for c16_t)
	add SPL,w
	; --- End Epilogue
	
	ret
.endfunc

;-----------------------------------------------------------------
;void as_calc_bits (void);
.section .pram, "ax" ; locate in flash
.global _as_calc_bits
.func as_calc_bits, _as_calc_bits
_as_calc_bits:
	mov w, _fft_N_l
	mov _fft_temp1,w
	mov w, _fft_N_h
	clr _fft_bits
bitsloop:
	inc _fft_bits
	rr wreg
	rr _fft_temp1
	sb _fft_temp1, 0
	jmp bitsloop
	ret
.endfunc
;-----------------------------------------------------------------
;s16_t get_sine_val (u16_t index); // passing one parameter
.section .text ; locate in flash
.global _get_sine_val
.func get_sine_val, _get_sine_val
_get_sine_val:
	; --- Prologue: Save the current stack frame
	push $fe
	push $fd
	; Load the new stack frame
	mov w,SPL
	mov $fe,w
	mov w,SPH
	mov $fd,w
	; --- End Prologue	

	mov w, 4(sp)
	and w, #((FFTTABLESIZE-1))
	mov addrl, w
	mov w, 3(sp)
	and w, #((FFTTABLESIZE-1)/256)
	mov addrh,w
	mov w, #%hi8insn(_sine_table)
	add addrh, w
	mov w, #%lo8insn(_sine_table)
	add addrl,w
	clrb STATUS, C
	rl addrl;
	rl addrh;
	mov w,#TEXT_ADDRX
	mov addrx, w
	iread ; have to wait a few cycles, so do epilogue now
	nop
	nop
	nop
	nop
	mov w,DATAL ; get data retrieved from iread.
	mov $81,w
	mov w,DATAH
	mov $80,w
	sb 3(sp),(FFTTABLESIZE/512)
	jmp sine_done
	not $80
	not $81
	inc $81
	snz
	inc $80
sine_done:
	; ----
	; --- Epilogue: Restore the saved stack frame
	pop $fd
	pop $fe
	; Adjust the stack to account for the parameters passed
	mov w,#2 ; add 6 (2 for u16_t, 4 for c16_t)
	add SPL,w
	; --- End Epilogue
	ret
.endfunc
;-----------------------------------------------------------------
.section .pram, "ax"
.global _fft_results
_fft_results:
	.space MAXFFTSIZE * 4, 0xFE

	
;-----------------------------------------------------------------
.section .text, "ax" ; locate in flash
.global _sine_table
_sine_table:
;.org sine_table_org
	.hword	0
	.hword	402
	.hword	804
	.hword	1206
	.hword	1608
	.hword	2009
	.hword	2411
	.hword	2811
	.hword	3212
	.hword	3612
	.hword	4011
	.hword	4410
	.hword	4808
	.hword	5205
	.hword	5602
	.hword	5998
	.hword	6393
	.hword	6787
	.hword	7180
	.hword	7571
	.hword	7962
	.hword	8351
	.hword	8740
	.hword	9127
	.hword	9512
	.hword	9896
	.hword	10279
	.hword	10660
	.hword	11039
	.hword	11417
	.hword	11793
	.hword	12167
	.hword	12540
	.hword	12910
	.hword	13279
	.hword	13646
	.hword	14010
	.hword	14373
	.hword	14733
	.hword	15091
	.hword	15447
	.hword	15800
	.hword	16151
	.hword	16500
	.hword	16846
	.hword	17190
	.hword	17531
	.hword	17869
	.hword	18205
	.hword	18538
	.hword	18868
	.hword	19195
	.hword	19520
	.hword	19841
	.hword	20160
	.hword	20475
	.hword	20788
	.hword	21097
	.hword	21403
	.hword	21706
	.hword	22006
	.hword	22302
	.hword	22595
	.hword	22884
	.hword	23170
	.hword	23453
	.hword	23732
	.hword	24008
	.hword	24279
	.hword	24548
	.hword	24812
	.hword	25073
	.hword	25330
	.hword	25583
	.hword	25833
	.hword	26078
	.hword	26320
	.hword	26557
	.hword	26791
	.hword	27020
	.hword	27246
	.hword	27467
	.hword	27684
	.hword	27897
	.hword	28106
	.hword	28311
	.hword	28511
	.hword	28707
	.hword	28899
	.hword	29086
	.hword	29269
	.hword	29448
	.hword	29622
	.hword	29792
	.hword	29957
	.hword	30118
	.hword	30274
	.hword	30425
	.hword	30572
	.hword	30715
	.hword	30853
	.hword	30986
	.hword	31114
	.hword	31238
	.hword	31357
	.hword	31471
	.hword	31581
	.hword	31686
	.hword	31786
	.hword	31881
	.hword	31972
	.hword	32058
	.hword	32138
	.hword	32214
	.hword	32286
	.hword	32352
	.hword	32413
	.hword	32470
	.hword	32522
	.hword	32568
	.hword	32610
	.hword	32647
	.hword	32679
	.hword	32706
	.hword	32729
	.hword	32746
	.hword	32758
	.hword	32766
	.hword	32767 ; This should be 32768, which cannot be represented in 16-bit signed format
	.hword	32766
	.hword	32758
	.hword	32746
	.hword	32729
	.hword	32706
	.hword	32679
	.hword	32647
	.hword	32610
	.hword	32568
	.hword	32522
	.hword	32470
	.hword	32413
	.hword	32352
	.hword	32286
	.hword	32214
	.hword	32138
	.hword	32058
	.hword	31972
	.hword	31881
	.hword	31786
	.hword	31686
	.hword	31581
	.hword	31471
	.hword	31357
	.hword	31238
	.hword	31114
	.hword	30986
	.hword	30853
	.hword	30715
	.hword	30572
	.hword	30425
	.hword	30274
	.hword	30118
	.hword	29957
	.hword	29792
	.hword	29622
	.hword	29448
	.hword	29269
	.hword	29086
	.hword	28899
	.hword	28707
	.hword	28511
	.hword	28311
	.hword	28106
	.hword	27897
	.hword	27684
	.hword	27467
	.hword	27246
	.hword	27020
	.hword	26791
	.hword	26557
	.hword	26320
	.hword	26078
	.hword	25833
	.hword	25583
	.hword	25330
	.hword	25073
	.hword	24812
	.hword	24548
	.hword	24279
	.hword	24008
	.hword	23732
	.hword	23453
	.hword	23170
	.hword	22884
	.hword	22595
	.hword	22302
	.hword	22006
	.hword	21706
	.hword	21403
	.hword	21097
	.hword	20788
	.hword	20475
	.hword	20160
	.hword	19841
	.hword	19520
	.hword	19195
	.hword	18868
	.hword	18538
	.hword	18205
	.hword	17869
	.hword	17531
	.hword	17190
	.hword	16846
	.hword	16500
	.hword	16151
	.hword	15800
	.hword	15447
	.hword	15091
	.hword	14733
	.hword	14373
	.hword	14010
	.hword	13646
	.hword	13279
	.hword	12910
	.hword	12540
	.hword	12167
	.hword	11793
	.hword	11417
	.hword	11039
	.hword	10660
	.hword	10279
	.hword	9896
	.hword	9512
	.hword	9127
	.hword	8740
	.hword	8351
	.hword	7962
	.hword	7571
	.hword	7180
	.hword	6787
	.hword	6393
	.hword	5998
	.hword	5602
	.hword	5205
	.hword	4808
	.hword	4410
	.hword	4011
	.hword	3612
	.hword	3212
	.hword	2811
	.hword	2411
	.hword	2009
	.hword	1608
	.hword	1206
	.hword	804
	.hword	402
.end	


