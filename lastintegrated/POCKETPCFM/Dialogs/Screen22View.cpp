// Screen22View.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "FootballDirector.h"
#include "Screen22View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen22View dialog


IMPLEMENT_DYNCREATE(CScreen22View, CFormView)

/*------------------------------------------------------------------------------
	Method:   	CScreen22View::CScreen22View
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen22View::CScreen22View()
	: CBitmapFormView(CScreen22View::IDD) {};


BEGIN_MESSAGE_MAP(CScreen22View, CBitmapFormView)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()

	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)
	ON_COMMAND(ID_ACTION1, OnApplyLoan)
	ON_COMMAND(ID_ACTION2, OnRepayLoan)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen22View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen22View::OnActionClicked
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen22View::OnActionClicked()
{
	CBitmapFormView::OnActionMenu(IDR_SCREEN22MENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT2);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen22View::OnInitialUpdate
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen22View::OnInitialUpdate()
{
	CBitmapFormView::OnInitialUpdate(TEXT("Bank"), &theApp.GetBasicDisplay(), IDS_SCREEN22);

	CClub* pClub = WorldData().GetCurrentUserClub();
	EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
	m_TextSelector[eBORROW].DoInitialise(BUTTON1Y, IDS_BORROW, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TextSelector[eBORROW].SetStyle(COL_ALIGNC | COL_MONEY);
	m_TextSelector[eREPAY].DoInitialise(BUTTON4Y, IDS_REPAY, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TextSelector[eREPAY].SetStyle(COL_ALIGNC | COL_MONEY);
	m_CurrentTextSelector.OnInitialise(&m_TextSelector[0], eMAXSELECTOR);

	m_Grid.OnInitialUpdate(this, GetBasicDisplay());
	m_Grid.DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
	m_Grid.DoAddColumn(GetCentreX(), COL_ALIGNR);
	m_Grid.DoAddColumn(GetCentreX(), COL_ALIGNL);
	m_Grid.DoAddRows(3);
	DoUpdateLoandetails();
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen22View::DoUpdateLoandetails
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen22View::DoUpdateLoandetails()
{

	CClub* pClub = WorldData().GetCurrentUserClub();
	EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
	
	m_TextSelector[eREPAY].SetCurValue(pClub->GetRecommendedLoanRepayment());
	m_TextSelector[eBORROW].SetCurValue(pClub->GetFinancialLoan()->GetRecommendedLoanBorrow(&pClub->GetPopularity()));

	m_Grid.SetItemStrId(0, 0, IDS_LOANOUTSTANDING);
	m_Grid.SetItemStrId(1, 0, IDS_WEEKLYREPAYMENTS);
	m_Grid.SetItemStrId(2, 0, IDS_WEEKSREMAINING);
	m_Grid.SetItemText(0, 1, pClub->GetFinancialLoan()->LoanOwedStr(pClub->GetCountry()));
	m_Grid.SetItemText(1, 1, pClub->GetFinancialLoan()->LoanRepaymentStr(pClub->GetCountry()));
	m_Grid.SetItemNum(2, 1, pClub->GetFinancialLoan()->WeeksToRepayLoan(pClub->GetCountry()));
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen22View::OnDraw
	Access:    	virtual protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen22View::OnDraw(CDC* pDC)
{
	CBitmapFormView::OnInitRedrawArea();
	m_Grid.OnPaint();
	m_CurrentTextSelector.OnPaint();
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen22View::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen22View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapFormView::OnKeyDown(nChar, nRepCnt, nFlags);
	m_CurrentTextSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen22View::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen22View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapFormView::OnKeyUp(nChar, nRepCnt, nFlags);
//	m_CurrentTextSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen22View::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen22View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapFormView::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	CBitmapFormView::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen22View::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen22View::OnLButtonUp(UINT nFlags, CPoint point)
{
	int ButtonPressed = CBitmapFormView::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	switch (ButtonPressed)
	{
		case eBORROWDOWN:
		case eBORROWUP:
			m_TextSelector[eBORROW].OnLButtonUp(ButtonPressed);
			break;

		case eREPAYUP:
		case eREPAYDOWN:
			m_TextSelector[eREPAY].OnLButtonUp(ButtonPressed);
			break;
	}
	CBitmapFormView::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen22View::OnApplyLoan
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen22View::OnApplyLoan()
{
	double dValue = (double)m_TextSelector[eBORROW].GetCurValue();
	if (dValue > 0)
	{
		CClub* pClub = WorldData().GetCurrentUserClub();
		EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
		pClub->DoUserApplyFinancialLoan(dValue);
		DoUpdateLoandetails();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen22View::OnRepayLoan
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen22View::OnRepayLoan()
{
	double dValue = (double)m_TextSelector[eREPAY].GetCurValue();
	if (dValue > 0)
	{
		EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
		WorldData().GetCurrentUserClub()->DoUserRepayFinancialLoan(dValue);
		DoUpdateLoandetails();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen22View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen22View::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);
		int BorrowX = GetCentreX() + 10;
		m_TextSelector[eBORROW].MoveWindow();
		m_TextSelector[eREPAY].MoveWindow();

		m_Grid.MoveWindow(CRect(0, 140, GetCentreX() * 2, 144 + m_Grid.GetRowHeight() * 3));
		m_Grid.SetColumnWidth(0, GetCentreX() + 20);
		m_Grid.SetColumnWidth(1, GetCentreX() - 20);
		Sleep(1);
	}
}
