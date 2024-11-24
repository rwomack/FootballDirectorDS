//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdarrangefriendliesscreen.h
// Description : FDArrangeFriendlies interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDARRANGEFRIENDLIESSCREEN_H_INCLUDED
#define FDARRANGEFRIENDLIESSCREEN_H_INCLUDED

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

#include <SoccerDefines.h>

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDArrangeFriendlies - Arrange Friendlies
// (54)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDArrangeFriendlies : public FDScreen, public lHasSlots<>
{
 public:
  FDArrangeFriendlies( FDMainMenuUI *parent );
  virtual ~FDArrangeFriendlies();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void AutoplaySkip();

  // FDArrangeFriendlies interface
  void Update();

 private:

  void Init();
  void ResetScreenData();

  void OnRowSelect( FDTableWidget *, int row );
  void OnHomeButton( lUIControlWidget * );
  void OnAwayButton( lUIControlWidget * );
  void OnContinueButton( lUIControlWidget * );
  void OnConfirmButton( lUIControlWidget * );
  void OnClearButton( lUIControlWidget * );
  void OnTableInfoCombo( FDComboWidget *, int );

  void SetDate( bool bHome );

  // Sub screen
  lUIScreenWidget       * m_arrangeFriendliesScreenSub;

  //lUITextWidget         * m_titleText;
  FDTableWidget         * m_summaryTable;

  lArray<lUITextWidget*> m_summaryText;
  lArray< lUISpriteWidget* > m_backBox;
  lArray< lUISpriteWidget* > m_clubKit;
  lArray< lUISpriteWidget* > m_gradients;

  // Main screen
  lUIScreenWidget       * m_arrangeFriendliesScreen;

  FDComboWidget         * m_datesInfo;
  FDComboWidget         * m_divisions;

  FDTableWidget         * m_clubsTable;

  FDButtonWidget        * m_homeButton;
  FDButtonWidget        * m_awayButton;
  FDButtonWidget        * m_continueButton;
  FDButtonWidget        * m_confirmButton;
  FDButtonWidget        * m_clearButton;

  struct Friendly
  {
    ClubID m_club;
    bool m_set;
    bool m_home;
  };

  bool                        m_bHome;
  Friendly                  m_friendly[4];
  lArray<int>             m_clubIdxList;
};

END_L_NAMESPACE
#endif

