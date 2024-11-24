
/*

	Screen65DLG.cpp : View Match implementation file
	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "MessageDisplay.h"
#include "UI.h"
#include "Dialog.h"
#include "SoccerResultGrid.h"
#include "Screen65DLG.h"
#include "Screen64Dlg.h"
#include "PlayerNameTextRegion.h"
#include "FootballDirector.h"


BEGIN_MESSAGE_MAP(CScreen65Dlg, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_ACTION_KICKOFF, DoCommenceHalf)
	ON_COMMAND(ID_ACTION_SUBSTITUTIONS, OnSubstitutions)
	ON_COMMAND(ID_ACTION_COMMENTARY, OnViewCommentary)
	ON_COMMAND(ID_ACTION_SKILL, OnViewSkill)
	ON_COMMAND(ID_ACTION_TEAMS, OnViewTeams)
	ON_COMMAND(ID_ACTION_EVENTS, OnViewEvents)
	ON_COMMAND(ID_ACTION_LATESTSCORES, OnViewLatestScores)
	ON_COMMAND(ID_ACTION_LEAGUETABLE, OnViewLeagueTables)
	ON_COMMAND(ID_ACTION_SPEEDMATCH, OnSpeedMatch)
	ON_COMMAND(ID_ACTION_SLOWMATCH, OnSlowMatch)
	ON_COMMAND(ID_ACTION_MATCHRADAR, OnMatchRadar)
	ON_COMMAND(ID_ACTION_SKIPTOENDOFMATCH, OnSkipToEndOfMatch)
	ON_COMMAND(ID_CONTINUE, OnOK)
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::CScreen65Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier:61
	Purpose:
------------------------------------------------------------------------------*/
CScreen65Dlg::CScreen65Dlg(CMatchPlay* _MatchPlay, CWnd* pParent /*=NULL*/)
	: CBitmapDialog(CScreen65Dlg::IDD, pParent)
{
	EXT_ASSERT_POINTER(_MatchPlay, CMatchPlay);
	m_MatchPlay = _MatchPlay;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::~CScreen65Dlg
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen65Dlg::~CScreen65Dlg()
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::DoBuildLineupGrid
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::DoBuildLineupGrid()
{
	CPlayer* pPlayer;
	CString strPrint;
	for (int LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++)
	{
		pPlayer = &m_MatchPlay->GetPlayer(HOME, LoopCount);
		m_LineupGrid.SetItemTextColour(LoopCount, 0, PosnColour[pPlayer->GetSkills().GetPosition()]);
		m_LineupGrid.SetItemTextColour(LoopCount, 1, PosnColour[pPlayer->GetSkills().GetPosition()]);
		m_LineupGrid.SetItemText(LoopCount, 0, pPlayer->GetName(), pPlayer, PLAYER_POINTER);
		m_LineupGrid.SetItemNum(LoopCount, 1, m_MatchPlay->GetPlayerSkill(HOME, LoopCount));

		m_LineupGrid.SetItemNum(LoopCount, 2, LoopCount + 1);
		m_LineupGrid.SetItemTextColour(LoopCount, 2, BLACK);

		pPlayer = &m_MatchPlay->GetPlayer(AWAY, LoopCount);
		m_LineupGrid.SetItemTextColour(LoopCount, 3, PosnColour[pPlayer->GetSkills().GetPosition()]);
		m_LineupGrid.SetItemTextColour(LoopCount, 4, PosnColour[pPlayer->GetSkills().GetPosition()]);
		m_LineupGrid.SetItemNum(LoopCount, 3, m_MatchPlay->GetPlayerSkill(AWAY, LoopCount));
		m_LineupGrid.SetItemText(LoopCount, 4, pPlayer->GetName(), pPlayer, PLAYER_POINTER);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen65Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN65, IDS_SCREEN65);
	CBitmapDialog::LoadMenu(IDR_SCREEN65KICKOFFMENU);

	m_bIsMatchPaused = true;
	m_SubstitutionRequested = false;
	m_ViewMode = eMAXVIEWMODE;

	m_HomeScoreStatic.DoInitialise(GetBasicDisplay(), _T(""), COL_ALIGNR);
	m_HomeScoreStatic.SetTextColour(BLUE);
	m_AwayScoreStatic.DoInitialise(GetBasicDisplay(), "", COL_ALIGNL);
	m_AwayScoreStatic.SetTextColour(ORANGE);
	GetBasicDisplay()->DoAddTextRegion(&m_HomeScoreStatic);
	GetBasicDisplay()->DoAddTextRegion(&m_AwayScoreStatic);

	m_TimeStatic.DoInitialise(GetBasicDisplay(), "", COL_ALIGNC);
	m_TimeStatic.SetTextColour(BLACK);
	GetBasicDisplay()->DoAddTextRegion(&m_TimeStatic);

	//  eCOMMENTARY,
	m_CommentaryDisplay.SetDrawInfo(this,  0x1000, CRect(5, 144, 236, 218), BLACK, true);

	//  eSKILL,

	//  eLINEUPS,
	m_LineupGrid.OnInitialUpdate(this, GetBasicDisplay(), LC_SHOWCOLUMNS | LC_NOHIGHLIGHT);
	m_LineupGrid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);

	m_LineupGrid.RemoveAllData(true);
	m_LineupGrid.DoAddColumn(90, COL_TEXT | COL_ALIGNR);	// Home Player Name
	m_LineupGrid.DoAddColumn(18, COL_NUMERICR);				// Rating
	m_LineupGrid.DoAddColumn(18, COL_NUMERICR);				// Squad number
	m_LineupGrid.DoAddColumn(18, COL_NUMERICR);				// Rating
	m_LineupGrid.DoAddColumn(90, COL_TEXT | COL_ALIGNL);	// Away Player Name

	m_LineupGrid.DoAddRows(PLAYERSINTEAM);
	m_LineupGrid.ShowWindow(SW_HIDE);
	DoBuildLineupGrid();

	//  eEVENTS,
	//  eRADAR,
	HDC dc = ::GetDC(m_hWnd);
	m_Pitch.SetBitmap(dc, IDR_PNGPITCH, TEXT("PNG"));
	m_Ball.SetBitmap(dc, IDR_PNGBALL, TEXT("PNG"));
	::ReleaseDC(m_hWnd, dc);

	//  eLATESTSCORES,
	// Construct the list of matches to report latest scores on
#ifdef LATESTSCORES
	if (m_MatchPlay->AreLatestScoresAvailable() == true)
	{
		if (m_MatchPlay->IsLeagueMatch())
		{
			m_MatchPlay->GetDivision().DoBuildCurrentFixtureList(m_LatestScoresGrid.GetFixtures());
		}
	 	else
	 	{
	 		m_MatchPlay->GetCupRound().BuildCurrentNonUserFixtureList(m_LatestScoresGrid.GetFixtures());
	 	}
		m_LatestScoresGrid.OnInitialUpdate(this, m_hWnd, GetBasicDisplay());
	}
#endif

	//  eLEAGUETABLE,
	if (m_MatchPlay->AreLeagueTablesAvailable() == true)
	{
		m_TableGrid.OnInitialUpdate(this, GetBasicDisplay(), LC_SHOWCOLUMNS | LC_NOHIGHLIGHT);
		m_TableGrid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
		m_TableGrid.DoAddColumn(72, COL_NORMAL);	// Club Name
		m_TableGrid.DoAddColumn(18, COL_NUMERICR);	// Played
		m_TableGrid.DoAddColumn(18, COL_NUMERICR);	// Won
		m_TableGrid.DoAddColumn(18, COL_NUMERICR);	// Drawn
		m_TableGrid.DoAddColumn(18, COL_NUMERICR);	// Lost
		m_TableGrid.DoAddColumn(26, COL_NUMERICR);	// For
		m_TableGrid.DoAddColumn(26, COL_NUMERICR);	// Against
		m_TableGrid.DoAddColumn(26, COL_NUMERICR);	// Points
		m_TableGrid.DoAddRows(m_MatchPlay->GetDivision().GetNumberOfClubsInDivision());
		m_TableGrid.ShowWindow(SW_HIDE);
	}

	SwitchToMode(eCOMMENTARY);
	SetTimer(ID_KICKOFF, 500, NULL);
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	m_CommentaryDisplay.OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			break;
		
		case VK_RETURN:
		case VK_SELECT:
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnLButtonUp
	Access:    	public 
	Parameter: 	UINT _Flags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnLButtonUp(UINT _Flags, CPoint _Point)
{
	int ButtonPressed = GetBasicDisplay()->OnLButtonUp(_Flags, _Point);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	CFixture& Fixture = m_MatchPlay->GetFixture();
	COffScreenBuffer& memDC = *CBitmapDialog::GetMemDC();
	switch(m_ViewMode)
	{
		case eCOMMENTARY:
			m_CommentaryDisplay.OnPaint(CBitmapDialog::GetMemDC());
			break;
		case eSKILL:
		{
			GetBasicDisplay()->SetFontNormal();
			int CentreX = GetCentreX();
			int iGapY = 30;
			int BlobHeight = 8;
			CRect rectWnd;
			GetClientRect(&rectWnd);
			CRect DataRect(CentreX - 114, TOPY + 30, CentreX + 114, 280);
			if (rectWnd.bottom < 210)
			{
				iGapY = 22;
				BlobHeight = 4;
			}
			else
			{
				memDC.DrawBar(DataRect.top, Fixture.GetPercentageOfPlay(HOME), 100 - Fixture.GetPercentageOfPlay(HOME), ORANGE, BLUE);
				CString str;
				str.Format(IDS_MSTATPERCENT, Fixture.GetPercentageOfPlay(HOME), Fixture.GetPercentageOfPlay(AWAY));
				GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, NORMALFONT);
				DataRect.top += 6;
			}
			DataRect.top += iGapY;

			DataRect.left = CentreX + 8;
			memDC.DoDrawBlobsRight(DataRect, Fixture.GetGoalKeeperSkill(AWAY) / 5, PosnColour[0], 4, BlobHeight);
			DataRect.left = CentreX - 8;
			memDC.DoDrawBlobsLeft(DataRect, Fixture.GetGoalKeeperSkill(HOME) / 5, PosnColour[0], 4, BlobHeight);
			
			memDC.SetBkMode(TRANSPARENT);
			GetBasicDisplay()->PrintCentredStringID(DataRect.top - 16, IDS_GOALKEEPER, PosnColour[0], NORMALFONT);
			DataRect.top += iGapY;

			DataRect.left = CentreX + 8;
			memDC.DoDrawBlobsRight(DataRect, Fixture.GetDefenderSkill(AWAY) / 5, PosnColour[1], 4, BlobHeight);
			DataRect.left = CentreX - 8;
			memDC.DoDrawBlobsLeft(DataRect, Fixture.GetDefenderSkill(HOME) / 5, PosnColour[1], 4, BlobHeight);
			GetBasicDisplay()->PrintCentredStringID(DataRect.top - 16, IDS_DEFENDER, PosnColour[1], NORMALFONT);
			DataRect.top += iGapY;

			DataRect.left = CentreX + 8;
			memDC.DoDrawBlobsRight(DataRect, Fixture.GetMidfielderSkill(AWAY) / 5, PosnColour[2], 4, BlobHeight);
			DataRect.left = CentreX - 8;
			memDC.DoDrawBlobsLeft(DataRect, Fixture.GetMidfielderSkill(HOME) / 5, PosnColour[2], 4, BlobHeight);
			GetBasicDisplay()->PrintCentredStringID(DataRect.top - 16, IDS_MIDFIELDER, PosnColour[2], NORMALFONT);
			DataRect.top += iGapY;

			DataRect.left = CentreX + 8;
			memDC.DoDrawBlobsRight(DataRect, Fixture.GetAttackerSkill(AWAY) / 5, PosnColour[3], 4, BlobHeight);
			DataRect.left = CentreX - 8;
			memDC.DoDrawBlobsLeft(DataRect, Fixture.GetAttackerSkill(HOME) / 5, PosnColour[3], 4, BlobHeight);
			GetBasicDisplay()->PrintCentredStringID(DataRect.top - 16, IDS_ATTACKER, PosnColour[3], NORMALFONT);
		}
			break;
		case eLINEUPS:
			m_LineupGrid.OnPaint();
			break;
		case eEVENTS:
			{
				CRect DataRect;
				GetClientRect(&DataRect);
				int iGapY = 17;
				DataRect.top = 70;
				int BarHeight = 17;
				int GapBetweenBars = 4;
				if (DataRect.bottom < 200)
				{
					iGapY = 14;
					BarHeight = 15;
					GapBetweenBars = 2;
				}

				GetBasicDisplay()->PrintCentredStringID(DataRect.top, IDS_MSTATBOOK, BLACK, NORMALFONT);
				DataRect.top += 4;
				DataRect.left = GetCentreX() + 40;
				memDC.DoDrawBlobsRight(DataRect, Fixture.GetNumberOfPlayersBooked(AWAY), YELLOW);
				DataRect.left = GetCentreX()  - 40;
				memDC.DoDrawBlobsLeft(DataRect, Fixture.GetNumberOfPlayersBooked(HOME), YELLOW);
				DataRect.top += (iGapY - 10);

				memDC.SetBkColor(WHITE);
				GetBasicDisplay()->PrintCentredStringID(DataRect.top, IDS_MSTATSOFFS, BLACK, NORMALFONT);
				DataRect.top += 4;
				DataRect.left = GetCentreX() + 60;
				memDC.DoDrawBlobsRight(DataRect, Fixture.GetNumberOfPlayersSentOff(AWAY), RED);
				DataRect.left = GetCentreX() - 60;
				memDC.DoDrawBlobsLeft(DataRect, Fixture.GetNumberOfPlayersSentOff(HOME), RED);

				CMatchEventsStats& HomeEvents = Fixture.GetClubData(HOME);
				CMatchEventsStats& AwayEvents = Fixture.GetClubData(AWAY);
				DoDrawBar(DataRect.top += 18, HomeEvents.GetCorners(), AwayEvents.GetCorners(), IDS_MSTATCORNERS, BarHeight, DataRect);
				DoDrawBar(DataRect.top += iGapY + GapBetweenBars, HomeEvents.GetFreeKicks(), AwayEvents.GetFreeKicks(), IDS_MSTATFKICKS, BarHeight, DataRect);
				DoDrawBar(DataRect.top += iGapY + GapBetweenBars, HomeEvents.GetShotsOnTarget(), AwayEvents.GetShotsOnTarget(), IDS_MSTATSHOTSONT, BarHeight, DataRect);
				DoDrawBar(DataRect.top += iGapY + GapBetweenBars, HomeEvents.GetShotsOffTarget(), AwayEvents.GetShotsOffTarget(), IDS_MSTATSHOTSFT, BarHeight, DataRect);
				DoDrawBar(DataRect.top += iGapY + GapBetweenBars, HomeEvents.GetOffsides(), AwayEvents.GetOffsides(), IDS_MSTATSOFFSIDES, BarHeight, DataRect);
			}
			break;
		case eRADAR:
			{
				m_Pitch.Draw(memDC.operator HDC(), m_PitchRect.left, m_PitchRect.top);
				CString str;
				CRect DataRect;

				DataRect.top = m_PitchRect.top + PITCH_HEIGHT / 2 - 10;
				DataRect.left = m_PitchRect.left + 30;
				DataRect.bottom = DataRect.top + 30;
				DataRect.right = DataRect.left + 100;
				str.Format(__T("%d%%     %d%%,     %d%%"), m_MatchPlay->HomeAttackPossesion(), m_MatchPlay->MidfieldPossesion(), m_MatchPlay->AwayAttackPossesion());
				memDC.DisplayText(str, &DataRect, BLUE,  24, true);

				m_Ball.DoDrawTransparent(memDC.operator HDC(), m_PitchRect.left + (m_MatchPlay->GetBallX() * PITCH_STEPX), m_PitchRect.top + m_MatchPlay->GetBallY() * PITCH_STEPY);
				m_CommentaryDisplay.OnPaint(CBitmapDialog::GetMemDC());
			}
			break;
		case eLATESTSCORES:
			m_LatestScoresGrid.DoBuild(m_MatchPlay->GetTimePlayed());
//			m_LatestScoresGrid.OnPaint();
			break;
		case eLEAGUETABLE:
			if (m_MatchPlay->AreLeagueTablesAvailable() == true)
			{
				DoBuildLeagueTable();
//				m_TableGrid.OnPaint();
			}
			break;
		default:
			break;
	}
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::DoBuildLeagueTable
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::DoBuildLeagueTable()
{
	const COLORREF TextColor[] =
	{
		TRANSCOLOUR,	// Normal
		PALERED,		// Relegation spot
		PALEBLUE,		// Playoff spot
		PALEGREEN		// Promotion spot
	};

	CDivision& pDivision = m_MatchPlay->GetDivision();
	CClub* pClub;
	// Overall, home and away
	pDivision.SortLeagueTable(m_ViewMode);
	for (int LoopCount = 0; LoopCount < pDivision.GetNumberOfClubsInDivision(); LoopCount++)
	{
		pClub = &pDivision.DoFindClubInTablePosition(LoopCount);
		m_TableGrid.SetRowBackgroundColour(LoopCount, TextColor[pDivision.LeaguePositionStatus(LoopCount)]);
		pClub->DoBuildLeagueTableRow(LoopCount, m_TableGrid);
	}

	CFixture* pFixture;
	uint HomeGoals, AwayGoals;
	for (int LoopCount = 0; LoopCount < m_LatestScoresGrid.GetFixtures().GetSize(); LoopCount++)
	{
		pFixture = m_LatestScoresGrid.GetFixtures().GetAt(LoopCount);
		for (int GridCount = 0; GridCount < pDivision.GetNumberOfClubsInDivision(); GridCount++)
		{
			if (m_TableGrid.GetRowPointer(GridCount) == &pFixture->GetClub(HOME))
			{
				HomeGoals = pFixture->GetNumberGoals(HOME, m_MatchPlay->GetTimePlayed());
				AwayGoals = pFixture->GetNumberGoals(AWAY, m_MatchPlay->GetTimePlayed());
				m_TableGrid.GetCell(5, GridCount)->DoAddToValue(HomeGoals);
				m_TableGrid.GetCell(6, GridCount)->DoAddToValue(AwayGoals);
				m_TableGrid.GetCell(1, GridCount)->DoAddToValue(1);	// Games Played
				if (HomeGoals > AwayGoals)
				{
					m_TableGrid.GetCell(2, GridCount)->DoAddToValue(1);
					m_TableGrid.GetCell(7, GridCount)->DoAddToValue(3);
				}
				else if (HomeGoals == AwayGoals)
				{
					m_TableGrid.GetCell(3, GridCount)->DoAddToValue(1);
					m_TableGrid.GetCell(7, GridCount)->DoAddToValue(1);
				}
				else
				{
					m_TableGrid.GetCell(4, GridCount)->DoAddToValue(1);
				}
			}
			else if (m_TableGrid.GetRowPointer(GridCount) == &pFixture->GetClub(AWAY))
			{
				HomeGoals = pFixture->GetNumberGoals(HOME, m_MatchPlay->GetTimePlayed());
				AwayGoals = pFixture->GetNumberGoals(AWAY, m_MatchPlay->GetTimePlayed());
				m_TableGrid.GetCell(6, GridCount)->DoAddToValue(HomeGoals);
				m_TableGrid.GetCell(5, GridCount)->DoAddToValue(AwayGoals);
				m_TableGrid.GetCell(1, GridCount)->DoAddToValue(1);	// Games Played
				if (HomeGoals < AwayGoals)
				{
					m_TableGrid.GetCell(2, GridCount)->DoAddToValue(1);
					m_TableGrid.GetCell(7, GridCount)->DoAddToValue(3);
				}
				else if (HomeGoals == AwayGoals)
				{
					m_TableGrid.GetCell(3, GridCount)->DoAddToValue(1);
					m_TableGrid.GetCell(7, GridCount)->DoAddToValue(1);
				}
				else
				{
					m_TableGrid.GetCell(4, GridCount)->DoAddToValue(1);
				}
			}
		}
 	}
	m_TableGrid.DoSortHighWithDifference(7, 5, 6);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::DoDrawBar
	Access:    	public 
	Parameter: 	int _YPos
	Parameter: 	int _Value1
	Parameter: 	int _Value2
	Parameter: 	int _HeaderStrID
	Parameter: 	int _BarHeight
	Parameter: 	CRect & _DataRect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::DoDrawBar(int _YPos, int _Value1, int _Value2, int _HeaderStrID, int _BarHeight, CRect& _DataRect)
{
	COffScreenBuffer& memDC = *CBitmapDialog::GetMemDC();
	memDC.DrawBar(_YPos, _Value1, _Value2, ClubColour[HOME], ClubColour[AWAY], _BarHeight);
	memDC.DisplayNumeric(_Value1, &_DataRect, GetCentreX() - 80, _DataRect.top, CYAN, true, _BarHeight - 1);
	memDC.DisplayNumeric(_Value2, &_DataRect, GetCentreX() + 96, _DataRect.top, CYAN, true, _BarHeight - 1);
	memDC.DisplayTitle(_HeaderStrID, &_DataRect, GetCentreX(), WHITE, _BarHeight - 1, true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnVScroll
	Access:    	protected 
	Parameter: 	UINT nSBCode
	Parameter: 	UINT nPos
	Parameter: 	CScrollBar * pScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch(m_ViewMode)
	{
		case eRADAR:
		case eCOMMENTARY:
			if (m_CommentaryDisplay.OnVScroll(nSBCode, nPos, pScrollBar) == TRUE)
			{
				ReDraw();
			}
			break;
		case eSKILL:
			break;
		case eLINEUPS:
			if (m_LineupGrid.OnVScroll(nSBCode, nPos, pScrollBar) == TRUE)
			{
				ReDraw();
			}
			break;
		case eEVENTS:
			break;
		case eLATESTSCORES:
			if (m_LatestScoresGrid.OnVScroll(nSBCode, nPos, pScrollBar) == TRUE)
			{
				ReDraw();
			}
			break;
		case eLEAGUETABLE:
// 			if (m_TableGrid.OnVScroll(nSBCode, nPos, pScrollBar) == TRUE)
// 			{
// 				ReDraw();
// 			}
			break;
		default:
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::DoAddCommentary
	Access:    	protected 
	Parameter: 	CString _Comment
	Parameter: 	COLORREF _colorText
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::DoAddCommentary(CString _Comment, COLORREF _colorText /*= BLUE*/)
{
	CString strPrint;
	strPrint.Format(_T("(%d) %s"), m_MatchPlay->GetTimePlayed(), _Comment);
	if (m_MatchPlay->GetMatchActionInfo().GetInformationStr().IsEmpty() != TRUE)
	{
		m_CommentaryDisplay.AddString(m_MatchPlay->GetMatchActionInfo().GetInformationStr(), RED);
		m_MatchPlay->GetMatchActionInfo().DoClearInformationStr();
		m_CommentaryDisplay.ShowLastLine();
		ReDraw();
	}
	else
	{
		m_CommentaryDisplay.AddString(strPrint, _colorText);
	}
	if (m_strLastComment != m_MatchPlay->GetMatchActionInfo().GetCommentaryStr() &&
		m_MatchPlay->GetMatchActionInfo().GetCommentaryStr().IsEmpty() != TRUE)
	{
		m_strLastComment = m_MatchPlay->GetMatchActionInfo().GetCommentaryStr();
		DoAddCommentary(m_strLastComment, m_MatchPlay->GetMatchActionInfo().GetCommentaryTextColor());
#ifdef DUMPCOMMENTARY
		afxDump << "** Actual Commentary **** " << m_strLastComment << "\n\n";
#endif
	}
	m_CommentaryDisplay.ShowLastLine();
	ReDraw();
}


/*------------------------------------------------------------------------------
Method:   	CScreen65Dlg::DoCommenceHalf
Access:    	protected 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::DoCommenceHalf()
{
	if (m_MatchPlay->IsMatchInProgress() == true)
	{
		if (m_MatchPlay->AreLeagueTablesAvailable() == true)
		{
			CBitmapDialog::ReplaceMenu(IDR_SCREEN65SCORESTABLEMENU);
		}
		else if (m_MatchPlay->AreLatestScoresAvailable() == true)
		{
			CBitmapDialog::ReplaceMenu(IDR_SCREEN65SCORESMENU);
		}
		else
		{
			CBitmapDialog::ReplaceMenu(IDR_SCREEN65MENU);
		}

		SetTimer(ID_CLOCK, 800 - (theApp.GetApplicationPreferences().GetMatchSpeed() * 80 + 80), null);
		m_MatchPlay->DoCommenceHalf();
		m_bIsMatchPaused = false;
	}
}


/*------------------------------------------------------------------------------
Method:   	CScreen65Dlg::DoEndOfHalf
Access:    	protected 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::DoEndOfHalf()
{
	m_bIsMatchPaused = true;
	switch (m_MatchPlay->GetMatchActionStage())
	{
		case MATCHEXTRATIMEHALFTIME:
		case MATCHHALFTIME:
		case MATCHENDOFNORMALTIME:
			if (m_MatchPlay->AreLeagueTablesAvailable() == true)
			{
				CBitmapDialog::ReplaceMenu(IDR_SCREEN65HALFTIMESCORESTABLEMENU);
			}
			else if (m_MatchPlay->AreLatestScoresAvailable() == true)
			{
				CBitmapDialog::ReplaceMenu(IDR_SCREEN65HALFTIMESCORESMENU);
			}
			else
			{
				CBitmapDialog::ReplaceMenu(IDR_SCREEN65HALFTIMEMENU);
			}
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnSkipToEndOfMatch
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnSkipToEndOfMatch()
{
	m_MatchPlay->SkipToEndOfMatch();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::DoPlaySoundFX
	Access:    	protected 
	Parameter: 	const eSOUNDFX theFX
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::DoPlaySoundFX(const eSOUNDFX theFX)
{
#ifdef FMOD
	if (theApp.GetApplicationPreferences().GetSoundFX().IsEnabled() == true && m_MatchPlay->GetMatchActionInfo().GetSoundFXID() != SOUND_MAX)
	{
		theApp.GetSoundManager().DoPlaySoundFX(theFX);
		m_MatchPlay->GetMatchActionInfo().SetSoundFXID(SOUND_MAX);

		// Lengthen the time to next event to allow sound to play
		KillTimer(ID_CLOCK);

		// Dont set timer if match over
		if (m_MatchPlay->GetMatchActionStage() != MATCHENDED && theApp.GetApplicationPreferences().GetMatchSpeed() > 0)
		{
			SetTimer(ID_CLOCK, 2500 - (theApp.GetApplicationPreferences().GetMatchSpeed() > 4) * 1200, null);
		}
	}
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::DoUpdateCommentary
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::DoUpdateCommentary()
{
	if (m_MatchPlay->GetMatchActionInfo().GetInformationStr().IsEmpty() != TRUE)
	{
		m_CommentaryDisplay.AddString(m_MatchPlay->GetMatchActionInfo().GetInformationStr(), RED);
		m_MatchPlay->GetMatchActionInfo().DoClearInformationStr();
		m_CommentaryDisplay.ShowLastLine();
		ReDraw();
	}
	if (m_strLastComment != m_MatchPlay->GetMatchActionInfo().GetCommentaryStr() &&
		m_MatchPlay->GetMatchActionInfo().GetCommentaryStr().IsEmpty() != TRUE)
	{
		m_strLastComment = m_MatchPlay->GetMatchActionInfo().GetCommentaryStr();
		DoAddCommentary(m_strLastComment, m_MatchPlay->GetMatchActionInfo().GetCommentaryTextColor());
#ifdef DUMPCOMMENTARY
		afxDump << "** Actual Commentary **** " << m_strLastComment << "\n\n";
#endif
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);

		CRect theWindow;
		GetClientRect(&theWindow);
		int CurrY = TOPY - 2;
		m_HomeScoreStatic.MoveWindow(CRect(15, CurrY, GetCentreX() + 5, CurrY + 16));
		m_AwayScoreStatic.MoveWindow(CRect(GetCentreX() + 3, CurrY, theWindow.right, CurrY + 16));
		CurrY += 16;
		m_TimeStatic.MoveWindow(CRect(0, CurrY, theWindow.right, CurrY + 16));
		CurrY += 24;

		//  eCOMMENTARY,
		
		//  eSKILL,

		//  eLINEUPS,
		int RowsToDisplay = min(PLAYERSINTEAM, (theWindow.bottom - 4 - CurrY) / m_LineupGrid.GetRowHeight());
		m_LineupGrid.MoveWindow(CRect(GetCentreX() - 118, CurrY, 
		GetCentreX() + 118 - (RowsToDisplay != 11) * m_LineupGrid.GetScrollBarWidth(), CurrY + RowsToDisplay * m_LineupGrid.GetRowHeight()));

		//  eEVENTS,

		//  eRADAR,
		CurrY -= 10;
		m_PitchRect.left = GetCentreX() - PITCH_WIDTH / 2;
		m_PitchRect.top = CurrY;
		m_PitchRect.right = GetCentreX() + PITCH_WIDTH / 2;
		m_PitchRect.bottom = CurrY + PITCH_HEIGHT;
		DoSizeCommentaryBox();

		//  eLATESTSCORES,
		if (m_MatchPlay->AreLatestScoresAvailable() == true)
		{
			RowsToDisplay = min(12, (theWindow.bottom - 4 - CurrY) / m_LatestScoresGrid.GetRowHeight());
			m_LatestScoresGrid.MoveWindow(CRect(GetCentreX() - 110, CurrY, GetCentreX() + 110, CurrY + RowsToDisplay * m_LatestScoresGrid.GetRowHeight()));
		}

		//  eLEAGUETABLE,
		if (m_MatchPlay->AreLeagueTablesAvailable() == true)
		{
 			RowsToDisplay = (theWindow.bottom - 4 - CurrY) / m_TableGrid.GetRowHeight();
 			m_TableGrid.MoveWindow(CRect(GetCentreX() - LEAGUETABLEGRIDWIDTH / 2 - m_TableGrid.GetScrollBarWidth() / 2, CurrY, GetCentreX() + LEAGUETABLEGRIDWIDTH / 2, CurrY + RowsToDisplay * m_TableGrid.GetRowHeight()));
		}

	}
	Sleep(1);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::DoSizeCommentaryBox
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::DoSizeCommentaryBox()
{
	CRect theWindow;
	GetClientRect(&theWindow);
	if (m_ViewMode == eRADAR)
	{
		if (theWindow.bottom > m_PitchRect.bottom + 32)
		{
			m_CommentaryDisplay.ShowWindow(SW_SHOW);
			m_CommentaryDisplay.MoveWindow(CRect(2, m_PitchRect.bottom + 2, theWindow.right - 2, theWindow.bottom - 2));
		}
		else
		{
			m_CommentaryDisplay.ShowWindow(SW_HIDE);
		}
	}
	else if (m_ViewMode == eCOMMENTARY)
	{
		m_CommentaryDisplay.ShowWindow(SW_SHOW);
		m_CommentaryDisplay.MoveWindow(CRect(2, TOPY + 38, theWindow.right - 2, theWindow.bottom - 2));
	}
}

/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnTimer
	Access:    	protected 
	Parameter: 	UINT _EventID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnTimer(UINT _EventID)
{
	switch(_EventID)
	{
		case ID_CLOCK:
			KillTimer(ID_CLOCK);
			DoUpdateCommentary();
			if (m_MatchPlay->IsUserSubstitutionRequired() == true)
			{
				CString str;
				str.Format(IDS_PLAYERINJURED, m_MatchPlay->GetPlayerName());
				m_CommentaryDisplay.AddString(str, RED);
				m_MatchPlay->SetUserSubstitutionRequired(false);
				////@@@ Need to call sUbstitutions dlg
				SetTimer(ID_CLOCK, 1800, null);
				break;
			}
			if (m_SubstitutionRequested == true)
			{
				OnDoSubstitutions();
			}
			// Pause match if on a user action page
			if (m_bIsMatchPaused == false)
			{
				m_MatchPlay->DoMinute();
				switch (m_MatchPlay->GetMatchActionStage())
				{
					case MATCHEXTRATIMEHALFTIME:
					case MATCHHALFTIME:
						DoEndOfHalf();
						//DoCommenceHalf();
						break;
					case MATCHENDOFNORMALTIME:
						DoEndOfHalf();
						break;
					case MATCHNOTSTARTED:
					case MATCHINPROGRESS:
						SetTimer(ID_CLOCK, (800 - theApp.GetApplicationPreferences().GetMatchSpeed() * 100 + 80), null);
						break;
					case MATCHENDOFEXTRATIME:
						DoEndOfHalf();
						break;
					case MATCHREQUIREPENALTYSHOOTOUT:
						DoEndOfHalf();
						DoAddCommentary(m_MatchPlay->PenaltyShootoutWinnerStr());
						m_MatchPlay->SetMatchActionStage(MATCHENDED);
						break;
					case MATCHENDED:
						DoEndOfHalf();
						break;

					default:
						EXT_ASSERT(false);
						break;
				}
			}
			else
			{
				// Match is paused so just keep feeding the timer until it restarts
				SetTimer(ID_CLOCK, 80, null);
			}
			break;

		case ID_KICKOFF:
			KillTimer(ID_KICKOFF);
			//UserMessageBox(IDS_KICKOFF);
			//DoCommenceHalf();
			break;

		default :
			EXT_ASSERT(false);
	}
#ifdef FMOD
	DoPlaySoundFX(m_MatchPlay->GetMatchActionInfo().GetSoundFXID());
#endif

	CString strPrint;
	strPrint.Format(_T("%s %d-"), m_MatchPlay->GetFixture().GetClub(HOME).GetName(), m_MatchPlay->GetFixture().GetNumberGoals(HOME));
	m_HomeScoreStatic.SetText(strPrint);
	strPrint.Format(_T("%d %s"), m_MatchPlay->GetFixture().GetNumberGoals(AWAY), m_MatchPlay->GetFixture().GetClub(AWAY).GetName());
	m_AwayScoreStatic.SetText(strPrint);
	switch (m_MatchPlay->GetMatchActionStage())
	{
		case MATCHENDED:
			CBitmapDialog::ReplaceMenu(IDR_MENUCONTINUE);
			m_TimeStatic.SetTextColour(RED);
			m_TimeStatic.SetStrId(IDS_FULLTIME);
#ifdef POCKETPC
			SHDoneButton(SHDB_SHOW);	  // Show 'ok' on dialog
#endif
			break;			
		case MATCHEXTRATIMEHALFTIME:
			m_TimeStatic.SetTextColour(RED);
			m_TimeStatic.SetStrId(IDS_HALFTIME);
			break;			
		case MATCHHALFTIME:
			m_TimeStatic.SetTextColour(RED);
			m_TimeStatic.SetStrId(IDS_HALFTIME);
			break;			
		case MATCHENDOFNORMALTIME:
			m_TimeStatic.SetTextColour(RED);
			m_TimeStatic.SetStrId(IDS_EXTRATIME);
			break;			

		default:
			m_TimeStatic.SetTextColour(BLACK);
			strPrint.LoadString(IDS_TIME);
			CString str;
			str.Format(strPrint, m_MatchPlay->GetTimePlayed());
			m_TimeStatic.SetText(str);
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnDoSubstitutions
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnDoSubstitutions()
{
	INT_PTR SubMode = SUBS_SUBS;
	while (SubMode != SUBS_MATCH)
	{
		switch(SubMode)
		{
			case SUBS_SUBS:
			{
				CScreen64Dlg theDlg(m_MatchPlay);
				SubMode = theDlg.DoModal();
				DoBuildLineupGrid();
				break;
			}
			case SUBS_ROLES:
			{
				CScreen27SpecialPlayersDlg theDlg;
				SubMode = theDlg.DoModal();
				break;
			}
			case SUBS_TACTICS:
			{
				CScreen27TacticsDlg theDlg;
				SubMode = theDlg.DoModal();
				break;
			}
			case SUBS_FORMATION:
			{
				CScreen27FormationDlg theDlg;
				SubMode = theDlg.DoModal();
				break;
			}
			default:
				break;
		}
	}
	m_SubstitutionRequested = false;
}

/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnViewCommentary
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnViewCommentary()
{
	SwitchToMode(eCOMMENTARY);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::SwitchToMode
	Access:    	protected 
	Parameter: 	eView _NewViewMode
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::SwitchToMode(eVIEWMODE _NewViewMode)
{
	if (m_ViewMode != _NewViewMode)
	{
		// Disable old view controls
		switch(m_ViewMode)
		{
			case eCOMMENTARY:
				m_CommentaryDisplay.ShowWindow(SW_HIDE);
				break;
			case eSKILL:
				break;
			case eLINEUPS:
				m_LineupGrid.ShowWindow(SW_HIDE);
				break;
			case eEVENTS:
				break;
			case eRADAR:
				m_CommentaryDisplay.ShowWindow(SW_HIDE);
				break;
			case eLATESTSCORES:
				m_LatestScoresGrid.ShowWindow(SW_HIDE);
				break;
			case eLEAGUETABLE:
				m_TableGrid.ShowWindow(SW_HIDE);
				break;
			default:
				break;
		}
		m_ViewMode = _NewViewMode;
//		m_LatestScoresGrid.ShowWindow(SW_SHOW);

		// Initialise the new view
		switch(m_ViewMode)
		{
			case eCOMMENTARY:
				DoSizeCommentaryBox();
				break;
			case eSKILL:
				break;
			case eLINEUPS:
				m_LineupGrid.ShowWindow(SW_SHOW);
				break;
			case eEVENTS:
				break;
			case eRADAR:
				DoSizeCommentaryBox();
				break;
			case eLATESTSCORES:
				m_LatestScoresGrid.ShowWindow(SW_SHOW);
				break;
			case eLEAGUETABLE:
				m_TableGrid.ShowWindow(SW_SHOW);
				break;
			default:
				break;
		}
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnViewSkill
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnViewSkill()
{
	SwitchToMode(eSKILL);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnViewTeams
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnViewTeams()
{
	SwitchToMode(eLINEUPS);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnViewEvents
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnViewEvents()
{
	SwitchToMode(eEVENTS);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnViewLatestScores
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnViewLatestScores()
{
	SwitchToMode(eLATESTSCORES);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnViewLeagueTables
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnViewLeagueTables()
{
	SwitchToMode(eLEAGUETABLE);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnSubstitutions
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnSubstitutions()
{
	m_SubstitutionRequested = true;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnSpeedMatch
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnSpeedMatch()
{
	theApp.GetApplicationPreferences().DoRaiseMatchSpeed();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnSlowMatch
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnSlowMatch()
{
	theApp.GetApplicationPreferences().DoLowerMatchSpeed();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Dlg::OnMatchRadar
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Dlg::OnMatchRadar()
{
	SwitchToMode(eRADAR);
}
