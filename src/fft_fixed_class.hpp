/******************************************************************************
Copyright 2014 Chris Fogelklou, Applaud Apps (Applicaudia)

This source code may under NO circumstances be distributed without the
express written permission of the author.

@author: chris.fogelklou@gmail.com
*******************************************************************************/
#ifndef FFTFIXEDCLASS_HPP
#define FFTFIXEDCLASS_HPP
#include <string.h>
#include <math.h>

#include "audiolib_types.h"
#include "FftClass.hpp"
#include "DaTunerApi.h"
#include "fft_c.h"
#include "audutils_debug.h"
#include "pcm_q.h"
#include "Platform.h"
  

template <typename TInFloat, typename TOutFloat, typename TAccInt, typename TSineInt>
class FftFixedClass {

private:
  //static const int FFT_INPUT_BITS = 32;

  // / Stores the current size of the FFT
  int mFftSize;

  // / Stores the number of bits needed for fftSize
  int mBitsNeeded;

  // / Allows the input arrays to be moved bit-reversed into pWorkingBuf
  int mReverseBitsLut[E_FFTSIZE_MAX + 4];

  // / Lookup table for sine values
  TSineInt mSinLut[(E_FFTSIZE_MAX/2) + 4];

  // / Internal buffers for storing real and imaginary results
  TAccInt mRealOut[E_FFTSIZE_MAX + 4];
  TAccInt mImagOut[E_FFTSIZE_MAX + 4];

  int mNumSineBits;//     (22)
  int mNumFftBits;//  (11)
  int mPcmInputBits;// = 16;
  int mExtraResolutionBits;
  TAccInt mExtraResolutionRound;
  TAccInt mSineRound;// = ((TAccInt)1u) << (mNumSineBits - 1);

  // ---------------------------------------------------------------------------
  void calculateConstants() {
    const int bitsPerChar = 8;
  
    int nAccBits = sizeof(TAccInt)*bitsPerChar - 2;
    mNumFftBits = fft_IntegerLog2(mFftSize);
    mPcmInputBits = 16;
    mNumSineBits = nAccBits - mNumFftBits - mPcmInputBits;
    mExtraResolutionBits = mNumSineBits - 20;
    mExtraResolutionBits = MAX(0, mExtraResolutionBits);

    mNumSineBits -= mExtraResolutionBits;
    mSineRound = ((TAccInt)1u) << (mNumSineBits - 1);
    if (mExtraResolutionBits > 1 ) {
      mExtraResolutionRound = ((TAccInt)1u) << (mExtraResolutionBits - 1);
    }
  }
  

  // static const double MREQ_PI = Math.PI;//3.14159265358979323846264338327;
  // static const double MREQ_TWO_PI = 2.0 * MREQ_PI;
public:
  // ---------------------------------------------------------------------------
  // Initializes or re-initializes the FFT.
  FftFixedClass(int nFftSize) {
    bool status = false;


    status = (FFT_IsPowerOfTwo(nFftSize) != 0);
    if (status) {
      status = setFftSize(nFftSize);
    }

    ASSERT( (1u << mNumFftBits )  <=  E_FFTSIZE_MAX );

  }

  // ---------------------------------------------------------------------------
  // Initializes or re-initializes the FFT.
  bool setFftSize(int nFftSize) {
    bool rval = false;
    uint_t bn;
    fft_NumberOfBitsNeeded(nFftSize, &bn);
    if ((FFT_IsPowerOfTwo(nFftSize)) && (0 != bn) && (nFftSize != mFftSize)) {
      int i;
      mFftSize = nFftSize;
      calculateConstants();

      mBitsNeeded = bn;
      // Create a bit-reversed table
      for (i = 0; i < mFftSize; i++) {
        mReverseBitsLut[i] = fft_ReverseBits(i, mBitsNeeded);
      }
      initTwiddles();
      rval = true;
    }
    return rval;
  }

  // ---------------------------------------------------------------------------
  // Initializes or re-initializes the FFT.
  void deInit() {}


public:
  // ---------------------------------------------------------------------------
  bool doFft(int16_t adRealIn[], int16_t adImagIn[], TAccInt xr[], TAccInt xi[]) {

    bool rval =
        doFftOrIfft(false, adRealIn, adImagIn, mRealOut, mImagOut, mFftSize);
    if (rval) {
      int i;
      // Copy the clipped samples into pRealOut and pImagOut
      for (i = 0; i < mFftSize; i++) {
        xr[i] = mRealOut[i];
        xi[i] = mImagOut[i];
      }
    }
    return rval;
  }

