
/*

	CupSelector.cpp: implementation for the CupSelector class.


	(c) Sports Director Ltd 2007 - 2008 - 2008

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "CupSelector.h"



/*------------------------------------------------------------------------------
	Method:   	CCupSelector::CCupSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupSelector::CCupSelector(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CCupSelector::~CCupSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupSelector::~CCupSelector(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CCupSelector::DoInitialise
	Access:    	public 
	Parameter: 	CWnd _Wnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	int _ButtonDownID
	Parameter: 	int _ButtonUpID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupSelector::DoInitialise(CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownID, int _ButtonUpID)
{
	CTextSelector::DoInitialise(_Wnd, _BasicDisplay, _ButtonDownID, _ButtonUpID);
	CSortedStringList theList;
	WorldData().DoBuildCupNameList(theList);
	CTextSelector::DoAddItemList(theList);
}


/*------------------------------------------------------------------------------
	Method:   	CCupSelector::GetCurrentDivision
	Access:    	public 
	Returns:   	CDivision*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCup& CCupSelector::GetCurrentCup()
{
	return *WorldData().GetCup(CTextSelector::GetCurSel());
}


/*------------------------------------------------------------------------------
	Method:   	CCupSelector::MoveWindow
	Access:    	public 
	Parameter: 	CRect _Rect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupSelector::MoveWindow(CRect _Rect)
{
	CRect theRect;
	CTextSelector::GetTextRegion()->GetParentWnd()->GetClientRect(&theRect);
	theRect.left = theRect.right / 2 - COMPETITIONNAMELENGTH;
	theRect.right = theRect.right / 2 + COMPETITIONNAMELENGTH;
	CTextSelector::MoveWindow(_Rect);
}



