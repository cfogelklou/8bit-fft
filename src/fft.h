#ifndef FFT___H
#define FFT___H


#define MAXFFTSIZE 512
#define FFTTABLESIZE MAXFFTSIZE/2

#include <stdint.h>

typedef uint8_t u8_t;
typedef int8_t s8_t;
typedef uint16_t u16_t;
typedef int16_t s16_t;
typedef uint32_t u32_t;
typedef int32_t s32_t;

typedef struct {
  s16_t real;
  s16_t imag;
} complex16, c16_t, fft_type;

typedef struct {
  s32_t real;
  s32_t imag;
} complex32, c32_t;


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
