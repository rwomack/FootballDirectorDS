//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lioapi.h
// Description : lIO API definition include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LIOAPI_H_INCLUDED
#define LIOAPI_H_INCLUDED

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LIO_BUILD
#  define LIO_API L_EXPORT
#else
#  define LIO_API L_IMPORT
#endif

#endif