  // ---------------------------------------------------------------------------
  bool doFft(int16_t adRealIn[], int16_t adImagIn[]) {

    bool rval =
        doFftOrIfft(false, adRealIn, adImagIn, mRealOut, mImagOut, mFftSize);
    return rval;
  }


private:
  // ---------------------------------------------------------------------------
  inline TSineInt getSin(const int tblIdx) {
    const int tblIdxMinusFftSizeOver2 = tblIdx - (mFftSize / 2);
    return (tblIdxMinusFftSizeOver2 > 0) ? -mSinLut[tblIdxMinusFftSizeOver2] : mSinLut[tblIdx];
  }

  // ---------------------------------------------------------------------------
  inline TSineInt getCos(const int tblIdx) {
    const int newIdx = (tblIdx - (mFftSize / 4)) & (mFftSize - 1);
    return getSin(newIdx);
  }

  // ---------------------------------------------------------------------------

  bool initTwiddles() {
    const double fSize = (double)mFftSize;
    const double fDenom = -MREQ_TWO_PI / fSize;
    const double fShiftLeft = (1u << mNumSineBits);
    for (int i = 0; i < (mFftSize / 2); i++) {
      mSinLut[i] = (TSineInt)ROUND(fShiftLeft * sin((double)i * fDenom));
    }
    return true;
  }

public:
  // ---------------------------------------------------------------------------
  bool doFftOrIfft(bool bInverseTransform, int16_t pAdRealIn[], int16_t pAdImagIn[],
                   TAccInt xAccR[], TAccInt xAccI[], int nNumSamples) {
    int i;
    ASSERT((nNumSamples == mFftSize) && (pAdRealIn != NULL) && (xAccR != NULL) &&
           (xAccI != NULL));

    // Reverse ordering of samples so FFT can be done in place.
    if (pAdImagIn == NULL) {
      for (i = 0; i < nNumSamples; i++) {
        int rev = mReverseBitsLut[i];
        xAccR[rev] = (int)pAdRealIn[i];
        xAccI[rev] = 0;
      }
    } else {
      for (i = 0; i < nNumSamples; i++) {
        int rev = mReverseBitsLut[i];
        xAccR[rev] = (int)pAdRealIn[i];
        xAccI[rev] = (int)pAdImagIn[i];
      }
    }

    int P;
    int p = mBitsNeeded;
    int Bp = 1 << (p - 1);
    int Np = 2;
    int twiddleMul = (mFftSize >> 1);
    const int round = 1 << (mNumSineBits - 1);

    for (P = 0; P < p; P++) {
      int b;
      int BaseT = 0;
      int Npp = Np >> 1;
      for (b = 0; b < Bp; b++) {
        int k_;
        int BaseB = BaseT + Npp;
        for (k_ = 0; k_ < Npp; k_++) {
          TAccInt tmp;
          int j = BaseT + k_;
          int k = BaseB + k_;
          TAccInt xreal = xAccR[k];
          TAccInt ximag = xAccI[k];

          int tblIdx = k_ * twiddleMul;
          TSineInt twr = getCos(tblIdx);
          TSineInt twi = (bInverseTransform) ? (int16_t)-getSin(tblIdx)
                                          : (int16_t)getSin(tblIdx);

          // Calculate real part
          tmp = ((xreal * (int)twr) - (ximag * (int)twi));
          tmp = ((tmp + round) >> mNumSineBits);

          // Calculate imaginary part
          ximag = ((xreal * (int)twi) + (ximag * (int)twr));
          ximag = ((ximag + round) >> mNumSineBits);

          xreal = tmp;

          xAccR[k] = xAccR[j] - xreal;
          xAccI[k] = xAccI[j] - ximag;

          xAccR[j] = xAccR[j] + xreal;
          xAccI[j] = xAccI[j] + ximag;
        }
        BaseT += Np;
      }
      Bp = Bp >> 1;
      Np = Np << 1;
      twiddleMul >>= 1;
    }

    // Normalize
    if (bInverseTransform) {
      // TODO: Inversasize
      // double dDenom = 1.0 / (double)nNumSamples;
      // for ( i=0; i < nNumSamples; i++ )
      // {
      // x.real[i] *= dDenom;
      // xAccI[i] *= dDenom;
      // }
    }
    return true;
  }

  // ---------------------------------------------------------------------------
  
