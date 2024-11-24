//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmath.h
// Description : Math system include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMATH_H_INCLUDED
#define LMATH_H_INCLUDED

#include "lmathapi.h"
#include "../io/lstream.h"
#include "../platform/lassert.h"

#include <math.h>

#if !defined(L_PLATFORM_PS2) && !defined(L_PLATFORM_PSP) && !defined(L_PLATFORM_SYMBIAN)
#include <float.h>
#endif

#if defined(L_PLATFORM_SYMBIAN)

#include <e32math.h>

//BAROG: HACK strict ansi is set by the math.h so no xxxxf functions exist so we hack them in here.

#define _finite finite
#define floorf floor
#define ceilf  ceil
#define fmodf  fmod
#define fabsf  fabs
#define sqrtf  sqrt
#define sinf   sin
#define asinf  asin
#define cosf   cos
#define powf   pow
#define acosf  acos
#define tanf   tan
#define atanf  atan
#define atan2f atan2
#define logf   log

inline float modff (float a, float *b)
{
  double bdbl;
  double ret = modf( (double)a, &bdbl );
  if( b != NULL ) *b = (float)bdbl;
  return (float)ret;
}

#endif

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Math
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
/*
  **NOTE** L_FPU_ASSUME_NEAREST_ROUNDING is DISABLED at the moment due to the following
 code:

   lLogFmt("%d\n", (int)floor( (float)7 ) );
   lLogFmt("%d\n", (int)lmFloor( (float)7 ) );

   Output:
   7
   6
*/

//#define L_FPU_ASSUME_NEAREST_ROUNDING

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Floating-point class test
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Note: These used to be after the common constant #defines below but
//       have been moved up to keep the documentation system happy.

//##
// Check if a floating pointer number is infinite.
// In:
//   x - value to check
// Out:
//   Returns true if the number is infinite, otherwise false.
//
// Todo: Isn't this the same as LM_ISFLOAT_INF?
inline bool lmIsInfinite(float x)
{
  // An infinity has an exponent of 255 (shift left 23 positions) and
  // a zero mantissa. There are two infinities - positive and negative.

  const int kInfAsInt = 0x7F800000;

  if ((*(int*)&x & 0x7FFFFFFF) == kInfAsInt)
  {
    return true;
  }
  return false;
}

//##
// Check if a floating pointer number is NaN.
// In:
//   x - value to check
// Out:
//   Returns true if the number is NaN, otherwise false.
//
// Todo: Isn't this the same as LM_ISFLOAT_NAN?
inline bool lmIsNan(float x)
{
  // A NAN has an exponent of 255 (shifted left 23 positions) and
  // a non-zero mantissa.

  int exp = *(int*)&x & 0x7F800000;
  int mantissa = *(int*)&x & 0x007FFFFF;
  if (exp == 0x7F800000 && mantissa != 0)
  {
    return true;
  }

  return false;
}

//##
// Check if a floating pointer number is finite.
// In:
//   x - value to check
// Out:
//   Returns true if the number is finite, otherwise false.
//
// Todo: Isn't this related to the LM_ISFLOAT_xxx macros?
inline bool lmIsFinite(float x)
{
  // TODO: PS2 / PSP doesnt have NANs just very big (or very small) numbers - check for 0x7FFFFFFF or 0xFFFFFFFF ?
#if defined(L_PLATFORM_PS2) || defined(L_PLATFORM_PSP)
  return true;
#elif defined(L_PLATFORM_NDS) || defined(L_PLATFORM_WII)
  // Check for infinite / nans
  return !(lmIsInfinite(x) || lmIsNan(x));
#else
  return (bool)(_finite(x) > 0);
#endif
}

