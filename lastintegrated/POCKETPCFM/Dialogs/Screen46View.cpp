
 /*

	Screen46View.cpp : Monthly Awards CView implementation file

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
 */


#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "FootballDirector.h"
#include "Screen46View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen46View dialog

IMPLEMENT_DYNCREATE(CScreen46View, CFormView)


/*------------------------------------------------------------------------------
	Method:   	CScreen46View::CScreen46View
	Access:    	public 
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen46View::CScreen46View()
    : CGridDivisionSelectorBitmapFormView(CScreen46View::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen46View::~CScreen46View
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen46View::~CScreen46View()
{
}


BEGIN_MESSAGE_MAP(CScreen46View, CGridDivisionSelectorBitmapFormView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen46View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen46View::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen46View::OnPaint()
{
	CGridDivisionSelectorBitmapFormView::OnPaint();
    if (m_DivisionSelector.GetCurrentDivision().DoCountMOMWinners() == 0)
	{
		COffScreenBuffer& memDC = *CBitmapFormView::GetMemDC();
		memDC.SetTextColor(BLACK);
		memDC.DisplayNoData(IDS_NODATA);
    }
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen46View::OnInitialUpdate
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen46View::OnInitialUpdate()
{
	CGridDivisionSelectorBitmapFormView::OnInitialUpdate(TEXT("Monthly Awards"), &theApp.GetBasicDisplay(), IDS_SCREEN46);
	m_Grid.HideHilite();
	OnSelchangeDivisionlist();
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen46View::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen46View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CGridDivisionSelectorBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
			OnSelchangeDivisionlist();
			ReDraw();
			break;

		case VK_RETURN:
		case VK_SELECT:
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen46View::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT _Flags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen46View::OnLButtonUp(UINT _Flags, CPoint _Point)
{
	int ButtonPressed = GetBasicDisplay()->OnLButtonUp(_Flags, _Point);
	if (m_DivisionSelector.OnLButtonUp(ButtonPressed) == true)
	{
		OnSelchangeDivisionlist();
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen46View::OnSelchangeDivisionlist
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen46View::OnSelchangeDivisionlist()
{
	m_Grid.RemoveAllData(true);

	m_Grid.DoAddColumn(30);	// Month
	m_Grid.DoAddColumn(95);	// Manager Name
	m_Grid.DoAddColumn(95);	// Club Name

	CDivision& pDivision = m_DivisionSelector.GetCurrentDivision();
	int HowMany = pDivision.DoCountMOMWinners();
 	m_Grid.DoAddRows(HowMany);
	for (int LoopCount = 0; LoopCount < HowMany; LoopCount++)
	{
		CManagerOfTheMonth& pManager = pDivision.GetManagerOfMonth(LoopCount);
		m_Grid.SetItemText(LoopCount, 0, pManager.MonthStr());
		m_Grid.SetItemText(LoopCount, 1, pManager.ManagerStr());
		m_Grid.SetItemText(LoopCount, 2, pManager.ClubStr());
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen46View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen46View::OnSize(UINT nType, int cx, int cy)
{
	CGridDivisionSelectorBitmapFormView::OnSize(nType, cx, cy);
	if (IsInitialised() == true)
	{
		CRect theWindow;
		GetClientRect(&theWindow);
		int RowsToDisplay = (theWindow.bottom - DIVISIONSELECTOR_TOPY - 2) / m_Grid.GetRowHeight();
		CRect theRect(GetCentreX() - 118, DIVISIONSELECTOR_TOPY, GetCentreX() + 118 - m_Grid.GetScrollBarWidth(), DIVISIONSELECTOR_TOPY + RowsToDisplay * m_Grid.GetRowHeight());	
		m_Grid.MoveWindow(theRect);

		int DivListY = TOPY;
		m_DivisionSelector.MoveWindow(CRect(GetCentreX() - 55, DivListY, GetCentreX() + 55, theWindow.bottom - 2));

		Sleep(1);
	}
}