//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lwin32.h
// Description : Win32 support header file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "lbase.h"

#ifndef LWIN32OPT_H_INCLUDED
#define LWIN32OPT_H_INCLUDED

BEGIN_L_NAMESPACE

struct lSIMD_Win32 : public lSIMD_Base
{
};

template<> struct MatrixElements<1,3,float,lSIMD_Win32>
{
  float m_element[4];

  void InitElements()
  {
  }

  inline const float & ElementRef(const unsigned int i, const unsigned int j) const { L_UNUSED_PARAM( i ); return m_element[j]; }
  inline       float & ElementRef(const unsigned int i, const unsigned int j)       { L_UNUSED_PARAM( i ); return m_element[j]; }
  inline const float & ElementRef(const unsigned int i) const                       { return m_element[i]; }
  inline       float & ElementRef(const unsigned int i)                             { return m_element[i]; }
};

#if 0
struct lSIMD_SSE : public lSIMD_Base
{
  static inline void Vector1x4MulMatrix4x4( float * r, const float * a, const float * b );
  static inline void Matrix4x4Mul( float * a, const float * b, const float * c );
};

//##
// SSE optimised vector 1x4 by 4x4
inline void lSIMD_SSE::Vector1x4MulMatrix4x4( float * r, const float * a, const float * b )
{
  // 1x4 = 1x4 * 4x4
__asm {
  mov     ecx,  dword ptr [esp+ 12] ; b
  mov     edx,  dword ptr [esp+ 8]  ; a
  movlps  xmm6, qword ptr [ecx ]
  movlps  xmm0, qword ptr [edx ]
  shufps  xmm6, xmm6, 0x44
  movhps  xmm0, qword ptr [edx+16]
  mulps   xmm0, xmm6
  movlps  xmm7, qword ptr [ecx+ 8]
  movlps  xmm2, qword ptr [edx+ 8]
  shufps  xmm7, xmm7, 0x44
  movhps  xmm2, qword ptr [edx+24]
  mulps   xmm2, xmm7
  movlps  xmm1, qword ptr [edx+32]
  movhps  xmm1, qword ptr [edx+48]
  mulps   xmm1, xmm6
  movlps  xmm3, qword ptr [edx+40]
  addps   xmm0, xmm2
  movhps  xmm3, qword ptr [edx+56]
  mov     eax,  dword ptr [esp+4] ; r
  mulps   xmm3, xmm7
  movaps  xmm4, xmm0
  addps   xmm1, xmm3
  shufps  xmm4, xmm1, 0x88
  shufps  xmm0, xmm1, 0xDD
  addps   xmm0, xmm4
  movaps  xmmword ptr [eax], xmm0
};
}

//##
// SSE optimised 4x4 by 4x4
inline void lSIMD_SSE::Matrix4x4Mul( float * r, const float * a, const float * b )
{
  // 4x4 = 4x4 * 4x4
__asm {
  mov edx, dword ptr [esp+8]  ; a
  mov eax, dword ptr [esp+4]  ; r
  mov ecx, dword ptr [esp+12] ; b
  movss xmm0, dword ptr [edx]
  movaps xmm1, xmmword ptr [ecx]
  shufps xmm0, xmm0, 0
  movss xmm2, dword ptr [edx+4]
  mulps xmm0, xmm1
  shufps xmm2, xmm2, 0
  movaps xmm3, xmmword ptr [ecx+10h]
  movss xmm7, dword ptr [edx+8]
  mulps xmm2, xmm3
  shufps xmm7, xmm7, 0
  addps xmm0, xmm2
  movaps xmm4, xmmword ptr [ecx+20h]
  movss xmm2, dword ptr [edx+0Ch]
  mulps xmm7, xmm4
  shufps xmm2, xmm2, 0
  addps xmm0, xmm7
  movaps xmm5, xmmword ptr [ecx+30h]
  movss xmm6, dword ptr [edx+10h]
  mulps xmm2, xmm5
  movss xmm7, dword ptr [edx+14h]
  shufps xmm6, xmm6, 0
  addps xmm0, xmm2
  shufps xmm7, xmm7, 0
  movlps qword ptr [eax], xmm0
  movhps qword ptr [eax+8], xmm0
  mulps xmm7, xmm3
  movss xmm0, dword ptr [edx+18h]
  mulps xmm6, xmm1
  shufps xmm0, xmm0, 0
  addps xmm6, xmm7
  mulps xmm0, xmm4
  movss xmm2, dword ptr [edx+24h]
  addps xmm6, xmm0
  movss xmm0, dword ptr [edx+1Ch]
  movss xmm7, dword ptr [edx+20h]
  shufps xmm0, xmm0, 0
  shufps xmm7, xmm7, 0
  mulps xmm0, xmm5
  mulps xmm7, xmm1
  addps xmm6, xmm0
  shufps xmm2, xmm2, 0
  movlps qword ptr [eax+10h], xmm6
  movhps qword ptr [eax+18h], xmm6
  mulps xmm2, xmm3
  movss xmm6, dword ptr [edx+28h]
  addps xmm7, xmm2
  shufps xmm6, xmm6, 0
  movss xmm2, dword ptr [edx+2Ch]
  mulps xmm6, xmm4
  shufps xmm2, xmm2, 0
  addps xmm7, xmm6
  mulps xmm2, xmm5
  movss xmm0, dword ptr [edx+34h]
  addps xmm7, xmm2
  shufps xmm0, xmm0, 0
  movlps qword ptr [eax+20h], xmm7
  movss xmm2, dword ptr [edx+30h]
  movhps qword ptr [eax+28h], xmm7
  mulps xmm0, xmm3
  shufps xmm2, xmm2, 0
  movss xmm6, dword ptr [edx+38h]
  mulps xmm2, xmm1
  shufps xmm6, xmm6, 0
  addps xmm2, xmm0
  mulps xmm6, xmm4
  movss xmm7, dword ptr [edx+3Ch]
  shufps xmm7, xmm7, 0
  addps xmm2, xmm6
  mulps xmm7, xmm5
  addps xmm2, xmm7
  movaps xmmword ptr [eax+30h], xmm2
};
#endif

END_L_NAMESPACE

#endif
