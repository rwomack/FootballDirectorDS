//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdliveresultscreen.h
// Description : FDLiveResultScreen interface
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
#include <fdcheckboxwidget.h>
#include <fdsliderwidget.h>
#include <fdtickerwidget.h>
#include <fdtablewidget.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

#ifndef FDLIVERESULTSCREEN_H_INCLUDED
#define FDLIVERESULTSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDTickerScreen
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍

class FDLiveResultScreen : public FDScreen
{
 public:
  FDLiveResultScreen( FDMainMenuUI *parent );
  virtual ~FDLiveResultScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDMatchDayScreen interface
  void Update( CMatchPlay* _MatchPlay );

 private:

  void Init();

  lUIScreenWidget             * m_screen;
  FDTableWidget               * m_table;
};

END_L_NAMESPACE
#endif
