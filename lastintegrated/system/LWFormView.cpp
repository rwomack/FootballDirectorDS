// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1999 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "LWFormView.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

//stolen from afxpriv.h
#define ASSERT_VALID_IDR(nIDR) ASSERT((nIDR) != 0 && (nIDR) < 0x8000)

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CLWFormView, CView)
	//{{AFX_MSG_MAP(CLWFormView)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
	Method:   	CLWFormView::CLWFormView
	Access:    	public 
	Parameter: 	LPCTSTR lpszTemplateName
	Returns:   
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLWFormView::CLWFormView(LPCTSTR lpszTemplateName)
{
	m_lpszTemplateName = lpszTemplateName;
	m_pCreateContext = null;
	m_hWndFocus = null;     // focus window is unknown
}


/*------------------------------------------------------------------------------
	Method:   	CLWFormView::CLWFormView
	Access:    	public 
	Parameter: 	UINT nIDTemplate
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLWFormView::CLWFormView(UINT nIDTemplate)
{
	m_lpszTemplateName = MAKEINTRESOURCE(nIDTemplate);
	m_pCreateContext = null;
	m_hWndFocus = null;     // focus window is unknown
}


/*------------------------------------------------------------------------------
	Method:   	CLWFormView::Create
	Access:    	public 
	Parameter: 	LPCTSTR 
	Parameter: 	LPCTSTR 
	Parameter: 	DWORD dwRequestedStyle
	Parameter: 	const RECT& rect
	Parameter: 	CWnd* pParentWnd
	Parameter: 	UINT nID
	Parameter: 	CCreateContext* pContext
	Returns:   	BOOL
	Qualifier: 	
	Purpose:	Virtual override of CWnd::Create
------------------------------------------------------------------------------*/
BOOL CLWFormView::Create(LPCTSTR /*lpszClassName*/, LPCTSTR /*lpszWindowName*/,
	DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
	CCreateContext* pContext)
{
	ASSERT(pParentWnd != null);
	ASSERT(m_lpszTemplateName != null);

	m_pCreateContext = pContext;    // save state for later OnCreate

#ifdef _DEBUG
	// dialog template must exist and be invisible with WS_CHILD set
	/*if (!_AfxCheckDialogTemplate(m_lpszTemplateName, TRUE))
	{
		ASSERT(FALSE);          // invalid dialog template name
		PostNcDestroy();        // cleanup if Create fails too soon
		return FALSE;
	}*/
#endif //_DEBUG

	// call PreCreateWindow to get prefered extended style
	CREATESTRUCT cs; memset(&cs, 0, sizeof(CREATESTRUCT));
	if (dwRequestedStyle == 0)
		dwRequestedStyle = AFX_WS_DEFAULT_VIEW;
	cs.style = dwRequestedStyle;
	if (!PreCreateWindow(cs))
		return FALSE;

	// create a modeless dialog
	if (!CreateDlg(m_lpszTemplateName, pParentWnd))
		return FALSE;

	m_pCreateContext = null;

	// we use the style from the template - but make sure that
	//  the WS_BORDER bit is correct
	// the WS_BORDER bit will be whatever is in dwRequestedStyle
	ModifyStyle(WS_BORDER|WS_CAPTION, cs.style & (WS_BORDER|WS_CAPTION));
	ModifyStyleEx(WS_EX_CLIENTEDGE, cs.dwExStyle & WS_EX_CLIENTEDGE);

	SetDlgCtrlID(nID);

	CRect rectTemplate;
	GetWindowRect(rectTemplate);

	// initialize controls etc
	if (!ExecuteDlgInit(m_lpszTemplateName))
		return FALSE;

	// force the size requested
	SetWindowPos(null, rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		SWP_NOZORDER|SWP_NOACTIVATE);

	// make visible if requested
	if (dwRequestedStyle & WS_VISIBLE)
		ShowWindow(SW_SHOWNORMAL);

	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CLWFormView::OnInitialUpdate
	Access:    	virtual public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLWFormView::OnInitialUpdate()
{
	ASSERT_VALID(this);

	if (!UpdateData(FALSE))
		TRACE0("UpdateData failed during formview initial update.\n");

	CView::OnInitialUpdate();
}


/*------------------------------------------------------------------------------
	Method:   	CLWFormView::OnCreate
	Access:    	protected 
	Parameter: 	LPCREATESTRUCT lpcs
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CLWFormView::OnCreate(LPCREATESTRUCT lpcs)
{
	// since we can't get the create context parameter passed in
	//  through CreateDialog, we use a temporary member variable
	lpcs->lpCreateParams = (LPVOID)m_pCreateContext;
	return CView::OnCreate(lpcs);
}


/*------------------------------------------------------------------------------
	Method:   	CLWFormView::OnActivateView
	Access:    	public 
	Parameter: 	BOOL bActivate
	Parameter: 	CView* pActivateView
	Parameter: 	CView* pDeactiveView
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLWFormView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if (SaveFocusControl())
		return;     // don't call base class when focus is already set

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


/*------------------------------------------------------------------------------
	Method:   	CLWFormView::OnActivateFrame
	Access:    	virtual protected 
	Parameter: 	UINT nState
	Parameter: 	CFrameWnd* 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLWFormView::OnActivateFrame(UINT nState, CFrameWnd* /*pFrameWnd*/)
{
	if (nState == WA_INACTIVE)
		SaveFocusControl();     // save focus when frame loses activation
}


