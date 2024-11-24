//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmrand.h
// Description : Random number generators
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMRAND_H_INCLUDED
#define LMRAND_H_INCLUDED

#include "lmath.h"

BEGIN_L_NAMESPACE

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lmRandomBasic
//   This is based on CRT rand()'s method
//
// rand()         : returns [0 - 32767]
// randFloat()    : returns [0.0f - 1.0f]
// randInt(n)     : returns [0 - n]
// randIntExc(n)  : returns [0 - n)
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

class LMATH_API lmRandomBasic
{
public:
  lmRandomBasic( unsigned int s = 1 )
  {
    setSeed( s );
  }

  inline unsigned int rand()
  {
    seed = seed * 214013 + 2531011;
    return((seed >> 16) & 0x7fff);
  }

  inline float randFloat()
  {
    seed = seed * 214013 + 2531011;
    return((float)((seed >> 16) & 0x7fff) / 32767.0f);
  }

  inline float randFloatBetween( float x, float y )
  {
    return x + ((y - x) * randFloat());
  }

  inline unsigned int randInt( int n )
  {
    float a = float(n) / 32767.0f;
    return (int) (float(rand()) * a);
  }

  inline unsigned int randIntExc( int n )
  {
    float a = float(n) / 32768.0f;
    return (int) (float(rand()) * a);
  }

#ifdef lmSSE
  // SIMD version, must be compatible
#else
  inline void randArray( unsigned int *array, unsigned int count )
  {
    for(unsigned int i = 0; i < count; i++)
    {
      seed = seed * 214013 + 2531011;
      array[i] = ((seed >> 16) & 0x7fff);
    }
  }

  inline void randFloatArray( float *array, unsigned int count )
  {
    for(unsigned int i = 0; i < count; i++)
    {
      seed = seed * 214013 + 2531011;
      array[i] = (float((seed >> 16) & 0x7fff) / 32767.0f);
    }
  }
#endif

  //##
  // Set the seed for this instance of the random number generator
  //
  // See Also: getOrigSeed
  //
  void setSeed( unsigned int s )
  {
    origseed = seed = s;
  }

  //##
  // Get the original seed for this instance of the random number generator
  //
  // See Also: setSeed
  //
  unsigned int getOrigSeed()
  {
    return origseed;
  }

protected:
  unsigned int origseed;
  unsigned int seed;
};


//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lmRandomMersenneTwister
//   Mersenne twister based random function
//
// rand()                     : returns [0 - 32767]
// randInt()                  : returns [0 - 4294967296)
// randFloat()                : returns [0.0f - 1.0f]
// randFloatExc()             : returns [0.0f - 1.0f)
// randDouble()               : returns [0.0 - 1.0]
// randDouble53()             : returns [0.0 - 1.0]
// randDoubleExc()            : returns [0.0 - 1.0)
// randDoubleDblExc()         : returns (0.0 - 1.0)
// randInt(n)                 : returns [0 - n]
// randIntExc(n)              : returns [0 - n)
// randNorm( mean, variance ) : return a real number from a normal (Gaussian) distribution with given mean and variance by Box-Muller method
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

class LMATH_API lmRandomMersenneTwister
{
public:
  lmRandomMersenneTwister( unsigned int s = 4357 )
  {
    setSeed( s );
  }

  //##
  // Set the seed for this instance of the random number generator
  //
  // See Also: getOrigSeed
  //
  void setSeed( unsigned int s )
  {
    origseed = s;
    seed( s );
  }

  //##
  // Get the original seed for this instance of the random number generator
  //
  // See Also: setSeed
  //
  unsigned int getOrigSeed()
  {
    return origseed;
  }

  inline unsigned int rand()
  {
    return (randInt() & 0x7fff);
  }

  inline unsigned int randInt()
  {
    // Pull a 32-bit integer from the generator state
    // Every other access funlmion simply transforms the numbers extralmed here

    if( left == 0 ) reload();
    --left;

    register unsigned int s1;
    s1 = *pNext++;
    s1 ^= (s1 >> 11);
    s1 ^= (s1 <<  7) & 0x9d2c5680;
    s1 ^= (s1 << 15) & 0xefc60000;
    return ( s1 ^ (s1 >> 18) );
  }

  inline unsigned int randInt( int n )
  {
    float a = float(n) / 4294967295.0f;
    return (int) (float(randInt()) * a);
  }

  inline unsigned int randIntExc( int n )
  {
    float a = float(n) / 4294967296.0f;
    return (int) (float(randInt()) * a);
  }

  inline float randFloat()
  {
    return float(randInt()) * (1.0f/4294967295.0f);
  }

  inline float randFloatExc()
  {
    return float(randInt()) * (1.0f/4294967296.0f);
  }

  inline float randFloatBetween( float x, float y )
  {
    return x + ((y - x) * randFloat());
  }

  // random number array builders

  inline void randArray( unsigned int *array, unsigned int count )
  {
    for(unsigned int i = 0; i < count; i++)
    {
      array[i] = rand();
    }
  }

  inline void randIntArray( unsigned int *array, unsigned int count )
  {
    for(unsigned int i = 0; i < count; i++)
    {
      array[i] = randInt();
    }
  }

