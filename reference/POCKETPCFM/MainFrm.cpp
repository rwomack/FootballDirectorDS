
/*

	MainFrm.cpp : implementation of the CMainFrame class

	(c) Rw Software 1994 - 2008

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/


#include "stdafx.h"

#include "MainFrm.h"
#include "UI.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const DWORD dwAdornmentFlags = 0; // exit button


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_HELPINFO()
	ON_WM_CREATE()
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CMainFrame::OnHelpInfo
	Access:    	public 
	Parameter: 	HELPINFO * pHelpInfo
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CMainFrame::OnHelpInfo(HELPINFO* pHelpInfo)
{
	CView* pOldActiveView = GetActiveView();
	return pOldActiveView->PostMessage(WM_HELP, 0, 0);
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction


/*------------------------------------------------------------------------------
	Method:   	CMainFrame::CMainFrame
	Access:    	private 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMainFrame::CMainFrame()
{
}


/*------------------------------------------------------------------------------
	Method:   	CMainFrame::~CMainFrame
	Access:    	virtual private 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMainFrame::~CMainFrame()
{
}


/*------------------------------------------------------------------------------
	Method:   	CMainFrame::OnCreate
	Access:    	private 
	Parameter: 	LPCREATESTRUCT lpCreateStruct
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndCommandBar.Create(this) ||
		!m_wndCommandBar.InsertMenuBar(IDR_MAINFRAME) ||
		!m_wndCommandBar.AddAdornments(dwAdornmentFlags))
	{
		return -1;      // fail to create
	}

	m_wndCommandBar.SetBarStyle(m_wndCommandBar.GetBarStyle() | CBRS_SIZE_FIXED);
	return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CMainFrame::PreCreateWindow
	Access:    	public 
	Parameter: 	CREATESTRUCT & cs
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}
