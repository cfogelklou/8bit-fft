#include "uart_tools.h"
#include "fft.h"


void csend(void *protocol_instance, u8_t in_char) 
{
	struct uart_instance *ui = (struct uart_instance *)protocol_instance;
	while (!ui->get_send_ready(ui));
	ui->send(ui, in_char);
}

char cget(void *protocol_instance)
{
	struct uart_instance *ui = (struct uart_instance *)protocol_instance;
	while (!ui->get_recv_ready(ui));
	return (ui->recv(ui));
}
	

void send_u16(void *protocol_instance, u16_t a)
{
	struct uart_instance *ui = (struct uart_instance *)protocol_instance;
	csend(ui, (u8_t)(a>>8));
	csend(ui, (u8_t)(a));
}

void send_u32(void *protocol_instance, u32_t a)
{
	struct uart_instance *ui = (struct uart_instance *)protocol_instance;
	csend(ui, (u8_t)(a>>24));
	csend(ui, (u8_t)(a>>16));
	csend(ui, (u8_t)(a>>8));
	csend(ui, (u8_t)(a));
}

void send_c16(void *protocol_instance, c16_t data) 
{
	struct uart_instance *ui = (struct uart_instance*)protocol_instance;
	send_u16(ui, (u16_t)data.real);
	send_u16(ui, (u16_t)data.imag);
}
void send_c32(void *protocol_instance, c32_t data) 
{
	struct uart_instance *ui = (struct uart_instance*)protocol_instance;
	send_u32(ui, (u32_t)data.real);
	send_u32(ui, (u32_t)data.imag);
}
u16_t get_u16(void *protocol_instance)
{
	struct uart_instance *ui = (struct uart_instance*)protocol_instance;
	u16_t a;
	a = (((u16_t)cget(ui))<<8)&0xFF00;
	a = a | (((u16_t)cget(ui))&0x00FF);
	return a;
}
c16_t get_c16(void *protocol_instance)
{
	struct uart_instance *ui = (struct uart_instance*)protocol_instance;
	u16_t t1;
	u16_t t2;
	c16_t a;
	t1 = get_u16(ui);
	t2 = get_u16(ui);
	a.real = (s16_t)t1;
	a.imag = (s16_t)t2;
	return a;
}
u32_t get_u32(void *protocol_instance)
{
	struct uart_instance *ui = (struct uart_instance*)protocol_instance;
	u32_t a;
	a = ((((u32_t)cget(ui))<<24)&0xFF000000);
	a = a | ((((u32_t)cget(ui))<<16)&0x00FF0000);
	a = a | ((((u32_t)cget(ui))<<8)&0x0000FF00);
	a = a | ((((u32_t)cget(ui)))&0x000000FF);
	return a;
}



c32_t get_c32(void *protocol_instance)
{
	struct uart_instance *ui = (struct uart_instance*)protocol_instance;
	c32_t a;
	a.real = (s32_t)get_u32(ui);
	a.imag = (s32_t)get_u32(ui);
	return a;
}

void send_fft_results(void *protocol_instance, u16_t N) 
{
	struct uart_instance *ui = (struct uart_instance*)protocol_instance;
	u16_t a;
	u8_t linefeed[] = {0x0a, 0x0d, 0};
	uart_print(ui, linefeed);
	uart_print(ui,"fft results:");
	uart_print(ui, linefeed);
	for (a = 0; a != N; a++) {
		send_c16(ui, get_fft_result(a));
	}
}

/*
 * Function to send an entire string via the UART passed in protocol_instance
 */
void uart_print(void *protocol_instance, u8_t *string)
{
	u8_t *index;
	struct uart_instance *ui = (struct uart_instance *)protocol_instance;
	index = string;
	while (*index != 0) {
		if (ui->get_send_ready(ui)) {
			ui->send(ui, *index);
			index++;
		}	
	}
}


u8_t ascii_to_hex(u8_t ascii)
{
	u8_t hex;
	if ((ascii >= '0') && (ascii <= '9')) {
		hex = ascii - '0';
	} else if ((ascii >= 'a') && (ascii <= 'f')) {
		hex = ascii - 'a' + 10;
	} else if ((ascii >= 'A') && (ascii <= 'F')) {
		hex = ascii - 'A' + 10;
	} else hex = 0;
	return hex;
}

u8_t hex_to_ascii(u8_t hex)
{
	hex = hex & 0x0F;
	switch (hex) {
		case 0: case 1: case 2: case 3: case 4: case 5:
		case 6: case 7: case 8: case 9:
			return hex + '0';
			break;
		case 10: case 11: case 12: case 13: case 14: case 15:
			return hex - 10 + 'A';
			break;
		default: return '0';
	}		
}
