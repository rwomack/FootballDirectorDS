//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrtextureresourcecommon.h
// Description : Common texture resource include
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#include "./lrenderapi.h"

#ifndef LRTEXTURERESOURCE_H_INCLUDED
#define LRTEXTURERESOURCE_H_INCLUDED

#include "./lrtextureresourcecommon.h"

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Platform specific implementation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// By default we include the texture resource for the compiling platform,
// however, other platforms can be included by the user if required.

#ifdef L_PLATFORM_WINDOWS
#include "./lrtextureresourcepc.h"
#define lrTextureResource lrTextureResourcePC
#endif

#ifdef L_PLATFORM_XBOX
#include "./lrtextureresourcexbox.h"
#define lrTextureResource lrTextureResourceXbox
#endif

#ifdef L_PLATFORM_XBOX360
#include "./lrtextureresourcexbox360.h"
#define lrTextureResource lrTextureResourceXbox360
#endif

#ifdef L_PLATFORM_PS2
#include "./lrtextureresourceps2.h"
#define lrTextureResource lrTextureResourcePS2
#endif

#ifdef L_PLATFORM_PSP
#include "./lrtextureresourcepsp.h"
#define lrTextureResource lrTextureResourcePSP
#endif

#ifdef L_PLATFORM_NDS
#include "./lrtextureresourcends.h"
#define lrTextureResource lrTextureResourceNDS
#endif

#ifdef L_PLATFORM_WII
#include "./lrtextureresourcewii.h"
#define lrTextureResource lrTextureResourceWII
#endif

#ifdef L_PLATFORM_SYMBIAN
#include "./lrtextureresourcesymbian.h"
#define lrTextureResource lrTextureResourceSymbian
#endif


#endif
