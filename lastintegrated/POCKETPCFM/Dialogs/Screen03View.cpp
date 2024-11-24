/*

	Screen03.cpp

	Status

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "PlayerNameTextRegion.h"
#include "ClubFixtureGrid.h"
#include "Form.h"
#include "Dialog.h"
#include "MatchInfo.h"
#include "MessageDisplay.h"
#include "CurrentForm.h"
#include "PopupPlayerMenu.h"
#include "IndexedIntArray.h"
#include "ProgressBar.h"

#include "YouthGrid.h"
#include "EmployeeGrid.h"
#include "PlayerSearch.h"
#include "Screen05Base.h"
#include "SoccerSquadTraining.h"
#include "CupDisplayEvent.h"
#include "Competition.h"
#include "LeadingGoalScorer.h"
#include "LeadingDisciplinaryPoints.h"

#include "Screen55Dlg.h"

#include "Screen03View.h"
#include "FootballDirector.h"
#include "FootballDirectorDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// // CScreen03View

static CExtRect PlayerNameRect(8, 234, 120, 256);

IMPLEMENT_DYNCREATE(CScreen03View, CFormView)

BEGIN_MESSAGE_MAP(CScreen03View, CBitmapFormView)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)

	ON_COMMAND(ID_SCREEN2, OnScreen02)				// Screen 2 Cup Schedules
	ON_COMMAND(ID_MENU_SCREEN03, OnScreen03)		// Screen 3 Status
	ON_COMMAND(ID_MENU_SCREEN05, OnScreen05)
	ON_COMMAND(ID_SCREEN8, OnScreen08)				// Screen 8 Wages and contracts
	ON_COMMAND(ID_SCREEN09, OnScreen09)				// Screen 9 Club History
	ON_COMMAND(ID_SCREEN11, OnScreen11)				// Screen 11 Club and league news
	ON_COMMAND(ID_SCREEN18, OnScreen18)				// Screen 18 Club fixtures
	ON_COMMAND(ID_SCREEN19, OnScreen19)				// Screen 19 Next league fixtures
	ON_COMMAND(ID_SCREEN20, OnScreen20)				// Screen 20 Cup Fixtures
	ON_COMMAND(ID_SCREEN22, OnScreen22)				// Screen 22 Bank
	ON_COMMAND(ID_SCREEN26, OnScreen26)				// Screen 26 Accounts(WMY)
	ON_COMMAND(ID_SCREEN27TACTICS, OnScreen27Tactics)		// Screen 27 Match GetTactics
	ON_COMMAND(ID_SCREEN27FORMATION, OnScreen27Formation)	// Screen 27 Match Formation
	ON_COMMAND(ID_SCREEN27SPECIALPLAYERS, OnScreen27SpecialPlayers)		// Screen 27 Special Players
	ON_COMMAND(ID_SCREEN33, OnScreen33)				// Screen 33 Leading goalscorers
	ON_COMMAND(ID_SCREEN36, OnScreen36)				// Screen 36 Training
	ON_COMMAND(ID_SCREEN39, OnScreen39)				// Screen 39 Job Centre
	ON_COMMAND(ID_SCREEN41, OnScreen41)				// Screen 41 Youth squad
	ON_COMMAND(ID_SCREEN46, OnScreen46)				// Screen 46 Monthly Awards
	ON_COMMAND(ID_SCREEN48, OnScreen48)				// Screen 48 Transfers
	ON_COMMAND(ID_MENU_SCREEN50, OnScreen50)
	ON_COMMAND(ID_NEXT_MATCH, OnNextMatch)
	ON_COMMAND(ID_NEXT_DATE, OnNextDate)

	ON_COMMAND(ID_ACTION1, OnAction1)
	ON_COMMAND(ID_ACTION2, OnAction2)
	ON_COMMAND(ID_ACTION3, OnAction3)
	ON_COMMAND(ID_ACTION4, OnAction4)
	ON_COMMAND(ID_ACTION5, OnAction5)
	ON_COMMAND(ID_ACTION6, OnAction6)
	ON_COMMAND(ID_ACTION7, OnAction7)
	ON_COMMAND(ID_ACTION8, OnAction8)
	ON_COMMAND(ID_ACTION9, OnAction9)
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnAction1
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnAction1()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	switch (m_CurrentView)
	{
		case SCREEN03_STATUSVIEW:
			OnScreen05();
			break;
		case SCREEN05_TEAMVIEW:
			WorldData().OnViewPlayerDetails((CPlayer*)m_Screen.GetGrid().GetCurrentRowPointer());
			break;
		case SCREEN50_TABLEVIEW:
			break;
		case SCREEN11_NEWSVIEW:
			break;
		case SCREEN26_ACCOUNTSVIEW:
			m_eWhichPeriod = WEEKLY_FINANCE;
			DoBuildGridScreen26();
			break;
		case SCREEN18_CLUBFIXTURESVIEW:
			m_ClubFixtureGrid.DoShowMatchSummary(m_ClubFixtureGrid.GetCurrentRow());
			break;
		case SCREEN28_PROGRESSVIEW:
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			theApp.OnPlayerSearch();
			DoBuildPlayerList();
			break;
		case SCREEN08_CONTRACTVIEW:
			m_SortOrder = SORT_LOW;
			DoSort();
			break;
		case SCREEN22_BANKVIEW:
			if (m_TextSelector[eBORROW].GetCurValue() > 0)
			{
				CClub* pClub = WorldData().GetCurrentUserClub();
				EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
				pClub->DoUserApplyFinancialLoan(m_TextSelector[eBORROW].GetCurValue());
				DoUpdateLoanDetails();
			}
			break;
		case SCREEN39_JOBCENTREVIEW:
			m_EmployeeGrid.SetCurrentEmployee();
			if (m_TypeList.GetCurSel() == 0)
			{
				theApp.OnViewOwnedEmployeeInformation();
			}
			else
			{
				theApp.OnViewNotOwnedEmployeeInformation();
			}
			DoBuildGridScreen39();
			ReDraw();
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			m_YouthGrid.SetCurrentYouth();
			if (m_TypeList.GetCurSel() == 0)
			{
				theApp.OnViewOwnedYouthInformation();
			}
			else
			{
				theApp.OnViewNotOwnedYouthInformation();
			}
			DoBuildGridScreen41();
			ReDraw();			break;
		case SCREEN36_TRAININGVIEW:
			break;
		case SCREEN02_CUPSCHEDULEVIEW:
			m_CupSelector.OnIncCurSel();
			OnSelchangeCupListScreen02();
			break;
		case SCREEN09_CLUBRECORDSVIEW:
			break;
		case SCREEN19_LEAGUEFIXTURESVIEW:
			m_DivisionSelector.GetCurrentDivision().NextMatchDateThisSeason(m_TheDate);
			DoBuildGridScreen19();
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			OnNextCupRoundScreen20();
			break;
		case SCREEN27_MATCHTACTICSVIEW:
			m_TacticsData.DoToggleCounterAttack();
			ReDraw();
			break;
		case SCREEN27_MATCHSPECIALPLAYERSVIEW:
			m_Captain.OnIncCurSel();
			DoUpdateData();
			break;
		case SCREEN27_MATCHFORMATIONVIEW:
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			break;
		default:
			EXT_ASSERT(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnAction2
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnAction2()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	switch (m_CurrentView)
	{
		case SCREEN03_STATUSVIEW:
			OnScreen50();
			break;
		case SCREEN05_TEAMVIEW:
			WorldData().OnViewPlayerDetails((CPlayer*)m_Screen.GetGrid().GetCurrentRowPointer());
			break;
		case SCREEN50_TABLEVIEW:
			break;
		case SCREEN11_NEWSVIEW:
			break;
		case SCREEN26_ACCOUNTSVIEW:
			m_eWhichPeriod = ANNUAL_FINANCE;
			DoBuildGridScreen26();
			break;
		case SCREEN18_CLUBFIXTURESVIEW:
			m_ClubFixtureGrid.DoShowOpponentSquad(m_ClubFixtureGrid.GetCurrentRow());
			break;
		case SCREEN28_PROGRESSVIEW:
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			EXT_ASSERT(false);		// @@TODO@@
			//OnSortHighestSkill();
			break;
		case SCREEN08_CONTRACTVIEW:
			m_SortOrder = SORT_HIGH;
			DoSort();
			break;
		case SCREEN22_BANKVIEW:
			if (m_TextSelector[eREPAY].GetCurValue() > 0)
			{
				EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
				WorldData().GetCurrentUserClub()->DoUserRepayFinancialLoan(m_TextSelector[eREPAY].GetCurValue());
				DoUpdateLoanDetails();
			}
			break;
		case SCREEN39_JOBCENTREVIEW:
			if (WorldData().GetCurrentUserClub()->AskSackEmployee((CEmployee*)m_EmployeeGrid.GetCurrentRowPointer()) == true)
			{
				DoBuildGridScreen39();
				ReDraw();
			}
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			if (WorldData().GetCurrentUserClub()->AskSackYouth((CYouth*)m_YouthGrid.GetCurrentRowPointer()) == true)
			{
				DoBuildGridScreen41();
				ReDraw();
			}
			break;
		case SCREEN36_TRAININGVIEW:
			break;
		case SCREEN02_CUPSCHEDULEVIEW:
			m_CupSelector.OnDecCurSel();
			OnSelchangeCupListScreen02();
			break;
		case SCREEN09_CLUBRECORDSVIEW:
			break;
		case SCREEN19_LEAGUEFIXTURESVIEW:
			m_DivisionSelector.GetCurrentDivision().PrevMatchDateThisSeason(m_TheDate);
			DoBuildGridScreen19();
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			OnPreviousCupRoundScreen20();
			break;
		case SCREEN27_MATCHTACTICSVIEW:
			m_TacticsData.DoToggleOffsideTrap();
			ReDraw();
			break;
		case SCREEN27_MATCHSPECIALPLAYERSVIEW:
			m_Captain.OnDecCurSel();
			DoUpdateData();
			break;
		case SCREEN27_MATCHFORMATIONVIEW:
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			break;
		default:
			EXT_ASSERT(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnAction3
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnAction3()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	switch (m_CurrentView)
	{
		case SCREEN03_STATUSVIEW:
			OnScreen26();
			break;
		case SCREEN05_TEAMVIEW:
			m_Screen.OnSelchangeTab(eSKILL);
			ReDraw();
			break;
		case SCREEN50_TABLEVIEW:
			m_ViewMode = 0;
			CBitmapFormView::GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN50);
			DoBuildGridScreen50();
			break;
		case SCREEN11_NEWSVIEW:
			break;
		case SCREEN26_ACCOUNTSVIEW:
			break;
		case SCREEN18_CLUBFIXTURESVIEW:
			break;
		case SCREEN28_PROGRESSVIEW:
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			EXT_ASSERT(false);		// @@TODO@@
			//OnSortLowestSkill()
			break;
		case SCREEN08_CONTRACTVIEW:
			m_SortOrder = SORT_LOW;
			DoSort();
			break;
		case SCREEN22_BANKVIEW:
			break;
		case SCREEN39_JOBCENTREVIEW:
			WorldData().SetCurrentEmployee((CEmployee*)m_EmployeeGrid.GetCurrentRowPointer());
			theApp.OnNegotiateEmployeeContract();
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			if (WorldData().GetCurrentUserClub()->AskSackYouth((CYouth*)m_YouthGrid.GetCurrentRowPointer()) == true)
			{
				DoBuildGridScreen41();
				ReDraw();
			}
			break;
		case SCREEN36_TRAININGVIEW:
			break;
		case SCREEN02_CUPSCHEDULEVIEW:
			break;
		case SCREEN09_CLUBRECORDSVIEW:
			break;
		case SCREEN19_LEAGUEFIXTURESVIEW:
			m_DivisionSelector.OnIncCurSel();
			OnSelchangeDivisionList();
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			OnNextCupScreen20();
			break;
		case SCREEN27_MATCHTACTICSVIEW:
			break;
		case SCREEN27_MATCHSPECIALPLAYERSVIEW:
			m_PenaltyTaker.OnIncCurSel();
			DoUpdateData();
			break;
		case SCREEN27_MATCHFORMATIONVIEW:
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			m_ViewMode = 0;
			CBitmapFormView::GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN3302);
			OnSelchangeDivisionList();
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			break;
		default:
			EXT_ASSERT(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnAction4
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnAction4()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	switch (m_CurrentView)
	{
		case SCREEN03_STATUSVIEW:
			OnScreen11();
			break;
		case SCREEN05_TEAMVIEW:
			m_Screen.OnSelchangeTab(eSTATUS);
			ReDraw();
			break;
		case SCREEN50_TABLEVIEW:
			m_ViewMode = 1;
			CBitmapFormView::GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN5001);
			DoBuildGridScreen50();
			break;
		case SCREEN11_NEWSVIEW:
			break;
		case SCREEN26_ACCOUNTSVIEW:
			break;
		case SCREEN18_CLUBFIXTURESVIEW:
			break;
		case SCREEN28_PROGRESSVIEW:
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			EXT_ASSERT(false);		// @@TODO@@
			//OnSortHighestPrice();
			break;
		case SCREEN08_CONTRACTVIEW:
			switch (TheGrid.GetCurrentRowPointerType())
			{
				case PLAYERCONTRACT:
					WorldData().SetCurrentPlayer((CPlayer*)TheGrid.GetCurrentRowPointer());
					theApp.OnNegotiatePlayerContract();
					break;

				case EMPLOYEECONTRACT:
					WorldData().SetCurrentEmployee((CEmployee*)TheGrid.GetCurrentRowPointer());
					theApp.OnNegotiateEmployeeContract();
					break;

				case YOUTHCONTRACT:
					WorldData().SetCurrentYouth((CYouth*)TheGrid.GetCurrentRowPointer());
					theApp.OnPromoteYouth();
					break;
			}
			DoBuildGridScreen08();
			break;
		case SCREEN22_BANKVIEW:
			break;
		case SCREEN39_JOBCENTREVIEW:
			WorldData().SetCurrentEmployee((CEmployee*)m_EmployeeGrid.GetCurrentRowPointer());
			theApp.OnHireEmployee();
			DoBuildGridScreen39();
			ReDraw();
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			m_YouthGrid.SetCurrentYouth();
			theApp.OnHireYouth();
			DoBuildGridScreen41();
			ReDraw();
			break;
		case SCREEN36_TRAININGVIEW:
			break;
		case SCREEN02_CUPSCHEDULEVIEW:
			break;
		case SCREEN09_CLUBRECORDSVIEW:
			break;
		case SCREEN19_LEAGUEFIXTURESVIEW:
			m_DivisionSelector.OnDecCurSel();
			OnSelchangeDivisionList();
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			OnPreviousCupScreen20();
			break;
		case SCREEN27_MATCHTACTICSVIEW:
			break;
		case SCREEN27_MATCHSPECIALPLAYERSVIEW:
			m_PenaltyTaker.OnDecCurSel();
			DoUpdateData();
			break;
		case SCREEN27_MATCHFORMATIONVIEW:
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			m_ViewMode = 1;
			CBitmapFormView::GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN3301);
			OnSelchangeDivisionList();
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			break;
		default:
			EXT_ASSERT(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnAction5
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnAction5()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	switch (m_CurrentView)
	{
		case SCREEN03_STATUSVIEW:
			break;
		case SCREEN05_TEAMVIEW:
			m_Screen.OnSelchangeTab(eFINANCE);
			ReDraw();
			break;
		case SCREEN50_TABLEVIEW:
			m_ViewMode = 2;
			CBitmapFormView::GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN5002);
			DoBuildGridScreen50();
			break;
		case SCREEN11_NEWSVIEW:
			break;
		case SCREEN26_ACCOUNTSVIEW:
			break;
		case SCREEN18_CLUBFIXTURESVIEW:
			break;
		case SCREEN28_PROGRESSVIEW:
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			EXT_ASSERT(false);		// @@TODO@@
			//OnSortLowestPrice();
			break;
		case SCREEN08_CONTRACTVIEW:
			switch(TheGrid.GetCurrentRowPointerType())
			{
				case PLAYERCONTRACT:
					WorldData().SetCurrentPlayer((CPlayer*)TheGrid.GetCurrentRowPointer());
					theApp.OnViewOwnedPlayerInformation();
					break;

				case EMPLOYEECONTRACT:
					WorldData().SetCurrentEmployee((CEmployee*)TheGrid.GetCurrentRowPointer());
					theApp.OnViewOwnedEmployeeInformation();
					break;

				case YOUTHCONTRACT:
					WorldData().SetCurrentYouth((CYouth*)TheGrid.GetCurrentRowPointer());
					theApp.OnViewOwnedYouthInformation();
					break;
			}
			DoBuildGridScreen08();
			break;
		case SCREEN22_BANKVIEW:
			break;
		case SCREEN39_JOBCENTREVIEW:
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			break;
		case SCREEN36_TRAININGVIEW:
			break;
		case SCREEN02_CUPSCHEDULEVIEW:
			break;
		case SCREEN09_CLUBRECORDSVIEW:
			break;
		case SCREEN19_LEAGUEFIXTURESVIEW:
			OnViewMatchReport();
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			break;
		case SCREEN27_MATCHTACTICSVIEW:
			break;
		case SCREEN27_MATCHSPECIALPLAYERSVIEW:
			m_CornerTaker.OnIncCurSel();
			DoUpdateData();
			break;
		case SCREEN27_MATCHFORMATIONVIEW:
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			break;
		default:
			EXT_ASSERT(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnAction6
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnAction6()
{
	switch (m_CurrentView)
	{
		case SCREEN03_STATUSVIEW:
			break;
		case SCREEN05_TEAMVIEW:
			m_Screen.OnSelchangeTab(eSTATS);
			ReDraw();
			break;
		case SCREEN50_TABLEVIEW:
			m_ViewMode = 3;
			CBitmapFormView::GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN5003);
			DoBuildGridScreen50();
			break;
		case SCREEN11_NEWSVIEW:
			break;
		case SCREEN26_ACCOUNTSVIEW:
			break;
		case SCREEN18_CLUBFIXTURESVIEW:
			break;
		case SCREEN28_PROGRESSVIEW:
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			EXT_ASSERT(false);		// @@TODO@@
			//OnViewDetails();
			break;
		case SCREEN08_CONTRACTVIEW:
			m_WhichEvents = 1;
			OnSwitchTab();
			break;
		case SCREEN22_BANKVIEW:
			break;
		case SCREEN39_JOBCENTREVIEW:
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			break;
		case SCREEN36_TRAININGVIEW:
			break;
		case SCREEN02_CUPSCHEDULEVIEW:
			break;
		case SCREEN09_CLUBRECORDSVIEW:
			break;
		case SCREEN19_LEAGUEFIXTURESVIEW:
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			{
				CScreen55Dlg theDlg(m_CupSelector.GetCurrentCup());
				theDlg.DoModal();
			}
			break;
		case SCREEN27_MATCHTACTICSVIEW:
			break;
		case SCREEN27_MATCHSPECIALPLAYERSVIEW:
			m_CornerTaker.OnDecCurSel();
			DoUpdateData();
			break;
		case SCREEN27_MATCHFORMATIONVIEW:
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			break;
		default:
			EXT_ASSERT(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnAction7
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnAction7()
{
	switch (m_CurrentView)
	{
		case SCREEN03_STATUSVIEW:
			break;
		case SCREEN05_TEAMVIEW:
			break;
		case SCREEN50_TABLEVIEW:
			m_ViewMode = 4;
			CBitmapFormView::GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN5004);
			DoBuildGridScreen50();
			break;
		case SCREEN11_NEWSVIEW:
			break;
		case SCREEN26_ACCOUNTSVIEW:
			break;
		case SCREEN18_CLUBFIXTURESVIEW:
			break;
		case SCREEN28_PROGRESSVIEW:
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			EXT_ASSERT(false);		// @@TODO@@
			//OnMakeOffer();
			break;
		case SCREEN08_CONTRACTVIEW:
			m_WhichEvents = 0;
			OnSwitchTab();
			break;
		case SCREEN22_BANKVIEW:
			break;
		case SCREEN39_JOBCENTREVIEW:
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			break;
		case SCREEN36_TRAININGVIEW:
			break;
		case SCREEN02_CUPSCHEDULEVIEW:
			break;
		case SCREEN09_CLUBRECORDSVIEW:
			break;
		case SCREEN19_LEAGUEFIXTURESVIEW:
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			break;
		case SCREEN27_MATCHTACTICSVIEW:
			break;
		case SCREEN27_MATCHSPECIALPLAYERSVIEW:
			m_FreeKickTaker.OnIncCurSel();
			DoUpdateData();
			break;
		case SCREEN27_MATCHFORMATIONVIEW:
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			break;
		default:
			EXT_ASSERT(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnAction8
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnAction8()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	switch (m_CurrentView)
	{
		case SCREEN03_STATUSVIEW:
			break;
		case SCREEN05_TEAMVIEW:
			WorldData().OnViewSquadDetails((CClub*)TheGrid.GetCurrentRowPointer());
			break;
		case SCREEN50_TABLEVIEW:
			break;
		case SCREEN11_NEWSVIEW:
			break;
		case SCREEN26_ACCOUNTSVIEW:
			break;
		case SCREEN18_CLUBFIXTURESVIEW:
			break;
		case SCREEN28_PROGRESSVIEW:
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			break;
		case SCREEN08_CONTRACTVIEW:
			m_WageMultiplier = 52;
			DoBuildGridScreen08(true);
			break;
		case SCREEN22_BANKVIEW:
			break;
		case SCREEN39_JOBCENTREVIEW:
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			break;
		case SCREEN36_TRAININGVIEW:
			break;
		case SCREEN02_CUPSCHEDULEVIEW:
			break;
		case SCREEN09_CLUBRECORDSVIEW:
			break;
		case SCREEN19_LEAGUEFIXTURESVIEW:
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			break;
		case SCREEN27_MATCHTACTICSVIEW:
			break;
		case SCREEN27_MATCHSPECIALPLAYERSVIEW:
			m_FreeKickTaker.OnIncCurSel();
			DoUpdateData();
			break;
		case SCREEN27_MATCHFORMATIONVIEW:
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			break;
		default:
			EXT_ASSERT(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnAction9
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnAction9()
{
	switch (m_CurrentView)
	{
		case SCREEN03_STATUSVIEW:
			break;
		case SCREEN05_TEAMVIEW:
			break;
		case SCREEN50_TABLEVIEW:
			break;
		case SCREEN11_NEWSVIEW:
			break;
		case SCREEN26_ACCOUNTSVIEW:
			break;
		case SCREEN18_CLUBFIXTURESVIEW:
			break;
		case SCREEN28_PROGRESSVIEW:
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			break;
		case SCREEN08_CONTRACTVIEW:
			m_WageMultiplier = 1;
			DoBuildGridScreen08(true);
			break;
		case SCREEN22_BANKVIEW:
			break;
		case SCREEN39_JOBCENTREVIEW:
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			break;
		case SCREEN36_TRAININGVIEW:
			break;
		case SCREEN02_CUPSCHEDULEVIEW:
			break;
		case SCREEN09_CLUBRECORDSVIEW:
			break;
		case SCREEN19_LEAGUEFIXTURESVIEW:
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			break;
		case SCREEN27_MATCHTACTICSVIEW:
			break;
		case SCREEN27_MATCHSPECIALPLAYERSVIEW:
			break;
		case SCREEN27_MATCHFORMATIONVIEW:
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			break;
	default:
		EXT_ASSERT(false);
	}
}

void CScreen03View::OnScreen02()
{
	OnInitialiseView(SCREEN02_CUPSCHEDULEVIEW);
}
void CScreen03View::OnScreen03()
{
	OnInitialiseView(SCREEN03_STATUSVIEW);
}
void CScreen03View::OnScreen05()
{
	OnInitialiseView(SCREEN05_TEAMVIEW);
}
void CScreen03View::OnScreen08()
{
	OnInitialiseView(SCREEN08_CONTRACTVIEW);
}
void CScreen03View::OnScreen09()
{
	OnInitialiseView(SCREEN09_CLUBRECORDSVIEW);
}
void CScreen03View::OnScreen11()
{
	OnInitialiseView(SCREEN11_NEWSVIEW);
}
void CScreen03View::OnScreen18()
{
	WorldData().SetCurrentClub(WorldData().GetCurrentUserClub());
	WorldData().SetFixturesToView(MATCHES_ALL);
	OnInitialiseView(SCREEN18_CLUBFIXTURESVIEW);
}
void CScreen03View::OnScreen19()
{
	OnInitialiseView(SCREEN19_LEAGUEFIXTURESVIEW);
}
void CScreen03View::OnScreen20()
{
	OnInitialiseView(SCREEN20_CUPFIXTURESVIEW);
}
void CScreen03View::OnScreen22()
{
	OnInitialiseView(SCREEN22_BANKVIEW);
}
void CScreen03View::OnScreen26()
{
	OnInitialiseView(SCREEN26_ACCOUNTSVIEW);
}
void CScreen03View::OnScreen27Tactics()
{
	OnInitialiseView(SCREEN27_MATCHTACTICSVIEW);
}
void CScreen03View::OnScreen27Formation()
{
	OnInitialiseView(SCREEN27_MATCHFORMATIONVIEW);
}
void CScreen03View::OnScreen27SpecialPlayers()
{
	OnInitialiseView(SCREEN27_MATCHSPECIALPLAYERSVIEW);
}
void CScreen03View::OnScreen33()
{
	OnInitialiseView(SCREEN33_LEADINGGOALSCORERSVIEW);
}
void CScreen03View::OnScreen36()
{
	OnInitialiseView(SCREEN36_TRAININGVIEW);
}
void CScreen03View::OnScreen39()
{
	OnInitialiseView(SCREEN39_JOBCENTREVIEW);
}
void CScreen03View::OnScreen41()
{
	OnInitialiseView(SCREEN41_YOUTHSQUADVIEW);
}
void CScreen03View::OnScreen46()
{
	OnInitialiseView(SCREEN46_MONTHLYAWARDSVIEW);
}
void CScreen03View::OnScreen48()
{
	OnInitialiseView(SCREEN48_TRANSFERMARKETVIEW);
}
void CScreen03View::OnScreen50()
{
	OnInitialiseView(SCREEN50_TABLEVIEW);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnNextDate
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnNextDate()
{
#ifdef NOTITLEMENU
	// Process to specific date
	if (WorldData().GetCurrentDate().GetMatchDay() < 8)
	{
		WorldData().SetFastTest(true);
		OnInitialiseView(SCREEN28_PROGRESSVIEW);
		do
		{
			DoResetProgress();
			ReDraw();
			WorldData().DoProcessMatches(false, true);
			ReDraw();
		}
		//while (m_CurrentDate.GetSeason() < 12);
		while (WorldData().GetCurrentDate().GetMatchDay() != 8);  // 24 cl g1
		OnScreen03();
		WorldData().SetFastTest(false);
	}
#else
	if (WorldData().CurrentUserCantProgress() == false)
	{
		OnInitialiseView(SCREEN28_PROGRESSVIEW);
		//				   	         View  Summ
		WorldData().DoProcessMatches(true, true);
		OnScreen03();
		WorldData().CurrentUserCantProgress();
	}
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnNextMatch
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnNextMatch()
{
	if (WorldData().CurrentUserCantProgress() == true)
	{
		return;
	}
	else
	{
		OnInitialiseView(SCREEN28_PROGRESSVIEW);
		do
		{
			DoResetProgress();
			ReDraw();
			WorldData().DoProcessMatches(false, true);
			ReDraw();
		}
		while (WorldData().CurrentUserCanProgress() == true && WorldData().GetProcessingHalt() == false);
		OnScreen03();
	}
	WorldData().CurrentUserCantProgress();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnExitView
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnExitView()
{
	m_MessageDisplay.GetScrollBar()->ShowWindow(SW_HIDE);
	CBitmapFormView::GetGrid().GetScrollBar()->ShowWindow(SW_HIDE);
	switch (m_CurrentView)
	{
		case NO_VIEW:
			break;
		case SCREEN03_STATUSVIEW:
			break;
		case SCREEN05_TEAMVIEW:
			break;
		case SCREEN50_TABLEVIEW:
			break;
		case SCREEN11_NEWSVIEW:
			break;
		case SCREEN26_ACCOUNTSVIEW:
			break;
		case SCREEN18_CLUBFIXTURESVIEW:
			break;
		case SCREEN28_PROGRESSVIEW:
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			break;
		case SCREEN08_CONTRACTVIEW:
			break;
		case SCREEN22_BANKVIEW:
			break;
		case SCREEN39_JOBCENTREVIEW:
			m_TypeList.DoRemoveAllData();
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			m_TypeList.DoRemoveAllData();
			break;
		case SCREEN36_TRAININGVIEW:
			break;
		case SCREEN02_CUPSCHEDULEVIEW:
			break;
		case SCREEN09_CLUBRECORDSVIEW:
			break;
		case SCREEN19_LEAGUEFIXTURESVIEW:
			m_ResultList.RemoveList();
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			m_ResultList.RemoveList();
			break;
		case SCREEN27_MATCHTACTICSVIEW:
			break;
		case SCREEN27_MATCHSPECIALPLAYERSVIEW:
			break;
		case SCREEN27_MATCHFORMATIONVIEW:
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			break;
		default:
			EXT_ASSERT(false);
	}
	CBitmapFormView::DoRemoveAllData();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnInitialiseView
	Access:    	protected 
	Parameter: 	const eView _NewView
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnInitialiseView(const eView _NewView)
{
	if (_NewView != m_CurrentView)
	{
		OnExitView();
		m_CurrentView = _NewView;
		m_HasDivisionSelector = m_HasCupSelector = m_HasGrid = false;
		switch (m_CurrentView)
		{
			case SCREEN02_CUPSCHEDULEVIEW:
			case SCREEN20_CUPFIXTURESVIEW:
				m_HasCupSelector = true;
				m_CupSelector.DoInitialise(this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
				break;

			case SCREEN19_LEAGUEFIXTURESVIEW:
			case SCREEN33_LEADINGGOALSCORERSVIEW:
			case SCREEN46_MONTHLYAWARDSVIEW:
			case SCREEN48_TRANSFERMARKETVIEW:
			case SCREEN50_TABLEVIEW:
				m_HasDivisionSelector = true;
				m_DivisionSelector.DoInitialise(this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
				break;
		}
		int HelpTopic[] = 
		{
			0, IDS_HELPSCREEN03, IDS_HELPSCREEN05, IDS_HELPSCREEN50, IDS_HELPSCREEN11, IDS_HELPSCREEN26, IDS_HELPSCREEN18, 
			IDS_HELPSCREEN28, IDS_HELPSCREEN48, IDS_HELPSCREEN08, IDS_HELPSCREEN22, IDS_HELPSCREEN39, IDS_HELPSCREEN41, 
			IDS_HELPSCREEN36, IDS_HELPSCREEN02, IDS_HELPSCREEN09, IDS_HELPSCREEN19, IDS_HELPSCREEN20, IDS_HELPSCREEN27T, 
			IDS_HELPSCREEN27R, IDS_HELPSCREEN27F, IDS_HELPSCREEN33, IDS_HELPSCREEN46
		};
		int ScreenHeaderID[] =
		{
			0, IDS_SCREEN03, IDS_SCREEN05, IDS_SCREEN50, IDS_SCREEN11, IDS_SCREEN26, IDS_SCREEN18, IDS_SCREEN28, IDS_SCREEN48,
			IDS_SCREEN08, IDS_SCREEN22, IDS_SCREEN39, IDS_SCREEN41, IDS_SCREEN36, IDS_SCREEN02, IDS_SCREEN09, IDS_SCREEN19,
			IDS_SCREEN20, IDS_SCREEN27TACTICS, IDS_SCREEN27SPECIALPLAYERS, IDS_FORMATION, IDS_SCREEN3301, IDS_SCREEN46
		};
	
		CClub* pClub;
		CGrid& TheGrid = CBitmapFormView::GetGrid();
		switch (m_CurrentView)
		{
			case SCREEN03_STATUSVIEW:
				m_CurrentSelection = eCLUB;
				EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
				m_NewsText = new CTextRegion;
				m_NewsText->DoInitialise(GetBasicDisplay(), _T(""), COL_ALIGNC, true);
				m_ClubBalanceText.DoInitialise(GetBasicDisplay(), WorldData().GetCurrentUserClub(), COL_ALIGNC, true);
 				{
 					CExtString str;
 					str.Format(IDS_NEWSCOUNT, WorldData().GetCurrentUser()->GetNewsList().GetCount());
 					m_NewsText->SetText(str);
 				}
				GetBasicDisplay()->DoAddTextRegion(m_NewsText);
				break;
			case SCREEN05_TEAMVIEW:
				WorldData().SetCurrentClub(WorldData().GetCurrentUserClub());
				m_Screen.DoInitialise(m_hWnd, this, GetBasicDisplay(), &CBitmapFormView::GetGrid());
				break;
			case SCREEN50_TABLEVIEW:
				CBitmapFormView::GetGrid().SetHiliteBackgroundColour(TRANSCOLOUR);
				m_ViewMode = 0;
				OnSelchangeDivisionList();
				break;
			case SCREEN11_NEWSVIEW:
				{
					m_strlistNews.RemoveAll();
					m_MessageDisplay.RemoveAll();
					m_MessageDisplay.SetDrawLastFirst(true);
					CString Date = TEXT("");
					WorldData().GetCurrentUserClub()->GetActiveCountry().GetNewsList().DoBuildDisplayList(m_strlistNews, Date, true);
					WorldData().GetCurrentUser()->GetNewsList().DoBuildDisplayList(m_strlistNews, Date, true);
					m_MessageDisplay.AddStringList(m_strlistNews, BLUE);
				}
				break;
			case SCREEN26_ACCOUNTSVIEW:
				TheGrid.OnInitialUpdate(this, GetBasicDisplay(), LC_SHOWCOLUMNS | LC_NOHIGHLIGHT, GAPY - 2);
				TheGrid.SetTextFont(GetBasicDisplay()->GetSmallFont());
				m_ClubBalanceText.DoInitialise(GetBasicDisplay(), WorldData().GetCurrentUserClub(), COL_ALIGNR, true);
				m_eWhichPeriod = WEEKLY_FINANCE;
				DoBuildGridScreen26();
				break;
			case SCREEN18_CLUBFIXTURESVIEW:
				m_ClubFixtureGrid.SetMatchType(MATCHES_ALL);
				m_ClubFixtureGrid.OnInitialUpdate(this, GetBasicDisplay(), WorldData().GetCurrentClub());
				break;
			case SCREEN28_PROGRESSVIEW:
				break;
			case SCREEN48_TRANSFERMARKETVIEW:
				TheGrid.SetRowHeight(20);
				m_ClubBalanceText.DoInitialise(GetBasicDisplay(), WorldData().GetCurrentUserClub(), COL_ALIGNC);
				OnSelchangeDivisionList();
				break;
			case SCREEN08_CONTRACTVIEW:
				m_WhichEvents = m_dTotalEmpWages = m_dTotalYouthWages = m_dTotalPlayerWages = m_iPlayInfoNum = 0;
				m_SortOrder = UNSORTED;
				m_WageMultiplier = 52;
				WorldData().SetCurrentClub(WorldData().GetCurrentUserClub());
				DoBuildGridScreen08();
				break;
			case SCREEN22_BANKVIEW:
				pClub = WorldData().GetCurrentUserClub();
				EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
				m_TextSelector[eBORROW].DoInitialise(BUTTON1Y22, IDS_BORROW, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
				m_TextSelector[eBORROW].SetStyle(COL_ALIGNC | COL_MONEY);
				m_TextSelector[eREPAY].DoInitialise(BUTTON4Y22, IDS_REPAY, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
				m_TextSelector[eREPAY].SetStyle(COL_ALIGNC | COL_MONEY);
				m_CurrentTitledTextSelector.OnInitialise(&m_TextSelector[eBORROW], eMAXSELECTOR22);

				DoUpdateLoanDetails();
				break;
			case SCREEN39_JOBCENTREVIEW:
				m_TypeList.DoInitialise(this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
				m_TypeList.DoAddItem(IDS_CLUBEMPLOYEES);
				m_TypeList.DoAddItem(IDS_AVAILEMPLOYEES);
				DoBuildGridScreen39();
				break;
			case SCREEN41_YOUTHSQUADVIEW:
				m_YouthGrid.OnInitialUpdate(this, GetBasicDisplay());
				m_TypeList.DoInitialise(this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
				m_TypeList.DoAddItem(IDS_CLUBYOUTHS);
				m_TypeList.DoAddItem(IDS_AVAILYOUTHS);
				DoBuildGridScreen41();
				break;
			case SCREEN36_TRAININGVIEW:
				{
					m_PercentSelector[0].DoInitialise(BUTTON1Y36, IDS_STRING3601T, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
					m_PercentSelector[1].DoInitialise(BUTTON2Y36, IDS_STRING3604T, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
					m_PercentSelector[2].DoInitialise(BUTTON3Y36, IDS_STRING3603T, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
					m_PercentSelector[3].DoInitialise(BUTTON4Y36, IDS_STRING3602T, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
					m_FormationSelector.DoInitialise(BUTTON5Y36, IDS_TACTICFORMATION, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
					m_CurrentPercentSelector.OnInitialise(&m_PercentSelector[0], NUM_PERCENT_SELECTORS36 + 1, NUM_PERCENT_SELECTORS36);

					CSquadTraining& rTraining = WorldData().GetCurrentUserClub()->GetFirstTeamSquadTrainingDetails();
					m_PercentSelector[0].SetCurValue(rTraining.Get5aSidePercent());
					m_PercentSelector[1].SetCurValue(rTraining.GetTactics().GetPercentage());
					m_PercentSelector[2].SetCurValue(rTraining.GetSetPiece().GetPercentage());
					m_PercentSelector[3].SetCurValue(rTraining.GetRestingPercent());
					m_FormationSelector.SetCurValue(rTraining.GetTactics().GetSelection());
					WorldData().GetCurrentUser()->GetTacticsList().DoBuildTitleAndFamiliarityStrList(m_FormationSelector);
					m_FormationSelector.SetCurSel(WorldData().GetCurrentUser()->GetTacticsList().GetTacticsInUse());
				}
				break;
			case SCREEN02_CUPSCHEDULEVIEW:
				CBitmapFormView::GetGrid().DoAddStyle(LC_SHOWCOLUMNS);
				DoBuildGridScreen02();
				break;
			case SCREEN09_CLUBRECORDSVIEW:
				m_CurrentSelection = WorldData().CurrentUserClubID();
				DoBuildGridScreen09();
				break;
			case SCREEN19_LEAGUEFIXTURESVIEW:
				DoAddLRButtons();
				m_TheDate.DoInitialise(WorldData().GetCurrentDate());
				m_DateText.DoInitialise(GetBasicDisplay(), _T(""), COL_ALIGNC);
				OnSelchangeDivisionList();
				break;
			case SCREEN20_CUPFIXTURESVIEW:
				DoAddLRButtons();
				OnSelchangeCupListScreen20();
				break;
			case SCREEN27_MATCHTACTICSVIEW:
				m_TacticsData.OnInitialUpdate(this, &theApp.GetBasicDisplay());
				break;
			case SCREEN27_MATCHSPECIALPLAYERSVIEW:
				{
					CPlayerSquad& pSquad = WorldData().GetCurrentUserClub()->GetFirstTeamSquad();
					m_Captain.DoInitialise((pSquad.GetCaptainSquadNum() != NOPLAYER) ? pSquad.GetCaptainSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
					m_PenaltyTaker.DoInitialise((pSquad.GetPenaltyTakerSquadNum() != NOPLAYER) ? pSquad.GetPenaltyTakerSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
					m_CornerTaker.DoInitialise((pSquad.GetCornerTakerSquadNum() != NOPLAYER) ? pSquad.GetCornerTakerSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
					m_FreeKickTaker.DoInitialise((pSquad.GetFreeKickTakerSquadNum() != NOPLAYER) ? pSquad.GetFreeKickTakerSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
				}
				break;
			case SCREEN27_MATCHFORMATIONVIEW:
				m_iPlayerSelected = NOPLAYER;
				m_FormationSelector27.DoInitialise(this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
				m_FormationSelector27.DoRemoveAllData();
				WorldData().GetCurrentUser()->GetTacticsList().DoBuildTitleAndFamiliarityStrList(m_FormationSelector27);
				m_FormationSelector27.SetCurSel(WorldData().GetCurrentUser()->GetTacticsList().GetTacticsInUse());
				break;
			case SCREEN33_LEADINGGOALSCORERSVIEW:
				CBitmapFormView::GetGrid().SetRowHeight(20);
				m_ViewMode = 0;
				break;
			case SCREEN46_MONTHLYAWARDSVIEW:
				CBitmapFormView::GetGrid().HideHilite();
				OnSelchangeDivisionList();
				break;
			default:
				EXT_ASSERT(false);
		}
		CBitmapFormView::OnInitialiseView(HelpTopic[m_CurrentView], ScreenHeaderID[m_CurrentView]);
		OnSize(0xff, 0, 0);
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoAddLRButtons
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoAddLRButtons()
{
	uint theButtons[] =
	{
		IDR_PNGLEFT, IDR_PNGRIGHT,
		NOTFOUND
	};
	GetBasicDisplay()->DoAddButtons(&theButtons[0], this);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoAdjustColumnWidthsScreen02
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoAdjustColumnWidthsScreen02()
{
	CBitmapFormView::GetGrid().SetColumnWidthPercentage(0, 40);		
	CBitmapFormView::GetGrid().SetColumnWidthPercentage(1, 60);							
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoAdjustColumnWidthsScreen19
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoAdjustColumnWidthsScreen19()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	if (TheGrid.CountColumns() == 3)	// All columns have been created
	{
		int Width = TheGrid.GetWidth();
		TheGrid.SetColumnWidth(0, (Width / 7) * 3);		// Home team
		TheGrid.SetColumnWidth(1, Width / 7);			// Result
		TheGrid.SetColumnWidth(2, (Width / 7) * 3);		// Away team
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoAdjustColumnWidthsScreen33
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoAdjustColumnWidthsScreen33()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	if (TheGrid.CountRows() > 0)
	{
		int ColumnWidths[2][6] =
		{
			{ 18, 76, 80, 18, 14, 18},
			{ 18, 76, 80, 18, 14, 18}
		};
		int ColumnWidthsToUse = 0;
		int GridWidth = 118;
		if (GetCentreX() > 140)
		{
			GridWidth = 140;
			ColumnWidthsToUse = 1;
		}
		int RowsToDisplay = (GetWindowHeight() - TOPY33 - 2) / TheGrid.GetRowHeight();
		CRect theRect(GetCentreX() - GridWidth, TOPY33, GetCentreX() + GridWidth - TheGrid.GetScrollBarWidth(), TOPY33 + RowsToDisplay * TheGrid.GetRowHeight());
		TheGrid.MoveWindow(&theRect);
		for (int LoopCount = 0; LoopCount < 5; LoopCount++)
		{
			TheGrid.SetColumnWidth(LoopCount, ColumnWidths[ColumnWidthsToUse][LoopCount]);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoBuildPlayerList
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoBuildPlayerList()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	WorldData().GetCurrentUser()->GetPlayerSearch().DoEmpty();
	m_DivisionSelector.GetCurrentDivision().GetAllTransferListed(WorldData().GetCurrentUserClub(), WorldData().GetCurrentUser()->GetPlayerSearch());
	if (WorldData().GetCurrentUser()->GetPlayerSearch().GetIncludeFlag(CPlayerSearch::INCLUDEOUTOFCONTRACT) == true)
	{
		WorldData().GetOutOfContractPlayerList().DoBuildSuitableList(WorldData().GetCurrentUser()->GetPlayerSearch());
	}

	// Clear List
	TheGrid.RemoveAllData();
	TheGrid.DoAddColumn(24, COL_BITMAP);
	TheGrid.DoAddColumn(86);
	TheGrid.DoAddColumn(26, COL_NUMERICR);
	TheGrid.DoAddColumn(80, COL_MONEY);

	CPlayer* pPlayer;
	for (int LoopCount = 0; LoopCount < WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList().GetSize(); LoopCount++)
	{
		pPlayer = WorldData().GetPlayer(WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList()[LoopCount]);
		TheGrid.DoAddRow();
		TheGrid.SetRowPointerDetails(LoopCount, pPlayer, PLAYERCONTRACT);

		TheGrid.SetItemBitmap(LoopCount, 0, &m_pngSmallKits, pPlayer->GetClub().GetHomeKitID());
		TheGrid.SetItemText(LoopCount, 1, pPlayer->GetName());
		TheGrid.SetItemTextColour(LoopCount, 1, PosnColour[pPlayer->GetSkills().GetPosition()]);

		TheGrid.SetItemNum(LoopCount, 2, pPlayer->GetSkills().GetOverallSkill());
		TheGrid.SetItemTextColour(LoopCount, 2, PosnColour[pPlayer->GetSkills().GetPosition()]);

		TheGrid.SetItemText(LoopCount, 3, WorldData().GetCurrentUserClub()->GetForeignStr(WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerValueList()[LoopCount]));
		TheGrid.SetItemTextColour(LoopCount, 3, PosnColour[pPlayer->GetSkills().GetPosition()]);
	}
	TheGrid.ResetScrollBar();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoBuildGridScreen02
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoBuildGridScreen02()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	TheGrid.RemoveAllData(false);
	TheGrid.DoAddColumn(40, COL_TEXT | COL_ALIGNR);		
	TheGrid.DoAddColumn(60);							
	CCupDisplayEventList	CupEventList;
	m_CupSelector.GetCurrentCup().DoBuildEventsStrList(CupEventList);
	TheGrid.DoAddRows(CupEventList.GetSize());
	TheGrid.DoInitialiseScrollBar(true);

	CCupRound* pCupRound;

	for (int LoopCount = 0; LoopCount < CupEventList.GetSize(); LoopCount++)
	{
		pCupRound = CupEventList[LoopCount].GetCupRound();
		switch (CupEventList[LoopCount].GetType())
		{
		case DRAWDATE:
			TheGrid.SetRowTextColour(LoopCount, RED);
			TheGrid.SetItemText(LoopCount, 0, pCupRound->LongDrawDateStr());
			break;
		case MATCHDATE:
			TheGrid.SetRowTextColour(LoopCount, BLACK);
			TheGrid.SetItemText(LoopCount, 0, pCupRound->LongMatchDateStr());
			break;
		case REPLAYDATE:
			TheGrid.SetRowTextColour(LoopCount, BLUE);
			TheGrid.SetItemText(LoopCount, 0, pCupRound->LongReplayDateStr());
			break;
		default:
			EXT_ASSERT(false);
			break;
		}
		TheGrid.SetItemText(LoopCount, 1, CupEventList[LoopCount].TitleStr());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoBuildGridScreen08
	Access:    	protected 
	Parameter: 	bool bPreservePosition
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoBuildGridScreen08(bool bPreservePosition /*= false*/)
{
	CPlayerList ThePlayers;
	CPlayer* pPlayer;
	CYouth* pYouth;
	CClub& theClub = *WorldData().GetCurrentUserClub();
	CCountry& rCountry = theClub.GetCountry();
	CString strString;
	int LoopCount;

	CGrid& TheGrid = CBitmapFormView::GetGrid();
	int CurrentPosition = 0;
	if (bPreservePosition == true)
	{
		CurrentPosition = TheGrid.GetCurrentRow();
	}
	// Clear List
	TheGrid.RemoveAllData();
	TheGrid.DoAddColumn(90);
	TheGrid.DoAddColumn(60);
	if (m_WhichEvents == 0)
	{
		TheGrid.DoAddColumn(64, COL_MONEY | COL_ALIGNR);
	}
	else
	{
		TheGrid.DoAddColumn(64, COL_DATE | COL_ALIGNL);
	}

	int CurrRow = 0;
	if (theClub.GetManagerID() != NOMANAGER)
	{
		TheGrid.DoAddRow();

		// Add Manager
		TheGrid.SetRowPointerDetails(CurrRow, &theClub.GetManager(), MANAGERCONTRACT);
		TheGrid.SetItemText(CurrRow, 0, theClub.GetManager().GetName());
		TheGrid.SetItemStrId(CurrRow, 1, IDS_MANAGER);
		switch(m_WhichEvents)
		{
			case 0:
				TheGrid.SetItemMoney(CurrRow, 2, theClub.GetForeignStr(theClub.GetManager().GetWage() * m_WageMultiplier), theClub.GetManager().GetWage() * m_WageMultiplier);
				break;
			case 1:
				TheGrid.SetItemDate(CurrRow, 2, (CDate&)theClub.GetManager().GetContractEndDate());
				break;
		}
		CurrRow++;
	}

	m_dTotalEmpWages = 0;
	if (theClub.TotalStaff () > 0)
	{
		// Add Employees  -  See Job Centre
		for (LoopCount = 0 ; LoopCount < theClub.TotalStaff(); LoopCount++)
		{
			CEmployee* pEmployee = (CEmployee*)theClub.GetEmployee(LoopCount);
			m_dTotalEmpWages +=  pEmployee->GetWage() * m_WageMultiplier;		// Calculate Total Employee Wages

			TheGrid.DoAddRow();
			TheGrid.SetRowPointerDetails(CurrRow, pEmployee, EMPLOYEECONTRACT);
			TheGrid.SetItemText(CurrRow, 0, pEmployee->GetName());
			TheGrid.SetItemText(CurrRow, 1, pEmployee->TypeTextStr());
			switch(m_WhichEvents)
			{
				case 0:
					TheGrid.SetItemMoney(CurrRow, 2, theClub.GetForeignStr(pEmployee->GetWage() * m_WageMultiplier), pEmployee->GetWage() * m_WageMultiplier);
					break;
				case 1:
					TheGrid.SetItemDate(CurrRow, 2, (CDate&)pEmployee->GetContractEndDate());
					break;
			}
			CurrRow++;
		}
	}

	// Add Players
	ThePlayers.DoBuildFromSquad(theClub.GetFirstTeamSquad(), WorldData().GetPlayerList());
	m_dTotalPlayerWages = 0;
	for (LoopCount = 0; LoopCount < ThePlayers.GetSize(); LoopCount++)
	{
		pPlayer = static_cast<CPlayer*> (ThePlayers.GetAt(LoopCount));
		// If is a player on loan then dont display
		if (pPlayer->GetWeeksOnLoan() != 0)
		{
			continue;
		}
		TheGrid.DoAddRow();
		TheGrid.SetRowPointerDetails(CurrRow, pPlayer, PLAYERCONTRACT);

		// Calculate total player wages
		m_dTotalPlayerWages +=  pPlayer->GetWage() * m_WageMultiplier;
		TheGrid.SetItemText(CurrRow, 0, pPlayer->GetName());
		TheGrid.SetItemTextColour(CurrRow, 0, PosnColour[pPlayer->GetSkills().GetPosition()]);
		TheGrid.SetItemStrId(CurrRow, 1, pPlayer->GetSkills().PosnLongAbbr());
		TheGrid.SetItemTextColour(CurrRow, 1, PosnColour[pPlayer->GetSkills().GetPosition()]);
		switch(m_WhichEvents)
		{
			case 0:
				TheGrid.SetItemMoney(CurrRow, 2, theClub.GetForeignStr(pPlayer->GetWage() * m_WageMultiplier), pPlayer->GetWage() * m_WageMultiplier);
				break;
			case 1:
				TheGrid.SetItemDate(CurrRow, 2, (CDate&)pPlayer->GetContractEndDate());
				break;
		}
		CurrRow++;
	}

	// Add Youth Header
	m_dTotalYouthWages = 0;
	if (theClub.TotalYouths() > 0)
	{
		strString.Format(IDS_YOUTHS, theClub.TotalYouths());

		// Add Youths
		for (LoopCount = 0; LoopCount < theClub.TotalYouths(); LoopCount++) 
		{
			pYouth = theClub.GetYouth(LoopCount);
			TheGrid.DoAddRow();
			TheGrid.SetRowPointerDetails(CurrRow, pYouth, YOUTHCONTRACT);

			TheGrid.SetItemText(CurrRow, 0, pYouth->GetName());
			TheGrid.SetItemTextColour(CurrRow, 0, PosnColour[pYouth->GetSkills().GetPosition()]);
			TheGrid.SetItemStrId(CurrRow, 1, pYouth->GetSkills().PosnLongAbbr());
			TheGrid.SetItemTextColour(CurrRow, 1, PosnColour[pYouth->GetSkills().GetPosition()]);
			switch(m_WhichEvents)
			{
				case 0:
					TheGrid.SetItemMoney(CurrRow, 2, theClub.GetForeignStr(pYouth->GetWage() * m_WageMultiplier), pYouth->GetWage() * m_WageMultiplier);
					break;
				case 1:
					TheGrid.SetItemDate(CurrRow, 2, (CDate&)pYouth->GetContractEndDate());
					break;
			}
			CurrRow++;
		}
	}
	TheGrid.ResetScrollBar();
	TheGrid.SetCurrentRow(CurrentPosition);
	DoSort();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoBuildGridScreen09
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoBuildGridScreen09()
{
	CClub* pClub = &WorldData().GetClub(m_CurrentSelection);
	CClubHistory& rClubHistory = pClub->GetClubHistory();

	CGrid& TheGrid = CBitmapFormView::GetGrid();
	TheGrid.RemoveAllData(true);
	TheGrid.DoAddColumn(-1, COL_ALIGNC);

	TheGrid.DoAddRows(36);
	int Count = 0;
	TheGrid.SetItemStrId(Count++, 0, IDS_YEARFOUNDED);
	TheGrid.SetItemText(Count++, 0, pClub->GetYearFoundedStr());
	TheGrid.SetItemStrId(Count++, 0, IDS_MANAGER);
	TheGrid.SetItemText(Count++, 0, pClub->GetManagerName());
	TheGrid.SetItemStrId(Count++, 0, IDS_STADIUM);
	TheGrid.SetItemText(Count++, 0, pClub->GetStadium()->GetName());
	TheGrid.SetItemStrId(Count++, 0, IDS_CAPACITY);
	TheGrid.SetItemText(Count++, 0, pClub->GetStadium()->GetCapacityStr());
	TheGrid.SetItemStrId(Count++, 0, IDS_AVATTEND);
	TheGrid.SetItemText(Count++, 0, pClub->GetSeasonStats().GetAverageAttendanceStr());
	TheGrid.SetItemStrId(Count++, 0, IDS_RECORDSALE);
	TheGrid.SetItemText(Count++, 0, rClubHistory.RecordSaleInfoStr(pClub->GetCountry()));
	TheGrid.SetItemStrId(Count++, 0, IDS_RECORDBUY);
	TheGrid.SetItemText(Count++, 0, rClubHistory.RecordBuyInfoStr(pClub->GetCountry()));
	TheGrid.SetItemStrId(Count++, 0, IDS_RECORDAPPEARANCES);
	TheGrid.SetItemText(Count++, 0, rClubHistory.RecordAppearanceInfoStr());
	TheGrid.SetItemStrId(Count++, 0, IDS_RECORDCLUBGOALS);
	TheGrid.SetItemText(Count++, 0, rClubHistory.RecordGoalsInfoStr());
	TheGrid.SetItemStrId(Count++, 0, IDS_RECORDSEASONGOALS);
	TheGrid.SetItemText(Count++, 0, rClubHistory.RecordSeasonGoalsInfoStr());
	TheGrid.SetItemStrId(Count++, 0, IDS_RECORDVICTORY);
	TheGrid.SetItemText(Count++, 0, rClubHistory.GetRecordVictory()->DetailsStr());
	TheGrid.SetItemStrId(Count++, 0, IDS_RECORDDEFEAT);
	TheGrid.SetItemText(Count++, 0, rClubHistory.GetRecordDefeat()->DetailsStr());
	TheGrid.SetItemStrId(Count++, 0, IDS_RECORDATTENDANCE);
	TheGrid.SetItemText(Count++, 0, rClubHistory.RecordAttendanceInfoStr());
	TheGrid.SetItemStrId(Count++, 0, IDS_LOWESTATTENDANCE);
	TheGrid.SetItemText(Count++, 0, rClubHistory.LowestAttendanceInfoStr());
	TheGrid.SetItemStrId(Count++, 0, IDS_RECORDPOINTS);
	TheGrid.SetItemText(Count++, 0, rClubHistory.GetMostPoints()->DetailsStr(pClub->GetCountry()));
	TheGrid.SetItemStrId(Count++, 0, IDS_LEASTPOINTS);
	TheGrid.SetItemText(Count++, 0, rClubHistory.GetLeastPoints()->DetailsStr(pClub->GetCountry()));
	TheGrid.SetItemStrId(Count++, 0, IDS_RECORDPOSITION);
	TheGrid.SetItemText(Count++, 0, rClubHistory.GetHighestPosition()->DetailsStr(pClub->GetCountry()));
	TheGrid.SetItemStrId(Count++, 0, IDS_LOWESTPOSITION);
	TheGrid.SetItemText(Count++, 0, rClubHistory.GetLowestPosition()->DetailsStr(pClub->GetCountry()));
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoBuildGridScreen19
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoBuildGridScreen19()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	m_DateText.SetText(m_TheDate.LongDateStr());
	TheGrid.RemoveAllData(false);
	TheGrid.ResetScrollBar();
	m_ResultList.RemoveList();
	m_DivisionSelector.GetCurrentDivision().DoBuildLeagueFixtureList(m_TheDate, m_ResultList);
	if (m_ResultList.GetSize() > 0)
	{
		TheGrid.DoAddColumn(10, COL_TEXT | COL_ALIGNR);		// Home team
		TheGrid.DoAddColumn(10, COL_TEXT | COL_ALIGNC);		// Result
		TheGrid.DoAddColumn(10);								// Away team
		DoAdjustColumnWidthsScreen19();
		TheGrid.DoAddRows(m_ResultList.GetSize());

		CSoccerResult* pResult;
		for (int LoopCount = 0; LoopCount < m_ResultList.GetSize(); LoopCount++)
		{
			pResult = m_ResultList.GetAt(LoopCount);
			// Highlight the user's result
			if (pResult->GetHomeClubDetails().IsUserControlledClub() == true || 
				pResult->GetAwayClubDetails().IsUserControlledClub() == true)
			{
				TheGrid.SetRowTextColour(LoopCount, WHITE);
			}
			else 
			{
				TheGrid.SetRowTextColour(LoopCount, BLACK);
			}
			TheGrid.SetItemText(LoopCount, 0, pResult->GetHomeClubDetails().GetName());
			TheGrid.SetItemText(LoopCount, 1, pResult->GoalsStr());
			TheGrid.SetItemText(LoopCount, 2, pResult->GetAwayClubDetails().GetName());
		}
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoBuildGridScreen20
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoBuildGridScreen20()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	TheGrid.RemoveAllData(false);
	TheGrid.DoAddColumn(98, COL_TEXT | COL_ALIGNR);		// Home team
	TheGrid.DoAddColumn(34, COL_TEXT | COL_ALIGNC);		// Result and league position
	TheGrid.DoAddColumn(98);								// Away team

	m_ResultList.RemoveList();
	CCupRound& CurrentCupRound = GetCurrentCupRound();
	if (m_bShowReplays == true)
	{
		CurrentCupRound.BuildReplayDisplayList(m_ResultList);
	}
	else
	{
		CurrentCupRound.BuildMatchDisplayList(m_ResultList);
	}

	if (m_bShowReplays == true)
	{
		m_DateStr = CurrentCupRound.LongReplayDateStr();
	}
	else
	{
		m_DateStr = CurrentCupRound.LongMatchDateStr();
	}

	if (CurrentCupRound.IsGroupMatch() == false)
	{
		m_ResultList.SortByHomeClub();
	}
	TheGrid.DoAddRows(m_ResultList.GetSize());
	MoveGridScreen20();	// Scrollbar any or not be visible

	COLORREF TextCol;
	for (int LoopCount = 0; LoopCount < m_ResultList.GetSize(); LoopCount++)
	{
		if (m_ResultList.GetAt(LoopCount)->GetHomeClubDetails().IsUserControlledClub() == true || m_ResultList.GetAt(LoopCount)->GetAwayClubDetails().IsUserControlledClub() == true)
		{
			TextCol = BLUE;
		}
		else
		{
			TextCol = BLACK;
		}
		TheGrid.SetRowTextColour(LoopCount, TextCol);

		TheGrid.SetItemText(LoopCount, 0, m_ResultList.GetAt(LoopCount)->GetHomeClubDetails().GetName());
		TheGrid.SetItemText(LoopCount, 1, m_ResultList.GetAt(LoopCount)->GoalsStr());
		TheGrid.SetItemText(LoopCount, 2, m_ResultList.GetAt(LoopCount)->GetAwayClubDetails().GetName());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoBuildGridScreen26
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoBuildGridScreen26()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	TheGrid.RemoveAllData(true);
	TheGrid.DoAddColumn(112);				// Description
	TheGrid.DoAddColumn(62, COL_NUMERICR);	// Income
	TheGrid.DoAddColumn(62, COL_NUMERICR);	// Expenditure

	CClub* pClub = WorldData().GetCurrentUserClub();

	CAccountsGridItem theList[50];		// Ensure when adding more to accounts that this is sufficient
	int NumFound = 0;
	int Income = 0, Expenditure = 0;


	if (m_eWhichPeriod == WEEKLY_FINANCE)
	{
		if (pClub->GetMidWeekMatchGateReceipts() > 0)
		{
			ProfitOut(static_cast<int>(pClub->GetMidWeekMatchGateReceipts()), IDS_MIDGATERECIEPTS)
		}
		if (pClub->GetWeekEndMatchGateReceipts() > 0)
		{
			ProfitOut(static_cast<int>(pClub->GetWeekEndMatchGateReceipts()), IDS_GATERECIEPTS)
		}
	}
	else if (pClub->GetAccounts()->GetAt(CAccountsItemList::GATERECIEPTS).GetAmount(m_eWhichPeriod) > 0)
	{
		ProfitOut(static_cast<int>(pClub->GetAccounts()->GetAt(CAccountsItemList::GATERECIEPTS).GetAmount(m_eWhichPeriod)), IDS_GATERECIEPTS)
	}
	// 	DEBUG_OUTPUT(("Annual Gate Receipts %d, Ticket Price %d, Average Attendance %d"), (pClub->GetAccounts()->GetAt(CAccountsItemList::GATERECIEPTS).GetAmount(m_eWhichPeriod), pClub->GetTicketPrice(), pClub->DoCalculateAverageAttendance()));

	double dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::GENERALWAGE).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		ProfitOut(dAmount, IDS_MERCHANDISE)
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::SPONSORSHIPINCOME).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		ProfitOut(static_cast<int>(dAmount), IDS_SPONSORSHIP)
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::TELEVISIONINCOME).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		ProfitOut(static_cast<int>(dAmount), IDS_TELEVISION)
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::SECURITY).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		DebitOut(static_cast<int>(dAmount), IDS_SECURITYCOSTS)
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::GENERALWAGE).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		DebitOut(static_cast<int>(dAmount), IDS_GENERALWAGES)	// General wages
	}

	if (pClub->GetManagerID() != NOMANAGER)
	{
		DebitOut(static_cast<int>(pClub->GetAccounts()->GetAt(CAccountsItemList::MANAGERSWAGES).GetAmount(m_eWhichPeriod)), IDS_MANAGERSWAGES)   // Manager's wage
	}

	DebitOut(pClub->GetAccounts()->GetAt(CAccountsItemList::PLAYERWAGES).GetAmount(m_eWhichPeriod), IDS_PLAYERWAGES)
		DebitOut(pClub->GetAccounts()->GetAt(CAccountsItemList::EMPLOYEEWAGES).GetAmount(m_eWhichPeriod), IDS_EMPLOYEEWAGES)
		DebitOut(pClub->GetAccounts()->GetAt(CAccountsItemList::YOUTHWAGES).GetAmount(m_eWhichPeriod), IDS_YOUTHWAGES)
		DebitOut(pClub->GetAccounts()->GetAt(CAccountsItemList::RUNNINGCOSTS).GetAmount(m_eWhichPeriod), IDS_RUNNINGCOSTS)       // Running costs

		dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::LOANRECEIVED).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		ProfitOut(static_cast<int>(dAmount), IDS_LOANRECEIVED)     // Loan repayments
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::LOANPAID).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		DebitOut(static_cast<int>(dAmount), IDS_LOANREPAY)     // Loan repayments
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::TRANSFERINCOME).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		ProfitOut(static_cast<int>(dAmount), IDS_TRANSFERINCOME)    //Transfer income
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::TRANSFEREXPENDITURE).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		DebitOut(static_cast<int>(dAmount), IDS_TRANSFEREXPEND) //Transfer expenditure
	}

	dAmount = pClub->GetAccounts()->GetAt(CAccountsItemList::GENERALEXPENDITURE).GetAmount(m_eWhichPeriod);
	if (dAmount > 0)
	{
		DebitOut(static_cast<int>(dAmount), IDS_GENERALEXPEND)
	}

	TheGrid.DoAddRows(NumFound + 2);
	TheGrid.SetColumnTextColour(2, RED);
	CAccountsGridItem* pItem = &theList[0];
	for (int LoopCount = 0; LoopCount < NumFound; LoopCount++)
	{
		TheGrid.SetItemText(LoopCount, 0, pItem->m_Description);

		if (pItem->m_IncomeStr.IsEmpty() == false)
		{
			TheGrid.SetItemText(LoopCount, 1, pItem->m_IncomeStr);
		}
		if (pItem->m_ExpenditureStr.IsEmpty() == false)
		{
			TheGrid.SetItemText(LoopCount, 2, pItem->m_ExpenditureStr);
		}
		pItem++;
	}

	CExtString strPrint;
	// Totals
	TheGrid.SetItemStrId(NumFound, 0, IDS_TOTALS);

	strPrint.FormatLargeNumber(Income);
	TheGrid.SetItemText(NumFound, 1, strPrint);

	strPrint.FormatLargeNumber(static_cast<int>(Expenditure));
	TheGrid.SetItemText(NumFound, 2, strPrint);

	// Week's Profit or loss
	strPrint.FormatLargeNumber(abs(Income - Expenditure));
	if (Income - Expenditure > 0)
	{
		TheGrid.SetItemStrId(NumFound + 1, 0, IDS_PROFIT);
		TheGrid.SetItemText(NumFound + 1, 1, strPrint);
	}
	else
	{
		TheGrid.SetItemStrId(NumFound + 1, 0, IDS_LOSS);
		TheGrid.SetItemText(NumFound + 1, 2, strPrint);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoBuildGridScreen33
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoBuildGridScreen33()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	TheGrid.RemoveAllData(true);
	CClub* pClub;
	CPlayer* pPlayer;

	TheGrid.DoAddColumn(20, COL_BITMAP);
	TheGrid.DoAddColumn(88);	// Player Name
	TheGrid.DoAddColumn(88);	// Club Name
	TheGrid.DoAddColumn(18, COL_NUMERICR);	// Yellow / League Goals
	TheGrid.DoAddColumn(12, COL_NUMERICR);	// Red / Cup Goals
	TheGrid.DoAddColumn(18, COL_NUMERICR);	// Overall 
	switch(m_ViewMode)
	{
		case 0:
		{
			// Disciplinary
			CLeadingDiscipinaryPoints* pLeadingDiscPlayer;
			TheGrid.DoAddRows(m_PointScorers.GetSize());
			for (int LoopCount = 0; LoopCount < m_PointScorers.GetSize(); LoopCount++)
			{
				pLeadingDiscPlayer = static_cast<CLeadingDiscipinaryPoints*>(m_PointScorers.GetAt(LoopCount));
				pPlayer = WorldData().GetPlayer(pLeadingDiscPlayer->GetPlayerNum());
				pClub = &WorldData().GetClub(pLeadingDiscPlayer->GetClubNum());

				TheGrid.SetRowTextColour(LoopCount, PosnColour[pPlayer->GetSkills().GetPosition()]);
				TheGrid.SetItemBitmap(LoopCount, 0, &m_pngSmallKits, pPlayer->GetClub().GetHomeKitID());
				TheGrid.SetItemText(LoopCount, 1, pPlayer->GetName(), pPlayer);
				TheGrid.SetItemText(LoopCount, 2, pClub->GetName(), pClub);

				TheGrid.SetItemNum(LoopCount, 3, pLeadingDiscPlayer->GetYellow());
				TheGrid.SetItemNum(LoopCount, 4, pLeadingDiscPlayer->GetRed());
				TheGrid.SetItemNum(LoopCount, 5, pLeadingDiscPlayer->GetTotalScored());
			}
		}
		break;

		case 1:
		{
			CLeadingGoalScorer* pLeadingGoalScorer;
			TheGrid.DoAddRows(m_GoalScorers.GetSize());
			for (int LoopCount = 0; LoopCount < m_GoalScorers.GetSize(); LoopCount++)
			{
				pLeadingGoalScorer = static_cast<CLeadingGoalScorer*>(m_GoalScorers.GetAt(LoopCount));
				pPlayer = WorldData().GetPlayer(pLeadingGoalScorer->GetPlayerNum());
				pClub = &WorldData().GetClub(pLeadingGoalScorer->GetClubNum());

				TheGrid.SetRowTextColour(LoopCount, PosnColour[pPlayer->GetSkills().GetPosition()]);
				TheGrid.SetItemBitmap(LoopCount, 0, &m_pngSmallKits, pPlayer->GetClub().GetHomeKitID());
				TheGrid.SetItemText(LoopCount, 1, pPlayer->GetName(), pPlayer);
				TheGrid.SetItemText(LoopCount, 2, pClub->GetName(), pClub);

				TheGrid.SetItemNum(LoopCount, 3, pLeadingGoalScorer->GetLeagueScored());
				TheGrid.SetItemNum(LoopCount, 4, pLeadingGoalScorer->GetCupScored());
				TheGrid.SetItemNum(LoopCount, 5, pLeadingGoalScorer->GetTotalScored());
			}
		}
	}
	DoAdjustColumnWidthsScreen33();
	ReDraw();
}


