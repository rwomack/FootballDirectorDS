// Screen18View.cpp : implementation file
//

#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "MatchInfo.h"
#include "FootballDirector.h"
#include "Screen18View.h"
#include "PopupPlayerMenu.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen18View

IMPLEMENT_DYNCREATE(CScreen18View, CFormView)

CScreen18View::CScreen18View()
	: CBitmapFormView(CScreen18View::IDD)
{
}


CScreen18View::~CScreen18View()
{
}


BEGIN_MESSAGE_MAP(CScreen18View, CBitmapFormView)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)
	ON_COMMAND(ID_ACTION1, OnViewMatchReport)
	ON_COMMAND(ID_ACTION2, OnViewOpponentSquad)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen18View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen18View::OnActionClicked
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18View::OnActionClicked()
{
	CBitmapFormView::OnActionMenu(IDR_SCREEN18MENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT2);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen18View::OnViewMatchReport
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18View::OnViewMatchReport()
{
	m_Grid.DoShowMatchSummary(m_Grid.GetCurrentRow());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen18View::OnViewOpponentSquad
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18View::OnViewOpponentSquad()
{
	m_Grid.DoShowOpponentSquad(m_Grid.GetCurrentRow());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen18View::OnDraw
	Access:    	virtual protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18View::OnPaint()
{
	CBitmapFormView::OnInitRedrawArea();
	m_Grid.OnPaint();
	CBitmapFormView::GetMemDC()->Blit();
}



/*------------------------------------------------------------------------------
	Method:   	CScreen18View::OnInitialUpdate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18View::OnInitialUpdate()
{
	CBitmapFormView::OnInitialUpdate(TEXT("Club Fixtures"), &theApp.GetBasicDisplay(), IDS_SCREEN18);
	m_Grid.SetMatchType(MATCHES_ALL);
	m_Grid.OnInitialUpdate(this, GetBasicDisplay(), WorldData().GetCurrentClub());
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen18View::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18View::OnLButtonDown(UINT nFlags, CPoint point)
{
	Default();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen18View::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18View::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Grid.OnLButtonUp(nFlags, point);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen18View::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_Grid.OnKeyDown(nChar, nRepCnt, nFlags);
	if (nChar == VK_UP || nChar == VK_DOWN)
	{
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen18View::OnVScroll
	Access:    	public 
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Parameter: 	CScrollBar * _ScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18View::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar)
{
	m_Grid.OnVScroll(_SBCode, _Pos, _ScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen18View::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen18View::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);
		CRect theWindow;
		GetClientRect(&theWindow);
		theWindow.top = TOPY;
		theWindow.bottom -= 2;
		m_Grid.MoveWindow(theWindow);
	}
	Sleep(1);
}