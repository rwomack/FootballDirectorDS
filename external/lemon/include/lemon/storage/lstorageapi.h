//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lstorageapi.h
// Description : lStorage API definition include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LSTORAGEAPI_H_INCLUDED
#define LSTORAGEAPI_H_INCLUDED

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LSTORAGE_BUILD
#  define LSTORAGE_API L_EXPORT
#else
#  define LSTORAGE_API L_IMPORT
#endif

#endif

