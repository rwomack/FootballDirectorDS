//����������������������������������������������������������������������������
// File        : fdsquadscreen.h
// Description : FDSquadScreen interface
// Notes       :
//
//����������������������������������������������������������������������������
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

#ifndef FDPLAYERINFOSCREEN_H_INCLUDED
#define FDPLAYERINFOSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//����������������������������������������������������������������������������
// FDPlayerInfoScreen - Player information screen (61)
//
//����������������������������������������������������������������������������
class FDPlayerInfoScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDPlayerInfoScreen( FDMainMenuUI *parent );
  virtual ~FDPlayerInfoScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void SerializeState( FDStateStorage &state );

  // FDPlayerInfoScreen interface
  void SetPlayerInfo( CPlayer *player );

 private:
  void Init();
  void OnTypeSelect(  FDComboWidget *, int index );

  void OnButton( lUIControlWidget * );

  lUIScreenWidget       * m_playerScreen;

  FDTableWidget         * m_playerInfoTable;
  FDComboWidget         * m_playerInfoType;

  CPlayer               * m_player;

  FDButtonWidget * m_transferStatusButton;
  FDButtonWidget * m_sackPlayerButton;
  FDButtonWidget * m_renewContractButton;

  FDButtonWidget * m_loanButton;
  FDButtonWidget * m_buyButton;

  lUISpriteWidget * m_messageBar;
  lUITextWidget * m_injuryText;
};





END_L_NAMESPACE
#endif

