
/*

	Screen23.h : header file for CScreen23


	(c) Rw Software 2004 - 2008

  ---------------------------------------------------------------------------------------
 */


#if !defined(AFX_Screen23_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen23_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// CScreen23 dialog


class CScreen23Dlg : public CPlayerContractDlg
{
// Construction
public:
    CScreen23Dlg(CString strPrompt, CPlayer* pPlayer, CClub* pClub, CWnd* pParent = null);   // standard constructor

// Dialog Data
    enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,
	};

protected:
	CPlayer*				m_pCurrentPlayer;
	CClub*					m_pClub;
	CPlayerNameTextRegion	m_PlayerName;

	void					OnCancel();
	virtual BOOL			OnInitDialog();
	void					OnSize(UINT nType, int cx, int cy);
	virtual void			OnOfferContract() = 0;

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class COwnedScreen23 : public CScreen23Dlg
{
public:
    COwnedScreen23(CString strPrompt, CPlayer* pPlayer, CClub* pClub, CWnd* pParent = null);   // standard constructor
	
protected:
	BOOL					OnInitDialog();
	void					OnOfferContract();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class COutOfContractScreen23 : public CScreen23Dlg
{
public:
    COutOfContractScreen23(CPlayer* a_pPlayer, CClub* a_pClub, bool* a_bResult, CContract* a_pPlayerContract, CWnd* pParent = null);   // standard constructor
	
protected:
	bool*					m_bResult;

	BOOL					OnInitDialog();
	void					OnOfferContract();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CNotOwnedScreen23 : public CScreen23Dlg
{
public:
	CNotOwnedScreen23(CTransferNegotiation* pNegotiations, CWnd* pParent = null);

protected:
	CTransferNegotiation*	m_pNegotiations;

	BOOL					OnInitDialog();
	void					OnOfferContract();
};

#endif // !defined(AFX_Screen23_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
