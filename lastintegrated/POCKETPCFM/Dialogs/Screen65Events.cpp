// Screen65Events.cpp : implementation file
//

#include "stdafx.h"

#include "UI.h"
#include "..\system\Screen\Win32\ExtPropertyPage.h"
#include "ExtStatic.h"
#include "Screen65PropertyPage.h"
#include "Screen65Events.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen65Events property page

IMPLEMENT_DYNCREATE(CScreen65Events, CPropertyPage)

CScreen65Events::CScreen65Events() :
	CMatchBitmapPropertyPage(CScreen65Events::IDD)
{
}

CScreen65Events::~CScreen65Events()
{
}

BEGIN_MESSAGE_MAP(CScreen65Events, CBitmapPropertyPage)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen65Events message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen65Events::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Events::OnPaint()
{
	// Offscreen buffer
	COffScreenBuffer& memDC = *CBitmapPropertyPage::GetMemDC();
	CBitmapPropertyPage::OnInitRedrawArea();

	CRect bar;
	const int iGapY = 22;
	CString str;
	CFixture& Fixture = m_pCurrentMatchPlay->GetFixture();

    CRect DataRect(6, 92, 240, 280);

	memDC.DisplayTitle(IDS_MSTATBOOK, &DataRect, (memDC.GetBufferWidth() /2), BLACK, 18);
	DataRect.top += 4;
	DataRect.left = 160;
	memDC.DoDrawBlobsRight(DataRect, Fixture.GetNumberOfPlayersBooked(AWAY), YELLOW);
	DataRect.left = 80;
	memDC.DoDrawBlobsLeft(DataRect, Fixture.GetNumberOfPlayersBooked(HOME), YELLOW);
	DataRect.top += (iGapY - 10);

	memDC.SetBkColor(WHITE);
	memDC.DisplayTitle(IDS_MSTATSOFFS, &DataRect, (memDC.GetBufferWidth() /2), BLACK, 18);
	DataRect.top += 4;
	DataRect.left = 180;
	memDC.DoDrawBlobsRight(DataRect, Fixture.GetNumberOfPlayersSentOff (AWAY), RED);
	DataRect.left = 60;
	memDC.DoDrawBlobsLeft(DataRect, Fixture.GetNumberOfPlayersSentOff (HOME), RED);

	CMatchEventsStats& HomeEvents =  m_pCurrentMatchPlay->GetFixture().GetClubData(HOME);
	CMatchEventsStats& AwayEvents =  m_pCurrentMatchPlay->GetFixture().GetClubData(AWAY);
	DrawBar(DataRect.top += 18, HomeEvents.GetCorners(), AwayEvents.GetCorners(), IDS_MSTATCORNERS, &DataRect);
	DrawBar(DataRect.top += iGapY, HomeEvents.GetFreeKicks(), AwayEvents.GetFreeKicks(), IDS_MSTATFKICKS, &DataRect);
	DrawBar(DataRect.top += iGapY, HomeEvents.GetShotsOnTarget(), AwayEvents.GetShotsOnTarget(), IDS_MSTATSHOTSONT, &DataRect);
	DrawBar(DataRect.top += iGapY, HomeEvents.GetShotsOffTarget(), AwayEvents.GetShotsOffTarget(), IDS_MSTATSHOTSFT, &DataRect);
	DrawBar(DataRect.top += iGapY, HomeEvents.GetOffsides(), AwayEvents.GetOffsides(), IDS_MSTATSOFFSIDES, &DataRect);
	memDC.SetBkMode(TRANSPARENT);
	CMatchBitmapPropertyPage::OnPaint((bool)(theApp.GetApplicationPreferences().GetMatchSpeed() > 0));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Events::DrawBar
	Access:    	protected 
	Parameter: 	int YPos
	Parameter: 	int Value1
	Parameter: 	int Value2
	Parameter: 	int iHeader
	Parameter: 	CRect * DataRect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Events::DrawBar(int YPos, int Value1, int Value2, int iHeader, CRect* DataRect)
{
	COffScreenBuffer& memDC = *CBitmapPropertyPage::GetMemDC();
	CString str;
	memDC.DrawBar(YPos, Value1, Value2, ClubColour[HOME], ClubColour[AWAY]);
	str.Format(_T("%d"), Value1);
	memDC.DisplayRightTitle(str, DataRect, 40, DataRect->top, CYAN, true);
	str.Format(_T("%d"), Value2);
	memDC.DisplayRightTitle(str, DataRect, 220, DataRect->top, CYAN, true);
	memDC.DisplayTitle(iHeader, DataRect, (memDC.GetBufferWidth() /2), WHITE, 18, true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Events::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen65Events::OnInitDialog()
{
	return CMatchBitmapPropertyPage::OnInitDialog(TEXT("Match Day"));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Events::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Events::OnSize(UINT nType, int cx, int cy)
{
	CMatchBitmapPropertyPage::OnSize(nType, cx, cy);
	CMatchBitmapPropertyPage::GetBasicDisplay().SetDisplayHeader(true, IDS_SCREEN65);

	Sleep(1);
}