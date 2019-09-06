#ifndef FFT___H
#define FFT___H
#include <ipOS.h>
#include "user_config.h"
#include "uart_tools.h"

//#include "fft_asm_defs.h"

typedef c16_t fft_type;

u16_t add_u16_u16(u16_t a, u16_t b);
c16_t sub_c16_c16 (c16_t a, c16_t b);

u32_t mul_u16_u16(u16_t a, u16_t b);
c16_t add_c16_c16 (c16_t a, c16_t b );
c32_t add_c32_c32 (c32_t a, c32_t b);
c32_t sub_c32_c32 (c32_t a, c32_t b);
c32_t mul_c16_s16(c16_t a, s16_t b); 
c16_t mul_c16_c16(c16_t a, c16_t b);
s16_t get_sine_val (u16_t index);
c16_t shift_c32_by_15(c32_t a);
c32_t mul_c16_twiddle(c16_t a, c16_t twiddle);

c16_t get_fft_result (u16_t index);
void set_fft_result (u16_t index, fft_type data);


void fft(s16_t *x, u16_t N, void *debug_ui);

//Interfaces to assembly only functions
void initialize_fft_data(s16_t *dp, u16_t N);
s32_t mul_s16_s16(s16_t a, s16_t b);
// Assembly only functions (Can't be called directly from C)
//void as_copy_scrambled(void) __attribute__((naked));
void as_copy_scrambled(void);
s32_t as_mul_s16_s16 (void) __attribute__((naked)); 
void as_calc_bits (void) __attribute__((naked));
s16_t TwiddleFactorCosine(u16_t k, u16_t N);
s16_t TwiddleFactorSine(u16_t k, u16_t N);


#endif