void CScreen03View::DoBuildGridScreen39()
{
	if (m_TypeList.GetCurSel() == 0)
	{
		m_EmployeeGrid.DoBuildGrid(WorldData().GetCurrentUserClub()->GetEmployeeList());
	}
	else
	{
		m_EmployeeGrid.DoBuildGrid(WorldData().GetCurrentUserClub()->GetActiveCountry().GetAvailableEmployeeList());
	}
}


void CScreen03View::DoBuildGridScreen41()
{
	if (m_TypeList.GetCurSel() == 0)
	{
		m_YouthGrid.DoBuildGrid(WorldData().GetCurrentUserClub()->GetYouthList());
	}
	else
	{
		m_YouthGrid.DoBuildGrid(WorldData().GetCurrentUserClub()->GetActiveCountry().GetAvailableYouthList());
	}
}


void CScreen03View::DoBuildGridScreen46()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	TheGrid.RemoveAllData(true);

	TheGrid.DoAddColumn(30);	// Month
	TheGrid.DoAddColumn(95);	// Manager Name
	TheGrid.DoAddColumn(95);	// Club Name

	CDivision& pDivision = m_DivisionSelector.GetCurrentDivision();
	int HowMany = pDivision.DoCountMOMWinners();
	TheGrid.DoAddRows(HowMany);
	for (int LoopCount = 0; LoopCount < HowMany; LoopCount++)
	{
		CManagerOfTheMonth& pManager = pDivision.GetManagerOfMonth(LoopCount);
		TheGrid.SetItemText(LoopCount, 0, pManager.MonthStr());
		TheGrid.SetItemText(LoopCount, 1, pManager.ManagerStr());
		TheGrid.SetItemText(LoopCount, 2, pManager.ClubStr());
	}
	ReDraw();
}


