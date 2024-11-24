//����������������������������������������������������������������������������
// File        : lrendertypes.h
// Description : Common rendering types
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "./lrenderapi.h"

#ifndef LRENDERTYPES_H_INCLUDED
#define LRENDERTYPES_H_INCLUDED

BEGIN_L_NAMESPACE

//����������������������������������������������������������������������������
// lrTransformType - Transform types for renderer
//����������������������������������������������������������������������������

enum lrTransformType
{
  LR_TRANSFORM_WORLD      = 0x0,
  LR_TRANSFORM_VIEW       = 0x1,
  LR_TRANSFORM_PROJECTION = 0x2,
  LR_TRANSFORM_WVP        = 0x3
};

//����������������������������������������������������������������������������
// lrPrimitiveType - Primitive types for renderer
//����������������������������������������������������������������������������

enum lrPrimitiveType
{
  LR_PRIM_TRILIST    = 0x0,
  LR_PRIM_LINELIST   = 0x1,
  LR_PRIM_POINTLIST  = 0x2,
  LR_PRIM_TRISTRIP   = 0x3,
  LR_PRIM_USER       = 0x100,            //# User primitive type, intended for a user defined shader to understand
};

//����������������������������������������������������������������������������
// lrCullMode - Culling mode for primitives
//����������������������������������������������������������������������������

enum lrCullMode
{
  LR_CULL_ANTICLOCKWISE = 0x0,
  LR_CULL_CLOCKWISE     = 0x1,
  LR_CULL_NONE          = 0x2,
};

inline lrCullMode lrInvertCullMode(lrCullMode mode)
{
  switch ( mode )
  {
    case LR_CULL_ANTICLOCKWISE:
      return LR_CULL_CLOCKWISE;
    case LR_CULL_CLOCKWISE:
      return LR_CULL_ANTICLOCKWISE;
    case LR_CULL_NONE:
      return LR_CULL_NONE;
    default:
      return LR_CULL_NONE;
  }
}

L_STREAM_ENUM( lrCullMode );

END_L_NAMESPACE
#endif
