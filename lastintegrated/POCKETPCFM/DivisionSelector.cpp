
/*

	DivisionSelector.cpp: implementation for the DivisionSelector class.


	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "DivisionSelector.h"



/*------------------------------------------------------------------------------
	Method:   	CDivisionSelector::CDivisionSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivisionSelector::CDivisionSelector(void)
{
	m_LeftX = NOTFOUND;
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelector::~CDivisionSelector
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivisionSelector::~CDivisionSelector(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelector::DoInitialise
	Access:    	public 
	Parameter: 	CWnd _Wnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	int _ButtonDownID
	Parameter: 	int _ButtonUpID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionSelector::DoInitialise(CWnd* _Wnd, CBasicDisplay* _BasicDisplay, int _ButtonDownID, int _ButtonUpID)
{
	CTextSelector::DoInitialise(_Wnd, _BasicDisplay, _ButtonDownID, _ButtonUpID);
	CSortedStringList theList;
	WorldData().GetDivisionList().DoBuildNameList(theList);
	CTextSelector::DoAddItemList(theList);
 }


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelector::GetCurrentDivision
	Access:    	public 
	Returns:   	CDivision*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivision& CDivisionSelector::GetCurrentDivision()
{
	return WorldData().GetDivisionList().GetAt(CTextSelector::GetCurSel());
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelector::MoveWindow
	Access:    	public 
	Parameter: 	CRect _Rect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionSelector::MoveWindow(CRect _Rect)
{
	CRect theRect;
	CTextSelector::GetTextRegion()->GetParentWnd()->GetClientRect(&theRect);
	if (m_LeftX == NOTFOUND)
	{
		// Centre on x axis
		_Rect.left = theRect.right / 2 - COMPETITIONNAMELENGTH;
		_Rect.right = theRect.right / 2 + COMPETITIONNAMELENGTH;
	}
	else
	{
		_Rect.right = _Rect.left + COMPETITIONNAMELENGTH * 2;
	}
	CTextSelector::MoveWindow(_Rect);
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionSelector::SetLeftX
	Access:    	public 
	Parameter: 	const int _LeftX
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionSelector::SetLeftX(const int _LeftX)
{
	m_LeftX = _LeftX;
}

