#if !defined(AFX_Screen2702_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
#define AFX_Screen2702_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_

// Screen27SpecialPlayersPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen27SpecialPlayersPage dialog

class CScreen27SpecialPlayersPage : public CBitmapPropertyPage
{
    DECLARE_DYNCREATE(CScreen27SpecialPlayersPage)

// Construction
public:
    CScreen27SpecialPlayersPage();
    ~CScreen27SpecialPlayersPage();

	void SetCurrentMatchPlay(CMatchPlay* _Match)				
	{ 
		ASSERT_POINTER(_Match, CMatchPlay);
		m_pCurrentMatchPlay = _Match; 
	}

#include "Screen27SpecialPlayersData.h"

	CMatchPlay*				m_pCurrentMatchPlay;

	CClub&  GetClub();
	BOOL					OnKillActive();
	BOOL					OnSetActive();
    virtual BOOL			OnInitDialog();
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_Screen2702_H__4BC64571_8D77_427E_8758_1AE49585B856__INCLUDED_)
