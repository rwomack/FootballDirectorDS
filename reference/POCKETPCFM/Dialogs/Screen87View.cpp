 // Screen87View.cpp : implementation file
//

#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "FootballDirector.h"
#include "Screen87View.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen87View

IMPLEMENT_DYNCREATE(CScreen87View, CFormView)


/*------------------------------------------------------------------------------
	Method:   	CScreen87View::CScreen87View
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen87View::CScreen87View()
	: CBitmapFormView(CScreen87View::IDD) {}


BEGIN_MESSAGE_MAP(CScreen87View, CBitmapFormView)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen87View message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen87View::OnKillFocus
	Access:    	protected 
	Parameter: 	CWnd * _pNewWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen87View::OnKillFocus(CWnd* _pNewWnd)
{
	//CBitmapDialog::DoTurnOffFullScreen();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen87View::OnSetFocus
	Access:    	protected 
	Parameter: 	CWnd * _pOldWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen87View::OnSetFocus(CWnd* _pOldWnd)
{
	// Call SHInitDialog with flags for full screen.
	SHINITDLGINFO shidi;
	shidi.dwMask = SHIDIM_FLAGS;
	shidi.dwFlags = 0;
	shidi.hDlg = m_hWnd;
	::SHInitDialog(&shidi);

	//	HWND cmdBar = ::SHFindMenuBar(this->GetSafeHwnd());
	//	::ShowWindow(cmdBar, SW_SHOW);

	// SHFullScreen fails if dialog box is not foreground.
	SetForegroundWindow();
	SHFullScreen(m_hWnd, SHFS_HIDETASKBAR);
	SHFullScreen(m_hWnd, SHFS_HIDESIPBUTTON);
	SHFullScreen(m_hWnd, SHFS_HIDESIPBUTTON);
	SHFullScreen(m_hWnd, SHFS_HIDESIPBUTTON);
	SHFullScreen(m_hWnd, SHFS_HIDESIPBUTTON);
	SHFullScreen(m_hWnd, SHFS_HIDESTARTICON);

	RECT rect;
	GetWindowRect(&rect);
	rect.top = 0;
	MoveWindow(&rect, TRUE);
//	SetWindowPos(_pOldWnd, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen87View::OnInitialUpdate
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen87View::OnInitialUpdate()
{
	CBitmapFormView::OnInitialUpdate(IDS_HELPSCREEN99, &theApp.GetBasicDisplay());
	CBitmapFormView::DoInitFullScreen();
	HDC dc = ::GetDC(m_hWnd);
	m_Title.SetBitmap(dc, IDR_PNGTITLEBACK, _T("PNG"));
	m_SDLogo.SetBitmap(dc, IDR_PNGLOGO, _T("PNG"));
	m_FDLogo.SetBitmap(dc, IDR_PNGLOGOFD, _T("PNG"));
	::ReleaseDC(m_hWnd, dc);
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen87View::OnDraw
	Access:    	virtual protected 
	Parameter: 	CDC * pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen87View::OnPaint()
{
	GetBasicDisplay()->OnInitRedrawArea(this);
	GetBasicDisplay()->OnPaint(false);
	COffScreenBuffer& memDC = *CBitmapFormView::GetMemDC();
	m_Title.Draw(memDC.operator HDC(), 0, 0);
	m_FDLogo.DoDrawTransparent(memDC.operator HDC(), GetCentreX() - 105, 20);
	m_SDLogo.DoDrawTransparent(memDC.operator HDC(), GetWidth() - 130, GetHeight() - 80);
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen87View::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen87View::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);
	}
	Sleep(1);
}