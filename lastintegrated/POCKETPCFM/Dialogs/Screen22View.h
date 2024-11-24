#if !defined(AFX_SCREEN22VIEW_H__702EA47A_2E5D_4ED3_87A1_EFD93688D0EE__INCLUDED_)
#define AFX_SCREEN22VIEW_H__702EA47A_2E5D_4ED3_87A1_EFD93688D0EE__INCLUDED_

// Screen22View.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen22View dialog


class CScreen22View : public CBitmapFormView
{
// Construction
public:
	CScreen22View();
	DECLARE_DYNCREATE(CScreen22View)

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS, 

		eDOWNX = 132,
	
		BUTTON1Y = TOPY,

		BUTTON4Y = TOPY + 48,
	};

	enum eDIALOG22
	{
		eBORROWDOWN,
		eBORROWUP,

		eREPAYDOWN,
		eREPAYUP, 

	};

	enum
	{
		eBORROW,
		eREPAY,

		eMAXSELECTOR
	};

protected:
	CCurrentTitledTextSelector	m_CurrentTextSelector;
	CTitledTextSelector		m_TextSelector[eMAXSELECTOR];
	CInvisibleGrid			m_Grid;


	void					DoUpdateLoandetails();
	virtual void			OnInitialUpdate();
	virtual void			OnDraw(CDC* pDC);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnSize(UINT nType, int cx, int cy);
	void 					OnActionClicked();
	void 					OnApplyLoan();
	void 					OnRepayLoan();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN22VIEW_H__702EA47A_2E5D_4ED3_87A1_EFD93688D0EE__INCLUDED_)
