//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lsprite.h
// Description : lSprite
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef __LSPRITE_H
#define __LSPRITE_H

#include <framework/lspritebank.h>
#include <framework/lspritemanager.h> // for flip flags
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

typedef float Scalar;
typedef lmMatrix<1,2,Scalar>           Position;  //## A 2D fixed point position
typedef Scalar                         Rotation;  //## A 2D fixed point rotation
typedef lmMatrix<1,2,Scalar>           Scale;     //## A 2D fixed point scale
typedef uint32 SizeT;                             //## A size type
typedef lmMatrix<1,2,Scalar>           Anchor;    //## A 2D fixed point position

class lTimer;
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// PlayController
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum PlayMode
{
  PLAY_LOOP,
  PLAY_LOOP_REVERSE,
  PLAY_LOOP_PINGPONG,
  PLAY_ONCE,
  PLAY_ONCE_REVERSE,
  PLAY_ONCE_PINGPONG,
};

class PlayController : public lHasSlots<>
{
protected:
  SizeT    m_numFrames;
  Scalar   m_frameRate; // int or scalar? was int. does code support this?

  PlayMode m_playMode;
  Scalar   m_playDirection;
  bool     m_bIsPlaying;
  bool     m_bIsPaused;
  Scalar   m_currentTime;
  SizeT    m_currentFrame;
  SizeT    m_currentLoop;
  lTimer*  m_timer;

public:

  PlayController();
  ~PlayController();

  void   Play( PlayMode );
  void   Stop();
  void   Pause();
  void   Resume();
  void   CompleteLoop();
  bool   IsPlaying();
  bool   IsPaused();

  void   SetFrame( SizeT frame );
  SizeT  GetFrame();
  void   SetTime( Scalar t );
  Scalar GetTime();

  void   SetTimer( lTimer* parentTimer );
  void   RemoveTimer();
  void   NextFrame();

  SizeT  GetNumFrames();

  void   SetFrameRate( Scalar );
  Scalar GetFrameRate();

  void   AdvancePlayTime( Scalar delta );
  SizeT  GetLoopCount();

  lSignal0<> SignalFinished;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Sprite
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lSprite : public PlayController
{
public:
  lSprite();
  ~lSprite();

  lSprite * Clone();

  static lSprite * Create( lSharedPtr<lSpriteBank> bank, const char * id );

  void  Tick(float delta);
  void  Render();

  void                   SetFlip( uint32 flip );
  uint32                 GetFlip() const;

  void                   SetColour( uint32 colour );
  uint32                 GetColour(void);

  // Set to use 0..1 coordinate system, for compatability
  void SetNormalCoordinates( bool );


  // Interface for compatability with legacy sprite class
  //////////////////

  void  SetPosition( float, float );
  void  SetX( float );
  void  SetY( float );
  float GetX();
  float GetY();

  float GetWidth();
  float GetHeight();
  void  SetWidth(float);
  void  SetHeight(float);

  void  SetScaleX(float);
  void  SetScaleY(float);
  void  SetHotSpot( float, float );

  float GetScaleX();
  float GetScaleY();

  float GetFrameOffsetX();
  float GetFrameOffsetY();
  float GetFrameTotalWidth();
  float GetFrameTotalHeight();

  void  SetAlpha( int );
  int   GetAlpha();

  void  SetAlphaEnable( bool );
  bool  GetAlphaEnable();

  bool  MouseHit( float x, float y, bool bIgnoreScale = false );

  void  SetClip( float x0, float y0, float x1, float y1 );
  void  SetRotation( float r );
  void  SetRotationCenter( float x, float y );
  void  SetTransparent( bool b );

	static const float       GetScreenWidth() { return s_screenWidth; }
	static const float       GetScreenHeight() { return s_screenHeight; }

private:
  lSharedPtr<lSpriteBank>   m_bank;
  const lSpriteBankSprite * m_sprite;
  uint32                    m_flipFlags;
  uint32                    m_colour;

  float                     m_x;
  float                     m_y;
  float                     m_width;
  float                     m_height;
  float                     m_hotspotX;
  float                     m_hotspotY;
  int                       m_alpha;
  bool                      m_bAlpha;
  float                     m_scaleX;
  float                     m_scaleY;
  float                     m_clipX0;
  float                     m_clipY0;
  float                     m_clipX1;
  float                     m_clipY1;
  float                     m_rotation;
  bool                      m_transparent;

  bool                      m_bNormal;

	static const float        s_screenWidth;
	static const float        s_screenHeight;

  float                     m_rotationCenterX;
  float                     m_rotationCenterY;
};

END_L_NAMESPACE
#endif

