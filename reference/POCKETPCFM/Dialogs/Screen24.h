#if !defined(AFX_Screen24_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen24_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen24.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen24Dlg dialog


class CScreen24Dlg : public CBitmapDialog
{
// Construction
public:
    CScreen24Dlg(CWnd* pParent = null);   // standard constructor

// Dialog Data
    enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,

		BUTTON1W = 68,
		BUTTON1H = 24,

		LOANY = 100
	};

	enum eDIALOG24
	{
		eOFFER,
	};

// Overrides
	int						DoModal(CPlayer* pPlayer);

protected:
	CPlayer*				m_pCurrentPlayer;
	CPlayerNameTextRegion	m_PlayerName;
	CClubNameTextRegion		m_ClubName;
	CTextRegion				m_LoansLeftStatic;
	CLoanPeriodSelector		m_LoanPeriodSelector;
	
	BOOL					OnInitDialog();
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPaint();
	void					OnMakeOffer();
	void					OnSize(UINT nType, int cx, int cy);

    DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_Screen24_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
