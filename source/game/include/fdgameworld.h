//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdgameworld.h
// Description : FDGameWorld interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDGAMEWORLD_H_INCLUDED
#define FDGAMEWORLD_H_INCLUDED

#include <game/lgameworld.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lUIWidget;
class lUIManager;
class FDSubGame;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Sub game types
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum
{
  FD_GAMEMODE_MAIN_MENU = 0,
  FD_GAMEMODE_GAME,
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDGameWorld
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDGameWorld : public lRTTI< FDGameWorld, lGameWorld, lCLSID<0x1F6440CB,0x61E7,0x4C0A,0x8C2C1EB93DBC11D1> >
{
public:
  static void ClassInitialiser();

  FDGameWorld();
  ~FDGameWorld();

  // lGameWorld interface

  virtual void Init();
  virtual void Deinit();

  virtual void BeginPlay();
  virtual void EndPlay();
  virtual void Tick(float);
  virtual void RenderClientView( lGameClientView *, unsigned int, lrCamera * );

  // FDGameWorld interface

  int          AddSubGame( FDSubGame * );
  int          GetNumSubGames();
  FDSubGame  * GetSubGame( int );
  void         SetCurrentSubGame( int index );
  FDSubGame  * GetCurrentSubGame();

  lUIWidget *  GetScreen();
  lUIManager * GetUIManager();
  
  static void Debounce();

private:
  lUIManager          * m_uiManager;
  lArray<FDSubGame *>   m_subGameList;
  int                   m_subGameIndex;
  bool                  m_bPlaying;
  float                 m_time;
  float                 m_tick;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDSubGame
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDSubGame
{
public:
  FDSubGame();
  virtual ~FDSubGame();

  virtual void          Init() = 0;
  virtual void          Deinit() = 0;
  virtual void          Activate() = 0;
  virtual void          Deactivate() = 0;
  virtual void          Tick(float) = 0;

  FDGameWorld * GetWorld();

private:
  FDGameWorld * m_world;
  friend class FDGameWorld;
};

END_L_NAMESPACE

#endif

