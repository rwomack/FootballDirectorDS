
/*

	screen64Dlg.cpp : InMatch tactics and substitutions
	
	PocketPC

	Football Director

	(c) Rw Software 1994 - 2008

	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "Screen64Dlg.h"
#include "HumanManager.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen64Dlg dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen64Dlg::CScreen64Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen64Dlg::CScreen64Dlg(CMatchPlay* _MatchPlay, CWnd* pParent /*=null*/)
	: CInMatchActionBitmapDialog(CScreen64Dlg::IDD, pParent)
{
	EXT_ASSERT_POINTER(_MatchPlay, CMatchPlay);
	m_MatchPlay = _MatchPlay;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen64Dlg::~CScreen64Dlg
	Access:    	public 
	Returns:  
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen64Dlg::~CScreen64Dlg()
{
}


BEGIN_MESSAGE_MAP(CScreen64Dlg, CInMatchActionBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SIZE()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen64Dlg message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen64Dlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen64Dlg::OnInitDialog()
{
	CInMatchActionBitmapDialog::OnInitDialog(IDS_HELPSCREEN64, IDS_SCREEN64);
	CBitmapDialog::LoadMenu(IDR_SCREEN64MENU);

	m_TeamGrid.OnInitialUpdate(this, GetBasicDisplay(), LC_SHOWCOLUMNS | LC_SWAPPABLECOLUMNS);
	m_SubGrid.OnInitialUpdate(this, GetBasicDisplay(), LC_SHOWCOLUMNS | LC_SWAPPABLECOLUMNS);
	m_TeamGrid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
	m_SubGrid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
	m_TeamGrid.RemoveAllData(true);
	m_TeamGrid.DoAddColumn(STATUS_COL_WIDTH, COL_BITMAP);			// Sub status
	m_TeamGrid.DoAddColumn(PLAYER_NAME_LENGTH, COL_TEXT | COL_ALIGNL);
	m_TeamGrid.DoAddColumn(18, COL_NUMERICR);						// Rating

	m_SubGrid.RemoveAllData(true);
	m_SubGrid.DoAddColumn(STATUS_COL_WIDTH, COL_BITMAP);			// Sub status
	m_SubGrid.DoAddColumn(PLAYER_NAME_LENGTH, COL_TEXT | COL_ALIGNL);

	m_TeamGrid.DoAddRows(PLAYERSINTEAM);
	m_SubGrid.DoAddRows(m_MatchPlay->GetFixture().GetSubsSelect());

	HDC dc = ::GetDC(m_hWnd);
	m_pngArrowLR.DoInitialise(dc, IDR_PNGMATCHSUBS, 5);
	::ReleaseDC(m_hWnd, dc);

	DoBuildGrids();
	m_WhichGrid = eTEAMGRID;
	m_TeamGrid.DoSwapClickedRows();
	m_TeamGrid.ShowCursorBar(SW_SHOW);
	m_SubGrid.ShowCursorBar(SW_HIDE);
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen64Dlg::DoBuildGrids
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen64Dlg::DoBuildGrids()
{
	CClub& CurrentClub = m_MatchPlay->GetCurrentUserControlledClub();
	CPlayer* pPlayer;
	for (uint LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++)
	{
		pPlayer = &CurrentClub.GetPlayer(LoopCount);
		m_TeamGrid.SetRowPointer(LoopCount, pPlayer);
 		if (pPlayer->GetMatchStatus() != MAXPLAYERSTATUSES)
 		{
 			m_TeamGrid.SetItemBitmap(LoopCount, 0, &m_pngArrowLR, pPlayer->GetMatchStatus() + eINJURED);
 			m_TeamGrid.SetItemTextColour(LoopCount, 1, GREY);
 		}
 		else 
		if (LoopCount == 2 || SubsInfo().HasPlayerBeenSubstitutedOn(CurrentClub.GetFirstTeamSquad().GetSquad(LoopCount)) == true)
		{
			m_TeamGrid.DisableRow(LoopCount, true);
		}
		else
		{
			m_TeamGrid.SetItemTextColour(LoopCount, 1, PosnColour[pPlayer->GetSkills().GetPosition()]);
		}
		m_TeamGrid.SetItemText(LoopCount, 1, pPlayer->GetName());
		m_TeamGrid.SetItemNum(LoopCount, 2, pPlayer->GetSkills().GetOverallSkill());
	}

	for (uint LoopCount = PLAYERSINTEAM; LoopCount < PLAYERSINTEAM + m_MatchPlay->GetFixture().GetSubsSelect(); LoopCount++)
	{
		pPlayer = &CurrentClub.GetPlayer(LoopCount);
		m_SubGrid.SetRowPointer(LoopCount - PLAYERSINTEAM, pPlayer);
		if (SubsInfo().HasPlayerBeenSubstitutedOff(CurrentClub.GetFirstTeamSquad().GetSquad(LoopCount)) == true)
		{
			m_SubGrid.SetItemBitmap(LoopCount - PLAYERSINTEAM, 0, &m_pngArrowLR, eTAKENOFF);
			m_SubGrid.DisableRow(LoopCount - PLAYERSINTEAM, true);
		}
		else
		{
			m_SubGrid.SetItemTextColour(LoopCount - PLAYERSINTEAM, 1, PosnColour[pPlayer->GetSkills().GetPosition()]);
		}
		m_SubGrid.SetItemText(LoopCount - PLAYERSINTEAM, 1, pPlayer->GetName());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen64Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT _Flags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen64Dlg::OnLButtonDown(UINT _Flags, CPoint _Point)
{
//	CGridBitmapDialog::OnLButtonDown(_Flags, _Point);
#ifdef POCKETPC
	SHRGINFO shrgi = {0};
	shrgi.cbSize = sizeof(SHRGINFO);
	shrgi.hwndClient = m_hWnd;
	shrgi.ptDown.x = _Point.x;
	shrgi.ptDown.y = _Point.y;
	shrgi.dwFlags = SHRG_RETURNCMD;

	if (GN_CONTEXTMENU == ::SHRecognizeGesture(&shrgi))
	{
		CPoint point(GetMessagePos());
		point.y -= 26;
	//	WorldData().OnViewPlayerInformation(&GetClub().GetPlayer(DoCalcPlayerClickedOn(point)));
		Default();
	}
#endif
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen64Dlg::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen64Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_WhichGrid == eTEAMGRID)
	{
		m_TeamGrid.OnKeyDown(nChar, nRepCnt, nFlags);
	}
	else
	{
		m_SubGrid.OnKeyDown(nChar, nRepCnt, nFlags);
	}
	DEBUG_OUTPUT(("%d, %d, %d, %d"), (m_TeamGrid.GetCurrentRow(), m_SubGrid.GetCurrentRow(), m_TeamGrid.GetHilite(), m_TeamGrid.GetHiliteBackgroundColour()));
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			break;

		case VK_RETURN:
		case VK_F23:
			if (m_WhichGrid == eTEAMGRID)
			{
				if (m_MatchPlay->GetFixture().GetMatchSubs().AnySubstitutionsLeft(m_MatchPlay->GetCurrentUserVenue()) == true)
				{
					DEBUG_OUTPUT(("%d, %d, %d"), (m_TeamGrid.GetCurrentRow(), m_TeamGrid.GetClickedItem(), m_TeamGrid.GetLastSelectedItem()));
					m_TeamGrid.SetSwap();
					DEBUG_OUTPUT(("%d, %d, %d"), (m_TeamGrid.GetCurrentRow(), m_TeamGrid.GetClickedItem(), m_TeamGrid.GetLastSelectedItem()));
					m_WhichGrid = eSUBGRID;
					m_TeamGrid.ShowCursorBar(SW_HIDE);
					m_SubGrid.ShowCursorBar(SW_SHOW);
					m_SubGrid.DoSwapClickedRows();
					m_SubGrid.SetSelectedRow(0);
					if (m_SubGrid.IsRowDisabled(0))
					{
						m_SubGrid.OnKeyDown(VK_DOWN, 0, 0);
					}
				}
			}
			else
			{
				m_SubGrid.SetSwap();
				m_WhichGrid = eTEAMGRID;
				SubsInfo().MakeSubstitution(m_MatchPlay->GetTimePlayed(), m_MatchPlay->GetCurrentUserVenue(), m_TeamGrid.GetCurrentRow(), m_SubGrid.GetCurrentRow() + PLAYERSINTEAM);
#ifdef FMOD
				theApp.GetSoundManager().DoPlaySoundFX(SOUND_BEEP);
#endif
				DoBuildGrids();
				m_TeamGrid.ShowCursorBar(SW_SHOW);
				m_SubGrid.ShowCursorBar(SW_HIDE);
				m_TeamGrid.SetSelectedRow(0);
				m_TeamGrid.DoSwapClickedRows();
				m_SubGrid.DoSwapClickedRows();
				if (m_TeamGrid.IsRowDisabled(0))
				{
					m_TeamGrid.OnKeyDown(VK_DOWN, 0, 0);
				}
				ReDraw();
			}
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen64Dlg::DoSetSwap
	Access:    	protected 
	Parameter: 	const ushort _SquadNumber
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen64Dlg::DoSetSwap(const ushort _SquadNumber)
{
	eSubstitutionAction eAction = m_MatchPlay->DoSwapPlayers(m_MatchPlay->GetCurrentUserVenue(), _SquadNumber);
	switch (eAction)
	{
		case eNOSUBSLEFT:
	#ifdef FMOD
			theApp.GetSoundManager().DoPlaySoundFX(SOUND_BEEP);
	#endif
			break;
		case eSWAP1SET:
		case eSWAPPEDONPITCH:
		case eSWAPPEDONBENCH:
		case eSUBSTITUTIONMADE:
			break;
		case eSUBSTITUTIONCANCELLED:
			break;
		default:
			EXT_ASSERT(false);
	};
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen64Dlg::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen64Dlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
//	CGridBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen64Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen64Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();

//	m_TeamGrid.OnPaint();
//	m_SubGrid.OnPaint();

	// Display how many further substitutions maybe made
// 	DataRect.top += GAPY * 5 - 4;
// 	DataRect.left = 128;
// 	memDC.FillSolidRect(124, DataRect.top, 114, GAPY, VPALEGREEN);
// 
// 	strName.Format(IDS_SUBSLEFT, m_pCurrentMatchPlay->GetFixture().GetMatchSubs().DoCountSubstitutionsLeft(CMatchBitmapPropertyPage::GetVenue()));
// 	memDC.SetTextColor(BLACK);
// 	memDC.ExtTextOut(DataRect.left, DataRect.top, 0, DataRect, strName, strName.GetLength(), null);

// 	GetBasicDisplay().RestoreFont();
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen64Dlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen64Dlg::OnLButtonUp(UINT _nFlags, CPoint _Point)
{
	if (m_TeamGrid.GetCurrentRow(_Point) != NOTFOUND)
	{
	}
	else 
	{
		int ButtonPressed = CBitmapDialog::GetBasicDisplay()->OnLButtonUp(_nFlags, _Point);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen64Dlg::SubsInfo
	Access:    	protected 
	Returns:   	CMatchSubs&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchSubs& CScreen64Dlg::SubsInfo()
{
	return m_MatchPlay->GetFixture().GetMatchSubs();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen64Dlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen64Dlg::OnSize(UINT nType, int cx, int cy)
{
	CBitmapDialog::OnSize(nType, cx, cy);

	const int GRID_WIDTH = 120; 
	if (IsInitialised() == true)
	{
 		CRect theRect;
 		GetClientRect(&theRect);
		int TeamGridX = max(2, (GetCentreX() - TEAM_GRID_WIDTH + m_TeamGrid.GetScrollBarWidth()) / 2);
		DEBUG_OUTPUT(("Team : %d, %d"), (TeamGridX, GetCentreX() - (TEAM_GRID_WIDTH + m_TeamGrid.GetScrollBarWidth()) / 2));
  		int RowsToDisplay = min(PLAYERSINTEAM, (theRect.bottom - TOPY - 2) / m_TeamGrid.GetRowHeight());
  		m_TeamGrid.MoveWindow(CRect(TeamGridX, TOPY, TEAM_GRID_WIDTH + TeamGridX, TOPY + RowsToDisplay * m_TeamGrid.GetRowHeight()));

		int SubGridX = max(TeamGridX + TEAM_GRID_WIDTH + m_TeamGrid.GetScrollBarWidth() + 2, GetCentreX() + (theRect.right - GetCentreX() - SUB_GRID_WIDTH) / 2);
//		DEBUG_OUTPUT(("Sub : %d, %d"), (TeamGridX + TEAM_GRID_WIDTH + m_TeamGrid.GetScrollBarWidth() + 2, GetCentreX() + (theRect.right - GetCentreX() - SUB_GRID_WIDTH) / 2));
		RowsToDisplay = min(m_MatchPlay->GetFixture().GetSubsSelect(), (theRect.bottom - TOPY - 2) / m_SubGrid.GetRowHeight());
		m_SubGrid.MoveWindow(CRect(SubGridX, TOPY, SubGridX + SUB_GRID_WIDTH, TOPY + RowsToDisplay * m_SubGrid.GetRowHeight()));
		Sleep(1);
	}
}