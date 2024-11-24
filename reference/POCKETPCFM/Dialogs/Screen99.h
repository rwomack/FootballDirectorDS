#if !defined(AFX_SCREEN99_H__6BB48EC6_A060_46C3_8A9B_71E2C354AA27__INCLUDED_)
#define AFX_SCREEN99_H__6BB48EC6_A060_46C3_8A9B_71E2C354AA27__INCLUDED_

// Screen99.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen99 dialog


class CScreen99Dlg : public CBitmapDialog
{
// Construction
public:
	CScreen99Dlg(CWnd* pParent = null);   // standard constructor
	~CScreen99Dlg();

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,

		BUTTONW = 170,
		BUTTONH = 27,
	};

	enum eDIALOG
	{
		eCONTINUE,
		eNEWGAME,
		eREGISTRATION,
		eLOADGAME,
		eOPTIONS,
		eCREDITS,
		eEXIT,

		eBUTTONS
	};

// Implementation
protected:
	CVOImage				m_Title;
	CPNGButton				m_Buttons[eBUTTONS];
	eDIALOG					m_CurrentButton;

	int						DoDialog(const eDIALOG eDlg);
	void					OnButtonPressed(eDIALOG _Button);
	void					OnPaint(); 
	virtual BOOL			OnInitDialog();
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKillFocus(CWnd* _pNewWnd);
	void					OnSetFocus(CWnd* _pOldWnd);
	void					OnSize(UINT nType, int cx, int cy);
	void					SetCurrentButton(eDIALOG _Button);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN99_H__6BB48EC6_A060_46C3_8A9B_71E2C354AA27__INCLUDED_)
