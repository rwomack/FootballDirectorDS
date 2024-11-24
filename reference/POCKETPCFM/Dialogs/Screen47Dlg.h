#if !defined(AFX_SCREEN47DLG_H__B5A919DD_1B36_4218_B525_9440381EFDF8__INCLUDED_)
#define AFX_SCREEN47DLG_H__B5A919DD_1B36_4218_B525_9440381EFDF8__INCLUDED_

// Screen47Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen47Dlg dialog


class CScreen47Dlg : public CBitmapDialog
{
// Construction
public:
	CScreen47Dlg(CWnd* pParent = null);   // standard constructor

	// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,
	};

	enum eDIALOG47
	{
		eREFUSE,
		eACCEPT,
		eCONTINUE,
	};

// Implementation
protected:
	CCurrentButtonSelector	m_ButtonSelector;
    CMessageDisplay			m_MessageDisplay;

	void					DoShowContinueButton();
	void					OnEndNegotiations(CString strMessage);
	virtual BOOL			OnInitDialog();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

class CScreen47RenewDlg : public CScreen47Dlg
{
public:
	CScreen47RenewDlg(CClub* _Club);

// Implementation
protected:
	CClub*					m_pClub;

	void					OnAccept();
	void					OnEndNegotiations(CString strMessage);
	BOOL					OnInitDialog();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnRefuse();

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

class CScreen47SackedDlg : public CScreen47Dlg
{
public:
	CScreen47SackedDlg(CString strReason);
	
	// Implementation
protected:
	CString					m_StrMessage;
	
	void					OnLButtonUp(UINT nFlags, CPoint point);
	BOOL					OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

class CScreen47WelcomeDlg : public CScreen47SackedDlg
{
public:
	CScreen47WelcomeDlg(CString strReason);
	
	// Implementation
protected:
	
	BOOL					OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN47DLG_H__B5A919DD_1B36_4218_B525_9440381EFDF8__INCLUDED_)
