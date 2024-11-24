// Screen65Commentary.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "..\system\Screen\Win32\ExtPropertyPage.h"
#include "MessageDisplay.h"
#include "ExtStatic.h"
#include "Screen65PropertyPage.h"
#include "Screen65Commentary.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CScreen65Commentary property page

IMPLEMENT_DYNCREATE(CScreen65Commentary, CPropertyPage)


//---------------------------------------------------------------------------------------
CScreen65Commentary::CScreen65Commentary() 
	: CMatchBitmapPropertyPage(CScreen65Commentary::IDD)
{
}


//---------------------------------------------------------------------------------------
CScreen65Commentary::~CScreen65Commentary()
{
}


BEGIN_MESSAGE_MAP(CScreen65Commentary, CBitmapPropertyPage)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen65Commentary message handlers


//---------------------------------------------------------------------------------------
void CScreen65Commentary::OnPaint()
{
	CBitmapPropertyPage::OnInitRedrawArea();
	m_MessageDisplay.OnPaint(CBitmapPropertyPage::GetMemDC());

	COffScreenBuffer& memDC = *CBitmapPropertyPage::GetMemDC();
	GetBasicDisplay().SetFontNormal();
	CString strPrint;
	int MatchSpeedID[] = 
	{
		IDS_MATCHSPEED2, IDS_MATCHSPEED3, 
		IDS_MATCHSPEED4, IDS_MATCHSPEED5, 
		IDS_MATCHSPEED6, IDS_MATCHSPEED7, 
	};
	strPrint.LoadString(MatchSpeedID[theApp.GetApplicationPreferences().GetMatchSpeed()]);
	CRect DataRect = CRect(175 - (memDC.GetTextExtent(strPrint).cx / 2), 228, 50, 246);
	memDC.ExtTextOut(DataRect.left, DataRect.top, 0, DataRect, strPrint, strPrint.GetLength(), null);
	strPrint.LoadString(IDS_MATCHSPEED);
	DataRect = CRect(116 - memDC.GetTextExtent(strPrint).cx, 228, 50, 246);
	memDC.ExtTextOut(DataRect.left, DataRect.top, 0, DataRect, strPrint, strPrint.GetLength(), null);
	GetBasicDisplay().RestoreFont();
	CMatchBitmapPropertyPage::OnPaint((bool)(theApp.GetApplicationPreferences().GetMatchSpeed() > 0));
}


//---------------------------------------------------------------------------------------
void CScreen65Commentary::DoAddCommentary(CString strComment, COLORREF colorText)
{
	CString strPrint;
	strPrint.Format(TEXT("(%d) %s"), m_pCurrentMatchPlay->GetTimePlayed(), strComment);
	m_MessageDisplay.AddString(strPrint, colorText);
	m_MessageDisplay.ShowLastLine();
	ReDraw();
}


//---------------------------------------------------------------------------------------
void CScreen65Commentary::OnLButtonUp(UINT nFlags, CPoint point)
{
	switch (CBitmapPropertyPage::GetBasicDisplay().OnLButtonUp(nFlags, point))
	{
		case eMATCHSPEEDDOWN:
			theApp.GetApplicationPreferences().DoLowerMatchSpeed();
			break;
		case eMATCHSPEEDUP:
			theApp.GetApplicationPreferences().DoRaiseMatchSpeed();
			break;
	}
	CBitmapPropertyPage::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Commentary::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Commentary::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapPropertyPage::GetBasicDisplay().OnLButtonDown(nFlags, point);
	CBitmapPropertyPage::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Commentary::UpdateCommentary
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Commentary::UpdateCommentary()
{
	if (m_pCurrentMatchPlay->GetMatchActionInfo().GetInformationStr().IsEmpty() != TRUE)
	{
		m_MessageDisplay.AddString(m_pCurrentMatchPlay->GetMatchActionInfo().GetInformationStr(), RED);
		m_pCurrentMatchPlay->GetMatchActionInfo().DoClearInformationStr();
		m_MessageDisplay.ShowLastLine();
		ReDraw();
	}
	if (m_strLastComment != m_pCurrentMatchPlay->GetMatchActionInfo().GetCommentaryStr() &&
		m_pCurrentMatchPlay->GetMatchActionInfo().GetCommentaryStr().IsEmpty() != TRUE)
	{
		m_strLastComment = m_pCurrentMatchPlay->GetMatchActionInfo().GetCommentaryStr();
		DoAddCommentary(m_strLastComment, m_pCurrentMatchPlay->GetMatchActionInfo().GetCommentaryTextColor());
#ifdef DUMPCOMMENTARY
	afxDump << "** Actual Commentary **** " << m_strLastComment << "\n\n";
#endif
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Commentary::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen65Commentary::OnInitDialog()
{
	CMatchBitmapPropertyPage::OnInitDialog(TEXT("Match Day"));

	m_CompetitionStatic.OnCreate(IDC_STATIC6506, this, 16, BLACK, WHITE);
	CString strPrint;
	strPrint.Format(TEXT("%s   %s"), m_pCurrentMatchPlay->GetFixture().GetMatchDate(), m_pCurrentMatchPlay->FullCompetitionTitle());
	m_CompetitionStatic.SetWindowText(strPrint);

    m_MessageDisplay.SetDrawInfo(this,  0x1000, CMatchBitmapPropertyPage::GetDataRect(), BLACK, true);

	uint theButtons[] =
	{
		IDR_PNGSCREEN65LEFT, IDR_PNGSCREEN65RIGHT, 
		NOTFOUND
	};
	GetBasicDisplay().DoAddButtons(&theButtons[0], this);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Commentary::OnVScroll
	Access:    	protected 
	Parameter: 	UINT nSBCode
	Parameter: 	UINT nPos
	Parameter: 	CScrollBar * pScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Commentary::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (m_MessageDisplay.OnVScroll(nSBCode, nPos, pScrollBar) == TRUE)
	{
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65Commentary::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65Commentary::OnSize(UINT nType, int cx, int cy)
{
	CMatchBitmapPropertyPage::OnSize(nType, cx, cy);
	CMatchBitmapPropertyPage::GetBasicDisplay().SetDisplayHeader(true, IDS_SCREEN65);

	GetBasicDisplay().DoMoveButton(eMATCHSPEEDDOWN, CRect(BUTTON1X, BUTTON1Y, BUTTON1X + BUTTON1W, BUTTON1Y + BUTTON1H));
	GetBasicDisplay().DoMoveButton(eMATCHSPEEDUP, CRect(BUTTON2X, BUTTON2Y, BUTTON2X + BUTTON2W, BUTTON2Y + BUTTON2H));

	Sleep(1);
}