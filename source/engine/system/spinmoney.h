// SpinEditBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpinEditBox window

 #ifndef __SPINMONEY_H__
 #define __SPINMONEY_H__


class CCountry;

class CSpinMoney : public CEdit
{
// Construction
public:
    CSpinMoney();

// Attributes
public:
	COLORREF		BK_COLOR;
	COLORREF		FG_COLOR;

	CBrush			s_brush;
	BOOL			brushstate;
	BOOL			gotfocus;

	HWND			m_hWndParent;
	UINT			boxID;

	UINT			MinValue;
	UINT			MaxValue;
    int				MaxIncPos;
    int				Top;
    int				Left;
    int				Height;

	UINT			Spin_ID;
    CString			str;

	CSpinButtonCtrl	SpinCtrl;
    //CClub*			ClubPtr;
    CCountry*		m_pCountry;

// Operations
public:

    void    SetIntAmount(int Amount);
	int		GetIntAmount(void);

	void    SetFloatAmount(float Amount);
	float	GetFloatAmount(void);
  float  GetAmount(void);
  void  SetAmount(float Amount);

    void    SetMinPos(int minval);
	void	SetMaxPos(int minval);

    void    SetBackColor(COLORREF backcolor);
	void	SetForeColor(COLORREF forecolor);
	void	SetEditFont(CFont* pFont);

    void    HideWindow(BOOL value);

    void    InitMoney(int x, int y, int h, HWND parent, UINT cID, UINT spinid, UINT engmin, UINT engmax, CCountry* CountPtr, COLORREF forecolor, COLORREF backcolor, CFont* pFont, CDC* pDC);

	int		GetWidth(void);

	// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSpinMoney)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CSpinMoney();

	// Generated message map functions
protected:
	float GetFloat();
	CBitmap*	CaretBitMap;

    //{{AFX_MSG(CSpinMoney)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnPaste(WPARAM w,LPARAM l);

	DECLARE_MESSAGE_MAP()
};
#endif
