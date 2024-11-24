//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdstatusscreen.h
// Description : FDStatusScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>
#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>
#include <fdbarwidget.h>


#ifndef FDSTATUSSCREEN_H_INCLUDED
#define FDSTATUSSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDStatusScreen - Status screen (03)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDStatusScreen : public FDScreen
{
 public:
  FDStatusScreen( FDMainMenuUI *parent );
  virtual ~FDStatusScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDStatusScreen interface
  void Update();

 private:

  enum StatusFieldType
  {
    SFT_LEAGUE,
    SFT_CLUBBALANCE,
    SFT_FANCONFIDENCE,
    SFT_BOARDCONFIDENCE,
    SFT_MAX,
  };

  lUISpriteWidget *m_backgroundStripes[SFT_MAX];
  FDBarWidget *m_fieldStripes[SFT_MAX];
  lUITextWidget *m_fieldLabels[SFT_MAX];
  lUITextWidget *m_fieldValues[SFT_MAX];

  void Init();
  void SetValueTextPercentageFromInt(const StatusFieldType eField, const int iValue, const float fMaxValue, const bool bReverseColors);

  lUIScreenWidget       * m_statusScreen;

  lUITextWidget         * m_teamName;
  lUITextWidget         * m_managerName;

  lUITextWidget         * m_nextMatch1;
  lUITextWidget         * m_nextMatch2;

  lUISpriteWidget       * m_teamKit;
  lUISpriteWidget       * m_nextteamKit;

  lUISpriteWidget       * m_badgeStripe1;
  lUISpriteWidget       * m_badgeStripe2;
};

END_L_NAMESPACE
#endif

