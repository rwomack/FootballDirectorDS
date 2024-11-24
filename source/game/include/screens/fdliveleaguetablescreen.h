//����������������������������������������������������������������������������
// File        : fdliveleaguetablescreen.h
// Description : FDLiveLeagueTableScreen interface
// Notes       :
//
//����������������������������������������������������������������������������
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

#ifndef FDLIVELEAGUETABLESCREEN_H_INCLUDED
#define FDLIVELEAGUETABLESCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//����������������������������������������������������������������������������
// FDLiveLeagueResultScreen
//
//����������������������������������������������������������������������������

class FDLiveLeagueTableScreen : public FDScreen
{
 public:
  FDLiveLeagueTableScreen( FDMainMenuUI *parent );
  virtual ~FDLiveLeagueTableScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDLiveLeagueResultScreen interface
  void Update( CMatchPlay* _MatchPlay );

 private:

  void Init();
  static void TableArrowRenderCallback(FDTableWidget *pTable, void *pScreen, luint32_t uiRow, luint32_t uiCol, lUIRect cCellRect);

  lUIScreenWidget             * m_screen;

  FDTableWidget               * m_table;
  lSharedPtr<lSprite>			m_upArrow;
  lSharedPtr<lSprite>     m_downArrow;
};

END_L_NAMESPACE
#endif
