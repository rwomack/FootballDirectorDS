//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdresourcemanager.h
// Description : FDResourceManager interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDRESOURCEMANAGER_H_INCLUDED
#define FDRESOURCEMANAGER_H_INCLUDED

#include <lemon/lapi.h>
#include <lemon/object/lsharedptr.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lFont;
class lSpriteBank;
class lSprite;
class lIOStreamSeekable;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDResourceManager
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDResourceManager
{
public:
  static void Init();
  static void Deinit();

  static lSharedPtr<lFont>       GetDefaultFont();
  static lSharedPtr<lFont>       GetFont( const char * resourceId, const char * fontId, bool bStream=true );
  static lSharedPtr<lSpriteBank> GetSpriteBank( const char * resourceId, bool bStream=true );
  static lSharedPtr<lSprite>     GetSprite( const char * bankId, const char * spriteId );
  static lSharedPtr<lSprite>     GetSpriteBG( const char * spriteId );
  static lIOStreamSeekable     * GetFile( const char* fileId );
};


END_L_NAMESPACE
#endif

