// Screen30.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "PlayerNameTextRegion.h"
#include "Screen30.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen30 dialog



/*------------------------------------------------------------------------------
	Method:   	CScreen30Dlg::CScreen30Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CScreen30Dlg::CScreen30Dlg(CPlayer* _Player, CWnd* pParent /*=null*/)
    : CBitmapDialog(CScreen30Dlg::IDD, pParent)
{
	EXT_ASSERT_POINTER(_Player, CPlayer);
	m_pCurrentPlayer = _Player;
	m_NegotiationsEnded = false;
}


BEGIN_MESSAGE_MAP(CScreen30Dlg, CBitmapDialog)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()

	ON_COMMAND(ID_ACTION_MAKEOFFER, OnMakeOffer)
	ON_COMMAND(ID_ACTION_CANCEL, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen30 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen30Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen30Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	m_CurrentTextSelector.OnPaint();

	if (m_NegotiationsEnded == false)
	{
		m_TextSelector[eFEE].OnPaint();
		m_TextSelector[eSELLON].OnPaint();
	}
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen30Dlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen30Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN30, IDS_SCREEN30);

	m_TextSelector[eFEE].DoInitialise(96, IDS_FEE, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TextSelector[eFEE].SetStyle(COL_ALIGNC | COL_MONEY);
	if (m_pCurrentPlayer->GetTransferStatus().IsTransferListed() == true)
	{
		m_TextSelector[eFEE].SetCurValue(m_pCurrentPlayer->GetTransferStatus().GetAskingPrice());
	}
	else
	{
		m_TextSelector[eFEE].SetCurValue(m_pCurrentPlayer->GetValuation(WorldData().GetCurrentUserClub()->GetCountry()));
	}
	m_TextSelector[eSELLON].DoInitialise(120, IDS_SELLONFEE, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TextSelector[eSELLON].SetStyle(COL_ALIGNC | COL_PERCENTAGE);
	m_TextSelector[eSELLON].SetCurValue(m_BidDetails.GetSellOnPercentOffered());
	m_CurrentTextSelector.OnInitialise(&m_TextSelector[0], eMAXSELECTOR);

	m_BidDetails.SetPlayer(m_pCurrentPlayer);
	m_BidDetails.SetBidClub(WorldData().GetCurrentUserClub());
	m_BidDetails.SetSellOnPercentOffered(0);
	
	m_PlayerName.DoInitialise(TOPY, this, GetBasicDisplay(), *m_pCurrentPlayer);
	m_ClubName.DoInitialise(TOPY + 20, this, GetBasicDisplay(), m_pCurrentPlayer->GetClub());
	m_ClubBalanceText.DoInitialise(GetBasicDisplay(), WorldData().GetCurrentUserClub(), COL_ALIGNC);
	CBitmapDialog::LoadMenu(IDR_MAKEOFFERMENU);
	SetIsInitialised(true);
    return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen30Dlg::OnKeyDown
	Access:    	protected 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
-------------------------------------------------------------- ----------------*/
void CScreen30Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	m_CurrentTextSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	switch (nChar)
	{
		case VK_F23:
		case VK_RETURN: 
			OnOK();
			break;
	}
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
void CScreen30Dlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
	m_CurrentTextSelector.OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen30Dlg::OnMakeOffer
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen30Dlg::OnMakeOffer()
{
	m_BidDetails.SetAmountOffered(m_TextSelector[eFEE].GetCurValue());
	bool bAccepted = false;
	if (m_pCurrentPlayer->GetClub().TryAcceptTransferBid(m_BidDetails) == true)
	{
		bAccepted = true;
		CString str = WorldData().GetCurrentUser()->AddTransferNegotiation(m_BidDetails);
		m_BidDetails.m_szReasonString += _T(" ") + str;

	}
	AfxMessageBox(m_BidDetails.m_szReasonString, MB_OK | MB_ICONHAND);
	m_NegotiationsEnded = true;
	if (bAccepted == true)
	{
		OnOK();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CScreen30Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen30Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetBasicDisplay()->OnLButtonDown(nFlags, point);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen30Dlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen30Dlg::OnLButtonUp(UINT nFlags, CPoint point)
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
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen30Dlg::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen30Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		m_PlayerName.MoveWindow();
		m_ClubName.MoveWindow();
		m_TextSelector[eFEE].MoveWindow();
		m_TextSelector[eSELLON].MoveWindow();

		CRect theWindow;
		GetClientRect(&theWindow);
		m_ClubBalanceText.MoveWindow(CRect(0, 142, theWindow.right, 160));
	}
	Sleep(1);
}