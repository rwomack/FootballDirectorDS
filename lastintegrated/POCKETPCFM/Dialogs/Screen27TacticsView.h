#if !defined(AFX_SCREEN27TACTICSVIEW_H__BDC42E3E_AEF7_42D1_BF10_10F42B11712A__INCLUDED_)
#define AFX_SCREEN27TACTICSVIEW_H__BDC42E3E_AEF7_42D1_BF10_10F42B11712A__INCLUDED_

// Screen27TacticsView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen27TacticsView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen27TacticsView : public CBitmapFormView
{
public:
	CScreen27TacticsView();           // protected constructor used by dynamic creation
	virtual ~CScreen27TacticsView();
	DECLARE_DYNCREATE(CScreen27TacticsView)

	// Form Data
	enum 
	{ 
		IDD = IDD_SCREEN2701
	};

	#include "Screen27TacticsData.h"

	void					OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
    void					OnInitialUpdate();

protected:
	CExtCheckBox			m_CounterAttackToggle;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_SCREEN27TACTICSVIEW_H__BDC42E3E_AEF7_42D1_BF10_10F42B11712A__INCLUDED_)
