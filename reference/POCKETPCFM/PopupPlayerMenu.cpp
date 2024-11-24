
/*

	PopupPlayerMenu.cpp: implementation for various PopupMenu classes.

	Football Director

	(c) Rw Software 1994 - 2008

---------------------------------------------------------------
*/


#include "stdafx.h"
#include "PopupPlayerMenu.h"
#include "PopupMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/*------------------------------------------------------------------------------
	Method:   	CPopupPlayerMenu::DoModal
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Parameter: 	CPoint & _Point
	Parameter: 	bool _ShowDetails
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPopupPlayerMenu::DoModal(CPlayer* _Player, CPoint& _Point, bool _ShowDetails /*= true*/)
{
	if (WorldData().GetCurrentUser()->IsManagingAClub() == false)
	{
		WorldData().SetCurrentPlayer(_Player);
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_OUTOFCONTRACTPLAYER));
		CMenu* pPopup = menu.GetSubMenu(0);
		EXT_ASSERT_POINTER(pPopup, CMenu);
		pPopup->RemoveMenu(IDP_CONTRACT1, MF_BYCOMMAND);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN, _Point.x, _Point.y + 16, AfxGetMainWnd());
		AfxGetApp()->PumpMessage();
	}
	else if (_Player->GetTransferStatus().IsOutOfContract() == true)
	{
		CPopupOutOfContractPlayerMenu TheMenu;
		TheMenu.DoModal(_Player, _Point, _ShowDetails);
	}
	else if (_Player->IsAtCurrentUserControlledClub() == true)
	{
		CPopupOwnedPlayerMenu TheMenu;
		TheMenu.DoModal(_Player, _Point, _ShowDetails);
	}
	else
	{
		CPopupNotOwnedPlayerMenu TheMenu;
		TheMenu.DoModal(_Player, _Point, _ShowDetails);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPopupOutOfContractPlayerMenu::DoModal
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Parameter: 	CPoint & _Point
	Parameter: 	bool _ShowDetails
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPopupOutOfContractPlayerMenu::DoModal(CPlayer* _Player, CPoint& _Point, bool _ShowDetails /*= true*/)
{
	WorldData().SetCurrentPlayer(_Player);
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_OUTOFCONTRACTPLAYER));
	CMenu* pPopup = menu.GetSubMenu(0);
	EXT_ASSERT_POINTER(pPopup, CMenu);
	if (_ShowDetails == false)
	{
		// View Player Info not required
		pPopup->RemoveMenu(IDP_INFO, MF_BYCOMMAND);
	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN, _Point.x, _Point.y + 16, AfxGetMainWnd());
	AfxGetApp()->PumpMessage();
}


/*------------------------------------------------------------------------------
	Method:   	CPopupOwnedPlayerMenu::DoModal
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Parameter: 	CPoint & _Point
	Parameter: 	bool _ShowDetails
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPopupOwnedPlayerMenu::DoModal(CPlayer* _Player, CPoint& _Point, bool _ShowDetails /*= true*/)
{
	WorldData().SetCurrentPlayer(_Player);
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_OWNEDPLAYER));
	CMenu* pPopup = menu.GetSubMenu(0);
	EXT_ASSERT_POINTER(pPopup, CMenu);
	if (_ShowDetails == false)
	{
		// View Player Info not required
		pPopup->RemoveMenu(IDP_INFO, MF_BYCOMMAND);
	}
	pPopup->TrackPopupMenu(TPM_LEFTALIGN, _Point.x, _Point.y, AfxGetMainWnd());
	AfxGetApp()->PumpMessage();
}


/*------------------------------------------------------------------------------
	Method:   	CPopupNotOwnedPlayerMenu::DoModal
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Parameter: 	CPoint & _Point
	Parameter: 	bool _ShowDetails
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPopupNotOwnedPlayerMenu::DoModal(CPlayer* _Player, CPoint& _Point, bool _ShowDetails /*= true*/)
{
	WorldData().SetCurrentPlayer(_Player);
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_NOTOWNEDPLAYER));
	CMenu* pPopup = menu.GetSubMenu(0);
	EXT_ASSERT_POINTER(pPopup, CMenu);
	if (_ShowDetails == false)
	{
		// View Player Info not required
		pPopup->RemoveMenu(IDP_NOTOWNEDPLAYERINFO, MF_BYCOMMAND);
		_Point.y = 233;
	}
	if (WorldData().GetCurrentUserClub()->AreLoanTransfersAllowed() == false)
	{
		pPopup->RemoveMenu(IDP_APROACHLOANPLAYER, MF_BYCOMMAND);
	}
	if (WorldData().GetCurrentUserClub()->AreBuyTransfersAllowed() == false)
	{
		pPopup->RemoveMenu(IDP_APROACHBUYPLAYER, MF_BYCOMMAND);
	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN, _Point.x, _Point.y, AfxGetMainWnd());
	AfxGetApp()->PumpMessage();
}


