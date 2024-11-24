// Screen08View.cpp : implementation file
//

#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "FootballDirector.h"
#include "Screen08View.h"
#include "PopupPlayerMenu.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen08View dialog

IMPLEMENT_DYNCREATE(CScreen08View, CFormView)


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::CScreen08View
	Access:    	public 
	Returns:  
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen08View::CScreen08View() :
	  CBitmapFormView(IDD)
		, m_WhichEvents(0)
		, m_dTotalEmpWages(0)
		, m_dTotalYouthWages(0)
		, m_dTotalPlayerWages(0)
		, m_iPlayInfoNum(0)
		, m_SortOrder(UNSORTED)
		, m_WageMultiplier(52)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::~CScreen08View
	Access:    	public 
	Returns:   		Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen08View::~CScreen08View()
{
};


BEGIN_MESSAGE_MAP(CScreen08View, CBitmapFormView)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()

	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)
	ON_COMMAND(ID_ACTION1, OnSortExpiryDate)
	ON_COMMAND(ID_ACTION2, OnSortHighestEarner)
	ON_COMMAND(ID_ACTION3, OnSortLowestEarner)
	ON_COMMAND(ID_ACTION4, OnOfferContract)
	ON_COMMAND(ID_ACTION5, OnViewDetails)
	ON_COMMAND(ID_ACTION6, OnViewExpiryDate)
	ON_COMMAND(ID_ACTION7, OnViewWage)
	ON_COMMAND(ID_ACTION8, OnViewAnnualWage)
	ON_COMMAND(ID_ACTION9, OnViewWeeklyWage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen08View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnActionClicked
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnActionClicked()
{
	if (m_WhichEvents == 0)
	{
		CBitmapFormView::OnActionMenu(IDR_SCREEN08WAGEMENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT5);
	}
	else
	{
		CBitmapFormView::OnActionMenu(IDR_SCREEN08DATEMENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT5);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnSortExpiryDate
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnSortExpiryDate()
{
	m_SortOrder = SORT_LOW;
	DoSort();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnSortHighestEarner
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnSortHighestEarner()
{
	m_SortOrder = SORT_HIGH;
	DoSort();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnSortLowestEarner
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnSortLowestEarner()
{
	m_SortOrder = SORT_LOW;
	DoSort();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::DoSort
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::DoSort()
{
	m_Grid.DoSort(2, m_SortOrder);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnOfferContract
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnOfferContract()
{
	switch(m_Grid.GetCurrentRowPointerType())
	{
		case PLAYERCONTRACT:
			WorldData().SetCurrentPlayer((CPlayer*)m_Grid.GetCurrentRowPointer());
			theApp.OnNegotiatePlayerContract();
			break;

		case EMPLOYEECONTRACT:
			WorldData().SetCurrentEmployee((CEmployee*)m_Grid.GetCurrentRowPointer());
			theApp.OnNegotiateEmployeeContract();
			break;

		case YOUTHCONTRACT:
			WorldData().SetCurrentYouth((CYouth*)m_Grid.GetCurrentRowPointer());
			theApp.OnPromoteYouth();
			break;
		}
		DoBuildList();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnViewDetails
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnViewDetails()
{
	switch(m_Grid.GetCurrentRowPointerType())
	{
		case PLAYERCONTRACT:
			WorldData().SetCurrentPlayer((CPlayer*)m_Grid.GetCurrentRowPointer());
			theApp.OnViewOwnedPlayerInformation();
			break;

		case EMPLOYEECONTRACT:
			WorldData().SetCurrentEmployee((CEmployee*)m_Grid.GetCurrentRowPointer());
			theApp.OnViewOwnedEmployeeInformation();
			break;

		case YOUTHCONTRACT:
			WorldData().SetCurrentYouth((CYouth*)m_Grid.GetCurrentRowPointer());
			theApp.OnViewOwnedYouthInformation();
			break;
		}
		DoBuildList();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnViewAnnualWage
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnViewAnnualWage()
{
	m_WageMultiplier = 52;
	DoBuildList(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnViewWeeklyWage
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnViewWeeklyWage()
{
	m_WageMultiplier = 1;
	DoBuildList(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnViewExpiryDate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnViewExpiryDate()
{
	m_WhichEvents = 1;
	OnSwitchTab();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnViewWage
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnViewWage()
{
	m_WhichEvents = 0;
	OnSwitchTab();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnDraw
	Access:    	virtual protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnDraw(CDC* pDC)
{
	CBitmapFormView::OnInitRedrawArea();
	m_Grid.OnPaint();
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_Grid.OnKeyDown(nChar, nRepCnt, nFlags);

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
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnActivateView
	Access:    	protected 
	Parameter: 	BOOL bActivate
	Parameter: 	CView * pActivateView
	Parameter: 	CView * pDeactiveView
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if (bActivate == TRUE)
	{
		DoBuildList();
	}

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnInitialUpdate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnInitialUpdate()
{
	CBitmapFormView::OnInitialUpdate(TEXT("Wages and Contracts"), &theApp.GetBasicDisplay(), IDS_SCREEN08);
	CBitmapFormView::OnInitRedrawArea();

	m_Grid.OnInitialUpdate(this, GetBasicDisplay());
	m_Grid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);

	HDC dc = ::GetDC(m_hWnd);
	m_pngPlayerStatus.DoInitialise(dc, IDR_PNGPLAYERSTATUS, MAXPLAYERSTATUSES);
	::ReleaseDC(m_hWnd, dc);

	m_WhichEvents = 0;
	WorldData().SetCurrentClub(WorldData().GetCurrentUserClub());
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::DoBuildList
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::DoBuildList(bool bPreservePosition /*= false*/)
{
	CPlayerList ThePlayers;
	CPlayer* pPlayer;
	CYouth* pYouth;
	CClub& theClub = *WorldData().GetCurrentUserClub();
	CCountry& rCountry = theClub.GetCountry();
	CString strString;
	int LoopCount;

	int CurrentPosition = 0;
	if (bPreservePosition == true)
	{
		CurrentPosition = m_Grid.GetCurrentRow();
	}
	// Clear List
	m_Grid.RemoveAllData();
	m_Grid.DoAddColumn(90);
	m_Grid.DoAddColumn(60);
	if (m_WhichEvents == 0)
	{
		m_Grid.DoAddColumn(64, COL_MONEY | COL_ALIGNR);
	}
	else
	{
		m_Grid.DoAddColumn(64, COL_DATE | COL_ALIGNL);
	}

	int CurrRow = 0;
	if (theClub.GetManagerID() != NOMANAGER)
	{
		m_Grid.DoAddRow();

		// Add Manager
		m_Grid.SetRowPointerDetails(CurrRow, &theClub.GetManager(), MANAGERCONTRACT);
		m_Grid.SetItemText(CurrRow, 0, theClub.GetManager().GetName());
		m_Grid.SetItemStrId(CurrRow, 1, IDS_MANAGER);
		switch(m_WhichEvents)
		{
			case 0:
				m_Grid.SetItemMoney(CurrRow, 2, theClub.GetForeignStr(theClub.GetManager().GetWage() * m_WageMultiplier), theClub.GetManager().GetWage() * m_WageMultiplier);
				break;
			case 1:
				m_Grid.SetItemDate(CurrRow, 2, (CDate&)theClub.GetManager().GetContractEndDate());
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

			m_Grid.DoAddRow();
			m_Grid.SetRowPointerDetails(CurrRow, pEmployee, EMPLOYEECONTRACT);
			m_Grid.SetItemText(CurrRow, 0, pEmployee->GetName());
			m_Grid.SetItemText(CurrRow, 1, pEmployee->TypeTextStr());
			switch(m_WhichEvents)
			{
				case 0:
					m_Grid.SetItemMoney(CurrRow, 2, theClub.GetForeignStr(pEmployee->GetWage() * m_WageMultiplier), pEmployee->GetWage() * m_WageMultiplier);
					break;
				case 1:
					m_Grid.SetItemDate(CurrRow, 2, (CDate&)pEmployee->GetContractEndDate());
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
		m_Grid.DoAddRow();
		m_Grid.SetRowPointerDetails(CurrRow, pPlayer, PLAYERCONTRACT);

		// Calculate total player wages
		m_dTotalPlayerWages +=  pPlayer->GetWage() * m_WageMultiplier;
		m_Grid.SetItemText(CurrRow, 0, pPlayer->GetName());
		m_Grid.SetItemTextColour(CurrRow, 0, PosnColour[pPlayer->GetSkills().GetPosition()]);
		m_Grid.SetItemStrId(CurrRow, 1, pPlayer->GetSkills().PosnLongAbbr());
		m_Grid.SetItemTextColour(CurrRow, 1, PosnColour[pPlayer->GetSkills().GetPosition()]);
		switch(m_WhichEvents)
		{
			case 0:
				m_Grid.SetItemMoney(CurrRow, 2, theClub.GetForeignStr(pPlayer->GetWage() * m_WageMultiplier), pPlayer->GetWage() * m_WageMultiplier);
				break;
			case 1:
				m_Grid.SetItemDate(CurrRow, 2, (CDate&)pPlayer->GetContractEndDate());
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
			m_Grid.DoAddRow();
			m_Grid.SetRowPointerDetails(CurrRow, pYouth, YOUTHCONTRACT);

			m_Grid.SetItemText(CurrRow, 0, pYouth->GetName());
			m_Grid.SetItemTextColour(CurrRow, 0, PosnColour[pYouth->GetSkills().GetPosition()]);
			m_Grid.SetItemStrId(CurrRow, 1, pYouth->GetSkills().PosnLongAbbr());
			m_Grid.SetItemTextColour(CurrRow, 1, PosnColour[pYouth->GetSkills().GetPosition()]);
			switch(m_WhichEvents)
			{
				case 0:
					m_Grid.SetItemMoney(CurrRow, 2, theClub.GetForeignStr(pYouth->GetWage() * m_WageMultiplier), pYouth->GetWage() * m_WageMultiplier);
					break;
				case 1:
					m_Grid.SetItemDate(CurrRow, 2, (CDate&)pYouth->GetContractEndDate());
					break;
			}
			CurrRow++;
		}
	}
	m_Grid.ResetScrollBar();
	m_Grid.SetCurrentRow(CurrentPosition);
	DoSort();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_Grid.GetCurrentRow(point) != NOTFOUND)
	{
		ushort iColumn = m_Grid.GetCurrentRow(point);
		void* DispPtr = m_Grid.GetRowPointer(iColumn);
		switch(m_Grid.GetRowPointerType(iColumn))
		{
			case PLAYERCONTRACT:
			{
				CPopupOwnedPlayerMenu TheMenu;
				TheMenu.DoModal((CPlayer*)DispPtr, point);
			}
			break;
			
			case EMPLOYEECONTRACT:
			{
				CPopupOwnedEmployeeMenu TheMenu;
				TheMenu.DoModal((CEmployee*)DispPtr, point);
			}
			break;
			
			case YOUTHCONTRACT:
			{
				CPopupOwnedYouthMenu TheMenu;
				TheMenu.DoModal((CYouth*)DispPtr, point);
			}
			break;
		}
		DoBuildList();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnSwitchTab
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnSwitchTab()
{
	m_SortOrder = UNSORTED;
	int SaveCurSel = m_Grid.GetCurrentRow();
	DoBuildList();
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::PreCreateWindow
	Access:    	public 
	Parameter: 	CREATESTRUCT & cs
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen08View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnVScroll
	Access:    	protected 
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Parameter: 	CScrollBar * _ScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar)
{
	m_Grid.OnVScroll(_SBCode, _Pos, _ScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen08View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen08View::OnSize(UINT nType, int cx, int cy)
{
	CBitmapFormView::OnSize(nType, cx, cy);

	if (IsInitialised() == true)
	{
		CRect theRect;
		GetClientRect(&theRect);

		int RowsToDisplay = (theRect.bottom - TOPY - 2) / m_Grid.GetRowHeight();
		theRect = CRect(GetCentreX() - GRID_WIDTH / 2, TOPY, GetCentreX() + GRID_WIDTH / 2 - m_Grid.GetScrollBarWidth(), TOPY + RowsToDisplay * m_Grid.GetRowHeight());	
		m_Grid.MoveWindow(theRect);

		Sleep(1);
	}
}