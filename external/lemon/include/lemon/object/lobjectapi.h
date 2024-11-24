//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lobjectapi.h
// Description : lObject API definition include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LOBJECTAPI_H_INCLUDED
#define LOBJECTAPI_H_INCLUDED

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LOBJECT_BUILD
#  define LOBJECT_API L_EXPORT
#else
#  define LOBJECT_API L_IMPORT
#endif

#endif

