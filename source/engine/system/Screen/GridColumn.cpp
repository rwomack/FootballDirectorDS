
/*

	GridColumn.cpp: implementation for the Grid class.


	(c) Rw Software 2007

  ---------------------------------------------------------------
*/

#include "stdafx.h"

#include "voimage.h"
#include "OffScreenBuffer.h"
#include "DateDisplay.h"
#include "PNGDrawInfo.h"
#include "PNGButton.h"
#include "BasicDisplay.h"
#include "SortedStringList.h"
#include "KeyScrollBar.h"
#include "GridColumn.h"
#include "Grid.h"
#include "GridRow.h"


/*------------------------------------------------------------------------------
Method:   	CColumnInfo::CColumnInfo
Access:    	public 
Returns:   	
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CGridColumn::CGridColumn()
{
	m_TextColour = TRANSCOLOUR;
	m_Style = COL_NORMAL;
	m_Width = 0;
}


/*------------------------------------------------------------------------------
Method:   	CColumnInfo::~CColumnInfo
Access:    	public 
Returns:   	
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CGridColumn::~CGridColumn()
{
}


/*------------------------------------------------------------------------------
Method:   	CColumnInfo::GetStyle
Access:    	public 
Returns:   	int
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
int CGridColumn::GetStyle()
{
	return m_Style;
}


/*------------------------------------------------------------------------------
Method:   	CColumnInfo::SetStyle
Access:    	public 
Parameter: 	int _Style
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CGridColumn::SetStyle(int _Style /*= COL_NORMAL*/)
{
	m_Style = _Style;
}


/*------------------------------------------------------------------------------
Method:   	CColumnInfo::SetWidth
Access:    	public 
Parameter: 	int _Width
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CGridColumn::SetWidth(int _Width)
{
	m_Width = _Width;
}


/*------------------------------------------------------------------------------
Method:   	CColumnInfo::GetWidth
Access:    	public 
Returns:   	int
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
int CGridColumn::GetWidth()
{
	return m_Width;
}


/*------------------------------------------------------------------------------
Method:   	CColumnInfo::SetTextColour
Access:    	public 
Parameter: 	COLORREF _TextColour
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CGridColumn::SetTextColour(COLORREF _TextColour)
{
	m_TextColour = _TextColour;
}


/*------------------------------------------------------------------------------
Method:   	CColumnInfo::GetTextColour
Access:    	public 
Returns:   	COLORREF
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
COLORREF CGridColumn::GetTextColour()
{
	return m_TextColour;
}
