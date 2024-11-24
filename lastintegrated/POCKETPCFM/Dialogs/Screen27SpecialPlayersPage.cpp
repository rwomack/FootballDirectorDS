// Screen27SpecialPlayersPage.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "..\system\Screen\Win32\ExtPropertyPage.h"
#include "Screen27SpecialPlayersPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreen27Tactics property page


IMPLEMENT_DYNCREATE(CScreen27SpecialPlayersPage, CPropertyPage)



/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersPage::CScreen27SpecialPlayersPage
	Access:    	public 
	Returns:   	
	Qualifier: 	: CBitmapPropertyPage(IDD)
	Purpose:
------------------------------------------------------------------------------*/
CScreen27SpecialPlayersPage::CScreen27SpecialPlayersPage() 
	: CBitmapPropertyPage(CScreen27SpecialPlayersPage::IDD)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersPage::~CScreen27SpecialPlayersPage
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen27SpecialPlayersPage::~CScreen27SpecialPlayersPage()
{
}


BEGIN_MESSAGE_MAP(CScreen27SpecialPlayersPage, CBitmapPropertyPage)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen27SpecialPlayersPage message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersPage::GetClub
	Access:    	public 
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CScreen27SpecialPlayersPage::GetClub()
{
	if (m_pCurrentMatchPlay->IsUserControlledClub(HOME) == true)
	{
		return m_pCurrentMatchPlay->GetFixture().GetClub(HOME);
	}
	return m_pCurrentMatchPlay->GetFixture().GetClub(AWAY);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersPage::OnKillActive
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen27SpecialPlayersPage::OnKillActive()
{
	CPlayerSquad* pSquad = &GetClub().GetFirstTeamSquad();
	CComboBox* pList1 = (CComboBox*)GetDlgItem(IDC_LIST3501);
	CComboBox* pList2 = (CComboBox*)GetDlgItem(IDC_LIST3502);
	CComboBox* pList3 = (CComboBox*)GetDlgItem(IDC_LIST3503);
	CComboBox* pList4 = (CComboBox*)GetDlgItem(IDC_LIST3504);
	
	if (pList1->GetCurSel() != NOPLAYER)
	{
		pSquad->SetCaptainsName(pList1->GetCurSel());
	}
	if (pList2->GetCurSel() != NOPLAYER)
	{
		pSquad->SetPenaltyTakerName(pList2->GetCurSel());
	}
	if (pList3->GetCurSel() != NOPLAYER)
	{
		pSquad->SetCornerTakerName(pList3->GetCurSel());
	}
	if (pList4->GetCurSel() != NOPLAYER)
	{
		pSquad->SetFreeKickTakerName(pList4->GetCurSel());
	}
	return TRUE;
}

		
/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersPage::OnSetActive
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen27SpecialPlayersPage::OnSetActive()
{
	CComboBox* pList1 = (CComboBox*)GetDlgItem(IDC_LIST3501);
	CComboBox* pList2 = (CComboBox*)GetDlgItem(IDC_LIST3502);
	CComboBox* pList3 = (CComboBox*)GetDlgItem(IDC_LIST3503);
	CComboBox* pList4 = (CComboBox*)GetDlgItem(IDC_LIST3504);
			
	pList1->ResetContent();
	pList2->ResetContent();
	pList3->ResetContent();
	pList4->ResetContent();

	CClub& CurrentClub = GetClub();
	CPlayerSquad* pSquad = &CurrentClub.GetFirstTeamSquad();
	for (int nCount = 0; nCount < PLAYERSINTEAM; nCount++)
	{
		if (pSquad->GetSquad(nCount) != NOPLAYER)
		{
			pList1->AddString(CurrentClub.GetPlayer(nCount).GetName());
			pList2->AddString(CurrentClub.GetPlayer(nCount).GetName());
			pList3->AddString(CurrentClub.GetPlayer(nCount).GetName());
			pList4->AddString(CurrentClub.GetPlayer(nCount).GetName());
		}
	}
		
	if (pSquad->GetCaptainSquadID() != NOPLAYER)
	{
		pList1->SetCurSel(pSquad->GetCaptainSquadID());
	}
	if (pSquad->GetPenaltyTakerSquadID() != NOPLAYER)
	{
		pList2->SetCurSel(pSquad->GetPenaltyTakerSquadID());
	}
	if (pSquad->GetCornerTakerSquadID() != NOPLAYER)
	{
		pList3->SetCurSel(pSquad->GetCornerTakerSquadID());
	}
	if (pSquad->GetFreeKickTakerSquadID() != NOPLAYER)
	{
		pList4->SetCurSel(pSquad->GetFreeKickTakerSquadID());
	}
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersPage::OnPaint
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersPage::OnPaint()
{
	CBitmapPropertyPage::OnPaint();
	m_pngRoles.DoDraw(CBitmapPropertyPage::GetMemDC()->operator HDC(), ROLEX, CAPTAINY - 2, 1);
	m_pngRoles.DoDraw(CBitmapPropertyPage::GetMemDC()->operator HDC(), ROLEX, PENALTYTAKERY - 2, 0);
	m_pngRoles.DoDraw(CBitmapPropertyPage::GetMemDC()->operator HDC(), ROLEX, CORNERTAKERY - 2, 2);
	m_pngRoles.DoDraw(CBitmapPropertyPage::GetMemDC()->operator HDC(), ROLEX, FREEKICKTAKERY - 2, 3);
	CBitmapPropertyPage::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersPage::OnInitDialog
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen27SpecialPlayersPage::OnInitDialog()
{
	m_pngRoles.DoInitialise(::GetDC(m_hWnd), IDR_PNGROLES, 4);
	
	CComboBox* pList1 = (CComboBox*)GetDlgItem(IDC_LIST3501);
	CComboBox* pList2 = (CComboBox*)GetDlgItem(IDC_LIST3502);
	CComboBox* pList3 = (CComboBox*)GetDlgItem(IDC_LIST3503);
	CComboBox* pList4 = (CComboBox*)GetDlgItem(IDC_LIST3504);
    for (uint LoopCount = 0; LoopCount < PLAYERSINSQUAD; LoopCount++)
	{
        if (WorldData().GetCurrentUserClub()->GetFirstTeamSquad().GetSquad(LoopCount) != NOPLAYER)
		{
			pList1->AddString(WorldData().GetCurrentUserClub()->GetPlayer(LoopCount).GetName());
			pList2->AddString(WorldData().GetCurrentUserClub()->GetPlayer(LoopCount).GetName());
			pList3->AddString(WorldData().GetCurrentUserClub()->GetPlayer(LoopCount).GetName());
			pList4->AddString(WorldData().GetCurrentUserClub()->GetPlayer(LoopCount).GetName());
		}
	}
    return CBitmapPropertyPage::OnInitDialog(TEXT("Match Roles"));
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersPage::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersPage::OnSize(UINT nType, int cx, int cy)
{
	CBitmapPropertyPage::OnSize(nType, cx, cy);
	CBitmapPropertyPage::GetBasicDisplay().SetDisplayHeader(true, IDS_SCREEN27SPECIALPLAYERS);

	Sleep(1);
}
