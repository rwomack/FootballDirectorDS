
 /*

	Screen17.cpp : Match Report implementation file

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
 */


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "Screen17.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen17Dlg dialog



/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::CScreen17Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns: 
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen17Dlg::CScreen17Dlg(CWnd* pParent /*=null*/)
    : m_pCup(null)
	, m_pFixture(null)
	, CBitmapDialog(CScreen17Dlg::IDD, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::~CScreen17Dlg
	Access:    	public 
	Returns:  
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen17Dlg::~CScreen17Dlg()
{
}


BEGIN_MESSAGE_MAP(CScreen17Dlg, CBitmapDialog)
	ON_WM_PAINT()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_CONTINUE, OnOK)
	ON_COMMAND(ID_ACTION_TIMELINE, OnTimeLine)
	ON_COMMAND(ID_ACTION_RATINGS, OnRatings)
	ON_COMMAND(ID_ACTION_EVENTS, OnEvents)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen17 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen17Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	CRect rect;
	GetClientRect(&rect);
	int GapY = 15;
	rect.top = 42;
	if (rect.bottom > 200)
	{
		GetBasicDisplay()->SetFontBold();
	
	}
	else
	{
		GetBasicDisplay()->SetFontNormal();
		GapY = 14;
	}
	
	COffScreenBuffer& memDC = *CBitmapDialog::GetMemDC();
	memDC.SetTextColor(BLACK);

	rect.left = GetCentreX() - memDC.GetTextExtent(m_szTitle).cx / 2;
	memDC.ExtTextOut(rect.left, rect.top, ETO_CLIPPED, rect, m_szTitle, m_szTitle.GetLength(), null);

	rect.top += GapY;
	CString strPrint;
	strPrint.Format(TEXT("%s %d"), m_pFixture->GetClub(HOME).GetName(), m_pFixture->GetNumberGoals(HOME));
	rect.left = GetCentreX() - 4 - memDC.GetTextExtent(strPrint).cx;
	memDC.SetTextColor(ClubColour[HOME]);
	memDC.ExtTextOut(rect.left, rect.top, ETO_CLIPPED, rect, strPrint, strPrint.GetLength(), null);

	strPrint.Format(TEXT("%d %s"), m_pFixture->GetNumberGoals(AWAY), m_pFixture->GetClub(AWAY).GetName());
	rect.left = GetCentreX() + 4;
	memDC.SetTextColor(ClubColour[AWAY]);
	memDC.ExtTextOut(rect.left, rect.top, ETO_CLIPPED, rect, strPrint, strPrint.GetLength(), null);

	strPrint = TEXT("-");
	rect.left = (memDC.GetBufferWidth() /2) - memDC.GetTextExtent(strPrint).cx / 2;
	memDC.SetTextColor(BLACK);
	memDC.ExtTextOut(rect.left, rect.top, ETO_CLIPPED, rect, strPrint, strPrint.GetLength(), null);

	rect.top += GapY;
	strPrint.Format(IDS_HALFTIMEANDATTENDANCE, m_pFixture->GetEventList()->HalfTimeHomeGoals(), m_pFixture->GetEventList()->HalfTimeAwayGoals(), m_pFixture->AttendanceStr());
	rect.left = (memDC.GetBufferWidth() /2) - memDC.GetTextExtent(strPrint).cx / 2;
	memDC.ExtTextOut(rect.left, rect.top, ETO_CLIPPED, rect, strPrint, strPrint.GetLength(), null);

	switch(m_WhichEvents)
	{
		case RATINGS_VIEW:
			DrawTeamSkill();
			break;
		case EVENTS_VIEW:
			DrawMatchEvents();
			break;
	}
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen17Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN17, IDS_SCREEN17);
	CBitmapDialog::LoadMenu(IDR_SCREEN17MENU);

	HDC dc = ::GetDC(m_hWnd);
	m_pngGoal.DoInitialise(dc, IDR_PNGBALL, 1);
	m_pngYellowCard.DoInitialise(dc, IDR_PNGYELLOWCARD, 1);
	m_pngRedCard.DoInitialise(dc, IDR_PNGREDCARD, 1);
	m_pngSmallKits.DoInitialise(dc, IDR_PNGSMALLHOMEKITS, MAXKITS);
	m_pngPenaltyShootOut.DoInitialise(dc, IDR_PNGPENALTYSHOOTOUT, 3);
	::ReleaseDC(m_hWnd, dc);

	CBitmapDialog::GetGrid().SetStyle(LC_NOHIGHLIGHT | LC_SHOWCOLUMNS);
	CBitmapDialog::GetGrid().SetRowHeight(20);
	CBitmapDialog::GetGrid().DoAddColumn(24, COL_BITMAP);				// EventGfx
	CBitmapDialog::GetGrid().DoAddColumn(28, COL_NUMERICR);			// Time
	CBitmapDialog::GetGrid().DoAddColumn(24, COL_BITMAP);				// Player shirt
	CBitmapDialog::GetGrid().DoAddColumn(124, COL_TEXT | COL_ALIGNL);	// Player Name

	GetBasicDisplay()->DoAddTextRegions(1);
	if (m_pFixture->GetEventList()->WasPenaltyShootOut() == true)
	{
		CBitmapDialog::GetTextRegion(0)->DoInitialise(GetBasicDisplay(), m_pFixture->PenaltyShootoutWinnerStr(), COL_ALIGNC, RED, NULL);
	}
	else if (m_pCup != null)
	{
		CCupRound* pRound = m_pCup->DoFindRoundFromFixture(m_pFixture);
		if (pRound->WasWonOnAwayGoals(m_pFixture, *m_pCup) == true)
		{
			CBitmapDialog::GetTextRegion(0)->DoInitialise(GetBasicDisplay(), pRound->WhoWonOnAwayGoalsCountDoublestr(m_pFixture, *m_pCup), COL_ALIGNC, RED, NULL);
		}
		else
		{
			CBitmapDialog::GetTextRegion(0)->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		CBitmapDialog::GetTextRegion(0)->ShowWindow(SW_HIDE);
	}
	OnTimeLine();
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::DoModal
	Access:    	public 
	Parameter: 	CCup * _Cup
	Parameter: 	CFixture * _Fixture
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CScreen17Dlg::DoModal(CCup* _Cup, CFixture*_Fixture)
{
	m_pCup = _Cup;
	EXT_ASSERT_POINTER(_Fixture, CFixture);
	m_pFixture = _Fixture;
	m_szTitle = m_pFixture->GetCompetitionTitle();
	return CDialog::DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::DoBuildTimeLineGrid
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen17Dlg::DoBuildTimeLineGrid()
{
	CMatchEvent* pEvent;
	CGrid& theGrid = CBitmapDialog::GetGrid();
	m_pFixture->GetEventList()->SortByTime();
	theGrid.DoAddRows(m_pFixture->GetEventList()->GetSize/*DoCountInGameEvents*/());
	for (int LoopCount = 0; LoopCount < m_pFixture->GetEventList()->GetSize(); LoopCount ++)
	{
		pEvent = static_cast<CMatchEvent*>(m_pFixture->GetEventList()->GetAt(LoopCount));
		switch (pEvent->GetEventType())
		{
			case AWAYGOAL:
			case HOMEGOAL:
			case HOMEOWNGOAL:
			case HOMEPENALTYGOAL:
			case AWAYOWNGOAL:
			case AWAYPENALTYGOAL:
				theGrid.SetItemBitmap(LoopCount, 0, &m_pngGoal);
				theGrid.SetItemNum(LoopCount, 1, pEvent->GetTime());
				break;

			case AWAYBOOKING:
			case HOMEBOOKING:
				theGrid.SetItemBitmap(LoopCount, 0, &m_pngYellowCard);
				theGrid.SetItemNum(LoopCount, 1, pEvent->GetTime());
				break;

			case AWAYSENDINGOFF:
			case HOMESENDINGOFF:
				theGrid.SetItemBitmap(LoopCount, 0, &m_pngRedCard);
				theGrid.SetItemNum(LoopCount, 1, pEvent->GetTime());
				break;

			case HOMEPENALTYSHOOTOUTGOAL:
			case AWAYPENALTYSHOOTOUTGOAL:
				theGrid.SetItemBitmap(LoopCount, 0, &m_pngPenaltyShootOut, 0);
				break;												  
			case HOMEPENALTYSHOOTOUTMISS:							  
			case AWAYPENALTYSHOOTOUTMISS:							  
				theGrid.SetItemBitmap(LoopCount, 0, &m_pngPenaltyShootOut, 1);
				break;												  
			case HOMEPENALTYSHOOTOUTSAVED:							  
			case AWAYPENALTYSHOOTOUTSAVED:							  
				theGrid.SetItemBitmap(LoopCount, 0, &m_pngPenaltyShootOut, 2);
				break;


			default:
				EXT_ASSERT(false);
		}
		theGrid.SetItemText(LoopCount, 3, pEvent->GetText());
		if (pEvent->IsHomeTeamEvent() == true)
		{
			theGrid.SetItemBitmap(LoopCount, 2, &m_pngSmallKits, m_pFixture->GetClub(HOME).GetHomeKitID());
			theGrid.SetItemTextColour(LoopCount, 3, ClubColour[HOME]);
		}
		else
		{
			theGrid.SetItemBitmap(LoopCount, 2, &m_pngSmallKits, m_pFixture->GetClub(AWAY).GetHomeKitID());
			theGrid.SetItemTextColour(LoopCount, 3, ClubColour[AWAY]);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::OnTimeLine
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen17Dlg::OnTimeLine()
{
	DoBuildTimeLineGrid();
	if (CBitmapDialog::GetTextRegion(0)->GetText().IsEmpty() == false)
	{
		CBitmapDialog::GetTextRegion(0)->ShowWindow(SW_SHOW);
	}
	SetViewMode(SW_SHOW, TIMELINE_VIEW);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::OnRatings
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen17Dlg::OnRatings()
{
	CBitmapDialog::GetTextRegion(0)->ShowWindow(SW_HIDE);
	SetViewMode(SW_HIDE, RATINGS_VIEW);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::OnEvents
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen17Dlg::OnEvents()
{
	CBitmapDialog::GetTextRegion(0)->ShowWindow(SW_HIDE);
	SetViewMode(SW_HIDE, EVENTS_VIEW);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::SetViewMode
	Access:    	public 
	Parameter: 	int _Grid
	Parameter: 	eVIEWMODE _View
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen17Dlg::SetViewMode(int _Grid, eVIEWMODE _View)
{
	CBitmapDialog::GetGrid().ShowWindow(_Grid);
	m_WhichEvents = _View;
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::DoDrawBlobs
	Access:    	protected 
	Parameter: 	CRect & _DataRect
	Parameter: 	const int _Num
	Parameter: 	const int _Num1
	Parameter: 	const COLORREF _Colour
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen17Dlg::DoDrawBlobs(CRect& _DataRect, const int _Num, const int _Num1, const COLORREF _Colour)
{
	int BlobWidth = 4;
	int Left = GetCentreX() - 8;
	int Right = GetCentreX() + 8;
	if (CBitmapDialog::GetMemDC()->GetBufferHeight() < 200)
	{
		BlobWidth = 3;
		Left -= 36;
		Right += 36;
	}
	_DataRect.left = Right;
	CBitmapDialog::GetMemDC()->DoDrawBlobsRight(_DataRect, _Num, _Colour, BlobWidth, 8);
	_DataRect.left = Left;
	CBitmapDialog::GetMemDC()->DoDrawBlobsLeft(_DataRect, _Num1, _Colour, BlobWidth, 8);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::DrawTeamSkill
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen17Dlg::DrawTeamSkill()
{
	COffScreenBuffer& memDC = *CBitmapDialog::GetMemDC();
	CString str;
	CRect bar;
	CRect rect;
	GetClientRect(&rect);

	int iGapY = 10;
	rect.top = 90;
	const int FontSize = 16;
	int DropBlob = FontSize;
	if (memDC.GetBufferHeight() < 200)
	{
		DropBlob = 6;
		iGapY = 18 - DropBlob;
	}

	memDC.DisplayTitle(IDS_MSTATGSKILL, &rect, (memDC.GetBufferWidth() /2), DARKGREEN, FontSize);
	rect.top += DropBlob;
	DoDrawBlobs(rect, m_pFixture->GetGoalKeeperSkill(AWAY) / 5, m_pFixture->GetGoalKeeperSkill(HOME) / 5, DARKGREEN);
	rect.top += iGapY;

	memDC.DisplayTitle(IDS_MSTATDSKILL, &rect, (memDC.GetBufferWidth() /2), BLUE, FontSize);
	rect.top += DropBlob;
	DoDrawBlobs(rect, m_pFixture->GetDefenderSkill(AWAY) / 5, m_pFixture->GetDefenderSkill(HOME) / 5, BLUE);
	rect.top += iGapY;

	memDC.DisplayTitle(IDS_MSTATMSKILL, &rect, (memDC.GetBufferWidth() /2), CYAN, FontSize);
	rect.top += DropBlob;
	DoDrawBlobs(rect, m_pFixture->GetMidfielderSkill(AWAY) / 5, m_pFixture->GetMidfielderSkill(HOME) / 5, CYAN);
	rect.top += iGapY;

	memDC.DisplayTitle(IDS_MSTATASKILL, &rect, (memDC.GetBufferWidth() /2), RED, FontSize);
	rect.top += DropBlob;
	DoDrawBlobs(rect, m_pFixture->GetAttackerSkill(AWAY) / 5, m_pFixture->GetAttackerSkill(HOME) / 5, RED);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::DrawMatchEvents
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen17Dlg::DrawMatchEvents()
{
	COffScreenBuffer& memDC = *CBitmapDialog::GetMemDC();
	memDC.SetBkMode(TRANSPARENT);

	CRect rect;
	GetClientRect(&rect);
	rect.top = 110;
	int iGapY = 19;
	int BarHeight = 17;
	if (memDC.GetBufferHeight() < 200)
	{
		BarHeight = 15;
		rect.top = 84;
		iGapY = 14;
	}
	DrawBar(m_pFixture->GetPercentageOfPlay(HOME), 100 - m_pFixture->GetPercentageOfPlay(HOME), IDS_MSTATPERCENT1, BarHeight, rect);

	rect.top += iGapY;
	DrawBar(m_pFixture->GetCorners(HOME), m_pFixture->GetCorners(AWAY), IDS_MSTATCORNERS, BarHeight, rect);
	rect.top += iGapY;

	DrawBar(m_pFixture->GetFreeKicks(HOME), m_pFixture->GetFreeKicks(AWAY), IDS_MSTATFKICKS, BarHeight, rect);
	rect.top += iGapY;

	DrawBar(m_pFixture->GetShotsOnTarget(HOME), m_pFixture->GetShotsOnTarget(AWAY), IDS_MSTATSHOTSONT, BarHeight, rect);
	rect.top += iGapY;

	DrawBar(m_pFixture->GetShotsOffTarget(HOME), m_pFixture->GetShotsOffTarget(AWAY), IDS_MSTATSHOTSFT, BarHeight, rect);
	rect.top += iGapY;

	DrawBar(m_pFixture->GetOffsides(HOME), m_pFixture->GetOffsides(AWAY), IDS_MSTATSOFFSIDES, BarHeight, rect);

	memDC.SetBkMode(OPAQUE);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::DrawBar
	Access:    	protected 
	Parameter: 	int _Value1
	Parameter: 	int _Value2
	Parameter: 	int _Header
	Parameter: 	int _BarHeight
	Parameter: 	CRect & _Rect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen17Dlg::DrawBar(int _Value1, int _Value2, int _Header, int _BarHeight, CRect& _Rect)
{
	COffScreenBuffer& memDC = *CBitmapDialog::GetMemDC();
	memDC.DrawBar(_Rect.top + 1, _Value1, _Value2, ClubColour[HOME], ClubColour[AWAY], _BarHeight);
	memDC.DisplayNumeric(_Value1, &_Rect, GetCentreX() - 80, _Rect.top, CYAN, true, _BarHeight - 1);
	memDC.DisplayNumeric(_Value2, &_Rect, GetCentreX() + 96, _Rect.top, CYAN, true, _BarHeight - 1);
	memDC.DisplayTitle(_Header, &_Rect, (memDC.GetBufferWidth() /2), WHITE, _BarHeight - 1, true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen17Dlg::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen17Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		CRect theRect;
		GetClientRect(&theRect);
		CBitmapDialog::GetTextRegion(0)->MoveWindow(CRect(0, 84, theRect.right, TOPY17 - 2));

		int RowsToDisplay = (theRect.bottom - 2 - TOPY17) / CBitmapDialog::GetGrid().GetRowHeight();
		CBitmapDialog::GetGrid().MoveWindow(CRect(GetCentreX() - GRID_WIDTH / 2, TOPY17, GetCentreX() + GRID_WIDTH / 2 - CBitmapDialog::GetGrid().GetScrollBarWidth(), TOPY17 + RowsToDisplay * CBitmapDialog::GetGrid().GetRowHeight()));
	}
	Sleep(1);
}
