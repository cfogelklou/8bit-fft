#include "fft.h"
//#include "fft_asm_defs.h"

extern c16_t add_c16_c16 (c16_t a, c16_t b );
extern c16_t sub_c16_c16 (c16_t a, c16_t b);
extern c16_t mul_c16_c16 (c16_t a, c16_t b);
extern u16_t add_u16_u16(u16_t a, u16_t b);
extern u32_t mul_u16_u16(u16_t a, u16_t b);
extern s16_t get_sine_val (u16_t index);
extern c16_t get_fft_result (u16_t index);
extern void set_fft_result (u16_t index, c16_t data);

extern void as_copy_scrambled(void);
//extern void as_calc_log2N(void);
extern s32_t as_mul_s16_s16 (void);
//extern void as_calc_bits(void);

extern void send_fft_results(void *protocol_instance, u16_t N); 
extern void send_c32_t(void *protocol_instance, c32_t data);
extern void csend(void *protocol_instance, u8_t in_char);
extern void send_c16_t(void *protocol_instance, c16_t data);

volatile u8_t fft_dp1_lo ;
volatile u8_t fft_dp1_hi ;
volatile u8_t fft_dp2_lo ;
volatile u8_t fft_dp2_hi ;
volatile u8_t fft_temp1 ;
volatile u8_t fft_count1_l ;
volatile u8_t fft_count1_h ;
volatile u8_t fft_N_mask_l ;
volatile u8_t fft_N_mask_h ;
volatile u8_t fft_N_l ;
volatile u8_t fft_N_h ;
volatile u8_t fft_bits ;
volatile u8_t fft_index_l ;
volatile u8_t fft_index_h ;
volatile u8_t fft_a_l ;
volatile u8_t fft_a_h ;
volatile u8_t fft_b_l ;
volatile u8_t fft_b_h ;
volatile u8_t fft_sign ;

void initialize_fft_data(s16_t *dp, u16_t N)
{
	fft_dp1_hi = (u8_t)((u16_t)dp >> 8);
	fft_dp1_lo = (u8_t)((u16_t)dp >> 0);
	fft_N_h = (u8_t)(N >> 8);
	fft_N_l = (u8_t)(N);
	as_copy_scrambled();
	as_calc_bits();
}

s32_t mul_s16_s16(s16_t a, s16_t b) 
{
	fft_a_l = (u8_t)a;
	fft_a_h = (u8_t)(a>>8);	
	fft_b_l = (u8_t)b;
	fft_b_h = (u8_t)(b>>8);	
	return as_mul_s16_s16();
}

/*
c32_t mul_c16_c16(c16_t a, c16_t b) 
{
	c32_t rval;
	s32_t real, imag;
	fft_a_l = (u8_t)a.real;
	fft_a_h = (u8_t)(a.real>>8);	
	fft_b_l = (u8_t)b.real;
	fft_b_h = (u8_t)(b.real>>8);	
	real = as_mul_s16_s16();	
	fft_a_l = (u8_t)a.real;
	fft_a_h = (u8_t)(a.real>>8);	
	fft_b_l = (u8_t)b.imag;
	fft_b_h = (u8_t)(b.imag>>8);	
	imag = as_mul_s16_s16();
	fft_a_l = (u8_t)a.imag;
	fft_a_h = (u8_t)(a.imag>>8);		
	fft_b_l = (u8_t)b.imag;
	fft_b_h = (u8_t)(b.imag>>8);	
	rval.real = real - as_mul_s16_s16();	
	fft_a_l = (u8_t)a.imag;
	fft_a_h = (u8_t)(a.imag>>8);		
	fft_b_l = (u8_t)b.real;
	fft_b_h = (u8_t)(b.real>>8);	
	rval.imag = imag + as_mul_s16_s16();
	return rval;
}
*/
c32_t mul_c16_s16(c16_t a, s16_t b) 
{
	c32_t rval;
/*	fft_a_l = (u8_t)a.real;
	fft_a_h = (u8_t)(a.real>>8);	
	fft_b_l = (u8_t)b;
	fft_b_h = (u8_t)(b>>8);	
	rval.real = as_mul_s16_s16();	
	fft_a_l = (u8_t)a.imag;
	fft_a_h = (u8_t)(a.imag>>8);	
	rval.imag = as_mul_s16_s16();
*/
	rval.real = (s32_t)a.real*(s32_t)b;
	rval.imag = (s32_t)a.imag*(s32_t)b;
	return rval;
}
c32_t add_c32_c32(c32_t a, c32_t b)
{
	c32_t rval;
	rval.real = a.real + b.real;
	rval.imag = a.imag + b.imag;
	return rval;
}
c32_t sub_c32_c32(c32_t a, c32_t b)
{
	c32_t rval;
	rval.real = a.real - b.real;
	rval.imag = a.imag - b.imag;
	return rval;
}

