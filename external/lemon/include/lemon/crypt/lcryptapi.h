//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lcryptapi.h
// Description : lCrypt API definition include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LCRYPTAPI_H_INCLUDED
#define LCRYPTAPI_H_INCLUDED

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LCRYPT_BUILD
#  define LCRYPT_API L_EXPORT
#else
#  define LCRYPT_API L_IMPORT
#endif

#endif

