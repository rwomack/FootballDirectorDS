//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : laudioapi.h
// Description : Audio API definition include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LAUDIOAPI_H_INCLUDED
#define LAUDIOAPI_H_INCLUDED

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LAUDIO_BUILD
#  define LAUDIO_API L_EXPORT
#else
#  define LAUDIO_API L_IMPORT
#endif

#endif

