#if !defined(AFX_PLAYERCONTRACTDLG_H__90F6B960_A867_45CB_AF03_5FF9A1BCABB2__INCLUDED_)
#define AFX_PLAYERCONTRACTDLG_H__90F6B960_A867_45CB_AF03_5FF9A1BCABB2__INCLUDED_


/*

	PlayerContractDlg.h : implementation file

	(c) Sports Director Ltd 2007 - 2008 - 2008

*/


class CPlayerContractDlg : public CBitmapDialog
{
	// Construction
public:
	CPlayerContractDlg(CString strPrompt, UINT nIDTemplate, CWnd* pParent = null);   // standard constructor

	enum 
	{
		eCONTRACTDOWN,
		eCONTRACTUP,
		eWAGEDOWN,
		eWAGEUP,
		eSIGNONFEEDOWN,
		eSIGNONFEEUP,
	};

	enum
	{
		eWAGE,
		eSIGNONFEE,
		eCONTRACT,

		eMAXSELECTOR
	};

protected:
	CCurrentTitledTextSelector	m_CurrentTextSelector;
	CTitledTextSelector		m_TextSelector[eMAXSELECTOR];
	CContract				m_ContractRequired;
	CContract				m_ContractOffered;
	CMessageDisplay			m_Response;
	CString					m_Prompt;
	bool					m_NegotiationsEnded;

	void					OnEndNegotiations();
	BOOL					OnInitDialog();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnOfferContract();
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
};

#endif //AFX_PLAYERCONTRACTDLG_H__90F6B960_A867_45CB_AF03_5FF9A1BCABB2__INCLUDED_

