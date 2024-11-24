
/*

	PlayerSelector.cpp: implementation for the PlayerSelector class.


	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "PlayerSelector.h"


/*------------------------------------------------------------------------------
	Method:   	CPlayerSelector::CPlayerSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerSelector::CPlayerSelector(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSelector::~CPlayerSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerSelector::~CPlayerSelector(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSelector::DoInitialise
	Access:    	public 
	Parameter: 	CPlayerSquad & _Squad
	Parameter: 	CWnd * _Wnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	int _ButtonDownID
	Parameter: 	int _ButtonUpID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSelector::DoInitialise(int _CurSel, CPlayerSquad& _Squad, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownID, int _ButtonUpID)
{
	CTextSelector::DoInitialise(_Wnd, _BasicDisplay, _ButtonDownID, _ButtonUpID);
	for (uint LoopCount = 0; LoopCount < PLAYERSINSQUAD; LoopCount ++)
	{
		if (_Squad.GetSquad(LoopCount) != NOPLAYER /*&& _Squad.GetPlayer(LoopCount).IsInjured() == false*/)
		{
// 			DEBUG_OUTPUT(("%d, Adding %s"), (LoopCount, _Squad.GetPlayer(LoopCount).GetName()));
			CTextSelector::DoAddItem(_Squad.GetPlayer(LoopCount).GetName(), &_Squad.GetPlayer(LoopCount));
		}
	}
	SetCurSel(min(_CurSel, CTextSelector::GetCount()));
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSelector::GetCurrentPlayer
	Access:    	public 
	Returns:   	CPlayer*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayer* CPlayerSelector::GetCurrentPlayer()
{
	return (CPlayer*)CTextSelector::GetCurPointer();
}
