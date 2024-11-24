#include "stdafx.h"

#include "UI.h"
#include "LoanPeriodSelector.h"


/*------------------------------------------------------------------------------
	Method:   	CLoanPeriodSelector::CLoanPeriodSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLoanPeriodSelector::CLoanPeriodSelector(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CLoanPeriodSelector::~CLoanPeriodSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLoanPeriodSelector::~CLoanPeriodSelector(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CLoanPeriodSelector::GetCurSel
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CLoanPeriodSelector::GetPeriod()
{
	return m_PeriodSelector.GetCurSel() + 5;
}


/*------------------------------------------------------------------------------
	Method:   	CLoanPeriodSelector::OnInitDialog
	Access:    	protected 
	Parameter: 	CBasicDisplay * _BasicDisplay
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CLoanPeriodSelector::OnInitDialog(CBasicDisplay* _BasicDisplay, CWnd* _Wnd)
{
	m_Static.DoInitialise(_BasicDisplay, IDS_LOAN, COL_ALIGNC);
	_BasicDisplay->DoAddTextRegion(&m_Static);
	m_PeriodSelector.DoInitialise(_Wnd, _BasicDisplay, IDR_PNGLEFT, IDR_PNGRIGHT);
	CString szStr;
	for (uint LoopCount = 1; LoopCount < 8; LoopCount++)
	{
		szStr.Format(IDS_LOANWEEKS, LoopCount + 4);
		m_PeriodSelector.DoAddItem(szStr);
	}
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CLoanPeriodSelector::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLoanPeriodSelector::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_PeriodSelector.OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CLoanPeriodSelector::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLoanPeriodSelector::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_PeriodSelector.OnKeyUp(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CLoanPeriodSelector::OnLButtonUp
	Access:    	protected 
	Parameter: 	const int _ButtonPressed
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLoanPeriodSelector::OnLButtonUp(const int _ButtonPressed)
{
	m_PeriodSelector.OnLButtonUp(_ButtonPressed);
}


/*------------------------------------------------------------------------------
	Method:   	CLoanPeriodSelector::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLoanPeriodSelector::OnPaint()
{
	m_PeriodSelector.OnPaint();
}


/*------------------------------------------------------------------------------
	Method:   	CLoanPeriodSelector::OnSize
	Access:    	protected 
	Parameter: 	CRect _Rect
	Parameter: 	const int _CentreX
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLoanPeriodSelector::OnSize(CRect _Rect, const int _CentreX)
{
	m_Static.MoveWindow(_Rect);
	_Rect.top += 20;
	_Rect.bottom = _Rect.top + eUPDOWNHEIGHT;
	_Rect.left = _CentreX - 35;
	_Rect.right = _CentreX + 35;
	m_PeriodSelector.MoveWindow(_Rect);
}