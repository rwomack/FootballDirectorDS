#if !defined(AFX_Screen20_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen20_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen20View.h : header file
//

/////////////////////////////////////////////////////////////////////////////


// CScreen20View dialog

class CScreen20View : public CGridCupSelectorBitmapFormView
{
// Construction
public:
    CScreen20View();   // standard constructor
	~CScreen20View();

	DECLARE_DYNCREATE(CScreen20View)

// Dialog Data
    enum 
	{ 
		TOPY20 = DATEDIVISIONSELECTOR_TOPY + 16,
		GRID_WIDTH = 234,
		
		IDD = IDD_SCREENNOCONTROLS
	};

	enum eDIALOG20
	{
		eROUNDDOWN = 2,
		eROUNDUP,
	};

// Implementation
protected:
	int						m_CurrentSelector;
	int						m_iCurrentRound;
	int						m_RoundY;
	CSoccerResultList		m_ResultList;
	CString					m_Date;
	bool					m_bShowReplays;
	
	void					BuildMatchDisplayList();
	CCupRound&				GetCurrentCupRound();
	void					MoveGrid();
	virtual void			OnInitialUpdate();
	void					OnLButtonUp(UINT _nFlags, CPoint _Point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnNextCupRound();
	void					OnPaint();
	void					OnPreviousCupRound();
 	void					OnSelchangeCupList();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnActionClicked();
	void					OnNextCup();
	void					OnPreviousCup();
	void					OnNextRound();
	void					OnPreviousRound();
	void					OnViewGroupTables();
	void					OnViewMatchReport();

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_Screen20_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
