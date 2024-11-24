
/*

	PopupPlayerMenu.h: header for various PopupMenu classes.


	(c) Rw Software 1994-2007

---------------------------------------------------------------
*/


#ifndef _POPUPOWNEDPLAYERMENU__H__
 #define _POPUPOWNEDPLAYERMENU__H__


class CPopupPlayerMenu
{
public:
	void					DoModal(CPlayer* _Player, CPoint& _Point, bool _ShowDetails = true);
};
/////////////////////////////////////////////////////////////////////////////

class CPopupOwnedPlayerMenu
{
public:
	void					DoModal(CPlayer* _Player, CPoint& _Point, bool _ShowDetails = true);
};
/////////////////////////////////////////////////////////////////////////////

class CPopupOutOfContractPlayerMenu
{
public:
	void					DoModal(CPlayer* _Player, CPoint& _Point, bool _ShowDetails = true);
};
/////////////////////////////////////////////////////////////////////////////

class CPopupNotOwnedPlayerMenu
{
public:
	void					DoModal(CPlayer* _Player, CPoint& _Point, bool _ShowDetails = true);
};
/////////////////////////////////////////////////////////////////////////////

class CPopupOwnedYouthMenu
{
public:
	void					DoModal(CYouth* _Youth, CPoint& _Point);
};
/////////////////////////////////////////////////////////////////////////////

class CPopupNotOwnedYouthMenu
{
public:
	void					DoModal(CYouth* _Youth, CPoint& _Point);
};
/////////////////////////////////////////////////////////////////////////////

class CPopupOwnedEmployeeMenu
{
public:
	void					DoModal(CEmployee* _Employee, CPoint& _Point);
};
/////////////////////////////////////////////////////////////////////////////

class CPopupNotOwnedEmployeeMenu
{
public:
	void					DoModal(CEmployee* _Employee, CPoint& _Point);
};
/////////////////////////////////////////////////////////////////////////////

class CPopupClubMenu
{
public:
	void					DoModal(CClub* _Club, CPoint& _Point);
};
/////////////////////////////////////////////////////////////////////////////

class CPopupClubFixtureMenu
{
public:
	void					DoModal(CClub* _Club, CMatchInfo* _MatchInfo, CPoint& _Point);
};
#endif
