
/*

	CScrollDialog.cpp : implementation file

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "aygshell.h"
#include "UI.h"
#include "BitmapDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CBitmapDialog, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_GETDLGCODE()
	ON_WM_HELPINFO()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::CBitmapDialog
	Access:    	public 
	Parameter: 	UINT nIDTemplate
	Parameter: 	CWnd * pParentWnd
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CBitmapDialog::CBitmapDialog(UINT nIDTemplate, CWnd* pParentWnd) 
	: CDialog(nIDTemplate, pParentWnd)
{
	m_IsInitialised = false;
	m_BasicDisplay = new CBasicDisplay();
}


/*------------------------------------------------------------------------------
Method:   	CBitmapDialog::~CBitmapDialog
Access:    	virtual public 
Returns:   	
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CBitmapDialog::~CBitmapDialog()
{
	//CBitmapDialog::GetBasicDisplay()->ButtonList().RemoveList();
	delete m_BasicDisplay;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::DoDisableOKButton
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::DoDisableOKButton()
{
#ifdef POCKETPC
	SHDoneButton(SHDB_HIDE);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::DoEnableOKButton
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::DoEnableOKButton()
{
#ifdef POCKETPC
	SHDoneButton(SHDB_SHOW);	  // Show 'ok' on dialog
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::SetIsInitialised
	Access:    	public 
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::SetIsInitialised(bool _Value)
{
	m_IsInitialised = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::IsInitialised
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CBitmapDialog::IsInitialised()
{
	return m_IsInitialised;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnLButtonDown
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	Default();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_UP:
		case VK_DOWN:
			GetGrid().OnKeyDown(nChar, nRepCnt, nFlags);
			ReDraw();
			break;

		case VK_RETURN:
		case VK_F23:
			OnOK();
			break;
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::ReDraw
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::ReDraw()
{
	//m_BasicDisplay->OnInitRedrawArea(this);
	Invalidate();
	UpdateWindow();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnHelpInfo
	Access:    	public 
	Parameter: 	HELPINFO * pHelpInfo
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CBitmapDialog::OnHelpInfo(HELPINFO* _HelpInfo)
{
	return GetBasicDisplay()->OnHelpInfo(_HelpInfo);
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnPaint
	Access:    	virtual public 
	Parameter: 	const bool bShowDate
	Parameter: 	bool bShouldBlit
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::OnPaint(const bool bShowDate, bool bShouldBlit)
{
	GetBasicDisplay()->OnPaint(bShowDate, bShouldBlit);
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnInitDialog
	Access:    	public 
	Parameter: 	const int _HelpTopic
	Parameter: 	const int _StrID
	Parameter: 	UINT _BitmapID
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CBitmapDialog::OnInitDialog(const int _HelpTopic, const int _StrID, UINT _BitmapID)
{
	BOOL bRet = OnInitDialog(_HelpTopic, _StrID);
	GetBasicDisplay()->SetBitmap(_BitmapID);
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnInitDialog
	Access:    	public 
	Parameter: 	const int _HelpTopic
	Parameter: 	const int _StrID
	Parameter: 	const bool _FullScreen
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CBitmapDialog::OnInitDialog(const int _HelpTopic, const int _StrID, const bool _FullScreen /*= false*/)
{
	GetBasicDisplay()->OnInitDialog(this, _HelpTopic, _StrID);
	//GetGrid().OnInitialUpdate(this, GetBasicDisplay());
	GetGrid().DoLoadBitmapBars(m_hWnd, IDR_PNGHEADER2, IDR_PNGHEADER3);
	BOOL bRet = CDialog::OnInitDialog();

// 	SHINITDLGINFO shidi;
// 	shidi.dwMask = SHIDIM_FLAGS;
// 	shidi.hDlg = m_hWnd;
// 	RECT rect;
// 	GetWindowRect(&rect);
// 	if (_FullScreen == true)
// 	{
// 		// Call SHInitDialog with flags for full screen.
// 		shidi.dwFlags = SHIDIF_FULLSCREENNOMENUBAR;
// 		::SHInitDialog(&shidi);
// 
// 		HWND cmdBar = ::SHFindMenuBar(this->GetSafeHwnd());
// 		::ShowWindow(cmdBar,SW_HIDE);
// 
// 		// SHFullScreen fails if dialog box is not foreground.
// 		SetForegroundWindow();
// 		SHFullScreen(m_hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON);
// 
// 		// Resize the window over the taskbar area.
// 		rect.top -= MENU_HEIGHT;
// 	}
// 	else
// 	{
// 		shidi.dwFlags = 0;
// 		::SHInitDialog(&shidi);
// 
// 		HWND cmdBar = ::SHFindMenuBar(this->GetSafeHwnd());
// 		::ShowWindow(cmdBar,SW_SHOW);
// 
// 		// SHFullScreen fails if dialog box is not foreground.
// 		SetForegroundWindow();
// 
// 		// Resize the window over the taskbar area.
// 		rect.top += MENU_HEIGHT;
// 	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnEraseBkgnd
	Access:    	virtual public 
	Parameter: 	CDC * pDC
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CBitmapDialog::OnEraseBkgnd(CDC* pDC)
{
	return GetBasicDisplay()->OnEraseBkgnd(pDC);
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::GetCentreX
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBitmapDialog::GetCentreX()
{
	CRect rectWnd;
	GetClientRect(&rectWnd);
	return rectWnd.CenterPoint().x;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::DoBlit
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::DoBlit()
{
	GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::DoInitFullScreen
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::DoInitFullScreen()
{
	// Call SHInitDialog with flags for full screen.
	SHINITDLGINFO shidi;
	shidi.dwMask = SHIDIM_FLAGS;
	shidi.dwFlags = SHIDIF_FULLSCREENNOMENUBAR;
	shidi.hDlg = m_hWnd;
	::SHInitDialog(&shidi);

	HWND cmdBar = ::SHFindMenuBar(this->GetSafeHwnd());
	::ShowWindow(cmdBar,SW_HIDE);

	// SHFullScreen fails if dialog box is not foreground.
	SetForegroundWindow();
//	SHFullScreen(m_hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON);
	SHFullScreen(m_hWnd, SHFS_HIDETASKBAR);
	SHFullScreen(m_hWnd, SHFS_HIDESIPBUTTON);
	SHFullScreen(m_hWnd, SHFS_HIDESTARTICON);

   // Resize the window over the taskbar area.
	RECT rect;
	GetWindowRect(&rect);
	rect.top -= MENU_HEIGHT;
	MoveWindow(&rect, TRUE);
//	GetBasicDisplay()->SetFullScreen();
//	OnInitRedrawArea();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::DoTurnOffFullScreen
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::DoTurnOffFullScreen()
{
	GetBasicDisplay()->SetFullScreen();
	OnInitRedrawArea();

	// Call SHInitDialog with flags for full screen.
	SHINITDLGINFO shidi;
	shidi.dwMask = SHIDIM_FLAGS;
	shidi.dwFlags = 0;
	shidi.hDlg = m_hWnd;
	::SHInitDialog(&shidi);

	HWND cmdBar = ::SHFindMenuBar(this->GetSafeHwnd());
	::ShowWindow(cmdBar,SW_SHOW);

   // SHFullScreen fails if dialog box is not foreground.
	SetForegroundWindow();
	SHFullScreen(m_hWnd, SHFS_SHOWTASKBAR);
	SHFullScreen(m_hWnd, SHFS_SHOWSIPBUTTON);
	SHFullScreen(m_hWnd, SHFS_SHOWSTARTICON);

//	SetWindowPos(m_hWnd, -1, -1, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE);

// 	RECT rect;
// 	GetWindowRect(&rect);
// 	rect.top -= MENU_HEIGHT;
// 	MoveWindow(&rect, TRUE);
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnGetDlgCode
	Access:    	virtual public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CBitmapDialog::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::DoLoadBitmap
	Access:    	public 
	Parameter: 	const UINT nIDResource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::DoLoadBitmap(const UINT nIDResource)
{
	HDC dc = ::GetDC(m_hWnd);
	GetBasicDisplay()->GetBitmap().SetBitmap(dc, nIDResource, TEXT("PNG"));
	::ReleaseDC(m_hWnd, dc);
	Invalidate();
	UpdateWindow();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnLButtonUp
	Access:    	virtual public 
	Parameter: 	UINT _nFlags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::OnLButtonUp(UINT _nFlags, CPoint _Point)
{
	CDialog::OnLButtonUp(_nFlags, _Point);
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnInitRedrawArea
	Access:    	virtual public 
	Parameter: 	const bool _bShowDate
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::OnInitRedrawArea(const bool _bShowDate /*= true*/)
{
    GetBasicDisplay()->OnInitRedrawArea(this);
	GetBasicDisplay()->OnPaint(_bShowDate);
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::GetGrid
	Access:    	public 
	Returns:   	CGrid&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CGrid& CBitmapDialog::GetGrid()
{
	return GetBasicDisplay()->GetGrid();
}

/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::GetMemDC
	Access:    	public 
	Returns:   	COffScreenBuffer*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
COffScreenBuffer* CBitmapDialog::GetMemDC()
{
	return GetBasicDisplay()->GetOffScreenBuffer();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::GetBasicDisplay
	Access:    	public 
	Returns:   	CBasicDisplay*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CBasicDisplay* CBitmapDialog::GetBasicDisplay()
{
	EXT_ASSERT_POINTER(m_BasicDisplay, CBasicDisplay);
	return m_BasicDisplay;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::GetTextRegion
	Access:    	public 
	Parameter: 	const int _TextRegionID
	Returns:   	CTextRegion*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTextRegion* CBitmapDialog::GetTextRegion(const int _TextRegionID)
{
	return GetBasicDisplay()->GetTextRegion(_TextRegionID);
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::LoadMenu
	Access:    	public 
	Parameter: 	int _MenuBarID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::LoadMenu(int _MenuBarID)
{
	if (!m_wndCommandBar.Create(this) || !m_wndCommandBar.InsertMenuBar(_MenuBarID))
	{
		TRACE0("Failed to create CommandBar\n");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::ReplaceMenu
	Access:    	public 
	Parameter: 	int _MenuBarID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::ReplaceMenu(int _MenuBarID)
{
	if (!m_wndCommandBar.InsertMenuBar(_MenuBarID))
	{
		TRACE0("Failed to update CommandBar\n");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnSize
	Access:    	virtual public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::OnSize(UINT nType, int cx, int cy) 
{
	ModifyStyle(WS_HSCROLL | WS_VSCROLL, null);
	GetBasicDisplay()->OnSize(this);
	CDialog::OnSize(nType, cx, cy); 
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapDialog::OnVScroll
	Access:    	public 
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Parameter: 	CScrollBar * _ScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapDialog::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar)
{
	GetGrid().OnVScroll(_SBCode, _Pos, _ScrollBar);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
