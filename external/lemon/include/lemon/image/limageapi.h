//����������������������������������������������������������������������������
// File        : limageapi.h
// Description : lImage API definition include file
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LIMAGEAPI_H_INCLUDED
#define LIMAGEAPI_H_INCLUDED

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LIMAGE_BUILD
#  define LIMAGE_API L_EXPORT
#else
#  define LIMAGE_API L_IMPORT
#endif

//����������������������������������������������������������������������������
// Image typedefs
//����������������������������������������������������������������������������

#include "../containers/larray.h"

// image array trait
BEGIN_L_NAMESPACE
typedef TArrayTrait<3,8,16,L_ARENA_IMAGE> TImageArrayTrait;  // 3/8 + 16
END_L_NAMESPACE

#endif