void CScreen03View::DoBuildGridScreen48()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	WorldData().GetCurrentUser()->GetPlayerSearch().DoEmpty();
	m_DivisionSelector.GetCurrentDivision().GetAllTransferListed(WorldData().GetCurrentUserClub(), WorldData().GetCurrentUser()->GetPlayerSearch());
	if (WorldData().GetCurrentUser()->GetPlayerSearch().GetIncludeFlag(CPlayerSearch::INCLUDEOUTOFCONTRACT) == true)
	{
		WorldData().GetOutOfContractPlayerList().DoBuildSuitableList(WorldData().GetCurrentUser()->GetPlayerSearch());
	}

	// Clear List
	TheGrid.RemoveAllData();
	TheGrid.DoAddColumn(24, COL_BITMAP);
	TheGrid.DoAddColumn(86);
	TheGrid.DoAddColumn(26, COL_NUMERICR);
	TheGrid.DoAddColumn(80, COL_MONEY);

	CPlayer* pPlayer;
	for (int LoopCount = 0; LoopCount < WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList().GetSize(); LoopCount++)
	{
		pPlayer = WorldData().GetPlayer(WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList()[LoopCount]);
		TheGrid.DoAddRow();
		TheGrid.SetRowPointerDetails(LoopCount, pPlayer, PLAYERCONTRACT);

		TheGrid.SetItemBitmap(LoopCount, 0, &m_pngSmallKits, pPlayer->GetClub().GetHomeKitID());
		TheGrid.SetItemText(LoopCount, 1, pPlayer->GetName());
		TheGrid.SetItemTextColour(LoopCount, 1, PosnColour[pPlayer->GetSkills().GetPosition()]);

		TheGrid.SetItemNum(LoopCount, 2, pPlayer->GetSkills().GetOverallSkill());
		TheGrid.SetItemTextColour(LoopCount, 2, PosnColour[pPlayer->GetSkills().GetPosition()]);

		TheGrid.SetItemText(LoopCount, 3, WorldData().GetCurrentUserClub()->GetForeignStr(WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerValueList()[LoopCount]));
		TheGrid.SetItemTextColour(LoopCount, 3, PosnColour[pPlayer->GetSkills().GetPosition()]);
	}
	TheGrid.ResetScrollBar();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoBuildGridScreen50
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoBuildGridScreen50()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	TheGrid.RemoveAllData(true);

	if (m_ViewMode < 4)
	{
		// Overall, home, away and current form
		TheGrid.DoAddColumn(m_ClubNameLength);	// Club Name
		TheGrid.DoAddColumn(18, COL_NUMERICR);	// Played
		TheGrid.DoAddColumn(18, COL_NUMERICR);	// Won
		TheGrid.DoAddColumn(18, COL_NUMERICR);	// Drawn
		TheGrid.DoAddColumn(18, COL_NUMERICR);	// Lost
		TheGrid.DoAddColumn(26, COL_NUMERICR);	// For
		TheGrid.DoAddColumn(26, COL_NUMERICR);	// Against
		TheGrid.DoAddColumn(26, COL_NUMERICR);	// Points
	}
	else
	{
		// Disciplinary
		TheGrid.DoAddColumn(124);				// Club Name
		TheGrid.DoAddColumn(34, COL_NUMERICR);	// Red
		TheGrid.DoAddColumn(32, COL_NUMERICR);	// Yellow
		TheGrid.DoAddColumn(32, COL_NUMERICR);	// Overall

	}
	const COLORREF TextColor[] =
	{
		TRANSCOLOUR,	// Normal
		PALERED,		// Relegation spot
		PALEBLUE,		// Playoff spot
		PALEGREEN		// Promotion spot
	};

	CDivision& pDivision = m_DivisionSelector.GetCurrentDivision();
	TheGrid.DoAddRows(pDivision.GetNumberOfClubsInDivision());
	CClub* pClub;
	if (m_ViewMode < 3)
	{
		// Overall, home and away
		pDivision.SortLeagueTable(m_ViewMode);
		for (int LoopCount = 0; LoopCount < pDivision.GetNumberOfClubsInDivision(); LoopCount++)
		{
			pClub = &pDivision.DoFindClubInTablePosition(LoopCount);
			TheGrid.SetItemText(LoopCount, 0, pClub->GetName());
			TheGrid.SetRowBackgroundColour(LoopCount, TextColor[pDivision.LeaguePositionStatus(LoopCount)]);
			TheGrid.SetRowPointerDetails(LoopCount, pClub);

			switch(m_ViewMode)
			{
			case 0:
				/*#ifdef TEST_DIALOGS
				TheGrid.SetItemNum(LoopCount, 1, 99);
				TheGrid.SetItemNum(LoopCount, 2, 99);
				TheGrid.SetItemNum(LoopCount, 3, 99);
				TheGrid.SetItemNum(LoopCount, 4, 99);
				TheGrid.SetItemNum(LoopCount, 5, 199);
				TheGrid.SetItemNum(LoopCount, 6, 199);
				TheGrid.SetItemNum(LoopCount, 7, 199);
				#else*/
				pClub->DoBuildLeagueTableRow(LoopCount, TheGrid);
				//#endif
				break;
			case 1:
				TheGrid.SetItemNum(LoopCount, 1, pClub->GetTableData().GetHomeGames()->GetNumberGamesPlayed());
				TheGrid.SetItemNum(LoopCount, 2, pClub->GetTableData().GetHomeGames()->GetNumberGamesWon());
				TheGrid.SetItemNum(LoopCount, 3, pClub->GetTableData().GetHomeGames()->GetNumberGamesDrawn());
				TheGrid.SetItemNum(LoopCount, 4, pClub->GetTableData().GetHomeGames()->GetNumberGamesLost());
				TheGrid.SetItemNum(LoopCount, 5, pClub->GetTableData().GetHomeGames()->GetNumberGoalsScored());
				TheGrid.SetItemNum(LoopCount, 6, pClub->GetTableData().GetHomeGames()->GetNumberGoalsConceded());
				TheGrid.SetItemNum(LoopCount, 7, pClub->GetTableData().GetHomeGames()->GetPoints());
				break;

			case 2:
				TheGrid.SetItemNum(LoopCount, 1, pClub->GetTableData().GetAwayGames()->GetNumberGamesPlayed());
				TheGrid.SetItemNum(LoopCount, 2, pClub->GetTableData().GetAwayGames()->GetNumberGamesWon());
				TheGrid.SetItemNum(LoopCount, 3, pClub->GetTableData().GetAwayGames()->GetNumberGamesDrawn());
				TheGrid.SetItemNum(LoopCount, 4, pClub->GetTableData().GetAwayGames()->GetNumberGamesLost());
				TheGrid.SetItemNum(LoopCount, 5, pClub->GetTableData().GetAwayGames()->GetNumberGoalsScored());
				TheGrid.SetItemNum(LoopCount, 6, pClub->GetTableData().GetAwayGames()->GetNumberGoalsConceded());
				TheGrid.SetItemNum(LoopCount, 7, pClub->GetTableData().GetAwayGames()->GetPoints());
				break;
			}
		}
	}
	else
	{
		int iGamesPlayed = CCurrentForm::NUMFORMMATCHES;
		switch(m_ViewMode)
		{
		case 3:
			// Current form
			if (pDivision.FindClubNumberInCurrentFormPosition(0).GetTableData().GetNumberGamesPlayed() < CCurrentForm::NUMFORMMATCHES)
			{
				iGamesPlayed = pDivision.FindClubNumberInCurrentFormPosition(0).GetTableData().GetNumberGamesPlayed();
			}
			for (int LoopCount = 0; LoopCount < TheGrid.GetNumRowsOnScreen(); LoopCount++)
			{
				pClub = &pDivision.FindClubNumberInCurrentFormPosition(LoopCount + TheGrid.GetVerticalScroll());
				TheGrid.SetItemText(LoopCount, 0, pClub->GetName());

				TheGrid.SetItemNum(LoopCount, 1, iGamesPlayed);
				TheGrid.SetItemNum(LoopCount, 2, pClub->GetCurrentForm()->GetCurrentFormWon());
				TheGrid.SetItemNum(LoopCount, 3, pClub->GetCurrentForm()->GetCurrentFormDrawn());
				TheGrid.SetItemNum(LoopCount, 4, pClub->GetCurrentForm()->GetCurrentFormLost());
				TheGrid.SetItemNum(LoopCount, 5, pClub->GetCurrentForm()->GetCurrentFormScored());
				TheGrid.SetItemNum(LoopCount, 6, pClub->GetCurrentForm()->GetCurrentFormConceded());
				TheGrid.SetItemNum(LoopCount, 7, pClub->GetCurrentForm()->GetCurrentFormPts());
			}
			break;
		case 4:
			for (int LoopCount = 0; LoopCount < TheGrid.GetNumRowsOnScreen(); LoopCount++)
			{
				pClub = &WorldData().GetClub(m_FairPlayList.GetAt(LoopCount + TheGrid.GetVerticalScroll())->GetIndex());
				TheGrid.SetItemText(LoopCount, 0, pClub->GetName());

				TheGrid.SetItemNum(LoopCount, 1, pClub->GetSeasonStats().GetSeasonBookings());
				TheGrid.SetItemNum(LoopCount, 2, pClub->GetSeasonStats().GetSeasonSentOff ());
				TheGrid.SetItemNum(LoopCount, 3, pClub->GetSeasonStats().GetSeasonDisciplinaryPoints());
			}
			break;
		}
	}
	CBitmapFormView::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoResetProgress
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoResetProgress()
{
	m_ProgressBar.DoReset(12);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoSaveTeamTraining
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoSaveTeamTraining()
{
	CSquadTraining&  rTraining = WorldData().GetCurrentUserClub()->GetFirstTeamSquadTrainingDetails();
	rTraining.Set5aSidePercent(m_PercentSelector[0].GetCurValue());
	rTraining.GetTactics().SetPercentage(m_PercentSelector[3].GetCurValue());
	rTraining.GetSetPiece().SetPercentage(m_PercentSelector[2].GetCurValue());
	rTraining.GetSetPiece().SetPercentage(m_PercentSelector[1].GetCurValue());
	rTraining.GetSetPiece().SetSelection(m_FormationSelector.GetCurSel());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoSort
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoSort()
{
	CBitmapFormView::GetGrid().DoSort(2, m_SortOrder);
	//ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoUpdateData
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoUpdateData()
{
	CPlayerSquad& pSquad = WorldData().GetCurrentUserClub()->GetFirstTeamSquad();
	pSquad.SetCaptainSquadNum(m_Captain.GetCurSel());
	pSquad.SetPenaltyTakerSquadNum(m_PenaltyTaker.GetCurSel());
	pSquad.SetCornerTakerSquadNum(m_CornerTaker.GetCurSel());
	pSquad.SetFreeKickTakerSquadNum(m_FreeKickTaker.GetCurSel());

	CBitmapFormView::ReDraw();
}



/*------------------------------------------------------------------------------
	Method:   	CScreen03View::DoUpdateLoanDetails
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::DoUpdateLoanDetails()
{

	CClub* pClub = WorldData().GetCurrentUserClub();
	EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);

	m_TextSelector[eREPAY].SetCurValue(pClub->GetRecommendedLoanRepayment());
	m_TextSelector[eBORROW].SetCurValue(pClub->GetFinancialLoan()->GetRecommendedLoanBorrow(&pClub->GetPopularity()));

	m_InvisibleGrid.SetItemStrId(0, 0, IDS_LOANOUTSTANDING);
	m_InvisibleGrid.SetItemStrId(1, 0, IDS_WEEKLYREPAYMENTS);
	m_InvisibleGrid.SetItemStrId(2, 0, IDS_WEEKSREMAINING);
	m_InvisibleGrid.SetItemText(0, 1, pClub->GetFinancialLoan()->LoanOwedStr(pClub->GetCountry()));
	m_InvisibleGrid.SetItemText(1, 1, pClub->GetFinancialLoan()->LoanRepaymentStr(pClub->GetCountry()));
	m_InvisibleGrid.SetItemNum(2, 1, pClub->GetFinancialLoan()->WeeksToRepayLoan(pClub->GetCountry()));
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::CScreen03View
	Access:    	public 
	Returns:   		
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen03View::CScreen03View()
	: CBitmapFormView(IDD)
{
	m_CurrentView = NO_VIEW;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::~CScreen03View
	Access:    	public 
	Returns:   
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen03View::~CScreen03View()
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::GetCurrentCupRound
	Access:    	protected 
	Returns:   	CCupRound&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound& CScreen03View::GetCurrentCupRound()
{
	return m_CupSelector.GetCurrentCup().GetRound(m_iCurrentRound);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::MoveGridScreen20
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::MoveGridScreen20()
{
		CRect theRect;
		GetClientRect(&theRect);
		int CentreX = theRect.right / 2;

		// Cup Name
		m_RoundY = TOPY + 14;
		m_CupSelector.MoveWindow(CRect(CentreX - COMPETITIONNAMELENGTH, m_RoundY, CentreX + COMPETITIONNAMELENGTH, m_RoundY + TABHEIGHT));
		// Round
		m_RoundY += eUPDOWNHEIGHT + 2;
		int Button2X = CentreX + COMPETITIONNAMELENGTH;
		int Button1X = CentreX - COMPETITIONNAMELENGTH - eUPDOWNWIDTH;
		GetBasicDisplay()->DoMoveButton(eROUNDDOWN20, CRect(Button1X, m_RoundY, Button1X + eUPDOWNWIDTH, m_RoundY + eUPDOWNHEIGHT));
		GetBasicDisplay()->DoMoveButton(eROUNDUP20, CRect(Button2X, m_RoundY, Button2X + eUPDOWNWIDTH, m_RoundY + eUPDOWNHEIGHT));

		int GridBottom = (theRect.bottom - TOPY20 - 2);
		int RowsToDisplay = GridBottom / CBitmapFormView::GetGrid().GetRowHeight();
		GridBottom = TOPY20 + RowsToDisplay * CBitmapFormView::GetGrid().GetRowHeight();
		CBitmapFormView::GetGrid().MoveWindow(CRect(CentreX - GRID_WIDTH20 / 2, TOPY20, CentreX + GRID_WIDTH20 / 2 - CBitmapFormView::GetGrid().GetScrollBarWidth(), GridBottom));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnClickAdjustPercentage
	Access:    	protected 
	Parameter: 	const UINT _Which
	Parameter: 	const bool _bInc
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnClickAdjustPercentage(const UINT _Which, const bool _bInc)
{
	int nLockedPercentage = 0;
	int nAmountToAdjust = 6;

	int nMax = 100 - nLockedPercentage;
	// get max/min amount that can be added/subbed from adjusted value
	if (_bInc == true)
	{
		if (m_PercentSelector[_Which].GetCurValue() + nAmountToAdjust > nMax)
		{
			nAmountToAdjust = nMax - m_PercentSelector[_Which].GetCurValue();
		}
		m_PercentSelector[_Which].OnIncreaseValue(nAmountToAdjust);
	}
	else
	{
		if (m_PercentSelector[_Which].GetCurValue() - nAmountToAdjust < 0)
		{
			nAmountToAdjust = m_PercentSelector[_Which].GetCurValue();
		}
		m_PercentSelector[_Which].OnDecreaseValue(nAmountToAdjust);
	}

	// do {nAmountToAdjust} passes to add/sub to adjusted control
	bool bChanged;
	while (nAmountToAdjust)
	{
		// try to add/remove a value from a different no locked control
		bChanged = false;
		for (int nCount = 0; nCount < NUM_PERCENT_SELECTORS36; nCount++)
		{
			// if control not locked or adjusted one
			if (_Which != nCount)
			{
				// if adding to adjusted the subtract the others
				if (_bInc == true)
				{
					// if any left todo then sub from control
					if (nAmountToAdjust)
					{
						if (m_PercentSelector[nCount].GetCurValue() > 0)
						{
							m_PercentSelector[nCount].OnDecValue();
							nAmountToAdjust--;
							bChanged = true;
						}
					}
				}		// if less than adjusted one then increase
				else
				{
					// if any left todo then add to control
					if (nAmountToAdjust)
					{
						if (m_PercentSelector[nCount].GetCurValue() < 100)
						{
							m_PercentSelector[nCount].OnIncValue();
							nAmountToAdjust--;
							bChanged = true;
						}
					}
				}
			}
		}
		if (bChanged == false)
		{
			nAmountToAdjust = 0;
		}
	}

	// Save the values
	DoSaveTeamTraining();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnIncrementProgress
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnIncrementProgress()
{
	m_ProgressBar.DoStepIt();
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnInitialUpdate
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnInitialUpdate()
{
//	CFootballDirectorDoc* pApp = (CFootballDirectorDoc*)GetDocument();
	CBitmapFormView::OnInitialUpdate(IDS_HELPSCREEN03, &theApp.GetBasicDisplay(), IDS_SCREEN03);

	HDC dc = ::GetDC(HWND());
	m_pngLargeKits.DoInitialise(dc, IDR_PNGLARGEHOMEKITS, MAXKITS);
	m_pngSmallKits.DoInitialise(dc, IDR_PNGSMALLHOMEKITS, MAXKITS);
	m_pngPlayerStatus.DoInitialise(dc, IDR_PNGPLAYERSTATUS, MAXPLAYERSTATUSES);
	m_pngRoles.DoInitialise(dc, IDR_PNGROLES, 4);
	m_pngNumbers.DoInitialise(dc, IDR_PNGNUMBERS, PLAYERSINTEAM * 3);
	m_Pitch.SetBitmap(dc, IDR_PNGPITCH, _T("PNG"));
	m_pngProgressBar.SetBitmap(dc, IDR_PNGPROGRESSBAR, _T("PNG"));
	::ReleaseDC(HWND(), dc);
	m_ProgressBar.SetDrawInfo(&m_pngProgressBar);
	m_EmployeeGrid.OnInitialUpdate(this, GetBasicDisplay());
	m_YouthGrid.OnInitialUpdate(this, GetBasicDisplay());

	m_InvisibleGrid.OnInitialUpdate(this, GetBasicDisplay());
	m_InvisibleGrid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
	m_InvisibleGrid.DoAddColumn(GetCentreX(), COL_ALIGNR);
	m_InvisibleGrid.DoAddColumn(GetCentreX(), COL_ALIGNL);
	m_InvisibleGrid.DoAddRows(3);

	// Setup message display
	m_MessageDisplay.SetDrawInfo(this, 0x1000, CRect(2, 44, 240, 242), BLACK, TRUE);
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnDraw
	Access:    	public 
	Parameter: 	CDC* _pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
	------------------------------------------------------------------------------*/
void CScreen03View::OnDraw(CDC* _pDC)
{
	if (theApp.IsInitialised() == false || m_CurrentView == NO_VIEW)
	{
		return;
	}

	CBitmapFormView::OnInitRedrawArea();
	COffScreenBuffer& memDC = *CBitmapFormView::GetMemDC();
	CRect DataRect;
	CString str;
	int GapY = 18;
	eFONTSIZE FontSize = BIGFONT;
	COLORREF TextColour = BLACK;
	CMatchInfo Match;
	switch (m_CurrentView)
	{
		case SCREEN03_STATUSVIEW:
			{
				DataRect.SetRect(4, TOPY, GetBasicDisplay()->GetOffScreenBuffer()->GetBufferWidth(), GetBasicDisplay()->GetOffScreenBuffer()->GetBufferHeight());
				if (DataRect.bottom < 200)
				{
					FontSize = BOLDFONT;
					GapY = 13;
				}
				// Calculate the 'x' co-ord for the shirts
				GetBasicDisplay()->SetFont(FontSize);

				str.Format(IDS_NEXTMATCH, WorldData().GetCurrentDate().LongDateStr());
				int ShirtX = max(2, GetCentreX() - (memDC.GetTextExtent(str).cx / 2) - 40);
				m_pngLargeKits.DoDrawTransparent(CBitmapFormView::GetMemDC()->operator HDC(), ShirtX, TOPY + 18, WorldData().GetCurrentUserClub()->GetHomeKitID());

				if (m_CurrentSelection != eCLUB)
				{
					TextColour = BLACK;
				}
				else
				{
					TextColour = BLUE;
				}
				str.Format(_T("%s - %s"), WorldData().GetCurrentUserClub()->GetName(), WorldData().GetCurrentUserClub()->GetManager().GetName());
				GetBasicDisplay()->PrintCentredString(DataRect.top, str, TextColour, FontSize);
				DataRect.top += GapY;

				if (m_CurrentSelection != eDIVISION)
				{
					TextColour = BLACK;
				}
				else
				{
					TextColour = BLUE;
				}
				if (WorldData().GetCurrentUserClub()->GetTableData().GetNumberGamesPlayed() > 0)
				{
					str.Format(IDS_LEAGUEPOSITION, WorldData().GetCurrentUserClub()->GetDivision().GetName(), WorldData().GetCurrentUserClub()->GetTableData().GetLeaguePosition() + 1);
				}
				else
				{
					str = WorldData().GetCurrentUserClub()->GetDivision().GetName();
				}
				GetBasicDisplay()->PrintCentredString(DataRect.top, str, TextColour, FontSize);
				DataRect.top += GapY;

				TextColour = BLACK;
				str.Format(IDS_FANCONFIDENCE, WorldData().GetCurrentUserClub()->GetManager().GetSupportersConfidence());
				GetBasicDisplay()->PrintCentredString(DataRect.top, str, TextColour, FontSize);
				DataRect.top += GapY;

				str.Format(IDS_BOARDCONFIDENCE, WorldData().GetCurrentUserClub()->GetManager().GetBoardConfidence());
				GetBasicDisplay()->PrintCentredString(DataRect.top, str, TextColour, FontSize);
				DataRect.top += GapY * 2 + GapY / 2;

				WorldData().GetCurrentUserClub()->GetNextMatchDetails(Match);
				// Display the details of the next match, if known
				if (Match.GetCompetitionType() != NOTAMATCH && Match.GetOpponentClub(WorldData().GetCurrentUserClub()))
				{
					str.Format(IDS_NEXTMATCH, Match.GetTheDate().LongDateStr());
					GetBasicDisplay()->PrintCentredString(DataRect.top, str, TextColour, FontSize);
					DataRect.top += GapY;

					if (m_CurrentSelection != eMATCH)
					{
						TextColour = BLACK;
					}
					else
					{
						TextColour = BLUE;
					}
					GetBasicDisplay()->PrintCentredString(DataRect.top, Match.GetCompetitionName(), TextColour, FontSize);
					DataRect.top += GapY;
					m_pngLargeKits.DoDrawTransparent(CBitmapFormView::GetMemDC()->operator HDC(), ShirtX, DataRect.top - 6, Match.GetOpponentClub(WorldData().GetCurrentUserClub())->GetHomeKitID());
					if (Match.GetCompetitionType() == LEAGUEMATCH)
					{
						str.Format(IDS_LEAGUEPOSITION, Match.GetOpponentClub(WorldData().GetCurrentUserClub())->GetName(), Match.GetOpponentClub(WorldData().GetCurrentUserClub())->GetTableData().GetLeaguePosition() + 1);
					}
					else
					{
						str = Match.GetOpponentClub(WorldData().GetCurrentUserClub())->GetName();
					}
					GetBasicDisplay()->PrintCentredString(DataRect.top, str, TextColour, FontSize);
					DataRect.top += GapY;

					GetBasicDisplay()->PrintCentredString(DataRect.top, Match.GetStadiumName(), TextColour, FontSize);
				}
			}

			break;
		case SCREEN05_TEAMVIEW:
			m_Screen.OnDraw(CBitmapFormView::GetBasicDisplay());
			break;
		case SCREEN50_TABLEVIEW:
			break;
		case SCREEN11_NEWSVIEW:
			m_MessageDisplay.OnPaint(CBitmapFormView::GetMemDC());
			break;
		case SCREEN26_ACCOUNTSVIEW:
			//CBitmapFormView::GetGrid().OnPaint();
			break;
		case SCREEN18_CLUBFIXTURESVIEW:
			m_ClubFixtureGrid.OnPaint();
			break;
		case SCREEN28_PROGRESSVIEW:
			{
				DataRect.SetRect(4, TOPY, GetBasicDisplay()->GetOffScreenBuffer()->GetBufferWidth(), GetBasicDisplay()->GetOffScreenBuffer()->GetBufferHeight());

				int TopShirtY = TOPY + 18;
				int BottomShirtY = TopShirtY + GAPY * 6;
				if (DataRect.bottom < 200)
				{
					FontSize = BOLDFONT;
					GapY = 13;
					TopShirtY -= 12;
					BottomShirtY = (TopShirtY + GAPY * 5) - 4;
				}

				str.Format(IDS_NEXTMATCH, WorldData().GetCurrentDate().LongDateStr());
				int ShirtX = max(2, GetCentreX() - (memDC.GetTextExtent(str).cx / 2) - 40);
				m_pngLargeKits.DoDrawTransparent(CBitmapFormView::GetMemDC()->operator HDC(), ShirtX, TopShirtY, WorldData().GetCurrentUserClub()->GetHomeKitID());

				COLORREF oldColor = memDC.SetTextColor(BLACK);

				str.Format(_T("%s - %s"), WorldData().GetCurrentUserClub()->GetName(), WorldData().GetCurrentUserClub()->GetManager().GetName());
				GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);
				DataRect.top += GapY;

				if (WorldData().GetCurrentUserClub()->GetTableData().GetNumberGamesPlayed() > 0)
				{
					str.Format(IDS_LEAGUEPOSITION, WorldData().GetCurrentUserClub()->GetDivision().GetName(), WorldData().GetCurrentUserClub()->GetTableData().GetLeaguePosition() + 1);
				}
				else
				{
					str = WorldData().GetCurrentUserClub()->GetDivision().GetName();
				}
				GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);
				DataRect.top += GapY;

				str.Format(IDS_FANCONFIDENCE, WorldData().GetCurrentUserClub()->GetManager().GetSupportersConfidence());
				GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);
				DataRect.top += GapY;

				str.Format(IDS_BOARDCONFIDENCE, WorldData().GetCurrentUserClub()->GetManager().GetBoardConfidence());
				GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);
				DataRect.top += GapY * 2 + GapY / 2;

				WorldData().GetCurrentUserClub()->GetNextMatchDetails(Match);
				// Display the details of the next match, if known
				if (Match.GetCompetitionType() != NOTAMATCH && Match.GetOpponentClub(WorldData().GetCurrentUserClub()))
				{
					str.Format(IDS_NEXTMATCH, Match.GetTheDate().LongDateStr());
					GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);
					DataRect.top += GapY;
					GetBasicDisplay()->PrintCentredString(DataRect.top, Match.GetCompetitionName(), TRANSPARENT, FontSize);
					DataRect.top += GapY;
					m_pngLargeKits.DoDrawTransparent(CBitmapFormView::GetMemDC()->operator HDC(), ShirtX, BottomShirtY, Match.GetOpponentClub(WorldData().GetCurrentUserClub())->GetHomeKitID());
					if (Match.GetCompetitionType() == LEAGUEMATCH)
					{
						str.Format(IDS_LEAGUEPOSITION, Match.GetOpponentClub(WorldData().GetCurrentUserClub())->GetName(), Match.GetOpponentClub(WorldData().GetCurrentUserClub())->GetTableData().GetLeaguePosition() + 1);
					}
					else
					{
						str = Match.GetOpponentClub(WorldData().GetCurrentUserClub())->GetName();
					}
					GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);
					DataRect.top += GapY;

					GetBasicDisplay()->PrintCentredString(DataRect.top, Match.GetStadiumName(), TRANSPARENT, FontSize);
				}
				m_ProgressBar.OnPaint(memDC.operator HDC());
			}
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			break;
		case SCREEN08_CONTRACTVIEW:
			CBitmapFormView::GetGrid().OnPaint();
			break;
		case SCREEN22_BANKVIEW:
			m_InvisibleGrid.OnPaint();
			m_CurrentTitledTextSelector.OnPaint();
			break;
		case SCREEN39_JOBCENTREVIEW:
			m_EmployeeGrid.OnPaint();
			m_TypeList.OnPaint();	// Draws the top & bottom lines
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			m_YouthGrid.OnPaint();
			m_TypeList.OnPaint();	// Draws the top & bottom lines
			break;
		case SCREEN36_TRAININGVIEW:
			m_PercentSelector[0].OnPaint();
			m_PercentSelector[1].OnPaint();
			m_PercentSelector[2].OnPaint();
			m_PercentSelector[3].OnPaint();
			m_FormationSelector.OnPaint();
 			if (m_CurrentPercentSelector.GetCurSel() < m_CurrentPercentSelector.GetListSize())
 			{
 				m_CurrentPercentSelector.OnPaint();
 			}
 			else
 			{
				m_FormationSelector.DoDrawHighlight();
 			}
			break;
		case SCREEN02_CUPSCHEDULEVIEW:
			break;
		case SCREEN09_CLUBRECORDSVIEW:
			CBitmapFormView::GetGrid().OnPaint();
			break;
		case SCREEN19_LEAGUEFIXTURESVIEW:
			{
				CDivision& pDivision = m_DivisionSelector.GetCurrentDivision();
				if (pDivision.IsPlayOffMatchDay(m_TheDate.GetMatchDay()) == true)
				{
					if (m_ResultList.GetSize() == 0)
					{
						CBitmapFormView::GetMemDC()->DisplayNoData(IDS_PLAYOFFS);
					}
				}
				else if (pDivision.IsFriendlyMatchDay(m_TheDate.GetMatchDay()) == true)
				{ 
					if (m_ResultList.GetSize() == 0)
					{
						CBitmapFormView::GetMemDC()->DisplayNoData(IDS_FRIENDLYMATCH);
					}
				}
				else if (m_ResultList.GetSize() == 0)
				{
					CBitmapFormView::GetMemDC()->DisplayNoData(IDS_NOMATCH);
				}
				m_DateText.OnPaint();

				CPoint LHPoint, RHPoint;
				LHPoint.x = GetCentreX() - 55;
				LHPoint.y = TOPY;
				RHPoint.x = LHPoint.x + 110;
				RHPoint.y = LHPoint.y;
				GetBasicDisplay()->GetOffScreenBuffer()->DrawLine(LHPoint, RHPoint);
				LHPoint.y += eUPDOWNHEIGHT - 1;
				RHPoint.y += eUPDOWNHEIGHT - 1;
				GetBasicDisplay()->GetOffScreenBuffer()->DrawLine(LHPoint, RHPoint);
			}
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			{
				if (m_ResultList.GetSize() == 0)
				{
					CBitmapFormView::GetMemDC()->DisplayNoData(IDS_NOMATCH);
				}

				GetBasicDisplay()->PrintCentredString(TOPY - 2, m_DateStr, BLACK, SMALLFONT);

				CRect rect;
				GetClientRect(&rect);
				rect.top = m_RoundY + 2;
				if (m_bShowReplays == true)
				{
					CBitmapFormView::GetMemDC()->DisplayTitle(GetCurrentCupRound().GetReplayTitle(), &rect, (CBitmapFormView::GetMemDC()->GetBufferWidth() /2), BLACK, 16, false, FW_SEMIBOLD);
				}
				else
				{
					CBitmapFormView::GetMemDC()->DisplayTitle(GetCurrentCupRound().GetTitle(), &rect, (CBitmapFormView::GetMemDC()->GetBufferWidth() /2), BLACK, 16, false, FW_SEMIBOLD);
				}

				CPoint LHPoint, RHPoint;
				LHPoint.x = GetCentreX() - COMPETITIONNAMELENGTH;
				LHPoint.y = m_RoundY;
				RHPoint.x = LHPoint.x + COMPETITIONNAMELENGTH * 2;
				RHPoint.y = LHPoint.y;
				GetBasicDisplay()->GetOffScreenBuffer()->DrawLine(LHPoint, RHPoint);
				LHPoint.y += eUPDOWNHEIGHT - 1;
				RHPoint.y += eUPDOWNHEIGHT - 1;
				GetBasicDisplay()->GetOffScreenBuffer()->DrawLine(LHPoint, RHPoint);
			}
			break;
		case SCREEN27_MATCHTACTICSVIEW:
			m_TacticsData.OnPaint();
			break;
		case SCREEN27_MATCHSPECIALPLAYERSVIEW:
			{
// 				m_pngRoles.DoInitialise(::GetDC(m_hWnd), IDR_PNGROLES, 4);
// 				CPlayerSquad& pSquad = WorldData().GetCurrentUserClub()->GetFirstTeamSquad();
// 				m_Captain.DoInitialise((pSquad.GetCaptainSquadNum() != NOPLAYER) ? pSquad.GetCaptainSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
// 				m_PenaltyTaker.DoInitialise((pSquad.GetPenaltyTakerSquadNum() != NOPLAYER) ? pSquad.GetPenaltyTakerSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
// 				m_CornerTaker.DoInitialise((pSquad.GetCornerTakerSquadNum() != NOPLAYER) ? pSquad.GetCornerTakerSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
// 				m_FreeKickTaker.DoInitialise((pSquad.GetFreeKickTakerSquadNum() != NOPLAYER) ? pSquad.GetFreeKickTakerSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
				m_pngRoles.DoDraw(CBitmapFormView::GetMemDC()->operator HDC(), PICX, CAPTAINY, 1);
				m_pngRoles.DoDraw(CBitmapFormView::GetMemDC()->operator HDC(), PICX, PENALTYTAKERY, 0);
				m_pngRoles.DoDraw(CBitmapFormView::GetMemDC()->operator HDC(), PICX, CORNERTAKERY, 2);
				m_pngRoles.DoDraw(CBitmapFormView::GetMemDC()->operator HDC(), PICX, FREEKICKTAKERY, 3);
				m_Captain.OnPaint();
				m_PenaltyTaker.OnPaint();
				m_CornerTaker.OnPaint();
				m_FreeKickTaker.OnPaint();
			}
			break;
		case SCREEN27_MATCHFORMATIONVIEW:
			{
				m_Pitch.Draw(memDC.operator HDC(), m_PitchRect.left, m_PitchRect.top);

				GetBasicDisplay()->SetFontBold();
				memDC.SetTextColor(WHITE);
				CRect rect;
				GetClientRect(&rect);
				CTactic& pCurrentTactics = WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTactics();
				for (int LoopCount = 1; LoopCount < PLAYERSINTEAM; LoopCount++)
				{
					if (LoopCount == m_iPlayerSelected)
					{
						rect.left = PlayerNameRect.left;
						rect.top = PlayerNameRect.top;
						CPlayer* pPlayer = &WorldData().GetCurrentUserClub()->GetPlayer(m_iPlayerSelected);
						memDC.FillSolidRect(rect.left, rect.top, rect.right - rect.left, 16, PALEYELLOW);
						memDC.SetTextColor(PosnColour[pPlayer->GetSkills().GetPosition()]);
						memDC.ExtTextOut(rect.left, rect.top, ETO_CLIPPED, rect, pPlayer->GetName(), pPlayer->GetName().GetLength(), null);
						rect.left = PlayerNameRect.right + 16;
						memDC.ExtTextOut(rect.left, rect.top, ETO_CLIPPED, rect, pPlayer->GetSkills().PositionStr(), pPlayer->GetSkills().PositionStr().GetLength(), null);
					}
					m_pngNumbers.DoDrawTransparent(CBitmapFormView::GetMemDC()->operator HDC(), m_PitchRect.left + (pCurrentTactics.GetPlayerPositionX (LoopCount - 1) * iPitchXGap) - 8, 
						m_PitchRect.top + 5 + pCurrentTactics.GetPlayerPositionY (LoopCount - 1) * iPitchYGap, LoopCount + (pCurrentTactics.PlayerPosition(LoopCount - 1) - 1) * PLAYERSINTEAM);
				}
				GetBasicDisplay()->RestoreFont();

				m_FormationSelector27.OnPaint();
			}
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			if (CBitmapFormView::GetGrid().CountRows() == 0)
			{
				CBitmapFormView::GetMemDC()->DisplayNoData(IDS_NOMATCH);
			}
			else
			{
				// Display column headers
				int TitleID[] = 
				{
					IDS_SCREENHEADER3301, IDS_SCREENHEADER3302
				};
				GetBasicDisplay()->PrintStringID(CBitmapFormView::GetGrid().GetColumnScreenX(3) + 12, TOPY + eUPDOWNHEIGHT + 2, TitleID[m_ViewMode], BLACK, SMALLFONT);
				CBitmapFormView::GetGrid().OnPaint();
			}
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			m_DivisionSelector.OnPaint();
			if (m_DivisionSelector.GetCurrentDivision().DoCountMOMWinners() == 0)
			{
				COffScreenBuffer& memDC = *CBitmapFormView::GetMemDC();
				memDC.SetTextColor(BLACK);
				memDC.DisplayNoData(IDS_NODATA);
			}
			else
			{
				CBitmapFormView::GetGrid().OnPaint();
			}
			break;
		default:
			EXT_ASSERT(false);
	}
	if (m_HasCupSelector == true)
	{
		m_CupSelector.OnPaint();
	}
	if (m_HasDivisionSelector == true)
	{
		m_DivisionSelector.OnPaint();
	}
	memDC.Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::ReDraw
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::ReDraw()
{
	if (m_CurrentView == SCREEN03_STATUSVIEW)
	{
		m_ClubBalanceText.SetTextColour(BLACK);
		m_NewsText->SetTextColour(BLACK);
		switch (m_CurrentSelection)
		{
			case eACCOUNTS:
				m_ClubBalanceText.SetTextColour(BLUE);
				break;
 			case eNEWS:
				m_NewsText->SetTextColour(BLUE);
				break;
		}
	}
	CBitmapFormView::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnActionClicked
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnActionClicked()
{
	int MenuIDList[] =
	{
		0, IDR_SCREEN03MENU, IDR_SCREEN05MENU, IDR_SCREEN50MENU, IDR_BITMAPFORMMENU, IDR_SCREEN26MENU, 
		IDR_SCREEN18MENU, IDR_BITMAPFORMMENU, IDR_SCREEN48MENU, IDR_SCREEN08DATEMENU, IDR_SCREEN22MENU, 
		IDR_SCREEN39AVAILABLEMENU, IDR_SCREEN41AVAILABLEMENU, IDR_BITMAPFORMMENU, IDR_SCREEN02MENU, IDR_BITMAPFORMMENU, 
		IDR_SCREEN19MENU, IDR_SCREEN20MENU, IDR_SCREEN27TACTICVIEWMENU, IDR_SCREEN27ROLEVIEWMENU, IDR_BITMAPFORMMENU, 
		IDR_SCREEN33DISCMENU, IDR_BITMAPFORMMENU, 0
	};
	int MenuID = MenuIDList[m_CurrentView];
	EXT_ASSERT(MenuID > 0);
	if (m_CurrentView == SCREEN08_CONTRACTVIEW && m_WhichEvents == 0)
	{
		MenuID = IDR_SCREEN08WAGEMENU;
	}
	if (m_CurrentView == SCREEN33_LEADINGGOALSCORERSVIEW && m_ViewMode == 0)
	{
		MenuID = IDR_SCREEN33GOALSMENU;
	}

	int MenuHeightList[] =
	{
		0,						
		ACTION_MENU_HEIGHT4,
		ACTION_MENU_HEIGHT6,
		ACTION_MENU_HEIGHT6,
		0,	// SCREEN11_NEWSVIEW,
		ACTION_MENU_HEIGHT2,
		ACTION_MENU_HEIGHT2,	// SCREEN18_CLUBFIXTURESVIEW,
		0,	// SCREEN28_PROGRESSVIEW,
		ACTION_MENU_HEIGHT7,
		ACTION_MENU_HEIGHT5,
		ACTION_MENU_HEIGHT2,
		ACTION_MENU_HEIGHT3,
		ACTION_MENU_HEIGHT3,	// SCREEN41_YOUTHSQUADVIEW,
		0,	// SCREEN36_TRAININGVIEW,
		ACTION_MENU_HEIGHT2,
		0,	// SCREEN09_CLUBRECORDSVIEW,
		ACTION_MENU_HEIGHT5,
		ACTION_MENU_HEIGHT6,
		ACTION_MENU_HEIGHT2,
		ACTION_MENU_HEIGHT4,
		0,  // SCREEN27_MATCHFORMATIONVIEW,
		ACTION_MENU_HEIGHT2,
		0, 	// SCREEN46_MONTHLYAWARDSVIEW,
		0,  // SCREEN87_LOADINGVIEW

	};
	int MenuHeight = MenuHeightList[m_CurrentView];
	if (m_CurrentView == SCREEN39_JOBCENTREVIEW && m_TypeList.GetCurSel() == 0)
	{
		MenuID = IDR_SCREEN39EMPLOYEDMENU;
		MenuHeight = ACTION_MENU_HEIGHT3;
	}

	if (m_CurrentView == SCREEN20_CUPFIXTURESVIEW)
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_SCREEN20MENU));
		CMenu* pPopup = menu.GetSubMenu(0);
		EXT_ASSERT_POINTER(pPopup, CMenu);
		if (GetCurrentCupRound().IsGroupMatch() == false)
		{
			// No Group tables available
			MenuHeight -= ACTION_MENU_ITEMHEIGHT;
			pPopup->RemoveMenu(ID_ACTION6, MF_BYCOMMAND);
		}
		if (GetCurrentCupRound().HaveAllRoundMatchesBeenPlayed() == false)
		{
			// No match reports available
			MenuHeight -= ACTION_MENU_ITEMHEIGHT;
			pPopup->RemoveMenu(ID_ACTION5, MF_BYCOMMAND);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN, 10, GetBasicDisplay()->GetHeight() + 26 - MenuHeight, AfxGetMainWnd());
		AfxGetApp()->PumpMessage();
	}
	else
	{
		CBitmapFormView::OnActionMenu(MenuID, GetBasicDisplay()->GetHeight() + 26 - MenuHeight);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnNextCupScreen20
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnNextCupScreen20()
{
	m_CupSelector.OnIncCurSel();
	OnSelchangeCupListScreen20();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnPreviousCupScreen20
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnPreviousCupScreen20()
{
	m_CupSelector.OnDecCurSel();
	OnSelchangeCupListScreen20();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnNextCupRoundScreen20
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnNextCupRoundScreen20()
{
	bool bReBuild = false;
	CCup& TheCup = m_CupSelector.GetCurrentCup();
	if (m_bShowReplays == false)
	{
		if (TheCup.GetRound(m_iCurrentRound).GetReplayListSize() > 0 && TheCup.GetRound(m_iCurrentRound).Is2Regions() == false)
		{
			bReBuild = true;
			m_bShowReplays = true;
		}
	}
	if (bReBuild == false && m_iCurrentRound < TheCup.GetRounds().GetUpperBound())
	{
		// Show if next round draw available
		if (TheCup.GetRound(m_iCurrentRound + 1).HasDrawBeenMade() == true)
		{
			m_bShowReplays = false;
			m_iCurrentRound++;
			bReBuild = true;
		}
	}
	if (bReBuild == true)
	{
		DoBuildGridScreen20();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnPreviousCupRoundScreen20
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnPreviousCupRoundScreen20()
{
	bool bReBuild = false;
	CCup& TheCup = m_CupSelector.GetCurrentCup();
	if (m_bShowReplays == true)
	{
		bReBuild = true;
		m_bShowReplays = false;
	}
	if (bReBuild == false && m_iCurrentRound > 0)
	{
		bReBuild = true;
		if (TheCup.GetRounds()[m_iCurrentRound].GetReplayListSize() > 0 && TheCup.GetRound(m_iCurrentRound).Is2Regions() == false)
		{
			m_bShowReplays = true;
		}
		else
		{
			m_iCurrentRound--;
		}
	}
	if (bReBuild == true)
	{
		DoBuildGridScreen20();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnViewMatchReport
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnViewMatchReport()
{
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	switch(m_CurrentView)
	{
		case SCREEN18_CLUBFIXTURESVIEW:
		case SCREEN19_LEAGUEFIXTURESVIEW:
			{
				ushort theRow = TheGrid.GetCurrentRow();
				if (m_ResultList.GetAt(theRow)->GetFixture() != null)
				{
					m_ResultList.GetAt(theRow)->GetFixture()->DoShowSummary(null);
				}
			}
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			m_ResultList.GetAt(CBitmapFormView::GetGrid().GetCurrentRow())->GetFixture()->DoShowSummary(&m_CupSelector.GetCurrentCup());
			return;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnSelchangeCupListScreen02
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnSelchangeCupListScreen02()
{
	DoBuildGridScreen02();
	DoAdjustColumnWidthsScreen02();
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnSelchangeCupListScreen20
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnSelchangeCupListScreen20()
{
	m_iCurrentRound = 0;
	m_bShowReplays = false;
	DoBuildGridScreen20();
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnSelchangeDivisionList
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnSelchangeDivisionList()
{
	CDivision& theDivision = m_DivisionSelector.GetCurrentDivision();

	switch (m_CurrentView)
	{
		case SCREEN19_LEAGUEFIXTURESVIEW:
			DoBuildGridScreen19();
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			m_PointScorers.RemoveList();
			m_GoalScorers.RemoveList();
			if (m_DivisionSelector.GetCurrentDivision().GetFirstClubInDivision().GetTableData().GetNumberGamesPlayed())
			{
				m_PointScorers.CreateList(theDivision, MAXLISTSIZE33);
				m_GoalScorers.DoCreateList(theDivision, MAXLISTSIZE33);
			}
			DoBuildGridScreen33();
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			DoBuildGridScreen48();
			break;
		case SCREEN50_TABLEVIEW:
			theDivision.SortCurrentForm();

			m_FairPlayList.RemoveList();
			for (int LoopCount = 0; LoopCount < theDivision.GetNumberOfClubsInDivision(); LoopCount++)
			{
				m_FairPlayList.AddNew(LoopCount + theDivision.GetFirstClubNumberInDivision(), theDivision.GetClub(LoopCount).GetSeasonStats().GetSeasonDisciplinaryPoints());
			}
			m_FairPlayList.SortHigh();
			DoBuildGridScreen50();
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			DoBuildGridScreen46();
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnSwitchTab
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnSwitchTab()
{
	// Screen08
	m_SortOrder = UNSORTED;
	int SaveCurSel = CBitmapFormView::GetGrid().GetCurrentRow();
	DoBuildGridScreen08();
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnViewSquad
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnViewSquad()
{
	OnScreen05();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnViewLeagueTable
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnViewLeagueTable()
{
	OnScreen50();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnViewAccounts
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnViewAccounts()
{
	OnScreen26();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnViewNews
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnViewNews()
{
	OnScreen11();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnVScroll
	Access:    	public 
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Parameter: 	CScrollBar * _ScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar)
{
	if (m_HasGrid == true)
	{
		CBitmapFormView::GetGrid().OnVScroll(_SBCode, _Pos, _ScrollBar);
	}
	switch (m_CurrentView)
	{
		case SCREEN05_TEAMVIEW:
			m_Screen.OnVScroll(_SBCode, _Pos, _ScrollBar);
			break;
		case SCREEN11_NEWSVIEW:
			m_MessageDisplay.OnVScroll(_SBCode, _Pos, _ScrollBar);
			break;
		case SCREEN39_JOBCENTREVIEW:
			m_EmployeeGrid.OnVScroll(_SBCode, _Pos, _ScrollBar);
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			m_YouthGrid.OnVScroll(_SBCode, _Pos, _ScrollBar);
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
	if (m_HasDivisionSelector == true)
	{
		m_DivisionSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	}
	if (m_HasCupSelector == true)
	{
		m_CupSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	}
	if (m_HasGrid == true)
	{
	}
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	switch(m_CurrentView)	
	{
		case SCREEN03_STATUSVIEW:
			{
				switch (nChar)
				{
				case VK_UP:
					m_CurrentSelection--;
					if (m_CurrentSelection < 0)
					{
						m_CurrentSelection = eMAXVIEWS - 1;
					}
					ReDraw();
					break;

				case VK_DOWN:
					m_CurrentSelection++;
					if (m_CurrentSelection >= eMAXVIEWS)
					{
						m_CurrentSelection = 0;
					}
					ReDraw();
					break;

				case VK_RETURN:
				case VK_SELECT:
					switch (m_CurrentSelection)
					{
						case eCLUB:
							OnViewSquad();
							break;
						case eDIVISION:
							OnViewLeagueTable();
							break;
						case eMATCH:
							break;
						case eNEWS:
							OnViewNews();
							break;
						case eACCOUNTS:
							OnViewAccounts();
							break;
					}
					break;
				}
			}
			break;
		case SCREEN05_TEAMVIEW:
			m_Screen.OnKeyDown(nChar, nRepCnt, nFlags);
			break;
		case SCREEN50_TABLEVIEW:
			switch (nChar)
			{
				case VK_LEFT:
				case VK_RIGHT:
					OnSelchangeDivisionList();
					ReDraw();
					break;

				case VK_SELECT:
				case VK_RETURN:
					{
						CPopupClubMenu TheMenu;
						TheMenu.DoModal((CClub*)TheGrid.GetCurrentRowPointer(), CPoint(100, 100));
					}
					break;
			}
			break;
		case SCREEN11_NEWSVIEW:
			m_MessageDisplay.OnKeyDown(nChar, nRepCnt, nFlags);
			break;
		case SCREEN26_ACCOUNTSVIEW:
		case SCREEN18_CLUBFIXTURESVIEW:
		case SCREEN28_PROGRESSVIEW:
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			switch (nChar)
			{
				case VK_LEFT:
				case VK_RIGHT:
					DoBuildGridScreen48();
					ReDraw();
					break;

				case VK_F23:
				case VK_RETURN:
					WorldData().SetCurrentClub(WorldData().GetCurrentUserClub());
					CPlayer* pPlayer = (CPlayer*)TheGrid.GetCurrentRowPointer();
					CPopupPlayerMenu TheMenu;
					TheMenu.DoModal(pPlayer, CPoint(GetCentreX() - 30, GetCentreY() - 20));
					DoBuildPlayerList();
					m_ClubBalanceText.DoUpdate();
					break;
			}
			break;
		case SCREEN08_CONTRACTVIEW:
			switch (nChar)
			{
				case VK_LEFT:
				case VK_RIGHT:
					// Simulate tab control selection
					if (m_WhichEvents == 0)
					{
						m_WhichEvents = 1;
					}
					else
					{
						m_WhichEvents = 0;
					}
					OnSwitchTab();
					break;

				case VK_RETURN:
				case VK_SELECT:
				case VK_F23:
					break;
			}
			break;
		case SCREEN22_BANKVIEW:
			m_CurrentTitledTextSelector.OnKeyDown(nChar, nRepCnt, nFlags);
			break;
		case SCREEN39_JOBCENTREVIEW:
			switch (nChar)
			{
				case VK_UP:
				case VK_DOWN:
					m_EmployeeGrid.OnKeyDown(nChar, nRepCnt, nFlags);
					break;

				case VK_LEFT:
				case VK_RIGHT:
					m_TypeList.OnKeyDown(nChar, nRepCnt, nFlags);
					DoBuildGridScreen39();
					break;

				case VK_RETURN:
				case VK_F23:
					break;
			}
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			switch (nChar)
			{
				case VK_UP:
				case VK_DOWN:
					m_YouthGrid.OnKeyDown(nChar, nRepCnt, nFlags);
					break;

				case VK_LEFT:
				case VK_RIGHT:
					m_TypeList.OnKeyDown(nChar, nRepCnt, nFlags);
					DoBuildGridScreen41();
					break;

				case VK_RETURN:
				case VK_F23:
					break;
			}
			break;
		case SCREEN36_TRAININGVIEW:
			m_CurrentPercentSelector.OnKeyDown(nChar, nRepCnt, nFlags);
			if (m_CurrentPercentSelector.GetCurSel() == m_CurrentPercentSelector.GetListSize())
			{
				m_FormationSelector.OnKeyDown(nChar, nRepCnt, nFlags);
			}
			else
			{
				if (nChar == VK_LEFT)
				{
					m_PercentSelector[m_CurrentPercentSelector.GetCurSel()].OnIncValue();
					OnClickAdjustPercentage(m_CurrentPercentSelector.GetCurSel(), false);
				}
				else if (nChar == VK_RIGHT)
				{
					m_PercentSelector[m_CurrentPercentSelector.GetCurSel()].OnDecValue();
					OnClickAdjustPercentage(m_CurrentPercentSelector.GetCurSel());
				}
			}
			break;
		case SCREEN02_CUPSCHEDULEVIEW:
			switch (nChar)
			{
				case VK_LEFT:
				case VK_RIGHT:
					OnSelchangeCupListScreen02();
					break;

				case VK_SELECT:
					break;
			}
			break;
		case SCREEN09_CLUBRECORDSVIEW:
			break;
		case SCREEN19_LEAGUEFIXTURESVIEW:
			switch (nChar)
			{
				case VK_LEFT:
				case VK_RIGHT:
					OnSelchangeDivisionList();
					break;
				case VK_NUMPAD1:
					m_DivisionSelector.GetCurrentDivision().PrevMatchDateThisSeason(m_TheDate);
					DoBuildGridScreen19();
					break;
				case VK_NUMPAD3:
					m_DivisionSelector.GetCurrentDivision().NextMatchDateThisSeason(m_TheDate);
					DoBuildGridScreen19();
					break;
				case VK_SELECT:
				case VK_RETURN:
				case VK_F23:
					OnViewMatchReport();
					break;
			}
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			switch (nChar)
			{
				case VK_LEFT:
				case VK_RIGHT:
					OnSelchangeCupListScreen20();
					break;
				case VK_NUMPAD1:
					OnPreviousCupRoundScreen20();
					break;
				case VK_NUMPAD3:
					OnNextCupRoundScreen20();
					break;
				case VK_SELECT:
				case VK_RETURN:
				case VK_F23:
					OnViewMatchReport();
					break;
			}
			break;
		case SCREEN27_MATCHTACTICSVIEW:
			m_TacticsData.OnKeyDown(nChar, nRepCnt, nFlags);
			ReDraw();
			break;
		case SCREEN27_MATCHSPECIALPLAYERSVIEW:
			break;
		case SCREEN27_MATCHFORMATIONVIEW:
			m_FormationSelector27.OnKeyDown(nChar, nRepCnt, nFlags);
			WorldData().GetCurrentUser()->GetTacticsList().SetTacticsInUse(m_FormationSelector27.GetCurSel());
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			switch (nChar)
			{
				case VK_LEFT:
				case VK_RIGHT:
					OnSelchangeDivisionList();
					break;
			}
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			switch (nChar)
			{
				case VK_LEFT:
				case VK_RIGHT:
					OnSelchangeDivisionList();
					ReDraw();
					break;
			}
			break;
 			default:
 				EXT_ASSERT(false);
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapFormView::OnKeyUp(nChar, nRepCnt, nFlags);
	if (m_HasDivisionSelector == true)
	{
		m_DivisionSelector.OnKeyUp(nChar, nRepCnt, nFlags);
	}
	if (m_HasCupSelector == true)
	{
		m_CupSelector.OnKeyUp(nChar, nRepCnt, nFlags);
	}
	if (m_HasGrid == true)
	{
	}
	CGrid& TheGrid = CBitmapFormView::GetGrid();
	switch(m_CurrentView)	
	{
		case SCREEN03_STATUSVIEW:
			break;
		case SCREEN05_TEAMVIEW:
			break;
		case SCREEN50_TABLEVIEW:
			break;
		case SCREEN11_NEWSVIEW:
			break;
		case SCREEN26_ACCOUNTSVIEW:
			break;
		case SCREEN18_CLUBFIXTURESVIEW:
			break;
		case SCREEN28_PROGRESSVIEW:
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			break;
		case SCREEN08_CONTRACTVIEW:
			break;
		case SCREEN22_BANKVIEW:
			m_CurrentTitledTextSelector.OnKeyUp(nChar, nRepCnt, nFlags);
			break;
		case SCREEN39_JOBCENTREVIEW:
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			break;
		case SCREEN36_TRAININGVIEW:
			m_CurrentPercentSelector.OnKeyUp(nChar, nRepCnt, nFlags);
			break;
		case SCREEN02_CUPSCHEDULEVIEW:
			break;
		case SCREEN09_CLUBRECORDSVIEW:
			break;
		case SCREEN19_LEAGUEFIXTURESVIEW:
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			break;
		case SCREEN27_MATCHTACTICSVIEW:
			m_TacticsData.OnKeyUp(nChar, nRepCnt, nFlags);
			break;
		case SCREEN27_MATCHSPECIALPLAYERSVIEW:
			break;
		case SCREEN27_MATCHFORMATIONVIEW:
			m_FormationSelector27.OnKeyDown(nChar, nRepCnt, nFlags);
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			break;
		default:
			EXT_ASSERT(false);
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnLButtonUp
	Access:    	public 
	Parameter: 	UINT _nFlags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnLButtonUp(UINT _nFlags, CPoint _Point)
{
	int ButtonPressed = CBitmapFormView::GetBasicDisplay()->OnLButtonUp(_nFlags, _Point);
	switch (m_CurrentView)
	{
		case SCREEN03_STATUSVIEW:
			break;
		case SCREEN05_TEAMVIEW:
			break;
		case SCREEN50_TABLEVIEW:
			break;
		case SCREEN11_NEWSVIEW:
			break;
		case SCREEN26_ACCOUNTSVIEW:
			break;
		case SCREEN18_CLUBFIXTURESVIEW:
			break;
		case SCREEN28_PROGRESSVIEW:
			break;
		case SCREEN48_TRANSFERMARKETVIEW:
			break;
		case SCREEN08_CONTRACTVIEW:
			break;
		case SCREEN22_BANKVIEW:
			switch (ButtonPressed)
			{
				case eBORROWDOWN:
				case eBORROWUP:
					m_TextSelector[eBORROW].OnLButtonUp(ButtonPressed);
					break;

				case eREPAYUP:
				case eREPAYDOWN:
					m_TextSelector[eREPAY].OnLButtonUp(ButtonPressed);
					break;
			}
			break;
		case SCREEN39_JOBCENTREVIEW:
			break;
		case SCREEN41_YOUTHSQUADVIEW:
			break;
		case SCREEN36_TRAININGVIEW:
			{
				if (m_FormationSelector.OnLButtonUp(ButtonPressed) == true)
				{
					DoSaveTeamTraining();
				}
				else
				{
					for (int LoopCount = 0; LoopCount < NUM_PERCENT_SELECTORS36; LoopCount++)
					{
						if (m_PercentSelector[LoopCount].IsUpButton(ButtonPressed) == true)
						{
							OnClickAdjustPercentage(LoopCount);
							break;
						}
						if (m_PercentSelector[LoopCount].IsDownButton(ButtonPressed) == true)
						{
							OnClickAdjustPercentage(LoopCount, false);
							break;
						}
					}
				}
			}
			break;
		case SCREEN02_CUPSCHEDULEVIEW:
			break;
		case SCREEN09_CLUBRECORDSVIEW:
			break;
		case SCREEN19_LEAGUEFIXTURESVIEW:
			break;
		case SCREEN20_CUPFIXTURESVIEW:
			if (m_CupSelector.OnLButtonUp(ButtonPressed) == true)
			{
				OnSelchangeCupListScreen20();
			}
			else
			{
				switch (ButtonPressed)
				{
					case eROUNDDOWN20:
						OnPreviousCupRoundScreen20();
						break;
					case eROUNDUP20:
						OnNextCupRoundScreen20();
						break;
				}
			}
			ReDraw();
			{
				int iColumn = CBitmapFormView::GetGrid().GetCurrentRow(_Point);
				if (iColumn != NOTFOUND && iColumn < m_ResultList.GetSize() && m_ResultList.GetAt(iColumn)->GetFixture() != null)
				{
					m_ResultList.GetAt(iColumn)->GetFixture()->DoShowSummary(&m_CupSelector.GetCurrentCup());
				}
			}
			break;
		case SCREEN27_MATCHTACTICSVIEW:
			m_TacticsData.OnLButtonUp(ButtonPressed, _nFlags, _Point);
			break;
		case SCREEN27_MATCHSPECIALPLAYERSVIEW:
			m_Captain.OnLButtonUp(ButtonPressed);
			m_PenaltyTaker.OnLButtonUp(ButtonPressed);
			m_CornerTaker.OnLButtonUp(ButtonPressed);
			m_FreeKickTaker.OnLButtonUp(ButtonPressed);
			ReDraw();
			break;
		case SCREEN27_MATCHFORMATIONVIEW:
			m_FormationSelector27.OnLButtonUp(ButtonPressed);
			// Check pitch
			{
				CPoint TempPoint(_Point);
				if (m_PitchRect.PtInRect(_Point) == true)
				{
					TempPoint.x -= m_PitchRect.left - 8;
					TempPoint.y -= m_PitchRect.top;
					TempPoint.x /= iPitchXGap;
					TempPoint.y /= iPitchYGap;
					CExtPoint TPoint;
					TPoint.SetX(TempPoint.x);
					TPoint.SetY(TempPoint.y);

					ushort iFound = WorldData().GetCurrentUser()->GetTacticsList().GetAt(m_FormationSelector27.GetCurSel()).PlayerAtPosition(TPoint);
					if (iFound != wNOTFOUND)
					{
						m_iPlayerSelected = iFound + 1;		// Skip goalkeeper
					}
					ReDraw();
				}
				else
				{
					// Check player name
					if (PlayerNameRect.PtInRect(_Point) == true)
					{
						CPopupOwnedPlayerMenu TheMenu;
						EXT_ASSERT(m_iPlayerSelected < PLAYERSINSQUAD);
						TheMenu.DoModal(&WorldData().GetCurrentUserClub()->GetPlayer(m_iPlayerSelected), _Point);
					}
				}
			}
			break;
		case SCREEN33_LEADINGGOALSCORERSVIEW:
			break;
		case SCREEN46_MONTHLYAWARDSVIEW:
			break;
		default:
			EXT_ASSERT(false);
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen03View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen03View::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);
		if (theApp.IsInitialised() == true)
		{
			CRect theRect;
			CRect theWindow;
			GetClientRect(&theWindow);
			theRect = theWindow;
			int GapY = 18;
			int GridWidth;
			int RowsToDisplay;
			int CurrY = TOPY;
			CGrid& TheGrid = CBitmapFormView::GetGrid();
			switch (m_CurrentView)
			{
				case SCREEN03_STATUSVIEW:
					if (theWindow.bottom < 201)
					{
						GapY = 13;
						m_ClubBalanceText.SetFont(GetBasicDisplay()->GetBoldFont());
						m_NewsText->SetFont(GetBasicDisplay()->GetBoldFont());
					}
					else
					{
						m_ClubBalanceText.SetFont(GetBasicDisplay()->GetBigFont());
						m_NewsText->SetFont(GetBasicDisplay()->GetBigFont());
					}
					m_ClubBalanceText.MoveWindow(CRect(0, TOPY + GapY * 4, GetBasicDisplay()->GetWidth(), TOPY + GapY * 5 + 4));
					m_NewsText->MoveWindow(CRect(0, theWindow.bottom - GapY - 2, GetBasicDisplay()->GetWidth(), theWindow.bottom));
					break;
				case SCREEN05_TEAMVIEW:
					m_Screen.OnSize(theWindow);
					break;
				case SCREEN50_TABLEVIEW:
					m_DivisionSelector.MoveWindow(CRect(GetCentreX() - 55, TOPY, GetCentreX() + 55, TOPY + TABHEIGHT));
					GridWidth = 118;
					m_ClubNameLength = 74;
					if (m_ViewMode < 4 && theWindow.right > 300)
					{
						// Overall, home, away and current form
						m_ClubNameLength = 114;	// Club Name
						GridWidth += 20;
					}
					TheGrid.SetColumnWidth(0, m_ClubNameLength);	// Club Name
					RowsToDisplay = (theWindow.bottom - TOPY - 2 - TABHEIGHT) / TheGrid.GetRowHeight();
					TheGrid.MoveWindow(CRect (GetCentreX() - GridWidth, DIVISIONSELECTOR_TOPY, GetCentreX() + GridWidth - TheGrid.GetScrollBarWidth(), DIVISIONSELECTOR_TOPY + RowsToDisplay * TheGrid.GetRowHeight()));
					break;
				case SCREEN11_NEWSVIEW:
					m_MessageDisplay.MoveWindow(CRect(2, TOPY, theWindow.right, theWindow.bottom - 24));
					break;
				case SCREEN26_ACCOUNTSVIEW:
					RowsToDisplay = (theWindow.bottom - 56 - 20) / TheGrid.GetRowHeight();
					theRect = CRect(theWindow.right / 2 - 118, 56, theWindow.right / 2 + 118, 56 + RowsToDisplay * TheGrid.GetRowHeight());	
					TheGrid.MoveWindow(theRect);

					// Club Balance
					theRect.top = theRect.bottom + 2;
					theRect.left = GetCentreX() - 118;
					theRect.right = theRect.left + 148;
					theRect.bottom = theRect.top + 18;
					m_ClubBalanceText.MoveWindow(&theRect);
					break;
				case SCREEN18_CLUBFIXTURESVIEW:
					theWindow.top = TOPY;
					theWindow.bottom -= 2;
					m_ClubFixtureGrid.MoveWindow(theWindow);
					break;
				case SCREEN28_PROGRESSVIEW:
					theRect.left += 2;
					theRect.right -= 2;
					theRect.bottom -= 1;
					theRect.top = theRect.bottom - 27;
					m_ProgressBar.MoveWindow(&theRect);
					break;
				case SCREEN48_TRANSFERMARKETVIEW:
					m_DivisionSelector.MoveWindow(CRect(GetCentreX() - 55, TOPY, GetCentreX() + 55, TOPY + TABHEIGHT));
					RowsToDisplay = (theRect.bottom - 2 - DIVISIONSELECTOR_TOPY - 16) / TheGrid.GetRowHeight();
					theRect = CRect(GetCentreX() - GRID_WIDTH48 / 2, DIVISIONSELECTOR_TOPY, GetCentreX() + GRID_WIDTH48 / 2 - TheGrid.GetScrollBarWidth(), DIVISIONSELECTOR_TOPY + RowsToDisplay * TheGrid.GetRowHeight());	
					TheGrid.MoveWindow(theRect);
					m_ClubBalanceText.MoveWindow(CRect(0, theWindow.bottom - 18, theWindow.right, theWindow.bottom - 2));
					break;
				case SCREEN08_CONTRACTVIEW:
					RowsToDisplay = (theWindow.bottom - TOPY - 2) / TheGrid.GetRowHeight();
					TheGrid.MoveWindow(CRect(GetCentreX() - GRID_WIDTH08 / 2, TOPY, GetCentreX() + GRID_WIDTH08 / 2 - TheGrid.GetScrollBarWidth(), TOPY + RowsToDisplay * TheGrid.GetRowHeight()));
					break;
				case SCREEN22_BANKVIEW:
					{
						int BorrowX = GetCentreX() + 10;
						m_TextSelector[eBORROW].MoveWindow();
						m_TextSelector[eREPAY].MoveWindow();

						m_InvisibleGrid.MoveWindow(CRect(0, 140, GetCentreX() * 2, 144 + m_InvisibleGrid.GetRowHeight() * 3));
						m_InvisibleGrid.SetColumnWidth(0, GetCentreX() + 20);
						m_InvisibleGrid.SetColumnWidth(1, GetCentreX() - 20);
					}
					break;
				case SCREEN39_JOBCENTREVIEW:
					theRect.top = CurrY;
					CurrY += eUPDOWNHEIGHT;
					theRect.bottom = CurrY; 
					theRect.left = GetCentreX() - COMPETITIONNAMELENGTH;
					theRect.right = GetCentreX() + COMPETITIONNAMELENGTH;
					m_TypeList.MoveWindow(theRect);
					CurrY += 2;
					RowsToDisplay = (theWindow.bottom - CurrY - 2) / m_EmployeeGrid.GetRowHeight();
					m_EmployeeGrid.MoveWindow(CRect (GetCentreX() - GRIDWIDTH2, CurrY, GetCentreX() + GRIDWIDTH2, CurrY + RowsToDisplay * m_EmployeeGrid.GetRowHeight()));
					break;
				case SCREEN41_YOUTHSQUADVIEW:
					theRect.top = CurrY;
					CurrY += eUPDOWNHEIGHT;
					theRect.bottom = CurrY; 
					theRect.left = GetCentreX() - COMPETITIONNAMELENGTH;
					theRect.right = GetCentreX() + COMPETITIONNAMELENGTH;
					m_TypeList.MoveWindow(theRect);
					CurrY += 2;
					RowsToDisplay = (theWindow.bottom - CurrY - 2) / m_YouthGrid.GetRowHeight();
					m_YouthGrid.MoveWindow(CRect (GetCentreX() - YGRIDWIDTH2, CurrY, GetCentreX() + YGRIDWIDTH2, CurrY + RowsToDisplay * m_YouthGrid.GetRowHeight()));
					break;
				case SCREEN36_TRAININGVIEW:
					m_PercentSelector[0].MoveWindow();
					m_PercentSelector[1].MoveWindow();
					m_PercentSelector[2].MoveWindow();
					m_PercentSelector[3].MoveWindow();
					m_FormationSelector.MoveWindow();
					break;
				case SCREEN02_CUPSCHEDULEVIEW:
					m_CupSelector.MoveWindow(CRect(GetCentreX() - COMPETITIONNAMELENGTH, TOPY, GetCentreX() + COMPETITIONNAMELENGTH, TOPY + TABHEIGHT));
					GridWidth = min(theRect.right - 10, 260);
					RowsToDisplay = (theRect.bottom - 2 - DIVISIONSELECTOR_TOPY) / TheGrid.GetRowHeight();
					theRect = CRect(GetCentreX() - GridWidth / 2, DIVISIONSELECTOR_TOPY, GetCentreX() + GridWidth / 2 - TheGrid.GetScrollBarWidth(), DIVISIONSELECTOR_TOPY + RowsToDisplay * TheGrid.GetRowHeight());	
					TheGrid.MoveWindow(theRect);
					DoAdjustColumnWidthsScreen02();
					break;
				case SCREEN09_CLUBRECORDSVIEW:
					RowsToDisplay = (theWindow.bottom - TOPY - 2) / TheGrid.GetRowHeight();
					TheGrid.MoveWindow(CRect (2, TOPY, theWindow.right - 2 - TheGrid.GetScrollBarWidth(), TOPY + RowsToDisplay * TheGrid.GetRowHeight()));
					break;
				case SCREEN19_LEAGUEFIXTURESVIEW:
					m_DateText.MoveWindow(CRect(GetCentreX() - COMPETITIONNAMELENGTH, CurrY, GetCentreX() + COMPETITIONNAMELENGTH, CurrY + 16));
					{
						int Button1X = GetCentreX() - 75;
						int ButtonY = CurrY;
						int Button2X = GetCentreX() + 55;
						GetBasicDisplay()->DoMoveButton(eDATEDOWN19, CRect(Button1X, ButtonY, Button1X + eUPDOWNWIDTH, ButtonY + eUPDOWNHEIGHT));
						GetBasicDisplay()->DoMoveButton(eDATEUP19, CRect(Button2X, ButtonY, Button2X + eUPDOWNWIDTH, ButtonY + eUPDOWNHEIGHT));

						m_DivisionSelector.MoveWindow(CRect(0, TOPY + eUPDOWNHEIGHT + 4, 0, TOPY + eUPDOWNHEIGHT * 2 + 4));

						RowsToDisplay = (theRect.bottom - 2 - DATEDIVISIONSELECTOR_TOPY) / TheGrid.GetRowHeight();
						GridWidth = min(260, theRect.right - TheGrid.GetScrollBarWidth() - 2);
						TheGrid.MoveWindow(CRect(GetCentreX() - GridWidth / 2, DATEDIVISIONSELECTOR_TOPY, GetCentreX() + GridWidth / 2 - TheGrid.GetScrollBarWidth(), DATEDIVISIONSELECTOR_TOPY + RowsToDisplay * TheGrid.GetRowHeight()));
						DoAdjustColumnWidthsScreen19();
					}
					break;
				case SCREEN20_CUPFIXTURESVIEW:
					MoveGridScreen20();
					break;
				case SCREEN27_MATCHTACTICSVIEW:
					m_TacticsData.OnSize();
					break;
				case SCREEN27_MATCHSPECIALPLAYERSVIEW:
					m_Captain.MoveWindow(CRect(NAMEX, CAPTAINY + 8, NAMEX + COMPETITIONNAMELENGTH * 2, CAPTAINY + 24));
					m_PenaltyTaker.MoveWindow(CRect(NAMEX, PENALTYTAKERY + 8, NAMEX + COMPETITIONNAMELENGTH * 2, PENALTYTAKERY + 24));
					m_CornerTaker.MoveWindow(CRect(NAMEX, CORNERTAKERY + 8, NAMEX + COMPETITIONNAMELENGTH * 2, CORNERTAKERY + 24));
					m_FreeKickTaker.MoveWindow(CRect(NAMEX, FREEKICKTAKERY + 8, NAMEX + COMPETITIONNAMELENGTH * 2, FREEKICKTAKERY + 24));
					break;
				case SCREEN27_MATCHFORMATIONVIEW:
					m_PitchRect.left = GetCentreX() - 113;
					m_PitchRect.top = TOPY + 24;
					m_PitchRect.right = GetCentreX() + 113;
					m_PitchRect.bottom = TOPY + 24 + 116;

					// Formation title Listbox
					theWindow.top = TOPY + 2;
					theWindow.bottom = theWindow.top + 18;
					theWindow.left = GetCentreX() - 50;
					theWindow.right = theWindow.left + 100;
					m_FormationSelector27.MoveWindow(theWindow);
					break;
				case SCREEN33_LEADINGGOALSCORERSVIEW:
					DoAdjustColumnWidthsScreen33();
					m_DivisionSelector.MoveWindow(CRect(GetCentreX() - 55, TOPY, GetCentreX() + 55, TOPY + eUPDOWNHEIGHT));
					break;
				case SCREEN46_MONTHLYAWARDSVIEW:
					RowsToDisplay = (theWindow.bottom - DIVISIONSELECTOR_TOPY - 2) / TheGrid.GetRowHeight();
					TheGrid.MoveWindow(CRect (GetCentreX() - 118, DIVISIONSELECTOR_TOPY, GetCentreX() + 118 - TheGrid.GetScrollBarWidth(), DIVISIONSELECTOR_TOPY + RowsToDisplay * TheGrid.GetRowHeight()));
					m_DivisionSelector.MoveWindow(CRect(GetCentreX() - 55, TOPY, GetCentreX() + 55, theWindow.bottom - 2));
					break;
				default:
					EXT_ASSERT(false);
			}
		}
		Sleep(1);
	}
}