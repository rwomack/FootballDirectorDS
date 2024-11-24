// Screen35.cpp : implementation file
//

#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "MessageDisplay.h"
#include "PlayerNameTextRegion.h"
#include "Screen35.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen35Dlg dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen35Dlg::CScreen35Dlg
	Access:    	public 
	Parameter: 	CPlayer * pPlayer
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen35Dlg::CScreen35Dlg(CPlayer* pPlayer, CWnd* pParent /*=null*/)
	: CBitmapDialog(CScreen35Dlg::IDD, pParent)
{
	EXT_ASSERT_POINTER(pPlayer, CPlayer);
	m_pCurrentPlayer = pPlayer;
}


BEGIN_MESSAGE_MAP(CScreen35Dlg, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_CONTINUE, OnOK)
	ON_COMMAND(ID_ACTION_CANCEL, OnCancel)
	ON_COMMAND(ID_ACTION_CONFIRM, OnConfirm)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen35 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen35Dlg::OnOK
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen35Dlg::OnOK()
{
	CDialog::OnOK();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen35Dlg::OnConfirm
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen35Dlg::OnConfirm()
{
	// Sack/Release the player
	m_pCurrentPlayer->GetClub().SackPlayer(m_pCurrentPlayer);
	CBitmapDialog::ReplaceMenu(IDR_MENUCONTINUE);
}



/*------------------------------------------------------------------------------
	Method:   	CScreen35Dlg::OnCancel
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen35Dlg::OnCancel()
{
	CDialog::OnCancel();
}

/*------------------------------------------------------------------------------
	Method:   	CScreen35Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen35Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	m_MessageDisplay.OnPaint(CBitmapDialog::GetMemDC());
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen35Dlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen35Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN35, IDS_SCREEN35);

	m_PlayerName.DoInitialise(TOPY, this, GetBasicDisplay(), *m_pCurrentPlayer);

	CString str;
	uint ContractValue = m_pCurrentPlayer->ContractValue();
	int MenuBar = IDR_SCREEN35MENU;
/*	if (m_pCurrentPlayer->AllowSack() == false)
	{
		MenuBar = IDR_MENUCONTINUE;
		str.Format(IDS_CANTSACKPLAYER, m_pCurrentPlayer->GetName());
	}
	else if (m_pCurrentPlayer->GetClub().GetCashBalance() < static_cast<int>(ContractValue))
	{
		MenuBar = IDR_MENUCONTINUE;
		str.Format(IDS_NOTENOUGHCASHTOSACKPLAYER, m_pCurrentPlayer->GetName(), m_pCurrentPlayer->ContractValueStr(m_pCurrentPlayer->GetClub().GetCountry()));
	}
	else*/
	{
		str.Format(IDS_SACKPLAYER, m_pCurrentPlayer->GetName(), m_pCurrentPlayer->ContractValueStr(m_pCurrentPlayer->GetClub().GetCountry()));
	}
	CBitmapDialog::LoadMenu(MenuBar);
	// Setup message display
	m_MessageDisplay.SetDrawInfo(this, 0x1000, CRect(2, 84, 240, 242));						
	m_MessageDisplay.AddString(str);
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen35Dlg::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen35Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	m_MessageDisplay.OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen35Dlg::OnVScroll
	Access:    	protected 
	Parameter: 	UINT nSBCode
	Parameter: 	UINT nPos
	Parameter: 	CScrollBar * pScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen35Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_MessageDisplay.OnVScroll(nSBCode, nPos, pScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen35Dlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen35Dlg::OnSize(UINT nType, int cx, int cy)
{

	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		m_PlayerName.MoveWindow();
		CRect theWindow;
		GetClientRect(&theWindow);
		m_MessageDisplay.MoveWindow(CRect(4, 70, theWindow.right - 4, theWindow.bottom - 4));
	}
	Sleep(1);
}