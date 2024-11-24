// Screen44.cpp : User receive offer for 'Receive loan offer for player' implementation file


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "PlayerNameTextRegion.h"
#include "LoanPeriodSelector.h"
#include "MessageDisplay.h"
#include "PopupMenu.h"
#include "Screen44Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen44Dlg dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::CScreen44Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns: 
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen44Dlg::CScreen44Dlg(CClubTransferOffer* _BidDetails, CWnd* pParent /*=null*/)
	: CBitmapDialog(CScreen44Dlg::IDD, pParent)
{
	EXT_ASSERT_POINTER(_BidDetails, CClubTransferOffer);
	m_pBidDetails = _BidDetails;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::~CScreen44Dlg
	Access:    	public 
	Returns:   		Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen44Dlg::~CScreen44Dlg()
{
}


BEGIN_MESSAGE_MAP(CScreen44Dlg, CBitmapDialog)
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_PAINT()
	ON_WM_VSCROLL()

	ON_COMMAND(ID_ACTION_ACCEPT, OnAccept)
	ON_COMMAND(ID_ACTION_REJECT, OnReject)
	ON_COMMAND(ID_ACTION_COUNTER, OnCounter)
	ON_COMMAND(ID_CONTINUE, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen44 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen44Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_NegotiationsEnded == false)
	{
		CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
		m_LoanPeriodSelector.OnKeyDown(nChar, nRepCnt, nFlags);
		m_messagedisplayResponse.OnKeyDown(nChar, nRepCnt, nFlags);
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen44Dlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_NegotiationsEnded == false)
	{
		CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
		m_LoanPeriodSelector.OnKeyUp(nChar, nRepCnt, nFlags);
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::OnVScroll
	Access:    	protected 
	Parameter: 	UINT nSBCode
	Parameter: 	UINT nPos
	Parameter: 	CScrollBar * pScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen44Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_messagedisplayResponse.OnVScroll(nSBCode, nPos, pScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen44Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN44, IDS_SCREEN44);
	CBitmapDialog::DoDisableOKButton();

	m_PlayerName.DoInitialise(TOPY, this, GetBasicDisplay(), m_pBidDetails->GetPlayer());
	m_ClubName.DoInitialise(70, this, GetBasicDisplay(), m_pBidDetails->GetBidClub());
	m_LoanPeriodSelector.OnInitDialog(GetBasicDisplay(), this);
	CBitmapDialog::LoadMenu(IDR_ACCEPTREJECTCOUNTERMENU);

    // Setup message display
	m_messagedisplayResponse.SetDrawInfo(this, 0x1000, CRect(5, 130, 236, 218), BLACK, TRUE);
	CExtString strMessage;
	strMessage.Format(IDS_RECEIVELOANBID, m_pBidDetails->GetBidClub().GetName(), m_pBidDetails->GetPlayer().GetName(), m_pBidDetails->GetLoanPeriod());
	m_messagedisplayResponse.AddString(strMessage);
	m_NegotiationsEnded = false;
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen44Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	m_LoanPeriodSelector.OnPaint();
	m_messagedisplayResponse.OnPaint(CBitmapDialog::GetMemDC());
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::OnAccept
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen44Dlg::OnAccept()
{
	CString Str;
	Str.Format(IDS_AGREETOLOANPLAYER, m_pBidDetails->GetPlayer().GetName(), m_pBidDetails->GetBidClub().GetName());
	if (AfxMessageBox(Str, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDYES)
	{
		m_pBidDetails->SetDealCompleted(true);
		CString strMessage;
		strMessage.Format(IDS_PLAYER_SIGNED, m_pBidDetails->GetPlayer().GetName(), m_pBidDetails->GetBidClub().GetName());
		WorldData().OnLoanPlayerOut(m_pBidDetails->GetPlayerID(), m_pBidDetails->GetBidClubID(), m_pBidDetails->GetLoanPeriod());
		OnEndNegotiations(strMessage);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::OnOK
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen44Dlg::OnOK()
{
	CDialog::OnOK();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::OnEndNegotiations
	Access:    	protected 
	Parameter: 	CString strMessage
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen44Dlg::OnEndNegotiations(CString strMessage)
{
	m_NegotiationsEnded = true;
	m_messagedisplayResponse.DoShowStringImmediate(strMessage, BLACK);
	CBitmapDialog::ReplaceMenu(IDR_MENUCONTINUE);
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::OnReject
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen44Dlg::OnReject()
{
	CString strMessage;
	strMessage.Format(IDS_BIDREJECTED, m_pBidDetails->GetBidClub().GetName(), m_pBidDetails->GetPlayer().GetName());
	OnEndNegotiations(strMessage);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::OnCounter
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen44Dlg::OnCounter()
{
	CString strMessage;
	if (m_pBidDetails->GetBidClub().DoSubmitCounterLoanPeriod(m_pBidDetails, m_LoanPeriodSelector.GetPeriod()) == true)
	{
		// Bidding club agree to user demands
		strMessage.Format(IDS_AGREETOCOUNTERBIDDEMANDS, m_pBidDetails->GetBidClub().GetName(), m_pBidDetails->GetPlayer().GetName());
		m_messagedisplayResponse.DoShowStringImmediate(strMessage);
		OnAccept();
	}
	else
	{
		EXT_ASSERT(false); // Always accepted!!
	//	strMessage.Format(IDS_REFUSETOCOUNTERBIDDEMANDS, m_pBidDetails->GetBidClub().GetName(), m_pBidDetails->GetPlayer().GetName());
	//	m_messagedisplayResponse.DoShowStringImmediate(strMessage);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen44Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_NegotiationsEnded == false)
	{
		CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
		CBitmapDialog::ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen44Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
 	if (m_NegotiationsEnded == false)
 	{
 		int ButtonPressed = CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
		m_LoanPeriodSelector.OnLButtonUp(ButtonPressed);
 		CBitmapDialog::ReDraw();
 	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen44Dlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen44Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		m_PlayerName.MoveWindow();

		CRect theWindow;
		GetClientRect(&theWindow);
		m_LoanPeriodSelector.OnSize(CRect(0, TOPY + 20, GetCentreX() * 2, TOPY + 20 + 16), GetCentreX());
		m_messagedisplayResponse.MoveWindow(CRect(4, TOPY + 70, theWindow.right - 4, theWindow.bottom - 4));
	}
	Sleep(1);
}