void fft(s16_t *x, u16_t N, void *debug_ui)
{
//---------------------------------------------------------------------------
//function [f] = myfft4(N,f,twiddleQuant,bottomDiv);

    c16_t top, bot, temp_c16, twiddleFactor;
    u16_t b, k, Bp, Np, P, Npp, BaseT, BaseB;
    u8_t p;
    //% My FFT test program
    //% Written to measure the effect of quantization on the FFT algorithm.
    //%
    //f = arraybitrev(p,f);
    //p = log2(N);
    initialize_fft_data(x, N);
    p = fft_bits;
    //Bp = 2^(p-1);
    Bp = 1<<(p-1);
    //Np = 2;
    Np = 2;
    //for P=0:1:(p-1)
    for (P = 0; P < p; P++) {
        //Npp = Np/2;
        Npp = Np>>1;
        //BaseT = 0;
        BaseT = 0;
        //for b=0:1:(Bp-1)
        for (b = 0; b < Bp; b++) {
            //BaseB = BaseT + Npp;
            BaseB = BaseT + Npp;
            //for k=0:1:(Npp-1)
            for (k = 0; k < Npp; k++) {
                //top = f(BaseT + k + 1)*topMult; % left shift of multiply
                //top = f[BaseT + k]*65536.0;
                top = get_fft_result(BaseT + k);
                //twiddleFactor = round(exp(-i*2*pi*k/Np)*twiddleQuant);
                //twiddleFactor = 32768.0*fft_type(cosl(-2.0*pi*(long double)k/((long double)Np)),sinl(-2.0*pi*(long double)k/((long double)Np)));
                //twiddleFactor = fft_type(((long double)TwiddleFactorCosine(k, Np)),((long double)TwiddleFactorSine(k, Np)));
                twiddleFactor.real = TwiddleFactorCosine(k, Np);
                twiddleFactor.imag = TwiddleFactorSine(k, Np);
                //bot = round((f(BaseB + k + 1) * twiddleFactor)/bottomDiv);% 16*16 multiply
                temp_c16 = get_fft_result(BaseB + k);
                temp_c16.real = temp_c16.real<<1;
                temp_c16.imag = temp_c16.imag<<1;
                bot = mul_c16_c16(temp_c16,twiddleFactor);
                //f(BaseT + k + 1) = round((top + bot)/(topMult));
                //f[BaseT + k] = (top + bot)/(32768.0);
                temp_c16 = add_c16_c16(top, bot);
                set_fft_result(BaseT + k, temp_c16);
                //f(BaseB + k + 1) = round((top - bot)/(topMult));
                //f[BaseB + k] = (top - bot)/(32768.0);
                temp_c16 = sub_c16_c16(top, bot);
                set_fft_result(BaseB + k, temp_c16);
	        //send_fft_results(debug_ui, N);    
            //end
            }
            //BaseT = BaseT + Np;
            BaseT = BaseT + Np;
        //end
        }
        //Bp = Bp / 2;
        Bp = Bp >> 1;
        //Np = Np * 2;
        Np = Np << 1;
    //end
    }
}

s16_t TwiddleFactorCosine(u16_t k, u16_t N)
{
    u16_t newk;
    u16_t table_progress;
    newk = k ;
    table_progress = newk*(MAXFFTSIZE/N) + MAXFFTSIZE/4;
    return get_sine_val(table_progress);
}

s16_t TwiddleFactorSine(u16_t k, u16_t N)
{
    u16_t newk;
    u16_t table_progress;
    newk = k;
    table_progress = newk*(MAXFFTSIZE/N);
    return get_sine_val(table_progress);
}
