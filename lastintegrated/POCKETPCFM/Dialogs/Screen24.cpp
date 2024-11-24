// Screen24.cpp : Negotiate to get player on loan implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "LoanPeriodSelector.h"
#include "Dialog.h"
#include "PlayerNameTextRegion.h"
#include "Screen24.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen24 dialog



/*------------------------------------------------------------------------------
	Method:   	CScreen24Dlg::CScreen24Dlg
	Access:    	public 
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen24Dlg::CScreen24Dlg(CWnd* pParent /*=null*/)
    : CBitmapDialog(CScreen24Dlg::IDD, pParent)
{
}


BEGIN_MESSAGE_MAP(CScreen24Dlg, CBitmapDialog)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()

	ON_COMMAND(ID_ACTION_MAKEOFFER, OnMakeOffer)
	ON_COMMAND(ID_ACTION_CANCEL, OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen24 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen24Dlg::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen24Dlg::OnPaint()
{
	CBitmapDialog::OnInitRedrawArea();
	m_LoanPeriodSelector.OnPaint();
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen24Dlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen24Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN24, IDS_SCREEN24);
	m_PlayerName.DoInitialise(TOPY, this, GetBasicDisplay(), *m_pCurrentPlayer);
	m_ClubName.DoInitialise(70, this, GetBasicDisplay(), m_pCurrentPlayer->GetClub());
	m_LoanPeriodSelector.OnInitDialog(GetBasicDisplay(), this);

	CString szStr;
	EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
	szStr.Format(IDS_PLAYERLOANSLEFT, MAXPLAYERLOANSPERSEASON - WorldData().GetCurrentUserClub()->GetFirstTeamSquad().GetLoansThisSeason());
	m_LoansLeftStatic.DoInitialise(GetBasicDisplay(), szStr, COL_ALIGNC);
	GetBasicDisplay()->DoAddTextRegion(&m_LoansLeftStatic);

	CBitmapDialog::LoadMenu(IDR_MAKEOFFERMENU);
	SetIsInitialised(true);
    return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen24Dlg::DoModal
	Access:    	public 
	Parameter: 	CPlayer * pPlayer
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CScreen24Dlg::DoModal(CPlayer* pPlayer)
{
	m_pCurrentPlayer = pPlayer;
	return CDialog::DoModal();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen24Dlg::OnMakeOffer
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen24Dlg::OnMakeOffer()
{
	CString szResponse;
	EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
	if (WorldData().GetCurrentUserClub()->GetFirstTeamSquad().TotalSquadSize() == PLAYERSINSQUAD)
	{
		szResponse.Format(IDS_NO_ROOM_SQUAD, m_pCurrentPlayer->GetName(), WorldData().GetCurrentUserClub()->GetName());
	}
	else if (WorldData().GetCurrentUserClub()->GetFirstTeamSquad().GetLoansThisSeason () == MAXPLAYERLOANSPERSEASON) 
	{
		szResponse.LoadString(IDS_NOPLAYERLOANSLEFT);
	}

	else if (m_pCurrentPlayer->GetClub().TryAcceptLoanTransferBid(m_pCurrentPlayer, WorldData().GetCurrentUserClub(), m_LoanPeriodSelector.GetPeriod(), &szResponse) == true)
	{
		m_pCurrentPlayer->DoGoingOnLoan(&m_pCurrentPlayer->GetClub(), m_LoanPeriodSelector.GetPeriod());
		ushort iPlayer = m_pCurrentPlayer->DoFindID();
		m_pCurrentPlayer->GetClub().DoPlayerGoingOutOnLoan(iPlayer);
		WorldData().GetCurrentUserClub()->GotPlayerOnLoan(iPlayer, m_LoanPeriodSelector.GetPeriod());
	}
	AfxMessageBox(szResponse, MB_OK | MB_ICONHAND);
	OnOK();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen24Dlg::OnKeyDown
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen24Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyDown(nChar, nRepCnt, nFlags);
	m_LoanPeriodSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen24Dlg::OnKeyUp
	Access:    	public 
	Parameter: 	UINT nChar
	Parameter: 	UINT nRepCnt
	Parameter: 	UINT nFlags
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen24Dlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBitmapDialog::OnKeyUp(nChar, nRepCnt, nFlags);
	m_LoanPeriodSelector.OnKeyUp(nChar, nRepCnt, nFlags);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen24Dlg::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen24Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen24Dlg::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen24Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	int ButtonPressed = CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	m_LoanPeriodSelector.OnLButtonUp(ButtonPressed);
	//ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen24Dlg::OnSize
	Access:    	protected 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen24Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		m_PlayerName.MoveWindow();
		m_ClubName.MoveWindow();
		m_LoanPeriodSelector.OnSize(CRect(0, LOANY, GetCentreX() * 2, LOANY + 16), GetCentreX());
		int LoanY = LOANY + 40;
		m_LoansLeftStatic.MoveWindow(CRect(0, LoanY, GetCentreX() * 2, LoanY + 16));
	}

	Sleep(1);
}
