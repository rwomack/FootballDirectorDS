//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDClubInfoscreen.h
// Description : FDClubInfoSubScreen interface
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
#include <fdbarwidget.h>
#include <fdbackingwidget.h>

#ifndef FDCLUBINFOSUBSCREEN_H_INCLUDED
#define FDCLUBINFOSUBSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDClubInfoSubScreen - Player information screen (61)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDClubInfoSubScreen : public FDScreen
{
 public:
  FDClubInfoSubScreen( FDMainMenuUI *parent );
  virtual ~FDClubInfoSubScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDClubInfoSubScreen interface
  void SetClubInfo( CClub * club );
  void SetBacking( bool show );


 private:
  void Init();

  lUIScreenWidget       * m_clubScreenSub;

  FDTableWidget * m_playerTable;

  lUISpriteWidget * m_teamKit;
  lUISpriteWidget * m_badgeStripe1;
  lUISpriteWidget * m_badgeStripe2;

  lUITextWidget * m_teamName;
  lUITextWidget * m_managerName;

  FDBackingWidget * m_screenBacking;

};





END_L_NAMESPACE
#endif

