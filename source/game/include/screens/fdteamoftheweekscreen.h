//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdteamoftheweekscreen.h
// Description : FDTeamOfTheWeekscreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbuttonwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdsliderwidget.h>
#include <fdtablewidget.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

#ifndef FDTEAMOFTHEWEEKSCREEN_H_INCLUDED
#define FDTEAMOFTHEWEEKSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDTeamOfTheWeekScreen - TeamOfTheWeek screen (36)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDTeamOfTheWeekScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDTeamOfTheWeekScreen( FDMainMenuUI *parent );
  virtual ~FDTeamOfTheWeekScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDTeamOfTheWeekScreen interface
  void Update();
  virtual void AutoplaySkip();

 private:

  void Init();
  void OnSelectTeamSelection( FDComboWidget *, int index );
  void OnDone( lUIControlWidget * );
  void OnRowSelect( FDTableWidget *, int row );

  lUIScreenWidget       * m_towScreen;
  lUIScreenWidget       * m_towScreenSub;

  FDComboWidget         * m_selectteamDivision;
  FDTableWidget         * m_table;
};

END_L_NAMESPACE
#endif

