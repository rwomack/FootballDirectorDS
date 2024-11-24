// SplashScreen.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "SplashScreen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplashScreen dialog


/*------------------------------------------------------------------------------
	Method:   	CSplashScreen::CSplashScreen
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSplashScreen::CSplashScreen(CWnd* pParent /*=null*/)
	: CDialog(CSplashScreen::IDD, pParent)
{
	m_bModal = false;
	m_bKill = false;
	m_BitmapCoords = CPoint(0, 0 );
}


/*------------------------------------------------------------------------------
	Method:   	CSplashScreen::DoModal
	Access:    	public 
	Parameter: 	CPoint BitmapCoords
	Parameter: 	DWORD dwDisplayTime
	Parameter: 	DWORD dwResourceID
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CSplashScreen::DoModal(CPoint BitmapCoords, DWORD dwDisplayTime, DWORD dwResourceID)
{
	m_dwDisplayTime = dwDisplayTime;
	m_bModal = true;
	m_BitmapCoords = BitmapCoords;
	HDC dc = ::GetDC(m_hWnd);
	m_bmpLogo.SetBitmap(dc, dwResourceID, _T("PNG"));
	::ReleaseDC(m_hWnd,dc);
	return CDialog::DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CSplashScreen::TurnOff
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSplashScreen::TurnOff()				
{ 
	m_bKill = true; 
}


/*------------------------------------------------------------------------------
	Method:   	CSplashScreen::ShowSplashScreen
	Access:    	public 
	Parameter: 	CPoint BitmapCoords
	Parameter: 	DWORD dwDisplayTime
	Parameter: 	DWORD dwResourceID
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CSplashScreen::ShowSplashScreen(CPoint BitmapCoords, DWORD dwDisplayTime, DWORD dwResourceID)
{
	m_dwDisplayTime = dwDisplayTime;
	m_bKill = false;
	m_BitmapCoords = BitmapCoords;

	CWnd* pMainWnd = AfxGetMainWnd();
	ASSERT_POINTER(pMainWnd, CWnd);
	if (pMainWnd->IsIconic())
	{
		return FALSE;
	}
	if (!Create(CSplashScreen::IDD, pMainWnd))
	{
		return FALSE;
	}
	SetBitmap(dwResourceID);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CSplashScreen::SetBitmap
	Access:    	protected 
	Parameter: 	DWORD dwResourceID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSplashScreen::SetBitmap(DWORD dwResourceID)
{
	HDC dc = ::GetDC(m_hWnd);
	m_bmpLogo.SetBitmap(dc, dwResourceID, _T("PNG"));
	::ReleaseDC(m_hWnd, dc);
	ShowWindow(SW_SHOW);
	Invalidate();
	UpdateWindow();
}


BEGIN_MESSAGE_MAP(CSplashScreen, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplashScreen message handlers


/*------------------------------------------------------------------------------
	Method:   	CSplashScreen::OnEraseBkgnd
	Access:    	protected 
	Parameter: 	CDC * pDC
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CSplashScreen::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CSplashScreen::OnCreate
	Access:    	protected 
	Parameter: 	LPCREATESTRUCT lpCreateStruct
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CSplashScreen::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	SetTimer(ID_SPLASHSCREEN, m_dwDisplayTime, null);
	return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CSplashScreen::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CSplashScreen::OnInitDialog()
{
	CDialog::OnInitDialog();
#ifdef POCKETPCv3
	// Call SHInitDialog with flags for full screen.
	SHINITDLGINFO shidi;
	shidi.dwMask = SHIDIM_FLAGS;
	shidi.dwFlags = SHIDIF_FULLSCREENNOMENUBAR;
	shidi.hDlg = m_hWnd;
	SHInitDialog(&shidi);

	::CommandBar_Show(m_pWndEmptyCB->m_hWnd, FALSE);

   // SHFullScreen fails if dialog box is not foreground.
	SetForegroundWindow();
	SHFullScreen(m_hWnd, SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON | SHFS_HIDESTARTICON);

   // Resize the window over the taskbar area.
	#define MENU_HEIGHT 26
	RECT rect;
	GetWindowRect(&rect);
	rect.top -= MENU_HEIGHT;
	MoveWindow(&rect, TRUE);
	SHDoneButton(this->GetSafeHwnd(), SHDB_HIDE);
	SetTimer(ID_SPLASHSCREEN, m_dwDisplayTime, null);
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/*------------------------------------------------------------------------------
	Method:   	CSplashScreen::OnTimer
	Access:    	virtual protected 
	Parameter: 	UINT nIDEvent
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSplashScreen::OnTimer(UINT nIDEvent)
{
	if (m_bKill != true)
	{
		if (m_bModal == false)
		{
			DestroyWindow();
			CWnd* pMainWnd = AfxGetMainWnd();
			ASSERT_POINTER(pMainWnd, CWnd);
			pMainWnd->UpdateWindow();
		}
		else
		{
			OnOK();
		}
	}
	else
	{
		CDialog::OnTimer(nIDEvent);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSplashScreen::PreTranslateMessage
	Access:    	virtual public 
	Parameter: 	MSG * pMsg
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CSplashScreen::PreTranslateMessage(MSG* pMsg)
{
	if (m_bKill == true)
	{
		return FALSE;
	}
	if (pMsg->message == WM_KEYDOWN ||
		pMsg->message == WM_SYSKEYDOWN ||
		pMsg->message == WM_LBUTTONDOWN ||
		pMsg->message == WM_RBUTTONDOWN ||
		pMsg->message == WM_MBUTTONDOWN)
	{
		if (m_bModal == false)
		{
			DestroyWindow();
			CWnd* pMainWnd = AfxGetMainWnd();
			ASSERT_POINTER(pMainWnd, CWnd);
			pMainWnd->UpdateWindow();
		}
		else
		{
			OnOK();
		}
	}
		return CDialog::PreTranslateMessage(pMsg);
}


/*------------------------------------------------------------------------------
	Method:   	CSplashScreen::OnPaint
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSplashScreen::OnPaint()
{
	CPaintDC dc(this);
	m_bmpLogo.Draw(dc, m_BitmapCoords.x, m_BitmapCoords.y);
}
