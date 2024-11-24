//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdloadscreen.h
// Description : FDLoadScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbuttonwidget.h>

#ifndef FDLoadSCREEN_H_INCLUDED
#define FDLoadSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

enum
{
  PROCESS_LOAD,
  PROCESS_SAVE,
};

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDLoadScreen -
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDLoadScreen : public FDScreen, public FDLoadSaveCallback, public lHasSlots<>
{
 public:
  FDLoadScreen( FDMainMenuUI *parent );
  virtual ~FDLoadScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDLoadScreen interface
  void SetType( int processType );
  void LoadProgress( float pct );

 private:

  void Init();

  // New game creation screen

  lUIScreenWidget       * m_loadScreen;

  lUISpriteWidget       * m_progressText;
  lUISpriteWidget       * m_progressSprite;

  int                     m_processType;
  bool                    m_bActive;
};


END_L_NAMESPACE
#endif

