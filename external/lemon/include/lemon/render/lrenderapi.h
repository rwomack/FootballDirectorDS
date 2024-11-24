//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrenderapi.h
// Description : Render API definition include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LRENDERAPI_H_INCLUDED
#define LRENDERAPI_H_INCLUDED

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LRENDER_BUILD
#  define LRENDER_API L_EXPORT
#else
#  define LRENDER_API L_IMPORT
#endif

#include "../platform/lcore.h"
#include "../io/lstream.h"
#include "../containers/larray.h"
#include "../math/lmmatrix.h"
#include "../containers/llist.h"
#include "../containers/lstring.h"

#include "./lresource.h"
#include "./rcptr.h"

inline void RenderSystemError_Temp( const char * str, ... )
{
  L_UNUSED_PARAM( str );
  lBreak();
}

#include "../containers/larray.h"
// render array trait
BEGIN_L_NAMESPACE
typedef TArrayTrait<3,8,16,L_ARENA_RENDER> TRenderArrayTrait;  // 3/8 + 16
END_L_NAMESPACE


#endif

