
/*

    Edit Box Classes -

	(c) RW Software 1994 - 2005

    ---------------------------------------------------------------
 */


#ifndef __INTEDIT_H__
#define __INTEDIT_H__


class C1904YearSIPEdit : public CYearSIPEdit
{
public:
    int						GetValue(void);
	void					SetValue(const int x);

};
/////////////////////////////////////////////////////////////////////////////

class C1904YearSIPUnderLinedEdit : public C1904YearSIPEdit
{
public:
	C1904YearSIPUnderLinedEdit()
	{
		m_colorUnderLine = RGB(192, 192, 192);
		m_LineY = 15;
	}
		
protected:
	COLORREF				m_colorUnderLine;
	UInt16					m_LineY;
	
	void					OnPaint();
	void					DrawUnderLine(CDC* pDC);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnRecognizeGesture(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL					OnEraseBkgnd(CDC* pDC);
	void					OnSetFocus(CWnd* pOldWnd);
	void					OnKillFocus(CWnd* pNewWnd);

    DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
#endif