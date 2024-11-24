#ifndef __FLOATEDIT_H__
#define __FLOATEDIT_H__


class CFloatEdit : public CEdit 
{
public:

	// Message handler for WM_CHAR message.
	afx_msg void			OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	// Parses the window text into a floating point value.
    float					GetValue(bool b2dp = false);
	void					GetszValue(char* szDest, int iDestBufSize);

    // User defined conversion of a floatEdit to an float.
    operator float();

protected:
	char*					NormalizeAmount(char *szAmt, char *szNormalizedAmt, int iDestBufSize, bool bBlankZero = false, bool bAllowNegative = false);

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

class CFloatSIPEdit : public CFloatEdit
{
public:
	void					OnSetFocus(CWnd* pOldWnd);	
	
protected:
	
    DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

class CFloatSIPUnderLinedEdit : public CFloatSIPEdit
{
public:
	CFloatSIPUnderLinedEdit()
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

#endif