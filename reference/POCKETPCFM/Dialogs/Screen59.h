#if !defined(AFX_Screen59_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen59_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen59.h : Youth profile header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen59 dialog

class CScreen59Dlg : public CBitmapDialog
{
// Construction
public:
    CScreen59Dlg(CWnd* pParent = null);   // standard constructor
	~CScreen59Dlg();
	
// Dialog Data
    enum 
	{ 
		GAPY = 17,

		IDD = IDD_SCREENDLGNOCONTROLS,
	};

	enum
	{
		ePLAYERNAME,
		ePLAYERPOSITION,
		ePLAYERAGE,
		eHANDLING, 
		eTACKLING, 
		ePASSING, 
		eSHOOTING,
		eBSTFOOT,
		ePACE,
		eTEMPER,
		eFITNESS, 
		eMORALE,

		ePLAYERAGEVALUE,
		eHANDLINGVALUE, 
		eTACKLINGVALUE, 
		ePASSINGVALUE, 
		eSHOOTINGVALUE, 
		eBESTFOOTVALUE,
		ePACEVALUE,
		eTEMPERVALUE,
		eFITNESSVALUE, 
		eMORALEVALUE,

		eNUMTEXTREGION
	};

// Overrides
	int						DoModal(CYouth* _Youth);
	void					SetCurrentYouth(CYouth* _Youth);			

protected:
	CYouth*					m_pCurrentYouth;

	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnPaint();
	virtual BOOL			OnInitDialog();
	virtual void			OnButton1() = 0;
	virtual void			OnButton2() = 0;
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////


class COwnedScreen59 : public CScreen59Dlg
{
public:
	BOOL					OnInitDialog();

protected:
	void					OnButton1();		// Promote to 1st team squad
	void					OnButton2();		// Sack
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////


class CNotOwnedScreen59 : public CScreen59Dlg
{
public:
	BOOL					OnInitDialog();

protected:
	void					OnButton1();		// Hire
	void					OnButton2()			{};

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_Screen59_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
