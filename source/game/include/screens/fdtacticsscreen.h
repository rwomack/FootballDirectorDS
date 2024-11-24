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
#include <fdplayerblobwidget.h>
#include <fdbackingwidget.h>

#ifndef FDTACTICSSCREEN_H_INCLUDED
#define FDTACTICSSCREEN_H_INCLUDED

class CMatchPlay;

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDTacticsScreen - Tactics screen (27)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDTacticsScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDTacticsScreen( FDMainMenuUI *parent );
  virtual ~FDTacticsScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDTacticsScreen interface
  void Update();
  void SetMatchDetails( CMatchPlay * matchPlay );

 private:

  void Init();

  void OnCounterAttackBreak( lUIControlWidget * );
  void OnOffsideTrap( lUIControlWidget * );

  void OnStyleSelectionChange( FDComboWidget *, int );
  void OnMarkingSelectionChange( FDComboWidget *, int );
  void OnAggressionChange( lUIWidget *, float );
  void OnFormationSelect( FDComboWidget *, int idx );

  lUIScreenWidget       * m_tacticsScreen;

  lUIScreenWidget       * m_tacticsScreenSub;
  lUISpriteWidget       * m_tacticsScreenSubBg;

  lUISpriteWidget       * m_tacticsStripes[4];

  FDComboWidget         * m_tacticsStyleOfPlay;
  FDComboWidget         * m_tacticsMarking;
  FDSliderWidget        * m_tacticsAggression;
  FDCheckBoxWidget      * m_counterattackCheckBox;
  FDCheckBoxWidget      * m_offsidetrapCheckBox;

  FDComboWidget         * m_squadFormationCombo;

  lArray<FDPlayerBlobWidget *> m_playerBlobList;

  CMatchPlay *                m_matchPlay;

};

END_L_NAMESPACE
#endif

