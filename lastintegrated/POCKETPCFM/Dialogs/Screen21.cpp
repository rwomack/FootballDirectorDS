/*

	Screen21.cpp

	Player Requests Transfer

	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "MessageDisplay.h"
#include "Dialog.h"
#include "PlayerNameTextRegion.h"
#include "Screen21.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen21 dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen21Dlg::CScreen21Dlg
	Access:    	public 
	Parameter: 	CPlayer * pPlayer
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen21Dlg::CScreen21Dlg(CPlayer* _Player, CWnd* pParent /*=null*/)
	: CBitmapDialog(CScreen21Dlg::IDD, pParent)
{
	EXT_ASSERT_POINTER(_Player, CPlayer);
	m_pCurrentPlayer = _Player;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen21Dlg::~CScreen21Dlg
	Access:    	public 
	Returns:   		Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen21Dlg::~CScreen21Dlg()
{
}


BEGIN_MESSAGE_MAP(CScreen21Dlg, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_ACTION_ACCEPT, OnRequestGranted)
	ON_COMMAND(ID_ACTION_REFUSE, OnRequestDenied)
	ON_COMMAND(ID_CONTINUE, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen21 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen21Dlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen21Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN21, IDS_SCREEN21);
	m_PlayerName.DoInitialise(TOPY, this, GetBasicDisplay(), *m_pCurrentPlayer);
    m_MessageDisplay.SetDrawInfo(this, 0x1000, CRect(2, 84, 240, 242));						

	// Ask manager if he wishes to sell player
	CString str;
	str.Format(IDS_PLAYERREQUESTTRANSFER, m_pCurrentPlayer->GetName());
	m_MessageDisplay.AddString(str);
	CBitmapDialog::LoadMenu(IDR_ACCEPTREFUSEMENU);
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen21Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen21Dlg::OnPaint()
{
	m_MessageDisplay.OnPaint(CBitmapDialog::GetMemDC());
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen21Dlg::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen21Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	m_MessageDisplay.OnKeyDown(nChar, nRepCnt, nFlags);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen21Dlg::OnVScroll
	Access:    	protected 
	Parameter: 	UINT nSBCode
	Parameter: 	UINT nPos
	Parameter: 	CScrollBar * pScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen21Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_MessageDisplay.OnVScroll(nSBCode, nPos, pScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen21Dlg::OnRequestGranted
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:	Grant Transfer request
------------------------------------------------------------------------------*/
void CScreen21Dlg::OnRequestGranted()
{
	m_pCurrentPlayer->OnTransferRequestGranted(WorldData().GetCurrentClub()->GetCountry());
	// transfer request granted
	CString strMessage;
	if (RandomNumber.IntLessThan(10) < 6)
	{
		strMessage.Format(IDS_REQUESTTRANSFER, m_pCurrentPlayer->GetName(), WorldData().GetCurrentClub()->GetName());
	}
	else
	{
		strMessage.Format(IDS_UNHAPPYLISTED, m_pCurrentPlayer->GetName());
	}
	OnEndNegotiations(strMessage);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen21Dlg::OnRequestDenied
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:	Deny transfer request
------------------------------------------------------------------------------*/
void CScreen21Dlg::OnRequestDenied()
{
	CString strMessage = m_pCurrentPlayer->DoRespondTransferRequestDenied();
	OnEndNegotiations(strMessage);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen21Dlg::OnEndNegotiations
	Access:    	protected 
	Parameter: 	CString strMessage
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen21Dlg::OnEndNegotiations(CString strMessage)
{
	m_MessageDisplay.AddString(strMessage);
	CBitmapDialog::ReplaceMenu(IDR_MENUCONTINUE);
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen21Dlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen21Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen21Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen21Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen21Dlg::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen21Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		m_PlayerName.MoveWindow();
		CRect theRect;
		GetClientRect(&theRect);
		m_MessageDisplay.MoveWindow(CRect(theRect.left + 2, TOPY + 24, theRect.right - 2, theRect.bottom - 4));
	}
	Sleep(1);
}
