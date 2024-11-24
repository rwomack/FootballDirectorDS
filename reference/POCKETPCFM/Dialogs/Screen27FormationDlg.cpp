// Screen27.cpp : implementation file
//
#include "stdafx.h"

#include "PopupPlayerMenu.h"
#include "UI.h"
#include "Dialog.h"
#include "HumanManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen27FormationDlg property page

static CExtRect PlayerNameRect(8, 234, 120, 256);


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationDlg::CScreen27FormationDlg
	Access:    	public 
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen27FormationDlg::CScreen27FormationDlg() 
	: CInMatchActionBitmapDialog(CScreen27FormationDlg::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationDlg::~CScreen27FormationDlg
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen27FormationDlg::~CScreen27FormationDlg()
{
}


BEGIN_MESSAGE_MAP(CScreen27FormationDlg, CInMatchActionBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen27FormationDlg message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationDlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationDlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();

	COffScreenBuffer& memDC = *CBitmapDialog::GetMemDC();
	m_Pitch.Draw(memDC.operator HDC(), m_PitchRect.left, m_PitchRect.top);

	CString strPrint;
	GetBasicDisplay()->SetFontBold();

	COLORREF colors[2];
	memDC.SetTextColor(WHITE);
	CRect rect;
	GetClientRect(&rect);
	CTactic& pCurrentTactics = WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTactics();
	for (int LoopCount = 1; LoopCount < PLAYERSINTEAM; LoopCount++)
	{
		rect = CRect(m_PitchRect.left + (pCurrentTactics.GetPlayerPositionX (LoopCount - 1) * iPitchXGap) - 8,
			m_PitchRect.top + 5 + pCurrentTactics.GetPlayerPositionY (LoopCount - 1) * iPitchYGap,
			m_PitchRect.left + pCurrentTactics.GetPlayerPositionX (LoopCount - 1) * iPitchXGap + 20,
			m_PitchRect.top + 5 + pCurrentTactics.GetPlayerPositionY (LoopCount - 1) * iPitchYGap + 18);

		if (LoopCount != m_iPlayerSelected)
		{
			colors[0] = PosnColour[pCurrentTactics.PlayerPosition(LoopCount - 1)];
		}
		else
		{
			colors[0] = DARKGREEN;
		}
		memDC.SetTextColor(colors[0]);
		memDC.Ellipse(rect);

		strPrint.Format(_T("%d"), LoopCount + 1);
		rect.left = (m_PitchRect.left + (pCurrentTactics.GetPlayerPositionX (LoopCount - 1) * iPitchXGap) - 3) + 8 - memDC.GetTextExtent(strPrint).cx / 2;
		memDC.ExtTextOut(rect.left, rect.top, ETO_CLIPPED, rect, strPrint, strPrint.GetLength(), null);

	}

	if (m_iPlayerSelected != NOPLAYER)
	{
		rect.left = PlayerNameRect.left;
		rect.top = PlayerNameRect.top;
		CPlayer* pPlayer = &WorldData().GetCurrentUserClub()->GetPlayer(m_iPlayerSelected);
		memDC.FillSolidRect(rect.left, rect.top, rect.right - rect.left, 16, PALEYELLOW);
		memDC.SetTextColor(PosnColour[pPlayer->GetSkills().GetPosition()]);
		memDC.ExtTextOut(rect.left, rect.top, ETO_CLIPPED, rect, pPlayer->GetName(), pPlayer->GetName().GetLength(), null);
		rect.left = PlayerNameRect.right + 16;
		memDC.ExtTextOut(rect.left, rect.top, ETO_CLIPPED, rect, pPlayer->GetSkills().PositionStr(), pPlayer->GetSkills().PositionStr().GetLength(), null);
	}
	GetBasicDisplay()->RestoreFont();

	m_FormationSelector.OnPaint();
	memDC.Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationDlg::OnSelchangeFormationList
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationDlg::OnSelchangeFormationList()
{
	WorldData().GetCurrentUser()->GetTacticsList().SetTacticsInUse(m_FormationSelector.GetCurSel());
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationDlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen27FormationDlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN27F, IDS_FORMATION);

	HDC dc = ::GetDC(m_hWnd);
	m_Pitch.SetBitmap(dc, IDR_PNGPITCH, TEXT("PNG"));
	::ReleaseDC(m_hWnd, dc);

	m_iPlayerSelected = NOPLAYER;

	m_FormationSelector.DoInitialise(this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	WorldData().GetCurrentUser()->GetTacticsList().DoBuildTitleAndFamiliarityStrList(m_FormationSelector);
	m_FormationSelector.SetCurSel(WorldData().GetCurrentUser()->GetTacticsList().GetTacticsInUse());
	SetIsInitialised(true);

    return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationDlg::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	m_FormationSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	OnSelchangeFormationList();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationDlg::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
	m_FormationSelector.OnKeyUp(nChar, nRepCnt, nFlags);
	OnSelchangeFormationList();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationDlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	int ButtonPressed = CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	if (m_FormationSelector.OnLButtonUp(ButtonPressed))
	{
		OnSelchangeFormationList();
	}

	// Check pitch
	CPoint TempPoint(point);
	if (m_PitchRect.PtInRect(point) == true)
	{
		TempPoint.x -= m_PitchRect.left;
		TempPoint.y -= m_PitchRect.top;
		TempPoint.x /= iPitchXGap;
		TempPoint.y /= iPitchYGap;
		CExtPoint TPoint;
		TPoint.SetX (TempPoint.x);
		TPoint.SetY (TempPoint.y);

		ushort iFound = WorldData().GetCurrentUser()->GetTacticsList().GetAt(m_FormationSelector.GetCurSel()).PlayerAtPosition(TPoint);
		if (iFound != wNOTFOUND)
		{
			m_iPlayerSelected = iFound + 1;		// Skip goalkeeper
		}
		ReDraw();
	}
	else
	{
		// Check player name
		if (PlayerNameRect.PtInRect(point) == true)
		{
			CPopupOwnedPlayerMenu TheMenu;
			EXT_ASSERT(m_iPlayerSelected < PLAYERSINSQUAD);
			TheMenu.DoModal(&WorldData().GetCurrentUserClub()->GetPlayer(m_iPlayerSelected), point);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationDlg::OnLButtonDown
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationDlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationDlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		m_PitchRect.left = GetCentreX() - 113;
		m_PitchRect.top = TOPY + 24;
		m_PitchRect.right = GetCentreX() + 113;
		m_PitchRect.bottom = TOPY + 24 + 116;

		// Formation title Listbox
		CRect theRect;
		GetClientRect(&theRect);
		theRect.top = TOPY + 2;
		theRect.bottom = theRect.top + 18;
		theRect.left = GetCentreX() - 50;
		theRect.right = theRect.left + 100;
		m_FormationSelector.MoveWindow(theRect);
	}
	Sleep(1);
}
