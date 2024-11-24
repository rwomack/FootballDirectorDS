// ScrollPropertyPage.cpp : implementation file
//
#include "stdafx.h"

IMPLEMENT_DYNCREATE(CScrollPropertyPage, CPropertyPage)

BEGIN_MESSAGE_MAP(CScrollPropertyPage, CPropertyPage)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// ---------------------------------------------------------------------------
void CScrollPropertyPage::DoResetVariables()
{
	m_nVertInc = 0;
	m_nVscrollMax = 0;
	m_nVscrollPos = 0;
	m_nSavedVscrollMax = 0;
	m_nSavedVscrollPos = 0;
}


// ---------------------------------------------------------------------------
BOOL CScrollPropertyPage::OnSetActive()
{
	theApp.GetsipState()->DoRestore(this);
	m_nVscrollMax = m_nSavedVscrollMax;
	m_nVscrollPos = m_nSavedVscrollPos;
	CWnd::SetScrollRange(SB_VERT, 0, m_nVscrollMax, FALSE);
	CWnd::SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
	return TRUE;
}


// ---------------------------------------------------------------------------
BOOL CScrollPropertyPage::OnKillActive()
{
	m_nSavedVscrollMax = m_nVscrollMax;
	return TRUE;
}


 // ---------------------------------------------------------------------------
void CScrollPropertyPage::OnSize(UINT nType, int cx, int cy)
{
	if (cy > 200)
	{
		if (m_nVscrollPos > 0)
		{
			wce_ScrollChildren(GetSafeHwnd(), 0, -m_nVscrollPos);
		}
		m_nVertInc = 0;
		m_nVscrollMax = 0;
	}
	else
	{
		m_nVertInc = 80;
		m_nVscrollMax = 80;
	}
	m_nSavedVscrollPos = 0;
	m_nVscrollPos = 0;
	CWnd::SetScrollRange(SB_VERT, 0, m_nVscrollMax, FALSE);
	CWnd::SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
	CPropertyPage::OnSize(nType, cx, cy);
}


// ---------------------------------------------------------------------------
void CScrollPropertyPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    int nInc;
    switch(nSBCode)
    {
		case SB_TOP:
			nInc = -m_nVscrollPos;
			break;
		case SB_BOTTOM:
			nInc = m_nVscrollMax - m_nVscrollPos;
			break;
		case SB_LINEUP:
			nInc = -(m_nVscrollMax / 2);
			break;
		case SB_LINEDOWN:
			nInc = m_nVscrollMax / 2;
			break;
		case SB_PAGEUP:
			nInc = min(-1, -m_nVertInc);
			break;
		case SB_PAGEDOWN:
			nInc = max(1, m_nVertInc);
			break;
		case SB_THUMBTRACK:
			nInc = nPos - m_nVscrollPos;
			break;
		default:
			nInc = 0;
    }

    nInc = max(-m_nVscrollPos, min(nInc, m_nVscrollMax - m_nVscrollPos));
    if (nInc)
    {
        m_nVscrollPos += nInc;
		CWnd::ScrollWindow(0, nInc, NULL, NULL);
		CWnd::SetScrollPos(SB_VERT, m_nVscrollPos, TRUE);
		wce_ScrollChildren(GetSafeHwnd(), 0, nInc);
		m_nSavedVscrollPos = m_nVscrollPos;
    }
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}
