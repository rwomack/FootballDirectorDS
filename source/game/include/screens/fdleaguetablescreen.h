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
#include <fdgraphwidget.h>



#ifndef FDLEAGUETABLESCREEN_H_INCLUDED
#define FDLEAGUETABLESCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDLeagueTableScreen - League Fixtures screen (19)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDLeagueTableScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDLeagueTableScreen( FDMainMenuUI *parent );
  virtual ~FDLeagueTableScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void SerializeState( FDStateStorage &state );
  virtual void Prepare();

  // FDLeagueTableScreen interface
  void Update();

 private:

  void UpdateTable();
  void UpdateGraph();
  void UpdateClubInfo();

  void OnTableSelect( FDTableWidget *, int row );
  void OnDivisionSelect( FDComboWidget *, int index );
  void OnViewSelect( FDComboWidget *, int index );
  void OnGraphButton( lUIControlWidget * );
  void Init();

  void ClearRowSelectHistory();
  void AddClubToGraph(const int iRow);
  void AddClubToGraph(const ClubID cClubID);

  lUIScreenWidget       * m_leaguetableScreen;
  lUIScreenWidget       * m_leaguetableScreenSub;

  //View modes
  //NOTE: Set up to match Overall, Home, Away, Form, Discipline from original code (0-index based)
  enum LeagueTableViewType
  {
    VIEW_OVERALL = 0,
    VIEW_HOME,
    VIEW_AWAY,
    VIEW_FORM,
    VIEW_DISCIPLINE,
  };

  LeagueTableViewType     m_viewMode;

  // Bottom screen widgets
  FDTableWidget         * m_table;
  FDComboWidget         * m_divisionSelector;
  FDComboWidget         * m_viewSelector;

  FDGraphWidget *       m_graph;
  FDButtonWidget *      m_graphButton;

  ClubID m_iRowSelectHistory[4];
  luint32_t m_iRowColorHistory[4];
  int m_iLastRowSelectIndex;
};




END_L_NAMESPACE
#endif

