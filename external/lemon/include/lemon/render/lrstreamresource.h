//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrstreamresource.h
// Description : Common stream resource include
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "./lrenderapi.h"

#ifndef LRSTREAMRESOURCE_H_INCLUDED
#define LRSTREAMRESOURCE_H_INCLUDED

#include "./lrstreamresourcecommon.h"

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Platform specific implementation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// By default we include the stream resource for the compiling platform,
// however, other platforms can be included by the user if required.

#ifdef L_PLATFORM_WINDOWS
#include "./lrstreamresourcepc.h"
#define lrIndexStream  lrIndexStreamPC
#define lrVertexStream lrVertexStreamPC
#define lrCacheId      lrCacheIdPC
#endif

#ifdef L_PLATFORM_XBOX
#include "./lrstreamresourcexbox.h"
#define lrIndexStream  lrIndexStreamXbox
#define lrVertexStream lrVertexStreamXbox
#define lrCacheId      lrCacheIdXbox
#endif

#ifdef L_PLATFORM_XBOX360
#include "./lrstreamresourcexbox360.h"
#define lrIndexStream  lrIndexStreamXbox360
#define lrVertexStream lrVertexStreamXbox360
#define lrCacheId      lrCacheIdXbox360
#endif


#ifdef L_PLATFORM_PS2
#include "./lrstreamresourceps2.h"
#define lrIndexStream  lrIndexStreamPS2
#define lrVertexStream lrVertexStreamPS2
#define lrCacheId      lrCacheIdPS2
#endif

#ifdef L_PLATFORM_PSP
#include "./lrstreamresourcepsp.h"
#define lrIndexStream  lrIndexStreamPSP
#define lrVertexStream lrVertexStreamPSP
#define lrCacheId      lrCacheIdPSP
#endif

#ifdef L_PLATFORM_NDS
#include "./lrstreamresourcends.h"
#define lrIndexStream  lrIndexStreamNDS
#define lrVertexStream lrVertexStreamNDS
#define lrCacheId      lrCacheIdNDS
#endif

#ifdef L_PLATFORM_WII
#include "./lrstreamresourcewii.h"
#define lrIndexStream  lrIndexStreamWII
#define lrVertexStream lrVertexStreamWII
#define lrCacheId      lrCacheIdWII
#endif

#ifdef L_PLATFORM_SYMBIAN
#include "./lrstreamresourcesymbian.h"
#define lrIndexStream  lrIndexStreamSymbian
#define lrVertexStream lrVertexStreamSymbian
#define lrCacheId      lrCacheIdSymbian
#endif

#endif
