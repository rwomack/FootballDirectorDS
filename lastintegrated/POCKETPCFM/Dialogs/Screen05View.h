#if !defined(AFX_SCREEN05VIEW_H__47EA72E9_6D1E_459C_9E76_F490BF036479__INCLUDED_)
#define AFX_SCREEN05VIEW_H__47EA72E9_6D1E_459C_9E76_F490BF036479__INCLUDED_

// Screen05View.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen05View form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


class CScreen05View : public CBitmapFormView
{
	DECLARE_DYNCREATE(CScreen05View)
public:
	CScreen05View(); 
	virtual ~CScreen05View(); 

// Form Data
public:
	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS,
	};

	virtual void			OnInitialUpdate();

// Implementation
protected:
	CScreen05Base			m_Screen;

	virtual void			OnDraw(CDC* pDC);
	void					DoInitialiseGrid();
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPlayerPopupMenu(UINT iColumn, CPoint point);
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar);
	void 					OnActionClicked();
	void 					OnViewDetails();
	void 					OnOfferContract();
	void 					OnSkillView();
	void 					OnStatusView();
	void 					OnContractView();
	void 					OnStatsView();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_SCREEN05VIEW_H__47EA72E9_6D1E_459C_9E76_F490BF036479__INCLUDED_)
