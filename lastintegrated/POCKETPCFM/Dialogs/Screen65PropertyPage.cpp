// Screen65PropertyPage.cpp : implementation file
//

#include "stdafx.h"

#include "UI.h"
#include "..\system\Screen\Win32\ExtPropertyPage.h"
#include "ExtStatic.h"
#include "Screen65PropertyPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*------------------------------------------------------------------------------
	Method:   	CMatchBitmapPropertyPage::GetVenue
	Access:    	public 
	Parameter: 	void
	Returns:   	eWhichTeam
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eWhichTeam CMatchBitmapPropertyPage::GetVenue()
{
	eWhichTeam eWho = AWAY;
	if (m_pCurrentMatchPlay->IsUserControlledClub(HOME) == true)
	{
		eWho = HOME;
	}
	return eWho;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchBitmapPropertyPage::OnPaint
	Access:    	public 
	Parameter: 	const bool _IsPaused
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchBitmapPropertyPage::OnPaint(const bool _IsPaused)
{
	if (_IsPaused == true)
	{
		m_pngPause.DrawTransparent(CBitmapPropertyPage::GetMemDC()->operator HDC(), PAUSEX, PAUSEY);
	}
	CString strPrint;
	strPrint.Format(_T("%s  %d -"), m_pCurrentMatchPlay->GetFixture().GetClub(HOME).GetName(), m_pCurrentMatchPlay->GetFixture().GetNumberGoals(HOME));
	m_HomeScoreStatic.SetWindowText(strPrint);
	m_HomeScoreStatic.RedrawWindow();

	strPrint.Format(_T("%d  %s"), m_pCurrentMatchPlay->GetFixture().GetNumberGoals(AWAY), m_pCurrentMatchPlay->GetFixture().GetClub(AWAY).GetName());
	m_AwayScoreStatic.SetWindowText(strPrint);
	m_AwayScoreStatic.RedrawWindow();

	COffScreenBuffer& memDC = *CBitmapPropertyPage::GetMemDC();
	CRect& rect = CBitmapPropertyPage::GetDataRect();
	rect.top = 78;
	GetBasicDisplay().SetFontNormal();
	
	memDC.SetTextColor(BLACK);
	strPrint.LoadString(IDS_TIME);
	rect.left = (memDC.GetBufferWidth() /2) - memDC.GetTextExtent(strPrint).cx;
	memDC.ExtTextOut(rect.left, rect.top, 0, rect, strPrint, strPrint.GetLength(), null);

	rect.left = (memDC.GetBufferWidth() /2) + 6;
	if (m_pCurrentMatchPlay->GetMatchActionStage() != MATCHENDED)
	{
		memDC.SetTextColor(DARKGREEN);
		strPrint.Format(_T("%d"), m_pCurrentMatchPlay->GetTimePlayed());
	}
	else
	{
		memDC.SetTextColor(RED);
		strPrint.LoadString(IDS_FULLTIME);
	}
	memDC.ExtTextOut(rect.left, rect.top, 0, rect, strPrint, strPrint.GetLength(), null);
	memDC.DrawSeperator(rect.top + 10);

    GetBasicDisplay().RestoreFont();
	CBitmapPropertyPage::OnPaint(true);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchBitmapPropertyPage::OnInitDialog
	Access:    	public 
	Parameter: 	const CString _strHelpTopic
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CMatchBitmapPropertyPage::OnInitDialog(const CString _strHelpTopic)
{
	HDC dc = ::GetDC(m_hWnd);
	m_pngPause.SetBitmap(dc, IDR_PNGPAUSE, TEXT("PNG"));
	::ReleaseDC(m_hWnd, dc);
	
	m_HomeScoreStatic.OnCreate(IDC_STATIC6503, this, 16, BLUE, WHITE);
	m_AwayScoreStatic.OnCreate(IDC_STATIC6505, this, 16, ORANGE, WHITE);
 	m_HomeScoreStatic.MoveWindow(CRect(15, 62, 125, 78));
 	m_AwayScoreStatic.MoveWindow(CRect(126, 62, 240, 78));
	return CBitmapPropertyPage::OnInitDialog(_strHelpTopic);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchBitmapPropertyPage::SetCurrentMatchPlay
	Access:    	public 
	Parameter: 	CMatchPlay * _pMatchPlay
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchBitmapPropertyPage::SetCurrentMatchPlay(CMatchPlay* _pMatchPlay)
{ 
	ASSERT(_pMatchPlay != null);
	m_pCurrentMatchPlay = _pMatchPlay; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchBitmapPropertyPage::OnSetActive
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CMatchBitmapPropertyPage::OnSetActive()
{
	return CBitmapPropertyPage::OnSetActive();
}
