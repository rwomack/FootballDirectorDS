// Screen27SpecialPlayersPage.cpp : implementation file
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
// CScreen27Tactics property page



/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersDlg::CScreen27SpecialPlayersDlg
	Access:    	public 
	Returns:   	
	Qualifier: 	: CInMatchActionBitmapDialog(IDD)
	Purpose:
------------------------------------------------------------------------------*/
CScreen27SpecialPlayersDlg::CScreen27SpecialPlayersDlg(CWnd* pParent /*=null*/) 
	: CInMatchActionBitmapDialog(CScreen27SpecialPlayersDlg::IDD, pParent)
{
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersDlg::~CScreen27SpecialPlayersDlg
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CScreen27SpecialPlayersDlg::~CScreen27SpecialPlayersDlg()
{
}


BEGIN_MESSAGE_MAP(CScreen27SpecialPlayersDlg, CInMatchActionBitmapDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen27SpecialPlayersDlg message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersDlg::OnActionCaptainNext
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersDlg::OnActionCaptainNext()
{
	m_Captain.OnIncCurSel();
	DoUpdateData();
}
void CScreen27SpecialPlayersDlg::OnActionCaptainPrev()
{
	m_Captain.OnDecCurSel();
	DoUpdateData();
}
void CScreen27SpecialPlayersDlg::OnActionPenaltyNext()
{
	m_PenaltyTaker.OnIncCurSel();
	DoUpdateData();
}
void CScreen27SpecialPlayersDlg::OnActionPenaltyPrev()
{
	m_PenaltyTaker.OnDecCurSel();
	DoUpdateData();
}
void CScreen27SpecialPlayersDlg::OnActionCornerNext()
{
	m_CornerTaker.OnIncCurSel();
	DoUpdateData();
}
void CScreen27SpecialPlayersDlg::OnActionCornerPrev()
{
	m_CornerTaker.OnDecCurSel();
	DoUpdateData();
}
void CScreen27SpecialPlayersDlg::OnActionFreeKickNext()
{
	m_FreeKickTaker.OnIncCurSel();
	DoUpdateData();
}
void CScreen27SpecialPlayersDlg::OnActionFreeKickPrev()
{
	m_FreeKickTaker.OnIncCurSel();
	DoUpdateData();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersView::OnLButtonDown
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapDialog::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersView::OnLButtonUp
	Access:    	public 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	int ButtonPressed = CBitmapDialog::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	m_Captain.OnLButtonUp(ButtonPressed);
	m_PenaltyTaker.OnLButtonUp(ButtonPressed);
	m_CornerTaker.OnLButtonUp(ButtonPressed);
	m_FreeKickTaker.OnLButtonUp(ButtonPressed);
	DoUpdateData();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersDlg::DoUpdateData
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersDlg::DoUpdateData()
{
	CPlayerSquad& pSquad = WorldData().GetCurrentUserClub()->GetFirstTeamSquad();
	pSquad.SetCaptainSquadNum(m_Captain.GetCurSel());
	pSquad.SetPenaltyTakerSquadNum(m_PenaltyTaker.GetCurSel());
	pSquad.SetCornerTakerSquadNum(m_CornerTaker.GetCurSel());
	pSquad.SetFreeKickTakerSquadNum(m_FreeKickTaker.GetCurSel());

	CBitmapDialog::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersDlg::OnPaint
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersDlg::OnPaint()
{
	CInMatchActionBitmapDialog::OnPaint();
	m_Captain.OnPaint();
	m_PenaltyTaker.OnPaint();
	m_CornerTaker.OnPaint();
	m_FreeKickTaker.OnPaint();
	m_pngRoles.DoDraw(CBitmapDialog::GetMemDC()->operator HDC(), PICX, CAPTAINY, 1);
	m_pngRoles.DoDraw(CBitmapDialog::GetMemDC()->operator HDC(), PICX, PENALTYTAKERY, 0);
	m_pngRoles.DoDraw(CBitmapDialog::GetMemDC()->operator HDC(), PICX, CORNERTAKERY, 2);
	m_pngRoles.DoDraw(CBitmapDialog::GetMemDC()->operator HDC(), PICX, FREEKICKTAKERY, 3);
	CBitmapDialog::DoBlit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersDlg::OnInitDialog
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CScreen27SpecialPlayersDlg::OnInitDialog()
{
	CInMatchActionBitmapDialog::OnInitDialog(IDS_HELPSCREEN27R, IDS_SCREEN27SPECIALPLAYERS);
	m_pngRoles.DoInitialise(::GetDC(m_hWnd), IDR_PNGROLES, 4);
	CPlayerSquad& pSquad = WorldData().GetCurrentUserClub()->GetFirstTeamSquad();
	m_Captain.DoInitialise((pSquad.GetCaptainSquadNum() != NOPLAYER) ? pSquad.GetCaptainSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_PenaltyTaker.DoInitialise((pSquad.GetPenaltyTakerSquadNum() != NOPLAYER) ? pSquad.GetPenaltyTakerSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_CornerTaker.DoInitialise((pSquad.GetCornerTakerSquadNum() != NOPLAYER) ? pSquad.GetCornerTakerSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_FreeKickTaker.DoInitialise((pSquad.GetFreeKickTakerSquadNum() != NOPLAYER) ? pSquad.GetFreeKickTakerSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	SetIsInitialised(true);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersDlg::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersDlg::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapDialog::OnSize(nType, cx, cy);
		m_Captain.MoveWindow(CRect(NAMEX, CAPTAINY + 8, NAMEX + COMPETITIONNAMELENGTH * 2, CAPTAINY + 24));
		m_PenaltyTaker.MoveWindow(CRect(NAMEX, PENALTYTAKERY + 8, NAMEX + COMPETITIONNAMELENGTH * 2, PENALTYTAKERY + 24));
		m_CornerTaker.MoveWindow(CRect(NAMEX, CORNERTAKERY + 8, NAMEX + COMPETITIONNAMELENGTH * 2, CORNERTAKERY + 24));
		m_FreeKickTaker.MoveWindow(CRect(NAMEX, FREEKICKTAKERY + 8, NAMEX + COMPETITIONNAMELENGTH * 2, FREEKICKTAKERY + 24));
	}
	Sleep(1);
}
