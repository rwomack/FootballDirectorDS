 // Screen27.cpp : implementation file
//
#include "stdafx.h"

#include "ExtRect.h"
#include "PopupPlayerMenu.h"
#include "UI.h"
#include "..\system\Screen\Win32\ExtPropertyPage.h"
#include "Screen27FormationPage.h"
#include "HumanManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen27FormationPage property page


IMPLEMENT_DYNCREATE(CScreen27FormationPage, CPropertyPage)


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationPage::CScreen27FormationPage
	Access:    	public 
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen27FormationPage::CScreen27FormationPage() 
	: CBitmapPropertyPage(CScreen27FormationPage::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationPage::~CScreen27FormationPage
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen27FormationPage::~CScreen27FormationPage()
{
}


BEGIN_MESSAGE_MAP(CScreen27FormationPage, CBitmapPropertyPage)
	ON_WM_PAINT()
    ON_CBN_SELCHANGE(IDC_COMBO27a, OnSelchangeFormationList)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

	CExtRect PitchRect(4, 68, 234, 68 + 134);
	CExtRect PlayerNameRect(8, 206, 120, 226);

/////////////////////////////////////////////////////////////////////////////
// CScreen27 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationPage::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationPage::OnPaint()
{
	// Offscreen buffer
	COffScreenBuffer& memDC = *CBitmapPropertyPage::GetMemDC();
	CBitmapPropertyPage::OnPaint();

	m_Pitch.Draw(memDC.operator HDC(), PitchRect.left, PitchRect.top);

	CTactic& pCurrentTactics = WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTactics();

	CString strPrint;
	GetBasicDisplay().SetFontNormal();

	COLORREF colors[2];
	memDC.SetTextColor(WHITE);
	CRect rect;
	for (int LoopCount = 1; LoopCount < PLAYERSINTEAM; LoopCount++)
	{
		rect = CRect(PitchRect.left + (pCurrentTactics.GetPlayerPositionX (LoopCount - 1) * iPitchXGap) - 3,
						 PitchRect.top + 2 + pCurrentTactics.GetPlayerPositionY (LoopCount - 1) * iPitchYGap,
						 PitchRect.left + pCurrentTactics.GetPlayerPositionX (LoopCount - 1) * iPitchXGap + 14,
						 PitchRect.top + 2 + pCurrentTactics.GetPlayerPositionY (LoopCount - 1) * iPitchYGap + 18);

		if (LoopCount != m_iPlayerSelected)
		{
			colors[0] = PosnColour[pCurrentTactics.PlayerPosition(LoopCount - 1)];
		}
		else
		{
			colors[0] = DARKGREEN;
		}
		memDC.StripedRect(1, rect, colors);

		strPrint.Format(TEXT("%d"), LoopCount + 1);
		rect.left = (PitchRect.left + (pCurrentTactics.GetPlayerPositionX (LoopCount - 1) * iPitchXGap) - 3) + 8 - memDC.GetTextExtent(strPrint).cx / 2;
		memDC.ExtTextOut(rect.left, rect.top, 0, rect, strPrint, strPrint.GetLength(), null);

	}

	memDC.SetBkMode(OPAQUE);
	if (m_iPlayerSelected != NOPLAYER)
	{
		rect.left = PlayerNameRect.left;
		rect.top = PlayerNameRect.top;
		CPlayer* pPlayer = &WorldData().GetCurrentUserClub()->GetPlayer(m_iPlayerSelected);
		memDC.FillSolidRect(rect.left, rect.top, rect.right - rect.left, 16, PALEYELLOW);
		memDC.SetTextColor(PosnColour[pPlayer->GetSkills().GetPosition()]);
		memDC.ExtTextOut(rect.left, rect.top, 0, rect, pPlayer->GetName(), pPlayer->GetName().GetLength(), null);
		rect.left = PlayerNameRect.right + 16;
		memDC.ExtTextOut(rect.left, rect.top, 0, rect, pPlayer->GetSkills().PositionStr(), pPlayer->GetSkills().PositionStr().GetLength(), null);
	}
	GetBasicDisplay().RestoreFont();
	memDC.Blit();
	CBitmapPropertyPage::OnInitRedrawArea();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationPage::OnSelchangeFormationList
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationPage::OnSelchangeFormationList()
{
    CComboBox* pViewMode = (CComboBox*)GetDlgItem(IDC_COMBO27a);
	ASSERT(pViewMode != null);
	WorldData().GetCurrentUser()->GetTacticsList().SetTacticsInUse(pViewMode->GetCurSel());
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationPage::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen27FormationPage::OnInitDialog()
{
	HDC dc = ::GetDC(m_hWnd);
	m_Pitch.SetBitmap(dc, IDR_PNGPITCH, TEXT("PNG"));
	::ReleaseDC(m_hWnd, dc);

	m_iPlayerSelected = NOPLAYER;

    CComboBox* pViewMode = (CComboBox*)GetDlgItem(IDC_COMBO27a);
	ASSERT(pViewMode != null);
	WorldData().GetCurrentUser()->GetTacticsList().DoBuildTitleAndFamiliarityStrList(pViewMode);

    return CBitmapPropertyPage::OnInitDialog(TEXT("Match Formation"));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationPage::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationPage::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Check pitch
	CPoint TempPoint(point);
	if (PitchRect.PtInRect(point) == TRUE)
	{
		TempPoint.x -= PitchRect.left;
		TempPoint.y -= PitchRect.top;
		TempPoint.x /= iPitchXGap;
		TempPoint.y /= iPitchYGap;
		CExtPoint TPoint;
		TPoint.SetX(TempPoint.x);
		TPoint.SetY(TempPoint.y);

		int iFound = WorldData().GetCurrentUser()->GetTacticsList().GetAt(((CComboBox*)GetDlgItem(IDC_COMBO27a))->GetCurSel()).PlayerAtPosition(TPoint);
		if (iFound != NOTFOUND)
		{
			m_iPlayerSelected = iFound + 1;		// Skip goalkeeper
		}
		ReDraw();
	}
	else
	{
		// Check player name
		if (PlayerNameRect.PtInRect(point) == TRUE)
		{
			CPopupOwnedPlayerMenu TheMenu;
			ASSERT(m_iPlayerSelected < PLAYERSINSQUAD);
			TheMenu.DoModal(&WorldData().GetCurrentUserClub()->GetPlayer(m_iPlayerSelected), point);
		}
	}
}
