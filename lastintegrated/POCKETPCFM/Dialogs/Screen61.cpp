// Screen61.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "MessageDisplay.h"
#include "PopupPlayerMenu.h"
#include "PlayerNameTextRegion.h"
#include "Screen61.h"
#include "LoanPeriodSelector.h"
#include "Screen24.h"
#include "Screen30.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen61 dialog



/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::CScreen61Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen61Dlg::CScreen61Dlg(CPlayer* pPlayer, CWnd* pParent /*=null*/)
    : CBitmapDialog(CScreen61Dlg::IDD, pParent)
{
	EXT_ASSERT_POINTER(pPlayer, CPlayer);
	m_pCurrentPlayer = pPlayer;
	m_ViewMode = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::~CScreen61Dlg
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen61Dlg::~CScreen61Dlg()
{
}


BEGIN_MESSAGE_MAP(CScreen61Dlg, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()

	ON_COMMAND(ID_CONTINUE, OnOK)
	ON_COMMAND(ID_ACTION1, OnViewStats)
	ON_COMMAND(ID_ACTION2, OnViewPersonal)
	ON_COMMAND(ID_ACTION3, OnViewCareer)
	ON_COMMAND(ID_ACTION4, OnViewAvailability)

	ON_COMMAND(ID_ACTION_PLAYERSELL, OnPlayerSell)
	ON_COMMAND(ID_ACTION_OFFERCONTRACT, OnPlayerOutOfContract)
	ON_COMMAND(ID_ACTION_FREE, OnPlayerFree)
	ON_COMMAND(ID_ACTION_CONTRACT, OnPlayerContract)
	ON_COMMAND(ID_ACTION_APROACHBUYPLAYER, OnPlayerBuy)
	ON_COMMAND(ID_ACTION_APROACHLOANPLAYER, OnPlayerLoan)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CScreen61 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnViewStats
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnViewStats()
{
	m_ViewMode = 0;
	OnSelchangeTab();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnViewPersonal
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnViewPersonal()
{
	m_ViewMode = 1;
	OnSelchangeTab();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnViewCareer
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnViewCareer()
{
	m_ViewMode = 2;
	OnSelchangeTab();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnViewAvailability
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnViewAvailability()
{
	m_ViewMode = 3;
	OnSelchangeTab();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	CBitmapDialog::OnPaint(true);
	if (m_pCurrentPlayer->GetTransferStatus().IsOutOfContract() ==  false)
	{
		m_pngLargeKits.DoDrawTransparent(CBitmapDialog::GetMemDC()->operator HDC(), 2, TOPY - 2, m_pCurrentPlayer->GetClub().GetHomeKitID());
	}
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen61Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN61, IDS_SCREEN61);
	if (WorldData().GetMasterState() != CWorld::IN_PROCESSING)
	{
		int res = IDR_SCREEN61OWNEDPLAYERMENU;
		if (WorldData().GetCurrentUser()->IsManagingAClub() == false)
		{
			res = IDR_SCREEN61NOTMANAGINGMENU;
		}
		else if (m_pCurrentPlayer->GetTransferStatus().IsOutOfContract() == true)
		{
			res = IDR_SCREEN61OUTOFCONTRACTPLAYERMENU;
		}
		else if (m_pCurrentPlayer->IsAtCurrentUserControlledClub() == false)
		{ 
			if (WorldData().GetCurrentUserClub()->AreBuyTransfersAllowed() == false)
			{
				if (WorldData().GetCurrentUserClub()->AreLoanTransfersAllowed() == false)
				{
					res = IDR_SCREEN61NOTMANAGINGMENU;
				}
				else
				{
					res = IDR_SCREEN61NOBUYALLOWEDMENU;
				}
			}
			else
			{
				res = IDR_SCREEN61NOTOWNEDPLAYERMENU;
			}
		}
		CBitmapDialog::LoadMenu(res);
	}
	HDC dc = ::GetDC(m_hWnd);
	m_pngLargeKits.DoInitialise(dc, IDR_PNGLARGEHOMEKITS, MAXKITS);
	::ReleaseDC(m_hWnd, dc);

	m_PlayerName.DoInitialise(TOPY, this, GetBasicDisplay(), *m_pCurrentPlayer);
	CBitmapDialog::GetGrid().SetStyle(LC_NOHIGHLIGHT);
	CBitmapDialog::GetGrid().SetTextFont(GetBasicDisplay()->GetNormalFont());
	CBitmapDialog::GetGrid().SetRowHeight(18);
	m_TextRegionList[TEXT_APPEARANCES] = GetBasicDisplay()->DoAddTextRegion(new CTextRegion);
	CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_APPEARANCES])->DoInitialise(GetBasicDisplay(), IDS_APPEARANCES);
	m_TextRegionList[TEXT_GOALS] = GetBasicDisplay()->DoAddTextRegion(new CTextRegion);
	CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_GOALS])->DoInitialise(GetBasicDisplay(), IDS_GOALS);

	m_TextRegionList[TEXT_STATUS1] = GetBasicDisplay()->DoAddTextRegion(new CTextRegion);
	CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_STATUS1])->DoInitialise(GetBasicDisplay(), IDS_GOALS, COL_ALIGNC);
	m_TextRegionList[TEXT_STATUS2] = GetBasicDisplay()->DoAddTextRegion(new CTextRegion);
	CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_STATUS2])->DoInitialise(GetBasicDisplay(), IDS_GOALS, COL_ALIGNC);
	m_TextRegionList[TEXT_STATUS3] = GetBasicDisplay()->DoAddTextRegion(new CTextRegion);
	CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_STATUS3])->DoInitialise(GetBasicDisplay(), IDS_GOALS, COL_ALIGNC);
	m_TextRegionList[TEXT_STATUS4] = GetBasicDisplay()->DoAddTextRegion(new CTextRegion);
	CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_STATUS4])->DoInitialise(GetBasicDisplay(), IDS_GOALS, COL_ALIGNC);
	OnViewStats();
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::DoShowAvailability
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::DoShowAvailability()
{
	CBitmapDialog::GetGrid().ShowWindow(SW_HIDE);
	int CurrTextRegion = TEXT_STATUS1;
	CString str;
	if (m_pCurrentPlayer->GetDiscipline().SuspensionStr().IsEmpty() == false)
	{
		CBitmapDialog::GetTextRegion(m_TextRegionList[CurrTextRegion])->ShowWindow(SW_SHOW);
		CBitmapDialog::GetTextRegion(m_TextRegionList[CurrTextRegion++])->SetText(m_pCurrentPlayer->GetDiscipline().SuspensionStr());
	}
	if (m_pCurrentPlayer->InjuryDetailsStr().IsEmpty() == false)
	{
		CBitmapDialog::GetTextRegion(m_TextRegionList[CurrTextRegion])->ShowWindow(SW_SHOW);
		CBitmapDialog::GetTextRegion(m_TextRegionList[CurrTextRegion++])->SetText(m_pCurrentPlayer->InjuryDetailsStr());
	}
	if (m_pCurrentPlayer->HowLongUnableToPlayStr().IsEmpty() == false)
	{
		CBitmapDialog::GetTextRegion(m_TextRegionList[CurrTextRegion])->ShowWindow(SW_SHOW);
		CBitmapDialog::GetTextRegion(m_TextRegionList[CurrTextRegion++])->SetText(m_pCurrentPlayer->HowLongUnableToPlayStr());
	}
	if (m_pCurrentPlayer->GetWeeksOnLoan() > 0)
	{
		CExtString str;
		switch (m_pCurrentPlayer->GetWeeksOnLoan())
		{
			case 1:
				str.LoadString(IDS_LOANEDIN1WEEK);
				break;

			case MAXLOANPLAYERWEEKS:
				str.LoadString(IDS_LOANEDINSEASON);
				break;

			default :
				str.Format(IDS_LOANEDIN, m_pCurrentPlayer->GetWeeksOnLoan());
				break;
		}
		CBitmapDialog::GetTextRegion(m_TextRegionList[CurrTextRegion])->ShowWindow(SW_SHOW);
		CBitmapDialog::GetTextRegion(m_TextRegionList[CurrTextRegion])->SetText(str);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::DoBuildPersonalGrid
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::DoBuildPersonalGrid()
{
	DoSizePersonalGrid();

	CGrid& theGrid = CBitmapDialog::GetGrid();
	theGrid.DoAddColumn(100, COL_ALIGNR);
	CBitmapDialog::GetGrid().DoAddColumn(100, COL_ALIGNL);

	theGrid.SetColumnTextColour(0, BLACK);
	theGrid.SetColumnTextColour(1, BLUE);
	theGrid.DoAddRows(6);

	theGrid.SetItemStrId(0, 0, IDS_AGE);
	theGrid.SetItemNum(0, 1, m_pCurrentPlayer->GetAge());
	theGrid.SetItemStrId(1, 0, IDS_WEEKLYWAGE);
	CCountry& rCountry = m_pCurrentPlayer->GetClub().GetCountry();
	if (WorldData().GetCurrentUserClub() != NULL)
	{
		rCountry = WorldData().GetCurrentUserClub()->GetCountry();
	}
	theGrid.SetItemText(1, 1, rCountry.GetCurrency().GetForeignStr(m_pCurrentPlayer->GetWage()));
	theGrid.SetItemStrId(2, 0, IDS_CLUBVALUATION);
	theGrid.SetItemText(2, 1, m_pCurrentPlayer->SuggestedAskingPriceStr());
	theGrid.SetItemStrId(3, 0, IDS_LASTCLUB);
	theGrid.SetItemText(3, 1, m_pCurrentPlayer->LastClubName());
	theGrid.SetItemStrId(4, 0, IDS_NATIONALITY);
	theGrid.SetItemText(4, 1, m_pCurrentPlayer->NationalityStr());
	theGrid.SetItemStrId(5, 0, IDS_CONTRACT);
	theGrid.SetItemText(5, 1, m_pCurrentPlayer->ContractStr());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::ShowCareer
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::DoBuildCareerGrid()
{
	CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_APPEARANCES])->ShowWindow(SW_SHOW);
	CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_GOALS])->ShowWindow(SW_SHOW);

	DoSizeCareerGrid();

	CGrid& theGrid = CBitmapDialog::GetGrid();
	theGrid.DoAddColumn(36, COL_ALIGNR);
	theGrid.DoAddColumn(90, COL_ALIGNC);
	theGrid.DoAddColumn(30, COL_ALIGNR);

	theGrid.SetColumnTextColour(0, BLUE);
	theGrid.SetColumnTextColour(1, BLACK);
	theGrid.SetColumnTextColour(2, BLUE);

	theGrid.DoAddRows(5);

	theGrid.SetItemNum(0, 0, m_pCurrentPlayer->GetNumberAppearancesThisSeason());
	theGrid.SetItemStrId(0, 1, IDS_SEASON);
	theGrid.SetItemNum(0, 2, m_pCurrentPlayer->GetClubSeasonGoal());

	theGrid.SetItemNum(1, 0, m_pCurrentPlayer->GetLastSeasonAppear());
	theGrid.SetItemStrId(1, 1, IDS_LASTSEASON);
	theGrid.SetItemNum(1, 2, m_pCurrentPlayer->GetLastSeasonGoal());

	theGrid.SetItemNum(2, 0, m_pCurrentPlayer->GetCupAppearancesThisSeason());
	theGrid.SetItemStrId(2, 1, IDS_CUP);
	theGrid.SetItemNum(2, 2, m_pCurrentPlayer->GetCupGoalsScoredThisSeason());

	theGrid.SetItemNum(3, 0, m_pCurrentPlayer->GetClubAppearances());
	theGrid.SetItemStrId(3, 1, IDS_CLUBS);
	theGrid.SetItemNum(3, 2, m_pCurrentPlayer->GetClubGoal());
	
	theGrid.SetItemNum(4, 0, m_pCurrentPlayer->GetCareerAppearances());
	theGrid.SetItemStrId(4, 1, IDS_CAREER);
	theGrid.SetItemNum(4, 2, m_pCurrentPlayer->GetCareerGoals());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::DoBuildStatsGrid
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::DoBuildStatsGrid()
{
	DoSizeStatsGrid();

	CGrid& theGrid = CBitmapDialog::GetGrid();
	theGrid.DoAddColumn(74, COL_ALIGNR);
	theGrid.DoAddColumn(24, COL_ALIGNR);
	theGrid.DoAddColumn(74, COL_ALIGNR);
	theGrid.DoAddColumn(theGrid.GetWidth() - 172, COL_ALIGNL);

	theGrid.SetColumnTextColour(0, BLACK);
	theGrid.SetColumnTextColour(1, BLUE);
	theGrid.SetColumnTextColour(2, BLACK);
	theGrid.SetColumnTextColour(3, BLUE);

	theGrid.DoAddRows(6);
	theGrid.SetItemStrId(0, 0, IDS_SKILL);
	theGrid.SetItemNum(0, 1, m_pCurrentPlayer->GetSkills().GetOverallSkill());
	theGrid.SetItemStrId(0, 2, IDS_TEMPER);
	theGrid.SetItemText(0, 3, m_pCurrentPlayer->TemperamentStr());

	theGrid.SetItemStrId(1, 0, IDS_HANDLING);
	theGrid.SetItemNum(1, 1, m_pCurrentPlayer->GetSkills().GetHandling());
	theGrid.SetItemStrId(1, 2, IDS_FITNESS);
	theGrid.SetItemNum(1, 3, m_pCurrentPlayer->GetSkills().GetFitness());

	theGrid.SetItemStrId(2, 0, IDS_TACKLING);
	theGrid.SetItemNum(2, 1, m_pCurrentPlayer->GetSkills().GetTackling());
	theGrid.SetItemStrId(2, 2, IDS_FORM);
	theGrid.SetItemNum(2, 3, m_pCurrentPlayer->GetPerformance());

	theGrid.SetItemStrId(3, 0, IDS_PASSING);
	theGrid.SetItemNum(3, 1, m_pCurrentPlayer->GetSkills().GetPassing());
	theGrid.SetItemStrId(3, 2, IDS_MORALE);
	theGrid.SetItemNum(3, 3, m_pCurrentPlayer->GetMorale());

	theGrid.SetItemStrId(4, 0, IDS_SHOOTING);
	theGrid.SetItemNum(4, 1, m_pCurrentPlayer->GetSkills().GetShooting());
	theGrid.SetItemStrId(4, 2, IDS_DISCIPLINARYPOINTS);
	theGrid.SetItemNum(4, 3, m_pCurrentPlayer->GetDiscipline().GetDisciplinaryPoints());

	theGrid.SetItemStrId(5, 0, IDS_PACE);
	theGrid.SetItemNum(5, 1, m_pCurrentPlayer->GetSkills().GetPace());
	theGrid.SetItemStrId(5, 2, IDS_BESTFOOT);
	theGrid.SetItemText(5, 3, m_pCurrentPlayer->GetSkills().GetBestFootStr());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnSelchangeTab
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnSelchangeTab()
{
	CBitmapDialog::GetGrid().ShowWindow(SW_SHOW);
	CBitmapDialog::GetGrid().RemoveAllData(false);
	CBitmapDialog::GetGrid().ResetScrollBar();
	CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_APPEARANCES])->ShowWindow(SW_HIDE);
	CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_GOALS])->ShowWindow(SW_HIDE);
	CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_STATUS1])->ShowWindow(SW_HIDE);
	CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_STATUS2])->ShowWindow(SW_HIDE);
	CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_STATUS3])->ShowWindow(SW_HIDE);
	CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_STATUS4])->ShowWindow(SW_HIDE);
	switch(m_ViewMode)
	{
         case 0:
			DoBuildStatsGrid();
			break;

		 case 1:
			DoBuildPersonalGrid();
            break;

		 case 2:
			 DoBuildCareerGrid();
			 break;

		 case 3:
			 DoShowAvailability();
			 break;

		 default:
			EXT_ASSERT(false);
    }
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnButton3
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnPlayerMenu()
{
	CPopupPlayerMenu TheMenu;
	TheMenu.DoModal(m_pCurrentPlayer, CPoint(8, 216), false);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_RETURN:
		case VK_F23:
			OnOK();
			break;
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	switch (CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point))
	{
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::DoSizeCareerGrid
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::DoSizeCareerGrid()
{
	CRect theWindow;
	GetClientRect(&theWindow);
	int RowsToDisplay = min(5, (theWindow.bottom - TOPY61) / CBitmapDialog::GetGrid().GetRowHeight());
	int GridWidth = 160;
	CBitmapDialog::GetGrid().MoveWindow(CRect(GetCentreX() - GridWidth / 2, TOPY61, GetCentreX() + GridWidth / 2, TOPY61 + RowsToDisplay * CBitmapDialog::GetGrid().GetRowHeight()));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::DoSizePersonalGrid
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::DoSizePersonalGrid()
{
	CRect theWindow;
	GetClientRect(&theWindow);
	int RowsToDisplay = min(6, (theWindow.bottom - TOPY61) / CBitmapDialog::GetGrid().GetRowHeight());
	int GridWidth = 200;
	CBitmapDialog::GetGrid().MoveWindow(CRect(GetCentreX() - GridWidth / 2, TOPY61, GetCentreX() + GridWidth / 2, TOPY61 + RowsToDisplay * CBitmapDialog::GetGrid().GetRowHeight()));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::DoSizeStatsGrid
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::DoSizeStatsGrid()
{
	CRect theWindow;
	GetClientRect(&theWindow);
	int RowsToDisplay = min(6, (theWindow.bottom - TOPY61) / CBitmapDialog::GetGrid().GetRowHeight());
	int GridWidth = min(260, theWindow.right - 2);
	CBitmapDialog::GetGrid().MoveWindow(CRect(GetCentreX() - GridWidth / 2, TOPY61, GetCentreX() + GridWidth / 2, TOPY61 + RowsToDisplay * CBitmapDialog::GetGrid().GetRowHeight()));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnSize(UINT nType, int cx, int cy)
{
	CBitmapDialog::OnSize(nType, cx, cy);

	if (IsInitialised() == true)
	{
		m_PlayerName.MoveWindow();
		CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_APPEARANCES])->MoveWindow(CRect(GetCentreX() - 70, TOPY61, GetCentreX(), TOPY61 + 16));
		CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_GOALS])->MoveWindow(CRect(GetCentreX() + 46, TOPY61, GetCentreX() + 90, TOPY61 + 16));
		CRect theWindow;
		GetClientRect(&theWindow);
		CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_STATUS1])->MoveWindow(CRect(0, TOPY61, theWindow.right, TOPY61 + 16));
		CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_STATUS2])->MoveWindow(CRect(0, TOPY61 + 20, theWindow.right, TOPY61 + 36));
		CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_STATUS3])->MoveWindow(CRect(0, TOPY61 + 40, theWindow.right, TOPY61 + 56));
		CBitmapDialog::GetTextRegion(m_TextRegionList[TEXT_STATUS4])->MoveWindow(CRect(0, TOPY61 + 60, theWindow.right, TOPY61 + 76));

		switch(m_ViewMode)
		{
			case 0:
				DoSizeStatsGrid();
				break;
	
			 case 1:
				 DoSizePersonalGrid();
				break;

			 case 2:
				 DoSizeCareerGrid();
				 break;

			 case 3:
				 break;

			 default:
				EXT_ASSERT(false);
		}
	}
	Sleep(1);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnPlayerBuy
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnPlayerBuy()
{
	theApp.OnMakeOfferForPlayer();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnPlayerContract
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnPlayerContract()
{
	theApp.OnNegotiatePlayerContract();
}


/*------------------------------------------------------------------------------
Method:   	CScreen61Dlg::OnPlayerFree
Access:    	protected 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnPlayerFree()
{
	theApp.OnSackPlayer();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnPlayerLoan
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnPlayerLoan()
{
	theApp.OnApproachClubLoanInPlayer();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnPlayerOutOfContract
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnPlayerOutOfContract()
{
	theApp.OnNegotiateOutOfContractPlayerContract();
}

/*------------------------------------------------------------------------------
	Method:   	CScreen61Dlg::OnPlayerSell
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen61Dlg::OnPlayerSell()
{
	theApp.OnChangeTransferStatus();
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	COwnedScreen61::COwnedScreen61
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Parameter: 	CWnd * _Parent
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
COwnedScreen61::COwnedScreen61(CPlayer* _Player, CWnd* _Parent /*=null*/)
	: CScreen61Dlg(_Player, _Parent)
{
}


/*------------------------------------------------------------------------------
	Method:   	COwnedScreen61::OnInitDialog
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL COwnedScreen61::OnInitDialog()
{
	return CScreen61Dlg::OnInitDialog();
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
Method:   	CNotOwnedScreen61::CNotOwnedScreen61
Access:    	public 
Parameter: 	CPlayer * _Player
Parameter: 	CWnd * _Parent
Returns:   	
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
CNotOwnedScreen61::CNotOwnedScreen61(CPlayer* _Player, CWnd* _Parent /*=null*/)
	: CScreen61Dlg(_Player, _Parent)
{
}


/*------------------------------------------------------------------------------
	Method:   	CNotOwnedScreen61::OnInitDialog
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CNotOwnedScreen61::OnInitDialog()
{
	return CScreen61Dlg::OnInitDialog();
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CNoActionScreen61::OnInitDialog
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CNoActionScreen61::OnInitDialog()
{
	return CScreen61Dlg::OnInitDialog();
}
