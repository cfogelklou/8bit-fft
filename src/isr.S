;
; isr.S
;
; Copyright © 2002 Ubicom Inc. <www.ubicom.com>.  All rights reserved.
;

#include <config.h>
#include <ip2k/ip2000_asm.h>
#include <ip2k/ip2022_asm.h>
#include <ip2k/tmr0_isr.inc>

;
; inline isr code
;

	.section .pram,"ax"
	.global _isr

_isr:
	isr_entry

;
; Install any non TMR0 interrupt tasks first.
;

;
; TMR0 - Must be last.
;
	tmr0_isr_check

;
; Install any tasks to execute every TMR0 interrupt here.
;

;
; Install any tasks to be scheduled from TMR0 here.
;
	tmr0_isr_table
	tmr0_isr_table_entry(_tmr0_slot0)
	tmr0_isr_table_entry(_tmr0_slot1)
	tmr0_isr_table_entry(_tmr0_slot2)
	tmr0_isr_table_entry(_tmr0_slot3)
	tmr0_isr_table_entry(_tmr0_slot4)
	tmr0_isr_table_entry(_tmr0_slot5)
	tmr0_isr_table_entry(_tmr0_slot6)
	tmr0_isr_table_entry(_tmr0_slot7)

_tmr0_isr_skip:
	isr_exit
	reti	#(RETI_REINSTATE_SPEED)

_tmr0_slot0:
_tmr0_slot2:
_tmr0_slot4:
_tmr0_slot6:
	#include "U1_uart_vp_rx_isr_inline.S"
	isr_exit
	mov     w, #-(SYSTEM_FREQ / TMR0_PRESCALE / TMR0_INT_FREQ)
	reti	#(RETI_REINSTATE_SPEED | RETI_ADD_TMR0)
_tmr0_slot1:
	#include "U1_uart_vp_tx_isr_inline.S"
	isr_exit
	mov     w, #-(SYSTEM_FREQ / TMR0_PRESCALE / TMR0_INT_FREQ)
	reti	#(RETI_REINSTATE_SPEED | RETI_ADD_TMR0)
_tmr0_slot5:
_tmr0_slot3:
_tmr0_slot7:
	isr_exit
	mov     w, #-(SYSTEM_FREQ / TMR0_PRESCALE / TMR0_INT_FREQ)
	reti	#(RETI_REINSTATE_SPEED | RETI_ADD_TMR0)

;
; isr subroutines
;
	#include "U1_uart_vp_isr_subroutines.S"
