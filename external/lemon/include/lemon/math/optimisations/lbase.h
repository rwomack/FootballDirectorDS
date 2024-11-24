//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lbase.h
// Description : Base optimisations header file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LBASEOPT_H_INCLUDED
#define LBASEOPT_H_INCLUDED

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Generic MatrixElements
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<unsigned int N, unsigned int M,typename T,typename TSIMD>
struct MatrixElements
{
  T m_element[N*M];

  void InitElements()
  {
  }

  inline const T & ElementRef(const unsigned int i, const unsigned int j) const { return m_element[(i*M)+j]; }
  inline       T & ElementRef(const unsigned int i, const unsigned int j)       { return m_element[(i*M)+j]; }
  inline const T & ElementRef(const unsigned int i) const                       { return m_element[i]; }
  inline       T & ElementRef(const unsigned int i)                             { return m_element[i]; }
};

struct lSIMD_Base
{
  static inline void Memcpy( void *dst, const void *src, const int numbytes );
  static inline void Memset( void *dst, const int fill, const int numbytes );

  // Vector 1x3 functions

  static inline void  Vector1x3Copy(float *result, const float *v );
  static inline void  Vector1x3Add(float * result, const float *a, const float *b);
  static inline void  Vector1x3Sub(float *result, const float *a, const float *b);
  static inline void  Vector1x3Cross(float *result, const float *a, const float *b);
  static inline float Vector1x3Dot(const float * a, const float * b);
  static inline float Vector1x3Normalize(float *result);
  static inline float Vector1x3NormalizeScale(float *result, float len);
  static inline void  Vector1x3ComponentScale(float *result, const float *a, const float *b);
  static inline void  Vector1x3ScalarScale(float *result, const float *v, float scalar);

  // Vector 1x4 functions

  static inline void  Vector1x4Copy(float *result, const float *v );
  static inline void  Vector1x4Add(float * result, const float *a, const float *b);
  static inline void  Vector1x4Sub(float *result, const float *a, const float *b);
  static inline void  Vector1x4Cross(float *result, const float *a, const float *b);
  static inline float Vector1x4Dot(const float * a, const float * b);
  static inline float Vector1x4Normalize(float *result);
  static inline float Vector1x4NormalizeScale(float *result, float len);
  static inline void  Vector1x4ComponentScale(float *result, const float *a, const float *b);
  static inline void  Vector1x4ScalarScale(float *result, const float *v, float scalar);

  // Vector 1x3 / 1x4 multiplication functions

  static inline void Vector1x3MulMatrix3x3( float * r, const float * a, const float * b );
  static inline void Vector1x4MulMatrix4x4( float * r, const float * a, const float * b );
  static inline void Vector1x3MulMatrix4x3( float * r, const float * a, const float * b );
  static inline void Vector1x3MulMatrix4x4( float * r, const float * a, const float * b );

  // Matrix 3x3 functions

  static inline void Matrix3x3Copy(float *result, const float * m);
  static inline void Matrix3x3Identity(float *result);
  static inline void Matrix3x3Transpose( float * r, const float * a );
  static inline void Matrix3x3OrthoInverse( float * r, const float * a );
  static inline void Matrix3x3Mul( float * a, const float * b, const float * c );
  static inline void Matrix3x3ApplyScale( float *r , const float *a );

  // Matrix 4x3 functions

  static inline void Matrix4x3Mul( float * a, const float * b, const float * c );

  // Matrix 4x4 functions

  static inline void Matrix4x4Copy(float *result, const float * m);
  static inline void Matrix4x4Identity(float *result);
  static inline void Matrix4x4Transpose( float * r, const float * a );
  static inline void Matrix4x4OrthoInverse( float * r, const float * a );
  static inline void Matrix4x4Mul( float * a, const float * b, const float * c );
  static inline void Matrix4x4ApplyScale( float *r , const float *a );

  // Misc Matrix functions

  static inline void Convert3x3To4x4( float * r, const float * a );
  static inline void Convert4x4To3x3( float * r, const float * a );

  static inline void Convert1x4To1x3( float * r, const float * a );
  static inline void Convert1x3To1x4( float * r, const float * a );

  static inline void Get4x4Row0( float * r, const float * a );
  static inline void Get4x4Row1( float * r, const float * a );
  static inline void Get4x4Row2( float * r, const float * a );
  static inline void Get4x4Row3( float * r, const float * a );
  static inline void Get4x4Translate( float * r, const float * a );

