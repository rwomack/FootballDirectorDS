//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmsolve.h
// Description : Equation solving
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMSOLVE_H_INCLUDED
#define LMSOLVE_H_INCLUDED

BEGIN_L_NAMESPACE

#include "lmath.h"

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Linear equation and system solving functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

int LMATH_API lmSolveLinearSystem(double **a, double *x, int size);

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Solving routines
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define IS_ZERO(_x) ((_x) > -LM_EPSILON && (_x) < LM_EPSILON)
#define CBRT(_x) ((_x) > 0.0f ? lmPow((_x), 1.0f/3.0f) : \
                 ((_x) < 0.0f ? -lmPow(-(_x), 1.0f/3.0f) : 0.0f))

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// Solves a linear equation for (ax+b=0)
// lmSolveLinear
//
// In:
//   outX - root of linear equation
//   a,b  - linear coefficients
//
// Out:
//   Number of roots
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍

template <typename T>
int lmSolveLinear(T &outR, T a, T b)
{
  if ( IS_ZERO( a ) )
  {
    return 0;
  }
  else
  {
    outR = - b / a;
    return 1;
  }
}


//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// Solves a quadratic equation (ax^2+by+c=0)
// lmSolveQuadratic
//
// In:
//   outX,outY - roots of quadratic equation
//   a,b,c     -  quadratic coefficients
//
// Out:
//   Number of roots
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍

template <typename T>
int lmSolveQuadratic(T &outX, T &outY, T a, T b, T c)
{
  float determinant = (b * b)-(4 * a * c);
  float root = lmSqrt( determinant );

  if ( IS_ZERO( a ) )
  {
    //no quadratic term: linear
    return lmSolveLinear( outX, b, c );
  }

  if ( determinant < T( 0.0 ) )
  {
    return 0;
  }

  a *= 2.0;

  outX = ( -b - root ) / a;

  if ( determinant == T( 0.0 ) )
  {
    outY = outX;
    return 1;
  }

  outY = ( -b + root ) / a;

  return 2;
}


//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// Solves a cubic equation ( dx^3  +  cx^2  +  bx  +  a  =  0 )
// lmSolveCubic
//
// In:
//   outX,outY,outZ - roots of cubic equation
//   a,b,c,d        -  cubic coefficients
//
// Out:
//   Number of roots
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍

template <typename T>
int lmSolveCubic(T &outX, T &outY, T &outZ, T a, T b, T c, T d)
{
  int   num;
  T     sub;
  T     A, B, C;
  T     sq_A, p, q;
  T     cb_p, D;

  if ( IS_ZERO( d ) )
  {
    //no cubic term: quadratic
    return lmSolveQuadratic( outX, outY, c, b, a );
  }
  else
  {
    // normalize the equation:x ^ 3 + Ax ^ 2 + Bx  + C = 0

    A = c / d;
    B = b / d;
    C = a / d;

    // substitute x = y - A / 3 to eliminate the
    // quadric term: x^3 + px + q = 0

    sq_A = A * A;
    p = 1.0 / 3.0   *   ( -1.0 / 3.0  *  sq_A + B );
    q = 1.0 / 2.0   *   ( 2.0 / 27.0  *  A * sq_A  -  1.0 / 3.0 * A * B  +  C );

    // use Cardano's formula

    cb_p = p * p * p;
    D = q * q + cb_p;

    if ( IS_ZERO( D ) )
    {
      if ( IS_ZERO ( q ) )
      {
        // one triple solution
        outX = 0.0;
        num = 1;
      }
      else
      {
        // one single and one double solution
        double u = CBRT( -q );
        outX = 2.0 * u;
        outY = - u;
        num = 2;
      }
    }
    else if (D < 0.0)
    {
      // casus irreductibilis: three real solutions
      double phi = 1.0 / 3.0  *  lmAcos( -q / lmSqrt( -cb_p ) );
      double t = 2.0 * lmSqrt( -p );
      outX = t * lmCos( phi );
      outY = -t * lmCos( phi + LM_PI / 3.0 );
      outZ = -t * lmCos( phi - LM_PI / 3.0 );
      num = 3;
    }
    else
    {
      // one real solution
      double sqrt_D = lmSqrt( D );
      double u = CBRT( sqrt_D + lmFabs( q ) );
      if ( q > 0.0 )
      {
        outX = - u + p / u;
      }
      else
      {
        outX = u - p / u;
      }
      num = 1;
    }

    // resubstitute

    sub = 1.0 / 3.0 * A;

    if( num == 1 )
    {
      outX -= sub;
    }
    else if( num == 2 )
    {
      outX -= sub;
      outY -= sub;
    }
    else if( num == 3 )
    {
      outX -= sub;
      outY -= sub;
      outZ -= sub;
    }

    return num;
  }
}

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// Solves a quartic equation (ax^4+by^3+cz^2+dw+e=0)
// lmSolveQuartic
//
// In:
//   outX,outY,outZ,outW  - quartic roots
//   a,b,c,d,e            - quartic coefficients
//
// Out:
//   Number of roots
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍

