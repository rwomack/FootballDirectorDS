#if !defined(AFX_SCREEN18VIEW_H__374A8C1D_29F5_49AB_BEE6_9AB23CB76AD2__INCLUDED_)
#define AFX_SCREEN18VIEW_H__374A8C1D_29F5_49AB_BEE6_9AB23CB76AD2__INCLUDED_


// Screen18View.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen18View form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen18View : public CBitmapFormView
{
public:
	CScreen18View();           // protected constructor used by dynamic creation
	virtual ~CScreen18View();
	
	DECLARE_DYNCREATE(CScreen18View)

// Form Data
	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS,
	};
	
	virtual void			OnInitialUpdate();

// Implementation
protected:
	CClubFixtureGrid		m_Grid;

	void					InitRedrawArea();
	virtual void			OnPaint();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar);
	void					OnActionClicked();
	void					OnViewMatchReport();
	void					OnViewOpponentSquad();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_SCREEN18VIEW_H__374A8C1D_29F5_49AB_BEE6_9AB23CB76AD2__INCLUDED_)
