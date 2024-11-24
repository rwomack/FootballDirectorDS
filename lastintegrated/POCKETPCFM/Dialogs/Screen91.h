#if !defined(AFX_SCREEN91_H__A96F200A_EDB9_4DAD_B4CC_61547C6A551F__INCLUDED_)
#define AFX_SCREEN91_H__A96F200A_EDB9_4DAD_B4CC_61547C6A551F__INCLUDED_

// Screen91.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen91 dialog

class CScreen91Dlg : public CBitmapDialog
{
// Construction
public:
	CScreen91Dlg(CWnd* pParent = null);   // standard constructor
	~CScreen91Dlg();

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS, 
	};

	enum eDIALOG91
	{
		eSOUNDFXDOWN,
		eSOUNDFXUP,
		eMENUMUSICDOWN,
		eMENUMUSICUP,
		eINGAMEMUSICDOWN,
		eINGAMEMUSICUP,
		
		eBUTTONS,

		EFFECTS = 0,
		MENU,
		INGAME
	};	

// Implementation
protected:
	CCurrentTitledTextSelector	m_CurrentTextSelector;
	CTitledTextSelector		m_TextSelector[3];

	BOOL					OnInitDialog();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnOK();
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN91_H__A96F200A_EDB9_4DAD_B4CC_61547C6A551F__INCLUDED_)