  void doRealForwardFft(
    const int16_t * const pAdRealIn, TInFloat * const pfAdRealIn, // Choose 1
		  TAccInt xAccR[], TAccInt xAccI[],
      TOutFloat pfxr[], TOutFloat pfxi[]) {
    int i;
    xAccR = (xAccR == NULL) ? mRealOut : xAccR;
    xAccI = (xAccI == NULL) ? mImagOut : xAccI;
    ASSERT((mFftSize) && ((pAdRealIn) || (pfAdRealIn)) && (xAccR) && (xAccI));
    if (pfAdRealIn) {
      const TInFloat scaleup = ((TInFloat)32767.0)*(((TAccInt)1u) << mExtraResolutionBits);
  	  // Reverse ordering of samples so FFT can be done in place.
      for (i = 0; i < mFftSize; i++) {
  	    const int rev = mReverseBitsLut[i];
        xAccR[rev] = (TAccInt)(scaleup*pfAdRealIn[i]);
      }
    }
    else {
	  // Reverse ordering of samples so FFT can be done in place.
	    for (i = 0; i < mFftSize; i++) {
	      const int rev = mReverseBitsLut[i];
        xAccR[rev] = ((TAccInt)pAdRealIn[i]) << mExtraResolutionBits;
      }
    }
    memset(xAccI, 0, mFftSize*sizeof(TAccInt));

    int p = mBitsNeeded;
    int Bp = 1 << (p - 1);
    int Np = 2;
    int twiddleMul = (mFftSize >> 1);
    

    {
      int BaseT = 0;
      int Npp = Np >> 1;
      for (int b = 0; b < Bp; b++) {
        int BaseB = BaseT + Npp;
        for (int k_ = 0; k_ < Npp; k_++) {
          const int j = BaseT + k_;
          const int k = BaseB + k_;
          TAccInt xreal = xAccR[k];
          TAccInt ximag = xAccI[k];

          const int tblIdx = k_ * twiddleMul;
          const TSineInt twr = getCos(tblIdx);
          const TSineInt twi = getSin(tblIdx);

          // Calculate real part
          TAccInt tmp = ((xreal * (TAccInt)twr));
          tmp = ((tmp + mSineRound) >> mNumSineBits);

          // Calculate imaginary part
          ximag = ((xreal * (TAccInt)twi));
          ximag = ((ximag + mSineRound) >> mNumSineBits);

          xreal = tmp;

          xAccR[k] = xAccR[j] - xreal;
          xAccI[k] = xAccI[j] - ximag;

          xAccR[j] = xAccR[j] + xreal;
          xAccI[j] = xAccI[j] + ximag;
        }
        BaseT += Np;
      }
      Bp = Bp >> 1;
      Np = Np << 1;
      twiddleMul >>= 1;
    }
    for (int P = 1; P < p; P++) {
      int BaseT = 0;
      int Npp = Np >> 1;
      for (int b = 0; b < Bp; b++) {
        int BaseB = BaseT + Npp;
        for (int k_ = 0; k_ < Npp; k_++) {
          
          const int j = BaseT + k_;
          const int k = BaseB + k_;
          TAccInt xreal = xAccR[k];
          TAccInt ximag = xAccI[k];

          const int tblIdx = k_ * twiddleMul;
          const TSineInt twr = getCos(tblIdx);
          const TSineInt twi = getSin(tblIdx);

          // Calculate real part
          TAccInt tmp = ((xreal * (TAccInt)twr) - (ximag * (TAccInt)twi));
          tmp = ((tmp + mSineRound) >> mNumSineBits);

          // Calculate imaginary part
          ximag = ((xreal * (TAccInt)twi) + (ximag * (TAccInt)twr));
          ximag = ((ximag + mSineRound) >> mNumSineBits);

          xreal = tmp;

          xAccR[k] = xAccR[j] - xreal;
          xAccI[k] = xAccI[j] - ximag;

          xAccR[j] = xAccR[j] + xreal;
          xAccI[j] = xAccI[j] + ximag;
        }
        BaseT += Np;
      }
      Bp = Bp >> 1;
      Np = Np << 1;
      twiddleMul >>= 1;
    }

    if ((pfxr) && (pfxi)) {
      TOutFloat div = (TOutFloat)(1.0 / ((double)(((TAccInt)1u) << mExtraResolutionBits)));
      if (pfAdRealIn) {
        div /= 32768.0;
      }
      const TOutFloat scale = (TOutFloat)div;
      for (i = 0; i < mFftSize/2; i++) {
        pfxr[i] = (TOutFloat)(scale*xAccR[i]);
        pfxi[i] = (TOutFloat)(scale*xAccI[i]);
      }
    }
    else {
      for (i = 0; i < mFftSize/2; i++) {
        xAccR[i] = (xAccR[i] + mExtraResolutionRound) >> mExtraResolutionBits;
        xAccI[i] = (xAccI[i] + mExtraResolutionRound) >> mExtraResolutionBits;
      }
    }
  }

};


typedef FftFixedClass<float, float, int64_t, int32_t> FftFixed64F;
typedef FftFixedClass<float, float, int32_t, int16_t> FftFixed32F;

typedef FftFixedClass<float, double, int64_t, int32_t> FftFixed64D;
typedef FftFixedClass<float, double, int32_t, int16_t> FftFixed32D;


#endif // FFTFIXEDCLASS_HPP
