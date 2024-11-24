//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsquadscreen.cpp
// Description : FDSquadScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdaccountsscreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDAccountsScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDAccountsScreen::FDAccountsScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
  m_bAnnual = true;
}

FDAccountsScreen::~FDAccountsScreen()
{
}

void FDAccountsScreen::Update()
{
	CClub* pClub = WorldData().GetCurrentUserClub();
	CAccountsItemList *pAccounts = pClub->GetAccounts();

  eFINANCIALPERIOD period = m_bAnnual ? ANNUAL_FINANCE : WEEKLY_FINANCE;

  const uint uiMerchandiseIncome = pAccounts->GetAt(CAccountsItemList::MERCHANDISEINCOME).GetAmount(period);
  const uint uiSponsorshipIncome = pAccounts->GetAt(CAccountsItemList::SPONSORSHIPINCOME).GetAmount(period);
  const uint uiBroadcastIncome = pAccounts->GetAt(CAccountsItemList::TELEVISIONINCOME).GetAmount(period);
  const uint uiGeneralWages = pAccounts->GetAt(CAccountsItemList::GENERALWAGE).GetAmount(period);
  const uint uiManagerWages = pAccounts->GetAt(CAccountsItemList::MANAGERSWAGES).GetAmount(period);
  const uint uiPlayerWages = pAccounts->GetAt(CAccountsItemList::PLAYERWAGES).GetAmount(period);
  const uint uiEmployeeWages = pAccounts->GetAt(CAccountsItemList::EMPLOYEEWAGES).GetAmount(period);
  const uint uiYouthWages = pAccounts->GetAt(CAccountsItemList::YOUTHWAGES).GetAmount(period);
  const uint uiRunningCosts = pAccounts->GetAt(CAccountsItemList::RUNNINGCOSTS).GetAmount(period);

	m_fieldValues[AIFT_MERCHANDISE]->SetText(lUIText(pClub->GetForeignStr(uiMerchandiseIncome)));
	m_fieldValues[AIFT_SPONSOR]->SetText(lUIText(pClub->GetForeignStr(uiSponsorshipIncome)));
	m_fieldValues[AIFT_BROADCAST_RIGHTS]->SetText(lUIText(pClub->GetForeignStr(uiBroadcastIncome)));
	m_fieldValues[AIFT_GENERAL_WAGES]->SetText(lUIText(pClub->GetForeignStr(uiGeneralWages)));
	m_fieldValues[AIFT_MANAGER_WAGES]->SetText(lUIText(pClub->GetForeignStr(uiManagerWages)));
	m_fieldValues[AIFT_PLAYER_WAGES]->SetText(lUIText(pClub->GetForeignStr(uiPlayerWages)));
	m_fieldValues[AIFT_EMPLOYEE_WAGES]->SetText(lUIText(pClub->GetForeignStr(uiEmployeeWages)));
	m_fieldValues[AIFT_YOUTH_WAGES]->SetText(lUIText(pClub->GetForeignStr(uiYouthWages)));
	m_fieldValues[AIFT_RUNNING_COSTS]->SetText(lUIText(pClub->GetForeignStr(uiRunningCosts)));

	const uint uiTotalIncome = uiMerchandiseIncome + uiSponsorshipIncome + uiBroadcastIncome;
	const uint uiTotalExpenditure = uiGeneralWages + uiManagerWages + uiPlayerWages + uiEmployeeWages + uiYouthWages + uiRunningCosts;

	m_fieldValues[AIFT_TOTAL_INCOME]->SetText(lUIText(pClub->GetForeignStr(uiTotalIncome)));
	m_fieldValues[AIFT_TOTAL_EXPENDITURE]->SetText(lUIText(pClub->GetForeignStr(uiTotalExpenditure)));
	m_fieldValues[AIFT_CLUB_BALANCE]->SetText(lUIText(pClub->GetCashStr()));

	// Update loan information
	if( pClub->GetFinancialLoan()->GetLoanOwed() )
	{
		// we have a loan set repayment bar
		m_bankSlider->SetValueRange( 0, pClub->GetFinancialLoan()->GetLoanOwed() );
		m_bankSlider->SetValue( pClub->GetRecommendedLoanRepayment() );
		m_bankRepayButton->Show();

    m_fieldValues[AIFT_LOAN_OUTSTANDING]->SetText(lUIText(pClub->GetFinancialLoan()->LoanOwedStr(pClub->GetCountry())));
    m_fieldValues[AIFT_WEEKLY_REPAYMENTS]->SetText(lUIText(pClub->GetFinancialLoan()->LoanRepaymentStr(pClub->GetCountry())));
    char str[32];
    sprintf( str, "%d", pClub->GetFinancialLoan()->WeeksToRepayLoan(pClub->GetCountry()));
    m_fieldValues[AIFT_WEEKS_REMAINING]->SetText(lUIText(str));

    m_fieldLabels[AIFT_BORROW_AMOUNT]->SetText(lUIText(IDS_REPAYAMOUNT));
    m_fieldLabels[AIFT_BORROW_AMOUNT]->SetTextToUpper();

    m_fieldLabels[AIFT_LOAN_OUTSTANDING]->SetTextColour( L_ARGB( 255, 255, 255, 255 ) );
    m_fieldLabels[AIFT_WEEKLY_REPAYMENTS]->SetTextColour( L_ARGB( 255, 255, 255, 255 ) );
    m_fieldLabels[AIFT_WEEKS_REMAINING]->SetTextColour( L_ARGB( 255, 255, 255, 255 ) );

		m_bankApplyButton->Hide();
	}
	else
	{
		int recloanborrow = pClub->GetFinancialLoan()->GetRecommendedLoanBorrow( &pClub->GetPopularity() );
    m_bankSlider->SetValueRange( 0, (float)recloanborrow * 1.25f );  // allow up to 25% extra loan request  // TODO: Check this is right w/ roger
		m_bankSlider->SetValue( recloanborrow );

		m_bankRepayButton->Hide();

    m_fieldValues[AIFT_LOAN_OUTSTANDING]->SetText(lUIText(""));
    m_fieldValues[AIFT_WEEKLY_REPAYMENTS]->SetText(lUIText(""));
    m_fieldValues[AIFT_WEEKS_REMAINING]->SetText(lUIText(""));

    m_fieldLabels[AIFT_BORROW_AMOUNT]->SetText( lUIText( IDS_BORROWAMOUNT ) );
    m_fieldLabels[AIFT_BORROW_AMOUNT]->SetTextToUpper();

    m_fieldLabels[AIFT_LOAN_OUTSTANDING]->SetTextColour( L_ARGB( 127, 80, 80, 80 ) );
    m_fieldLabels[AIFT_WEEKLY_REPAYMENTS]->SetTextColour( L_ARGB( 127, 80, 80, 80 ) );
    m_fieldLabels[AIFT_WEEKS_REMAINING]->SetTextColour( L_ARGB( 127, 80, 80, 80 ) );

		m_bankApplyButton->Show();
	}
}


