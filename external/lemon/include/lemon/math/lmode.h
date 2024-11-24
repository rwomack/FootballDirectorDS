//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmode.h
// Description : ODE math classes
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMODE_H_INCLUDED
#define LMODE_H_INCLUDED

BEGIN_L_NAMESPACE

#include "lmath.h"
#include "./optimisations/lbase.h"

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Defines
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmODE - Base ODE class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define EXPAND_ARRAY( _x ) { if(_x) lFree( _x ); _x = (T *)lAlloc( sizeof(T) * _length ); }
#define FREE_ARRAY( _x )   { if(_x) lFree( _x ); }

template <class T>
class lmODE
{
  public:
    lmODE() : _tempDYDT(NULL), _length(0), _func(NULL) {}

    virtual ~lmODE() {}

    void SetLength( int length )
    {
      _length = length;
      Expand();
    }

    int Length() { return _length; }

    typedef void (*dydtFunc)(float,T *,T *,int);

    void SetDYDT( dydtFunc func ) { _func = func; }
    dydtFunc DYDT() { return _func; }

    virtual void Expand() = 0;
    float        Integrate( T *in, T *out, float t0, float t1, int ptr = 0) = 0;

  protected:
    T          * _tempDYDT;
    int          _length;
    dydtFunc     _func;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmODEEuler - Euler Method ODE solver
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <class T>
class lmODEEuler : public lmODE<T>
{
  public:
    lmODEEuler() {}
    lmODEEuler(int length) { _length = length; Expand(); }

    virtual ~lmODEEuler()
    {
      FREE_ARRAY(_tempDYDT);
    }

    void Expand()
    {
      EXPAND_ARRAY(_tempDYDT);
    }

    float Integrate( T *in, T *out, float t0, float t1, int ptr = 0 )
    {
      float dT = t1 - t0;

      _func(t0, in, _tempDYDT, ptr);

      for(int i = 0; i < _length; i++)
        out[i] = in[i] + _tempDYDT[i] * dT;

      return dT;
    }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmODEMidpoint - Midpoint Method ODE solver
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <class T>
class lmODEMidpoint : public lmODE<T>
{
  public:
    lmODEMidpoint() : _temp1(NULL) {}
    lmODEMidpoint(int length) : _temp1(NULL) { _length = length; Expand(); }

    virtual ~lmODEMidpoint()
    {
      FREE_ARRAY( _tempDYDT );
      FREE_ARRAY( _temp1 );
    }

    void Expand()
    {
      EXPAND_ARRAY( _tempDYDT );
      EXPAND_ARRAY( _temp1 );
    }

    float Integrate( T *in, T *out, float t0, float t1, int ptr = 0 )
    {
      float dT = (t1 - t0) * 0.5f;
      int i;

      _func(t0, in, _tempDYDT, ptr);

      for(i = 0; i < _length; i++)
        _temp1[i] = in[i] + _tempDYDT[i] * dT;

      _func(t1 + dT, _temp1, _tempDYDT, ptr);

      dT *= 2.0f;

      for(i = 0; i < _length; i++)
        out[i] = in[i] + _tempDYDT[i] * dT;

      return dT;
    }

  protected:
    T          * _temp1;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmODERungeKutta4 - Runge Kutta4 Method ODE solver
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <class T>
class lmODERungeKutta4 : public lmODE<T>
{
  public:
    lmODERungeKutta4() : _temp1(NULL), _temp2(NULL), _temp3(NULL), _temp4(NULL), _temp5(NULL) {}
    lmODERungeKutta4(int length) : _temp1(NULL), _temp2(NULL), _temp3(NULL), _temp4(NULL), _temp5(NULL)
      { _length = length; Expand(); }

    virtual ~lmODERungeKutta4()
    {
      FREE_ARRAY( _tempDYDT );
      FREE_ARRAY( _temp1 );
      FREE_ARRAY( _temp2 );
      FREE_ARRAY( _temp3 );
      FREE_ARRAY( _temp4 );
    }

    void Expand()
    {
      EXPAND_ARRAY( _tempDYDT );
      EXPAND_ARRAY( _temp1 );
      EXPAND_ARRAY( _temp2 );
      EXPAND_ARRAY( _temp3 );
      EXPAND_ARRAY( _temp4 );
    }

    float Integrate( T *in, T *out, float t0, float t1, int ptr = 0 )
    {
      float h   = t1 - t0;
      float h_2 = h * 0.5f;
      int i;

      // 1st iteration
      _func(t0, in, _temp1, ptr);

      for (i = 0; i < _length; i++)
        _tempDYDT[i] = in[i] + h_2 * _temp1[i];

      // 2nd iteration
      _func(t0 + h_2, _tempDYDT, _temp2, ptr);

      for (i = 0; i < _length; i++)
        _tempDYDT[i] = in[i] + h_2 * _temp2[i];

      // 3rd iteration
      _func(t0 + h_2, _tempDYDT, _temp3, ptr);
      for (i = 0; i < _length; i++)
        _tempDYDT[i] = in[i] + h * _temp3[i];

      // 4th iteration
      _func(t0 + h, _tempDYDT, _temp4, ptr);

      for (i = 0; i < _length; i++)
        out[i] = in[i] + h * ((_temp1[i] * 0.1666666f) + (_temp2[i] * 0.3333333f) +
                              (_temp3[i] * 0.3333333f) + (_temp4[i] * 0.1666666f));

      return h;
    }

  protected:
    T          * _temp1;
    T          * _temp2;
    T          * _temp3;
    T          * _temp4;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmODERungeKutta4 - Runge Kutta4 Adaptive Method ODE solver
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <class T>
class lmODERungeKutta4Adaptive : public lmODE<T>
{
  public:
    lmODERungeKutta4Adaptive() : _temp1(NULL), _temp2(NULL), _temp3(NULL), _temp4(NULL), _temp5(NULL), _maxError( 0.02f ) {}

