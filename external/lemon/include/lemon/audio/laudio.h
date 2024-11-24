//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : laudio.h
// Description : Common audio include
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

// include the api linkage definitions

#include "./laudioapi.h"

#ifndef LAUDIO_H_INCLUDED
#define LAUDIO_H_INCLUDED

#include "./laudiocommon.h"

// switch to platform specific implementation

#ifdef L_PLATFORM_WINDOWS
#include "./laudiopc.h"
#endif

#ifdef L_PLATFORM_PS2
#include "./laudiops2.h"
#endif

#ifdef L_PLATFORM_PSP
#include "./laudiopsp.h"
#endif

#ifdef L_PLATFORM_WII
#include "./laudiowii.h"
#endif

#ifdef L_PLATFORM_NDS
#include "./laudionds.h"
#endif

#endif
