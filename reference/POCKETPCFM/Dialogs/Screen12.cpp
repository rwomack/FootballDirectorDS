// Screen12.cpp : User receive offer for player implementation file


#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "MessageDisplay.h"
#include "PopupMenu.h"
#include "PlayerNameTextRegion.h"
#include "Screen12.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen12 dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::CScreen12Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns: 
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen12Dlg::CScreen12Dlg(CClubTransferOffer* _BidDetails, CWnd* pParent /*=null*/)
	: CBitmapDialog(CScreen12Dlg::IDD, pParent)
{
	EXT_ASSERT_POINTER(_BidDetails, CClubTransferOffer);
	m_pBidDetails = _BidDetails;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::~CScreen12Dlg
	Access:    	public 
	Returns:   		Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen12Dlg::~CScreen12Dlg()
{
}


BEGIN_MESSAGE_MAP(CScreen12Dlg, CBitmapDialog)
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
// CScreen12 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen12Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_NegotiationsEnded == false)
	{
		CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
		m_messagedisplayResponse.OnKeyDown(nChar, nRepCnt, nFlags);
		m_CurrentTextSelector.OnKeyDown(nChar, nRepCnt, nFlags);
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::OnKeyUp
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen12Dlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_NegotiationsEnded == false)
	{
		m_CurrentTextSelector.OnKeyUp(nChar, nRepCnt, nFlags);
		CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::OnVScroll
	Access:    	protected 
	Parameter: 	UINT nSBCode
	Parameter: 	UINT nPos
	Parameter: 	CScrollBar * pScrollBar
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen12Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_messagedisplayResponse.OnVScroll(nSBCode, nPos, pScrollBar);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::SetValues
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen12Dlg::SetValues()
{
	m_TextSelector[eFEE].SetCurValue(m_pBidDetails->GetAmountOffered());
	m_TextSelector[eSELLON].SetCurValue(m_pBidDetails->GetSellOnPercentOffered());
	CString strMessage;
	if (m_pBidDetails->GetSellOnPercentOffered() == 0)
	{
		strMessage.Format(IDS_ACCEPTBID, m_TextSelector[eFEE].GetCurValueStr(), m_pBidDetails->GetBidClub().GetName());
	}
	else
	{
		strMessage.Format(IDS_ACCEPTBIDSELLON, m_TextSelector[eFEE].GetCurValueStr(), m_pBidDetails->GetSellOnPercentOffered(), m_pBidDetails->GetBidClub().GetName());
	}
	m_messagedisplayResponse.AddString(strMessage, BLUE);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen12Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN12, IDS_SCREEN12);
	CBitmapDialog::DoDisableOKButton();

	m_PlayerName.DoInitialise(TOPY, this, GetBasicDisplay(), m_pBidDetails->GetPlayer());
	m_TextSelector[eFEE].DoInitialise(72, IDS_FEE, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TextSelector[eFEE].SetStyle(COL_ALIGNC | COL_MONEY);
	m_TextSelector[eSELLON].DoInitialise(96, IDS_SELLONFEE, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TextSelector[eSELLON].SetStyle(COL_ALIGNC | COL_PERCENTAGE);
	m_CurrentTextSelector.OnInitialise(&m_TextSelector[0], 5);
	CBitmapDialog::LoadMenu(IDR_ACCEPTREJECTCOUNTERMENU);

    // Setup message display
	m_messagedisplayResponse.SetDrawInfo(this, 0x1000, CRect(5, 130, 236, 218));
	SetValues();
	m_NegotiationsEnded = false;
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen12Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	m_messagedisplayResponse.OnPaint(CBitmapDialog::GetMemDC());
	m_TextSelector[eFEE].OnPaint();
	m_TextSelector[eSELLON].OnPaint();
	if (m_NegotiationsEnded == false)
	{
		m_CurrentTextSelector.OnPaint();
	}
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::OnAccept
	Access:    	virtual protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen12Dlg::OnAccept()
{
	CString Str;
	Str.Format(IDS_AGREETOSELLPLAYER, m_pBidDetails->GetPlayer().GetName(), m_pBidDetails->GetBidClub().GetName());
	if (AfxMessageBox(Str, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDYES)
	{
		m_pBidDetails->SetDealCompleted(true);
		CString strMessage;
		strMessage.Format(IDS_PLAYER_SIGNED, m_pBidDetails->GetPlayer().GetName(), m_pBidDetails->GetBidClub().GetName());
		OnEndNegotiations(strMessage);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::OnOK
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen12Dlg::OnOK()
{
	CDialog::OnOK();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::OnEndNegotiations
	Access:    	protected 
	Parameter: 	CString strMessage
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen12Dlg::OnEndNegotiations(CString strMessage)
{
	m_NegotiationsEnded = true;
	m_messagedisplayResponse.DoShowStringImmediate(strMessage, BLACK);
	CBitmapDialog::ReplaceMenu(IDR_MENUCONTINUE);
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::OnReject
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen12Dlg::OnReject()
{
	CString strMessage;
	strMessage.Format(IDS_BIDREJECTED, m_pBidDetails->GetBidClub().GetName(), m_pBidDetails->GetPlayer().GetName());
	OnEndNegotiations(strMessage);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::OnCounter
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen12Dlg::OnCounter()
{
	CClubTransferOffer CounterBidDetails(m_pBidDetails);
	CounterBidDetails.SetAmountOffered(m_TextSelector[eFEE].GetCurValue());
	CounterBidDetails.SetSellOnPercentOffered(m_TextSelector[eSELLON].GetCurValue());
	CString strMessage;
	if (m_pBidDetails->GetBidClub().DoSubmitCounterAskingPrice(m_pBidDetails, CounterBidDetails) == true)
	{
		// Bidding club agree to user demands
		strMessage.Format(IDS_AGREETOCOUNTERBIDDEMANDS, m_pBidDetails->GetBidClub().GetName(), m_pBidDetails->GetPlayer().GetName());
		m_messagedisplayResponse.DoShowStringImmediate(strMessage);
		OnAccept();
	}
	else
	{
		strMessage.Format(IDS_REFUSETOCOUNTERBIDDEMANDS, m_pBidDetails->GetBidClub().GetName(), m_pBidDetails->GetPlayer().GetName());
		m_messagedisplayResponse.DoShowStringImmediate(strMessage);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen12Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_NegotiationsEnded == false)
	{
		CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
		CBitmapDialog::ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen12Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_NegotiationsEnded == false)
	{
		int ButtonPressed = CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
		switch (ButtonPressed)
		{
			case eFEEDOWN:
			case eFEEUP:
				m_TextSelector[eFEE].OnLButtonUp(ButtonPressed);
				break;
			case eSELLONDOWN:
			case eSELLONUP:
				m_TextSelector[eSELLON].OnLButtonUp(ButtonPressed);
				break;
		}
		CBitmapDialog::ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen12Dlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen12Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		m_PlayerName.MoveWindow();
		m_TextSelector[eFEE].MoveWindow();
		m_TextSelector[eSELLON].MoveWindow();

		CRect theWindow;
		GetClientRect(&theWindow);
		m_messagedisplayResponse.MoveWindow(CRect(4, TOPY + 74, theWindow.right - 4, theWindow.bottom - 4));
	}
	Sleep(1);
}