    lmODERungeKutta4Adaptive(int length, float maxError = 0.02f) : _temp1(NULL), _temp2(NULL), _temp3(NULL),
                                                                   _temp4(NULL), _temp5(NULL), _maxError( maxError )
    {
      _length = length;
      Expand();
    }

    virtual ~lmODERungeKutta4Adaptive()
    {
      FREE_ARRAY( _tempDYDT );
      FREE_ARRAY( _temp1 );
      FREE_ARRAY( _temp1half );
      FREE_ARRAY( _temp2 );
      FREE_ARRAY( _temp3 );
      FREE_ARRAY( _temp4 );
      FREE_ARRAY( _temp5 );
    }

    void Expand()
    {
      EXPAND_ARRAY( _tempDYDT );
      EXPAND_ARRAY( _temp1 );
      EXPAND_ARRAY( _temp1half );
      EXPAND_ARRAY( _temp2 );
      EXPAND_ARRAY( _temp3 );
      EXPAND_ARRAY( _temp4 );
      EXPAND_ARRAY( _temp5 );
    }

    void SetMaxError( float maxError )
    {
      _maxError = maxError;
    }

    float Integrate( T *in, T *out, float t0, float t1, int ptr = 0 )
    {
      float h_2, h_4, curmax, curerror;
      float h = t1 - t0;
      int i, n;

      // Adaptive methods calculates each half then does a full RK4
      // and compares the error and reduces the delta to get a better
      // result

      for ( n = 0; n < 4; n++ )
      {
        h_2 = h * 0.5f;
        h_4 = h * 0.25f;

        // 1st half delta
        _func(t0, in, _temp1, ptr);
        for (i = 0; i < _length; i++)
        {
          _tempDYDT[i] = in[i] + h_4 * _temp1[i];
        }

        // second step of first half delta
        _func(t0 + h_4, _tempDYDT, _temp2, ptr);
        for (i = 0; i < _length; i++)
        {
          _tempDYDT[i] = in[i] + h_4 * _temp2[i];
        }

        // third step of first half delta
        _func(t0 + h_4, _tempDYDT, _temp3, ptr);
        for (i = 0; i < _length; i++)
        {
          _tempDYDT[i] = in[i] + h_2 * _temp3[i];
        }

        // fourth step of first half delta
        _func(t0 + h_2, _tempDYDT, _temp4, ptr);

        for (i = 0; i < _length; i++)
        {
          _tempDYDT[i] = in[i] + h_2 * ((_temp1[i] * 0.1666666f) + (_temp2[i] * 0.3333333f) +
                                        (_temp3[i] * 0.3333333f) + (_temp4[i] * 0.1666666f));
        }


        // first step of second half delta
        _func(t0 + h_2, _tempDYDT, _temp1half, ptr);
        for (i = 0; i < _length; i++)
        {
          _tempDYDT[i] = in[i] + h_4 * _temp1half[i];
        }

        // second step of second half delta
        _func(t0 + h_2 + h_4, _tempDYDT, _temp2, ptr);
        for (i = 0; i < _length; i++)
        {
          _tempDYDT[i] = in[i] + h_4 * _temp2[i];
        }

        // third step of second half delta
        _func(t0 + h_2 + h_4, _tempDYDT, _temp3, ptr);
        for (i = 0; i < _length; i++)
        {
          _tempDYDT[i] = in[i] + h_2 * _temp3[i];
        }

        // fourth step of second half delta
        _func(t0 + h, _tempDYDT, _temp4, ptr);
        for (i = 0; i < _length; i++)
        {
          out[i] = in[i] + h_2 * ((_temp1[i] * 0.1666666f) + (_temp2[i] * 0.3333333f) +
                                  (_temp3[i] * 0.3333333f) + (_temp4[i] * 0.1666666f));
        }

        // first step of full delta
        for (i = 0; i < _length; i++)
        {
          _tempDYDT[i] = in[i] + h_2 * _temp1[i];
        }

        // second step of full delta
        _func(t0 + h_2, _tempDYDT, _temp2, ptr);
        for (i = 0; i < _length; i++)
        {
          _tempDYDT[i] = in[i] + h_2 * _temp2[i];
        }

        // third step of full delta
        _func(t0 + h_2, _tempDYDT, _temp3, ptr);
        for (i = 0; i < _length; i++)
        {
          _tempDYDT[i] = in[i] + h * _temp3[i];
        }

        // fourth step of full delta
        _func(t0 + h, _tempDYDT, _temp4, ptr);
        for (i = 0; i < _length; i++)
        {
          _tempDYDT[i] = in[i] + h * ((_temp1[i] * 0.1666666f) + (_temp2[i] * 0.3333333f) +
                                      (_temp3[i] * 0.3333333f) + (_temp4[i] * 0.1666666f));
        }

        // get max estimated error
        curmax = 0.0f;

        for (i = 0; i < _length; i++)
        {
          curerror = lmFabsf( (out[i] - _tempDYDT[i]) / (h * _temp1[i] + 1e-10f) );

          if ( curerror > curmax )
          {
            curmax = curerror;
          }
        }

        curerror = curmax / maxError;

        if ( curerror <= 1.0f )
        {
          return h * 4.0f;
        }
        if ( h <= 1e-7f )
        {
          return h;
        }

        h *= 0.25f;
      }

      return h;
    }

  protected:
    T          * _temp1;
    T          * _temp1half;
    T          * _temp2;
    T          * _temp3;
    T          * _temp4;
    T          * _temp5;
};


END_L_NAMESPACE
#endif

