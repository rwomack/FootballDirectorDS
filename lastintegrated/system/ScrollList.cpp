
 /*

	File:- ScrollList.cpp


	(c) RW Software 2002-2003

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"
#include "ScrollList.h"


BEGIN_MESSAGE_MAP(CScrollList, CScrollBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()


//---------------------------------------------------------------------
CScrollList::CScrollList()
: m_iScrollBarId(0)
, m_iVerticalScroll(0)
, m_iMaxScroll(0)
, m_iNumRowsOnScreen(0)
, m_ScrollBarIsVisible(false)
, m_nCurSel(0)
, m_nListSize(0)
, m_rectScrollArea(0, 0, 0, 0)
, m_bSurrenderAllKeys(false)
, m_nScrollBarWidth(14)
{
}


//---------------------------------------------------------------------
BOOL CScrollList::PreTranslateMessage(MSG* pMsg)
{
	if(m_bSurrenderAllKeys && pMsg->message>=WM_KEYFIRST && pMsg->message<=WM_KEYLAST)
		return GetParent()->SendMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
	else
		return CScrollBar::PreTranslateMessage(pMsg);
}

//---------------------------------------------------------------------------------------
uint CScrollList::OnGetDlgCode()
{
	DWORD dwVal=CScrollBar::OnGetDlgCode();

	if(m_bSurrenderAllKeys)
		dwVal|=DLGC_WANTALLKEYS|DLGC_WANTARROWS;  //grab the keys so we can toss it back at our parent

	return dwVal;
}


//---------------------------------------------------------------------
void CScrollList::OnLButtonDown(UINT nFlags, CPoint point)
{
	Default();
}


//---------------------------------------------------------------------
void CScrollList::SetListSize(const ushort nCount)
{
	m_nListSize = nCount;
 	DoResetScrollBar(m_nListSize);
	OnReDraw();
}


//---------------------------------------------------------------------
void CScrollList::DoInitialise()
{
	SetCurSel(0);
	SetListSize(m_nListIds.GetSize());
	InvalidateRect(GetScrollArea(), TRUE);
}


//---------------------------------------------------------------------
bool CScrollList::OnVScroll(UINT nSBCode, UINT nPos, const bool bRedraw)
{
	ushort iOldScrollPos = m_iVerticalScroll;
	bool bNeedRedraw = false;
	switch(nSBCode)
	{
		case SB_LINEDOWN:
			if (m_iVerticalScroll < m_iMaxScroll)
			{
				m_iVerticalScroll++;
			}
			break;
		case SB_LINEUP:
			if (m_iVerticalScroll > 0)
			{
				m_iVerticalScroll--;
			}
			break;
		case SB_BOTTOM:
			// Scroll to bottom.
			m_iVerticalScroll = m_iMaxScroll;
			break;
		case SB_ENDSCROLL:
			//End scroll.
			break;
		case SB_PAGEDOWN:
			// Scroll one page down.
			SetVerticalScroll(m_iVerticalScroll + (m_iNumRowsOnScreen - 1));
			break;
		case SB_PAGEUP:
			// Scroll one page up.
			m_iVerticalScroll = max(m_iVerticalScroll - (m_iNumRowsOnScreen - 1), 0);
			break;
		case SB_THUMBPOSITION:
			// Scroll to the absolute position. The current position is provided in nPos.
			SetVerticalScroll(nPos);
			break;
		case SB_THUMBTRACK:
			// Drag scroll box to specified position. The current position is provided in nPos.
			SetVerticalScroll(nPos);
			break;
		default:
			ASSERT(false);
	}
	if (nSBCode != SB_THUMBTRACK)
	{
		OnReDraw();
	}
	if ((iOldScrollPos != m_iVerticalScroll || nSBCode == SB_THUMBPOSITION) && bRedraw == true)
	{
		bNeedRedraw = true;
	}
	return bNeedRedraw;
}


//---------------------------------------------------------------------
void CScrollList::OnReDraw()
{
	if (m_ScrollBarIsVisible == true)
	{
		CScrollBar::SetScrollPos(m_iVerticalScroll);
	}
}


//---------------------------------------------------------------------
void CScrollList::RemoveAll()
{
	m_nListSize = 0;
	m_nListIds.RemoveAll();
	DoResetScrollBar(0);
	OnReDraw();
}


//---------------------------------------------------------------------
void CScrollList::DoResetScrollBar(ushort iNumItems)
{
	if (::IsWindow(m_hWnd))
	{
		ASSERT(m_rectScrollArea.right > 0);
		m_iVerticalScroll = 0;
		m_iMaxScroll = max(0, iNumItems - m_iNumRowsOnScreen);
		if (m_iMaxScroll > 0)
		{
			CScrollBar::SetScrollRange(0, m_iMaxScroll);

			int nSavedLeft = m_rectScrollArea.left;
			m_rectScrollArea.left = m_rectScrollArea.right - m_nScrollBarWidth;
			CScrollBar::MoveWindow(m_rectScrollArea);
			m_rectScrollArea.left = nSavedLeft;

			CScrollBar::ShowWindow(SW_SHOW);
			m_ScrollBarIsVisible = true;
		}
		else
		{
			CScrollBar::ShowWindow(SW_HIDE);
			m_ScrollBarIsVisible = false;
		}
	}
}


//---------------------------------------------------------------------
bool CScrollList::DoDecCurSel()
{
	bool bRet = false;
	if (m_nCurSel > 0)
	{
		m_nCurSel--;
		bRet = true;
	}
	return bRet;
}


//---------------------------------------------------------------------
bool CScrollList::DoIncCurSel()
{
	bool bRet = false;
	if (m_nCurSel < m_nListSize - 1)
	{
		m_nCurSel++;
		bRet = true;
	}
	return bRet;
}


//---------------------------------------------------------------------
bool CScrollList::IsRowVisible(const ushort nCurrentRow)
{
	if (nCurrentRow >= GetVerticalScroll() && nCurrentRow < GetVerticalScroll() + GetNumRowsOnScreen())
	{
		return true;
	}
	return false;
}


//---------------------------------------------------------------------
void CScrollList::SetCurSel(const ushort CurSel)
{
	m_nCurSel = CurSel;
}


// Sets current selection and ensures is visible, at the top of the screen
// list if possible
//---------------------------------------------------------------------
void CScrollList::DoScrollTo(const ushort CurSel)
{
	SetCurSel(CurSel);
	if ((m_nCurSel >= m_iVerticalScroll + m_iNumRowsOnScreen) || (m_nCurSel < m_iVerticalScroll) )
	{
		SetVerticalScroll(CurSel);
		OnReDraw();
	}
}


//---------------------------------------------------------------------
void CScrollList::DoInitialise(const ushort iNumRowsOnScreen, const ushort nCount)
{
	m_iVerticalScroll = 0;
	m_nCurSel = 0;
	m_iNumRowsOnScreen = iNumRowsOnScreen;
	SetListSize(nCount);
}


//---------------------------------------------------------------------
void CScrollList::SetScrollArea(CRect theRect, const ushort wRowHeight, const byte nScrollBarWidth, const bool bMoveWindow)
{
	m_nScrollBarWidth = nScrollBarWidth;
	m_rectScrollArea = theRect;
	m_iNumRowsOnScreen = (GetScrollArea()->bottom - GetScrollArea()->top) / wRowHeight;
	DoResetScrollBar(m_nListSize);
	if (bMoveWindow == true)
	{
		MoveWindow(GetScrollArea());
	}
	GetScrollArea()->left = 0;
}


//---------------------------------------------------------------------
void CScrollList::OnSize(CRect theRect, const ushort nGapY)
{
	ushort nCurSel = GetVerticalScroll();		// Will be reset to zero
	SetScrollArea(theRect, nGapY, m_nScrollBarWidth);
	SetVerticalScroll(nCurSel);					// Set to original position
	OnReDraw();
}
