// KeyScrollBar.cpp : implementation file
//

#include "stdafx.h"
#include "KeyScrollBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyScrollBar

/*------------------------------------------------------------------------------
	Method:   	CKeyScrollBar::CKeyScrollBar
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CKeyScrollBar::CKeyScrollBar()
{
}


/*------------------------------------------------------------------------------
	Method:   	CKeyScrollBar::~CKeyScrollBar
	Access:    	virtual public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CKeyScrollBar::~CKeyScrollBar()
{
}


BEGIN_MESSAGE_MAP(CKeyScrollBar, CScrollBar)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyScrollBar message handlers


 /*------------------------------------------------------------------------------
 	Method:   	CKeyScrollBar::OnLButtonDown
 	Access:    	public 
 	Parameter: 	UINT _nFlags
 	Parameter: 	CPoint _point
 	Returns:   	void
 	Qualifier: 	
 	Purpose:
 ------------------------------------------------------------------------------*/
 void CKeyScrollBar::OnLButtonDown(UINT _nFlags, CPoint _point)
{
	Default();
}


/*------------------------------------------------------------------------------
	Method:   	CKeyScrollBar::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT _nChar
	Parameter: 	UINT _nRepCnt
	Parameter: 	UINT _nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CKeyScrollBar::OnKeyDown(UINT _nChar, UINT _nRepCnt, UINT _nFlags)
{
	DWORD wScrollNotify;

    switch(_nChar)
	{
        case VK_UP:
            wScrollNotify = SB_LINEUP;
            break;

        case VK_PRIOR:
            wScrollNotify = SB_PAGEUP;
            break;

        case VK_NEXT:
            wScrollNotify = SB_PAGEDOWN;
            break;

        case VK_DOWN:
            wScrollNotify = SB_LINEDOWN;
            break;

        case VK_HOME:
            wScrollNotify = SB_TOP;
            break;

        case VK_END:
            wScrollNotify = SB_BOTTOM;
            break;

		default:
			 //CScrollBar::OnKeyDown(nChar, nRepCnt, nFlags);
			return;
    }

    GetParent()->SendMessage(WM_VSCROLL, (WPARAM)wScrollNotify, (LPARAM)GetSafeHwnd());
}
