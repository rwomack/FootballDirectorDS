//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmsphericalharmonics.h
// Description : Spherical harmonics class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMSPHERICALHARMONICS_H_INCLUDED
#define LMSPHERICALHARMONICS_H_INCLUDED

#include "../containers/larray.h"
#include "../image/limage.h"
#include "lmath.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lmSphericalHarmonics - Spherical Harmonics class
// TODO: needs rotation and more generation functions / options
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define NUM_HARMONICS 9
#define NUM_VSHADER_CONSTANTS 7

//##
//  Spherical harmonic storage + utility class
class lmSphericalHarmonics
{
public:
  lmVector3 m_harmonics[NUM_HARMONICS];

  lmSphericalHarmonics()
  {
    Clear();
  }

  lmVector3 EvaluateFirstHarmonic() const
  {
    static float sqrtPi = lmSqrt( LM_PI );
    static float c0 = 1.0f  /  ( sqrtPi * 2.0f );

    return m_harmonics[0] * c0;
  }

  lmVector3 Evaluate( const lmVector3 &v ) const
  {
    static const float sqrtPi = lmSqrt( LM_PI );
    static const float c0 = 1.0f  /  ( sqrtPi * 2.0f );
    static const float c1 = lmSqrt( 3.0f )  /  ( sqrtPi * 3.0f );
    static const float c2 = lmSqrt( 15.0f )  /  ( sqrtPi * 8.0f );
    static const float c3 = lmSqrt( 5.0f )  /  ( sqrtPi * 16.0f );
    static const float c4 = c2 * 0.5f;

    lmVector3 result = m_harmonics[0] * c0  -  m_harmonics[6] * c3;    //constant (1.0)

    result += m_harmonics[3] * v(0) * c1;                               //x
    result += m_harmonics[1] * v(1) * c1;                               //y
    result += m_harmonics[2] * v(2) * c1;                               //z

    result += m_harmonics[4] * v(0) * v(1) * c2;                        //xy
    result += m_harmonics[5] * v(1) * v(2) * c2;                        //yz
    result += m_harmonics[6] * v(2) * v(2) * c3 * 3.0f;                 //zz
    result += m_harmonics[7] * v(0) * v(2) * c2;                        //xz

    result += m_harmonics[8] * ( v(0) * v(0)  -  v(1) * v(1) ) * c4;    //xx-yy

    return result;
  }

  void Clear()
  {
    for (int i = 0; i < NUM_HARMONICS; i++)
    {
      m_harmonics[i].Init( 0.0f, 0.0f, 0.0f );
    }
  }

  void AddSample( const lmVector3 &v, float scale, const lmVector3 &colour )
  {
    float w[9];
    ComputeInputWeightTable( v, scale, w );

    for (int i = 0; i < NUM_HARMONICS; i++)
    {
      m_harmonics[i] += colour * w[i];
    }
  }

  void Spheremap( const lImage &image, bool bCCW, float seam )
  {
    Clear();

    // TODO assert image type

    int width = image.Width();
    int height = image.Height();
    float deltaAzimuth  = LM_PI * 2.0f /  (float)width;
    float deltaAltitude = LM_PI        /  (float)height;

    if ( bCCW )
    {
      deltaAzimuth = -deltaAzimuth;
    }

    float seamRadians = lmDeg2Rad( seam );

    lmVector3 direction;

    const unsigned char *data = (const unsigned char*)image.GetLevelData(0);
    float altitude = -LM_PI * 0.5f + deltaAltitude * 0.5f;
    for (int j = 0; j < height; j++)
    {
      direction(1) = -lmSin( altitude );      // y
      float a = lmCos( altitude );
      float sampleScale = deltaAzimuth * deltaAltitude * a;

      float azimuth = seamRadians  +  deltaAzimuth * 0.5f;
      for (int i = 0; i < width; i++)
      {
        direction(0) = lmSin( azimuth ) * a;        // x
        direction(2) = lmCos( azimuth ) * a;

        float r = (float)data[0] / 255.0f;
        float g = (float)data[1] / 255.0f;
        float b = (float)data[2] / 255.0f;

        AddSample( direction, sampleScale, lmVector3( r, g, b ) );

        azimuth += deltaAzimuth;

        data += 4;
      }

      altitude += deltaAltitude;
    }
  }

  void DirectionalLight( const lmVector3 &direction, const lmVector3 &colour )
  {
    float w[NUM_HARMONICS];

    Clear();
    ComputeInputWeightTable( direction, 2.954795f, w );

    for (int i = 0; i < NUM_HARMONICS; i++)
    {
      m_harmonics[i] += colour * w[i];
    }
  }

  void DirectionalLightAdd( const lmVector3 &direction, const lmVector3 &colour )
  {
    float w[NUM_HARMONICS];

    ComputeInputWeightTable( direction, 2.954795f, w );

    for (int i = 0; i < NUM_HARMONICS; i++)
    {
      m_harmonics[i] += colour * w[i];
    }
  }

  bool Set( const float *vals, int numvals )
  {
    if( numvals == NUM_HARMONICS )
    {
      for (int i = 0; i < NUM_HARMONICS; i++)
      {
        m_harmonics[i](0) = vals[i*3+0];
        m_harmonics[i](1) = vals[i*3+1];
        m_harmonics[i](2) = vals[i*3+2];
      }
      return true;
    }

    return false;
  }

