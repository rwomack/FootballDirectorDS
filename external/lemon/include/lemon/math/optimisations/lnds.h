//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lnds.h
// Description : NDS support header file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "lbase.h"

#ifndef LNDSOPT_H_INCLUDED
#define LNDSOPT_H_INCLUDED

BEGIN_L_NAMESPACE

struct lSIMD_NDS : public lSIMD_Base
{
};

template<> struct MatrixElements<1,3,float,lSIMD_NDS>
{
  float m_element[4];

  void InitElements()
  {
  }

  inline const float & ElementRef(const unsigned int i, const unsigned int j) const { L_UNUSED_PARAM(i); return m_element[j]; }
  inline       float & ElementRef(const unsigned int i, const unsigned int j)       { L_UNUSED_PARAM(i); return m_element[j]; }
  inline const float & ElementRef(const unsigned int i) const                       { return m_element[i]; }
  inline       float & ElementRef(const unsigned int i)                             { return m_element[i]; }
};



END_L_NAMESPACE

#endif
