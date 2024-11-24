#if !defined(AFX_SCREEN28VIEW_H__06E44A4B_A73F_47AE_B497_C412EE7F6249__INCLUDED_)
#define AFX_SCREEN28VIEW_H__06E44A4B_A73F_47AE_B497_C412EE7F6249__INCLUDED_

// Screen28View.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen28View form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen28View : public CBitmapFormView
{
public:
	CScreen28View();
	virtual ~CScreen28View() {};
	DECLARE_DYNCREATE(CScreen28View)

// Form Data
	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS 
	};

	void					OnIncrementProgress();
	void					OnSetDate();
	CProgressCtrl*			GetProgressCtl();
	
protected:
	CProgressCtrl			m_ProgressBar;
	CPNGDrawInfo			m_pngLargeKits;
	
	virtual void			OnInitialUpdate();
	virtual void			OnPaint();
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_SCREEN28VIEW_H__06E44A4B_A73F_47AE_B497_C412EE7F6249__INCLUDED_)
