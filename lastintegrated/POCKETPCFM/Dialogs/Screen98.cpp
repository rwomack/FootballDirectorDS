
/*

	Screen98.cpp
	
	Manager Name

	Football Director

	(c) Rw Software 1994 - 2008

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "ExtEdit.h"
#include "Screen99.h"
#include "Screen98.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*------------------------------------------------------------------------------
	Method:   	CScreen98::CScreen98
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen98::CScreen98(CWnd* pParent /*=null*/)
	: CBitmapDialog(CScreen98::IDD, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen98::~CScreen98
	Access:    	public 
	Returns:
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen98::~CScreen98()
{
}


BEGIN_MESSAGE_MAP(CScreen98, CBitmapDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()

	ON_COMMAND(ID_CONTINUE, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen98 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen98::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen98::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case VK_F23:
		case VK_RETURN:
			OnOK();
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen98::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen98::OnInitDialog()
{
	BOOL bRet = CBitmapDialog::OnInitDialog(IDS_HELPSCREEN98, IDS_SCREEN98);
	m_editName.SubclassDlgItem(IDC_EDIT9801, this);
	m_editName.SetWindowText(theApp.GetApplicationPreferences().GetManagerName());
	CBitmapDialog::LoadMenu(IDR_MENUCONTINUE);
	SetIsInitialised(true);
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen98::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT _nFlags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen98::OnLButtonDown(UINT _nFlags, CPoint _Point)
{
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen98::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen98::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	COffScreenBuffer& memDC = *CBitmapDialog::GetMemDC();

	CRect rectMess(memDC.GetBufferWidth() / 2 - 100, m_EditTopY - 30, memDC.GetBufferWidth() / 2 + 100, m_EditTopY - 10);
	memDC.DisplayText(IDS_ENTERNAME, &rectMess, BLACK, 20, true, FW_SEMIBOLD);

	CBitmapDialog::OnPaint(false, true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen98::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT _nFlags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen98::OnLButtonUp(UINT _nFlags, CPoint _Point)
{
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen98::OnOK
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen98::OnOK()
{
	WorldData().SetCurrentUserID(0);
	CString strName;
	m_editName.GetWindowText(strName);
	if (strName.IsEmpty() == FALSE)
	{
		theApp.GetApplicationPreferences().SetManagerName(strName);
		CString strForeName = _T("@");
		WorldData().GetCurrentUser()->SetFullName(strForeName, strName);
		CDialog::OnOK();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen98::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen98::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		CRect theRect;
		GetClientRect(&theRect);
		GetClientRect(&theRect);
		theRect.top = 42 + theRect.bottom / 3 - theRect.top;
		m_EditTopY = theRect.top;
		theRect.bottom = theRect.top + 20;
		theRect.left = theRect.right / 2 - 100;
		theRect.right = theRect.left + 200;
		GetDlgItem(IDC_EDIT9801)->MoveWindow(&theRect);
	}

	Sleep(1);
}