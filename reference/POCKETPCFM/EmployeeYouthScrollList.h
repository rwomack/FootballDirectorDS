#if !defined(AFX_EMPLOYEEYOUTHSCROLLLIST_H__7B9E99E5_DB4B_437A_96AB_24AC6C5AE25F__INCLUDED_)
#define AFX_EMPLOYEEYOUTHSCROLLLIST_H__7B9E99E5_DB4B_437A_96AB_24AC6C5AE25F__INCLUDED_

// EmployeeYouthScrollList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEmployeeYouthScrollList

class CEmployeeYouthScrollList
{
public:
	CEmployeeYouthScrollList()
	{
		m_iVerticalScroll = 0;
		m_iMaxScroll = 0;
	}

	CScrollBar*				pScrollBar()
	{
		return (CScrollBar*)m_pParentWnd->GetDlgItem(m_nScrollbarID);
	}
	void					DoInitialise(const int nScrollbarID, CRect rectArea, int nNumRowsOnScreen, CWnd* pParentWnd)
	{
		m_nScrollbarID = nScrollbarID;
		m_Rect = rectArea;
		m_iNumRowsOnScreen = nNumRowsOnScreen;
		m_pParentWnd = pParentWnd;
		m_Rect.left = 240 - 16;
		pScrollBar()->MoveWindow(m_Rect);
		m_Rect.left = 0;
	}
	void					OnVScroll(uint nSBCode, UINT nPos)
	{
		int nSaveVerticalScroll = m_iVerticalScroll;
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
				m_iVerticalScroll = m_iMaxScroll;
				break;
			case SB_ENDSCROLL:
				break;
			case SB_PAGEDOWN:
				m_iVerticalScroll = min(m_iVerticalScroll + m_iNumRowsOnScreen, m_iMaxScroll);
				break;
			case SB_PAGEUP:
				m_iVerticalScroll = max(m_iVerticalScroll - m_iNumRowsOnScreen, 0);
				break;
			case SB_THUMBPOSITION:
				m_iVerticalScroll = nPos;
				break;
			case SB_THUMBTRACK:
				m_iVerticalScroll = nPos;
				break;
		}
		if (nSaveVerticalScroll != m_iVerticalScroll)
		{
			m_pParentWnd->InvalidateRect(m_Rect);
		}
		pScrollBar()->SetScrollPos(m_iVerticalScroll);
	}
	void					Redraw(void)
	{
	}
	CWnd*					GetParentWnd(void)
	{
		return m_pParentWnd;
	}
	int						GetVerticalScroll()
	{
		return m_iVerticalScroll;
	}
	bool					IsRowVisible(int nRow)
	{
		if (nRow >= GetVerticalScroll() && nRow < GetVerticalScroll() + m_iNumRowsOnScreen)
		{
			return true;
		}
		return false;
	}
	int						GetTop(void)
	{
		return m_Rect.top;
	}
	int						GetBottom(void)
	{
		return m_Rect.bottom;
	}
	void					DoSetListSize(int nNumEntries)
	{
		m_iVerticalScroll = 0;
		if (nNumEntries > m_iNumRowsOnScreen)
		{
			m_iMaxScroll = nNumEntries - m_iNumRowsOnScreen;
			pScrollBar()->ShowWindow(SW_SHOW);
			pScrollBar()->SetScrollRange(0, m_iMaxScroll, FALSE);
			pScrollBar()->SetScrollPos(m_iVerticalScroll);
		}
		else
		{
			pScrollBar()->ShowWindow(SW_HIDE);
		}
	}
private:
	int						m_nScrollbarID;
	CWnd*					m_pParentWnd;
	int						m_iVerticalScroll;
	int						m_iMaxScroll;
	int						m_iNumRowsOnScreen;
	CRect					m_Rect;
};
#endif