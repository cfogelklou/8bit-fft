#ifndef UART_TOOLS___H
#define UART_TOOLS___H
#include <ipOS.h>
#include <ipUART.h>

typedef struct {
	s16_t real;
	s16_t imag;
} complex16, c16_t;

typedef struct {
	s32_t real;
	s32_t imag;
} complex32, c32_t;

/* Send a single character through protocol_instance */
void csend(void *protocol_instance, u8_t in_char); 

/* Get a single character through protocol_instance */
char cget(void *protocol_instance);

/* Send a 16-bit integer through protocol_instance */
void send_u16(void *protocol_instance, u16_t a);

/* Send a 32-bit integer through protocol_instance */
void send_u32(void *protocol_instance, u32_t a);

/* Function to send an entire string via the UART passed in protocol_instance */
void uart_print(void *protocol_instance, u8_t *string);

/* Convert the character in {'a'..'f' and '0'..'9' to the digital equivalent */
u8_t ascii_to_hex(u8_t ascii);

/* Convert the 4-bit hex number in hex to the ascii equivalent*/
u8_t hex_to_ascii(u8_t hex);

void send_c16(void *protocol_instance, c16_t data);
void send_c32(void *protocol_instance, c32_t data);

u16_t get_u16(void *protocol_instance);
u32_t get_u32(void *protocol_instance);
c16_t get_c16(void *prototol_instance);
c32_t get_c32(void *protocol_instance);

void send_fft_results(void *protocol_instance, u16_t N);

#endif
