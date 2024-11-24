  // Screen28View.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "MatchInfo.h"
#include "FootballDirector.h"
#include "Screen28View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen28View

IMPLEMENT_DYNCREATE(CScreen28View, CFormView)


/*------------------------------------------------------------------------------
	Method:   	CScreen28View::CScreen28View
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen28View::CScreen28View()
	: CBitmapFormView(CScreen28View::IDD) {}


BEGIN_MESSAGE_MAP(CScreen28View, CBitmapFormView)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen28View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen28View::OnSetDate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen28View::OnSetDate()
{
	// New date resets progress bar to start
	m_ProgressBar.SetPos(0);
	CBitmapFormView::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen28View::GetProgressCtl
	Access:    	public 
	Returns:   	CProgressCtrl*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CProgressCtrl* CScreen28View::GetProgressCtl()
{
	return &m_ProgressBar;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen28View::OnIncrementProgress
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen28View::OnIncrementProgress()
{
	m_ProgressBar.StepIt();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen28View::OnInitialUpdate
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen28View::OnInitialUpdate()
{
	CBitmapFormView::OnInitialUpdate(TEXT("Main Menu"), &theApp.GetBasicDisplay());
	m_ProgressBar.SubclassDlgItem(IDC_PROGRESS, this);
	m_ProgressBar.SetRange(0, 10);
	m_ProgressBar.SetStep(1);

	HDC dc = ::GetDC(m_hWnd);
	m_pngLargeKits.DoInitialise(dc, IDR_PNGLARGEHOMEKITS, MAXKITS);
	::ReleaseDC(m_hWnd, dc);

	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen28View::OnDraw
	Access:    	virtual protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen28View::OnPaint()
{
	GetBasicDisplay()->SetDisplayHeader(true, IDS_SCREEN28);
	CBitmapFormView::OnInitRedrawArea();

	COffScreenBuffer& memDC = *CBitmapFormView::GetMemDC();
	CRect DataRect;
	DataRect.SetRect(4, TOPY, GetBasicDisplay()->GetOffScreenBuffer()->GetBufferWidth(), GetBasicDisplay()->GetOffScreenBuffer()->GetBufferHeight());

	int GapY = 18;
	eFONTSIZE FontSize = BIGFONT;
	if (DataRect.bottom < 200)
	{
		FontSize = BOLDFONT;
		GapY = 13;
	}

	CString str;
	str.Format(IDS_NEXTMATCH, WorldData().GetCurrentDate().LongDateStr());
	int ShirtX = max(2, GetCentreX() - (memDC.GetTextExtent(str).cx / 2) - 40);
	m_pngLargeKits.DoDrawTransparent(CBitmapFormView::GetMemDC()->operator HDC(), ShirtX, TOPY + 18, WorldData().GetCurrentUserClub()->GetHomeKitID());

	COLORREF oldColor = memDC.SetTextColor(BLACK);

	str.Format(_T("%s - %s"), WorldData().GetCurrentUserClub()->GetName(), WorldData().GetCurrentUserClub()->GetManager().GetName());
	GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);
	DataRect.top += GapY;

	if (WorldData().GetCurrentUserClub()->GetTableData().GetNumberGamesPlayed() > 0)
	{
		str.Format(IDS_LEAGUEPOSITION, WorldData().GetCurrentUserClub()->GetDivision().GetName(), WorldData().GetCurrentUserClub()->GetTableData().GetLeaguePosition() + 1);
	}
	else
	{
		str = WorldData().GetCurrentUserClub()->GetDivision().GetName();
	}
	GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);
	DataRect.top += GapY;

	str.Format(IDS_FANCONFIDENCE, WorldData().GetCurrentUserClub()->GetManager().GetSupportersConfidence());
	GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);
	DataRect.top += GapY;

	str.Format(IDS_BOARDCONFIDENCE, WorldData().GetCurrentUserClub()->GetManager().GetBoardConfidence());
	GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);
	DataRect.top += GapY * 2 + GapY / 2;

	CMatchInfo Match;
	WorldData().GetCurrentUserClub()->GetNextMatchDetails(Match);
	// Display the details of the next match, if known
	if (Match.GetCompetitionType() != NOTAMATCH && Match.GetOpponentClub(WorldData().GetCurrentUserClub()))
	{
		str.Format(IDS_NEXTMATCH, Match.GetTheDate().LongDateStr());
		GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);
		DataRect.top += GapY;
		GetBasicDisplay()->PrintCentredString(DataRect.top, Match.GetCompetitionName(), TRANSPARENT, FontSize);
		DataRect.top += GapY;
		m_pngLargeKits.DoDrawTransparent(CBitmapFormView::GetMemDC()->operator HDC(), ShirtX, DataRect.top - 6, Match.GetOpponentClub(WorldData().GetCurrentUserClub())->GetHomeKitID());
		if (Match.GetCompetitionType() == LEAGUEMATCH)
		{
			str.Format(IDS_LEAGUEPOSITION, Match.GetOpponentClub(WorldData().GetCurrentUserClub())->GetName(), Match.GetOpponentClub(WorldData().GetCurrentUserClub())->GetTableData().GetLeaguePosition() + 1);
		}
		else
		{
			str = Match.GetOpponentClub(WorldData().GetCurrentUserClub())->GetName();
		}
		GetBasicDisplay()->PrintCentredString(DataRect.top, str, TRANSPARENT, FontSize);
		DataRect.top += GapY;

		GetBasicDisplay()->PrintCentredString(DataRect.top, Match.GetStadiumName(), TRANSPARENT, FontSize);
	}
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen28View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen28View::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);
		CRect theRect;
		GetClientRect(&theRect);
		theRect.left += 2;
		theRect.right -= 2;
		theRect.bottom -= 2;
		theRect.top = theRect.bottom - 14;
		m_ProgressBar.MoveWindow(&theRect);
	}

	Sleep(1);
}