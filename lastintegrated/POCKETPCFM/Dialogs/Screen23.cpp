
// Screen23.cpp : Player Transfer Negotiations implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "MessageDisplay.h"
#include "PlayerContractDlg.h"
#include "PlayerNameTextRegion.h"

#include "Screen23.h"
#include "TransferNegotiation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen23 dialog


/*------------------------------------------------------------------------------
	Method:   	CScreen23Dlg::CScreen23Dlg
	Access:    	public 
	Parameter: 	CString strPrompt
	Parameter: 	CPlayer * pPlayer
	Parameter: 	CClub * pClub
	Parameter: 	CWnd * pParent
	Returns:   		
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen23Dlg::CScreen23Dlg(CString strPrompt, CPlayer* pPlayer, CClub* pClub, CWnd* pParent /*=null*/)
	: m_pCurrentPlayer(pPlayer)
	, m_pClub(pClub)
    , CPlayerContractDlg(_T(""), CScreen23Dlg::IDD, pParent)
{
	// Player's response
	CPlayerContractDlg::m_Prompt = strPrompt;
}


BEGIN_MESSAGE_MAP(CScreen23Dlg, CPlayerContractDlg)
	ON_COMMAND(ID_ACTION_MAKEOFFER, OnOfferContract)
	ON_COMMAND(ID_ACTION_CANCEL, OnCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen23 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen23Dlg::OnInitDialog
	Access:    	virtual protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen23Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN23, IDS_SCREEN23);
	m_pCurrentPlayer->DoGenerateRandomContract(m_ContractRequired, m_pClub->GetCountry());
	CPlayerContractDlg::OnInitDialog();

	m_PlayerName.DoInitialise(TOPY - 4, this, GetBasicDisplay(), *m_pCurrentPlayer);

	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen23Dlg::OnCancel
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen23Dlg::OnCancel()
{
	OnOK();
}

/*------------------------------------------------------------------------------
	Method:   	CScreen23Dlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen23Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		m_PlayerName.MoveWindow();
		CPlayerContractDlg::OnSize(nType, cx, cy);
	}

	Sleep(1);
}


/*------------------------------------------------------------------------------
	Method:   	CNotOwnedScreen23::CNotOwnedScreen23
	Access:    	public 
	Parameter: 	CTransferNegotiation * pNegotiations
	Parameter: 	CWnd * pParent
	Returns:   
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CNotOwnedScreen23::CNotOwnedScreen23(CTransferNegotiation* pNegotiations, CWnd* pParent /*=null*/)
	: m_pNegotiations(pNegotiations)
	, CScreen23Dlg(TEXT("")
	, WorldData().GetPlayer(pNegotiations->GetPlayerID())
	, &WorldData().GetClub(pNegotiations->GetSellClubID())
	, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	CNotOwnedScreen23::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CNotOwnedScreen23::OnInitDialog()
{
	CScreen23Dlg::OnInitDialog();
	m_TextSelector[eCONTRACT].DoAddDates();
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CNotOwnedScreen23::OnOfferButton
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CNotOwnedScreen23::OnOfferContract()
{
	CPlayerContractDlg::OnOfferContract();

	eContractOfferResponse eResponse = CScreen23Dlg::m_pCurrentPlayer->TryAcceptContract(&m_ContractRequired, &m_ContractOffered, *CScreen23Dlg::m_pClub);
	if (eResponse == ACCEPTED)
	{
		m_pNegotiations->SetPlayerAgreed(&m_ContractOffered);
	}
	CString szStr = CScreen23Dlg::m_pCurrentPlayer->GetContractResponseStr(eResponse, &m_ContractOffered);
	AfxMessageBox(szStr, MB_OK | MB_ICONHAND);
	OnOK();
}


/*------------------------------------------------------------------------------
	Method:   	COwnedScreen23::COwnedScreen23
	Access:    	public 
	Parameter: 	CString strPrompt
	Parameter: 	CPlayer * pPlayer
	Parameter: 	CClub * pClub
	Parameter: 	CWnd * pParent
	Returns:   		Qualifier: 	: CScreen23Dlg(strPrompt, pPlayer, pClub, pParent)
	Purpose:
------------------------------------------------------------------------------*/
COwnedScreen23::COwnedScreen23(CString strPrompt, CPlayer* pPlayer, CClub* pClub, CWnd* pParent /*=null*/)
	: CScreen23Dlg(strPrompt, pPlayer, pClub, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	COwnedScreen23::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL COwnedScreen23::OnInitDialog()
{
	CScreen23Dlg::OnInitDialog();

	// Add extra years onto existing contract
	m_TextSelector[eCONTRACT].DoAddDates(CScreen23Dlg::m_pCurrentPlayer->GetContractEndDate().SeasonsRemaining());
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
Method:   	COwnedScreen23::OnOfferButton
Access:    	protected 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void COwnedScreen23::OnOfferContract()
{
	CPlayerContractDlg::OnOfferContract();
	eContractOfferResponse eResponse = CScreen23Dlg::m_pCurrentPlayer->TryAcceptContract(&m_ContractRequired, &m_ContractOffered, *CScreen23Dlg::m_pClub);
	if (eResponse == ACCEPTED)
	{
		CScreen23Dlg::m_pCurrentPlayer->DoUpdateContractOffered(m_ContractOffered);
		OnEndNegotiations();
	}
	CString szStr = CScreen23Dlg::m_pCurrentPlayer->GetContractResponseStr(eResponse, &m_ContractOffered);
	m_Response.AddString(szStr, BLUE);
	m_Response.ShowLastLine();
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	COutOfContractScreen23::COutOfContractScreen23
	Access:    	public 
	Parameter: 	CPlayer * a_pPlayer
	Parameter: 	CClub * a_pClub
	Parameter: 	bool * a_bResult
	Parameter: 	CContract * a_pPlayerContract
	Parameter: 	CWnd * pParent
	Returns:  
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
COutOfContractScreen23::COutOfContractScreen23(CPlayer* a_pPlayer, CClub* a_pClub, bool* a_bResult, CContract* a_pPlayerContract, CWnd* pParent /*= null*/)
	: m_bResult(a_bResult)
	, CScreen23Dlg(TEXT(""), a_pPlayer, a_pClub, pParent)
{
	m_ContractRequired = *a_pPlayerContract;
}


/*------------------------------------------------------------------------------
	Method:   	COutOfContractScreen23::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL COutOfContractScreen23::OnInitDialog()
{
	CScreen23Dlg::OnInitDialog();
	m_TextSelector[eCONTRACT].DoAddDates();
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	COutOfContractScreen23::OnOfferButton
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COutOfContractScreen23::OnOfferContract()
{
	CPlayerContractDlg::OnOfferContract();
	CString szStr;
	if (CScreen23Dlg::m_pClub->GetFirstTeamSquad().TotalSquadSize() < PLAYERSINSQUAD)
	{
		eContractOfferResponse eResponse = CScreen23Dlg::m_pCurrentPlayer->TryAcceptContract(&m_ContractRequired, &m_ContractOffered, *CScreen23Dlg::m_pClub);
		if (eResponse == ACCEPTED)
		{
			*m_bResult = true;
			OnEndNegotiations();
		}
		szStr = CScreen23Dlg::m_pCurrentPlayer->GetContractResponseStr(eResponse, &m_ContractOffered);
	}
	else
	{
		szStr.LoadString(IDS_SQUADFULL);
	}
	AfxMessageBox(szStr, MB_OK | MB_ICONHAND);
	if (*m_bResult == true)
	{
		OnOK();
	}
}
