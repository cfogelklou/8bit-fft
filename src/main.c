/*
 * main.c
 *	Starting point for demo application.
 *
 * Copyright © 2002 Ubicom Inc. <www.ubicom.com>.  All rights reserved.
 */
#include <ipOS.h>
#include <ipUART.h>
#include "fft.h"
#include "uart_tools.h"
#define CMD_PROBE 	'P'
#define CMD_MUL_C16_C16 2
#define CMD_MUL_S16_S16 4
#define CMD_GET_SINE	3
#define CMD_GET_TW_SINE	5
#define CMD_GET_TW_COSINE 6
#define CMD_SET_RESULT	7
#define CMD_GET_RESULT	8
#define CMD_ADD_S16_S16	9
#define CMD_ADD_C16_C16	10
#define CMD_ADD_C32_C32	11
#define CMD_DO_FFT 12
#define CMD_SUB_C16_C16 13

#define RSP_PROBE 	'R'

/*
 * IP2022 configuration block
 */
CONFIG_BLOCK (FUSE0(FUSE0_XTAL | FUSE0_PIN_DIV1 | FUSE0_POUT_DIV2 | FUSE0_WUDP_128us | FUSE0_WUDX_1ms),
		0xffc0,
		OSC1_FREQ,
		"UBICOM",
		"Empty Template",
		CONFIG_VER(0, 0, 0, 0),
		CONFIG_DATE(0, 0, 0),
		CONFIG_DATE(0, 0, 0));

/*
 * Stack size for our application.
 */
#define APP_STACK_SIZE 0x200

/*
 * A linker-defined symbol that signifies the end of the bss section.  We will use
 * the address of this to work out how much space to make available for our heap
 * memory.
 */
char command_state, command_substate;
extern void *_bss_end;

/*
 * main()
 *	Entry point to our application.
 */
int main(void) __attribute__((noreturn));
int main(void)
{
	struct uart_instance *ui;
	u8_t linefeed[] = {0x0a, 0x0d, 0};
	u8_t received_char;
	unsigned int i;
	/*
	 * Provide debugging capabilities before we attempt anything else.
	 */
	debug_init();

	/*
	 * Make space available from the end of the compiler-allocated RAM to the
	 * start of our call/argument stack.
	 */
	heap_add((addr_t)(&_bss_end), (addr_t)(RAMEND - (APP_STACK_SIZE - 1)) - (addr_t)(&_bss_end));

	/*
	 * Start timer 0 (used for virtual peripheral implementation), establish our
	 * interrupt vector and enable interrupts from all sources.
	 */
	tmr0_init();
	set_int_vector(isr);
	global_int_enable();


	/* 
	 * Create a new UART instance
	 */
	ui = U1_uart_vp_instance_alloc();
	/*
	 * Print "Hello World!" to the new UART
	 */
	uart_print(ui, linefeed);
	uart_print(ui, "Hello World!");
	uart_print(ui, linefeed);
	
	/* 
	 * Start character echoing
	 */
	ui->listen(ui, (void *)ui, NULL, NULL, NULL); 
			
	c16_t a_c16, b_c16, c_c16;
	s16_t a_s16, b_s16, c_s16;
	c32_t a_c32, b_c32, c_c32;
	s32_t a_s32, b_s32, c_s32;
	u16_t a_u16, b_u16, c_u16;
	s16_t * a_ps16;
	/*
	 * Start event polling.
	 */
	while (1) {
		received_char = cget(ui);
		switch(received_char){
			case CMD_PROBE:
				csend(ui,RSP_PROBE);
				break;
			case CMD_MUL_C16_C16:
				a_c16 = get_c16(ui);
				b_c16 = get_c16(ui);
				c_c16 = mul_c16_c16(a_c16,b_c16);
				send_c16(ui,c_c16);
				break;
			case CMD_MUL_S16_S16:
				a_s16 = (s16_t)get_u16(ui);
				b_s16 = (s16_t)get_u16(ui);
				c_s32 = mul_s16_s16(a_s16,b_s16);
				send_u32(ui,(u32_t)c_s32);
				break;
			case CMD_GET_SINE:
				a_u16 = get_u16(ui);
				c_s16 = get_sine_val(a_u16);
				send_u16(ui,(u16_t)c_s16);
				break;
			case CMD_GET_TW_SINE:
				a_u16 = get_u16(ui);
				b_u16 = get_u16(ui);
				c_s16 = TwiddleFactorSine(a_u16, b_u16);
				send_u16(ui, (u16_t)c_s16);
				break;
			case CMD_GET_TW_COSINE:
				a_u16 = get_u16(ui);
				b_u16 = get_u16(ui);
				c_s16 = TwiddleFactorCosine(a_u16, b_u16);
				send_u16(ui, (u16_t)c_s16);
				break;
			case CMD_GET_RESULT:
				a_u16 = get_u16(ui);
				c_c16 = get_fft_result (a_u16);
				send_c16(ui,c_c16);
				break;
			case CMD_SET_RESULT:
				a_u16 = get_u16(ui);
				b_c16 = get_c16(ui);
				set_fft_result (a_u16, b_c16);
				break;
			case CMD_ADD_S16_S16:
				a_s16 = (s16_t)get_u16(ui);
				b_s16 = (s16_t)get_u16(ui);
				c_s16 = (s16_t)add_u16_u16((u16_t)a_s16,(u16_t)b_s16);
				send_u16(ui,(u16_t)c_s16);
				break;
			case CMD_ADD_C16_C16:
				a_c16 = get_c16(ui);
				b_c16 = get_c16(ui);
				c_c16 = add_c16_c16(a_c16,b_c16);
				send_c16(ui,c_c16);
				break;			
			case CMD_SUB_C16_C16:
				a_c16 = get_c16(ui);
				b_c16 = get_c16(ui);
				c_c16 = sub_c16_c16(a_c16,b_c16);
				send_c16(ui,c_c16);
				break;
			case CMD_ADD_C32_C32:
				a_c32 = get_c32(ui);
				b_c32 = get_c32(ui);
				c_c32 = add_c32_c32(a_c32,b_c32);
				send_c32(ui,c_c32);
				break;
			case CMD_DO_FFT:
				a_u16 = get_u16(ui);
				a_ps16 = (s16_t *)mem_alloc((addr_t)a_u16,PKG_USER1, NULL);
				for (i = 0; i < a_u16; i++) {
					a_ps16[i] = (s16_t)get_u16(ui);
				}
				fft(a_ps16, a_u16, (void *) ui);
				for (i = 0; i < a_u16; i++) {
					send_c16(ui,get_fft_result(i));
				}
				break;
			default:
				break;
		}	
	}
}

