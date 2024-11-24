//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdmanagerinfoscreen.h
// Description : FDManagerInfoScreen interface
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

#ifndef FDManagerInfoScreen_H_INCLUDED
#define FDManagerInfoScreen_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
//  - Manager information screen
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDManagerInfoScreen : public FDScreen
{
 public:
  FDManagerInfoScreen( FDMainMenuUI *parent );
  virtual ~FDManagerInfoScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDManagerInfoScreen interface
  void SetManagerInfo( CManager *player );

 private:
  void Init();

  lUIScreenWidget       * m_managerScreenSub;

  lUISpriteWidget       * m_managerStripes[3];
  FDBarWidget     *       m_managerBars[3];
  lUITextWidget         * m_managerInfoLabel[3];
  lUITextWidget         * m_managerInfoData[3];

  lUITextWidget         * m_managerInfoName;
  lUITextWidget         * m_managerInfoPos;

  lUISpriteWidget * m_kit;
  lUISpriteWidget * m_badgeStripe1;
  lUISpriteWidget * m_badgeStripe2;

  CManager                * m_currentManager;
};
END_L_NAMESPACE
#endif



