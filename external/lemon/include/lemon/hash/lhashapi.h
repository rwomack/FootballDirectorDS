//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lhashapi.h
// Description : lHash API definition include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LHASHAPI_H_INCLUDED
#define LHASHAPI_H_INCLUDED

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LHASH_BUILD
#  define LHASH_API L_EXPORT
#else
#  define LHASH_API L_IMPORT
#endif

#endif

