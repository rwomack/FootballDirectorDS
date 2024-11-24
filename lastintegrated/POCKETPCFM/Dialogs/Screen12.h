#if !defined(AFX_SCREEN12_H__90F6B960_A867_45CB_AF03_5FF9A1BCABB2__INCLUDED_)
#define AFX_SCREEN12_H__90F6B960_A867_45CB_AF03_5FF9A1BCABB2__INCLUDED_


/*

	Screen12.h : implementation file

	Incoming Offer for a player from a club

	(c) Rw Software 2003 - 2008

*/



/////////////////////////////////////////////////////////////////////////////
// CScreen12 dialog

class CScreen12Dlg : public CBitmapDialog
{
// Construction
public:
	CScreen12Dlg(CClubTransferOffer* _BidDetails, CWnd* pParent = null);   // standard constructor
	~CScreen12Dlg();

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,
	};

	enum eDIALOG12
	{
		eFEEDOWN,
		eFEEUP,
		eSELLONDOWN,
		eSELLONUP
	};

	enum 
	{
		eFEE,
		eSELLON,

		eMAXSELECTOR
	};

// Implementation
protected:
	CClubTransferOffer*		m_pBidDetails;
	CCurrentTitledTextSelector	m_CurrentTextSelector;
	CTitledTextSelector		m_TextSelector[eMAXSELECTOR];
	CMessageDisplay			m_messagedisplayResponse;
	CPlayerNameTextRegion	m_PlayerName;
	bool					m_NegotiationsEnded;

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
	virtual void			SetValues();
	
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SCREEN12_H__90F6B960_A867_45CB_AF03_5FF9A1BCABB2__INCLUDED_)
