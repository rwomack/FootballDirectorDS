
 /*

	Screen19View.cpp : League Fixtures CFormView implementation file

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------  
 */

#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "FootballDirector.h"
#include "Screen19View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/*------------------------------------------------------------------------------
	Method:   	CScreen19View::CScreen19View
	Access:    	public 
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen19View::CScreen19View()
    : CGridDivisionSelectorBitmapFormView(CScreen19View::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::~CScreen19View
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen19View::~CScreen19View()
{
}


BEGIN_MESSAGE_MAP(CScreen19View, CGridDivisionSelectorBitmapFormView)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()

	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)
	ON_COMMAND(ID_ACTION1, OnNextDate)
	ON_COMMAND(ID_ACTION2, OnPreviousDate)
	ON_COMMAND(ID_ACTION3, OnNextDivision)
	ON_COMMAND(ID_ACTION4, OnPreviousDivision)
	ON_COMMAND(ID_ACTION5, OnViewMatchReport)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen19View message handlers

void CScreen19View::OnActionClicked()
{
	CBitmapFormView::OnActionMenu(IDR_SCREEN19MENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT5);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::OnNextDivision
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19View::OnNextDivision()
{
	m_DivisionSelector.OnIncCurSel();
	OnSelchangeDivisionlist();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::OnPreviousDivision
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19View::OnPreviousDivision()
{
	m_DivisionSelector.OnDecCurSel();
	OnSelchangeDivisionlist();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::OnViewMatchReport
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19View::OnViewMatchReport()
{
	ushort theRow = m_Grid.GetCurrentRow();
	if (m_ResultList.GetAt(theRow)->GetFixture() != null)
	{
		m_ResultList.GetAt(theRow)->GetFixture()->DoShowSummary(null);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19View::OnPaint()
{
	CGridDivisionSelectorBitmapFormView::OnPaint();
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

	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGridDivisionSelectorBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			OnSelchangeDivisionlist();
			ReDraw();
			break;
		case VK_NUMPAD1:
			OnPreviousDate();
			break;
		case VK_NUMPAD3:
			OnNextDate();
			break;
		case VK_SELECT:
		case VK_RETURN:
		case VK_F23:
			OnViewMatchReport();
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::OnInitialUpdate
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19View::OnInitialUpdate()
{
	CGridDivisionSelectorBitmapFormView::OnInitialUpdate(TEXT("League Fixtures"), &theApp.GetBasicDisplay(), IDS_SCREEN19);
	//m_Grid.HideHilite();
	uint theButtons[] =
	{
		IDR_PNGLEFT, IDR_PNGRIGHT,
		NOTFOUND
	};
	GetBasicDisplay()->DoAddButtons(&theButtons[0], this);
	m_TheDate.DoInitialise(WorldData().GetCurrentDate());
	m_DateText.DoInitialise(GetBasicDisplay(), _T(""), COL_ALIGNC);
	OnSelchangeDivisionlist();
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::OnSelchangeDivisionlist
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19View::OnSelchangeDivisionlist()
{
    OnNewDate();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::OnPreviousDate
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19View::OnPreviousDate()
{
    m_DivisionSelector.GetCurrentDivision().PrevMatchDateThisSeason(m_TheDate);
    OnNewDate();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::OnNewDate
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19View::OnNewDate()
{
	m_DateText.SetText(m_TheDate.LongDateStr());
	m_Grid.RemoveAllData(false);
	m_Grid.ResetScrollBar();
	m_ResultList.RemoveList();
	m_DivisionSelector.GetCurrentDivision().DoBuildLeagueFixtureList(m_TheDate, m_ResultList);
	if (m_ResultList.GetSize() > 0)
	{
		m_Grid.DoAddColumn(10, COL_TEXT | COL_ALIGNR);		// Home team
		m_Grid.DoAddColumn(10, COL_TEXT | COL_ALIGNC);		// Result
		m_Grid.DoAddColumn(10);								// Away team
		SetGridColumnWidths();
		m_Grid.DoAddRows(m_ResultList.GetSize());

		CSoccerResult* pResult;
		for (int LoopCount = 0; LoopCount < m_ResultList.GetSize(); LoopCount++)
		{
			pResult = m_ResultList.GetAt(LoopCount);
			// Highlight the user's result
			if (pResult->GetHomeClubDetails().IsUserControlledClub() == true || 
				pResult->GetAwayClubDetails().IsUserControlledClub() == true)
			{
				m_Grid.SetRowTextColour(LoopCount, WHITE);
			}
			else 
			{
				m_Grid.SetRowTextColour(LoopCount, BLACK);
			}
			m_Grid.SetItemText(LoopCount, 0, pResult->GetHomeClubDetails().GetName());
			m_Grid.SetItemText(LoopCount, 1, pResult->GoalsStr());
			m_Grid.SetItemText(LoopCount, 2, pResult->GetAwayClubDetails().GetName());
		}
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::OnNextDate
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19View::OnNextDate()
{
    m_DivisionSelector.GetCurrentDivision().NextMatchDateThisSeason(m_TheDate);
    OnNewDate();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19View::OnLButtonUp(UINT _Flags, CPoint _Point)
{
	int ButtonPressed = GetBasicDisplay()->OnLButtonUp(_Flags, _Point);
	switch (ButtonPressed)
	{
		case eDATEDOWN:
			OnPreviousDate();
			break;
		case eDATEUP:
			OnNextDate();
			break;
	}
	if (m_DivisionSelector.OnLButtonUp(ButtonPressed) == true)
	{
		OnSelchangeDivisionlist();
		ReDraw();
	}
	if (m_Grid.GetCurrentRow(_Point) != NOTFOUND)
	{
		ushort theRow = m_Grid.GetCurrentRow(_Point);
		if (m_ResultList.GetAt(theRow)->GetFixture() != null)
		{
			m_ResultList.GetAt(theRow)->GetFixture()->DoShowSummary(null);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19View::OnLButtonDown(UINT _Flags, CPoint _Point)
{
	CGridDivisionSelectorBitmapFormView::OnLButtonDown(_Flags, _Point);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19View::OnSize(UINT nType, int cx, int cy)
{
	CGridDivisionSelectorBitmapFormView::OnSize(nType, cx, cy);

	if (IsInitialised() == true)
	{
		CRect theRect;
		GetClientRect(&theRect);

		int RoundY = TOPY;
		m_DateText.MoveWindow(CRect(GetCentreX() - COMPETITIONNAMELENGTH, RoundY, GetCentreX() + COMPETITIONNAMELENGTH, RoundY + 16));
		int Button1X = GetCentreX() - 75;
		int ButtonY = RoundY;
		int Button2X = GetCentreX() + 55;
		GetBasicDisplay()->DoMoveButton(eDATEDOWN, CRect(Button1X, ButtonY, Button1X + eUPDOWNWIDTH, ButtonY + eUPDOWNHEIGHT));
		GetBasicDisplay()->DoMoveButton(eDATEUP, CRect(Button2X, ButtonY, Button2X + eUPDOWNWIDTH, ButtonY + eUPDOWNHEIGHT));

		m_DivisionSelector.MoveWindow(CRect(0, TOPY + eUPDOWNHEIGHT + 4, 0, TOPY + eUPDOWNHEIGHT * 2 + 4));

		int RowsToDisplay = (theRect.bottom - 2 - DATEDIVISIONSELECTOR_TOPY) / m_Grid.GetRowHeight();
		int GridWidth = min(260, theRect.right - m_Grid.GetScrollBarWidth() - 2);
		m_Grid.MoveWindow(CRect(GetCentreX() - GridWidth / 2, DATEDIVISIONSELECTOR_TOPY, GetCentreX() + GridWidth / 2 - m_Grid.GetScrollBarWidth(), DATEDIVISIONSELECTOR_TOPY + RowsToDisplay * m_Grid.GetRowHeight()));
		SetGridColumnWidths();
	}
	Sleep(1);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen19View::SetGridColumnWidths
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen19View::SetGridColumnWidths()
{
	if (IsInitialised() == true)
	{
		if (m_Grid.CountColumns() == 3)	// All columns have been created
		{
			int Width = m_Grid.GetWidth();
			m_Grid.SetColumnWidth(0, (Width / 7) * 3);		// Home team
			m_Grid.SetColumnWidth(1, Width / 7);			// Result
			m_Grid.SetColumnWidth(2, (Width / 7) * 3);		// Away team
		}
	}
}