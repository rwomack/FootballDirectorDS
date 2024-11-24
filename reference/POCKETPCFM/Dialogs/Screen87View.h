#if !defined(AFX_SCREEN87VIEW_H__06E44A4B_A73F_47AE_B497_C412EE7F6249__INCLUDED_)
#define AFX_SCREEN87VIEW_H__06E44A4B_A73F_47AE_B497_C412EE7F6249__INCLUDED_

// Screen87View.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen87View form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen87View : public CBitmapFormView
{
public:
	CScreen87View();
	virtual ~CScreen87View() {};
	DECLARE_DYNCREATE(CScreen87View)

// Form Data
	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS 
	};

protected:
	CVOImage				m_Title;
	CVOImage				m_SDLogo;
	CVOImage				m_FDLogo;

	virtual void			OnInitialUpdate();
	void					OnKillFocus(CWnd* _pNewWnd);
	virtual void			OnPaint();
	void					OnSetFocus(CWnd* _pOldWnd);
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_SCREEN28VIEW_H__06E44A4B_A73F_47AE_B497_C412EE7F6249__INCLUDED_)
