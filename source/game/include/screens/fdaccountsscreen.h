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
#include <fdbackingwidget.h>

#ifndef FDACCOUNTSSCREEN_H_INCLUDED
#define FDACCOUNTSSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDAccountsScreen - Accounts screen
// (22,26)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDAccountsScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDAccountsScreen( FDMainMenuUI *parent );
  virtual ~FDAccountsScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void SerializeState( FDStateStorage &state );

  // FDAccountsScreen interface
  void Update();
  void OnApplyLoan( lUIControlWidget * );
  void OnRepayLoan( lUIControlWidget * );

 private:
  enum AccountInfoFieldType
  {
	  AIFT_MERCHANDISE,
	  AIFT_SPONSOR,
	  AIFT_BROADCAST_RIGHTS,
	  AIFT_GENERAL_WAGES,
	  AIFT_MANAGER_WAGES,
	  AIFT_PLAYER_WAGES,
	  AIFT_EMPLOYEE_WAGES,
	  AIFT_YOUTH_WAGES,
	  AIFT_RUNNING_COSTS,
	  AIFT_TOTAL_INCOME,
	  AIFT_TOTAL_EXPENDITURE,
	  AIFT_CLUB_BALANCE,
	  AIFT_BORROW_AMOUNT,
    AIFT_LOAN_OUTSTANDING,
    AIFT_WEEKLY_REPAYMENTS,
    AIFT_WEEKS_REMAINING,
	  AIFT_MAX
  };

  void Init();
  void OnPeriodCombo( FDComboWidget *, int idx );

  lUIScreenWidget       * m_accountsScreen;
  lUIScreenWidget       * m_accountsScreenSub;

  bool m_bAnnual;

  lUISpriteWidget *m_backgroundStripes[AIFT_MAX];
  lUISpriteWidget *m_fieldStripes[AIFT_MAX];
  lUITextWidget *m_fieldLabels[AIFT_MAX];
  lUITextWidget *m_fieldValues[AIFT_MAX];

  lUISpriteWidget *m_loanStripe;
  lUITextWidget *m_loanText;

  FDSliderWidget        * m_bankSlider;

  FDButtonWidget * m_bankApplyButton;
  FDButtonWidget * m_bankRepayButton;

  FDComboWidget         * m_periodSelect;

  int             m_lastLoanedOut;
};

END_L_NAMESPACE
#endif

