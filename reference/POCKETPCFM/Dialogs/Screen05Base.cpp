

/********************************************************************

	filename: 	D:\PROJECTS\POCKETPCFM\Screen05Base.cpp

	purpose: Squad Display Screen common code

	author:		Roger Womack

	(c) Rw Software 2003 - 2008
		

*********************************************************************/

#include "stdafx.h"

#include "UI.h"
#include "PopupPlayerMenu.h"

#include "Screen05Base.h"


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::CScreen05Base
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen05Base::CScreen05Base(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::~CScreen05Base
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen05Base::~CScreen05Base(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::DoInitialise
	Access:    	public 
	Parameter: 	HWND _hWnd
	Parameter: 	CWnd * _Wnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Base::DoInitialise(HWND _hWnd, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, CGrid* _Grid)
{
	EXT_ASSERT_POINTER(_Grid, CGrid);
	m_Grid = _Grid;
	m_pCurrentClub = WorldData().GetCurrentClub();
	EXT_ASSERT_POINTER(m_pCurrentClub, CClub);
	GetGrid().SetStyle(LC_SWAPPABLECOLUMNS);
	m_bWhichEvents = 0;
	m_HeaderTextRegion.DoInitialise(_BasicDisplay, 0);
	m_HeaderTextRegion.SetFont(_BasicDisplay->GetSmallFont());
	_BasicDisplay->DoAddTextRegion(&m_HeaderTextRegion);
	HDC dc = ::GetDC(_hWnd);
	m_pngPlayerStatus.DoInitialise(dc, IDR_PNGPLAYERSTATUS, MAXPLAYERSTATUSES);
	::ReleaseDC(_hWnd, dc);
	DoInitialiseGrid();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::DoInitialiseGrid
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Base::DoInitialiseGrid()
{
	CGrid& TheGrid = GetGrid();
	TheGrid.DoSaveState();
	TheGrid.RemoveAllData(true);
	TheGrid.DoAddColumn(20);	// Squad number
	TheGrid.DoAddColumn(90);	// Player Name
	switch (m_bWhichEvents)
	{
		case 0: // Skills
			TheGrid.DoAddColumn(GAPX, COL_NUMERICR);	// Overall Skill
			TheGrid.DoAddColumn(GAPX, COL_NUMERICR);	// Handling
			TheGrid.DoAddColumn(GAPX, COL_NUMERICR);	// Tackling
			TheGrid.DoAddColumn(GAPX, COL_NUMERICR);	// Passing
			TheGrid.DoAddColumn(GAPX, COL_NUMERICR);	// Shooting
			break;

		case 1: // Status
			TheGrid.DoAddColumn(GAPX * 6);
			break;

		case 2: // Contract
			TheGrid.DoAddColumn(45, COL_ALIGNR | COL_TEXT);	// Wage
			TheGrid.DoAddColumn(70, COL_ALIGNR | COL_TEXT);	// Contract end date
			break;

		case 3: // Stats
			TheGrid.DoAddColumn(GAPX + 4, COL_NUMERICR);	// Age
			TheGrid.DoAddColumn(GAPX + 4, COL_NUMERICR);	// Appearances
			TheGrid.DoAddColumn(GAPX + 4, COL_NUMERICR);	// Goals
			TheGrid.DoAddColumn(GAPX + 4, COL_NUMERICR);	// Discipline
			break;

		default:
			EXT_ASSERT(false);
			break;
	}
	int Count = m_pCurrentClub->GetFirstTeamSquad().TotalSquadSize();
	TheGrid.DoAddRows(Count);

	COLORREF ColourToUse;
	COLORREF ColourStripe[] =
	{
		// Team
		PALEGREEN, VPALEGREEN, PALEGREEN, VPALEGREEN,
		PALEGREEN, VPALEGREEN, PALEGREEN, VPALEGREEN,
		PALEGREEN, VPALEGREEN, PALEGREEN,
		// Substitutes
		PALEYELLOW, VPALEYELLOW, PALEYELLOW, VPALEYELLOW, PALEYELLOW,
		// Squad
		PALEBLUE, VPALEBLUE, PALEBLUE, VPALEBLUE,
		PALEBLUE, VPALEBLUE, PALEBLUE, VPALEBLUE,
		PALEBLUE, VPALEBLUE, PALEBLUE, VPALEBLUE,
		PALEBLUE, VPALEBLUE,
	};

	CPlayer* pPlayer;
	for (int LoopCount = 0; LoopCount < Count; LoopCount++)
	{
		if (m_pCurrentClub->GetPlayerID(LoopCount) == NOPLAYER)
		{
			ASSERT(false);
			continue;
		}
		pPlayer = &m_pCurrentClub->GetPlayer(LoopCount);
		ColourToUse = ColourStripe[LoopCount];
		if (pPlayer->IsAvailableForMatch() == false)
		{
			// Unavailable
			ColourToUse = PALERED;
			TheGrid.SetItemBitmap(LoopCount, 0, &m_pngPlayerStatus, (int)pPlayer->GetSkillViewStatus());
		}
		else if (LoopCount < PLAYERSINTEAM + 5)
		{
			TheGrid.SetItemNum(LoopCount, 0, LoopCount + 1);
		}
		TheGrid.SetRowPointer(LoopCount, pPlayer);
		TheGrid.SetRowBackgroundColour(LoopCount, ColourToUse);

		if (LoopCount < PLAYERSINTEAM)
		{
			if (LoopCount == 0)
			{
				ColourToUse = PosnColour[GOALKEEPER];
			}
			else
			{
				ColourToUse = PosnColour[WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTactics().PlayerPosition(LoopCount - 1)];
			}
		}
		else
		{
			ColourToUse = BLACK;
		}
		TheGrid.SetItemTextColour(LoopCount, 0, ColourToUse);

		TheGrid.SetItemTextColour(LoopCount, 1, PosnColour[pPlayer->GetSkills().GetPosition()]);
		TheGrid.SetItemText(LoopCount, 1, pPlayer->GetName());

		switch(m_bWhichEvents)
		{
			case 0:
				TheGrid.SetItemNum(LoopCount, 2, pPlayer->GetSkills().GetOverallSkill());
				TheGrid.SetItemNum(LoopCount, 3, pPlayer->GetSkills().GetHandling());
				TheGrid.SetItemNum(LoopCount, 4, pPlayer->GetSkills().GetTackling());
				TheGrid.SetItemNum(LoopCount, 5, pPlayer->GetSkills().GetPassing());
				TheGrid.SetItemNum(LoopCount, 6, pPlayer->GetSkills().GetShooting());
				break;

			case 1:
				TheGrid.SetItemText(LoopCount, 2, pPlayer->MatchAvailabilityStr());
				break;

			case 2:
				TheGrid.SetItemText(LoopCount, 2, WorldData().GetCurrentUserClub()->GetForeignStr(pPlayer->GetWage()));
				TheGrid.SetItemText(LoopCount, 3, pPlayer->ContractShortStr());
				break;

			case 3:
				TheGrid.SetItemNum(LoopCount, 2, pPlayer->GetAge());
				TheGrid.SetItemNum(LoopCount, 3, pPlayer->GetNumberAppearancesThisSeason());
				TheGrid.SetItemNum(LoopCount, 4, pPlayer->GetNumberGoalsThisSeason());
				TheGrid.SetItemNum(LoopCount, 5, pPlayer->GetDiscipline().GetDisciplinaryPoints());
				break;

			default:
				EXT_ASSERT(false);
				break;
		}
	}
	int TitleID[] =
	{
		IDS_SCREENHEADER0501, IDS_SCREENHEADER0502,
		IDS_SCREENHEADER0503, IDS_SCREENHEADER0504
	};
	m_HeaderTextRegion.SetStrId(TitleID[m_bWhichEvents]);
	TheGrid.DoRestoreState();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::GetGrid
	Access:    	public 
	Returns:   	CGrid&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CGrid& CScreen05Base::GetGrid()
{
	EXT_ASSERT_POINTER(m_Grid, CGrid);
	return *m_Grid;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::OnDraw
	Access:    	public 
	Parameter: 	CBasicDisplay * _BasicDisplay
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Base::OnDraw(CBasicDisplay* _BasicDisplay)
{
	GetGrid().OnPaint();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Base::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	GetGrid().OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_SELECT:
		case VK_F23:
		case VK_RETURN:
			if (GetGrid().GetCurrentRow() >= 0)
			{
				OnViewPlayer();
			}
			else
			{
				GetGrid().DoSwapClickedRows();
			}
			break;
	}
	DoSwapPlayers();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::OnViewPlayer
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Base::OnViewPlayer()
{
	if (GetGrid().GetCurrentRowPointer() != NULL)
	{
		WorldData().OnViewPlayerDetails((CPlayer*)GetGrid().GetCurrentRowPointer());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::OnLButtonDown
	Access:    	public 
	Parameter: 	HWND _hWnd
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Base::OnLButtonDown(HWND _hWnd, UINT nFlags, CPoint point)
{
	SHRGINFO shrgi = {0};
	shrgi.cbSize = sizeof(SHRGINFO);
	shrgi.hwndClient = _hWnd;
	shrgi.ptDown.x = point.x;
	shrgi.ptDown.y = point.y;
	shrgi.dwFlags = SHRG_RETURNCMD;

	if (GetGrid().GetCurrentRow(point) != NOTFOUND && GN_CONTEXTMENU == ::SHRecognizeGesture(&shrgi))
	{
		CPoint point(GetMessagePos());
		point.y -= 26;
		OnPlayerPopupMenu(GetGrid().GetCurrentRow(point), point);
	}
	else
	{
		DoSwapPlayers();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::DoSwapPlayers
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Base::DoSwapPlayers()
{
	if (GetGrid().IsSwappableRows() && m_pCurrentClub == WorldData().GetCurrentUserClub())
	{
		if (GetGrid().IsSwapRows() == true)
		{
			DEBUG_OUTPUT(("Swapping %d & %d\n"), (GetGrid().GetLastSelectedItem(), GetGrid().GetClickedItem()));
			m_pCurrentClub->GetFirstTeamSquad().DoSwapPlayers(GetGrid().GetLastSelectedItem(), GetGrid().GetClickedItem());
			DoInitialiseGrid();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::OnPlayerPopupMenu
	Access:    	public 
	Parameter: 	UINT iColumn
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Base::OnPlayerPopupMenu(UINT iColumn, CPoint point)
{
	CPopupPlayerMenu TheMenu;
	TheMenu.DoModal(&m_pCurrentClub->GetPlayer(iColumn), point);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::OnSelchangeTab
	Access:    	public 
	Parameter: 	int _CurTab
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Base::OnSelchangeTab(int _CurTab)
{
	m_bWhichEvents = _CurTab;
//	int CurrSel = GetGrid().GetVerticalScroll();
	DoInitialiseGrid();
//	GetGrid().SetVerticalScroll(CurSel);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::OnVScroll
	Access:    	public 
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Parameter: 	CScrollBar * _ScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Base::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar)
{
	GetGrid().OnVScroll(_SBCode, _Pos, _ScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::OnSize
	Access:    	public 
	Parameter: 	CRect & theWindow
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Base::OnSize(CRect& theWindow)
{
	int RowsToDisplay = (theWindow.bottom - 56 - 2) / GetGrid().GetRowHeight();
	CRect theRect(theWindow.right / 2 - 118, 56, theWindow.right / 2 + 118 - GetGrid().GetScrollBarWidth(), 56 + RowsToDisplay * GetGrid().GetRowHeight());	
	GetGrid().MoveWindow(theRect);

	theWindow.bottom = theRect.bottom;
	m_HeaderTextRegion.MoveWindow(CRect(theWindow.right / 2 + 2, TOPY - 4, theWindow.right, 55));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05Base::SetGrid
	Access:    	public 
	Parameter: 	CGrid * _Grid
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05Base::SetGrid(CGrid* _Grid)
{
	EXT_ASSERT_POINTER(_Grid, CGrid);
	m_Grid = _Grid;
}
