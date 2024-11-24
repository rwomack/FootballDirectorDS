#if !defined(AFX_SCREEN21_H__2F5EEFE3_BCFA_465C_B35E_B95654A989DF__INCLUDED_)
#define AFX_SCREEN21_H__2F5EEFE3_BCFA_465C_B35E_B95654A989DF__INCLUDED_

// Screen21.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen21 dialog

class CScreen21Dlg : public CBitmapDialog
{
// Construction
public:
	CScreen21Dlg(CPlayer* pPlayer, CWnd* pParent = null);   // standard constructor
	~CScreen21Dlg();

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,
	};

protected:
    CMessageDisplay			m_MessageDisplay;
	CPlayer*				m_pCurrentPlayer;	
	CPlayerNameTextRegion	m_PlayerName;

	void					OnEndNegotiations(CString strMessage);
	void OnRequestGranted();
	void OnRequestDenied();
	BOOL					OnInitDialog();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN21_H__2F5EEFE3_BCFA_465C_B35E_B95654A989DF__INCLUDED_)
