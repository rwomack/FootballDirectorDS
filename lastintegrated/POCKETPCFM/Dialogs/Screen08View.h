#if !defined(AFX_SCREEN08VIEW_H__29AF3D7F_6AB7_4F67_95BE_A6351BF60002__INCLUDED_)
#define AFX_SCREEN08VIEW_H__29AF3D7F_6AB7_4F67_95BE_A6351BF60002__INCLUDED_

// Screen08View.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen08View dialog

class CScreen08View : public CBitmapFormView
{
// Construction
public:
	CScreen08View();
	~CScreen08View(); 

	DECLARE_DYNCREATE(CScreen08View)
// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS,
		GRID_WIDTH = 230
	};

	// Overrides
	virtual BOOL			PreCreateWindow(CREATESTRUCT& cs);
	virtual void			OnInitialUpdate();

protected:
	// Implementation
	CGrid					m_Grid;
	eSortOrder				m_SortOrder;
	int						m_WageMultiplier;

	int						m_WhichEvents;
	uint					m_dTotalEmpWages;
	uint					m_dTotalYouthWages;
	uint					m_dTotalPlayerWages;
	ushort					m_iPlayInfoNum;
	CPNGDrawInfo			m_pngPlayerStatus;

	void					DoBuildList(bool bPreservePosition = false);
	void					DoSort();
	void					OnActionClicked();
	void					OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void			OnDraw(CDC* pDC);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnOfferContract();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnSortExpiryDate();
	void					OnSortHighestEarner();
	void					OnSortLowestEarner();
	void					OnSwitchTab();
	void					OnViewDetails();
	void					OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar);
	void					OnViewExpiryDate();
	void					OnViewWage();
	void					OnViewWeeklyWage();
	void					OnViewAnnualWage();

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SCREEN08VIEW_H__29AF3D7F_6AB7_4F67_95BE_A6351BF60002__INCLUDED_)
