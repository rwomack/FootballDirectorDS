#if !defined(AFX_Screen30_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen30_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen30.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen30 dialog


class CScreen30Dlg : public CBitmapDialog
{
// Construction
public:
    CScreen30Dlg(CPlayer* _Player, CWnd* pParent = null);   // standard constructor

// Dialog Data
    enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS
	};

	enum eDIALOG12
	{
		eFEEDOWN,
		eFEEUP,
		eSELLONDOWN,
		eSELLONUP,
	};

	enum
	{
		eFEE,
		eSELLON,

		eMAXSELECTOR
	};
protected:
	CPlayer*				m_pCurrentPlayer;
	CPlayerNameTextRegion	m_PlayerName;
	CClubNameTextRegion		m_ClubName;
	CTitledTextSelector		m_FeeInput;
    CTitledTextSelector		m_SellonInput;
	CClubTransferOffer		m_BidDetails;
	CClubBalanceTextRegion	m_ClubBalanceText;
	CCurrentTitledTextSelector	m_CurrentTextSelector;
	CTitledTextSelector		m_TextSelector[eMAXSELECTOR];
	bool					m_NegotiationsEnded;

	BOOL					OnInitDialog();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnMakeOffer();
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_Screen30_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
