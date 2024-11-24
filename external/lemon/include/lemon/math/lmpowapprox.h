//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmpowapprox.h
// Description : pow() approximation
// Notes       : This code is based on Philippe Beaudoin and Juan Guardados
//               article on gamasutra 'A Non-Integer Power Function on the Pixel Shader'
//
//               This approximation is only good for x^n where x is [0,1]
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMPOWAPPROX_H_INCLUDED
#define LMPOWAPPROX_H_INCLUDED

#include "lmath.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//  lmPowerApproximation  - class to produce approximations of powers
//                          this class is good only for x^n operations where x is 0 - 1
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename T = double>
class lmPowerApproximation
{
 public:
    //##
    // Perform the power approximation using A+B (which represent a power n)
    // returns x^n
    static T Evaluate( T x, T m, T A, T B )
    {
      lAssert( x >= T(0.0) && x <= T(1.0) );
      T result = lmMax<T>((A * x) + B,0.0);

      for(int i = 1; i < m; i++)
      {
        result *= result;
      }

      return result;
    }

    //##
    // Find values for A minimizing the maximal error given n and m
    // In:
    //   n - This is the power we want to raise a value to
    //   m -
    static T FindA( T n, T m )
    {
      T A, B;
      FindAB( n, m, A, B );
      return A;
    }

    //##
    // Find values for A and B minimizing the maximal error given n and m
    // In:
    //   n - This is the power we want to raise a value to
    //   m -
    static void FindAB( T n, T m, T &A, T &B )
    {
      const T m_thresholdError = T(0.0001);
      T k, k0 = T(0.0), k1 = T(1.0);
      T ErrorLeft, ErrorRight;

      // Binary search for optimal crossing point
      do
      {
        k = (k0 + k1)/T(2.0);
        A = ( lmPow( k, n/m ) - T(1.0) ) / ( k - T(1.0) );
        B = T(1.0) - A;

        EvaluateMaxError( n, m, A, B, k, ErrorLeft, ErrorRight );

        if( ErrorLeft < ErrorRight )
            k0 = k;
        else
            k1 = k;

      } while( lmFabs( ErrorLeft - ErrorRight ) > m_thresholdError );
    }

    // Evaluate the approximation function
    static inline T Approx( T x, T m, T A, T B )
    {
       if( x < -B/A )
          return 0;
       return lmPow( A * x + B, m );
    }

    // Evaluate the derivative of the approximation function
    static inline T DerApprox( T x, T m, T A, T B )
    {
      if( x < -B/A )
        return 0;
      return A*m*lmPow( A * x + B, m-T(1) );
    }

    // Evaluate the error function
    static inline T Error( T x, T n, T m, T A, T B )
    {
      return Approx( x, m, A, B ) - lmPow( x, n );
    }

    // Evaluate the derivative of the error function
    static inline T DerError( T x, T n, T m, T A, T B )
    {
      return DerApprox( x, m, A, B ) - n*lmPow( x, n-1 );
    }

    // Evaluate the maximal error to the left and right of the crossing point
    // for given values of A and B
    // The crossing point k is given to reduce computation
    static void EvaluateMaxError( T n, T m, T A, T B, T k, T &ErrorLeft, T &ErrorRight )
    {
      const T m_epsilon        = T(0.0001);
      T x;
      T DerErr;

      // Find maximal point of the error function on the left of the crossing point
      T x0 = 0;
      T x1 = k;
      do
      {
        x = (x0 + x1)/T(2.0);

        DerErr = DerError( x, n, m, A, B );
        if( DerErr < 0 )
            x0 = x;
        else
            x1 = x;
      } while( lmFabs( x0-x1 ) > m_epsilon );

      // Evaluate the error at that point
      ErrorLeft = lmFabs( Error( x, n, m, A, B ) );

      x0 = k;
      x1 = 1;

      // Find maximal point of the error function on the right of the crossing point
      do
      {
        x = (x0 + x1)/T(2.0);

        DerErr = DerError( x, n, m, A, B );
        if( DerErr > 0 )
            x0 = x;
        else
            x1 = x;
      } while( lmFabs( x0-x1 ) > m_epsilon );

      // Evaluate the error at that point
      ErrorRight = lmFabs( Error( x, n, m, A, B ) );
    }

    //鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
    // Functions below here are all for restricted floating point systems
    //
    //鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍

    // Multiplier in the following functions contains the multiplier for each multiply instruction
    // this is used on shaders which are limited from -1 to 1

    // Compute value of k at instruction i given a table of multiply instruction modifiers
    static T ComputeK( int i, const int *Multiplier )
    {
      if( i == 0 )
        return Multiplier[i];

      T Temp = ComputeK( i-1, Multiplier );
      return Multiplier[i] * Temp * Temp;
    }

    // Compute maximum absolute values for A and B given some m and a multiplier table
    // LogM: log of m in base 2 (number of instructions - 1)
    static T MaxAB( int LogM, const int *Multiplier )
    {
      T m = lmPow( T(2.0), LogM );           // Find the value of m
      T K = ComputeK( LogM, Multiplier );         // Compute K
      return lmPow( K, T(1.0)/m );
    }

    // Compute values of A' and B' given A and B and a multiplier table
    // LogM: log of m in base 2 (number of instructions - 1)
    static void ComputeApBp( int LogM, T A, T B, T &APrime, T &BPrime, const int *Multiplier )
    {
      T Temp = T(1.0)/MaxAB( LogM, Multiplier );     // Note that k -1/m = 1/MaxAB

      APrime = A * Temp;
      BPrime = B * Temp;
    }

    // Compute maximum possible exponent given some m and a multiplier table
    // LogM: log of m in base 2 (number of instructions - 1)
    static T MaxN( int LogM, const int *Multiplier )
    {
      const T m_epsilon        = T(0.0001);
      T m = lmPow( T(2.0), LogM );           // Find the value of m
      T Max = MaxAB( LogM, Multiplier );
      T A, B;

      T n0 = m;                                // Lower bound for maximal exponent
      T n1 = 5000;                           // Upper bound for maximal exponent
      T n;

      do
      {
        n = (n0 + n1)/T(2.0);

        FindAB( n, m, A, B );

        if( lmFabs(A) > Max || lmFabs(B) > Max )
          n1 = n;
        else
          n0 = n;
      } while( lmFabs( n0 - n1 ) > m_epsilon );

      return n;
    }
};

END_L_NAMESPACE
#endif

