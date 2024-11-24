// Screen97.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "ExtStatic.h"
#include "IntEd.h"
#include "Screen97.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen97 dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen97::CScreen97
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 	: CBitmapDialog(IDD, pParent)
	Purpose:
------------------------------------------------------------------------------*/
CScreen97::CScreen97(CWnd* pParent /*=null*/)
	: CBitmapDialog(CScreen97::IDD, pParent)
{
}


BEGIN_MESSAGE_MAP(CScreen97, CBitmapDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen97 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen97::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen97::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(TEXT("Main Menu"), IDS_SCREEN97);

	GetBasicDisplay()->ButtonList().SetSize(eBUTTONS);
	GetBasicDisplay()->ButtonList().SetAt(eREGISTER, &m_buttonRegister);
	GetBasicDisplay()->ButtonList().SetAt(eCANCEL, &m_buttonCancel);

//PNG	m_buttonRegister.DoSetDrawInfo(IDR_PNGSCREEN97REGISTER, this);
//PNG	m_buttonCancel.DoSetDrawInfo(IDR_PNGCANCEL, this);

	m_editCode.SubclassDlgItem(IDC_EDIT9701, this);

	m_static01.OnCreate(IDC_STATIC9701, this, 16, BLACK, WHITE, TEXT(""));
	m_static02.OnCreate(IDC_STATIC9702, this, 16, BLACK, WHITE, TEXT(""));
	m_static03.OnCreate(IDC_STATIC9703, this, 16, BLACK, WHITE, TEXT(""));
	m_static04.OnCreate(IDC_STATIC9704, this, 16, BLACK, WHITE, TEXT(""));
	m_static05.OnCreate(IDC_STATIC9705, this, 16, BLACK, WHITE, TEXT(""));
	m_static06.OnCreate(IDC_STATIC9706, this, 16, BLACK, WHITE, TEXT(""));
	m_static07.OnCreate(IDC_STATIC9707, this, 16, RED, WHITE, theApp.GetSystemPreferences().GetUserName());
	
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen97::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen97::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetBasicDisplay()->OnLButtonDown(nFlags, point);
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen97::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen97::OnLButtonUp(UINT nFlags, CPoint point)
{
	// New game
	switch (GetBasicDisplay()->OnLButtonUp(nFlags, point))
	{
		case eREGISTER:
			DoRegister();
			break;
		case eCANCEL:
			DoCancel();
			break;
	}
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen97::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen97::OnPaint()
{
	CBitmapDialog::OnPaint(false, true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen97::DoRegister
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen97::DoRegister()
{
#ifdef POCKETPC
	theApp.GetApplicationPreferences().WriteRegistrationCode(m_editCode.GetValue());
	if (theApp.IsRegistered(false) == true)
	{
		AfxMessageBox(IDS_REGISTRATIONSUCCESSFUL);
		SHSipPreference(CWnd::GetSafeHwnd(), SIP_FORCEDOWN);
		AfxGetApp()->PumpMessage();
		CDialog::OnOK();
	}
	else
	{
		if (AfxMessageBox(IDS_REGISTRATIONFAILED, MB_YESNO + MB_ICONWARNING) == IDNO)
		{
			SHSipPreference(CWnd::GetSafeHwnd(), SIP_FORCEDOWN);
			AfxGetApp()->PumpMessage();
			CDialog::OnOK();
		}
	}
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CScreen97::DoCancel
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen97::DoCancel()
{
	CDialog::OnCancel();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen97::OnOK
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen97::OnOK()
{
	DoRegister();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen97::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen97::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		m_buttonRegister.MoveWindow(CRect(BUTTON1X, BUTTON1Y, BUTTON1X + BUTTON1W, BUTTON1Y + BUTTON1H));
		m_buttonCancel.MoveWindow(CRect(BUTTON2X, BUTTON2Y, BUTTON2X + BUTTON2W, BUTTON2Y + BUTTON2H));
	}
	Sleep(1);
}