  static inline void Get3x3Row0( float * r, const float * a );
  static inline void Get3x3Row1( float * r, const float * a );
  static inline void Get3x3Row2( float * r, const float * a );
};

/*struct lSIMD_SSE2 : public lSIMD_Base
{
  static inline void Vector1x3MulMatrix3x3( float * r, const float * a, const float * b );

};

struct lSIMD_Select
{

  static void Vector1x3MulMatrix3x3( float * r, const float * a, const float * b )
  {

    goodSSE->Vector1x3MulMatrix3x3( float * r, const float * a, const float * b );
  }

};


typedef lSIMD_SSE2 lSIMD;
typedef lSIMD_Select lSIMD;


lSIMD_Base *curSIMD =
 */

void lSIMD_Base::Memcpy( void *dst, const void *src, const int numbytes )
{
  memcpy( dst, src, numbytes );
}

void lSIMD_Base::Memset( void *dst, const int fill, const int numbytes )
{
  memset( dst, fill, numbytes );
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Vector 1x3 functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

inline void lSIMD_Base::Vector1x3Copy(float *result, const float *v )
{
  for( unsigned int i=0;i<3;i++ ) result[i] = v[i];
}

inline void lSIMD_Base::Vector1x3Add(float * result, const float *a, const float *b)
{
  for( unsigned int i=0;i<3;i++ ) result[i] = a[i] + b[i];
}

inline void lSIMD_Base::Vector1x3Sub(float *result, const float *a, const float *b)
{
  for( unsigned int i=0;i<3;i++ ) result[i] = a[i] - b[i];
}

inline void lSIMD_Base::Vector1x3Cross(float *result, const float *a, const float *b)
{
  result[0] = (a[1] * b[2]) - (a[2] * b[1]);
  result[1] = (a[2] * b[0]) - (a[0] * b[2]);
  result[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

inline float lSIMD_Base::Vector1x3Dot(const float * a, const float * b)
{
  float r = 0.0f;
  for( unsigned int i=0; i<3; i++ ) r += a[i] * b[i];
  return r;
}

inline float lSIMD_Base::Vector1x3Normalize(float *result)
{
  float l = lmSqrt( (result[0] * result[0]) + (result[1] * result[1]) + (result[2] * result[2]) );
  if( l == 0 ) return l;
  float ooLen = 1.0f / l;
  for(unsigned int i = 0; i<3; i++ ) result[i] *= ooLen;
  return l;
}

inline float lSIMD_Base::Vector1x3NormalizeScale(float *result, float len)
{
  float l = lmSqrt( (result[0] * result[0]) + (result[1] * result[1]) + (result[2] * result[2]) );
  if( l == 0 ) return l;
  float ooLen = len / l;
  for(unsigned int i = 0; i<3; i++ ) result[i] *= ooLen;
  return l;
}

inline void lSIMD_Base::Vector1x3ComponentScale(float *result, const float *a, const float *b)
{
  for( unsigned int i=0;i<3;i++ ) result[i] = a[i] * b[i];
}

inline void lSIMD_Base::Vector1x3ScalarScale(float *result, const float *v, float scalar)
{
  for( unsigned int i=0;i<3;i++ ) result[i] = v[i] * scalar;
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Vector 1x4 functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

inline void lSIMD_Base::Vector1x4Copy(float *result, const float *v )
{
  for( unsigned int i=0;i<4;i++ ) result[i] = v[i];
}

inline void lSIMD_Base::Vector1x4Add(float * result, const float *a, const float *b)
{
  for( unsigned int i=0;i<4;i++ ) result[i] = a[i] + b[i];
}

inline void lSIMD_Base::Vector1x4Sub(float *result, const float *a, const float *b)
{
  for( unsigned int i=0;i<4;i++ ) result[i] = a[i] - b[i];
}

inline void lSIMD_Base::Vector1x4Cross(float *result, const float *a, const float *b)
{
  result[0] = (a[1] * b[2]) - (a[2] * b[1]);
  result[1] = (a[2] * b[0]) - (a[0] * b[2]);
  result[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

inline float lSIMD_Base::Vector1x4Dot(const float * a, const float * b)
{
  float r = 0.0f;
  for( unsigned int i=0; i<4; i++ ) r += a[i] * b[i];
  return r;
}

inline float lSIMD_Base::Vector1x4Normalize(float *result)
{
  float l = lmSqrt( (result[0] * result[0]) + (result[1] * result[1]) + (result[2] * result[2]) + (result[3] * result[3]) );
  if( l == 0 ) return l;
  float ooLen = 1.0f / l;
  for(unsigned int i = 0; i<4; i++ ) result[i] *= ooLen;
  return l;
}

inline float lSIMD_Base::Vector1x4NormalizeScale(float *result, float len)
{
  float l = lmSqrt( (result[0] * result[0]) + (result[1] * result[1]) + (result[2] * result[2]) + (result[3] * result[3]) );
  if( l == 0 ) return l;
  float ooLen = len / l;
  for(unsigned int i = 0; i<4; i++ ) result[i] *= ooLen;
  return l;
}

inline void lSIMD_Base::Vector1x4ComponentScale(float *result, const float *a, const float *b)
{
  for( unsigned int i=0;i<4;i++ ) result[i] = a[i] * b[i];
}

inline void lSIMD_Base::Vector1x4ScalarScale(float *result, const float *v, float scalar)
{
  for( unsigned int i=0;i<4;i++ ) result[i] = v[i] * scalar;
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Vector multiplication functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// These functions are generated by the genmatrixcode tool
// see genmatrixcode.cpp

void lSIMD_Base::Vector1x3MulMatrix3x3( float * r, const float * a, const float * b )
{
  // 1x3 = 1x3 * 3x3

  r[0] = (a[0]*b[0]) + (a[1]*b[3]) + (a[2]*b[6]);
  r[1] = (a[0]*b[1]) + (a[1]*b[4]) + (a[2]*b[7]);
  r[2] = (a[0]*b[2]) + (a[1]*b[5]) + (a[2]*b[8]);
}

void lSIMD_Base::Vector1x4MulMatrix4x4( float * r, const float * a, const float * b )
{
  // 1x4 = 1x4 * 4x4

  r[0] = (a[0]*b[0]) + (a[1]*b[4]) + (a[2]*b[8])  + (a[3]*b[12]);
  r[1] = (a[0]*b[1]) + (a[1]*b[5]) + (a[2]*b[9])  + (a[3]*b[13]);
  r[2] = (a[0]*b[2]) + (a[1]*b[6]) + (a[2]*b[10]) + (a[3]*b[14]);
  r[3] = (a[0]*b[3]) + (a[1]*b[7]) + (a[2]*b[11]) + (a[3]*b[15]);
}

void lSIMD_Base::Vector1x3MulMatrix4x3( float * r, const float * a, const float * b )
{
  // 1x3 = 1x4 * 4x3
  // special case: assume incoming 1x3 is a 1x4 with w=1

  r[0] = (a[0]*b[0]) + (a[1]*b[3]) + (a[2]*b[6]) + (b[9]);
  r[1] = (a[0]*b[1]) + (a[1]*b[4]) + (a[2]*b[7]) + (b[10]);
  r[2] = (a[0]*b[2]) + (a[1]*b[5]) + (a[2]*b[8]) + (b[11]);
}

void lSIMD_Base::Vector1x3MulMatrix4x4( float * r, const float * a, const float * b )
{
  // 1x3 = 1x4 * 4x3
  // special case: assume incoming 1x3 is a 1x4 with w=1

  r[0] = (a[0]*b[0]) + (a[1]*b[4]) + (a[2]*b[8])  + (b[12]);
  r[1] = (a[0]*b[1]) + (a[1]*b[5]) + (a[2]*b[9])  + (b[13]);
  r[2] = (a[0]*b[2]) + (a[1]*b[6]) + (a[2]*b[10]) + (b[14]);
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Matrix 3x3 functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

inline void lSIMD_Base::Matrix3x3Copy(float *result, const float * m)
{
  for( unsigned int i=0;i<9;i++ ) result[i] = m[i];
}

inline void lSIMD_Base::Matrix3x3Identity(float *result)
{
  memset( result, 0, sizeof(float)*9 );
  result[0] = 1.0f;
  result[4] = 1.0f;
  result[8] = 1.0f;
}

void lSIMD_Base::Matrix3x3Mul( float * r, const float * a, const float * b )
{
  // 3x3 = 3x3 * 3x3

  r[0] = (a[0]*b[0]) + (a[1]*b[3]) + (a[2]*b[6]);
  r[1] = (a[0]*b[1]) + (a[1]*b[4]) + (a[2]*b[7]);
  r[2] = (a[0]*b[2]) + (a[1]*b[5]) + (a[2]*b[8]);
  r[3] = (a[3]*b[0]) + (a[4]*b[3]) + (a[5]*b[6]);
  r[4] = (a[3]*b[1]) + (a[4]*b[4]) + (a[5]*b[7]);
  r[5] = (a[3]*b[2]) + (a[4]*b[5]) + (a[5]*b[8]);
  r[6] = (a[6]*b[0]) + (a[7]*b[3]) + (a[8]*b[6]);
  r[7] = (a[6]*b[1]) + (a[7]*b[4]) + (a[8]*b[7]);
  r[8] = (a[6]*b[2]) + (a[7]*b[5]) + (a[8]*b[8]);
}

inline void lSIMD_Base::Matrix3x3Transpose( float * r, const float * m )
{
  r[0] = m[0];
  r[1] = m[3];
  r[2] = m[6];
  r[3] = m[1];
  r[4] = m[4];
  r[5] = m[7];
  r[6] = m[2];
  r[7] = m[5];
  r[8] = m[8];
}

inline void lSIMD_Base::Matrix3x3OrthoInverse( float * r, const float * a )
{
  // inverse matrix of an orthonormal 3*3 matrix is just it's transpose
  Matrix3x3Transpose( r, a );
}

inline void lSIMD_Base::Matrix3x3ApplyScale( float *r , const float *a )
{
  r[0] *= a[0];
  r[1] *= a[1];
  r[2] *= a[2];

  r[3] *= a[0];
  r[4] *= a[1];
  r[5] *= a[2];

  r[6] *= a[0];
  r[7] *= a[1];
  r[8] *= a[2];
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Matrix 4x3 functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void lSIMD_Base::Matrix4x3Mul( float * r, const float * a, const float * b )
{
  // 4x3 = 4x3 * 4x3
  // special case, treat the 4x3's are 4x4's with 0,0,0,1 in the last column

  // Assuming aR and bR are the sub 3x3 rotation part
  // Assuming aT and bT are the translation part

  // R' = aR * bR

  r[0] = (a[0]*b[0]) + (a[1]*b[3]) + (a[2]*b[6]);
  r[1] = (a[0]*b[1]) + (a[1]*b[4]) + (a[2]*b[7]);
  r[2] = (a[0]*b[2]) + (a[1]*b[5]) + (a[2]*b[8]);
  r[3] = (a[3]*b[0]) + (a[4]*b[3]) + (a[5]*b[6]);
  r[4] = (a[3]*b[1]) + (a[4]*b[4]) + (a[5]*b[7]);
  r[5] = (a[3]*b[2]) + (a[4]*b[5]) + (a[5]*b[8]);
  r[6] = (a[6]*b[0]) + (a[7]*b[3]) + (a[8]*b[6]);
  r[7] = (a[6]*b[1]) + (a[7]*b[4]) + (a[8]*b[7]);
  r[8] = (a[6]*b[2]) + (a[7]*b[5]) + (a[8]*b[8]);

  // T' = (aT * bR) + bT

  r[9]   = (a[9]*b[0]) + (a[10]*b[3]) + (a[11]*b[6]) + (b[9]);
  r[10]  = (a[9]*b[1]) + (a[10]*b[4]) + (a[11]*b[7]) + (b[10]);
  r[11]  = (a[9]*b[2]) + (a[10]*b[5]) + (a[11]*b[8]) + (b[11]);
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Matrix 4x4 functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

inline void lSIMD_Base::Matrix4x4Copy(float *result, const float * m)
{
  for( unsigned int i=0;i<16;i++ ) result[i] = m[i];
}

inline void lSIMD_Base::Matrix4x4Identity(float *result)
{
  memset( result, 0, sizeof(float)*16 );
  result[0]  = 1.0f;
  result[5]  = 1.0f;
  result[10] = 1.0f;
  result[15] = 1.0f;
}

inline void lSIMD_Base::Matrix4x4Transpose( float * r, const float * m )
{
  r[0] = m[0];
  r[1] = m[4];
  r[2] = m[8];
  r[3] = m[12];
  r[4] = m[1];
  r[5] = m[5];
  r[6] = m[9];
  r[7] = m[13];
  r[8] = m[2];
  r[9] = m[6];
  r[10] = m[10];
  r[11] = m[14];
  r[12] = m[3];
  r[13] = m[7];
  r[14] = m[11];
  r[15] = m[15];
}

void lSIMD_Base::Matrix4x4Mul( float * r, const float * a, const float * b )
{
  // 4x4 = 4x4 * 4x4

  r[0]  = (a[0]*b[0])  + (a[1]*b[4])  + (a[2]*b[8])   + (a[3]*b[12]);
  r[1]  = (a[0]*b[1])  + (a[1]*b[5])  + (a[2]*b[9])   + (a[3]*b[13]);
  r[2]  = (a[0]*b[2])  + (a[1]*b[6])  + (a[2]*b[10])  + (a[3]*b[14]);
  r[3]  = (a[0]*b[3])  + (a[1]*b[7])  + (a[2]*b[11])  + (a[3]*b[15]);
  r[4]  = (a[4]*b[0])  + (a[5]*b[4])  + (a[6]*b[8])   + (a[7]*b[12]);
  r[5]  = (a[4]*b[1])  + (a[5]*b[5])  + (a[6]*b[9])   + (a[7]*b[13]);
  r[6]  = (a[4]*b[2])  + (a[5]*b[6])  + (a[6]*b[10])  + (a[7]*b[14]);
  r[7]  = (a[4]*b[3])  + (a[5]*b[7])  + (a[6]*b[11])  + (a[7]*b[15]);
  r[8]  = (a[8]*b[0])  + (a[9]*b[4])  + (a[10]*b[8])  + (a[11]*b[12]);
  r[9]  = (a[8]*b[1])  + (a[9]*b[5])  + (a[10]*b[9])  + (a[11]*b[13]);
  r[10] = (a[8]*b[2])  + (a[9]*b[6])  + (a[10]*b[10]) + (a[11]*b[14]);
  r[11] = (a[8]*b[3])  + (a[9]*b[7])  + (a[10]*b[11]) + (a[11]*b[15]);
  r[12] = (a[12]*b[0]) + (a[13]*b[4]) + (a[14]*b[8])  + (a[15]*b[12]);
  r[13] = (a[12]*b[1]) + (a[13]*b[5]) + (a[14]*b[9])  + (a[15]*b[13]);
  r[14] = (a[12]*b[2]) + (a[13]*b[6]) + (a[14]*b[10]) + (a[15]*b[14]);
  r[15] = (a[12]*b[3]) + (a[13]*b[7]) + (a[14]*b[11]) + (a[15]*b[15]);
}

inline void lSIMD_Base::Matrix4x4OrthoInverse( float * r, const float * m )
{
  float t0,t1,t2;

  // inverse matrix of an orthonormal 4x4 matrix is just it's transpose
  // in the 3x3 section

  r[0]=m[0]; r[4]=m[1]; r[8]=m[2];
  r[1]=m[4]; r[5]=m[5]; r[9]=m[6];
  r[2]=m[8]; r[6]=m[9]; r[10]=m[10];

  // clear the end column

  r[3] = r[7] = r[11] = (float)0.0f; r[15] = (float)1.0f;

  // compute inverse of translation vector

  t0 = -m[12]; t1 = -m[13]; t2 = -m[14];

  r[12] = (t0 * r[0]) + (t1 * r[4]) + (t2 * r[8]);
  r[13] = (t0 * r[1]) + (t1 * r[5]) + (t2 * r[9]);
  r[14] = (t0 * r[2]) + (t1 * r[6]) + (t2 * r[10]);
}

inline void lSIMD_Base::Matrix4x4ApplyScale( float *r , const float *a )
{
  r[0]  *= a[0];
  r[1]  *= a[1];
  r[2]  *= a[2];

  r[4]  *= a[0];
  r[5]  *= a[1];
  r[6]  *= a[2];

  r[8]  *= a[0];
  r[9]  *= a[1];
  r[10] *= a[2];

  r[12] *= a[0];
  r[13] *= a[1];
  r[14] *= a[2];
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Misc Matrix functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

inline void lSIMD_Base::Convert3x3To4x4( float * r, const float * a )
{
  r[0] = a[0];
  r[1] = a[1];
  r[2] = a[2];
  r[3] = 0.0f;

  r[4] = a[3];
  r[5] = a[4];
  r[6] = a[5];
  r[7] = 0.0f;

  r[8] = a[6];
  r[9] = a[7];
  r[10] = a[8];
  r[11] = 0.0f;

  r[12] = 0.0f;
  r[13] = 0.0f;
  r[14] = 0.0f;
  r[15] = 1.0f;
}

inline void lSIMD_Base::Convert4x4To3x3( float * r, const float * a )
{
  r[0] = a[0];
  r[1] = a[1];
  r[2] = a[2];

  r[3] = a[4];
  r[4] = a[5];
  r[5] = a[6];

  r[6] = a[8];
  r[7] = a[9];
  r[8] = a[10];
}

inline void lSIMD_Base::Convert1x4To1x3( float * r, const float * a )
{
  r[0] = a[0];
  r[1] = a[1];
  r[2] = a[2];
}

inline void lSIMD_Base::Convert1x3To1x4( float * r, const float * a )
{
  r[0] = a[0];
  r[1] = a[1];
  r[2] = a[2];
  r[3] = 1.0f;
}

inline void lSIMD_Base::Get4x4Row0( float * r, const float * a )
{
  r[0] = a[0];
  r[1] = a[1];
  r[2] = a[2];
  r[3] = a[3];
}

inline void lSIMD_Base::Get4x4Row1( float * r, const float * a )
{
  r[0] = a[4];
  r[1] = a[5];
  r[2] = a[6];
  r[3] = a[7];
}

inline void lSIMD_Base::Get4x4Row2( float * r, const float * a )
{
  r[0] = a[8];
  r[1] = a[9];
  r[2] = a[10];
  r[3] = a[11];
}

inline void lSIMD_Base::Get4x4Row3( float * r, const float * a )
{
  r[0] = a[12];
  r[1] = a[13];
  r[2] = a[14];
  r[3] = a[15];
}

inline void lSIMD_Base::Get4x4Translate( float * r, const float * a )
{
  r[0] = a[12];
  r[1] = a[13];
  r[2] = a[14];
}

inline void lSIMD_Base::Get3x3Row0( float * r, const float * a )
{
  r[0] = a[0];
  r[1] = a[1];
  r[2] = a[2];
}

inline void lSIMD_Base::Get3x3Row1( float * r, const float * a )
{
  r[0] = a[3];
  r[1] = a[4];
  r[2] = a[5];
}

inline void lSIMD_Base::Get3x3Row2( float * r, const float * a )
{
  r[0] = a[6];
  r[1] = a[7];
  r[2] = a[8];
}

END_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Platform specific versions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef L_PLATFORM_PS2
  #include "./lps2.h"
#endif

#ifdef L_PLATFORM_PSP
  #include "./lpsp.h"
#endif

#ifdef L_PLATFORM_NDS
  #include "./lnds.h"
#endif

#ifdef L_PLATFORM_WII
  #include "./lwii.h"
#endif

#ifdef L_PLATFORM_SYMBIAN
  #include "./lsymbian.h"
#endif

#ifdef L_PLATFORM_WINDOWS
  #include "./lwin32.h"
#endif

#ifdef L_PLATFORM_XBOX
  #include "./lwin32.h"                 // Use same as Win32 (SSE)
#endif

BEGIN_L_NAMESPACE

#ifdef L_PLATFORM_PS2
  typedef lSIMD_VU0 lSIMD_Default;
#elif L_PLATFORM_PSP
  //typedef lSIMD_PSP lSIMD_Default;
  typedef lSIMD_Base lSIMD_Default;
#elif L_PLATFORM_NDS
  typedef lSIMD_NDS lSIMD_Default;
#elif L_PLATFORM_WII
  typedef lSIMD_WII lSIMD_Default;
#elif L_PLATFORM_SYMBIAN
  typedef lSIMD_SYMBIAN lSIMD_Default;
#elif L_PLATFORM_XBOX
  typedef lSIMD_Win32 lSIMD_Default;
#elif L_PLATFORM_XBOX360
  typedef lSIMD_Base lSIMD_Default;
#else
  typedef lSIMD_Win32 lSIMD_Default;
#endif

END_L_NAMESPACE


#endif
