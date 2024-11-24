//����������������������������������������������������������������������������
// File        : lrender.h
// Description : Common rendering interface
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "./lrenderapi.h"

#ifndef LRENDER_H_INCLUDED
#define LRENDER_H_INCLUDED

#include "./lrtextureresource.h"
#include "./lrstreamresource.h"

#include "./lrendertypes.h"
#include "./lrendercommon.h"

//����������������������������������������������������������������������������
// Platform specific render implementation
//����������������������������������������������������������������������������

#ifdef L_PLATFORM_WINDOWS
#include "./lrenderpc.h"
#endif

#ifdef L_PLATFORM_XBOX
#include "./lrenderxbox.h"
#endif

#ifdef L_PLATFORM_XBOX360
#include "./lrenderxbox360.h"
#endif

#ifdef L_PLATFORM_PS2
#include "./lrenderps2.h"
#endif

#ifdef L_PLATFORM_PSP
#include "./lrenderpsp.h"
#endif

#ifdef L_PLATFORM_NDS
#include "./lrendernds.h"
#endif

#ifdef L_PLATFORM_WII
#include "./lrenderwii.h"
#endif

#ifdef L_PLATFORM_SYMBIAN
#include "./lrendersymbian.h"
#endif

#endif