  inline void randFloatArray( float *array, unsigned int count )
  {
    for(unsigned int i = 0; i < count; i++)
    {
      array[i] = randFloat();
    }
  }

  // special double / real number stuff

  inline double randDouble()
  {
    return double(randInt()) * (1.0/4294967295.0);
  }

  inline double randDouble53()
  {
    unsigned int a = randInt() >> 5, b = randInt() >> 6;
    return ( a * 67108864.0 + b ) * (1.0/9007199254740992.0);  // by Isaku Wada
  }

  inline double randDoubleExc()
  {
    return double(randInt() + 0.5 ) * (1.0/4294967296.0);
  }

  inline double randDoubleDblExc()
  {
    return ( double(randInt()) + 0.5 ) * (1.0/4294967296.0);
  }

  inline double randNorm( const double& mean, const double& variance )
  {
    double r = lmSqrt( -2.0 * lmLog( 1.0-randDoubleDblExc()) ) * variance;
    double phi = 2.0 * 3.14159265358979323846264338328 * randDoubleExc();
    return mean + r * lmCos(phi);
  }

protected:
  enum { N = 624 };       // length of state velmor
	enum { SAVE = N + 1 };  // length of array for save()
	enum { M = 397 };  // period parameter

  inline unsigned int hiBit( const unsigned int& u ) const                                     { return u & 0x80000000; }
  inline unsigned int loBit( const unsigned int& u ) const                                     { return u & 0x00000001; }
  inline unsigned int loBits( const unsigned int& u ) const                                    { return u & 0x7fffffff; }
  inline unsigned int mixBits( const unsigned int& u, const unsigned int& v ) const
  {
    return hiBit(u) | loBits(v);
  }

 #ifdef L_COMPILER_MSVC
  #pragma warning( push )
  #pragma warning( disable : 4146 )
 #endif

  inline unsigned int twist( const unsigned int& m, const unsigned int& s0, const unsigned int& s1 ) const
  {
    return m ^ (mixBits(s0,s1)>>1) ^ (-loBit(s1) & 0x9908b0df);
  }

 #ifdef L_COMPILER_MSVC
  #pragma warning( pop )
 #endif

  unsigned int origseed;   // original seed

  unsigned int state[N];   // internal state
  unsigned int  *pNext;    // next value to get from state
  unsigned int  left;      // number of values left before reload needed

  inline void seed( const unsigned int oneSeed )
  {
    // Seed the generator with a simple unsigned int
    initialize(oneSeed);
    reload();
  }

  inline void initialize( const unsigned int seed )
  {
    // Initialize generator state with seed
    // See Knuth TAOCP Vol 2, 3rd Ed, p.106 for multiplier.
    // In previous versions, most significant bits (MSBs) of the seed affelm
    // only MSBs of the state array.  Modified 9 Jan 2002 by Makoto Matsumoto.

    register unsigned int *s = state;
    register unsigned int *r = state;
    register int i = 1;

    *s++ = seed & 0xffffffff;

    for( ; i < N; ++i )
    {
      *s++ = ( 1812433253 * ( *r ^ (*r >> 30) ) + i ) & 0xffffffff;
      r++;
    }
  }

  inline void reload()
  {
    // Generate N new values in state
    // Made clearer and faster by Matthew Bellew (matthew.bellew@home.com)
    register unsigned int *p = state;
    register int i;
    for( i = N - M; i--; ++p )
      *p = twist( p[M], p[0], p[1] );

    for( i = M; --i; ++p )
      *p = twist( p[M-N], p[0], p[1] );

    *p = twist( p[M-N], p[0], state[0] );

    left = N, pNext = state;
  }

};

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Global defined random routines
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

LMATH_API extern lmRandomBasic           randomGen;
LMATH_API extern lmRandomMersenneTwister randomGenMT;

//##
// Get original seed of the rand() like generator
inline unsigned int lRandGetOrigSeed()
{
  return randomGen.getOrigSeed();
}

//##
// Set of the rand() like generator
inline void lRandSetSeed( unsigned int s )
{
  randomGen.setSeed( s );
}

//##
// Same result as rand() [0 - 32767]
inline unsigned int lRand()
{
  return randomGen.rand();
}

//##
// rand() based [0.0f - 1.0f]
inline float lRandFloat()
{
  return randomGen.randFloat();
}

//##
// Set of the Mersenne Twister generator
inline void lRandSetSeedMersenne( unsigned int s )
{
  randomGenMT.setSeed( s );
}

// Mersenne Twister based [0.0f - 1.0f]
inline float lRandFloatMersenne()
{
  return randomGenMT.randFloat();
}

//##
// Mersenne Twister based [lo - hi]
inline float lRandFloatMersenneBetween( float lo, float hi )
{
  return randomGenMT.randFloatBetween( lo, hi );
}

//##
// Mersenne Twister based [0.0f - 1.0f)
inline float lRandFloatMersenneExc()  // same as drand48 behavoir
{
  return randomGenMT.randFloatExc();
}

END_L_NAMESPACE
#endif
