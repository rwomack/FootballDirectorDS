#if !defined(AFX_SCREEN42_H__6BB48EC6_A060_46C3_8A9B_71E2C354AA27__INCLUDED_)
#define AFX_SCREEN42_H__6BB48EC6_A060_46C3_8A9B_71E2C354AA27__INCLUDED_

// Screen42.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen42 dialog


class CScreen42 : public CBitmapDialog
{
// Construction
public:
	CScreen42(CWnd* pParent = null);   // standard constructor
	~CScreen42();

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,
	};

// Implementation
protected:
    CMessageDisplay			m_MessageDisplay;

	void					DoClearRegistry();
	virtual BOOL			OnInitDialog();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPaint(); 
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN42_H__6BB48EC6_A060_46C3_8A9B_71E2C354AA27__INCLUDED_)
