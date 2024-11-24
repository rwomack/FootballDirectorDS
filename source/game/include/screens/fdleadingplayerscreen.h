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

#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdcombowidget.h>
#include <fdsliderwidget.h>
#include <fdtickerwidget.h>
#include <fdtablewidget.h>

#include <LeadingGoalscorer.h>          // must be included first as it contains CPlayerClub
#include <LeadingDisciplinaryPoints.h>

#ifndef FDLEADINGPLAYERSSCREEN_H_INCLUDED
#define FDLEADINGPLAYERSSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDLeadingPlayersScreen - Leading Players screen (33)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDLeadingPlayersScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDLeadingPlayersScreen( FDMainMenuUI *parent );
  virtual ~FDLeadingPlayersScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void SerializeState( FDStateStorage &state );
  virtual void Prepare();

  // FDLeadingPlayersScreen interface
  void Update();

  bool HasDataToDisplay() const;

 private:

  void UpdateTable();

  void OnTableSelect( FDTableWidget *, int row );
  void OnDivisionSelect( FDComboWidget *, int index );
  void OnViewSelect( FDComboWidget *, int index );
  void Init();

  lUIScreenWidget       * m_leadingScreen;
  //View modes
  //NOTE: Set up to match Disciplinary, Goals
  enum LeadingPlayerViewType
  {
    VIEW_DISCIPLINARY = 0,
    VIEW_GOALS,
  };

  LeadingPlayerViewType   m_viewMode;
  FDTableWidget         * m_table;

  lUITextWidget         * m_matchText1;

  FDComboWidget         * m_divisionSelector;
  FDComboWidget         * m_viewSelector;

  lArray<CPlayer *>       m_playerList;

  CLeadingDisciplinaryList m_pointScorers;
  CLeadingGoalScorerList   m_goalScorers;

};


END_L_NAMESPACE
#endif