void FDAccountsScreen::Init()
{
  m_accountsScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "accounts_screen" );
  m_accountsScreen->Hide();

  // slider bars / buttons / text on bottom screen
  FDBackingWidget * styleBacking = FD_GAME_NEW FDBackingWidget ( m_accountsScreen, 0, "bacstyle_hdr", L_ARGB(80, 0, 0, 0), LUI_LEFT_TOP, lUIPoint( 19, 77 ), lUIPoint( 237, 100 ) );
  //FDBackingWidget * styleBacking2 = FD_GAME_NEW FDBackingWidget ( m_accountsScreen, 0, "bacstyle_hdr", L_ARGB(80, 0, 0, 0), LUI_LEFT_TOP, lUIPoint( 19, 107 ), lUIPoint( 237, 40 ) );
  //FDBackingWidget * styleBacking3 = FD_GAME_NEW FDBackingWidget ( m_accountsScreen, 0, "bacstyle_hdr", L_ARGB(80, 0, 0, 0), LUI_LEFT_TOP, lUIPoint( 19, 162 ), lUIPoint( 237, 20 ) );

  // Accounts on top screen
  m_accountsScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "accounts_screen_sub" );
  m_accountsScreenSub->Hide();

  // Create background stripes, field stripes, field labels, and field value widgets.
  for (int i = 0; i < (int)AIFT_TOTAL_INCOME; i++)
  {
	  m_backgroundStripes[i] = FD_GAME_NEW lUISpriteWidget(m_accountsScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
		  lUIPoint(0, 53 + (14 * i)), lUIPoint(256, 12));
	  if (i < AIFT_GENERAL_WAGES)
	  {
		  m_fieldStripes[i] = FD_GAME_NEW lUISpriteWidget(m_accountsScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_green"), LUI_TOP_LEFT,
			  lUIPoint(150, 53 + (14 * i)), lUIPoint(82, 12));
	  }
	  else
	  {
		  m_fieldStripes[i] = FD_GAME_NEW lUISpriteWidget(m_accountsScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_red"), LUI_TOP_LEFT,
			  lUIPoint(150, 53 + (14 * i)), lUIPoint(82, 12));
	  }

	  m_fieldLabels[i] = FD_GAME_NEW lUITextWidget(m_accountsScreenSub, 0, "label", lUIText(""), LUI_TOP_LEFT, lUIPoint(0, 53 + (14 * i)), lUIPoint(127, 12));
	  m_fieldValues[i] = FD_GAME_NEW lUITextWidget(m_accountsScreenSub, 0, "label", lUIText(""), LUI_TOP_LEFT, lUIPoint(150, 53 + (14 * i)), lUIPoint(82, 12));
  }

  for (int i = (int)AIFT_TOTAL_INCOME; i < AIFT_BORROW_AMOUNT; i++)
  {
	  m_backgroundStripes[i] = FD_GAME_NEW lUISpriteWidget(m_accountsScreen, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
      lUIPoint(20, 28 + (14 * (i - AIFT_TOTAL_INCOME))), lUIPoint(236, 12));
	  m_fieldStripes[i] = FD_GAME_NEW lUISpriteWidget(m_accountsScreen, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_LEFT,
      lUIPoint(150, 28 + (14 * (i - AIFT_TOTAL_INCOME))), lUIPoint(82, 12));
    m_fieldLabels[i] = FD_GAME_NEW lUITextWidget(m_accountsScreen, 0, "label", lUIText(""), LUI_TOP_LEFT, lUIPoint(20, 28 + (14 * (i - AIFT_TOTAL_INCOME))), lUIPoint(118, 12));
    m_fieldValues[i] = FD_GAME_NEW lUITextWidget(m_accountsScreen, 0, "label", lUIText(""), LUI_TOP_LEFT, lUIPoint(150, 28 + (14 * (i - AIFT_TOTAL_INCOME))), lUIPoint(82, 12));
  }

  m_periodSelect = FD_GAME_NEW FDComboWidget( m_accountsScreen, 0, "period_combo", lUIText(""), LUI_CENTER_TOP, lUIPoint( 138, 2 ), lUIPoint( 200, 19 ) );
  m_periodSelect->SetAllToUpper( true );

  m_periodSelect->AddItem( lUIText( "YEAR TO DATE" ) );
  m_periodSelect->AddItem( lUIText( "WEEKLY" ) );

  m_periodSelect->SignalSelection.Connect( this, &FDAccountsScreen::OnPeriodCombo );

  // loan info
  m_loanStripe = FD_GAME_NEW lUISpriteWidget(m_accountsScreen, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT, lUIPoint(20, 81), lUIPoint(236, 12));
  // TODO LOCALE
  m_loanText = FD_GAME_NEW lUITextWidget(m_accountsScreen, 0, "loantext", lUIText("LOAN"), LUI_TOP_LEFT, lUIPoint(20, 81), lUIPoint(236, 12));
  m_loanText->SetAlign(LUI_CENTER_CENTER);
  m_loanText->SetTextToUpper();

  m_backgroundStripes[AIFT_BORROW_AMOUNT] = FD_GAME_NEW lUISpriteWidget(m_accountsScreen, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
	  lUIPoint(20, 95), lUIPoint(236, 12));
  m_fieldStripes[AIFT_BORROW_AMOUNT] = FD_GAME_NEW lUISpriteWidget(m_accountsScreen, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_LEFT,
	  lUIPoint(150, 95), lUIPoint(82, 12));
  m_fieldLabels[AIFT_BORROW_AMOUNT] = FD_GAME_NEW lUITextWidget(m_accountsScreen, 0, "label", lUIText(""), LUI_TOP_LEFT, lUIPoint(20, 95), lUIPoint(118, 12));
  m_fieldValues[AIFT_BORROW_AMOUNT] = FD_GAME_NEW lUITextWidget(m_accountsScreen, 0, "label", lUIText(""), LUI_TOP_LEFT, lUIPoint(150, 95), lUIPoint(82, 12));

  for (int i = (int)AIFT_LOAN_OUTSTANDING; i < AIFT_MAX; i++)
  {
	  m_backgroundStripes[i] = FD_GAME_NEW lUISpriteWidget(m_accountsScreen, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
      lUIPoint(20, 148 + (14 * (i - AIFT_LOAN_OUTSTANDING))), lUIPoint(236, 12));
	  m_fieldStripes[i] = FD_GAME_NEW lUISpriteWidget(m_accountsScreen, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_LEFT,
      lUIPoint(150, 148 + (14 * (i - AIFT_LOAN_OUTSTANDING))), lUIPoint(82, 12));
    m_fieldLabels[i] = FD_GAME_NEW lUITextWidget(m_accountsScreen, 0, "label", lUIText(""), LUI_TOP_LEFT, lUIPoint(20, 148 + (14 * (i - AIFT_LOAN_OUTSTANDING))), lUIPoint(118, 12));
    m_fieldValues[i] = FD_GAME_NEW lUITextWidget(m_accountsScreen, 0, "label", lUIText(""), LUI_TOP_LEFT, lUIPoint(150, 148 + (14 * (i - AIFT_LOAN_OUTSTANDING))), lUIPoint(82, 12));
  }

  m_bankSlider = FD_GAME_NEW FDSliderWidget( m_accountsScreen, 0, "bank_slider", FDSliderWidget::SLIDER_FLAG_POUND, lUIText(""), LUI_TOP_LEFT, lUIPoint( 120, 107 ) );

  m_bankApplyButton = FD_GAME_NEW FDButtonWidget( m_accountsScreen, 0, "apply_button", FDResourceManager::GetSprite("ui","button75"),lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPoint( 20+256/2, 132 ) );
  m_bankApplyButton->SetText(lUIText("APPLY"));

  m_bankRepayButton = FD_GAME_NEW FDButtonWidget( m_accountsScreen, 0, "repay_button", FDResourceManager::GetSprite("ui","button75"),lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPoint( 20+256/2, 132 ) ); // 99
  m_bankRepayButton->SetText(lUIText("REPAY"));

  m_bankApplyButton->SignalControlCommand.Connect( this, &FDAccountsScreen::OnApplyLoan );
  m_bankRepayButton->SignalControlCommand.Connect( this, &FDAccountsScreen::OnRepayLoan );

  for (int i = 0; i < AIFT_MAX; i++)
  {
	  m_fieldLabels[i]->SetAlign(LUI_CENTER_RIGHT);
	  m_fieldValues[i]->SetAlign(LUI_CENTER_CENTER);
	  m_fieldValues[i]->PushToBack();
  }

  m_fieldLabels[AIFT_MERCHANDISE]->SetText(lUIText(IDS_MERCHANDISE));
  m_fieldLabels[AIFT_SPONSOR]->SetText(lUIText(IDS_SPONSORSHIP));
  m_fieldLabels[AIFT_BROADCAST_RIGHTS]->SetText(lUIText(IDS_TELEVISION));
  m_fieldLabels[AIFT_GENERAL_WAGES]->SetText(lUIText(IDS_GENERALWAGES));
  m_fieldLabels[AIFT_MANAGER_WAGES]->SetText(lUIText(IDS_MANAGERSWAGES));
  m_fieldLabels[AIFT_PLAYER_WAGES]->SetText(lUIText(IDS_PLAYERWAGES));
  m_fieldLabels[AIFT_EMPLOYEE_WAGES]->SetText(lUIText(IDS_EMPLOYEEWAGES));
  m_fieldLabels[AIFT_YOUTH_WAGES]->SetText(lUIText(IDS_YOUTHWAGES));
  m_fieldLabels[AIFT_RUNNING_COSTS]->SetText(lUIText(IDS_RUNNINGCOSTS));
  m_fieldLabels[AIFT_TOTAL_INCOME]->SetText(lUIText(IDS_TOTALINCOME));
  m_fieldLabels[AIFT_TOTAL_EXPENDITURE]->SetText(lUIText(IDS_TOTALEXPENDITURE));
  m_fieldLabels[AIFT_CLUB_BALANCE]->SetText(lUIText(IDS_CLUBBALANCELABEL));
  m_fieldLabels[AIFT_BORROW_AMOUNT]->SetText(lUIText(IDS_BORROWAMOUNT));
  m_fieldLabels[AIFT_LOAN_OUTSTANDING]->SetText(lUIText("LOAN OUTSTANDING"));
  m_fieldLabels[AIFT_WEEKLY_REPAYMENTS]->SetText(lUIText("WEEKLY REPAYMENTS"));
  m_fieldLabels[AIFT_WEEKS_REMAINING]->SetText(lUIText("WEEKS REMAINING"));

  m_fieldLabels[AIFT_MERCHANDISE]->SetTextToUpper();
  m_fieldLabels[AIFT_SPONSOR]->SetTextToUpper();
  m_fieldLabels[AIFT_BROADCAST_RIGHTS]->SetTextToUpper();
  m_fieldLabels[AIFT_GENERAL_WAGES]->SetTextToUpper();
  m_fieldLabels[AIFT_MANAGER_WAGES]->SetTextToUpper();
  m_fieldLabels[AIFT_PLAYER_WAGES]->SetTextToUpper();
  m_fieldLabels[AIFT_EMPLOYEE_WAGES]->SetTextToUpper();
  m_fieldLabels[AIFT_YOUTH_WAGES]->SetTextToUpper();
  m_fieldLabels[AIFT_RUNNING_COSTS]->SetTextToUpper();
  m_fieldLabels[AIFT_TOTAL_INCOME]->SetTextToUpper();
  m_fieldLabels[AIFT_TOTAL_EXPENDITURE]->SetTextToUpper();
  m_fieldLabels[AIFT_CLUB_BALANCE]->SetTextToUpper();
  m_fieldLabels[AIFT_BORROW_AMOUNT]->SetTextToUpper();
}