  #ifdef LCORELOG_H_INCLUDED
  void Log(const char * str = NULL) const
  {
    if( str != NULL )lLogFmt("%s:\n", str);

    for (int i = 0; i < NUM_HARMONICS; i++)
      lLogFmt(" %d: %f, %f, %f\n", i, m_harmonics[i](0), m_harmonics[i](1), m_harmonics[i](2) );
  }
  #endif

  void Add( const lmSphericalHarmonics &harmonics )
  {
    for (int i = 0; i < NUM_HARMONICS; i++)
    {
      m_harmonics[i] += harmonics.m_harmonics[i];
    }
  }

  void Scale( float s )
  {
    for (int i = 0; i < NUM_HARMONICS; i++)
    {
      m_harmonics[i] *= s;
    }
  }

  static void Lerp( lmSphericalHarmonics &result, const lmSphericalHarmonics &h0, const lmSphericalHarmonics &h1, float t)
  {
    for (int i = 0; i < NUM_HARMONICS; i++)
    {
      result.m_harmonics[i] = lmLerp( t, h0.m_harmonics[i], h1.m_harmonics[i] );
    }
  }

  float Difference( const lmSphericalHarmonics &h ) const
  {
    float scale[] = { 0.282905f, 0.488603f, 0.488603f, 0.488603f, 1.092548f, 1.092548f, 0.315392f, 1.092548f, 0.546274f };

    float result = 0.0f;
    for (int i = 0; i < NUM_HARMONICS; i++)
    {
      float diff = m_harmonics[i].Distance( h.m_harmonics[i] )  *  scale[i];
      result += diff;
    }

    return result;
  }

  bool operator==( const lmSphericalHarmonics &h ) const
  {
    // use array operator== to do the work
    return (m_harmonics == h.m_harmonics);
  }

  void ComputeVShaderConstants( lmVector4 *vShaderConstants, int numConstants ) const
  {
    lAssertAlways( numConstants == NUM_VSHADER_CONSTANTS );

    const float sqrtPi = lmSqrt( LM_PI );
    const float c0 = 1.0f  /  ( sqrtPi * 2.0f );
    const float c1 = lmSqrt( 3.0f )  /  ( sqrtPi * 3.0f );
    const float c2 = lmSqrt( 15.0f )  /  ( sqrtPi * 8.0f );
    const float c3 = lmSqrt( 5.0f )  /  ( sqrtPi * 16.0f );
    const float c4 = c2 * 0.5f;

    for (int i = 0; i < 3; i++)
    {
      vShaderConstants[i] = lmVector4( m_harmonics[3](i) * c1,                               //x
                                       m_harmonics[1](i) * c1,                               //y
                                       m_harmonics[2](i) * c1,                               //z
                                       m_harmonics[0](i) * c0  -  m_harmonics[6](i) * c3 );  //1.0

#ifdef L_PLATFORM_PS2
      vShaderConstants[i+3] = lmVector4( m_harmonics[5](i) * c2,                             //yz
                                         m_harmonics[7](i) * c2,                             //xz
                                         m_harmonics[4](i) * c2,                             //xy
                                         m_harmonics[6](i) * c3 * 3.0f );                    //zz

#else
      vShaderConstants[i+3] = lmVector4( m_harmonics[4](i) * c2,                             //xy
                                         m_harmonics[5](i) * c2,                             //yz
                                         m_harmonics[6](i) * c3 * 3.0f,                      //zz
                                         m_harmonics[7](i) * c2 );                           //xz
#endif

    }

    vShaderConstants[6] = lmVector4( m_harmonics[8](0) * c4,                                 //xx-yy
                                     m_harmonics[8](1) * c4,
                                     m_harmonics[8](2) * c4,
                                     0.0f );
  }

  friend lStream & operator<<( lStream &ar, lmSphericalHarmonics &c )
  {
    for( int i=0;i<NUM_HARMONICS;i++ )
    {
      ar << c.m_harmonics[i];
    }
    return ar;
  }


private:
  static void ComputeInputWeightTable(const lmVector3 &v, float scale, float *w)
  {
                                          //    const float fNormalise = LM_PI * (16.0f/17.0f);
    const float fConst1 =  4.f / 17.f;    //    const float mfConst1 = lmSq( 0.282905f ) * fNormalise * 1.0f;
    const float fConst2 =  8.f / 17.f;    //    const float mfConst2 = lmSq( 0.488603f ) * fNormalise * (2.0f/3.0f);
    const float fConst3 = 15.f / 17.f;    //    const float mfConst3 = lmSq( 1.092548f ) * fNormalise * (1.0f/4.0f);
    const float fConst4 =  5.f / 68.f;    //    const float mfConst4 = lmSq( 0.315392f ) * fNormalise * (1.0f/4.0f);
    const float fConst5 = 15.f / 68.f;    //    const float mfConst5 = lmSq( 0.546274f ) * fNormalise * (1.0f/4.0f);

    w[0] = scale * fConst1;
    w[1] = scale * fConst2 * v(1);
    w[2] = scale * fConst2 * v(2);
    w[3] = scale * fConst2 * v(0);
    w[4] = scale * fConst3 * v(0) * v(1);
    w[5] = scale * fConst3 * v(1) * v(2);
    w[6] = scale * fConst4 * ( 3.0f * v(2) * v(2)  -  1.0f );
    w[7] = scale * fConst3 * v(0) * v(2);
    w[8] = scale * fConst5 * ( v(0) * v(0)  -  v(1) * v(1) );
  }
};

END_L_NAMESPACE

#endif
