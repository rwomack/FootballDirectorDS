#if !defined(AFX_Screen18Dlg_H__374A8C1D_29F5_49AB_BEE6_9AB23CB76AD2__INCLUDED_)
#define AFX_Screen18Dlg_H__374A8C1D_29F5_49AB_BEE6_9AB23CB76AD2__INCLUDED_


// Screen18Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen18Dlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


class CScreen18Dlg : public CBitmapDialog
{
public:
	CScreen18Dlg(CClub*	pCurrentClub, const eClubResultsType eMatchType, CWnd* pParent = null);
	virtual ~CScreen18Dlg();
	
// Form Data
	enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,
	};
	
// Implementation
protected:
	CClubFixtureGrid		m_Grid;
	CClubNameTextRegion		m_ClubName;
	CClub*					m_pCurrentClub;

	void					InitRedrawArea();
	virtual BOOL			OnInitDialog();
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_Screen18Dlg_H__374A8C1D_29F5_49AB_BEE6_9AB23CB76AD2__INCLUDED_)
