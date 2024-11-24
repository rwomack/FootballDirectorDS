
 /*

	Screen50View.cpp : League Tables CFormView implementation file

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
 */


#include "stdafx.h"

#include "UI.h"
#include "CurrentForm.h"
#include "PopupPlayerMenu.h"
#include "IndexedIntArray.h"
#include "Form.h"
#include "Screen50View.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CScreen50View

IMPLEMENT_DYNCREATE(CScreen50View, CFormView)


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::CScreen50View
	Access:    	public 
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen50View::CScreen50View()
	: CGridDivisionSelectorBitmapFormView(CScreen50View::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::~CScreen50View
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen50View::~CScreen50View()
{
}


BEGIN_MESSAGE_MAP(CScreen50View, CGridDivisionSelectorBitmapFormView)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()

	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)
	ON_COMMAND(ID_ACTION3, OnOverall)
	ON_COMMAND(ID_ACTION4, OnHome)
	ON_COMMAND(ID_ACTION5, OnAway)
	ON_COMMAND(ID_ACTION6, OnForm)
	ON_COMMAND(ID_ACTION7, OnDiscipline)
	ON_COMMAND(ID_ACTION8, OnViewSquadDetails)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen50View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::OnActionClicked
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::OnActionClicked()
{
	CBitmapFormView::OnActionMenu(IDR_SCREEN50MENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT6);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::OnViewSquadDetails
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::OnViewSquadDetails()
{
	WorldData().OnViewSquadDetails((CClub*)m_Grid.GetCurrentRowPointer());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::OnOverall
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::OnOverall()
{
	m_ViewMode = 0;
	CBitmapFormView::GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN50);
	DoBuildGrid();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::OnHome
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::OnHome()
{
	m_ViewMode = 1;
	CBitmapFormView::GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN5001);
	DoBuildGrid();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::OnAway
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::OnAway()
{
	m_ViewMode = 2;
	CBitmapFormView::GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN5002);
	DoBuildGrid();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::OnForm
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::OnForm()
{
	m_ViewMode = 3;
	CBitmapFormView::GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN5003);
	DoBuildGrid();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::OnDiscipline
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::OnDiscipline()
{
	m_ViewMode = 4;
	CBitmapFormView::GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN5004);
	DoBuildGrid();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::OnInitialUpdate
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::OnInitialUpdate()
{
	CGridDivisionSelectorBitmapFormView::OnInitialUpdate(TEXT("League Tables"), &theApp.GetBasicDisplay(), IDS_SCREEN50);
	m_Grid.SetHiliteBackgroundColour(TRANSCOLOUR);
	m_ViewMode = 0;
	OnSelchangeDivisionlist();
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			CGridDivisionSelectorBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
			OnSelchangeDivisionlist();
			ReDraw();
			break;

		case VK_SELECT:
		case VK_RETURN:
			{
				CPopupClubMenu TheMenu;
				TheMenu.DoModal((CClub*)m_Grid.GetCurrentRowPointer(), CPoint(100, 100));
			}
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::DoBuildGrid
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::DoBuildGrid()
{
	m_Grid.RemoveAllData(true);

	if (m_ViewMode < 4)
	{
		// Overall, home, away and current form
		m_Grid.DoAddColumn(m_ClubNameLength);	// Club Name
		m_Grid.DoAddColumn(18, COL_NUMERICR);	// Played
		m_Grid.DoAddColumn(18, COL_NUMERICR);	// Won
		m_Grid.DoAddColumn(18, COL_NUMERICR);	// Drawn
		m_Grid.DoAddColumn(18, COL_NUMERICR);	// Lost
		m_Grid.DoAddColumn(26, COL_NUMERICR);	// For
		m_Grid.DoAddColumn(26, COL_NUMERICR);	// Against
		m_Grid.DoAddColumn(26, COL_NUMERICR);	// Points
	}
	else
	{
		// Disciplinary
		m_Grid.DoAddColumn(124);				// Club Name
		m_Grid.DoAddColumn(34, COL_NUMERICR);	// Red
		m_Grid.DoAddColumn(32, COL_NUMERICR);	// Yellow
		m_Grid.DoAddColumn(32, COL_NUMERICR);	// Overall

	}
	const COLORREF TextColor[] =
	{
		TRANSCOLOUR,	// Normal
		PALERED,		// Relegation spot
		PALEBLUE,		// Playoff spot
		PALEGREEN		// Promotion spot
	};

	CDivision& pDivision = m_DivisionSelector.GetCurrentDivision();
	m_Grid.DoAddRows(pDivision.GetNumberOfClubsInDivision());
	CClub* pClub;
	if (m_ViewMode < 3)
	{
		// Overall, home and away
		pDivision.SortLeagueTable(m_ViewMode);
		for (int LoopCount = 0; LoopCount < pDivision.GetNumberOfClubsInDivision(); LoopCount++)
		{
			pClub = &pDivision.DoFindClubInTablePosition(LoopCount);
			m_Grid.SetItemText(LoopCount, 0, pClub->GetName());
			m_Grid.SetRowBackgroundColour(LoopCount, TextColor[pDivision.LeaguePositionStatus(LoopCount)]);
			m_Grid.SetRowPointerDetails(LoopCount, pClub);

			switch(m_ViewMode)
			{
				case 0:
/*#ifdef TEST_DIALOGS
					m_Grid.SetItemNum(LoopCount, 1, 99);
					m_Grid.SetItemNum(LoopCount, 2, 99);
					m_Grid.SetItemNum(LoopCount, 3, 99);
					m_Grid.SetItemNum(LoopCount, 4, 99);
					m_Grid.SetItemNum(LoopCount, 5, 199);
					m_Grid.SetItemNum(LoopCount, 6, 199);
					m_Grid.SetItemNum(LoopCount, 7, 199);
#else*/
					pClub->DoBuildLeagueTableRow(LoopCount, m_Grid);
//#endif
					break;
				case 1:
					m_Grid.SetItemNum(LoopCount, 1, pClub->GetTableData().GetHomeGames()->GetNumberGamesPlayed());
					m_Grid.SetItemNum(LoopCount, 2, pClub->GetTableData().GetHomeGames()->GetNumberGamesWon());
					m_Grid.SetItemNum(LoopCount, 3, pClub->GetTableData().GetHomeGames()->GetNumberGamesDrawn());
					m_Grid.SetItemNum(LoopCount, 4, pClub->GetTableData().GetHomeGames()->GetNumberGamesLost());
					m_Grid.SetItemNum(LoopCount, 5, pClub->GetTableData().GetHomeGames()->GetNumberGoalsScored());
					m_Grid.SetItemNum(LoopCount, 6, pClub->GetTableData().GetHomeGames()->GetNumberGoalsConceded());
					m_Grid.SetItemNum(LoopCount, 7, pClub->GetTableData().GetHomeGames()->GetPoints());
					break;

				case 2:
					m_Grid.SetItemNum(LoopCount, 1, pClub->GetTableData().GetAwayGames()->GetNumberGamesPlayed());
					m_Grid.SetItemNum(LoopCount, 2, pClub->GetTableData().GetAwayGames()->GetNumberGamesWon());
					m_Grid.SetItemNum(LoopCount, 3, pClub->GetTableData().GetAwayGames()->GetNumberGamesDrawn());
					m_Grid.SetItemNum(LoopCount, 4, pClub->GetTableData().GetAwayGames()->GetNumberGamesLost());
					m_Grid.SetItemNum(LoopCount, 5, pClub->GetTableData().GetAwayGames()->GetNumberGoalsScored());
					m_Grid.SetItemNum(LoopCount, 6, pClub->GetTableData().GetAwayGames()->GetNumberGoalsConceded());
					m_Grid.SetItemNum(LoopCount, 7, pClub->GetTableData().GetAwayGames()->GetPoints());
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
				for (int LoopCount = 0; LoopCount < m_Grid.GetNumRowsOnScreen(); LoopCount++)
				{
					pClub = &pDivision.FindClubNumberInCurrentFormPosition(LoopCount + m_Grid.GetVerticalScroll());
					m_Grid.SetItemText(LoopCount, 0, pClub->GetName());

					m_Grid.SetItemNum(LoopCount, 1, iGamesPlayed);
					m_Grid.SetItemNum(LoopCount, 2, pClub->GetCurrentForm()->GetCurrentFormWon());
					m_Grid.SetItemNum(LoopCount, 3, pClub->GetCurrentForm()->GetCurrentFormDrawn());
					m_Grid.SetItemNum(LoopCount, 4, pClub->GetCurrentForm()->GetCurrentFormLost());
					m_Grid.SetItemNum(LoopCount, 5, pClub->GetCurrentForm()->GetCurrentFormScored());
					m_Grid.SetItemNum(LoopCount, 6, pClub->GetCurrentForm()->GetCurrentFormConceded());
					m_Grid.SetItemNum(LoopCount, 7, pClub->GetCurrentForm()->GetCurrentFormPts());
				}
				break;
		case 4:
			for (int LoopCount = 0; LoopCount < m_Grid.GetNumRowsOnScreen(); LoopCount++)
			{
				pClub = &WorldData().GetClub(m_FairPlayList.GetAt(LoopCount + m_Grid.GetVerticalScroll())->GetIndex());
				m_Grid.SetItemText(LoopCount, 0, pClub->GetName());

				m_Grid.SetItemNum(LoopCount, 1, pClub->GetSeasonStats().GetSeasonBookings());
				m_Grid.SetItemNum(LoopCount, 2, pClub->GetSeasonStats().GetSeasonSentOff ());
				m_Grid.SetItemNum(LoopCount, 3, pClub->GetSeasonStats().GetSeasonDisciplinaryPoints());
			}
			break;
		}
	}
	CGridDivisionSelectorBitmapFormView::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::OnPaint
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::OnPaint()
{
	CGridDivisionSelectorBitmapFormView::OnPaint();
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::OnSelchangeDivisionlist
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::OnSelchangeDivisionlist()
{
	CDivision& theDivision = m_DivisionSelector.GetCurrentDivision();
	theDivision.SortCurrentForm();

	m_FairPlayList.RemoveAll();
	for (int LoopCount = 0; LoopCount < theDivision.GetNumberOfClubsInDivision(); LoopCount++)
	{
		m_FairPlayList.AddNew(LoopCount + theDivision.GetFirstClubNumberInDivision(), theDivision.GetClub(LoopCount).GetSeasonStats().GetSeasonDisciplinaryPoints());
	}
	m_FairPlayList.SortHigh();
	DoBuildGrid();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT _Flags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::OnLButtonDown(UINT _Flags, CPoint _Point)
{
	bool Found = false;
	int iColumn = m_Grid.GetCurrentRow(_Point);
	if (iColumn == NOTFOUND)
	{
		CGridDivisionSelectorBitmapFormView::OnLButtonDown(_Flags, _Point);
	}
	else
	{
		CDivision& theDivision = m_DivisionSelector.GetCurrentDivision();
		CClub* pClub;
		switch(m_ViewMode)
		{
			case 0:
				pClub = &theDivision.DoFindClubInTablePosition(iColumn);
				break;
			case 1:
				pClub = &theDivision.DoFindClubInTablePosition(iColumn);
				break;
			case 2:
				pClub = &theDivision.DoFindClubInTablePosition(iColumn);
				break;
			case 3:
				pClub = &theDivision.FindClubNumberInCurrentFormPosition(iColumn);
				break;
			case 4:
				pClub = &WorldData().GetClub(m_FairPlayList.GetAt(iColumn)->GetIndex());
				break;
		}

		int WhichColumn = m_Grid.HitTestCol(_Point);
		if (WhichColumn == 0)
		{
			// Clicked on club 
			CPopupClubMenu TheMenu;
			TheMenu.DoModal(pClub, _Point);
		}
		else
		{
			WhichColumn--; // make 1st data column zero index
			eClubResultsType eResultToShow = MATCHES_NONE;
			switch(m_ViewMode)
			{
				case 0:		// All Games
				{
					eClubResultsType eResultsType[] =
					{
						MATCHES_PLAYED, 
						MATCHES_WON, MATCHES_DRAWN, MATCHES_LOST,
						MATCHES_SCORED, MATCHES_CONCEDED
					};
					eResultToShow = eResultsType[WhichColumn];
					break;
				}

				case 1:	// Home Games
				{
					eClubResultsType eResultsType[] =
					{
						MATCHES_HOME_PLAYED, 
						MATCHES_HOME_WON, MATCHES_HOME_DRAWN, MATCHES_HOME_LOST,
						MATCHES_HOME_SCORED, MATCHES_HOME_CONCEDED,
					};
					eResultToShow = eResultsType[WhichColumn];
					break;
				}
				
				case 2:		// Away Games
				{
					eClubResultsType eResultsType[] =
					{
						MATCHES_AWAY_PLAYED,
						MATCHES_AWAY_WON, MATCHES_AWAY_DRAWN, MATCHES_AWAY_LOST,
						MATCHES_AWAY_SCORED, MATCHES_AWAY_CONCEDED,
					};
					eResultToShow = eResultsType[WhichColumn];
					break;
				}

				case 3:		// Last 6 Games
				{
					eClubResultsType eResultsType[] =
					{
						MATCHES_FORM_PLAYED, 
						MATCHES_FORM_WON, MATCHES_FORM_DRAWN, MATCHES_FORM_LOST,
						MATCHES_FORM_SCORED, MATCHES_FORM_CONCEDED,
					};
					eResultToShow = eResultsType[WhichColumn];
					break;
				}

				case 4:		// Fair play tables
					break;

				default:
					EXT_ASSERT(false);
					break;
			};
			if (eResultToShow != MATCHES_NONE)
			{
				theApp.OnViewClubFixtures(pClub, eResultToShow);
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::OnLButtonUp
	Access:    	public 
	Parameter: 	UINT _Flags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::OnLButtonUp(UINT _Flags, CPoint _Point)
{
	int ButtonPressed = GetBasicDisplay()->OnLButtonUp(_Flags, _Point);
	if (m_DivisionSelector.OnLButtonUp(ButtonPressed) == true)
	{
		OnSelchangeDivisionlist();
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen50View::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen50View::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);

		CRect theWindow;
		GetClientRect(&theWindow);
		m_DivisionSelector.MoveWindow(CRect(GetCentreX() - 55, TOPY, GetCentreX() + 55, TOPY + TABHEIGHT));

		int GridWidth = 118;
		m_ClubNameLength = 74;
		if (m_ViewMode < 4 && theWindow.right > 300)
		{
			// Overall, home, away and current form
			m_ClubNameLength = 114;	// Club Name
			GridWidth += 20;
		}
		m_Grid.SetColumnWidth(0, m_ClubNameLength);	// Club Name
		int RowsToDisplay = (theWindow.bottom - TOPY - 2 - TABHEIGHT) / m_Grid.GetRowHeight();
		CRect theRect(GetCentreX() - GridWidth, DIVISIONSELECTOR_TOPY, GetCentreX() + GridWidth - m_Grid.GetScrollBarWidth(), DIVISIONSELECTOR_TOPY + RowsToDisplay * m_Grid.GetRowHeight());	
		m_Grid.MoveWindow(theRect);
		Sleep(1);
	}
}