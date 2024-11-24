//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdemployeeinfoscreen.h
// Description : FDEmployeeInfoScreen interface
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
#include <fdbuttonrolloutwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdsliderwidget.h>
#include <fdtickerwidget.h>
#include <fdtablewidget.h>
#include <fdplayerblobwidget.h>
#include <fdfullscreenwidget.h>


#ifndef FDEMPLOYEEINFOSCREEN_H_INCLUDED
#define FDEMPLOYEEINFOSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDEmployeeInfoScreen - Player information screen (61)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDEmployeeInfoScreen : public FDScreen
{
 public:
  FDEmployeeInfoScreen( FDMainMenuUI *parent );
  virtual ~FDEmployeeInfoScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDEmployeeInfoScreen interface
  void SetEmployeeInfo( CEmployee * );

 private:
  void Init();

  lUIScreenWidget       * m_employeeScreenSub;

  lUITextWidget         * m_employeeInfoNamePos;

  lUISpriteWidget       * m_employeeStripes[5];
  FDBarWidget     *       m_employeeBars[5];
  lUITextWidget         * m_employeeInfoLabel[5];
  lUITextWidget         * m_employeeInfoData[5];

  CEmployee             * m_currentEmployee;
  lUITextWidget         * m_employeeInfoRolePos;

  lUISpriteWidget *m_kitBG;
  lUISpriteWidget *m_teamBadge;

  lUISpriteWidget *     m_kit;
  lUISpriteWidget *     m_badgeStripe1;
  lUISpriteWidget *     m_badgeStripe2;
};





END_L_NAMESPACE
#endif

