#if !defined(AFX_Screen2702_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen2702_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen27SpecialPlayersPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen27SpecialPlayersDlg dialog

class CScreen27SpecialPlayersDlg : public CInMatchActionBitmapDialog
{

// Construction
public:
    CScreen27SpecialPlayersDlg(CWnd* pParent = null);
    ~CScreen27SpecialPlayersDlg();


	// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS,

		PICGAP = 36,
		CAPTAINY = 42,
		PENALTYTAKERY = CAPTAINY + PICGAP,
		CORNERTAKERY =  PENALTYTAKERY + PICGAP,
		FREEKICKTAKERY = CORNERTAKERY + PICGAP,
		NAMEX = 70,
		PICX = NAMEX - 60,
	};
	#include "Screen27SpecialPlayersData.h"

    virtual BOOL			OnInitDialog();

};
#endif // !defined(AFX_Screen2702_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
