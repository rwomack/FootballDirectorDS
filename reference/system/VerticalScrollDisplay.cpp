
/*
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	Class:- CVerticalScrollDisplay


	(c) Rw Software 1994 - 2007

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/


#include "stdafx.h"

#include "../system/Screen/Grid.h"


/*------------------------------------------------------------------------------
Method:   	CVerticalScrollDisplay::OnInitialise
Access:    	public 
Parameter: 	const int _RowsOnScreen
Parameter: 	const int _ScrollBarId
Parameter: 	const uint _RowHeight
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CVerticalScrollDisplay::OnInitialise(const int _RowsOnScreen, const uint _RowHeight, int _ScrollBarID, CRect _ListArea)
{
	m_Grid.OnInitialise();
}


/*------------------------------------------------------------------------------
Method:   	CVerticalScrollDisplay::OnVScroll
Access:    	public 
Parameter: 	const uint _SBCode
Parameter: 	const uint _Pos
Parameter: 	const CScrollBar* _pScrollBar
Returns:   	bool
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
bool CVerticalScrollDisplay::OnVScroll(const uint _SBCode, const uint _Pos, CScrollBar* _pScrollBar)
{
	m_Grid.OnVScroll(_SBCode, _Pos, _pScrollBar);
	return true;
}


/*------------------------------------------------------------------------------
Method:   	CVerticalScrollDisplay::ResetScrollBar
Access:    	public 
Parameter: 	const uint _NumItems
Parameter: 	const CScrollBar* _pScrollBar
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CVerticalScrollDisplay::ResetScrollBar(const uint _NumItems, CScrollBar* _pScrollBar)
{
	m_Grid.ResetScrollBar(_NumItems, _pScrollBar);
}


/*------------------------------------------------------------------------------
Method:   	CVerticalScrollDisplay::IsScrollBarVisible
Access:    	public const 
Parameter: 	void
Returns:   	bool
Qualifier: 	const
Purpose:
------------------------------------------------------------------------------*/
bool CVerticalScrollDisplay::IsScrollBarVisible()
{
	return m_Grid.IsScrollBarVisible();
}


/*------------------------------------------------------------------------------
Method:   	CVerticalScrollDisplay::GetFirstRowOffBottomScreen
Access:    	public const 
Parameter: 	void
Returns:   	int
Qualifier: 	const
Purpose:
------------------------------------------------------------------------------*/
int CVerticalScrollDisplay::GetFirstRowOffBottomScreen()
{
	return m_Grid.GetVerticalScroll() + m_Grid.GetNumRowsOnScreen();
}


/*------------------------------------------------------------------------------
Method:   	CVerticalScrollDisplay::GetVerticalScroll
Access:    	public const 
Parameter: 	void
Returns:   	int
Qualifier: 	const
Purpose:
------------------------------------------------------------------------------*/
int CVerticalScrollDisplay::GetVerticalScroll()
{
	return m_Grid.GetVerticalScroll();
}


/*------------------------------------------------------------------------------
Method:   	CVerticalScrollDisplay::GetNumRowsOnScreen
Access:    	public const 
Parameter: 	void
Returns:   	int
Qualifier: 	const
Purpose:
------------------------------------------------------------------------------*/
int CVerticalScrollDisplay::GetNumRowsOnScreen()
{
	return m_Grid.GetNumRowsOnScreen();
}


/*------------------------------------------------------------------------------
Method:   	CVerticalScrollDisplay::GetRowHeight
Access:    	public const 
Parameter: 	void
Returns:   	int
Qualifier: 	const
Purpose:
------------------------------------------------------------------------------*/
int CVerticalScrollDisplay::GetRowHeight()
{
	return m_Grid.GetRowHeight();
}


/*------------------------------------------------------------------------------
Method:   	CVerticalScrollDisplay::SetRowHeight
Access:    	public 
Parameter: 	const uint _RowHeight
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CVerticalScrollDisplay::SetRowHeight(const uint _Value)
{
	m_Grid.SetRowHeight(_Value);
}
