#if !defined(AFX_SCREEN11VIEW_H__06E44A4B_A73F_47AE_B497_C412EE7F6249__INCLUDED_)
#define AFX_SCREEN11VIEW_H__06E44A4B_A73F_47AE_B497_C412EE7F6249__INCLUDED_

// Screen11View.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen11View form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen11View : public CBitmapFormView
{
public:
	CScreen11View();           // protected constructor used by dynamic creation
	virtual ~CScreen11View();

	DECLARE_DYNCREATE(CScreen11View)

// Form Data
	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS
	};

	virtual void			OnInitialUpdate();

protected:
    CMessageDisplay			m_MessageDisplay;
	CStringArray			m_strlistNews;

	virtual void			OnDraw(CDC* pDC);
	void					OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void					OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	void					OnClearDisplayLists();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_SCREEN11VIEW_H__06E44A4B_A73F_47AE_B497_C412EE7F6249__INCLUDED_)
