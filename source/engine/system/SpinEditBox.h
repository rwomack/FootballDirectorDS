// SpinEditBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpinEditBox window

 #ifndef __SPINEDIT_H__
 #define __SPINEDIT_H__

class CSpinEditBox : public CEdit
{
// Construction
public:
    CSpinEditBox();

// Attributes
public:
	COLORREF BK_COLOR;
	COLORREF FG_COLOR;

	CBrush		s_brush;
	BOOL		brushstate;
	BOOL		gotfocus;

	CWnd*		m_cWndParent;
	HWND		m_hWndParent;
	UINT		boxID;

	int			m_iMinValue;
	int			m_iMaxValue;
	int			m_iLimitNum;

	UINT		Spin_ID;

	CSpinButtonCtrl		m_SpinCtrl;

// Operations
public:

	void	SetAmount(int x);
	int		GetAmount(void);
	void	SetMinPos(int minval);
	void	SetMaxPos(int minval);
	void	SetBackColor(COLORREF backcolor);
	void	SetForeColor(COLORREF forecolor);
	void	SetEditFont(CFont* pFont);
	void	HideWindow(BOOL value);

	void	InitEditBox(const RECT& rect, HWND parent, UINT cID, UINT spinid, int minval, int maxval, int limnum, COLORREF forecolor, COLORREF backcolor, CFont* pFont);

public:
    virtual ~CSpinEditBox();

protected:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChange();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};
#endif
