//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdmonthlyawardscreen.h
// Description : FDMonthlyAwardScreen interface
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
#include <fdbackingwidget.h>

#ifndef FDMONTHLYAWARDSCREEN_H_INCLUDED
#define FDMONTHLYAWARDSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDMonthlyAwardScreen - monthlyawardscreen screen
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDMonthlyAwardScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDMonthlyAwardScreen( FDMainMenuUI *parent );
  virtual ~FDMonthlyAwardScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void SerializeState( FDStateStorage &state );

  // FDMonthlyAwardScreen interface
  void Update();

  bool HasDataToDisplay() const;

 private:

  void Init();

  void OnMonthSelection( FDComboWidget *, int index );
  void OnDivisionSelection( FDComboWidget *, int index );

  // Main screen / underlying tabs
  lUIScreenWidget       * m_monthlyawardscreenScreen;

  FDComboWidget *     m_monthSelection;
  FDComboWidget *     m_divisionSelection;

  lUITextWidget *         m_managerLabels[2];
  lUISpriteWidget *        m_managerStripes[3];
  FDBackingWidget *        m_managerBacking[3];

  lUITextWidget *         m_managerText;

  // Top screen
  lUIScreenWidget       * m_monthlyawardscreenScreenSub;

  bool m_bInGame;
};

END_L_NAMESPACE
#endif
