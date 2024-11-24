#if !defined(AFX_SCREEN16_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_SCREEN16_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen16.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen16 dialog

class CScreen16Dlg : public CBitmapDialog
{
	DECLARE_DYNAMIC(CScreen16Dlg)

// Construction
public:
	CScreen16Dlg(CPlayer* pPlayer, CWnd* pParent = null);   // standard constructor

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREEN16, 

		BUTTON5W = CANCELBUTTONWIDTH,
		BUTTON5H = CANCELBUTTONHEIGHT,
	};

	enum eDIALOG61
	{
		eFEEDOWN,
		eFEEUP,
		eCANCEL,
	};
	
	enum 
	{
		eFEE,

		eMAXSELECTOR
	};

protected:
	virtual void			DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CPlayer*				m_pCurrentPlayer;
	CPlayerNameTextRegion	m_PlayerName;
	//CCurrentTitledTextSelector m_CurrentTextSelector;
	CTextSelector			m_TextSelector[eMAXSELECTOR];

	void					OnButtonRejectAllOffers();
	void					OnButtonRejectOffersBelow();
	void					OnButtonTransferList();
	void					OnButtonLoanList();
	void					OnButtonOk(); 
	void					OnButtonCancel(); 
	virtual BOOL			OnInitDialog();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN16_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
