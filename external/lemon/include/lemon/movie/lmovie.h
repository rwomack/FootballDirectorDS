//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±s±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : lmovie.h
// Description : Movie playback class
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMOVIE_H_INCLUDED
#define LMOVIE_H_INCLUDED

#include "../containers/larray.h"
#include "../render/lrender.h"
#include "lmovieapi.h"

BEGIN_L_NAMESPACE

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lMovie - movie controls
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

enum
{
  LMOVIE_RENDERFLAG_KEEPASPECT = 0x00001,  //## keep aspect ratio of movie
  LMOVIE_RENDERFLAG_CENTRE     = 0x00002,  //## positional coordinates relative to centre
  LMOVIE_RENDERFLAG_DONT_DROP  = 0x00002,  //## don't drop any frames (note: may desync things)
};

class LMOVIE_API lMovie
{
 public:
  lMovie()
  {
  }

  virtual ~lMovie()
  {
  }

  // playback controls
  virtual bool      Play( bool bLoop ) = 0;
  virtual bool      Pause() = 0;
  virtual bool      Resume() = 0;
  virtual bool      Stop() = 0;

  virtual bool      IsPlaying() = 0;

  virtual bool      Preload()
  {
    return false;
  }

  // volume controls
  virtual void      SetVolume( float f ) = 0;
  virtual float     GetVolume() = 0;

//  // playback speed
//  virtual void      SetSpeed( float f ) = 0;
//  virtual float     GetSpeed() = 0;

  // loop controls
  virtual void      SetLoop( bool b ) = 0;
  virtual bool      GetLoop() = 0;

   virtual void      Tick( float delta ) = 0;

  // default these functions do nothing
   virtual int      GetFrame() { return 0; }
   virtual float    GetTime()  { return 0.0f; }

  // output
  virtual void     SetRenderPos( bool bRenderToScreen, int renderFlags = 0, float x = 0.0f, float y = 0.0f, float w = 0.0f, float h = 0.0f ) = 0;
  virtual RcPtr<lrTextureResource> GetTexture() = 0;
  virtual void     Render() = 0;

  virtual bool     GetMovieInfoStr( char *str, int strsize ) { return false; }

  // TODO: output format selection, rescaling,
  // TODO: Provide way to query the size / type of image created in the texture
  //       Also operations to change these - however these may fail if the implementation doesn't support other formats.
};

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lMoviePlayer
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

enum
{
  LMOVIE_CREATEFLAG_STREAMCD   = 0x00001,   //## use streaming file from cd for the current platform
  LMOVIE_CREATEFLAG_STREAMDVD  = 0x00002,   //## use streaming file from dvd for the current platform
  LMOVIE_CREATEFLAG_CACHEFILE  = 0x00004,   //## load entire file into memory to playback
  LMOVIE_CREATEFLAG_TEXTURE    = 0x00008,   //## render file to texture
  LMOVIE_CREATEFLAG_FULLSCREEN = 0x00010,   //## render file to the full size of the screen with aspect ratio kept intact, safe areas will be adhered to
};

class LMOVIE_API lMovieManager
{
 public:
  static void Init();
  static void Deinit();

  // Init / Deinit for full screen mode initialised by lMovieManager
  static void InitFullscreen( bool bInitAudio = true );
  static void DeinitFullscreen();

  //##
  // Create an lMovie class which controls the movie specified in the parameters to this function.
  // The pointer to the lMovie class returned is to be managed by the user application
  //
  // In:
  //   fileName       - Filename of movie to load, the interface may use the extension to establish the file format of the file loaded
  //   flags          - Specifies creation flags for this lMovie (see LMOVIE_CREATEFLAG_XXXX) describing how the movie is to be loaded / streamed / played.
  //   videoStreamIdx - Specifies which video stream to playback and control with the lMovie class
  //   audioStreamIdx - Specifies which audio stream to playback and control with the lMovie class
  //
  // Out:
  //   If successful returns a pointer to an lMovie class for controlling playback and rendering of the movie.
  //   If an error occured performing the creation (or possible loading of the file depending on the lMovie implementation) the
  //   returned value will be NULL.
  static lMovie * CreateMovie( const char *fileName, int flags, int videoStreamIdx = 0, int audioStreamIdx = 0 );

  // Tick functions
  // NOTE: TODO: These may be used to do any per platform operations or they could be used to perform ticks on all the
  //             lMovie classes currently created.. or do we let the user manage independent ticking of each movie class ?
  static void     Tick( float delta );
  static void     TickIdle( float idleTime );
};


END_L_NAMESPACE

#endif
