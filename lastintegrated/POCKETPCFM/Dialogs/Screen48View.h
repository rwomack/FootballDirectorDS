#if !defined(AFX_SCREEN48_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_SCREEN48_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_


// Screen48View.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen48View dialog

class CScreen48View : public CGridDivisionSelectorBitmapFormView
{
// Construction
public:
    CScreen48View();   // standard constructor
	~CScreen48View();

// Dialog Data
    enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS,

		LEFTX = 4,
		GRID_WIDTH = 230,
	};

protected:
	CClubBalanceTextRegion	m_ClubBalanceText;
	CPNGDrawInfo			m_pngSmallKits;

	void					DoBuildPlayerList();
	void					OnSelchangeDivisionlist();

	void					OnPaint();
	virtual void			OnInitialUpdate();
	void					OnLButtonUp(UINT _nFlags, CPoint _Point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnSearchButton();
	void					OnSize(UINT nType, int cx, int cy);
	void 					OnActionClicked();
	void 					OnFilter();
	void 					OnSortHighestSkill();
	void 					OnSortLowestSkill();
	void 					OnSortHighestPrice();
	void 					OnSortLowestPrice();
	void 					OnViewDetails();
	void 					OnMakeOffer();
	void					OnViewPlayerMenu();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN48_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