template<typename T>
int lmSolveQuartic( float &outX,float &outY,float &outZ,float &outW,
                    float a,float b,float c,float d,float e)
{
  T coeffs[4];
  T zz, u, v, sub;
  T A, B, C, D;
  T sq_A, p, q, r;
  int    num = 0;
  T s[4];

  // normalize the equation: x ^ 4 + Ax ^ 3 + Bx ^ 2 + Cx + D = 0

  A = d / e;
  B = c / e;
  C = b / e;
  D = a / e;

  // subsitute x = y - A / 4 to eliminate the
  // cubic term: x^4 + px^2 + qx + r = 0

  sq_A = A * A;

  p = -3.0 / 8.0 * sq_A + B;

  q = 1.0 / 8.0 * sq_A * A - 1.0 / 2.0 * A * B + C;

  r = -3.0 / 256.0 * sq_A * sq_A + 1.0 / 16.0 *
      sq_A * B - 1.0 / 4.0 * A * C + D;

  if (IS_ZERO(r))
  {
    // no absolute term:y(y ^ 3 + py + q) = 0
    coeffs[0] = q;
    coeffs[1] = p;
    coeffs[2] = 0.0;
    coeffs[3] = 1.0;

    num = lmSolveCubic(&s[0],&s[1],&s[2],a,b,c,d);
    s[num++] = 0;
  }
  else
  {
    // solve the resolvent cubic...
    coeffs[0] = 1.0 / 2.0 * r * p - 1.0 / 8.0 * q * q;
    coeffs[1] = -r;
    coeffs[2] = -1.0 / 2.0 * p;
    coeffs[3] = 1.0;
    (void) lmSolveCubic(&s[0],&s[1],&s[2],a,b,c,d);

    // ...and take the one real solution...
    zz = s[0];

    // ...to build two quadratic equations
    u = zz * zz - r;
    v = 2.0 * zz - p;

    if (IS_ZERO(u))
    u = 0.0;
    else if (u > 0.0)
    u = lmSqrt(u);
    else
    return 0;

    if (IS_ZERO(v))
    v = 0;
    else if (v > 0.0)
    v = lmSqrt(v);
    else
    return 0;

    coeffs[0] = zz - u;
    coeffs[1] = q < 0 ? -v : v;
    coeffs[2] = 1.0;

    num = lmSolveQuadric(&s[0],&s[1],&s[2],&s[3],a,b,c,d,e);

    coeffs[0] = zz + u;
    coeffs[1] = q < 0 ? v : -v;
    coeffs[2] = 1.0;

    num += lmSolveQuadric(&s[num+0],&s[num+1],&s[num+2],&s[num+3],a,b,c,d,e);
  }

  lAssert( num <= 4 );

  // resubstitute

  sub = 1.0 / 4 * A;

  if( num == 1 )
  {
    outX = s[0] - sub;
    outY = 0;
    outZ = 0;
    outW = 0;
  }
  else if( num == 2 )
  {
    outX = s[0] - sub;
    outY = s[1] - sub;
    outZ = 0;
    outW = 0;
  }
  else if( num == 3 )
  {
    outX = s[0] - sub;
    outY = s[1] - sub;
    outZ = s[2] - sub;
    outW = 0;
  }
  else if( num == 4 )
  {
    outX = s[0] - sub;
    outY = s[1] - sub;
    outZ = s[2] - sub;
    outW = s[3] - sub;
  }

  return num;
}




#undef IS_ZERO
#undef CBRT

END_L_NAMESPACE

#endif

