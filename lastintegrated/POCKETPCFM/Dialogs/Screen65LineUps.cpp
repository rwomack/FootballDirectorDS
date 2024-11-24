// Screen65LineUps.cpp : implementation file
//

#include "stdafx.h"

#include "UI.h"
#include "..\system\Screen\Win32\ExtPropertyPage.h"
#include "ExtStatic.h"
#include "Screen65PropertyPage.h"
#include "Screen65LineUps.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen65LineUps property page

IMPLEMENT_DYNCREATE(CScreen65LineUps, CPropertyPage)

/*------------------------------------------------------------------------------
	Method:   	CScreen65LineUps::CScreen65LineUps
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen65LineUps::CScreen65LineUps() :
	CMatchBitmapPropertyPage(CScreen65LineUps::IDD)
{
}

/*------------------------------------------------------------------------------
	Method:   	CScreen65LineUps::~CScreen65LineUps
	Access:    	public 
	Returns:   
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen65LineUps::~CScreen65LineUps()
{
}


BEGIN_MESSAGE_MAP(CScreen65LineUps, CBitmapPropertyPage)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen65LineUps message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen65LineUps::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65LineUps::OnPaint()
{
	// Offscreen buffer
	COffScreenBuffer& memDC = *CBitmapPropertyPage::GetMemDC();
	CBitmapPropertyPage::OnInitRedrawArea();
	CPlayer* pPlayer;
	CString strPrint;
	GetBasicDisplay().SetFontNormal();
    CRect DataRect(6, 90, 240, 280);
	for (int LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++)
	{
        pPlayer = &m_pCurrentMatchPlay->GetPlayer(HOME, LoopCount);
		memDC.SetTextColor(PosnColour[pPlayer->GetSkills().GetPosition()]);

		// Home player name
		DataRect.left = 104 - memDC.GetTextExtent(pPlayer->GetName()).cx  - 16;
		memDC.ExtTextOut(DataRect.left, DataRect.top, ETO_CLIPPED, DataRect, pPlayer->GetName(), pPlayer->GetName().GetLength(), null);

		// Home player skill
		if (pPlayer->IsAvailableForMatch() == false)
		{
			DataRect.left = 94;
			m_bmpPlayerStatus.DoDrawTransparent(memDC.operator HDC(), DataRect.left, DataRect.top, pPlayer->GetMatchStatus());
		}
		else
		{
			strPrint.Format(_T("%d"), m_pCurrentMatchPlay->GetPlayerSkill(HOME, LoopCount));
			DataRect.left = 102 - memDC.GetTextExtent(strPrint).cx / 2;
			memDC.ExtTextOut(DataRect.left, DataRect.top, ETO_CLIPPED, DataRect, strPrint, strPrint.GetLength(), null);
		}

		strPrint.Format(_T("%d"), LoopCount + 1);
		DataRect.left = (memDC.GetBufferWidth() /2) - memDC.GetTextExtent(strPrint).cx / 2;
		memDC.SetTextColor(BLACK);
		memDC.ExtTextOut(DataRect.left, DataRect.top, ETO_CLIPPED, DataRect, strPrint, strPrint.GetLength(), null);

        pPlayer = &m_pCurrentMatchPlay->GetPlayer(AWAY, LoopCount);
		memDC.SetTextColor(PosnColour[pPlayer->GetSkills().GetPosition()]);

		// Away player skill
		DataRect.left = 132;
		if (pPlayer->IsAvailableForMatch() == false)
		{
			DataRect.left = 130;
			m_bmpPlayerStatus.DoDrawTransparent(memDC.operator HDC(), DataRect.left, DataRect.top, pPlayer->GetMatchStatus());
		}
		else
		{
			strPrint.Format(_T("%d"), m_pCurrentMatchPlay->GetPlayerSkill(AWAY, LoopCount));
			memDC.ExtTextOut(DataRect.left, DataRect.top, ETO_CLIPPED, DataRect, strPrint, strPrint.GetLength(), null);
		}

		// Away player name
		DataRect.left = 136 + 16;
		memDC.ExtTextOut(DataRect.left, DataRect.top, ETO_CLIPPED, DataRect, pPlayer->GetName(), pPlayer->GetName().GetLength(), null);
		DataRect.top += 14;
    }
    GetBasicDisplay().RestoreFont();
	CMatchBitmapPropertyPage::OnPaint((bool)(theApp.GetApplicationPreferences().GetMatchSpeed() > 0));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65LineUps::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen65LineUps::OnInitDialog()
{
	HDC dc = ::GetDC(m_hWnd);
	m_bmpPlayerStatus.DoInitialise(dc, IDR_PNGPLAYERSTATUS, MAXPLAYERSTATUSES);
	::ReleaseDC(m_hWnd, dc);
	return CMatchBitmapPropertyPage::OnInitDialog(TEXT("Match Day"));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65LineUps::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65LineUps::OnLButtonUp(UINT nFlags, CPoint point)
{

	int iPlayerClickedOn = NOPLAYER;
	if (point.y > 90)
	{
		int iColumn = (point.y - 90) / 14;
		// clicked on team
		if (iColumn < PLAYERSINTEAM)
		{
			iPlayerClickedOn = iColumn;
		}
	}

	if (iPlayerClickedOn != NOPLAYER)
	{
		CPlayer* pPlayer;
		if (point.x < (CBitmapPropertyPage::GetMemDC()->GetBufferWidth() /2))
		{
			pPlayer = &m_pCurrentMatchPlay->GetPlayer(HOME, iPlayerClickedOn);
		}
		else
		{
			pPlayer = &m_pCurrentMatchPlay->GetPlayer(AWAY, iPlayerClickedOn);
		}
		WorldData().OnViewPlayerInformation(pPlayer);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen65LineUps::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen65LineUps::OnSize(UINT nType, int cx, int cy)
{
	CMatchBitmapPropertyPage::OnSize(nType, cx, cy);
	CMatchBitmapPropertyPage::GetBasicDisplay().SetDisplayHeader(true, IDS_SCREEN65);

	Sleep(1);
}