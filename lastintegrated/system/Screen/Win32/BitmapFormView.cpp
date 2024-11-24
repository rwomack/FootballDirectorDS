
/*

	BitmapFormView.cpp : implementation file

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "BitmapFormView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CBitmapFormView, CFormView)
	ON_WM_ERASEBKGND()
	ON_WM_GETDLGCODE()
	ON_WM_HELPINFO()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()

	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::CBitmapFormView
Access:    	public 
Parameter: 	UINT nIDTemplate
Returns:   	
Qualifier: 
Purpose:
------------------------------------------------------------------------------*/
CBitmapFormView::CBitmapFormView(UINT nIDTemplate) 
	: CFormView(nIDTemplate)
	, m_BasicDisplay(NULL)
	, m_IsInitialised(false)
{
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::DoInitFullScreen
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::DoInitFullScreen()
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
	SHFullScreen(m_hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON);

	// Resize the window over the taskbar area.
	RECT rect;
	GetWindowRect(&rect);
	rect.top = 0/*-= MENU_HEIGHT*/;
	MoveWindow(&rect, TRUE);
	GetBasicDisplay()->SetFullScreen();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::Isinitialised
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CBitmapFormView::IsInitialised()
{
	return m_IsInitialised;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::SetIsInitialised
	Access:    	public 
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::SetIsInitialised(bool _Value)
{
	m_IsInitialised = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::GetCentreX
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBitmapFormView::GetCentreX()
{
	CRect rectWnd;
	GetClientRect(&rectWnd);
	return rectWnd.CenterPoint().x;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::GetWidth
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBitmapFormView::GetWidth()
{
	CRect rectWnd;
	GetClientRect(&rectWnd);
	return rectWnd.right;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::GetHeight
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBitmapFormView::GetHeight()
{
	CRect rectWnd;
	GetClientRect(&rectWnd);
	return rectWnd.bottom;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::GetCentreY
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBitmapFormView::GetCentreY()
{
	CRect rectWnd;
	GetClientRect(&rectWnd);
	return rectWnd.CenterPoint().y;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::GetWindowHeight
	Access:    	protected 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBitmapFormView::GetWindowHeight()
{
	CRect rectWnd;
	GetClientRect(&rectWnd);
	return rectWnd.bottom;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::OnActionClicked
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::OnActionClicked()
{
	OnActionMenu(IDR_BITMAPFORMMENU, GetBasicDisplay()->GetHeight() + 26 - 37);
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::OnActionMenu
	Access:    	public 
	Parameter: 	int _Menu
	Parameter: 	int _yPos
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::OnActionMenu(int _Menu, int _yPos, int _CommandToRemove /*= 0*/)
{
	CMenu menu;
	VERIFY(menu.LoadMenu(_Menu));
	CMenu* pPopup = menu.GetSubMenu(0);
	EXT_ASSERT_POINTER(pPopup, CMenu);
	if (_CommandToRemove > 0)
	{
		pPopup->RemoveMenu(_CommandToRemove, MF_BYCOMMAND);
	}
	pPopup->TrackPopupMenu(TPM_LEFTALIGN, 10, _yPos, AfxGetMainWnd());
	AfxGetApp()->PumpMessage();
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::OnLButtonDown
Access:    	protected 
Parameter: 	UINT nFlags
Parameter: 	CPoint point
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::OnLButtonDown(UINT nFlags, CPoint point)
{
	Default();
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::OnHelpInfo
Access:    	public 
Parameter: 	HELPINFO * _HelpInfo
Returns:   	BOOL
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
BOOL CBitmapFormView::OnHelpInfo(HELPINFO* _HelpInfo)
{
	return GetBasicDisplay()->OnHelpInfo(_HelpInfo);
}

/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::OnEraseBkgnd
Access:    	public 
Parameter: 	CDC * pDC
Returns:   	BOOL
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
BOOL CBitmapFormView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::DoLoadBitmap
Access:    	public 
Parameter: 	const UINT nIDResource
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::DoLoadBitmap(const UINT nIDResource)
{
	HDC dc = ::GetDC(m_hWnd);
	GetBasicDisplay()->GetBitmap().SetBitmap(dc, nIDResource, TEXT("PNG"));
	::ReleaseDC(m_hWnd, dc);
	Invalidate();
	UpdateWindow();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::OnInitialUpdate
	Access:    	public 
	Parameter: 	const CString _strHelpTopic
	Parameter: 	CBasicDisplay * _BasicDisplay
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::OnInitialUpdate(const CString _strHelpTopic, CBasicDisplay* _BasicDisplay, int _StrID /*= 0*/)
{
	CFormView::OnInitialUpdate();

	EXT_ASSERT_POINTER(_BasicDisplay, CBasicDisplay);
	m_BasicDisplay = _BasicDisplay;
	GetBasicDisplay()->OnInitDialog(this, _strHelpTopic);
	GetBasicDisplay()->SetDisplayHeader(true, _StrID);

	GetBasicDisplay()->DoRemoveButtonList();
	GetBasicDisplay()->DoRemoveTextRegionList();

	ModifyStyle(WS_HSCROLL | WS_VSCROLL, null);
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::GetBasicDisplay
Access:    	public 
Returns:   	CBasicDisplay&
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CBasicDisplay* CBitmapFormView::GetBasicDisplay()
{
	EXT_ASSERT_POINTER(m_BasicDisplay, CBasicDisplay);
	return m_BasicDisplay;
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::ReDraw
Access:    	public 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::ReDraw()
{
	Invalidate();
	UpdateWindow();
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::OnInitRedrawArea
Access:    	public 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::OnInitRedrawArea()
{
	GetBasicDisplay()->OnInitRedrawArea(this);
	GetBasicDisplay()->OnPaint(true);
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::GetMemDC
Access:    	public 
Returns:   	COffScreenBuffer*
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
COffScreenBuffer* CBitmapFormView::GetMemDC()
{
	return GetBasicDisplay()->GetOffScreenBuffer();
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::OnGetDlgCode
Access:    	public 
Returns:   	uint
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
uint CBitmapFormView::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::OnSize
	Access:    	virtual public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::OnSize(UINT nType, int cx, int cy) 
{
	if (IsInitialised() == true)
	{
		CRect theRect;
		GetClientRect(&theRect);
		GetBasicDisplay()->OnSize(this);
	}
	CFormView::OnSize(nType, cx, cy);
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::OnSetFocus
Access:    	protected 
Parameter: 	CWnd * _OldWnd
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::OnSetFocus(CWnd* _OldWnd)
{
//	GetBasicDisplay()->OnInitDialog(this, _T(""), false, BLACK);
	CFormView::OnSetFocus(_OldWnd);
}
