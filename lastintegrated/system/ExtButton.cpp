/*

	File:- ExtButton.cpp

	Extension to CButton class

	(c) RW Software 2002 - 2003

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"
#include "ExtButton.h"


BEGIN_MESSAGE_MAP(CExtButton, CButton)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// Elminates PocketPC MFC tap & hold
// ---------------------------------------------------------------------------
void CExtButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
   Default(); 
}
/////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CExtColorButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()


 //--------------------------------------------------------
void CExtColorButton::OnInit(const UINT nID, CWnd* pParent, CRect rectWindow, 
						const COLORREF ForeColor, const COLORREF BackColor,
						const COLORREF ForeColorFocus, const COLORREF BackColorFocus)
{
	SubclassDlgItem(nID, pParent);
	MoveWindow(rectWindow);

	SetColors(BackColor, ForeColor);
	SetNormalColors(ForeColor, BackColor);
	SetFocusColors(ForeColorFocus, BackColorFocus);
		
	m_Font.CreateFont(13, 0, 0, 0, \
					  FW_MEDIUM, \
					  FALSE, FALSE, 0, \
					  ANSI_CHARSET, \
					  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, \
					  DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
					  _T("Tahoma Bold"));
	SetFont(&m_Font);
}


 //--------------------------------------------------------
void CExtColorButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	Default();
	SetColors(m_ForeColorFocus, m_BackColorFocus);
}

// ---------------------------------------------------------------------------
void CExtColorButton::OnSetFocus(CWnd* pOldWnd)
{
	CButton::OnSetFocus(pOldWnd);
	SetColors(m_ForeColorFocus, m_BackColorFocus);
}


// ---------------------------------------------------------------------------
void CExtColorButton::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);
	SetColors(m_ForeColorNormal, m_BackColorNormal);
}

 //--------------------------------------------------------------------------
void CExtColorButton::SetForeColor(const COLORREF ForeColor)
{
	m_ForeColor = ForeColor;
}


 //--------------------------------------------------------------------------
void CExtColorButton::SetBackColor(const COLORREF BackColor)
{
	m_BackColor = BackColor;
	if (m_hBrush)
	{
		delete m_hBrush;
	}
	m_hBrush = new CBrush(m_BackColor);
}


 //--------------------------------------------------------------------------
void CExtColorButton::SetColors(const COLORREF BackColor, const COLORREF ForeColor)
{
	SetBackColor(BackColor);
	SetForeColor(ForeColor);
	Invalidate(FALSE);
}


 //--------------------------------------------------------------------------
void CExtColorButton::SetFocusColors(const COLORREF BackColor, const COLORREF ForeColor)
{
	m_BackColorFocus = BackColor;
	m_ForeColorFocus = ForeColor;
}


 //--------------------------------------------------------------------------
void CExtColorButton::SetNormalColors(const COLORREF BackColor, const COLORREF ForeColor)
{
	m_BackColorNormal = BackColor;
	m_ForeColorNormal = ForeColor;
}


 //--------------------------------------------------------------------------
HBRUSH CExtColorButton::CtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	pDC->SetTextColor(m_ForeColor);
	pDC->SetBkColor(m_BackColor);
    return(HBRUSH)(m_hBrush->m_hObject);
}
