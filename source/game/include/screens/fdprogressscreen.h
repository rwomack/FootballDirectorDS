//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdProgressscreen.h
// Description : FDProgressScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbuttonwidget.h>

#ifndef FDPROGRESSSCREEN_H_INCLUDED
#define FDPROGRESSSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE


//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDProgressScreen -
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDProgressScreen : public FDScreen, public CScreen28View
{
 public:
  FDProgressScreen( FDMainMenuUI *parent );
  virtual ~FDProgressScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDProgressScreen interface
  virtual void OnIncrementProgress( float );

  lUISpriteWidget       * m_progressSprite;

 private:

  void Init();

  // New game creation screen

  lUIScreenWidget       * m_progressScreen;
  lUIScreenWidget       * m_progressScreenSub;
  lUISpriteWidget       * m_progressText;
};


END_L_NAMESPACE
#endif

