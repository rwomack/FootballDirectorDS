
/*

	Class - CScreen13

	Youth Player contract Negotiation Dialog Box class

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */

#include "stdafx.h"

#include "UI.h"
#include "Dialog.h"
#include "MessageDisplay.h"
#include "PlayerContractDlg.h"
#include "PlayerNameTextRegion.h"

#include "Screen13.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen13 dialog



/*------------------------------------------------------------------------------
	Method:   	CScreen13Dlg::CScreen13Dlg
	Access:    	public 
	Parameter: 	CYouth * pYouth
	Parameter: 	CString strPrompt
	Parameter: 	CWnd * pParent
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen13Dlg::CScreen13Dlg(CYouth* pYouth, CString strPrompt, CWnd* pParent /*=null*/)
    : m_pCurrentYouth(pYouth)
	, CPlayerContractDlg(strPrompt, CScreen13Dlg::IDD, pParent)
{
}


BEGIN_MESSAGE_MAP(CScreen13Dlg, CPlayerContractDlg)
	ON_COMMAND(ID_ACTION_MAKEOFFER, OnOfferContract)
	ON_COMMAND(ID_ACTION_CANCEL, OnCancelButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen13 message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen13Dlg::OnInitDialog
	Access:    	protected 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen13Dlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog(IDS_HELPSCREEN13, IDS_SCREEN13);
	m_pCurrentYouth->DoGenerateRandomContract(m_ContractRequired, m_pCurrentYouth->GetClub().GetCountry());
	CPlayerContractDlg::OnInitDialog();
	m_NameText.DoInitialise(TOPY -  4, this, GetBasicDisplay(), *m_pCurrentYouth);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen13Dlg::OnCancelButton
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen13Dlg::OnCancelButton()
{
	m_pCurrentYouth->SetContractRenewalStatus(CLUBREFUSED);
	OnOK();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen13Dlg::OnOfferContract
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen13Dlg::OnOfferContract()
{
	CPlayerContractDlg::OnOfferContract();
	if (m_pCurrentYouth->GetClub().GetFirstTeamSquad().TotalSquadSize() < PLAYERSINSQUAD)
	{
		m_Response.RemoveAll();
		eContractOfferResponse eResponse = m_pCurrentYouth->TryAcceptContract(&m_ContractRequired,  m_pCurrentYouth->GetClub().GetCountry(), &m_ContractOffered);
		CString szStr = m_pCurrentYouth->GetContractResponseStr(eResponse, &m_ContractOffered);
		if (eResponse == ACCEPTED)
		{
			m_Response.AddString(szStr, BLUE);
			OnEndNegotiations();
			m_pCurrentYouth->GetClub().PromoteYouth(m_pCurrentYouth->GetClub().GetYouthList().Find(m_pCurrentYouth), &m_ContractOffered);
			m_pCurrentYouth = null;		// Current Youth no longer exists
		}
		else
		{
			m_Response.AddString(szStr, RED);
		}
	}
	else
	{
		m_Response.AddString(IDS_SQUADFULL, RED);
		OnEndNegotiations();
	}
	m_Response.ShowLastLine();
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen13Dlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen13Dlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CPlayerContractDlg::OnSize(nType, cx, cy);
		m_NameText.MoveWindow();
	}
	Sleep(1);
}