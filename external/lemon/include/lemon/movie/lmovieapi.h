//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmovieapi.h
// Description : lMovie API definition include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMOVIEAPI_H_INCLUDED
#define LMOVIEAPI_H_INCLUDED

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LMOVIE_BUILD
#  define LMOVIE_API L_EXPORT
#else
#  define LMOVIE_API L_IMPORT
#endif

#endif

