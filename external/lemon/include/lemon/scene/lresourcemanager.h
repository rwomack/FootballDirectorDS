//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lresourcemanager.h
// Description : Resource manager
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "./lsceneapi.h"

#ifndef LRESOURCEMANAGER_H_INCLUDED
#define LRESOURCEMANAGER_H_INCLUDED

#include "../object/lsharedptr.h"
#include "../render/lrtextureresource.h"  // for lrTextureResrouce

BEGIN_L_NAMESPACE

class lSceneNode;
class lAnimNode;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lResourceManager - Resource manager
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LSCENE_API lResourceManager
{
public:
  static void Init();
  static void Deinit();

  static RcPtr<lrTextureResource>    GetTexture( const char * );
  static lSharedPtr<lSceneNode>      GetScene( const char * );
  static lSharedPtr<lAnimNode>       GetAnim( const char * );

#ifdef L_PLATFORM_WIN32
  static void                        PreConvertTexture( const char * );
#endif

  static void AddTextureDirectory( const char * );
  static void AddSceneDirectory( const char * );
  static void AddAnimDirectory( const char * );

  static void AddTexture( RcPtr<lrTextureResource>, const char * );
  static void AddScene( lSharedPtr<lSceneNode>, const char * );
  static void AddAnim( lSharedPtr<lAnimNode>, const char * );

private:

  class Impl;
  static lSharedPtr<Impl> s_pimpl;
};


END_L_NAMESPACE

#endif
