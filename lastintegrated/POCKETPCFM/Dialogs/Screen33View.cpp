
 /*

	Screen33View.cpp : Leading goalscorers/disciplined players CFormView implementation file

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
 */


#include "stdafx.h"

#include "UI.h"

#include "Form.h"
#include "LeadingGoalScorer.h"
#include "LeadingDisciplinaryPoints.h"
#include "FootballDirector.h"
#include "Screen33View.h"
#include "PopupPlayerMenu.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/*------------------------------------------------------------------------------
	Method:   	CScreen33View::CScreen33View
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen33View::CScreen33View ()
    : CGridDivisionSelectorBitmapFormView(CScreen33View::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::~CScreen33View
	Access:    	public 
	Returns:
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen33View::~CScreen33View()
{
}


BEGIN_MESSAGE_MAP(CScreen33View, CGridDivisionSelectorBitmapFormView)
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()

	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)
	ON_COMMAND(ID_ACTION3, OnViewDisciplinary)
	ON_COMMAND(ID_ACTION4, OnViewGoals)
	ON_COMMAND(ID_ACTION5, OnViewPlayerDetails)
	ON_COMMAND(ID_ACTION6, OnViewSquadDetails)
	//ON_COMMAND(ID_ACTION7, OnViewPlayerBreakdown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen33View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::OnActionClicked
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::OnActionClicked()
{
	if (m_ViewMode == 0)
	{
		CBitmapFormView::OnActionMenu(IDR_SCREEN33GOALSMENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT2);
	}
	else
	{
		CBitmapFormView::OnActionMenu(IDR_SCREEN33DISCMENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT2);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::OnViewDisciplinary
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::OnViewDisciplinary()
{
	m_ViewMode = 0;
	CBitmapFormView::GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN3302);
	OnSelchangeDivisionlist();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::OnViewGoals
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::OnViewGoals()
{
	m_ViewMode = 1;
	CBitmapFormView::GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN3301);
	OnSelchangeDivisionlist();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::OnViewPlayerDetails
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::OnViewPlayerDetails()
{
	WorldData().OnViewPlayerDetails((CPlayer*)m_Grid.GetCurrentRowPointer());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::OnViewSquadDetails
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::OnViewSquadDetails()
{
	WorldData().OnViewSquadDetails((CClub*)m_Grid.GetCurrentRowPointer(1));
}


void CScreen33View::OnViewPlayerBreakdown()
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::OnInitialUpdate
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::OnInitialUpdate()
{
	CGridDivisionSelectorBitmapFormView::OnInitialUpdate(TEXT("Leading Players"), &theApp.GetBasicDisplay(), IDS_SCREEN3301);
	m_Grid.SetRowHeight(20);
	HDC dc = ::GetDC(m_hWnd);
	m_pngSmallKits.DoInitialise(dc, IDR_PNGSMALLHOMEKITS, MAXKITS);
	::ReleaseDC(m_hWnd, dc);
	m_ViewMode = 0;
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGridDivisionSelectorBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			OnSelchangeDivisionlist();
			ReDraw();
			break;

		case VK_RETURN:
		case VK_SELECT:
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::OnDraw
	Access:    	protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::OnPaint()
{
	CGridDivisionSelectorBitmapFormView::OnPaint();
	if (m_Grid.CountRows() == 0)
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
		GetBasicDisplay()->PrintStringID(m_Grid.GetColumnScreenX(3) + 12, TOPY + eUPDOWNHEIGHT + 2, TitleID[m_ViewMode], BLACK, SMALLFONT);
	}
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::OnSetFocus
	Access:    	protected 
	Parameter: 	CWnd * _OldWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::OnSetFocus(CWnd* _OldWnd)
{
	CBitmapFormView::OnSetFocus(_OldWnd);
	OnSelchangeDivisionlist();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::OnSelchangeDivisionlist
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::OnSelchangeDivisionlist()
{
	m_PointScorers.RemoveAll();
	m_GoalScorers.RemoveAll();
	CDivision& theDivision = m_DivisionSelector.GetCurrentDivision(); 
	
	if (theDivision.GetFirstClubInDivision().GetTableData().GetNumberGamesPlayed())
	{
        m_PointScorers.CreateList(theDivision, MAXLISTSIZE);
		m_GoalScorers.DoCreateList(theDivision, MAXLISTSIZE);
	}
	DoBuildGrid();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::SetGridColumnWidths
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::SetGridColumnWidths()
{
	if (m_Grid.CountRows() > 0)
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
		int RowsToDisplay = (GetWindowHeight() - TOPY33 - 2) / m_Grid.GetRowHeight();
		CRect theRect(GetCentreX() - GridWidth, TOPY33, GetCentreX() + GridWidth - m_Grid.GetScrollBarWidth(), TOPY33 + RowsToDisplay * m_Grid.GetRowHeight());
		m_Grid.MoveWindow(&theRect);
		for (int LoopCount = 0; LoopCount < 5; LoopCount++)
		{
			m_Grid.SetColumnWidth(LoopCount, ColumnWidths[ColumnWidthsToUse][LoopCount]);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::DoBuildGrid
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::DoBuildGrid()
{
	m_Grid.RemoveAllData(true);
	CClub* pClub;
	CPlayer* pPlayer;

	m_Grid.DoAddColumn(20, COL_BITMAP);
	m_Grid.DoAddColumn(88);	// Player Name
	m_Grid.DoAddColumn(88);	// Club Name
	m_Grid.DoAddColumn(18, COL_NUMERICR);	// Yellow / League Goals
	m_Grid.DoAddColumn(12, COL_NUMERICR);	// Red / Cup Goals
	m_Grid.DoAddColumn(18, COL_NUMERICR);	// Overall 
	switch(m_ViewMode)
	{
		case 0:
			{
				// Disciplinary
				CLeadingDiscipinaryPoints* pLeadingDiscPlayer;
				m_Grid.DoAddRows(m_PointScorers.GetSize());
				for (int LoopCount = 0; LoopCount < m_PointScorers.GetSize(); LoopCount++)
				{
					pLeadingDiscPlayer = static_cast<CLeadingDiscipinaryPoints*>(m_PointScorers.GetAt(LoopCount));
					pPlayer = WorldData().GetPlayer(pLeadingDiscPlayer->GetPlayerNum());
					pClub = &WorldData().GetClub(pLeadingDiscPlayer->GetClubNum());

					m_Grid.SetRowTextColour(LoopCount, PosnColour[pPlayer->GetSkills().GetPosition()]);
					m_Grid.SetItemBitmap(LoopCount, 0, &m_pngSmallKits, pPlayer->GetClub().GetHomeKitID());
					m_Grid.SetItemText(LoopCount, 1, pPlayer->GetName(), pPlayer);
					m_Grid.SetItemText(LoopCount, 2, pClub->GetName(), pClub);

					m_Grid.SetItemNum(LoopCount, 3, pLeadingDiscPlayer->GetYellow());
					m_Grid.SetItemNum(LoopCount, 4, pLeadingDiscPlayer->GetRed());
					m_Grid.SetItemNum(LoopCount, 5, pLeadingDiscPlayer->GetTotalScored());
				}
			}
			break;

		case 1:
			{
				CLeadingGoalScorer* pLeadingGoalScorer;
				m_Grid.DoAddRows(m_GoalScorers.GetSize());
				for (int LoopCount = 0; LoopCount < m_GoalScorers.GetSize(); LoopCount++)
				{
					pLeadingGoalScorer = static_cast<CLeadingGoalScorer*>(m_GoalScorers.GetAt(LoopCount));
					pPlayer = WorldData().GetPlayer(pLeadingGoalScorer->GetPlayerNum());
					pClub = &WorldData().GetClub(pLeadingGoalScorer->GetClubNum());

					m_Grid.SetRowTextColour(LoopCount, PosnColour[pPlayer->GetSkills().GetPosition()]);
					m_Grid.SetItemBitmap(LoopCount, 0, &m_pngSmallKits, pPlayer->GetClub().GetHomeKitID());
					m_Grid.SetItemText(LoopCount, 1, pPlayer->GetName(), pPlayer);
					m_Grid.SetItemText(LoopCount, 2, pClub->GetName(), pClub);

					m_Grid.SetItemNum(LoopCount, 3, pLeadingGoalScorer->GetLeagueScored());
					m_Grid.SetItemNum(LoopCount, 4, pLeadingGoalScorer->GetCupScored());
					m_Grid.SetItemNum(LoopCount, 5, pLeadingGoalScorer->GetTotalScored());
				}
			}
	}
	SetGridColumnWidths();
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::OnLButtonDown(UINT _Flags, CPoint _Point)
{
	CGridDivisionSelectorBitmapFormView::OnLButtonDown(_Flags, _Point);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT _Flags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::OnLButtonUp(UINT _Flags, CPoint _Point)
{
	int ButtonPressed = GetBasicDisplay()->OnLButtonUp(_Flags, _Point);
	if (m_DivisionSelector.OnLButtonUp(ButtonPressed) == true)
	{
		OnSelchangeDivisionlist();
		ReDraw();
	}
	int iRow = m_Grid.GetCurrentRow(_Point);
	if (iRow != NOTFOUND)
	{
		ushort iColumn = m_Grid.HitTestCol(_Point);
		switch (iColumn)
		{
			case 0:
				OnViewPlayerDetails();
				break;
			case 1:
				OnViewSquadDetails();
				break;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen33View::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen33View::OnSize(UINT nType, int cx, int cy)
{
	CBitmapFormView::OnSize(nType, cx, cy);

	if (IsInitialised() == true)
	{
		CRect theWindow;
		GetClientRect(&theWindow);
		SetGridColumnWidths();

		int DivListY = TOPY;
		m_DivisionSelector.MoveWindow(CRect(GetCentreX() - 55, DivListY, GetCentreX() + 55, DivListY + eUPDOWNHEIGHT));

		Sleep(1);
	}
}