//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsquadscreen.h
// Description : FDSquadScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDSELECTTEAMSCREEN_H_INCLUDED
#define FDSELECTTEAMSCREEN_H_INCLUDED

#include <fdscreen.h>
#include <SoccerDefines.h>
#include <ui/luitextwidget.h>

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDSelectTeamScreen - SelectTeam screen
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDSelectTeamScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDSelectTeamScreen( FDMainMenuUI *parent );
  virtual ~FDSelectTeamScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDSelectTeamScreen interface
  void SetNonSelectableTeam(ClubID cClub);
  void SetInfoText(lUIText &text) { m_text = text; }

 private:
  void Init();

  void OnSelectTeamSelection( FDComboWidget *, int index );
  void OnSelectTeamSelectTeam( FDTableWidget *, int row );
  void OnSelectTeamDone( lUIControlWidget * );
  void OnReturnToMainMenuButton(lUIControlWidget *);
  void UpdateClubInfo();

  static bool IsOKToReturnModal(FDScreen *);

  lUIScreenWidget       * m_selectteamScreen;
  lUIScreenWidget       * m_selectteamSubScreen;

  FDTableWidget         * m_selectteamTable;
  FDButtonWidget        * m_selectteamDone;
  FDComboWidget         * m_selectteamDivision;
  FDButtonWidget		* m_returnToMainMenuButton;

  lUIText                 m_text;
  lUITextWidget         * m_textWidget;

  ClubID				  m_idNonSelectable;
  bool					  m_bOKToExit;
};


END_L_NAMESPACE
#endif

