//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : linputapi.h
// Description : Input API definition include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LINPUTAPI_H_INCLUDED
#define LINPUTAPI_H_INCLUDED

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LINPUT_BUILD
#  define LINPUT_API L_EXPORT
#else
#  define LINPUT_API L_IMPORT
#endif

#include "../platform/lcore.h"

#include "../io/lstream.h"
#include "../containers/larray.h"
#include "../containers/llist.h"
#include "../math/lmath.h"

#endif

