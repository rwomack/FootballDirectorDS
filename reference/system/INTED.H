
/*

    Edit Box Classes -

	(c) RW Software 1994 - 2005

    ---------------------------------------------------------------
 */


#ifndef __INTEDIT_H__
#define __INTEDIT_H__



class CIntEdit : public CEdit 
{
public:

	// Message handler for WM_CHAR message.
	void					OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	// Parses the window text into an integer value.
    int						GetValue();
	void					SetValue(const int _Value);
	void					SetValue(const uint x);
	void					DoIncrementMoneyStep();
	void					DoDecrementMoneyStep();
    
	// User defined conversion of an IntEdit to an int.
    operator int();

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

class CIntSIPEdit : public CIntEdit
{
public:
	void					OnSetFocus(CWnd* pOldWnd);	
	
protected:
	
    DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

class CIntSIPUnderLinedEdit : public CIntSIPEdit
{
public:
	CIntSIPUnderLinedEdit()
	{
		m_colorUnderLine = RGB(192, 192, 192);
		m_LineY = 15;
	}
		
protected:
	COLORREF				m_colorUnderLine;
	ushort					m_LineY;
	
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

class CYearSIPEdit : public CIntSIPEdit 
{
public:
	void			DoInitialise(int nID, CWnd* pParent)
	{
		SubclassDlgItem(nID, pParent);
		LimitText(4);
	}

protected:
	
};
/////////////////////////////////////////////////////////////////////////////


#endif