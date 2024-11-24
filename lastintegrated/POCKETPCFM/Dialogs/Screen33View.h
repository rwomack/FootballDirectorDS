#if !defined(AFX_SCREEN33_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_SCREEN33_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_


/*

	Screen33View.h : Leading goalscorers/disciplined players CFormView header file

	Football Director

	(c) Rw Software 1994 - 2008

	---------------------------------------------------------------------------------------
*/


class CScreen33View : public CGridDivisionSelectorBitmapFormView
{

// Construction
public:
    CScreen33View();
	~CScreen33View();

	// Dialog Data
    enum 
	{ 
		MAXLISTSIZE	= 15,
		TOPY33 = DIVISIONSELECTOR_TOPY + 12,

		IDD = IDD_SCREENNOCONTROLS 
	};


// Overrides
protected:
    CLeadingDisciplinaryList m_PointScorers;
    CLeadingGoalScorerList	m_GoalScorers;
	int						m_ViewMode;
	CPNGDrawInfo			m_pngSmallKits;

// Implementation
	void					DoBuildGrid();
	void					SetGridColumnWidths();
	void 					OnActionClicked();
	virtual void			OnInitialUpdate();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT _Flags, CPoint _Point);
	void					OnLButtonUp(UINT _Flags, CPoint _Point);
	void					OnPaint();
	void					OnSelchangeDivisionlist();
	void					OnSetFocus(CWnd* _OldWnd);
	void					OnSize(UINT nType, int cx, int cy);
	void 					OnViewDisciplinary();
	void 					OnViewGoals();
	void 					OnViewPlayerDetails();
	void 					OnViewPlayerBreakdown();
	void					OnViewSquadDetails();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN33_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
