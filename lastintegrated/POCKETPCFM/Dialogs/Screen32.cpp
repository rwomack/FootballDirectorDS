
/*

	Screen32.cpp : Match Intro implementation file
	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "MessageDisplay.h"
#include "UI.h"
#include "Dialog.h"
#include "PopupPlayerMenu.h"
#include "HumanManager.h"
#include "Screen32.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen32Dlg dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::CScreen32Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen32Dlg::CScreen32Dlg(CWnd* pParent /*=null*/)
	: CBitmapDialog(CScreen32Dlg::IDD, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::~CScreen32Dlg
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen32Dlg::~CScreen32Dlg()
{
}


BEGIN_MESSAGE_MAP(CScreen32Dlg, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_ACTION_VIEWMATCH, OnOK)
	ON_COMMAND(ID_ACTION_RESULTONLY, OnResultOnly)
	ON_COMMAND(ID_ACTION_MATCHINTRO, OnMatchIntro)
	ON_COMMAND(ID_ACTION_LINEUPS, OnMatchLineups)
	ON_COMMAND(ID_ACTION_OPPOSITIONREPORT, OnOppositionReport)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen32 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::OnVScroll
	Access:    	protected 
	Parameter: 	UINT nSBCode
	Parameter: 	UINT nPos
	Parameter: 	CScrollBar * pScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen32Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch(m_CurrentView)
	{
		case LINEUP_VIEW:
			// Lineups
			m_Grid.OnVScroll(nSBCode, nPos, pScrollBar);
			ReDraw();
			break;

		case REPORT_VIEW:
			// Match preview
			if (m_MessageDisplay.OnVScroll(nSBCode, nPos, pScrollBar) == true)
			{
				ReDraw();
			}
		break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen32Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	switch(m_CurrentView)
	{
		case LINEUP_VIEW:
			// Lineups
			m_Grid.OnKeyDown(nChar, nRepCnt, nFlags);
			ReDraw();
			break;

		case REPORT_VIEW:
			// Match preview
			m_MessageDisplay.OnKeyDown(nChar, nRepCnt, nFlags);
			break;
	}
}




/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen32Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN32, IDS_SCREEN32);

	m_Grid.OnInitialUpdate(this, GetBasicDisplay(), LC_SHOWCOLUMNS | LC_NOHIGHLIGHT);
	m_Grid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);

	m_Grid.RemoveAllData(true);
	m_Grid.DoAddColumn(100, COL_TEXT | COL_ALIGNR);	// Home Player Name
	m_Grid.DoAddColumn(20, COL_NUMERICR);				// Squad number
	m_Grid.DoAddColumn(100, COL_TEXT | COL_ALIGNL);	// Away Player Name

	int NumRows = PLAYERSINTEAM + WorldData().GetMatchHandler().GetMatchPlay().GetFixture().GetSubsSelect();
	m_Grid.DoAddRows(NumRows);

	CPlayer* pPlayer;
	CString strPrint;
	for (int LoopCount = 0; LoopCount < NumRows; LoopCount++)
	{
		pPlayer = &WorldData().GetMatchHandler().GetMatchPlay().GetPlayer(HOME, LoopCount);
		m_Grid.SetItemTextColour(LoopCount, 0, PosnColour[pPlayer->GetSkills().GetPosition()]);
		m_Grid.SetItemText(LoopCount, 0, pPlayer->GetName(), pPlayer, PLAYER_POINTER);
		
		m_Grid.SetItemNum(LoopCount, 1, LoopCount + 1);
		m_Grid.SetItemTextColour(LoopCount, 1, BLACK);

		pPlayer = &WorldData().GetMatchHandler().GetMatchPlay().GetPlayer(AWAY, LoopCount);
		m_Grid.SetItemTextColour(LoopCount, 2, PosnColour[pPlayer->GetSkills().GetPosition()]);
		m_Grid.SetItemText(LoopCount, 2, pPlayer->GetName(), pPlayer, PLAYER_POINTER);
	}

	// Setup message display
	m_MessageDisplay.SetDrawInfo(this, 0x1000, CRect(4, 68, 236, 240));

#ifndef TEST_MATCH_REPORT
	if (WorldData().GetCurrentUserClub()->DoCountLeagueGamesPlayedThisSeason() > 1)
