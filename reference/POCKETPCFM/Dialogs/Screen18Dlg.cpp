// Screen18Dlg.cpp : implementation file
//

#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "ClubFixtureGrid.h"
#include "PlayerNameTextRegion.h"
#include "Screen18Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen18Dlg


/*------------------------------------------------------------------------------
	Method:   	CScreen18Dlg::CScreen18Dlg
	Access:    	public 
	Parameter: 	CClub * pCurrentClub
	Parameter: 	const eClubResultsType eMatchType
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen18Dlg::CScreen18Dlg(CClub* pCurrentClub, const eClubResultsType eMatchType, CWnd* pParent /*=null*/)
	: m_pCurrentClub(pCurrentClub)
	, CBitmapDialog(CScreen18Dlg::IDD, pParent)
{
	m_Grid.SetMatchType(eMatchType);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen18Dlg::~CScreen18Dlg
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen18Dlg::~CScreen18Dlg()
{
}


BEGIN_MESSAGE_MAP(CScreen18Dlg, CBitmapDialog)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen18Dlg message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen18Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
//	m_Grid.OnPaint();
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen18Dlg::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen18Dlg::OnInitDialog()
{
	BOOL bRet = CBitmapDialog::OnInitDialog(IDS_HELPSCREEN18, IDS_SCREEN18);
	m_ClubName.DoInitialise(TOPY, this, GetBasicDisplay(), *m_pCurrentClub);

	m_Grid.OnInitialUpdate(this, GetBasicDisplay(), m_pCurrentClub);
	SetIsInitialised(true);
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen18Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	Default();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen18Dlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Grid.OnLButtonUp(nFlags, point);
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_Grid.OnKeyDown(nChar, nRepCnt, nFlags);
	if (nChar == VK_UP || nChar == VK_DOWN)
	{
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen18Dlg::OnVScroll
	Access:    	public 
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Parameter: 	CScrollBar * _ScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18Dlg::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar)
{
	m_Grid.OnVScroll(_SBCode, _Pos, _ScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen18Dlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18Dlg::OnSize(UINT nType, int cx, int cy)
{
	CBitmapDialog::OnSize(nType, cx, cy);

	if (IsInitialised() == true)
	{
		m_ClubName.MoveWindow();
		CRect theWindow;
		GetClientRect(&theWindow);
		theWindow.top = TOPY + 20;
		theWindow.bottom -= 2;
		m_Grid.MoveWindow(theWindow);
	}
	Sleep(1);
}
