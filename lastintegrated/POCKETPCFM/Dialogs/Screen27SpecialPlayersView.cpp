
// Screen27SpecialPlayersView.cpp : implementation file
//
#include "stdafx.h"

#include "UI.h"
#include "Form.h"
#include "Screen27SpecialPlayersView.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScreen27SpecialPlayersView dialog



/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersView::CScreen27SpecialPlayersView
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScreen27SpecialPlayersView::CScreen27SpecialPlayersView()
	: CBitmapFormView(CScreen27SpecialPlayersView::IDD)
{
}


BEGIN_MESSAGE_MAP(CScreen27SpecialPlayersView, CBitmapFormView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()

	ON_COMMAND(ID_ACTION_ACCEPT, OnActionClicked)
	ON_COMMAND(ID_CAPTAIN_NEXT, OnActionCaptainNext)
	ON_COMMAND(ID_CAPTAIN_PREV, OnActionCaptainPrev)
	ON_COMMAND(ID_PENALTY_NEXT, OnActionPenaltyNext)
	ON_COMMAND(ID_PENALTY_PREV, OnActionPenaltyPrev)
	ON_COMMAND(ID_CORNER_NEXT, OnActionCornerNext)
	ON_COMMAND(ID_CORNER_PREV, OnActionCornerPrev)
	ON_COMMAND(ID_FREEKICK_NEXT, OnActionFreeKickNext)
	ON_COMMAND(ID_FREEKICK_PREV, OnActionFreeKickPrev)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreen27SpecialPlayersView message handlers


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersView::OnActionClicked
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersView::OnActionClicked()
{
	CBitmapFormView::OnActionMenu(IDR_SCREEN27ROLEVIEWMENU, GetBasicDisplay()->GetHeight() + 26 - ACTION_MENU_HEIGHT4);
}


void CScreen27SpecialPlayersView::OnActionCaptainNext()
{
	m_Captain.OnIncCurSel();
	DoUpdateData();
}
void CScreen27SpecialPlayersView::OnActionCaptainPrev()
{
	m_Captain.OnDecCurSel();
	DoUpdateData();
}
void CScreen27SpecialPlayersView::OnActionPenaltyNext()
{
	m_PenaltyTaker.OnIncCurSel();
	DoUpdateData();
}
void CScreen27SpecialPlayersView::OnActionPenaltyPrev()
{
	m_PenaltyTaker.OnDecCurSel();
	DoUpdateData();
}
void CScreen27SpecialPlayersView::OnActionCornerNext()
{
	m_CornerTaker.OnIncCurSel();
	DoUpdateData();
}
void CScreen27SpecialPlayersView::OnActionCornerPrev()
{
	m_CornerTaker.OnDecCurSel();
	DoUpdateData();
}
void CScreen27SpecialPlayersView::OnActionFreeKickNext()
{
	m_FreeKickTaker.OnIncCurSel();
	DoUpdateData();
}
void CScreen27SpecialPlayersView::OnActionFreeKickPrev()
{
	m_FreeKickTaker.OnIncCurSel();
	DoUpdateData();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersView::OnLButtonDown
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBitmapFormView::GetBasicDisplay()->OnLButtonDown(nFlags, point);
	CBitmapFormView::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersView::OnLButtonUp
	Access:    	protected 
	Parameter: 	UINT nFlags
	Parameter: 	CPoint point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersView::OnLButtonUp(UINT nFlags, CPoint point)
{
	int ButtonPressed = CBitmapFormView::GetBasicDisplay()->OnLButtonUp(nFlags, point);
	m_Captain.OnLButtonUp(ButtonPressed);
	m_PenaltyTaker.OnLButtonUp(ButtonPressed);
	m_CornerTaker.OnLButtonUp(ButtonPressed);
	m_FreeKickTaker.OnLButtonUp(ButtonPressed);
	DoUpdateData();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersView::DoUpdateData
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersView::DoUpdateData()
{
	CPlayerSquad& pSquad = WorldData().GetCurrentUserClub()->GetFirstTeamSquad();
	pSquad.SetCaptainSquadNum(m_Captain.GetCurSel());
	pSquad.SetPenaltyTakerSquadNum(m_PenaltyTaker.GetCurSel());
	pSquad.SetCornerTakerSquadNum(m_CornerTaker.GetCurSel());
	pSquad.SetFreeKickTakerSquadNum(m_FreeKickTaker.GetCurSel());

	CBitmapFormView::ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersView::OnPaint
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersView::OnPaint()
{
	CBitmapFormView::OnInitRedrawArea();
	m_pngRoles.DoDraw(CBitmapFormView::GetMemDC()->operator HDC(), PICX, CAPTAINY, 1);
	m_pngRoles.DoDraw(CBitmapFormView::GetMemDC()->operator HDC(), PICX, PENALTYTAKERY, 0);
	m_pngRoles.DoDraw(CBitmapFormView::GetMemDC()->operator HDC(), PICX, CORNERTAKERY, 2);
	m_pngRoles.DoDraw(CBitmapFormView::GetMemDC()->operator HDC(), PICX, FREEKICKTAKERY, 3);
	m_Captain.OnPaint();
	m_PenaltyTaker.OnPaint();
	m_CornerTaker.OnPaint();
	m_FreeKickTaker.OnPaint();
	CBitmapFormView::GetMemDC()->Blit();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersView::OnInitialUpdate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersView::OnInitialUpdate()
{
    CBitmapFormView::OnInitialUpdate(TEXT("Match Roles"), &theApp.GetBasicDisplay(), IDS_SCREEN27SPECIALPLAYERS);
	m_pngRoles.DoInitialise(::GetDC(m_hWnd), IDR_PNGROLES, 4);
	CPlayerSquad& pSquad = WorldData().GetCurrentUserClub()->GetFirstTeamSquad();
	m_Captain.DoInitialise((pSquad.GetCaptainSquadNum() != NOPLAYER) ? pSquad.GetCaptainSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_PenaltyTaker.DoInitialise((pSquad.GetPenaltyTakerSquadNum() != NOPLAYER) ? pSquad.GetPenaltyTakerSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_CornerTaker.DoInitialise((pSquad.GetCornerTakerSquadNum() != NOPLAYER) ? pSquad.GetCornerTakerSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_FreeKickTaker.DoInitialise((pSquad.GetFreeKickTakerSquadNum() != NOPLAYER) ? pSquad.GetFreeKickTakerSquadNum() : 0, pSquad, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	SetIsInitialised(true);
}


/*------------------------------------------------------------------------------
	Method:   	CScreen27SpecialPlayersView::OnSize
	Access:    	public 
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CScreen27SpecialPlayersView::OnSize(UINT nType, int cx, int cy)
{
	if (IsInitialised() == true)
	{
		CBitmapFormView::OnSize(nType, cx, cy);
		m_Captain.MoveWindow(CRect(NAMEX, CAPTAINY + 8, NAMEX + COMPETITIONNAMELENGTH * 2, CAPTAINY + 24));
		m_PenaltyTaker.MoveWindow(CRect(NAMEX, PENALTYTAKERY + 8, NAMEX + COMPETITIONNAMELENGTH * 2, PENALTYTAKERY + 24));
		m_CornerTaker.MoveWindow(CRect(NAMEX, CORNERTAKERY + 8, NAMEX + COMPETITIONNAMELENGTH * 2, CORNERTAKERY + 24));
		m_FreeKickTaker.MoveWindow(CRect(NAMEX, FREEKICKTAKERY + 8, NAMEX + COMPETITIONNAMELENGTH * 2, FREEKICKTAKERY + 24));
	}
	Sleep(1);
}