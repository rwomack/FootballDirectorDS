//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lxmlapi.h
// Description : lXML API definition include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LXMLAPI_H_INCLUDED
#define LXMLAPI_H_INCLUDED

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LXML_BUILD
#  define LXML_API L_EXPORT
#else
#  define LXML_API L_IMPORT
#endif

#endif

