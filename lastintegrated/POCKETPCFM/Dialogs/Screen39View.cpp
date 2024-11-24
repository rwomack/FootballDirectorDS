
/*

	Screen39View.cpp : implementation file


	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "EmployeeGrid.h"
#include "Screen39View.h"
#include "PopupPlayerMenu.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CScreen39View 


IMPLEMENT_DYNCREATE(CScreen39View, CFormView)


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::CScreen39View
	Access:    	public 
	Returns:
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen39View::CScreen39View()
	: CBitmapFormView(CScreen39View::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::~CScreen39View
	Access:    	public 
	Returns:   		Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen39View::~CScreen39View()
{
};


BEGIN_MESSAGE_MAP(CScreen39View, CBitmapFormView)
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
// CScreen39View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::OnActionClicked
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen39View::OnActionClicked()
{
	if (m_TypeList.GetCurSel() == 0)
	{
		CBitmapFormView::OnActionMenu(IDR_SCREEN39EMPLOYEDMENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT3);
	}
	else
	{
		CBitmapFormView::OnActionMenu(IDR_SCREEN39AVAILABLEMENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT2);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::OnHire
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen39View::OnHire()
{
	WorldData().SetCurrentEmployee((CEmployee*)m_EmployeeGrid.GetCurrentRowPointer());
	theApp.OnHireEmployee();
	DoBuildGrid();
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::OnViewDetails
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen39View::OnViewDetails()
{
	m_EmployeeGrid.SetCurrentEmployee();
	if (m_TypeList.GetCurSel() == 0)
	{
		theApp.OnViewOwnedEmployeeInformation();
	}
	else
	{
		theApp.OnViewNotOwnedEmployeeInformation();
	}
	DoBuildGrid();
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::OnRelease
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen39View::OnRelease()
{
	if (WorldData().GetCurrentUserClub()->AskSackEmployee((CEmployee*)m_EmployeeGrid.GetCurrentRowPointer()) == true)
	{
		DoBuildGrid();
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::OnOfferContract
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen39View::OnOfferContract()
{
	WorldData().SetCurrentEmployee((CEmployee*)m_EmployeeGrid.GetCurrentRowPointer());
	theApp.OnNegotiateEmployeeContract();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::OnInitialUpdate
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen39View::OnInitialUpdate()
{
	CBitmapFormView::OnInitialUpdate(TEXT("Job Centre"), &theApp.GetBasicDisplay(), IDS_SCREEN39);
	m_EmployeeGrid.OnInitialUpdate(this, GetBasicDisplay());
	m_TypeList.DoInitialise(this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TypeList.DoAddItem(IDS_CLUBEMPLOYEES);
	m_TypeList.DoAddItem(IDS_AVAILEMPLOYEES);

	DoBuildGrid();
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::OnDraw
	Access:    	virtual protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen39View::OnDraw(CDC* pDC)
{
	CBitmapFormView::OnInitRedrawArea();

	m_EmployeeGrid.OnPaint();
	m_TypeList.OnPaint();	// Draws the top & bottom lines
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::DoBuildGrid
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen39View::DoBuildGrid()
{
	if (m_TypeList.GetCurSel() == 0)
	{
		m_EmployeeGrid.DoBuildGrid(WorldData().GetCurrentUserClub()->GetEmployeeList());
	}
	else
	{
		m_EmployeeGrid.DoBuildGrid(WorldData().GetCurrentUserClub()->GetActiveCountry().GetAvailableEmployeeList());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::OnVScroll
	Access:    	protected 
	Parameter: 	UINT nSBCode
	Parameter: 	UINT nPos
	Parameter: 	CScrollBar * pScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen39View::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_EmployeeGrid.OnVScroll(nSBCode, nPos, pScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen39View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_UP:
	case VK_DOWN:
		m_EmployeeGrid.OnKeyDown(nChar, nRepCnt, nFlags);
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
	Method:   	CScreen39View::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen39View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_TypeList.OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen39View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapFormView::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	int iRow = m_EmployeeGrid.GetCurrentRow(point);
	if (iRow != NOTFOUND)
	{
		if (m_TypeList.GetCurSel() == 0)
		{
			CPopupOwnedEmployeeMenu TheMenu;
			TheMenu.DoModal((CEmployee*)m_EmployeeGrid.GetRowPointer(iRow), point);
		}
		else
		{
			CPopupNotOwnedEmployeeMenu TheMenu;
			TheMenu.DoModal((CEmployee*)m_EmployeeGrid.GetRowPointer(iRow), point);
		}
		DoBuildGrid();
		ReDraw();
	}	
}


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT _nFlags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen39View::OnLButtonUp(UINT _nFlags, CPoint _Point)
{
	int ButtonPressed = CBitmapFormView::GetBasicDisplay()->OnLButtonUp(_nFlags, _Point);
	if (m_TypeList.OnLButtonUp(ButtonPressed) == true)
	{
		DoBuildGrid();
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen39View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen39View::OnSize(UINT nType, int cx, int cy)
{
	if (CBitmapFormView::IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);
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

		int RowsToDisplay = (theWindow.bottom - CurrY - 2) / m_EmployeeGrid.GetRowHeight();
		CRect theRect1(GetCentreX() - GRIDWIDTH2, CurrY, GetCentreX() + GRIDWIDTH2, CurrY + RowsToDisplay * m_EmployeeGrid.GetRowHeight());	
		m_EmployeeGrid.MoveWindow(theRect1);

		Sleep(1);
	}
}