//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsquadscreen.h
// Description : FDSquadScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdcombowidget.h>
#include <fdsliderwidget.h>
#include <fdtickerwidget.h>
#include <fdtablewidget.h>
#include <fdfullscreenwidget.h>

#ifndef FDSACKPLAYERSCREEN_H_INCLUDED
#define FDSACKPLAYERSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDSackPlayerScreen - Negotiation screen
// (35)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDSackPlayerScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDSackPlayerScreen( FDMainMenuUI *parent );
  virtual ~FDSackPlayerScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDSackPlayerScreen interface
  void Update();
  void SetPlayer( CPlayer *player );

 private:

  void Init();
  void OnConfirm( lUIControlWidget * );
  void OnCancel( lUIControlWidget * );

  lUIScreenWidget       * m_sackPlayerScreen;
  lUIScreenWidget       * m_sackPlayerScreenSub;

  lUISpriteWidget         * m_messageBar;
  lUITextWidget         * m_text;

  FDButtonWidget * m_confirmButton;
  FDButtonWidget * m_cancelButton;

  FDFullScreenWidget * m_fadeSub;

  //
  CPlayer               * m_player;
};

END_L_NAMESPACE
#endif

