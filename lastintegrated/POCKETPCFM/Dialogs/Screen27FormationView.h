#if !defined(AFX_SCREEN27FORMATIONVIEW_H__F31C85A9_D62D_4D5B_9C18_94F133920263__INCLUDED_)
#define AFX_SCREEN27FORMATIONVIEW_H__F31C85A9_D62D_4D5B_9C18_94F133920263__INCLUDED_

// Screen27FormationView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen27FormationView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CExtRect;

class CScreen27FormationView : public CBitmapFormView
{
public:
	CScreen27FormationView();           // protected constructor used by dynamic creation
	virtual					~CScreen27FormationView();

	// Form Data
	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS 
	};
	#include "Screen27FormationData.h"
	
	virtual void			OnInitialUpdate();
	void					OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	void					OnDraw(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_SCREEN27FORMATIONVIEW_H__F31C85A9_D62D_4D5B_9C18_94F133920263__INCLUDED_)
