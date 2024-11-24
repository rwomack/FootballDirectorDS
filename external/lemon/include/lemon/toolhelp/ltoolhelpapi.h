//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : ltoolhelpapi.h
// Description : Tool help API definition include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LTOOLHELPAPI_H_INCLUDED
#define LTOOLHELPAPI_H_INCLUDED

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LTOOLHELP_BUILD
#  define LTOOLHELP_API L_EXPORT
#else
#  define LTOOLHELP_API L_IMPORT
#endif

#endif

