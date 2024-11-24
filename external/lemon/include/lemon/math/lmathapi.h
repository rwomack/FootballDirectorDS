//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmathapi.h
// Description : lMath API definition include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMATHAPI_H_INCLUDED
#define LMATHAPI_H_INCLUDED

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LMATH_BUILD
#  define LMATH_API L_EXPORT
#else
#  define LMATH_API L_IMPORT
#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Math typedefs
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#include "../containers/larray.h"

// math array trait
BEGIN_L_NAMESPACE
typedef TArrayTrait<3,8,16,L_ARENA_MATH> TMathArrayTrait;  // 3/8 + 16
END_L_NAMESPACE

#endif