void FDAccountsScreen::DoShow()
{
  m_accountsScreen->Show();
  m_accountsScreenSub->Show();
}

void FDAccountsScreen::DoHide()
{
  m_accountsScreen->Hide();
  m_accountsScreenSub->Hide();
}

void FDAccountsScreen::SerializeState( FDStateStorage &state )
{
  if( state.IsSaving() )
  {
    int selectedPeriod  = m_periodSelect->GetSelectedItem();
    state << selectedPeriod;
    lLogFmt("Saving period %d\n", selectedPeriod);
  } else
  {
    int selectedPeriod;
    state << selectedPeriod;
    lLogFmt("Loading period %d\n", selectedPeriod);
    m_periodSelect->SetSelectedItem(selectedPeriod, false);
  }
}


void FDAccountsScreen::OnPeriodCombo( FDComboWidget *, int idx )
{
  lLogFmt("Period: %d\n", idx);
  if( idx == 0 ) m_bAnnual = true;
  else           m_bAnnual = false;
  Update();
}

void FDAccountsScreen::OnApplyLoan( lUIControlWidget * )
{
  CCalendar date = WorldData().GetCurrentDate();

  if( m_lastLoanedOut != date.GetDay() )
  {

    float dValue = (float)m_bankSlider->GetValue();
    if (dValue > 0)
    {
      CClub* pClub = WorldData().GetCurrentUserClub();
      ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);

      // No more loans until the next day aslong as the loan was accepted
      if( pClub->DoUserApplyFinancialLoan(dValue) )
        m_lastLoanedOut = date.GetDay();

      Update();
    }

    FDSound::PlaySoundEffect( FD_SFX_UI_KERCHING );
  }
  else if(m_bankSlider->GetValue() == 0)
  {
    UserMessageBox(IDS_LOANTOOSMALL);
  }
  else
  {
    // Show message NO!
    UserMessageBox(IDS_LOANREFUSED);
  }
}

void FDAccountsScreen::OnRepayLoan( lUIControlWidget * )
{
  float dValue = (float)m_bankSlider->GetValue();
	if (dValue > 0)
	{
		ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
		WorldData().GetCurrentUserClub()->DoUserRepayFinancialLoan(dValue);
		Update();
	}
}

