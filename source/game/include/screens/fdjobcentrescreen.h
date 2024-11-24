//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsquadscreen.h
// Description : FDSquadScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>
#include "fdemployeeinfoscreen.h"
#include "fdnegotiatescreen.h"

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdsliderwidget.h>
#include <fdtablewidget.h>

#ifndef FDJOBCENTRESCREEN_H_INCLUDED
#define FDJOBCENTRESCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDJobCentreScreen - JobCentre screen
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDJobCentreScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDJobCentreScreen( FDMainMenuUI *parent );
  virtual ~FDJobCentreScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void SerializeState( FDStateStorage &state );
  virtual void Prepare();

  // FDJobCentreScreen interface
  void Update();

 private:
  void PopulateClubTable();
  void PopulateAvailableTable();

  void UpdateTableRow(const int iRow, CEmployee *pEmployee);

  void OnRowSelect(FDTableWidget *pTable, int row);
  void OnClubRowSelect( FDTableWidget *, int row );
  void OnAvailableRowSelect( FDTableWidget *, int row );

  void OnHireButton( lUIControlWidget * );
  void OnSackButton( lUIControlWidget * );
  void OnOfferContractButton( lUIControlWidget * );

  void OnTableSelect(FDComboWidget *, int);

  void Init();

  enum ViewMode
  {
	  JC_VIEW_CLUB,
	  JC_VIEW_AVAILABLE
  };

  lUIScreenWidget       * m_jobCentreScreen;

  FDTableWidget         * m_table;

  FDComboWidget         * m_tableSelect;

  FDButtonWidget        * m_hireButton;
  FDButtonWidget        * m_sackButton;
  FDButtonWidget        * m_offerContractButton;

  lArray<CEmployee *>     m_clubEmployees;
  lArray<CEmployee *>     m_availableEmployees;

  int                     m_availableEmployeeSelected;
  int                     m_clubEmployeeSelected;

  ViewMode				m_currentView;
  bool					m_bComingFromDialogBox;
};



END_L_NAMESPACE
#endif