/*------------------------------------------------------------------------------
	Method:   	CLWFormView::SaveFocusControl
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CLWFormView::SaveFocusControl()
{
	// save focus window if focus is on this window's controls
	HWND hWndFocus = ::GetFocus();
	if (hWndFocus != null && ::IsChild(m_hWnd, hWndFocus))
	{
		m_hWndFocus = hWndFocus;
		return TRUE;
	}
	return FALSE;
}


/*------------------------------------------------------------------------------
	Method:   	CLWFormView::OnSetFocus
	Access:    	public 
	Parameter: 	CWnd*
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLWFormView::OnSetFocus(CWnd*)
{
	if (!::IsWindow(m_hWndFocus) || !::IsChild(m_hWnd, m_hWndFocus))
	{
		// invalid or unknown focus window... let windows handle it
		m_hWndFocus = null;
		Default();
		return;
	}
	// otherwise, set focus to the last known focus window
	::SetFocus(m_hWndFocus);
}


/*------------------------------------------------------------------------------
	Method:   	CLWFormView::PreTranslateMessage
	Access:    	virtual protected 
	Parameter: 	MSG* pMsg
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CLWFormView::PreTranslateMessage(MSG* pMsg)
{
	ASSERT(pMsg != null);
	ASSERT_VALID(this);
	ASSERT(m_hWnd != null);

	// allow tooltip messages to be filtered
	if (CView::PreTranslateMessage(pMsg))
		return TRUE;

	// don't translate dialog messages when in Shift+F1 help mode
	CFrameWnd* pFrameWnd = GetTopLevelFrame();

	// since 'IsDialogMessage' will eat frame window accelerators,
	//   we call all frame windows' PreTranslateMessage first
	pFrameWnd = GetParentFrame();   // start with first parent frame
	while (pFrameWnd != null)
	{
		// allow owner & frames to translate before IsDialogMessage does
		if (pFrameWnd->PreTranslateMessage(pMsg))
			return TRUE;

		// try parent frames until there are no parent frames
		pFrameWnd = pFrameWnd->GetParentFrame();
	}

	// don't call IsDialogMessage if form is empty
	if (::GetWindow(m_hWnd, GW_CHILD) == null)
		return FALSE;

#if defined(_WIN32_WCE) 
	// WinCE's ::IsDialogMessage contains a check for Alt-H (shortcut for help) and 
	// processes them.  However, it's not appropriate for the modeless dialog used 
	// by CLWFormView, because it prevents the Help menu from coming up.  This workaround
	// traps Alt-H before ::IsDialogMessage gets it and returns 0, meaning "not processed."
	if( (pMsg->message == WM_SYSCHAR) && 
		((WCHAR)(pMsg->wParam) == (WCHAR)('h') || (WCHAR)(pMsg->wParam) == (WCHAR)('H')) )
		return 0;
#endif // _WIN32_WCE
	// filter both messages to dialog and from children
	return PreTranslateInput(pMsg);
}


/*------------------------------------------------------------------------------
	Method:   	CLWFormView::OnDraw
	Access:    	virtual protected 
	Parameter: 	CDC* pDC
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLWFormView::OnDraw(CDC* pDC)
{
	ASSERT_VALID(this);


	UNUSED(pDC);     // unused in release build
}


//////////////////////////////////////////////////////////////////////////
// CLWFormView diagnostics

#ifdef _DEBUGv3
void CLWFormView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);

	dc << "m_lpszTemplateName = ";
	if (HIWORD(m_lpszTemplateName) == 0)
		dc << (int)LOWORD((DWORD)m_lpszTemplateName);
	else
		dc << m_lpszTemplateName;

	dc << "\n";
}

void CLWFormView::AssertValid() const
{
	CView::AssertValid();
}
#endif


IMPLEMENT_DYNAMIC(CLWFormView, CView)

//////////////////////////////////////////////////////////////////////////
