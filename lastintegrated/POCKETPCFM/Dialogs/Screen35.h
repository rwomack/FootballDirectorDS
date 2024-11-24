#if !defined(AFX_SCREEN35_H__3F595807_7948_4B40_B0A6_942D040C7E28__INCLUDED_)
#define AFX_SCREEN35_H__3F595807_7948_4B40_B0A6_942D040C7E28__INCLUDED_

// Screen35.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen35 dialog

class CScreen35Dlg : public CBitmapDialog
{
// Construction
public:
	CScreen35Dlg(CPlayer* pPlayer, CWnd* pParent = null);   // standard constructor

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,
	};

// Implementation
protected:
	CPlayer*				m_pCurrentPlayer;
	CPlayerNameTextRegion	m_PlayerName;
	CMessageDisplay			m_MessageDisplay;

	void 					OnOK(); 
	void 					OnCancel(); 
	void 					OnConfirm();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL					OnInitDialog();
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN35_H__3F595807_7948_4B40_B0A6_942D040C7E28__INCLUDED_)
