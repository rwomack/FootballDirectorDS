//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : dsrender.h
// Description : DSRender
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef __DSRENDER_H
#define __DSRENDER_H

#ifdef L_PLATFORM_NDS

#pragma showmessagenumber on
#pragma warning off (10317) // type conversion
#pragma warning off (10182) // unused parameters
#pragma warning off (10107) // triling commas on declaration lists
#pragma warning off (10216) // empty declarations
#pragma warning off (10178) // function has no prototype

#include <lemon/platform/lcore.h>

BEGIN_L_NAMESPACE

void DSRender_Init(void);
void DSRender_StartDisplay(void);
void DSRender_StartFrame(void);
void DSRender_SwapScreens(void);
int  DSRender_GetScreenIndex(void);
void DSRender_SetBookMode(int);
int  DSRender_GetBookMode(void);

END_L_NAMESPACE

#endif

#endif

