#if !defined(AFX_SCREEN19_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_SCREEN19_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_


// Screen19.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen19View FormView

class CScreen19View : public CGridDivisionSelectorBitmapFormView
{
// Construction
public:
    CScreen19View();   // standard constructor
    ~CScreen19View();   // standard destructor

	
	// Dialog Data
    enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS,
	};
	
	enum eDIALOG19
	{
		eDATEDOWN = 2,
		eDATEUP,
	};

protected:
	CTextRegion				m_DateText;
	CCalendar				m_TheDate;
	CSoccerResultList		m_ResultList;

	virtual void			OnInitialUpdate(void);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonUp(UINT _Flags, CPoint _Point);
	void					OnLButtonDown(UINT _Flags, CPoint _Point);
	void					OnNextDate(void);
	void					OnNewDate(void);
	void					OnPaint();
	void					OnPreviousDate(void);
	void					OnSelchangeDivisionlist(void);
	void					OnSize(UINT nType, int cx, int cy);
	void					OnActionClicked();
	void					OnNextDivision();
	void					OnPreviousDivision();
	void					OnViewMatchReport();

	void					SetGridColumnWidths();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN19_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
