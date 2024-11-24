/*

	ClubFixtureGrid.cpp: implementation for the ClubFixtureGrid classes.


	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "ClubFixtureGrid.h"

#include "MatchInfo.h"
#include "PopupPlayerMenu.h"
#include "FootballDirector.h"



/*------------------------------------------------------------------------------
	Method:   	CClubFixtureGrid::CClubFixtureGrid
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubFixtureGrid::CClubFixtureGrid(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CClubFixtureGrid::~CClubFixtureGrid
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubFixtureGrid::~CClubFixtureGrid(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CClubFixtureGrid::DoShowMatchSummary
	Access:    	public 
	Parameter: 	const ushort _Row
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubFixtureGrid::DoShowMatchSummary(const ushort _Row)
{
	CFixture* pFixture = (CFixture*)(CGrid::GetRowPointer(_Row));
	if (pFixture != NULL && pFixture->HasMatchBeenPlayed() == true)
	{
		pFixture->DoShowSummary(NULL);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubFixtureGrid::DoShowOpponentSquad
	Access:    	public 
	Parameter: 	const ushort _Row
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubFixtureGrid::DoShowOpponentSquad(const ushort _Row)
{
	CFixture* pFixture = (CFixture*)(CGrid::GetRowPointer(_Row));
	if (pFixture != NULL)
	{
		CClub* pClub = &pFixture->GetClub(HOME);
		if (pClub == m_pCurrentClub)
		{
			pClub = &pFixture->GetClub(AWAY);
		}
		theApp.OnViewSquadDetails(pClub);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubFixtureGrid::MoveWindow
	Access:    	public 
	Parameter: 	CRect & _ClientRect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubFixtureGrid::MoveWindow(CRect& _ClientRect)
{
	int RowsToDisplay = (_ClientRect.bottom - _ClientRect.top) / CGrid::GetRowHeight();
	int CentreX = _ClientRect.right / 2;
	CGrid::MoveWindow(CRect(CentreX - 118, _ClientRect.top, CentreX + 118 - CGrid::GetScrollBarWidth(), _ClientRect.top + RowsToDisplay * CGrid::GetRowHeight()));
}


/*------------------------------------------------------------------------------
	Method:   	CClubFixtureGrid::OnInitialUpdate
	Access:    	public 
	Parameter: 	CWnd * _ParentWnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	CClub * _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubFixtureGrid::OnInitialUpdate(CWnd* _ParentWnd, CBasicDisplay* _BasicDisplay, CClub* _Club)
{
	EXT_ASSERT_POINTER(_Club, CClub);
	m_pCurrentClub = _Club;
	CGrid::OnInitialUpdate(_ParentWnd, _BasicDisplay);
	CGrid::DoLoadBitmapBars(_ParentWnd->m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
	HDC dc = ::GetDC(_ParentWnd->m_hWnd);
	m_pngSmallKits.DoInitialise(dc, IDR_PNGSMALLHOMEKITS, MAXKITS);
	::ReleaseDC(_ParentWnd->m_hWnd, dc);
	CGrid::SetRowHeight(20);
	CGrid::DoAddColumn(46, COL_TEXT | COL_ALIGNR);		// Date
	CGrid::DoAddColumn(14, COL_TEXT | COL_ALIGNC);		// Venue
	CGrid::DoAddColumn(20, COL_BITMAP);					// Opponent Shirt
	CGrid::DoAddColumn(96, COL_TEXT | COL_ALIGNL);		// Opponent Name
	CGrid::DoAddColumn(26, COL_TEXT | COL_ALIGNC);		// Result
	CGrid::DoAddColumn(20, COL_NUMERICR);				// League position
	CGrid::SetHiliteBackgroundColour(TRANSCOLOUR);

	int CurrentRow;
	CCalendar TheDate(WorldData().GetCurrentDate());
	m_pCurrentClub->GetDivision().DoFindStartOfSeason(&TheDate);
	CMatchInfo MatchDetails;
	do 
	{
		if (m_pCurrentClub->IsMatchToday(TheDate, MatchDetails) == true)
		{
			CurrentRow = CGrid::DoAddRow();
			CGrid::SetItemText(CurrentRow, 0, TheDate.ShortDateStr());
			CGrid::SetItemBitmap(CurrentRow, 2, &m_pngSmallKits, MatchDetails.GetOpponentClub(m_pCurrentClub)->GetHomeKitID());
			if (MatchDetails.IsHomeClub(m_pCurrentClub) == true)
			{
				CGrid::SetRowTextColour(CurrentRow, BLACK);
				CGrid::SetItemStrId(CurrentRow, 1, IDS_HOMEOPPONENTS);
				CGrid::SetItemText(CurrentRow, 3, MatchDetails.GetAwayName().MakeUpper(), MatchDetails.GetOpponentClub(m_pCurrentClub));
			}
			else
			{
				CGrid::SetRowTextColour(CurrentRow, BLUE);
				CGrid::SetItemStrId(CurrentRow, 1, IDS_AWAYOPPONENTS);
				CGrid::SetItemText(CurrentRow, 3, MatchDetails.GetHomeName(), MatchDetails.GetOpponentClub(m_pCurrentClub));
			}

			// Display the result if it's been played
			if (MatchDetails.GetFixture() != null && MatchDetails.GetFixture()->HasMatchBeenPlayed() == true)
			{
				CGrid::SetRowPointer(CurrentRow, MatchDetails.GetFixture());
				CString strPrint;
				strPrint.Format(TEXT("%d-%d"), MatchDetails.GetFixture()->GetNumberGoals(HOME),
					MatchDetails.GetFixture()->GetNumberGoals(AWAY));
				CGrid::SetItemText(CurrentRow, 4, strPrint);
				CGrid::SetItemNum(CurrentRow, 5, m_pCurrentClub->GetLeaguePositionAfterMatch()->GetAt(CurrentRow) + 1);
			}
		}
		TheDate.NextMatchDay();
	} 
	while (m_pCurrentClub->GetDivision().GetMatchType(TheDate.GetMatchDay()) != DIVENDSEASON);
}


/*------------------------------------------------------------------------------
	Method:   	CClubFixtureGrid::OnLButtonUp
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubFixtureGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (CGrid::GetCurrentRow(point) != NOTFOUND)
	{
		ushort iRow = CGrid::GetCurrentRow(point);
		DoShowMatchSummary(iRow);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubFixtureGrid::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubFixtureGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGrid::OnKeyDown(nChar, nRepCnt, nFlags);
	if (nChar == VK_SELECT || nChar == VK_RETURN || nChar == VK_F23)
	{
		DoShowMatchSummary(CGrid::GetCurrentRow());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubFixtureGrid::SetMatchType
	Access:    	public 
	Parameter: 	eClubResultsType _eMatchType
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubFixtureGrid::SetMatchType(eClubResultsType _eMatchType)
{
	m_eMatchType = _eMatchType;
}