/*------------------------------------------------------------------------------
	Method:   	CPopupOwnedYouthMenu::DoModal
	Access:    	public 
	Parameter: 	CYouth * _Youth
	Parameter: 	CPoint & _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPopupOwnedYouthMenu::DoModal(CYouth* _Youth, CPoint& _Point)
{
	EXT_ASSERT_POINTER(_Youth, CYouth);
	WorldData().SetCurrentYouth(_Youth);
	CPopupMenu menu;
	menu.DoModal(IDR_YOUTH, _Point.x, _Point.y, AfxGetMainWnd());
}


/*------------------------------------------------------------------------------
	Method:   	CPopupNotOwnedYouthMenu::DoModal
	Access:    	public 
	Parameter: 	CYouth * _Youth
	Parameter: 	CPoint & _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPopupNotOwnedYouthMenu::DoModal(CYouth* _Youth, CPoint& _Point)
{
	EXT_ASSERT_POINTER(_Youth, CYouth);
	WorldData().SetCurrentYouth(_Youth);
	CPopupMenu menu;
	menu.DoModal(IDR_AVAILABLEYOUTH, _Point.x, _Point.y, AfxGetMainWnd());
}


/*------------------------------------------------------------------------------
	Method:   	CPopupOwnedEmployeeMenu::DoModal
	Access:    	public 
	Parameter: 	CEmployee * _Employee
	Parameter: 	CPoint & _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPopupOwnedEmployeeMenu::DoModal(CEmployee* _Employee, CPoint& _Point)
{
	EXT_ASSERT_POINTER(_Employee, CEmployee);
	WorldData().SetCurrentEmployee(_Employee);
	CPopupMenu menu;
	menu.DoModal(IDR_OWNEDEMPLOYEE, _Point.x, _Point.y, AfxGetMainWnd());
}


/*------------------------------------------------------------------------------
	Method:   	CPopupNotOwnedEmployeeMenu::DoModal
	Access:    	public 
	Parameter: 	CEmployee * _Employee
	Parameter: 	CPoint & _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPopupNotOwnedEmployeeMenu::DoModal(CEmployee* _Employee, CPoint& _Point)
{
	EXT_ASSERT_POINTER(_Employee, CEmployee);
	WorldData().SetCurrentEmployee(_Employee);
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_NOTOWNEDEMPLOYEE));
	CMenu* pPopup = menu.GetSubMenu(0);
	EXT_ASSERT_POINTER(pPopup, CMenu);
	if (WorldData().GetCurrentUserClub()->NeedEmployee(_Employee->GetType(), false) != true)
	{
		pPopup->RemoveMenu(IDP_EMPLOYEEHIRE, MF_BYCOMMAND);
	}
	pPopup->TrackPopupMenu(TPM_LEFTALIGN, _Point.x, _Point.y, AfxGetMainWnd());
	AfxGetApp()->PumpMessage();
}


/*------------------------------------------------------------------------------
	Method:   	CPopupClubMenu::DoModal
	Access:    	public 
	Parameter: 	CClub * _Club
	Parameter: 	CPoint & _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPopupClubMenu::DoModal(CClub* _Club, CPoint& _Point)
{
	EXT_ASSERT_POINTER(_Club, CClub);
	WorldData().SetCurrentClub(_Club);
	CPopupMenu menu;
	menu.DoModal(IDR_CLUB, _Point.x, _Point.y + 10, AfxGetMainWnd());
}


/*------------------------------------------------------------------------------
	Method:   	CPopupClubFixtureMenu::DoModal
	Access:    	public 
	Parameter: 	CClub * _Club
	Parameter: 	CMatchInfo * _MatchInfo
	Parameter: 	CPoint & _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPopupClubFixtureMenu::DoModal(CClub* _Club, CMatchInfo* _MatchInfo, CPoint& _Point)
{
	EXT_ASSERT_POINTER(_Club, CClub);
	WorldData().SetCurrentClub(_Club);
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_CLUBFIXTURE));
	CMenu* pPopup = menu.GetSubMenu(0);
	EXT_ASSERT_POINTER(pPopup, CMenu);
	if (_MatchInfo == null)
	{
		pPopup->RemoveMenu(IDP_VIEWMATCHREPORT, MF_BYCOMMAND);
	}
	else
	{
		WorldData().SetCurrentMatchInfo(_MatchInfo);
	}
	pPopup->TrackPopupMenu(TPM_LEFTALIGN, _Point.x, _Point.y, AfxGetMainWnd());
	AfxGetApp()->PumpMessage();
}