#if !defined(AFX_SCREEN2701_H__D9C88F2A_EF5B_41C4_9627_E5155CBCD29B__INCLUDED_)
#define AFX_SCREEN2701_H__D9C88F2A_EF5B_41C4_9627_E5155CBCD29B__INCLUDED_

// Screen27TacticsPage.h : header file
//

// Screen27TacticsPage dialog

class CScreen27TacticsPage : public CBitmapPropertyPage
{
    DECLARE_DYNCREATE(CScreen27TacticsPage)

// Construction
public:
    CScreen27TacticsPage();
    ~CScreen27TacticsPage();

// Dialog Data
	#include "Screen27TacticsData.h"

	void					OnPaint();
    BOOL					OnInitDialog();
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SCREEN2701_H__D9C88F2A_EF5B_41C4_9627_E5155CBCD29B__INCLUDED_)
