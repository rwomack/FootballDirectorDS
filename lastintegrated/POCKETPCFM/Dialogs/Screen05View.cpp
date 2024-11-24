

/********************************************************************

	filename: 	D:\PROJECTS\POCKETPCFM\Screen05View.cpp

	purpose: Squad Display Screen

	author:		Roger Womack

	(c) Rw Software 2003 - 2008
		

*********************************************************************/

#include "stdafx.h"

#include "UI.h"
#include "Form.h"

#include "Screen05Base.h"
#include "Screen05View.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen05View

IMPLEMENT_DYNCREATE(CScreen05View, CFormView)


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::CScreen05View
	Access:    	public 
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen05View::CScreen05View() 
	: CBitmapFormView(IDD)
{

}


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::~CScreen05View
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen05View::~CScreen05View() 
{
};


BEGIN_MESSAGE_MAP(CScreen05View, CBitmapFormView)
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()

	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)
	ON_COMMAND(ID_ACTION1, OnViewDetails)
	ON_COMMAND(ID_ACTION2, OnOfferContract)
	ON_COMMAND(ID_ACTION3, OnSkillView)
	ON_COMMAND(ID_ACTION4, OnStatusView)
	ON_COMMAND(ID_ACTION5, OnContractView)
	ON_COMMAND(ID_ACTION6, OnStatsView)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CScreen05View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::OnDraw
	Access:    	virtual protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05View::OnDraw(CDC* pDC)
{
	CBitmapFormView::OnInitRedrawArea();
	m_Screen.OnDraw(CBitmapFormView::GetBasicDisplay());
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::DoInitialiseGrid
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05View::DoInitialiseGrid()
{
	m_Screen.DoInitialiseGrid();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::OnInitialUpdate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05View::OnInitialUpdate()
{
	CBitmapFormView::OnInitialUpdate(TEXT("Squad Menu"), &theApp.GetBasicDisplay(), IDS_SCREEN05);
	m_Screen.DoInitialise(m_hWnd, this, GetBasicDisplay());

	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::OnActionClicked
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05View::OnActionClicked()
{
	CBitmapFormView::OnActionMenu(IDR_SCREEN05MENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT6);
}


void CScreen05View::OnOfferContract()
{
//	WorldData().OnViewPlayerDetails((CPlayer*)m_Screen.GetGrid().GetCurrentRowPointer());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::OnViewDetails
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05View::OnViewDetails()
{
	WorldData().OnViewPlayerDetails((CPlayer*)m_Screen.GetGrid().GetCurrentRowPointer());
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::OnSkillView
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05View::OnSkillView()
{
	m_Screen.OnSelchangeTab(eSKILL);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::OnStatusView
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05View::OnStatusView()
{
	m_Screen.OnSelchangeTab(eSTATUS);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::OnContractView
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05View::OnContractView()
{
	m_Screen.OnSelchangeTab(eFINANCE);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::OnStatsView
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05View::OnStatsView()
{
	m_Screen.OnSelchangeTab(eSTATS);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_Screen.OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05View::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Screen.OnLButtonDown(m_hWnd, nFlags, point);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::OnPlayerPopupMenu
	Access:    	protected 
	Parameter: 	UINT iColumn
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05View::OnPlayerPopupMenu(UINT iColumn, CPoint point)
{
	m_Screen.OnPlayerPopupMenu(iColumn, point);
	m_Screen.DoInitialiseGrid();		// Player may have been sacked
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::OnVScroll
	Access:    	protected 
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Parameter: 	CScrollBar * _ScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05View::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar)
{
	m_Screen.OnVScroll(_SBCode, _Pos, _ScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen05View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen05View::OnSize(UINT nType, int cx, int cy)
{
	CBitmapFormView::OnSize(nType, cx, cy);

	if (IsInitialised() == true)
	{
		CRect theWindow;
		GetClientRect(&theWindow);
		m_Screen.OnSize(theWindow);

		Sleep(1);
	}
}