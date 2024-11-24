// Screen42.cpp : implementation file
//

#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "MessageDisplay.h"
#include "Screen42.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen42 dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen42::CScreen42
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen42::CScreen42(CWnd* pParent /*=null*/)
	: CBitmapDialog(CScreen42::IDD, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen42::~CScreen42
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen42::~CScreen42()
{
}


BEGIN_MESSAGE_MAP(CScreen42, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_CONTINUE, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen42 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen42::DoClearRegistry
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen42::DoClearRegistry()
{
	theApp.GetApplicationPreferences().WriteRegistrationCode(0);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen42::OnVScroll
	Access:    	protected 
	Parameter: 	UINT nSBCode
	Parameter: 	UINT nPos
	Parameter: 	CScrollBar * pScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen42::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen42::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen42::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	m_MessageDisplay.OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen42::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen42::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	m_MessageDisplay.OnPaint(CBitmapDialog::GetMemDC());
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen42::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen42::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_SCREEN42, IDS_SCREEN42, true);
	CBitmapDialog::LoadMenu(IDR_MENUCONTINUE);

    // Setup message display
    m_MessageDisplay.SetDrawInfo(this, 										// parent
								 0x1000,									// scrollbar ID
								 CRect(4, 42, 236, 266),					// rectangle
								 NAVY);

//				CString wstrFileName;
//				GetModuleFileName(null, wstrFileName.GetBufferSetLength(256), 256);
//				wstrFileName.ReleaseBuffer();
//				wchar_t szVersion[256];
//				BOOL bRet = GetFileVersionInfo(wstrFileName.GetBuffer(80), 0, 256, szVersion);
//				UINT nVerSize;
//				VS_FIXEDFILEINFO* vsFileInfo;
//				LPVOID pVoid = (LPVOID)&vsFileInfo;
//				int version = VerQueryValue(szVersion, _T("\\FileVersion"), &pVoid, &nVerSize);

	CString strTest;
	strTest = TEXT("Football Director 08 Build 0073\n");
	m_MessageDisplay.AddString(strTest, NAVY);
	strTest = TEXT("Version 0.21\n");
	m_MessageDisplay.AddString(strTest, NAVY);
	strTest = TEXT("(c) Sports Director Ltd 2001-8\n");
	m_MessageDisplay.AddString(strTest);

	strTest = TEXT("Design and Programming");
	m_MessageDisplay.AddString(strTest);
	strTest = TEXT("Roger Womack");
	m_MessageDisplay.AddString(strTest);
	strTest = TEXT("\n");
	m_MessageDisplay.AddString(strTest);

	strTest = TEXT("Artwork");
	m_MessageDisplay.AddString(strTest);
	strTest = TEXT("Rob James");
	m_MessageDisplay.AddString(strTest);
	strTest = TEXT("\n");
	m_MessageDisplay.AddString(strTest);

	strTest = TEXT("Published By Astraware");
	m_MessageDisplay.AddString(strTest);
	strTest = TEXT("\n");
	m_MessageDisplay.AddString(strTest);

// 	strTest = TEXT("Executive Producer");
// 	m_MessageDisplay.AddString(strTest);
// 	strTest = TEXT("\n");
// 	m_MessageDisplay.AddString(strTest);
// 	strTest = TEXT("\n");
// 	m_MessageDisplay.AddString(strTest);

	strTest = TEXT("Producer\n");
	m_MessageDisplay.AddString(strTest);
	strTest = TEXT("Kieran Smith\n");
	m_MessageDisplay.AddString(strTest);

	strTest = TEXT("Music");
	m_MessageDisplay.AddString(strTest);
	strTest = TEXT("Brad Derrick");
	m_MessageDisplay.AddString(strTest);
	strTest = TEXT("\n");
	m_MessageDisplay.AddString(strTest);

	/*strTest = TEXT("Marketing And PR");
	m_MessageDisplay.AddString(strTest);
	strTest = TEXT("\n");
	m_MessageDisplay.AddString(strTest);
	strTest = TEXT("\n");
	m_MessageDisplay.AddString(strTest);*/

	strTest = TEXT("Special Thanks");
	m_MessageDisplay.AddString(strTest);
// 	strTest = TEXT("Herbert Buchanan");
// 	m_MessageDisplay.AddString(strTest);
	
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen42::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen42::OnSize(UINT nType, int cx, int cy)
{
	CBitmapDialog::OnSize(nType, cx, cy);

	if (IsInitialised() == true)
	{
		CRect theRect;
		GetClientRect(&theRect);
		theRect.top = TOPY;
		theRect.bottom -= 2;
		theRect.left += 2;
		theRect.right -= 2;
		m_MessageDisplay.MoveWindow(theRect);
	}
	Sleep(1);
}