//##
// Get the sign of a floating point number
// In:
//   x - value to obtain the sign of
// Out:
//   Returns 1 if the sign bit is set, otherwise return 0.
//
inline int lmSign(float x)
{
  return (*(int*)&x) & 0x80000000;
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Common math constants rounded to 21 decimals.
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// Logarithm functions

#define LM_E                               2.71828182845904523536f     //## e
#define LM_LOG2E                           1.44269504088896340736f     //## log(e)
#define LM_LOG10E                          0.434294481903251827651f    //## log10(e)
#define LM_LN2                             0.693147180559945309417f    //## ln(2)
#define LM_LN10                            2.30258509299404568402f     //## ln(10)
#define LM_ILOG2                           3.32192809488736234787f     //## 1.0f / log10( 2 )

// PI and SQRT

#define LM_PI                              3.141592653589793238463f    //## PI
#define LM_2PI                             6.283185307179586476925f    //## 2 * PI
#define LM_PI_2                            1.57079632679489661923f     //## 1/2 PI
#define LM_PI_4                            0.785398163397448309616f    //## 1/4 PI
#define LM_1_PI                            0.318309886183790671538f    //## 1/PI
#define LM_2_PI                            0.636619772367581343076f    //## 2/PI
#define LM_1_SQRTPI                        0.564189583547756286948f    //## 1/sqrt(PI)
#define LM_2_SQRTPI                        1.12837916709551257390f     //## 2/sqrt(PI)
#define LM_SQRT2                           1.414213562373095048802f    //## sqrt of 2
#define LM_SQRT3                           1.732050807568877293527f    //## sqrt of 2
#define LM_1_SQRT2                         0.707106781186547524401f    //## 1/sqrt of 2
#define LM_SQRT1_2                         1.414213562373095048802f    //## sqrt of (1/2)

// More precise versions for double

#define LM_PI_DOUBLE                       3.141592653589793238462643383279502884197169399375    //## PI

// Radian/Degree ratios

#define LM_DEGREE                          57.295779513082320876798f   //## 180/PI (rad to deg)
#define LM_RADIAN                          0.017453292519943295769f    //## PI/180 (deg to rad)

// Integer min / max ranges

#define LM_INT8_MAX      0x7f              //## max int8 value
#define LM_INT8_MIN      0x80              //## min int8 value
#define LM_UINT8_MAX     0xff              //## max uint8 value
#define LM_UINT8_MIN     0x00              //## min uint8 value

#define LM_INT16_MAX      0x7fff            //## max int16 value
#define LM_INT16_MIN      0x8000            //## min int16 value
#define LM_UINT16_MAX     0xffff            //## max uint16 value
#define LM_UINT16_MIN     0x0000            //## min uint16 value

#define LM_INT32_MAX      0x7fffffff        //## max int32 value
#define LM_INT32_MIN      0x80000000        //## min int32 value
#define LM_UINT32_MAX     0xffffffff        //## max uint32 value
#define LM_UINT32_MIN     0x00000000        //## min uint32 value

// Floating point min/max and epsilon comparisons

#define LM_EPSILON                         1e-4f                      //## Genereal epsilon value
#define LM_SQR_EPSILON                     (LM_EPSILON*LM_EPSILON)    //## LM_EPSILON squared
#define LM_ONE_MINUS_EPSILON               (1.0f-LM_EPSILON)
#define LM_ONE_PLUS_EPSILON                (1.0f+LM_EPSILON)

#define LM_FLOAT_1_0_HEX                   0x3f800000  //## hex representation of 1.0
#define LM_FLOAT_255_0_HEX                 0x437f0000  //## hex representation of 255.0

#define LM_FLOAT_MAX                       3.402823466e+38f           //## Max float number
#define LM_FLOAT_MIN                       1.175494351e-38f           //## Min float number
#define LM_FLOAT_EPSILON                   1.192092896e-7f            // Fpu float epsilon

#define LM_DOUBLE_MAX                      1.79769313486231500e+308   //## Max double number
#define LM_DOUBLE_MIN                      2.22507385850720200e-308   //## Min double number
#define LM_DOUBLE_EPSILON                  2.22044604925031300e-016   //## Fpu double epsilon

// Fast float conversions to integer fixed point.
// 2^N * 1.5. Where N is (52-fixed_point_shift)

#define LM_DOUBLE2INT                       0x43380000      //## 2^52 * 1.5 (double, with upper 32 bits zero)
#define LM_DOUBLE2FIX                       0x42380000      //## 2^36 * 1.5 (double, with upper 32 bits zero)
#define LM_FLOAT2INT                        0x59c00000      //## 2^52 * 1.5 (float)
#define LM_FLOAT2FIX                        0x51c00000      //## 2^36 * 1.5 (float)

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// IEEE
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define LM_FLOAT_SIGN_BIT         31      //## Bit position of the sign bit on a 32-bit floating point number
#define LM_FLOAT_MANTISSA_BITS    23      //## Bits for the mantissa on a 32-bit floating point number
#define LM_FLOAT_EXPONENT_BITS    8       //## Bits for the exponent on a 32-bit floating point number
#define LM_FLOAT_EXPONENT_BIAS    127     //## Exponent bias on a 32-bit floating point number

#define LM_DOUBLE_SIGN_BIT        63      //## Bit position of the sign bit on a 64-bit floating point number
#define LM_DOUBLE_MANTISSA_BITS   52      //## Bits for the mantissa on a 64-bit floating point number
#define LM_DOUBLE_EXPONENT_BITS   11      //## Bits for the exponent on a 64-bit floating point number
#define LM_DOUBLE_EXPONENT_BIAS   1023    //## Exponent bias on a 64-bit floating point number

#define LM_ISFLOAT_NAN(x)       (((*(const unsigned int *)&x) & 0x7f800000) == 0x7f800000)  //## Check if a floating point number is NaN
#define LM_ISFLOAT_INF(x)       (((*(const unsigned int *)&x) & 0x7fffffff) == 0x7f800000)  //## Check if a floating point number is INF
#define LM_ISFLOAT_IND(x)       ((*(const unsigned int *)&x) == 0xffc00000)                 //## Check if a floating point number is IND
#define LM_ISFLOAT_DENORMAL(x)  (((*(const unsigned int *)&x) & 0x7f800000) == 0 && ((*(const unsigned int *)&x) & 0x007fffff) != 0 ) //## Check if a floating point number is denormal

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Template macros
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Templated square (x * x) routine
template<class T> inline const T lmSq(const T& x)                                     { return x*x; }

//##
// Templated absolute routine
template<class T> inline const T lmAbs(const T& x)                                    { return (x>0) ? (x) : (-x); }

//##
// Templated minimum routine
template<class T> inline const T lmMin(const T& a,const T& b)                         { return (a<b) ? (a) : (b); }
template<class T> inline const T lmMin3(const T& a,const T& b,const T& c)             { return ((c < ((a < b) ? a : b)) ? c : ((a < b) ? a : b)); }

//##
// Templated maximum routine
template<class T> inline const T lmMax(const T& a,const T& b)                         { return (a>b) ? (a) : (b); }
template<class T> inline const T lmMax3(const T& a,const T& b,const T&c)              { return ((c > ((a > b) ? a : b)) ? c : ((a > b) ? a : b)); }

//##
// Templated minimum + maximum routine
// In:
//   a - 1st value to compare
//   b - 2nd value to compare
//   min - Value to set to the smallest one of a+b
//   max - Value to set to the largest one of a+b
template<class T> inline void lmMinMax(const T& a,const T& b, T&min, T&max)           { if(a>b) { min = b; max = a; } else { min = a; max = b; } }

//##
// Templated clamp routine
// In:
//   x - value to clamp
//   lo - low to clamp to
//   hi - high to clamp to
template<class T> inline const T lmClamp(const T& x,const T& lo,const T& hi)          { return (x<lo) ? (lo) : ((x>hi) ? hi : x); }

//##
// Convert degrees to radians
inline const float lmDeg2Rad(const float x)                                           { return x * LM_RADIAN; }
//##
// Convert radians to degrees
inline const float lmRad2Deg(const float x)                                           { return x * LM_DEGREE; }

//##
// Convert degrees to radians
inline const double lmDeg2Rad(const double x)                                         { return x * double(LM_RADIAN); }
//##
// Convert radians to degrees
inline const double lmRad2Deg(const double x)                                         { return x * double(LM_DEGREE); }

//##
// Find square root of number
inline float lmSqrt(float num);

//##
// Find square root of number (optimised for values near 1.0)
inline const float lmFSqrt(const float x)                                         { return (x>0.9996f && x<1.001f)?(0.5f+0.5f*x) : lmSqrt(x); }

//##
// Find square root of number
inline double lmSqrt(double num);
//##
// Find square root of number (optimised for values near 1.0)
inline const double lmFSqrt(const double x)                                       { return (x>0.9996 && x<1.001)?(0.5+0.5*x) : lmSqrt(x); }

// Epsilon comparisons

//##
// Check if values are almost equal based on epsilon
template<class T> bool lmAlmostEqual(const T& a,const T& b)                 { return (a >= b-T(LM_EPSILON) && a<=b+T(LM_EPSILON)); }
template<class T> bool lmAlmostEqualTo(const T& a,const T& b,const T& tol)  { return (a >= b-tol && a<=(b+tol)); }

//##
// Linear interpolate on alpha, (a*v1) + (1-a)*v2
template<class TA,class T> inline const T lmLerp(const TA& a,const T& v1,const T& v2)  { return v1+((v2-v1)*a); }

//##
// Cosine-linear interpolate on alpha
template<class TA,class T> inline const T lmCLerp(const TA& a,const T& v1,const T& v2) { return lmLerp<TA,T>( ((T(1.0)-lmCos(a*T(LM_PI)))*T(0.5)), v1, v2 ); }

//##
// Round the value to the nearest whole integer
// In:
//   x - value to round
// Out:
//   Returns the value rounded to the nearest whole integer
template<class T> inline const T lmRound(const T x)                                 { return x > T(0) ? lmFloor( x + T(0.5) ) : -lmFloor( T(0.5) - x ); }


//TODO:
// Sort out lmRoundTo .. what's it supposed to do again?
// Round to the specified integer multiple
//template<class T> inline const T lmRoundTo(const T x,const int y)                    { return ((int)(x*(int)y)) / y; }

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Epsilon comparisons
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define LM_EPSILON_INFINITYCHECK
#define LM_EPSILON_NANCHECK
#define LM_EPSILON_SIGNCHECK

//##
// Check if two floating point numbers are almost equal based on the number of units-in-last-place.
// In:
//   fA      - first float to compare
//   fB      - second float to compare
//   maxUlps - maximum units-in-last-place between the floats.
// Out:
//   Returns true if the two values are equal within the units-in-last-place. Otherwise returns false.
//
// Remarks:
// Check if two floating point numbers are almost equal based on the number of units-in-last-place.
// <p>Allow maxUlps-1 floats between A and B.
// <p>For 32 bit floats, one ULPs implies a relative error of between 1/8,000,000 and 1/16,000,000.
//
// This routine behaves in the follow mannor.
// * Measures whether two floats are close to each other, where close is defined by ulps,
//   also interpreted as how many floats there are in-between the numbers.
// * Treats NANs as being four million ulps away from everything (assuming the default NAN value for x87),
//   except other NANs.
// * Accepts greater relative error as numbers gradually underflow to subnormals
// * Treats infinity as being close to LM_FLOAT_MAX, unless LM_EPSILON_INFINITYCHECK is defined.
// * Treats tiny negative numbers as being close to tiny positive numbers, unless LM_EPSILON_SIGNCHECK is defined.
//
// Reference:
// http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
//
inline bool lmAlmostEqualUlps(float fA, float fB, int maxUlps)
{
  // Make sure maxUlps is non-negative
  lAssert(maxUlps > 0);

  #ifdef LM_EPSILON_INFINITYCHECK
  // If fA or fB are infinity (positive or negative) then
  // only return true if they are exactly equal to each other -
  // that is, if they are both infinities of the same sign.
  // This check is only needed if you will be generating
  // infinities and you don't want them 'close' to numbers
  // near LM_FLOAT_MAX.
  if( lmIsInfinite(fA) || lmIsInfinite(fB) )
      return (fA == fB);
  #endif

  #ifdef LM_EPSILON_NANCHECK
  // If fA or fB are a NAN, return false. NANs are equal to nothing, not even themselves.
  // This check is only needed if you will be generating NANs and you use a maxUlps
  // greater than 4 million or you want to ensure that a NAN does not equal itself.
  if( lmIsNan(fA) || lmIsNan(fB) )
      return false;
  #else
  // Make sure maxUlps is small enough that the default NAN won't compare as equal to anything.
  lAssert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
  #endif

  #ifdef LM_EPSILON_SIGNCHECK
  // After adjusting floats so their representations are lexicographically
  // ordered as twos-complement integers a very small positive number
  // will compare as 'close' to a very small negative number. If this is
  // not desireable, and if you are on a platform that supports
  // subnormals (which is the only place the problem can show up) then
  // you need this check.
  // The check for fA == fB is because zero and negative zero have different
  // signs but are equal to each other.
  if (lmSign(fA) != lmSign(fB))
      return (fA == fB);
  #endif

  // Make aInt lexicographically ordered as a twos-complement int

  int aInt = *(int*)&fA;
  if (aInt < 0)
  {
    aInt = 0x80000000 - aInt;
  }

  // Make bInt lexicographically ordered as a twos-complement int

  int bInt = *(int*)&fB;
  if (bInt < 0)
  {
    bInt = 0x80000000 - bInt;
  }

  // Now we can compare aInt and bInt to find out how far apart fA and fB are.

  int intDiff = abs(aInt - bInt);

  if (intDiff <= maxUlps)
  {
    return true;
  }

  return false;
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Bit routines
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Integer base-2 logarithm of a float
inline int lmIntLog2( float num )
{
  int v = *reinterpret_cast<int *>(&num);
  return ( (v>>LM_FLOAT_MANTISSA_BITS) & ((1<<LM_FLOAT_EXPONENT_BITS)-1) ) - LM_FLOAT_EXPONENT_BIAS;
}

//##
// Integer base-2 logarithm of an integer
inline int lmIntLog2( int num )
{
  return lmIntLog2( static_cast<float>(num) );
}

//##
// Returns the number of bits needed to represent ceil(f)
inline int lmBitsForFloat( float f )
{
  return lmIntLog2( f ) + 1;
}

//##
// Returns the number of bits needed to represent i
inline int lmBitsForInt( int i )
{
  return lmIntLog2( (float)i ) + 1;
}

//##
// Returns true if x is a power of 2
inline bool lmIsPowerOfTwo( int x )
{
  return ( x & ( x - 1 ) ) == 0 && x > 0;
}

//##
// Round x up to the nearest power of two
inline int lmCeilPowerOfTwo( int x )
{
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x++;
	return x;
}

//##
// Round x down to the nearest power of two
inline int lmFloorPowerOfTwo( int x )
{
  return lmCeilPowerOfTwo( x ) >> 1;
}

//##
// Convert the specified exponent and mantissa bits of the packed integer into a floating point value
inline float lmBitsToFloat( int i, int exponentBits, int mantissaBits )
{
  /*static*/ int exponentSign[2] = { 1, -1 };
  //TODO: we actually want that to be static. but gcc 2.95 complains.

	int sign, exponent, mantissa, value;

  lAssert( exponentBits >= 2 && exponentBits <= 8 );
  lAssert( mantissaBits >= 2 && mantissaBits <= 23 );

	exponentBits--;
	sign = i >> ( 1 + exponentBits + mantissaBits );
  exponent = ( (i>>mantissaBits) & ((1<<exponentBits)-1) ) * exponentSign[(i>>(exponentBits+mantissaBits))&1];
  mantissa = ( i&((1<<mantissaBits)-1) ) << (LM_FLOAT_MANTISSA_BITS-mantissaBits);
  value = sign << LM_FLOAT_SIGN_BIT | (exponent+LM_FLOAT_EXPONENT_BIAS)<<LM_FLOAT_MANTISSA_BITS|mantissa;
	return *reinterpret_cast<float *>(&value);
}

//##
// Convert the floating point number into the specified number of exponent and mantissa bits
// and return the result as a packed integer.
inline int lmFloatToBits( float f, int exponentBits, int mantissaBits )
{
	int i, sign, exponent, mantissa, value;

  lAssert( exponentBits >= 2 && exponentBits <= 8 );
  lAssert( mantissaBits >= 2 && mantissaBits <= 23 );

	int maxBits = ( ( ( 1 << ( exponentBits - 1 ) ) - 1 ) << mantissaBits ) | ( ( 1 << mantissaBits ) - 1 );
	int minBits = ( ( ( 1 <<   exponentBits       ) - 2 ) << mantissaBits ) | 1;

  float max = lmBitsToFloat( maxBits, exponentBits, mantissaBits );
  float min = lmBitsToFloat( minBits, exponentBits, mantissaBits );

  if ( f >= 0.0f )
  {
    if ( f >= max )
    {
			return maxBits;
    }
    else if ( f <= min )
    {
			return minBits;
		}
  }
  else
  {
    if ( f <= -max )
    {
			return ( maxBits | ( 1 << ( exponentBits + mantissaBits ) ) );
    }
    else if ( f >= -min )
    {
			return ( minBits | ( 1 << ( exponentBits + mantissaBits ) ) );
		}
	}

	exponentBits--;
	i = *reinterpret_cast<int *>(&f);
  sign = (i>>LM_FLOAT_SIGN_BIT) & 1;
  exponent = ( (i>>LM_FLOAT_MANTISSA_BITS) & ((1<<LM_FLOAT_EXPONENT_BITS)-1) ) - LM_FLOAT_EXPONENT_BIAS;
  mantissa = i & ( (1<<LM_FLOAT_MANTISSA_BITS)-1 );
  value = sign << (1+exponentBits+ mantissaBits);
  value |= ( (((unsigned int)(exponent) >> 31) << exponentBits) | (lmAbs(exponent)&((1<<exponentBits)-1))) << mantissaBits;
  value |= mantissa >> (LM_FLOAT_MANTISSA_BITS-mantissaBits);
	return value;
}



//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// General math routines
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

LMATH_API extern int lmSqrt5Table[];
LMATH_API extern int lmSqrt5ExpTable[];
LMATH_API extern int lmOOSqrt5Table[];
LMATH_API extern int lmOOSqrt5ExpTable[];

//##
// Square root accurate to 5 decimal places (using table look ups)
inline float lmSqrt5(float num)
{
  unsigned int  index1 = (((*((unsigned int  *)&num)) & 0xffffff) ^ 0x800000) >> 12;
  unsigned int  index2 = ((*((unsigned int  *)&num)) >> 23);
  unsigned int  r      = (lmSqrt5Table[index1]|lmSqrt5ExpTable[index2]);
  return *((float *)&r);
}

//##
// 1/sqrt accurate to 5 decimal places (using table look ups)
inline float lmOOSqrt5(float num)
{
  unsigned int  p1     = lmOOSqrt5Table[(((*((unsigned int  *)&num)) & 0xffffff) ^ 0x800000) >> 12];
  unsigned int  p2     = lmOOSqrt5ExpTable[((*((unsigned int  *)&num)) >> 23)];
  if( p1 == 0 ) p2+=0x800000;
  unsigned int  r      = (p1|p2);
  return *((float *)&r);
}

//##
// 1/sqrt using actual sqrt
inline float lmOOSqrt(float num)
{
  return 1.0f/lmSqrt(num);
}

//##
// Approximate 1/sqrt
inline float lmOOSqrtApprox( float num )
{
  uint32 i;
  float x2,threehalfs = 1.5f;
  float y;

  y  = num;
  i  = *(uint32 *) &y;
  i  = 0x5f3759df - (i>>1);
  y  = *(float *)&i;
  x2 = num * 0.5f;
  y  = y * (threehalfs - (x2 * y * y));
  y  = y * (threehalfs - (x2 * y * y));
  return y;
}

//##
// Sin / Cos lookup table
// Todo: Document the construction of this table, and link it from
// lmSinLU, lmSinCosLU and lmCosLU.
LMATH_API extern float lmSinTable[];

typedef union
{
  uint32  i;
  float f;
} INTORFLOAT;

#define FTOIBIAS    12582912.0f
#define DIV256BY2PI 40.743665431525205956834243423364f  // 256.0 / LM_2PI

//##
// sine (lookup table version)
inline float lmSinLU(float theta)
{
  INTORFLOAT ftmp;
  ftmp.f = theta * DIV256BY2PI + FTOIBIAS;  // +64.0f for cos
  return lmSinTable[ftmp.i & 255];
}

//##
// cosine (lookup table version)
inline float lmCosLU(float theta)
{
  INTORFLOAT ftmp;
  ftmp.f = theta * DIV256BY2PI + FTOIBIAS + 64.0f;
  return lmSinTable[ftmp.i & 255];
}

//##
// sine/cosine (lookup table version)
inline void lmSinCosLU(float theta, float &sinV, float &cosV)
{
  INTORFLOAT ftmp;
  ftmp.f = theta * DIV256BY2PI + FTOIBIAS;  // +64.0f for cos
  sinV = lmSinTable[ftmp.i & 255];
  cosV = lmSinTable[(ftmp.i+64) & 255];
}

#undef FTOIBIAS
#undef DIV256BY2PI

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Double precision routines - requires a double precision FPU
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Convert a floating point number an integer.
// Rounds the value down.
inline int lmDoubleToInt(double f)
{
  //#ifdef L_FPU_ASSUME_NEAREST_ROUNDING
  //unsigned int DoubleToIntConst[2] = { 0, LM_DOUBLE2INT };
  //double temp = ((f-0.5)+(*(double*)DoubleToIntConst));
  //return *(int *)&temp;
  //#else
  return static_cast<int>(f);
  //#endif
}

//##
// Convert a floating point number to 16.16 fixed point
// This method always rounds the value down
inline int lmDoubleToFixed1616(double f)
{
  //#ifdef L_FPU_ASSUME_NEAREST_ROUNDING
  //unsigned int DoubleToFixedConst[2] = { 0, LM_DOUBLE2FIX };
  //double temp = (f+(*(double*)DoubleToFixedConst));
  //return *(int *)&temp;
  //#else
  return static_cast<int>(f * 65536.0f);
  //#endif
}

//##
// Round a value down to the nearest integer
inline double lmFloor(double f)
{
  //#ifdef L_FPU_ASSUME_NEAREST_ROUNDING
  //unsigned int DoubleToIntConst[2] = { 0, LM_DOUBLE2INT };
  //double temp = ((f-0.5)+(*(double*)DoubleToIntConst));
  //return *(int *)&temp;
  //#else
  return floor(f);
  //#endif
}

//##
// Round a value upwards to the nearest integer
inline double lmCeil(double f)
{
  //#ifdef L_FPU_ASSUME_NEAREST_ROUNDING
  //unsigned int DoubleToIntConst[2] = { 0, LM_DOUBLE2INT };
  //double temp = ((f+0.5)+(*(double*)DoubleToIntConst));
  //return *(int *)&temp;
  //#else
  return ceil(f);
  //#endif
}

//##
// fmod; floating point modulo
inline double lmFmod(double x, double y)
{
  return fmod( x, y );
}

//##
// modf; decompose into integral and fractional parts
inline double lmModf(double x, double *integralPointer)
{
  return modf( x, integralPointer );
}

//##
// Square root
inline double lmSqrt(double num)
{
  return sqrt(num);
}

//##
// sine
inline double lmSin(double num)
{
  return sin(num);
}

//##
// cosine
inline double lmCos(double num)
{
  return cos(num);
}

//##
// tangent
inline double lmTan(double num)
{
  return tan(num);
}

//##
// arc tangent
inline double lmAtan(double num)
{
  return atan(num);
}

//##
// arc tangent 2
inline double lmAtan2(double x, double y)
{
  return atan2(x, y);
}

//##
// logarithm
inline double lmLog(double num)
{
  return log(num);
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Single precision routines
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Convert a floating point number to an integer
// Rounds the value down.
inline int lmFloatToInt(float f)
{
  //#ifdef L_FPU_ASSUME_NEAREST_ROUNDING
  //unsigned int FloatToIntConst = LM_FLOAT2INT;
  //double temp = ((f-0.5f)+(*(float*)&FloatToIntConst));
  //return *(int *)&temp;
  //#else
  return static_cast<int>(f);
  //#endif
}

//##
// Convert a floating point number to 16.16 fixed point
inline int lmFloatToFixed1616(float f)
{
  //#ifdef L_FPU_ASSUME_NEAREST_ROUNDING
  //unsigned int FloatToIntConst = LM_FLOAT2FIX;
  //double temp = (f+(*(float*)&FloatToIntConst));
  //return *(int *)&temp;
  //#else
  return static_cast<int>(f * 65536.0f);
  //#endif
}

//##
// Round a value down to the nearest integer
inline float lmFloor(float f)
{
  //#ifdef L_FPU_ASSUME_NEAREST_ROUNDING
  //unsigned int FloatToIntConst = LM_FLOAT2INT;
  //double temp = ((f-0.5f)+(*(float*)&FloatToIntConst));
  //return (float)*(int *)&temp;
  //#else
  return floorf(f);
  //#endif
}

//##
// Round a value upwards to the nearest integer
inline float lmCeil(float f)
{
  //#ifdef L_FPU_ASSUME_NEAREST_ROUNDING
  //unsigned int FloatToIntConst = LM_FLOAT2INT;
  //double temp = ((f+0.5f)+(*(float*)&FloatToIntConst));
  //return (float)*(int *)&temp;
  //#else
  return ceilf(f);
  //#endif
}

//##
// fmod; floating point modulo
inline float lmFmod(float x, float y)
{
  return fmodf( x, y );
}

//##
// modf; decompose into integral and fractional parts
inline float lmModf(float x, float *integralPointer)
{
  return modff( x, integralPointer );
}

//##
// Square root
inline float lmSqrt(float num)
{
  return sqrtf(num);
}

//##
// sine
inline float lmSin(float num)
{
  return sinf(num);
}

//##
// cosine
inline float lmCos(float num)
{
  return cosf(num);
}

//##
// tangent
inline float lmTan(float num)
{
  return tanf(num);
}

//##
// arc tangent
inline float lmAtan(float num)
{
  return atanf(num);
}

//##
// arc tangent 2
inline float lmAtan2(float x, float y)
{
  return atan2f(x, y);
}

//##
// logarithm
inline float lmLog(float num)
{
  return logf(num);
}


#ifndef L_CHIPSET_X86

// float functions

#if defined(L_PLATFORM_PS2) || defined(L_PLATFORM_PSP)


//##
// lmFabsf (float)
// mask off the sign bit
inline float lmFabsf(float num)
{
  uint32 r = (*((uint32 *)&num)) & 0x7fffffff;
  return *((float *)&r);

  //float r;
  //__asm
  //{
  //  mov eax, dword ptr [num]
  //  and eax, 0x7fffffff
  //  mov dword ptr [r], eax
  //}
  //return r;
}

//##
// floating point abs
inline float lmFabs(float num)
{
  return lmFabsf(num);
}

#else

//##
// floating point abs
inline float lmFabs(float num)
{
  return fabsf(num);
}

//##
// floating point abs
inline float lmFabsf(float num)
{
  return fabsf(num);
}

#endif

//##
// floating point arcsin
inline float lmAsin(float x)
{
  return asinf(x);
}

//##
// arccosine
inline float lmAcos(float x)
{
  return acosf(x);
}

//##
// power function - x to the power of y
inline float lmPow(float x,float y)
{
  return powf(x,y);
}

// double functions

//##
// double abs
inline double lmFabs(double num)
{
  return fabs(num);
}

//##
// double abs
inline double lmFabsf(double num)
{
  return fabsf(num);
}

//##
// arcsine
inline double lmAsin(double x)
{
  return asin(x);
}

//##
// arccosine
inline double lmAcos(double x)
{
  return acos(x);
}

//##
// power function - x to the power of y
inline double lmPow(double x,double y)
{
  return pow(x,y);
}

//##
// sin cos
inline void lmSinCos( float theta, float &sinV, float &cosV )
{
  sinV = lmSin( theta );
  cosV = lmCos( theta );
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// X86 specific functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#else

//##
// sin cos
inline void lmSinCos( float theta, float &sinV, float &cosV )
{
  float cr, sr;
  __asm
	{
    fld       theta

		fsincos

    fstp     cr
    fstp     sr
	}
  sinV = sr;
  cosV = cr;
}

//##
// arcsine optimised
inline float lmAsin(float x)
{
  // acos = atan( sqrt( ((-x)*x)+1) / (-x) ) + (2*atan(1));
  // or: acos = atan( x / sqrt((x+1)*(1-x))  )

  float f;
  __asm
  {
    fld x               ; x
    fld1                ; 1 , x
    fadd  st(0),st(1)   ; x+1, x
    fld1                ; 1, x+1, x
    fsub  st(0),st(2)   ; 1-x, x+1, x
    fmulp st(1),st(0)   ; x+1 * 1-x, x
    fsqrt               ; sqrt( x+1 * 1-x ), x
    fpatan              ; atan( x / sqrt((x+1)*(1-x)) )
    fstp f
  }
  return f;
}

//##
// arccosine
inline float lmAcos(float x)
{
  //NOTE: This code generates warning C4725: instruction may be inaccurate on some Pentiums on VC 7
  // We can either set the /QIfdiv (enable pentium FDIV fix) which performs a runtime check or recode this
  // some other way

  // acos = atan( (-x)/sqrt( ((-x)*x)+1) ) + (2*atan(1));
  // or: acos = atan( sqrt((x+1)*(1-x)) / x )

  float f;
  __asm
  {
    fld x               ; x
    fld1                ; 1 , x
    fadd  st(0),st(1)   ; x+1, x
    fld1                ; 1, x+1, x
    fsub  st(0),st(2)   ; 1-x, x+1, x
    fmulp st(1),st(0)   ; x+1 * 1-x, x
    fsqrt               ; sqrt( x+1 * 1-x ), x
    fxch st(1)          ; x, sqrt( (x+1)*(1-x) )
    fpatan              ; atan( sqrt((x+1)*(1-x)) / x )
    fstp f
  }
  return f;
}

//##
// fabs (float)
// mask off the sign bit
inline float lmFabsf(float num)
{
  unsigned int  r = (*((unsigned int  *)&num)) & 0x7fffffff;
  return *((float *)&r);

  //float r;
  //__asm
  //{
  //  mov eax, dword ptr [num]
  //  and eax, 0x7fffffff
  //  mov dword ptr [r], eax
  //}
  //return r;
}

//##
// Check which returns true or false if float is positive or negative
inline bool lmIsNegativef(float num)
{
  return (((*((unsigned int *)&num)) & 0x80000000) == 0x80000000);
}

//##
// fabs (double)
// mask off the sign bit
inline double lmFabs(double num)
{
 #ifdef L_HAS_PRAGMA_PACK
  #pragma pack(push,4)
 #endif
  union L_GCC_PACKED(4) { struct { unsigned int  lo; unsigned int  hi; } dword; int64 qword; } r;
 #ifdef L_HAS_PRAGMA_PACK
  #pragma pack(pop)
 #endif
  r.qword = (*((int64 *)&num));
  r.dword.hi &= 0x7fffffff;
  return *((double *)&r.qword);

  //double r;
  //__asm
  //{
  //  mov eax, dword ptr [num]
  //  mov edx, dword ptr [num+4]
  //  and edx, 0x7fffffff
  //  mov dword ptr [r] , eax
  //  mov dword ptr [r+4] , edx
  //}
  //return r;
}

inline float lmFabs(float num)
{
  return lmFabsf(num);
}

//##
// x to the yth power = 2^(y*log2(x))
//
// In:
//   x,y - x and y
//
// Out:
//   x raised to the yth power
//
// Notes:
//   pow(x,y) == exp( log(x) * y )
//   or, pow(x,y) == 2^(y*log2(x))
//
//   1: pow(x,y) == exp( log(x) * y )
//   start               x             , y            -
//   fldln2              loge2         , x      , y   - load loge2
//   fxch st(1)          x             , loge2  , y   - swap em
//   fyl2x               log(x)        , y            - loge2 * log2(x)
//   fmulp st(1),st(0)   log(x) * y                   - multiply em
//   nb: r == log(x) * y for brevity
//   fldl2e              log2e         , r            - load log2e
//   fmulp st(1),st(0)   r * log2e                    - multiply em
//   fld st(0)           r * log2e     , r * log2e    - dupe st(0)
//   frndint             frumple       , r * log2e    - round to integer
//   fxch st(1)          r * log2e     , frumple      - swap em
//   fsub st(0), st(1)   blah          , frumple      - sub rounded from orig
//   f2xm1               2^(blah)-1    , frumple      - calculate 2^st(0)-1
//   fld1                1, 2^(blah)-1 , frumple      - load 1.0
//   addp st(1), st(0)   2^(blah)      , frumple      - add 1
//   fscale              pow(x,y)      , frumple      - st(0) * 2^st(1)
//   ffree st(1)         pow(x,y)                     - free st(1)
//
//   2: pow(x,y) == 2^(y*log2(x))
//   start               x             , y
//   fyl2x               y * log2(x)                  - y * log(x)
//   fld st(0)           y * log2(x)   , y * log2(x)  - dupe st(0)
//   frndint             frumple       , y * log2(x)  - round to integer
//   fxch st(1)          y * log2(x)   , frumple      - swap em
//   fsub st(0), st(1)   blah          , frumple      - sub rounded from orig
//   f2xm1               2^(blah)-1    , frumple      - calculate 2^st(0)-1
//   fld1                1, 2^(blah)-1 , frumple      - load 1.0
//   faddp st(1), st(0)  2^(blah)      , frumple      - add 1
//   fscale              pow(x,y)      , frumple      - st(0) * 2^st(1)
//   ffree st(1)         pow(x,y)                     - free st(1)
//

inline double lmPow(double x,double y)
{
  double r;
  __asm
  {
    fld y
    fld x
    fyl2x
    fld st(0)
    frndint
    fxch st(1)
    fsub st(0), st(1)
    f2xm1
    fld1
    faddp st(1),st(0)
    fscale
    ffree st(1)
    fstp r
  }
  return r;
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FPU Status control word manipulation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//## X86 precision control bits
// Remarks:
// The precision-control bits only affect the results of the following
// floating-point instructions:
// FADD, FADDP, FSUB, FSUBP, FSUBR, FSUBRP, FMUL, FMULP, FDIV, FDIVP, FDIVR,
// FDIVRP, and FSQRT.
enum L_FPU_PRECISION
{
  L_FPU_PRECISION_SINGLE   = 0,      //## 24 bit precision
  L_FPU_PRECISION_DOUBLE   = 1,      //## 53 bit precision
  L_FPU_PRECISION_EXTENDED = 2       //## 64 bit precision
};


//## X86 rounding control bits
// Remarks:
// The precision-control bits only affect the results of the following
// floating-point instructions:
// FADD, FADDP, FSUB, FSUBP, FSUBR, FSUBRP, FMUL, FMULP, FDIV, FDIVP, FDIVR,
// FDIVRP, and FSQRT.
enum L_FPU_ROUND
{
  L_FPU_ROUND_NEAREST = 0,   //## round to nearest
  L_FPU_ROUND_DOWN    = 1,   //## round down (towards -INF)
  L_FPU_ROUND_UP      = 2,   //## round up (towards +INF)
  L_FPU_ROUND_CHOP    = 3    //## chop (truncate, towards zero)
};

//##
// lmFPUPrecisionSet
// Set the FPU precision bits
//
// In:
//   prec - L_FPU_PRECISION
//
inline void lmFPUPrecisionSet(enum L_FPU_PRECISION val)
{
  unsigned int  scw,prec;
  prec = val;
  prec <<= 8;
  __asm fnclex
  __asm fstcw word ptr [scw]
  __asm mov eax, dword ptr [scw]
  __asm and eax, 0000fcffh
  __asm or  eax, prec
  __asm mov dword ptr [scw], eax
  __asm fldcw word ptr [scw]
}

//##
// lmFPUPrecisionGet
// Get the FPU precision bits
//
// Out:
//   Returns one of the L_FPU_PRECISION constants
//
inline enum L_FPU_PRECISION lmFPUPrecisionGet(void)
{
  unsigned int  scw;
  __asm fnclex
  __asm fstcw word ptr [scw]
  scw &= 0x300;
  scw >>= 8;
  return (enum L_FPU_PRECISION)scw;
}

//##
// lmFPURoundingSet
// Set FPU rounding mode
//
// In:
//   rmode - L_FPU_ROUND
//
inline void lmFPURoundingSet(enum L_FPU_ROUND val)
{
  unsigned int  scw,rmode;
  rmode = val;
  rmode <<= 10;
  __asm fnclex
  __asm fstcw word ptr [scw]
  __asm mov eax, dword ptr [scw]
  __asm and eax, 0000f3ffh
  __asm or  eax, rmode
  __asm mov dword ptr [scw], eax
  __asm fldcw word ptr [scw]
}

//##
// lmFPURoundingGet
// Get the FPU rounding mode
//
// Out:
//   Returns one of the L_FPU_ROUND constants
//
inline enum L_FPU_ROUND lmFPURoundingGet(void)
{
  unsigned int  scw;
  __asm fnclex
  __asm fstcw word ptr [scw]
  scw &= 0xc00;
  scw >>= 10;
  return (enum L_FPU_ROUND)scw;
}

#endif


//##
// Floating point minimum function (branch-free)
inline float lmFMin( float x, float y ) { return ( x + y - lmFabs( x - y ) ) * 0.5f; }
//##
// Floating point maximum function (branch-free)
inline float lmFMax( float x, float y ) { return ( x + y + lmFabs( x - y ) ) * 0.5f; }

//##
// Floating point minimum function (branch-free)
inline double lmFMin( double x, double y ) { return ( x + y - lmFabs( x - y ) ) * double(0.5); }

//##
// Floating point maximum function (branch-free)
inline double lmFMax( double x, double y ) { return ( x + y + lmFabs( x - y ) ) * double(0.5); }

//##
// copysign
// Copy the sign of a to b
inline float lmCopySignf(float a, float b)
{
  int i = (int&)b & 0x7fffffff | (int&)a & 0x80000000;
  return (float&)i;
}

END_L_NAMESPACE

#endif

//*
// *  Smoothing function.
// *
// *  Uses critically damped spring for ease-in/ease-out smoothing. Stable
// *  at any time intervals. Based on GPG4 article.
// *
// *  @param from Current value.
// *  @param to Target value (may be moving).
// *  @param vel  Velocity (updated by the function, should be maintained between calls).
// *  @param smoothTime Time in which the target should be reached, if travelling at max. speed.
// *  @param dt Delta time.
// *  @return Updated value.
// *
//template< typename T >
//inline T smoothCD( const T& from, const T& to, T& vel, float smoothTime, float dt )
//{
//  float omega = 2.0f / smoothTime;
//  float x = omega * dt;
//  // approximate exp()
//  float exp = 1.0f / (1.0f + x + 0.48f*x*x + 0.235f*x*x*x );
//  T change = from - to;
//  T temp = ( vel + omega * change ) * dt;
//  vel = ( vel - omega * temp ) * exp;
//  return to + ( change + temp ) * exp;
//}




