#if !defined(AFX_SCROLLLIST_H__888D8E26_11C8_4A03_B5E3_D26BE9681A50__INCLUDED_)
#define AFX_SCROLLLIST_H__888D8E26_11C8_4A03_B5E3_D26BE9681A50__INCLUDED_

// ScrollList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScrollList

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

	const ushort		NOROWSELECTED = 0xffff;

class CScrollList : public CScrollBar
{
public:
	CScrollList();

	void					SetSurrenderAllKeys(const bool bSurrenderAllKeys)
	{
		m_bSurrenderAllKeys = bSurrenderAllKeys;
	}
	void					OnSize(CRect theRect, const ushort nGapY);
	void					SetScrollArea(CRect theRect, const ushort wRowHeight, const byte nScrollBarWidth, const bool bMoveWindow = false);
	CRect*					GetScrollArea()
	{
		return &m_rectScrollArea;
	}
	void					DoInitialise(const ushort iNumRowsOnScreen, const ushort nCount = 0);
	inline ushort			GetMaxScroll()
	{
		return m_iMaxScroll;
	}
	inline ushort			GetVerticalScroll() const
	{
		return m_iVerticalScroll;
	}
	inline ushort			GetNumRowsOnScreen() const
	{
		return m_iNumRowsOnScreen;
	}
	inline const ushort		GetCurSel() const
	{
		return m_nCurSel;
	}
	inline const ushort		GetCurScreenSel() const
	{
		return m_nCurSel - GetVerticalScroll();
	}
	const ushort			GetSize() const
	{
		return m_nListSize;
	}
	const uint			GetAt(int nIndex) const
	{
		return m_nListIds.GetAt(nIndex);
	}
	void					SetListSize(const ushort nCount);
	inline void				SetNumRowsOnScreen(const ushort nRows)
	{
		m_iNumRowsOnScreen = nRows;
	}
	void					SetCurSel(const ushort CurSel);
	void					SetVerticalScroll(const ushort CurSel)
	{
		m_iVerticalScroll = min(CurSel, m_iMaxScroll);
	}

	CDWordArray&			GetList()
	{
		return m_nListIds;
	}
	const uint			GetAtCurSel()const
	{
		return GetAt(GetCurSel());
	}
	void					DoScrollToCurSel()
	{
		DoScrollTo(m_nCurSel);
	}
	void					DoScrollTo(const ushort CurSel);
	bool					OnVScroll(UINT nSBCode, UINT nPos, const bool bRedraw = true);
	void					OnReDraw();
	void					DoResetScrollBar(ushort iNumItems);
	bool					DoDecCurSel();
	bool					DoIncCurSel();
	void					DoInitialise();
	bool					IsScrollBarVisible()
	{
		return m_ScrollBarIsVisible;
	}
	bool					IsCurSelVisible()
	{
		return IsRowVisible(m_nCurSel);
	}
	bool					IsRowVisible(const ushort nCurrentRow);
	void					RemoveAll();
	afx_msg void			OnLButtonDown(UINT nFlags, CPoint point);
	uint					OnGetDlgCode(); //DSR
	virtual BOOL			PreTranslateMessage(MSG* pMsg);

protected:
	int						m_iScrollBarId;
	byte					m_nScrollBarWidth;
	ushort					m_iVerticalScroll;
	ushort					m_iMaxScroll;
	ushort					m_iNumRowsOnScreen;
	bool					m_ScrollBarIsVisible;
	ushort					m_nCurSel;
	ushort					m_nListSize;
	CRect					m_rectScrollArea;
	CDWordArray				m_nListIds;
	bool					m_bSurrenderAllKeys;		//DSR - set to true if this control should toss keys back at parent

	DECLARE_MESSAGE_MAP()
};
#endif
