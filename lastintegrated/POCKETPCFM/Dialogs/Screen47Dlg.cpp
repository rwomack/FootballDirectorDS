// Screen47Dlg.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "MessageDisplay.h"
#include "Screen47Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen47Dlg dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen47Dlg::CScreen47Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen47Dlg::CScreen47Dlg(CWnd* pParent /*=null*/)
	: CBitmapDialog(CScreen47Dlg::IDD, pParent)
{
}


BEGIN_MESSAGE_MAP(CScreen47WelcomeDlg, CScreen47SackedDlg)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CScreen47SackedDlg, CScreen47Dlg)
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CScreen47RenewDlg, CScreen47Dlg)
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()

	ON_COMMAND(ID_ACTION_ACCEPT, OnAccept)
	ON_COMMAND(ID_ACTION_REFUSE, OnRefuse)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CScreen47Dlg, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_CONTINUE, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen47Dlg message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen47RenewDlg::CScreen47RenewDlg
	Access:    	public 
	Parameter: 	CClub * _Club
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen47RenewDlg::CScreen47RenewDlg(CClub* _Club)
	: m_pClub(_Club)
	, CScreen47Dlg()
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen47RenewDlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen47RenewDlg::OnInitDialog()
{
	CScreen47Dlg::OnInitDialog();

	// Ask manager if he wishes to extend his contract
	CString str;
	str.Format(IDS_OFFERRENEWMANAGERCONTRACT, m_pClub->GetManagerName(), m_pClub->GetName());
	m_MessageDisplay.AddString(str, BLUE);
	CBitmapDialog::LoadMenu(IDR_ACCEPTREFUSEMENU);
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen47RenewDlg::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen47RenewDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	m_ButtonSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	ReDraw();
}



/*------------------------------------------------------------------------------
	Method:   	CScreen47RenewDlg::OnAccept
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:	Accept new contract
------------------------------------------------------------------------------*/
void CScreen47RenewDlg::OnAccept()
{
	m_pClub->GetManager().GetContractEndDate().IncreaseLength(5);
#ifdef TEST_DIALOGS
	OnEndNegotiations(_T("Hello World"));
#else
	OnEndNegotiations(m_pClub->DoHumanManagerWelcomeStr());
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CScreen47RenewDlg::OnRefuse
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:	Manager resigns
------------------------------------------------------------------------------*/
void CScreen47RenewDlg::OnRefuse()
{
	m_pClub->SetManagerID(NOMANAGER);
	CString strMessage;
    strMessage.Format(IDS_FAREWELLMESS, m_pClub->GetName());
	OnEndNegotiations(strMessage);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen47RenewDlg::OnEndNegotiations
	Access:    	protected 
	Parameter: 	CString strMessage
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen47RenewDlg::OnEndNegotiations(CString strMessage)
{
	m_MessageDisplay.DoAddBlankLine();
	m_MessageDisplay.AddString(strMessage, BLUE);
	m_MessageDisplay.ShowLastLine();
	DoShowContinueButton();
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen47RenewDlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen47RenewDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	CBitmapDialog::ReDraw();
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CScreen47SackedDlg::CScreen47SackedDlg
	Access:    	public 
	Parameter: 	CString strReason
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen47SackedDlg::CScreen47SackedDlg(CString strReason)
	: m_StrMessage(strReason)
	, CScreen47Dlg()
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen47SackedDlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen47SackedDlg::OnInitDialog()
{
	CScreen47Dlg::OnInitDialog();
	// Inform manager he is sacked
	m_MessageDisplay.AddString(m_StrMessage, RED);
	CBitmapDialog::LoadMenu(IDR_MENUCONTINUE);
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen47SackedDlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen47SackedDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	CBitmapDialog::ReDraw();
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CScreen47Dlg::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen47Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN47, IDS_SCREEN47);

    // Setup message display
    m_MessageDisplay.SetDrawInfo(this, 0x1000, CRect(2, 84, 240, 242));
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen47Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen47Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	m_MessageDisplay.OnPaint(CBitmapDialog::GetMemDC());
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen47Dlg::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen47Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	m_MessageDisplay.OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen47Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen47Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen47Dlg::OnVScroll
	Access:    	protected 
	Parameter: 	UINT nSBCode
	Parameter: 	UINT nPos
	Parameter: 	CScrollBar * pScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen47Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_MessageDisplay.OnVScroll(nSBCode, nPos, pScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen47Dlg::DoShowContinueButton
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen47Dlg::DoShowContinueButton()
{
	CBitmapDialog::ReplaceMenu(IDR_MENUCONTINUE);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen47Dlg::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen47Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		CRect theWindow;
		GetClientRect(&theWindow);
		m_MessageDisplay.MoveWindow(CRect(4, 50, theWindow.right - 4, theWindow.bottom - 4));
	}
	Sleep(1);
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CScreen47WelcomeDlg::CScreen47WelcomeDlg
	Access:    	public 
	Parameter: 	CString strReason
	Returns:   	
	Qualifier: 	: CScreen47SackedDlg(strReason)
	Purpose:
------------------------------------------------------------------------------*/
CScreen47WelcomeDlg::CScreen47WelcomeDlg(CString strReason)
	: CScreen47SackedDlg(strReason)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen47WelcomeDlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen47WelcomeDlg::OnInitDialog()
{
	CScreen47Dlg::OnInitDialog();
	CBitmapDialog::LoadMenu(IDR_ACCEPTREJECTCOUNTERMENU);
	m_MessageDisplay.AddString(CScreen47SackedDlg::m_StrMessage, BLUE);
	return TRUE;
}
