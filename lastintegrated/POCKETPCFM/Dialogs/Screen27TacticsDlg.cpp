// Screen27.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen27TacticsDlg dialog



/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsDlg::CScreen27TacticsDlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen27TacticsDlg::CScreen27TacticsDlg(CWnd* pParent /*=null*/) 
	: CInMatchActionBitmapDialog(CScreen27TacticsDlg::IDD, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsDlg::~CScreen27TacticsDlg
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen27TacticsDlg::~CScreen27TacticsDlg()
{
}


BEGIN_MESSAGE_MAP(CScreen27TacticsDlg, CInMatchActionBitmapDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK2701, OnButtonCounterAttack)
	ON_BN_CLICKED(IDC_CHECK2702, OnButtonOffsideTrap)

	ON_COMMAND(ID_ACTION1, OnAction1Clicked)
	ON_COMMAND(ID_ACTION2, OnAction2Clicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen27 message handlers
/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsDlg::OnActionClicked
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsDlg::OnActionClicked()
{
//	CBitmapDialog::OnActionMenu(IDR_SCREEN27TACTICVIEWMENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT2);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsDlg::OnAction1Clicked
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsDlg::OnAction1Clicked()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK2701);
	EXT_ASSERT(pButton);
	if (pButton->GetCheck() == BST_CHECKED)
	{
		pButton->SetCheck(BST_UNCHECKED);
		WorldData().GetCurrentUser()->SetCounterAttack(false);
	}
	else
	{
		pButton->SetCheck(BST_CHECKED);
		WorldData().GetCurrentUser()->SetCounterAttack(true);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsDlg::OnAction2Clicked
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsDlg::OnAction2Clicked()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK2702);
	EXT_ASSERT(pButton);
	if (pButton->GetCheck() == BST_UNCHECKED)
	{
		pButton->SetCheck(BST_CHECKED);
		WorldData().GetCurrentUser()->SetOffsideTrap(true);
	}
	else
	{
		pButton->SetCheck(BST_UNCHECKED);
		WorldData().GetCurrentUser()->SetOffsideTrap(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsDlg::OnButtonCounterAttack
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsDlg::OnButtonCounterAttack()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK2701);
	EXT_ASSERT(pButton);
	if (pButton->GetCheck() == BST_CHECKED)
	{
		WorldData().GetCurrentUser()->SetCounterAttack(true);
	}
	else
	{
		WorldData().GetCurrentUser()->SetCounterAttack(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsDlg::OnButtonOffsideTrap
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsDlg::OnButtonOffsideTrap()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK2702);
	EXT_ASSERT(pButton);
	if (pButton->GetCheck() == BST_CHECKED)
	{
		WorldData().GetCurrentUser()->SetOffsideTrap(true);
	}
	else
	{
		WorldData().GetCurrentUser()->SetOffsideTrap(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsDlg::OnPaint
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsDlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	m_TacticsData.OnPaint();
 	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsDlg::OnInitDialog
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen27TacticsDlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN27T, IDS_SCREEN27TACTICS);
	CBitmapDialog::LoadMenu(IDR_SCREEN27TACTICMENU);
	m_TacticsData.OnInitialUpdate(this, CBitmapDialog::GetBasicDisplay());
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsDlg::OnLButtonDown
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsDlg::OnLButtonUp
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	int ButtonPressed = CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	m_TacticsData.OnLButtonUp(ButtonPressed, nFlags, point);
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsDlg::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	m_TacticsData.OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_RETURN:
		case VK_SELECT:
			break;
	}
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CWnd::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
	m_TacticsData.OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27TacticsDlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27TacticsDlg::OnSize(UINT nType, int cx, int cy)
{
	CBitmapDialog::OnSize(nType, cx, cy);
	if (CBitmapDialog::IsInitialised() == true)
	{
		m_TacticsData.OnSize();
	}
	Sleep(1);
}
