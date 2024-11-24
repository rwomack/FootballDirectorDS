//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrshaderresourcedx.h
// Description : DirectX shader resource include
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "./lrenderapi.h"

#ifndef LRSHADERRESOURCEDX_H_INCLUDED
#define LRSHADERRESOURCEDX_H_INCLUDED

#include "./lrshaderresourcedxcommon.h"

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Platform specific implementation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// By default we include the shader resource for the compiling platform,
// however, other platforms can be included by the user if required.

#ifdef L_PLATFORM_WINDOWS
#include "./lrshaderresourcedxpc.h"
#define lrCompiledVS lrCompiledVSPC
#define lrCompiledPS lrCompiledPSPC
#endif

#ifdef L_PLATFORM_XBOX
#include "./lrshaderresourcedxxbox.h"
#define lrCompiledVS lrCompiledVSXbox
#define lrCompiledPS lrCompiledPSXbox
#endif

#ifdef L_PLATFORM_XBOX360
#include "./lrshaderresourcedxxbox360.h"
#define lrCompiledVS lrCompiledVSXbox360
#define lrCompiledPS lrCompiledPSXbox360
#endif


#ifdef L_PLATFORM_PS2
// Not implemented on PS2
#endif


#endif
