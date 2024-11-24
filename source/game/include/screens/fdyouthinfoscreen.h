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

#include <fdbuttonwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdsliderwidget.h>
#include <fdtablewidget.h>
#include <fdbarwidget.h>

#ifndef FDYOUTHINFOSCREEN_H_INCLUDED
#define FDYOUTHINFOSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDYouthInfoScreen - Youth information screen (59)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDYouthInfoScreen : public FDScreen
{
 public:
  FDYouthInfoScreen( FDMainMenuUI *parent );
  virtual ~FDYouthInfoScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDYouthInfoScreen interface
  void SetYouthInfo( CYouth *player );
  void SetValueTextPercentageFromInt(const int eField, const int iValue, const float fMaxValue, const bool bReverseColors);


 private:
  void Init();

  lUIScreenWidget       * m_youthScreenSub;

  //FDTableWidget         * m_youthInfoTable;

  lUISpriteWidget *m_backgroundStripes[7];
  lUISpriteWidget *m_separatorStripes[7];

  lUITextWidget         * m_youthInfoText[11];
  lUITextWidget         * m_youthInfoData[11];
  FDBarWidget        * m_youthInfoComboData[10];

  lUITextWidget         * m_youthInfoName;
  lUITextWidget         * m_youthInfoPos;

  lUISpriteWidget *       m_kit;
  lUISpriteWidget *       m_badgeStripe1;
  lUISpriteWidget *       m_badgeStripe2;

  lUISpriteWidget *m_kitBG;
  lUISpriteWidget *m_teamBadge;

  CYouth                * m_currentYouth;
};
END_L_NAMESPACE
#endif

