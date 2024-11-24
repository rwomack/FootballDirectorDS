#include "stdafx.h"

#include "ToggleButton.h"
#include "ExtPaintDC.h"


 // ---------------------------------------------------------------------------
void CToggleButton::DoInitialise(CRect rectArea, int nFirstResourceId, int nFirstStrId, int nSecondStrId, CWnd* wndParent)
{
	m_rectRectangle = rectArea;
	int pointCentreX = ((m_rectRectangle.right - m_rectRectangle.left) / 2) + m_rectRectangle.left;
	m_static1.OnCreate(14, BLACK, WHITE, nFirstStrId, 
					   CRect(m_rectRectangle.left + 1,
							 m_rectRectangle.top + 1, 
							 pointCentreX,
							 m_rectRectangle.bottom - 1),
					   wndParent, nFirstResourceId, SS_NOTIFY | SS_CENTER);
	m_static2.OnCreate(14, BLACK, WHITE, nSecondStrId, 
					   CRect(pointCentreX + 1, 
							 m_rectRectangle.top + 1, 
							 m_rectRectangle.right - 1, 
							 m_rectRectangle.bottom - 1), 
					   wndParent, nFirstResourceId + 1, SS_NOTIFY | SS_CENTER);
	SetCurSel(0);
}


 // ---------------------------------------------------------------------------
void CToggleButton::OnDraw(CExtPaintDC* pDC)
{
	CPen borderpen(PS_SOLID, 1, BLACK);
	CPen* old = pDC->SelectObject(&borderpen);
	HBRUSH hOldBrush = (HBRUSH)pDC->SelectObject(::GetStockObject(NULL_BRUSH));
	pDC->Rectangle(&m_rectRectangle);

	int pointCentreX = ((m_rectRectangle.right - m_rectRectangle.left) / 2) + m_rectRectangle.left;
	pDC->MoveTo(CPoint(pointCentreX, m_rectRectangle.top));
	pDC->LineTo(CPoint(pointCentreX, m_rectRectangle.bottom));
	
	pDC->SelectObject(hOldBrush);
	pDC->SelectObject(old);
}


 // ---------------------------------------------------------------------------
void CToggleButton::SetCurSel(const UInt16 nCurSel)
{
	m_nCurSel = nCurSel;
	if (m_nCurSel == 0)
	{
		m_static1.SetForeColor(WHITE);
		m_static2.SetForeColor(BLACK);
		m_static1.SetBackColor(RGB(0, 48, 152));
		m_static2.SetBackColor(WHITE);
	}
	else
	{
		m_static2.SetForeColor(WHITE);
		m_static1.SetForeColor(BLACK);
		m_static2.SetBackColor(RGB(0, 48, 152));
		m_static1.SetBackColor(WHITE);
	}
	m_static1.RedrawWindow();
	m_static2.RedrawWindow();
}
