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
#include <fdbuttonrolloutwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdsliderwidget.h>
#include <fdtablewidget.h>
#include <fdplayerblobwidget.h>

#ifndef FDSQUADSCREEN_H_INCLUDED
#define FDSQUADSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

class FDSquadTableWidget;

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDSquadScreen - Squad screen
// includes formation + roles (27)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDSquadScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDSquadScreen( FDMainMenuUI *parent );
  virtual ~FDSquadScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void AutoplaySkip();
  virtual void SerializeState( FDStateStorage &state );

  // FDSquadScreen interface
  CClub * GetSquadInfo() { return m_pCurrentClub; }
  void SetSquadInfo( CClub * club );
  void Update();

  int GetInfoToShow();

 private:

  void Init();

  // table
  void OnSquadRowSelect( FDTableWidget *, CPlayer *pPlayer, int row );

  // buttons
  void OnSquadFormationButton( lUIControlWidget * );
  void OnSquadRolesButton( lUIControlWidget * );
  void OnTransferStatusButton( lUIControlWidget * );
  void OnSackPlayerButton( lUIControlWidget * );
  void OnRenewContractButton( lUIControlWidget * );
  void OnCoachButton( lUIControlWidget * );
  void OnSwapButton( lUIControlWidget * );

  // combo
  void OnTableInfoCombo( FDComboWidget *, int );

  void SetSwapButtonState(const bool bActivated);

  // Squad screen

  lUIScreenWidget       * m_squadScreen;

  FDSquadTableWidget    * m_squadTable;

  FDButtonWidget        * m_squadButtonRoles;
  FDButtonWidget        * m_squadButtonFormation;

  FDButtonWidget        * m_sackPlayerButton;
  FDButtonWidget        * m_renewContractButton;
  FDButtonWidget        * m_transferStatusButton;
  FDButtonWidget        * m_coachButton;
  FDButtonWidget        * m_swapButton;

  lUITextWidget         * m_otherClubName;

  CClub                 * m_pCurrentClub;
  bool                    m_bUserClub;

  int                     m_playerSelectedIdx;
  int                     m_playerSwapIdx;
  bool                    m_bSwapMode;

  bool                    m_bClearPlayerInfo;
};

END_L_NAMESPACE
#endif

