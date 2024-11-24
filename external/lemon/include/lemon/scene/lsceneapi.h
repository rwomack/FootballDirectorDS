//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lsceneapi.h
// Description : Scene API definition include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LSCENEAPI_H_INCLUDED
#define LSCENEAPI_H_INCLUDED

// include the api linkage definitions

#include "../lapi.h"
#include "../containers/larray.h"

// api linkage

#ifdef LSCENE_BUILD
#  define LSCENE_API L_EXPORT
#else
#  define LSCENE_API L_IMPORT
#endif

// scene array trait
BEGIN_L_NAMESPACE
typedef TArrayTrait<3,8,16,L_ARENA_SCENE> TSceneArrayTrait;  // 3/8 + 16
END_L_NAMESPACE


#endif

