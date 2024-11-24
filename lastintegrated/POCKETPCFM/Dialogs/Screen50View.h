#if !defined(AFX_SCREEN50VIEW_H__8E61EED4_3191_42A9_B375_7678C84D4BCF__INCLUDED_)
#define AFX_SCREEN50VIEW_H__8E61EED4_3191_42A9_B375_7678C84D4BCF__INCLUDED_

// Screen50View.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen50View form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen50View : public CGridDivisionSelectorBitmapFormView
{
public:
	virtual ~CScreen50View();
	CScreen50View();
	DECLARE_DYNCREATE(CScreen50View)

// Form Data
	enum
	{
		IDD = IDD_SCREENNOCONTROLS
	};

// Implementation
protected:
	int						m_ViewMode;
	CString					m_strPrint;
  	CIndexedIntArray		m_FairPlayList;
	int						m_ClubNameLength;

	void					DoBuildGrid();
	void					OnSelchangeDivisionlist();

	virtual void			OnPaint();
	virtual void			OnInitialUpdate();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT _Flags, CPoint _Point);
	void					OnLButtonUp(UINT _Flags, CPoint _Point);
	void					OnSize(UINT nType, int cx, int cy);
	void 					OnActionClicked();
	void 					OnOverall();
	void 					OnHome();
	void 					OnAway();
	void 					OnForm();
	void 					OnDiscipline();
	void					OnViewSquadDetails();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_SCREEN50VIEW_H__8E61EED4_3191_42A9_B375_7678C84D4BCF__INCLUDED_)
