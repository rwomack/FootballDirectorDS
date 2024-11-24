#if !defined(AFX_SCREEN27SPECIALPLAYERSVIEW_H__E6AC8CE9_C8D2_4879_AF0D_ACFC988E8FC5__INCLUDED_)
#define AFX_SCREEN27SPECIALPLAYERSVIEW_H__E6AC8CE9_C8D2_4879_AF0D_ACFC988E8FC5__INCLUDED_

// Screen27SpecialPlayersView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen27SpecialPlayersView dialog

class CScreen27SpecialPlayersView : public CBitmapFormView
{
// Construction
public:
	CScreen27SpecialPlayersView();   // standard constructor

	// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS,
	};
	#include "Screen27SpecialPlayersData.h"

	virtual void			OnInitialUpdate();
	void 					OnActionClicked();
};
#endif // !defined(AFX_SCREEN27SPECIALPLAYERSVIEW_H__E6AC8CE9_C8D2_4879_AF0D_ACFC988E8FC5__INCLUDED_)
 