#endif
	{
		WorldData().GetCurrentUser()->DoPreMatchReport(&m_MessageDisplay);
	}
	else
	{

	}

	HDC dc = ::GetDC(m_hWnd);
	m_pngLargeKits.DoInitialise(dc, IDR_PNGLARGEHOMEKITS, MAXKITS);
	::ReleaseDC(m_hWnd, dc);

	OnMatchIntro();
	CBitmapDialog::DoDisableOKButton();
	CBitmapDialog::LoadMenu(IDR_SCREEN32MENU);
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen32Dlg::OnPaint()
{
	CBitmapDialog::OnPaint();
	COffScreenBuffer& memDC = *CBitmapDialog::GetMemDC();
	switch(m_CurrentView)
	{
		case INTRO_VIEW:
			DisplayMatchDetails();
			break;

		case LINEUP_VIEW:
			m_Grid.OnPaint();
			break;

		case REPORT_VIEW:
			m_MessageDisplay.OnPaint(&memDC);
			break;
	}
	memDC.Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen32Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_CurrentView)
	{
		CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen32Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	int iPlayerClickedOn = NOPLAYER;
	CPlayer* pPlayer;

	// clicked on team
	if (m_CurrentView == LINEUP_VIEW)
	{
		int iColumn = m_Grid.GetCurrentRow(point);
		if (iColumn != NOTFOUND)
		{
			// Show Player Information Screen?
			CRect rectWnd;
			GetClientRect(&rectWnd);
			if (point.x < rectWnd.CenterPoint().x)
			{
				pPlayer = (CPlayer*)m_Grid.GetItemPointer(iColumn, 0);
			}
			else
			{
				pPlayer = (CPlayer*)m_Grid.GetItemPointer(iColumn, 2);;
			}
			WorldData().OnViewPlayerDetails(pPlayer);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::OnResultOnly
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen32Dlg::OnResultOnly()
{
	m_bViewMatch = false;
	CDialog::OnCancel();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::DisplayMatchDetails
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen32Dlg::DisplayMatchDetails()
{
	CFixture& Fixture = WorldData().GetMatchHandler().GetMatchPlay().GetFixture();
	CString str;

	COffScreenBuffer& memDC = *CBitmapDialog::GetMemDC();
	memDC.SetTextColor(BLACK);

	int GapY = 20;
	CRect DataRect;
	GetClientRect(DataRect);

	eFONTSIZE FontSize = BIGFONT;
	if (DataRect.bottom > 200)
	{
		DataRect.top = 64;
	}
	else
	{
		FontSize = BOLDFONT;
		DataRect.top = 46;
		GapY = 18;
	}

	str.Format(_T("%s - %s"), WorldData().GetCurrentUserClub()->GetName(), WorldData().GetCurrentUserClub()->GetManager().GetName());
	GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);
	DataRect.top += GapY;

	GetBasicDisplay()->PrintCentredString(DataRect.top, WorldData().GetMatchHandler().GetMatchPlay().FullCompetitionTitle(), TRANSPARENT, FontSize);

	DataRect.top += GapY;
	if (WorldData().GetMatchHandler().GetMatchPlay().IsLeagueMatch() == true && WorldData().GetMatchHandler().GetMatchPlay().GetDivision().GetNumberMatchesPlayedThisSeason() > 1)
	{
		str.Format(TEXT("%s (%d)"), Fixture.GetClub(HOME).GetName(), Fixture.GetClub(HOME).GetTableData().GetLeaguePosition() + 1);
	}
	else
	{
		str = Fixture.GetClub(HOME).GetName();
	}
	GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);

	m_pngLargeKits.DoDrawTransparent(CBitmapDialog::GetMemDC()->operator HDC(), GetCentreX() - 116, DataRect.top, Fixture.GetClub(HOME).GetHomeKitID());
	m_pngLargeKits.DoDrawTransparent(CBitmapDialog::GetMemDC()->operator HDC(), GetCentreX() + 86, DataRect.top, Fixture.GetClub(AWAY).GetHomeKitID());

	DataRect.top += GapY;
	str.LoadString(IDS_VERSUS);
	GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);

	DataRect.top += GapY;
	if (WorldData().GetMatchHandler().GetMatchPlay().IsLeagueMatch() == true && WorldData().GetMatchHandler().GetMatchPlay().GetDivision().GetNumberMatchesPlayedThisSeason() > 1)
	{
		str.Format(TEXT("%s (%d)"), Fixture.GetClub(AWAY).GetName(), Fixture.GetClub(AWAY).GetTableData().GetLeaguePosition() + 1);
	}
	else
	{
		str = Fixture.GetClub(AWAY).GetName();
	}
	GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);

	DataRect.top += GapY;
	GetBasicDisplay()->PrintCentredString(DataRect.top, Fixture.StadiumName(), TRANSPARENT, FontSize);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::SetCurrentView
	Access:    	public 
	Parameter: 	int _LineupShow
	Parameter: 	int _ReportShow
	Parameter: 	eView _NewView
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen32Dlg::SetCurrentView(int _LineupShow, int _ReportShow, eView _NewView)
{
	m_Grid.ShowWindow(_LineupShow);
	m_MessageDisplay.ShowWindow(_ReportShow);
	m_CurrentView = _NewView;
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::OnMatchIntro
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen32Dlg::OnMatchIntro()
{
	SetCurrentView(SW_HIDE, SW_HIDE, INTRO_VIEW);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::OnMatchLineups
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen32Dlg::OnMatchLineups()
{
	SetCurrentView(SW_SHOW, SW_HIDE, LINEUP_VIEW);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::OnOppositionReport
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen32Dlg::OnOppositionReport()
{
	SetCurrentView(SW_HIDE, SW_SHOW, REPORT_VIEW);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen32Dlg::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen32Dlg::OnSize(UINT nType, int cx, int cy)
{
	CBitmapDialog::OnSize(nType, cx, cy);

	if (IsInitialised() == true)
	{
		CRect theRect;
		GetClientRect(&theRect);

		int RowsToDisplay = (theRect.bottom - 2 - TOPY) / m_Grid.GetRowHeight();
		m_Grid.MoveWindow(CRect(GetCentreX() - GRID_WIDTH / 2, TOPY, GetCentreX() + GRID_WIDTH / 2 - m_Grid.GetScrollBarWidth(), TOPY + RowsToDisplay * m_Grid.GetRowHeight()));
		m_MessageDisplay.MoveWindow(CRect(2, TOPY, theRect.right - 2, theRect.bottom - 2));
	}
	Sleep(1);
}
