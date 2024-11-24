#if !defined(AFX_SCREEN44_H__90F6B960_A867_45CB_AF03_5FF9A1BCABB2__INCLUDED_)
#define AFX_SCREEN44_H__90F6B960_A867_45CB_AF03_5FF9A1BCABB2__INCLUDED_


/*

	Screen44.h : implementation file

	Incoming Loan Offer for a player from a club

	(c) Rw Software 2003 - 2008

*/



/////////////////////////////////////////////////////////////////////////////
// CScreen44 dialog

class CScreen44Dlg : public CBitmapDialog
{
// Construction
public:
	CScreen44Dlg(CClubTransferOffer* _BidDetails, CWnd* pParent = null);   // standard constructor
	~CScreen44Dlg();

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,
	};

// Implementation
protected:
	CClubTransferOffer*		m_pBidDetails;
	CMessageDisplay			m_messagedisplayResponse;
	CPlayerNameTextRegion	m_PlayerName;
	CClubNameTextRegion		m_ClubName;
	bool					m_NegotiationsEnded;
	CLoanPeriodSelector		m_LoanPeriodSelector;

	void					OnEndNegotiations(CString strMessage);
	virtual BOOL			OnInitDialog();
	virtual void			OnAccept();
	void					OnCounter();
	void					OnOK();
	void					OnReject();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);
	void	 				OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SCREEN44_H__90F6B960_A867_45CB_AF03_5FF9A1BCABB2__INCLUDED_)
