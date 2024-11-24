#if !defined(AFX_SCREEN2_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_SCREEN2_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

/*

---------------------------------------------------------------

	Screen2.h : header file

	Cup Schedules

	Football Director

	(c) Rw Software 1994 - 2008

---------------------------------------------------------------

*/


// CScreen02View FormView
class CScreen02View : public CGridCupSelectorBitmapFormView
{
public:
// Construction
    CScreen02View();   // standard constructor
	virtual ~CScreen02View();

	DECLARE_DYNCREATE(CScreen02View)

// Dialog Data
    enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS,
	};

// Implementation
protected:
	void					DoAdjustColumnWidths();

	void					OnActionClicked();
	virtual void			OnInitialUpdate();
	void					OnNextCup();
	void					OnPreviousCup();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonUp(UINT _nFlags, CPoint _Point);
	void					OnPaint();
	void					OnSelchangeCupList();
	void					OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN2_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
