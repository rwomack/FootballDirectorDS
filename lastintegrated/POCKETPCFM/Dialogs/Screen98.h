#if !defined(AFX_SCREEN98_H__A96F200A_EDB9_4DAD_B4CC_61547C6A551F__INCLUDED_)
#define AFX_SCREEN98_H__A96F200A_EDB9_4DAD_B4CC_61547C6A551F__INCLUDED_

// Screen98.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen98 dialog

class CScreen98 : public CBitmapDialog
{
// Construction
public:
	CScreen98(CWnd* pParent = null);   // standard constructor
	virtual ~CScreen98();

// Dialog Data
	enum 
	{ 
		BUTTON1W = 101,
		BUTTON1H = 20,

		IDD = IDD_SCREEN98 
	};

// Implementation
protected:
	CSIPEdit				m_editName;
	CPNGButton				m_buttonContinue;
	int						m_EditTopY;

	void					OnOK();
	BOOL					OnInitDialog();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT _nFlags, CPoint _Point); 
	void					OnLButtonUp(UINT _nFlags, CPoint _Point); 
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN98_H__A96F200A_EDB9_4DAD_B4CC_61547C6A551F__INCLUDED_)
