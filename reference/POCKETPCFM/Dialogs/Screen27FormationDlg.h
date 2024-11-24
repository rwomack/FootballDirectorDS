#if !defined(AFX_SCREEN2703_H__D9C88F2A_EF5B_41C4_9627_E5155CBCD29B__INCLUDED_)
#define AFX_SCREEN2703_H__D9C88F2A_EF5B_41C4_9627_E5155CBCD29B__INCLUDED_

// Screen27FormationDlg.h : header file
//

class CExtRect;

/////////////////////////////////////////////////////////////////////////////
// Screen27FormationDlg  dialog

class CScreen27FormationDlg : public CInMatchActionBitmapDialog
{

// Construction
public:
    CScreen27FormationDlg();
    ~CScreen27FormationDlg();

	// Form Data
	enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,
	};
	#include "Screen27FormationData.h"

	BOOL					OnInitDialog();
    void					OnPaint();
};
#endif // !defined(AFX_SCREEN2703_H__D9C88F2A_EF5B_41C4_9627_E5155CBCD29B__INCLUDED_)
