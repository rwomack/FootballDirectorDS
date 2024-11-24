// Screen27FormationView.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "PopupPlayerMenu.h"
#include "Screen27FormationView.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen27FormationView

	static CExtRect PlayerNameRect(8, 234, 120, 256);

	


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationView::CScreen27FormationView
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen27FormationView::CScreen27FormationView()
	: CBitmapFormView(CScreen27FormationView::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationView::~CScreen27FormationView
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen27FormationView::~CScreen27FormationView()
{
}


BEGIN_MESSAGE_MAP(CScreen27FormationView, CBitmapFormView)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen27FormationView message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationView::OnDraw
	Access:    	protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationView::OnDraw(CDC* pDC)
{
	CBitmapFormView::OnInitRedrawArea();

	COffScreenBuffer& memDC = *CBitmapFormView::GetMemDC();
	m_Pitch.Draw(memDC.operator HDC(), m_PitchRect.left, m_PitchRect.top);

	CString strPrint;
	GetBasicDisplay()->SetFontBold();

	memDC.SetTextColor(WHITE);
	CRect rect;
	GetClientRect(&rect);
	CTactic& pCurrentTactics = WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTactics();
	for (int LoopCount = 1; LoopCount < PLAYERSINTEAM; LoopCount++)
	{
 		if (LoopCount == m_iPlayerSelected)
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
		m_pngNumbers.DoDrawTransparent(CBitmapFormView::GetMemDC()->operator HDC(), m_PitchRect.left + (pCurrentTactics.GetPlayerPositionX (LoopCount - 1) * iPitchXGap) - 8, 
			m_PitchRect.top + 5 + pCurrentTactics.GetPlayerPositionY (LoopCount - 1) * iPitchYGap, LoopCount + (pCurrentTactics.PlayerPosition(LoopCount - 1) - 1) * PLAYERSINTEAM);
	}
    GetBasicDisplay()->RestoreFont();

	m_FormationSelector.OnPaint();
	memDC.Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationView::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
	m_FormationSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	OnSelchangeFormationList();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationView::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapFormView::OnKeyUp(nChar, nRepCnt, nFlags);
	m_FormationSelector.OnKeyUp(nChar, nRepCnt, nFlags);
	OnSelchangeFormationList();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationView::OnSelchangeFormationList
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationView::OnSelchangeFormationList()
{
	WorldData().GetCurrentUser()->GetTacticsList().SetTacticsInUse(m_FormationSelector.GetCurSel());
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationView::OnInitialUpdate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationView::OnInitialUpdate()
{
    CBitmapFormView::OnInitialUpdate(TEXT("Match Formation"), &theApp.GetBasicDisplay(), IDS_FORMATION);

	HDC dc = ::GetDC(m_hWnd);
	m_Pitch.SetBitmap(dc, IDR_PNGPITCH, _T("PNG"));
	m_pngNumbers.DoInitialise(dc, IDR_PNGNUMBERS, PLAYERSINTEAM * 3);
	::ReleaseDC(m_hWnd, dc);

	m_iPlayerSelected = NOPLAYER;

	m_FormationSelector.DoInitialise(this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	WorldData().GetCurrentUser()->GetTacticsList().DoBuildTitleAndFamiliarityStrList(m_FormationSelector);
	m_FormationSelector.SetCurSel(WorldData().GetCurrentUser()->GetTacticsList().GetTacticsInUse());
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationView::OnActivateView
	Access:    	protected 
	Parameter: 	BOOL bActivate
	Parameter: 	CView * pActivateView
	Parameter: 	CView * pDeactiveView
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if (bActivate == TRUE)
	{
		m_FormationSelector.SetCurSel(WorldData().GetCurrentUser()->GetTacticsList().GetTacticsInUse());
	}
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationView::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationView::OnLButtonUp(UINT nFlags, CPoint point)
{
	int ButtonPressed = CBitmapFormView::GetBasicDisplay()->OnLButtonUp(nFlags, point);
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
	Method:   	CScreen27FormationView::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapFormView::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27FormationView::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27FormationView::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);
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
