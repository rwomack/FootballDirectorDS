// Screen20.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "Form.h"
#include "FootballDirector.h"
#include "Competition.h"

#include "Screen20View.h"
#include "Screen55Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen20View dialog

IMPLEMENT_DYNCREATE(CScreen20View, CFormView)


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::CScreen20View
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen20View::CScreen20View()
    : m_CurrentSelector(0)
	, CGridCupSelectorBitmapFormView(CScreen20View::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::~CScreen20View
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen20View::~CScreen20View()
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::GetCurrentCupRound
	Access:    	public 
	Returns:   	CCupRound&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound& CScreen20View::GetCurrentCupRound()
{
	return m_CupSelector.GetCurrentCup().GetRound(m_iCurrentRound);
}


BEGIN_MESSAGE_MAP(CScreen20View, CGridCupSelectorBitmapFormView)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()

	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)
	ON_COMMAND(ID_ACTION1, OnNextRound)
	ON_COMMAND(ID_ACTION2, OnPreviousRound)
	ON_COMMAND(ID_ACTION3, OnNextCup)
	ON_COMMAND(ID_ACTION4, OnPreviousCup)
	ON_COMMAND(ID_ACTION5, OnViewMatchReport)
	ON_COMMAND(ID_ACTION6, OnViewGroupTables)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen20View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnActionClicked
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnActionClicked()
{
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_SCREEN20MENU));
	CMenu* pPopup = menu.GetSubMenu(0);
	EXT_ASSERT_POINTER(pPopup, CMenu);

	int MenuHeight = ACTION_MENU_HEIGHT6;
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


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnNextCup
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnNextCup()
{
	CGridCupSelectorBitmapFormView::OnNextCup();
	OnSelchangeCupList();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnPreviousCup
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnPreviousCup()
{
	CGridCupSelectorBitmapFormView::OnPreviousCup();
	OnSelchangeCupList();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnNextRound
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnNextRound()
{
	OnNextCupRound();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnPreviousRound
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnPreviousRound()
{
	OnPreviousCupRound();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnViewMatchReport
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnViewMatchReport()
{
	m_ResultList.GetAt(m_Grid.GetCurrentRow())->GetFixture()->DoShowSummary(&m_CupSelector.GetCurrentCup());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnViewGroupTables
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnViewGroupTables()
{
	CScreen55Dlg theDlg(m_CupSelector.GetCurrentCup());
	theDlg.DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnPaint()
{
	CGridCupSelectorBitmapFormView::OnPaint();
	if (m_ResultList.GetSize() == 0)
	{
		CBitmapFormView::GetMemDC()->DisplayNoData(IDS_NOMATCH);
	}

	GetBasicDisplay()->PrintCentredString(TOPY - 2, m_Date, BLACK, SMALLFONT);

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

	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnInitialUpdate
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnInitialUpdate()
{
	CGridCupSelectorBitmapFormView::OnInitialUpdate(TEXT("Cup Fixtures"), &theApp.GetBasicDisplay(), IDS_SCREEN20);
	
	uint theButtons[] =
	{
		IDR_PNGLEFT, IDR_PNGRIGHT, 
		NOTFOUND
	};
	GetBasicDisplay()->DoAddButtons(&theButtons[0], this);
	OnSelchangeCupList();
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnSelchangeCupList
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnSelchangeCupList()
{
	m_iCurrentRound = 0;
	m_bShowReplays = false;
	BuildMatchDisplayList();
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::BuildMatchDisplayList
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::BuildMatchDisplayList()
{
	m_Grid.RemoveAllData(false);
	m_Grid.DoAddColumn(98, COL_TEXT | COL_ALIGNR);		// Home team
	m_Grid.DoAddColumn(34, COL_TEXT | COL_ALIGNC);		// Result and league position
	m_Grid.DoAddColumn(98);								// Away team

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
		m_Date = CurrentCupRound.LongReplayDateStr();
	}
	else
	{
		m_Date = CurrentCupRound.LongMatchDateStr();
	}

	if (CurrentCupRound.IsGroupMatch() == false)
	{
		m_ResultList.SortByHomeClub();
	}
	m_Grid.DoAddRows(m_ResultList.GetSize());
	MoveGrid();	// Scrollbar any or not be visible

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
		m_Grid.SetRowTextColour(LoopCount, TextCol);

		m_Grid.SetItemText(LoopCount, 0, m_ResultList.GetAt(LoopCount)->GetHomeClubDetails().GetName());
		m_Grid.SetItemText(LoopCount, 1, m_ResultList.GetAt(LoopCount)->GoalsStr());
		m_Grid.SetItemText(LoopCount, 2, m_ResultList.GetAt(LoopCount)->GetAwayClubDetails().GetName());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnPreviousCupRound
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnPreviousCupRound()
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
		BuildMatchDisplayList();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnNextCupRound
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnNextCupRound()
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
		BuildMatchDisplayList();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGridCupSelectorBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			OnSelchangeCupList();
			break;
		case VK_NUMPAD1:
			OnPreviousCupRound();
			break;
		case VK_NUMPAD3:
			OnNextCupRound();
			break;
		case VK_SELECT:
		case VK_RETURN:
		case VK_F23:
			OnViewMatchReport();
			break;
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnLButtonUp(UINT _nFlags, CPoint _Point)
{
	int ButtonPressed = CBitmapFormView::GetBasicDisplay()->OnLButtonUp(_nFlags, _Point);
	if (m_CupSelector.OnLButtonUp(ButtonPressed) == true)
	{
		OnSelchangeCupList();
	}
	else
	{
		switch (ButtonPressed)
		{
			case eROUNDDOWN:
				OnPreviousCupRound();
				break;
			case eROUNDUP:
				OnNextCupRound();
				break;
		}
	}
	ReDraw();
	int iColumn = m_Grid.GetCurrentRow(_Point);
	if (iColumn != NOTFOUND && iColumn < m_ResultList.GetSize() && m_ResultList.GetAt(iColumn)->GetFixture() != null)
	{
		m_ResultList.GetAt(iColumn)->GetFixture()->DoShowSummary(&m_CupSelector.GetCurrentCup());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapFormView::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::MoveGrid
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::MoveGrid()
{
	if (IsInitialised() == true)
	{
		CRect theRect;
		GetClientRect(&theRect);
		int CentreX = theRect.right / 2;

		m_RoundY = TOPY + 14;
		m_CupSelector.MoveWindow(CRect(CentreX - COMPETITIONNAMELENGTH, m_RoundY, CentreX + COMPETITIONNAMELENGTH, m_RoundY + TABHEIGHT));

		int GridBottom = (theRect.bottom - TOPY20 - 2);
		int RowsToDisplay = GridBottom / m_Grid.GetRowHeight();
		GridBottom = TOPY20 + RowsToDisplay * m_Grid.GetRowHeight();
		m_Grid.MoveWindow(CRect(CentreX - GRID_WIDTH / 2, TOPY20, CentreX + GRID_WIDTH / 2 - m_Grid.GetScrollBarWidth(), GridBottom));

		// Round
		m_RoundY += eUPDOWNHEIGHT + 2;
		int Button2X = CentreX + COMPETITIONNAMELENGTH;
		int Button1X = CentreX - COMPETITIONNAMELENGTH - eUPDOWNWIDTH;
		GetBasicDisplay()->DoMoveButton(eROUNDDOWN, CRect(Button1X, m_RoundY, Button1X + eUPDOWNWIDTH, m_RoundY + eUPDOWNHEIGHT));
 		GetBasicDisplay()->DoMoveButton(eROUNDUP, CRect(Button2X, m_RoundY, Button2X + eUPDOWNWIDTH, m_RoundY + eUPDOWNHEIGHT));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen20View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen20View::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);
		MoveGrid();
	}
	Sleep(1);
}