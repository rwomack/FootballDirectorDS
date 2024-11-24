#if !defined(AFX_Screen60_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen60_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen60.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen60 dialog

class CScreen60Dlg : public CBitmapDialog
{
// Construction
public:
    CScreen60Dlg(CEmployee* _Employee, CWnd* pParent = null);   // standard constructor

	void				SetpCurrentEmployee(CEmployee* pEmployee);

// Dialog Data
    enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,
		GAPY = 17,

	};

	enum 
	{
		eNAME,
		eROLE,
		eAGE,
		eAGEVALUE,
		eNATIONALITY,
		eNATIONALITYVALUE,
		eWAGE,
		eWAGEVALUE,
		eCONTRACT,
		eCONTRACTVALUE,
		eRATING,
		eRATINGVALUE,

		eNUMTEXTREGION
	};

// Overrides

// Implementation
protected:
	CEmployee*				m_pCurrentEmployee;
	CTextRegion				m_NameStatic;
    CTextRegion				m_RoleStatic;
	CTextRegion				m_AgeStatic;
    CTextRegion				m_AgeValueStatic;
	CTextRegion				m_NationalityStatic;
    CTextRegion				m_NationalityValueStatic;
	CTextRegion				m_WageStatic;
    CTextRegion				m_WageValueStatic;
	CTextRegion				m_ContractStatic;
    CTextRegion				m_ContractValueStatic;
	CTextRegion				m_RatingStatic;
    CTextRegion				m_RatingValueStatic;

	virtual void			OnButton1() = 0;
	virtual void			OnButton2() = 0;
	void					OnPaint();
	virtual BOOL			OnInitDialog();
	void					CheckIfWillNegotiate();
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////


class COwnedScreen60 : public CScreen60Dlg
{
public:
	COwnedScreen60(CEmployee* _Employee, CWnd* pParent = null);   // standard constructor

	BOOL					OnInitDialog();

protected:
	void					OnButton1();	// Offer Contract
	void					OnButton2();	// Sack

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////


class CNotOwnedScreen60 : public CScreen60Dlg
{
public:
	CNotOwnedScreen60(CEmployee* _Employee, CWnd* pParent = null);   // standard constructor

	BOOL					OnInitDialog();

protected:
	void					OnButton1();		// Hire
	void					OnButton2()			{};

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_Screen60_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
