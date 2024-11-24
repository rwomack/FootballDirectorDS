
/*

	Screen41View.cpp : implementation file


	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "YouthGrid.h"
#include "Screen41View.h"
#include "FootballDirector.h"
#include "PopupPlayerMenu.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen41View dialog

IMPLEMENT_DYNCREATE(CScreen41View, CFormView)


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::CScreen41View
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen41View::CScreen41View()
	: CBitmapFormView(CScreen41View::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::~CScreen41View
	Access:    	public 
	Returns:   		Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen41View::~CScreen41View()
{
}


BEGIN_MESSAGE_MAP(CScreen41View, CBitmapFormView)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)
	ON_COMMAND(ID_ACTION1, OnViewDetails)
	ON_COMMAND(ID_ACTION2, OnRelease)
	ON_COMMAND(ID_ACTION3, OnOfferContract)
	ON_COMMAND(ID_ACTION4, OnHire)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen41View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::OnActionClicked
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen41View::OnActionClicked()
{
	if (m_TypeList.GetCurSel() == 0)
	{
		CBitmapFormView::OnActionMenu(IDR_SCREEN41EMPLOYEDMENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT3);
	}
	else
	{
		CBitmapFormView::OnActionMenu(IDR_SCREEN41AVAILABLEMENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT2);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::OnHire
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen41View::OnHire()
{
	m_YouthGrid.SetCurrentYouth();
	theApp.OnHireYouth();
	DoBuildGrid();
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::OnViewDetails
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen41View::OnViewDetails()
{
	m_YouthGrid.SetCurrentYouth();
	if (m_TypeList.GetCurSel() == 0)
	{
		theApp.OnViewOwnedYouthInformation();
	}
	else
	{
		theApp.OnViewNotOwnedYouthInformation();
	}
	DoBuildGrid();
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::OnRelease
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen41View::OnRelease()
{
	if (WorldData().GetCurrentUserClub()->AskSackYouth((CYouth*)m_YouthGrid.GetCurrentRowPointer()) == true)
	{
		DoBuildGrid();
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::OnOfferContract
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen41View::OnOfferContract()
{
	m_YouthGrid.SetCurrentYouth();
	theApp.OnPromoteYouth();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::OnInitialUpdate
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen41View::OnInitialUpdate()
{
	CBitmapFormView::OnInitialUpdate(TEXT("Youth Squad"), &theApp.GetBasicDisplay(), IDS_SCREEN41);
	m_YouthGrid.OnInitialUpdate(this, GetBasicDisplay());
	m_TypeList.DoInitialise(this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TypeList.DoAddItem(IDS_CLUBYOUTHS);
	m_TypeList.DoAddItem(IDS_AVAILYOUTHS);
	DoBuildGrid();
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::OnDraw
	Access:    	virtual protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen41View::OnDraw(CDC* pDC)
{
	CBitmapFormView::OnInitRedrawArea();

	m_YouthGrid.OnPaint();
	m_TypeList.OnPaint();	// Draws the top & bottom lines
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::DoBuildGrid
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen41View::DoBuildGrid()
{
	if (m_TypeList.GetCurSel() == 0)
	{
		m_YouthGrid.DoBuildGrid(WorldData().GetCurrentUserClub()->GetYouthList());
	}
	else
	{
		m_YouthGrid.DoBuildGrid(WorldData().GetCurrentUserClub()->GetActiveCountry().GetAvailableYouthList());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::OnVScroll
	Access:    	protected 
	Parameter: 	UINT nSBCode
	Parameter: 	UINT nPos
	Parameter: 	CScrollBar * pScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen41View::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_YouthGrid.OnVScroll(nSBCode, nPos, pScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen41View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_UP:
		case VK_DOWN:
			m_YouthGrid.OnKeyDown(nChar, nRepCnt, nFlags);
			ReDraw();
			break;

		case VK_LEFT:
		case VK_RIGHT:
			m_TypeList.OnKeyDown(nChar, nRepCnt, nFlags);
			DoBuildGrid();
			ReDraw();
			break;

		case VK_RETURN:
		case VK_F23:
			//OnOK();
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen41View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_TypeList.OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen41View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapFormView::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	int iRow = m_YouthGrid.GetCurrentRow(point);
	if (iRow != NOTFOUND)
	{
		if (m_TypeList.GetCurSel() == 0)
		{
			CPopupOwnedYouthMenu TheMenu;
			TheMenu.DoModal((CYouth*)m_YouthGrid.GetRowPointer(iRow), point);
		}
		else
		{
			CPopupNotOwnedYouthMenu TheMenu;
			TheMenu.DoModal((CYouth*)m_YouthGrid.GetRowPointer(iRow), point);
		}
		DoBuildGrid();
		ReDraw();
	}	
}


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT _nFlags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen41View::OnLButtonUp(UINT _nFlags, CPoint _Point)
{
	int ButtonPressed = CBitmapFormView::GetBasicDisplay()->OnLButtonUp(_nFlags, _Point);
	if (m_TypeList.OnLButtonUp(ButtonPressed) == true)
	{
		DoBuildGrid();
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen41View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen41View::OnSize(UINT nType, int cx, int cy)
{
	CBitmapFormView::OnSize(nType, cx, cy);
	if (CBitmapFormView::IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);
		CRect theWindow;
		GetClientRect(&theWindow);
		int CurrY = TOPY;
		CRect theRect;
		theRect.top = CurrY;
		CurrY += eUPDOWNHEIGHT;
		theRect.bottom = CurrY; 
		theRect.left = GetCentreX() - COMPETITIONNAMELENGTH;
		theRect.right = GetCentreX() + COMPETITIONNAMELENGTH;
		m_TypeList.MoveWindow(theRect);
		CurrY += 2;

		int RowsToDisplay = (theWindow.bottom - CurrY - 2) / m_YouthGrid.GetRowHeight();
		CRect theRect1(GetCentreX() - YGRIDWIDTH2, CurrY, GetCentreX() + YGRIDWIDTH2, CurrY + RowsToDisplay * m_YouthGrid.GetRowHeight());	
		m_YouthGrid.MoveWindow(theRect1);

		Sleep(1);
	}
}