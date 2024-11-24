
 /*

	SoccerResultGrid.cpp : CSoccerResultGrid implementation file

	Football Director

	(c) Sports Director 2007

  ---------------------------------------------------------------------------------------  
 */

#include "stdafx.h"

#include "UI.h"
#include "SoccerResultGrid.h"


/*------------------------------------------------------------------------------
	Method:   	CSoccerResultGrid::CSoccerResultGrid
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoccerResultGrid::CSoccerResultGrid(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResultGrid::~CSoccerResultGrid
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoccerResultGrid::~CSoccerResultGrid(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResultGrid::OnInitialUpdate
	Access:    	public 
	Parameter: 	CWnd * _ParentWnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerResultGrid::OnInitialUpdate(CWnd* _ParentWnd, HWND _hWnd, CBasicDisplay* _BasicDisplay)
{
	CGrid::OnInitialUpdate(_ParentWnd, _BasicDisplay);
	CGrid::DoLoadBitmapBars(_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
	CGrid::HideHilite();
	CGrid::DoAddColumn(1, COL_TEXT | COL_ALIGNR);		// Home team
	CGrid::DoAddColumn(1, COL_TEXT | COL_ALIGNC);		// Result
	CGrid::DoAddColumn(1, COL_TEXT | COL_ALIGNL);		// Away team
	CGrid::DoLoadBitmapBars(_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
	CGrid::DoAddRows(GetFixtures().GetSize());
	CGrid::ShowWindow(SW_HIDE);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResultGrid::DoBuild
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerResultGrid::DoBuild(const int _CurrentTime)
{
	int Width = CGrid::GetWidth();
	CGrid::SetColumnWidthPercentage(0, 43);		// Home team
	CGrid::SetColumnWidthPercentage(1, 14);		// Result
	CGrid::SetColumnWidthPercentage(2, 43);		// Away team

	for (int LoopCount = 0; LoopCount < m_Fixtures.GetSize(); LoopCount++)
	{
		// Highlight the user's result
		if (m_Fixtures[LoopCount].IsUserMatch() == true)
		{
			CGrid::SetRowTextColour(LoopCount, WHITE);
		}
		else 
		{
			CGrid::SetRowTextColour(LoopCount, BLACK);
		}
		CGrid::SetItemText(LoopCount, 0, m_Fixtures[LoopCount].GetClubName(HOME));
		CGrid::SetItemText(LoopCount, 1, m_Fixtures[LoopCount].GoalsStr(_CurrentTime));
		CGrid::SetItemText(LoopCount, 2, m_Fixtures[LoopCount].GetClubName(AWAY));
	}
	CGrid::DoSortLow(0);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerResultGrid::GetFixtures
	Access:    	public 
	Returns:   	CFixtureList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFixtureList& CSoccerResultGrid::GetFixtures()
{
	return m_Fixtures;
}
