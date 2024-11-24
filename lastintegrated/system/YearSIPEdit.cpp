
/*

	File:- YearSIPEdit.cpp

	Extension to CEdit class

	(c) RW Software 1994 - 2005

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"
#include "afxwin.h"
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

BEGIN_MESSAGE_MAP(C1904YearSIPUnderLinedEdit, C1904YearSIPEdit)
    ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS() 
END_MESSAGE_MAP()



 //---------------------------------------------------------------------------------------
int C1904YearSIPEdit::GetValue(void)
{
	int nValue = CIntEdit::GetValue();
	if (nValue > 1904)
	{
		return nValue - 1904;
	}
	return 0;
}


//---------------------------------------------------------------------------------------
void C1904YearSIPEdit::SetValue(const int x)
{
	if (x > 0)
	{
		CIntEdit::SetValue(x + 1904);
	}
	else
	{
		CEdit::SetWindowText(TEXT(""));
	}
	Invalidate();
}
/////////////////////////////////////////////////////////////////////////////


// ---------------------------------------------------------------------------
void C1904YearSIPUnderLinedEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CString str;
	GetWindowText(str);
	dc.SetTextColor(RGB(0, 0, 0));
	dc.ExtTextOut(0, 0, 0, CRect(0, 0, 200, 15), str, str.GetLength(), NULL);
	DrawUnderLine(&dc);
}


// ---------------------------------------------------------------------------
void C1904YearSIPUnderLinedEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
   Default(); 
}


// ---------------------------------------------------------------------------
void C1904YearSIPUnderLinedEdit::OnRecognizeGesture(NMHDR* pNMHDR, LRESULT* pResult) 
{
   *pResult = TRUE; 
} 


// ---------------------------------------------------------------------------
BOOL C1904YearSIPUnderLinedEdit::OnEraseBkgnd(CDC* pDC)
{
	CRect ClientRect;

	GetClientRect(&ClientRect);
	pDC->ExtTextOut(0, 0, ETO_OPAQUE, ClientRect, NULL, 0, NULL);
	DrawUnderLine(pDC);

	return TRUE;
}


// ---------------------------------------------------------------------------
void C1904YearSIPUnderLinedEdit::DrawUnderLine(CDC* pDC)
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
void C1904YearSIPUnderLinedEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);
	// Force a repaint
	Invalidate(FALSE);
}


// ---------------------------------------------------------------------------
void C1904YearSIPUnderLinedEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	// Force a repaint
	Invalidate(FALSE);
}