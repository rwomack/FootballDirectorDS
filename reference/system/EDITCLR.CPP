#include "stdafx.h"
#include "editclr.h"

//////////////////
// You could make these user-customizable.
//
BEGIN_MESSAGE_MAP(CColorEdit, CEdit)
   ON_WM_CTLCOLOR_REFLECT()
   ON_WM_ERASEBKGND()
   ON_WM_CHAR()
   ON_WM_DESTROY()
   ON_WM_SETFOCUS()
   ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

/////////////////
// Handle WM_CTLCOLOR reflected from parent:
// If I have the focus, return the "highlight color."
// Must also set text and background colors, that's a Windows thing.
// 
HBRUSH CColorEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	if (brushstate == FALSE) {
		s_brush.CreateSolidBrush(BK_COLOR);
		brushstate = TRUE;
	}

	pDC->SetBkMode(TRANSPARENT);		// Set background mode to opaque

	pDC->SetTextColor(FG_COLOR);  // ditto
	pDC->SetBkColor(BK_COLOR);    // required for edit controls  

	return s_brush;               // return background brush
}

BOOL CColorEdit::OnEraseBkgnd(CDC* dc)
{
	CRect ClientRect;

	GetClientRect(&ClientRect);
	dc->ExtTextOut(0, 0, ETO_OPAQUE, ClientRect, NULL, 0, NULL);

	return TRUE;
}

void CColorEdit::OnDestroy()
{
	s_brush.DeleteObject();
	brushstate = FALSE;
}

void CColorEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CColorEdit::OnSetFocus(CWnd* pOldWnd)
{
	gotfocus = TRUE;

	::CreateCaret(m_hWnd,NULL,0,0);
	::ShowCaret(m_hWnd);

	CEdit::OnSetFocus(pOldWnd);
}

void CColorEdit::OnKillFocus(CWnd* pOldWnd)
{
	gotfocus = FALSE;
	
	::DestroyCaret();
	
	CEdit::OnKillFocus(pOldWnd);
}