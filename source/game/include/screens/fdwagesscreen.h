//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdwagesscreen.h
// Description : FDWagesScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbuttonwidget.h>
#include <fdbuttonrolloutwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdsliderwidget.h>
#include <fdtablewidget.h>

#ifndef FDWAGESSCREEN_H_INCLUDED
#define FDWAGESSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDWagesScreen - Squad screen
// includes formation + roles (27)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDWagesScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDWagesScreen( FDMainMenuUI *parent );
  virtual ~FDWagesScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void SerializeState( FDStateStorage &state );
  virtual void Prepare();

  // FDWagesScreen interface
  void Update();

 private:

  void Init();

  void AddPlayerToTable(CPlayer *pPlayer);
  void UpdateColumnText();
  void UpdateTableEntryForManager(const int iPersonListIndex);
  void UpdateTableEntryForEmployee(const int iPersonListIndex);
  void UpdateTableEntryForPlayer(const int iPersonListIndex);
  void UpdateTableEntryForYouth(const int iPersonListIndex);

  void OnRowSelect( FDTableWidget *, int row );

  void OnSackButton( lUIControlWidget * );
  void OnContractButton( lUIControlWidget * );
  void OnTransferStatusButton( lUIControlWidget * );

  void OnTableInfoCombo( FDComboWidget *, int );

  // Squad screen

  lUIScreenWidget       * m_wagesScreen;

  FDComboWidget *m_wagesTableInfoType;

  FDTableWidget         * m_wagesTable;

  FDButtonWidget        * m_wagesSackButton;
  FDButtonWidget        * m_wagesContractButton;
  FDButtonWidget        * m_wagesTransferStatusButton;

  enum WageEarnerType
  {
    WAGE_MANAGER,
    WAGE_EMPLOYEE,
    WAGE_YOUTH,
    WAGE_PLAYER,
	WAGE_MAX
  };

  enum WageDisplayType
  {
	  WAGE_DISPLAY_WAGE,
	  WAGE_DISPLAY_CONTRACT,
  };

  struct FDWageType
  {
    FDWageType()
    {
      type = WAGE_MAX;
      person = NULL;
    }
    CPerson * person;
    WageEarnerType type;
  };

  lArray<FDWageType>       m_personList;

  WageDisplayType         m_WhichEvents;
  int                     m_dTotalYouthWages;
  int                     m_dTotalPlayerWages;
  int                     m_dTotalEmpWages;
  bool m_bComingFromDialogBox;
};

END_L_NAMESPACE
#endif

