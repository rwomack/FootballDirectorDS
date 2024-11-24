#if !defined(AFX_Screen14_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen14_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_



/*

	Class - CScreen14

	Employee contract Negotiation Dialog Box class

	Football Director

	(c) RW Software 1994 - 2004

 ---------------------------------------------------------------
 */



/////////////////////////////////////////////////////////////////////////////
// CScreen14 dialog


class CScreen14Dlg : public CBitmapDialog
{
// Construction
public:
    CScreen14Dlg(CEmployee* a_pEmployee, CString a_strPrompt, CWnd* pParent = null);   // standard constructor
	
// Dialog Data
    enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,
	};

	enum eDIALOG14
	{
		eWAGEDOWN,
		eWAGEUP,
		eCONTRACTDOWN,
		eCONTRACTUP,
	};

	enum
	{
		eWAGE,
		eCONTRACT,

		eMAXSELECTOR
	};

protected:		
	CEmployee*				m_pCurrentEmployee;
	CEmployeeNameTextRegion m_NameText;
	CContract				m_ContractRequired;
	CString					m_strPrompt;
	CMessageDisplay			m_EmployeeResponse;
	CCurrentTitledTextSelector	m_CurrentTextSelector;
	CTitledTextSelector		m_TextSelector[eMAXSELECTOR];
	bool					m_NegotiationsEnded;
	
	void					OnCancel();
	void					OnEndNegotiations();
	BOOL					OnInitDialog();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	virtual void			OnMakeOffer() = 0;
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////


class COwnedScreen14 : public CScreen14Dlg
{
public:
    COwnedScreen14(CEmployee* a_pEmployee, CString a_strPrompt, CWnd* pParent = null)
		: CScreen14Dlg(a_pEmployee, a_strPrompt, pParent) {};   // standard constructor

protected:
	BOOL					OnInitDialog();
	void					OnMakeOffer();
};
/////////////////////////////////////////////////////////////////////////////


class CNotOwnedScreen14 : public CScreen14Dlg
{
public:
    CNotOwnedScreen14(CEmployee* a_pEmployee, CString a_strPrompt, CWnd* pParent = null)
		: CScreen14Dlg(a_pEmployee, a_strPrompt, pParent) {};   // standard constructor

protected:
	BOOL					OnInitDialog();
	void					OnMakeOffer();
};
#endif // !defined(AFX_Screen14_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
