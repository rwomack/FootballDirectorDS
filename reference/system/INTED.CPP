
/*

	File:- IntEdit.cpp

	Extension to CEdit class

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"

#include "IntEd.h"
#include <RoundMoney.h>


BEGIN_MESSAGE_MAP(CIntEdit, CEdit)
    ON_WM_CHAR()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CIntSIPEdit, CIntEdit)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CIntSIPUnderLinedEdit, CIntSIPEdit)
    ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS() 
END_MESSAGE_MAP()


 /*------------------------------------------------------------------------------
 	Method:   	CIntEdit::DoIncrementMoneyStep
 	Access:    	public 
 	Returns:   	void
 	Qualifier: 	
 	Purpose:
 ------------------------------------------------------------------------------*/
 void CIntEdit::DoIncrementMoneyStep()
{
	SetValue(static_cast<uint>(CRoundMoney::RoundMoneyIncrement((uint)GetValue())));
	SetSel(-1, -1);
}


/*------------------------------------------------------------------------------
	Method:   	CIntEdit::DoDecrementMoneyStep
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CIntEdit::DoDecrementMoneyStep()
{
	SetValue(static_cast<uint>(CRoundMoney::RoundMoneyDecrement((uint)GetValue())));
	SetSel(-1, -1);
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CIntSIPEdit::OnSetFocus
	Access:    	public 
	Parameter: 	CWnd * pOldWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CIntSIPEdit::OnSetFocus(CWnd* pOldWnd)
{
#ifdef POCKETPC
	SHSipPreference(CWnd::GetSafeHwnd(), SIP_UP);
#endif
	CEdit::OnSetFocus(pOldWnd);
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CIntEdit::OnChar
	Access:    	public 
	Parameter: 	UINT nChar - character code of the hit key.
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:	Message handler member function for the WM_CHAR message.
				Called whenever a character key is hit while the edit control 
				has the input focus
------------------------------------------------------------------------------*/
void CIntEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // Check to see if the character is valid for a integer number
    // (the character must be a 0-9) or a control character was entered.
	// If the character is valid, pass it through to the base class
	// message handler, if the character is invalid, drop it.
    if (isdigit(nChar) || (nChar < 0x20))
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CIntEdit::GetValue
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:	Extract the integer value represented by the text 
				in the edit control.
------------------------------------------------------------------------------*/
int CIntEdit::GetValue()
{
#ifdef UNICODE
	wchar_t str[256];
#else
	char str[256];
#endif
	GetWindowText(str, 255);
#ifdef UNICODE
	return _wtoi(str);
#else
	return atoi(str);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CIntEdit::SetValue
	Access:    	public 
	Parameter: 	const int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CIntEdit::SetValue(const int _Value)
{
	CString str;
	str.Format(TEXT("%d"), _Value);
	CEdit::SetWindowText(str);
	Invalidate();
}


// ---------------------------------------------------------------------------
void CIntEdit::SetValue(const uint x)
{
	CString str;
	str.Format(TEXT("%u"), x);
	CEdit::SetWindowText(str);
	Invalidate();
}


// Conversion from CIntEdit to int.
// ---------------------------------------------------------------------------
CIntEdit::operator int()
{
	return(GetValue());
}
/////////////////////////////////////////////////////////////////////////////


// ---------------------------------------------------------------------------
void CIntSIPUnderLinedEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CString str;
	GetWindowText(str);
	dc.SetTextColor(RGB(0, 0, 0));
	dc.ExtTextOut(0, 0, 0, CRect(0, 0, 200, 15), str, str.GetLength(), null);
	DrawUnderLine(&dc);
}


// ---------------------------------------------------------------------------
void CIntSIPUnderLinedEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
   Default(); 
}


// ---------------------------------------------------------------------------
void CIntSIPUnderLinedEdit::OnRecognizeGesture(NMHDR* pNMHDR, LRESULT* pResult) 
{
   *pResult = TRUE; 
} 


// ---------------------------------------------------------------------------
BOOL CIntSIPUnderLinedEdit::OnEraseBkgnd(CDC* pDC)
{
	CRect ClientRect;

	GetClientRect(&ClientRect);
	pDC->ExtTextOut(0, 0, ETO_OPAQUE, ClientRect, null, 0, null);
	DrawUnderLine(pDC);

	return TRUE;
}


// ---------------------------------------------------------------------------
void CIntSIPUnderLinedEdit::DrawUnderLine(CDC* pDC)
{
	CPen borderpen(PS_SOLID, 1, m_colorUnderLine);
	CPen* oldPen = pDC->SelectObject(&borderpen);
	CRect rc;
	// Get the client area of the edit control
	GetClientRect(&rc);
	pDC->MoveTo(0, m_LineY);
	pDC->LineTo(rc.right, m_LineY);		// Top
	pDC->SelectObject(oldPen);
}


// ---------------------------------------------------------------------------
void CIntSIPUnderLinedEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);
	SetSel(-1, -1);
	Invalidate(FALSE);
}


// ---------------------------------------------------------------------------
void CIntSIPUnderLinedEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	// Force a repaint
	Invalidate(FALSE);
}
/////////////////////////////////////////////////////////////////////////////
