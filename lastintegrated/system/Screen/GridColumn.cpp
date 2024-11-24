
/*

	GridColumn.cpp: implementation for the Grid class.


	(c) Rw Software 2007

  ---------------------------------------------------------------
*/

#include "stdafx.h"

#include "UI.h"


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
	m_SortOrder = UNSORTED;
}


/*------------------------------------------------------------------------------
	Method:   	CGridColumn::CGridColumn
	Access:    	public 
	Parameter: 	const int _Style
	Parameter: 	const int _Width
	Parameter: 	const COLORREF _TextColour
	Parameter: 	const eSortOrder _SortOrder
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CGridColumn::CGridColumn(const int _Style, const int _Width, const COLORREF _TextColour /*= BLACK*/, const eSortOrder _SortOrder /*= UNSORTED*/)
{
	m_Style = _Style;
	m_TextColour = _TextColour;
	m_Width = _Width;
	m_SortOrder = _SortOrder;
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
	Method:   	CGridColumn::DoRemoveAllData
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridColumn::DoRemoveAllData()
{
	for (int CurrentRow = 0; CurrentRow < GetRowCount(); CurrentRow++)
	{
		CGridRow *pRow = m_Rows.GetAt(CurrentRow);
		EXT_ASSERT_POINTER(pRow, CGridRow);
		delete pRow;
	}
	m_Rows.RemoveAll();
}


/*------------------------------------------------------------------------------
	Method:   	CGridColumn::GetRow
	Access:    	public 
	Parameter: 	int _RowID
	Returns:   	CGridRow*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CGridRow* CGridColumn::GetRow(int _RowID)
{
	return (CGridRow*)m_Rows.GetAt(_RowID);
}


/*------------------------------------------------------------------------------
	Method:   	CGridColumn::GetRowCount
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CGridColumn::GetRowCount()
{
	return m_Rows.GetSize();
}


/*------------------------------------------------------------------------------
	Method:   	CGridColumn::GetSortOrder
	Access:    	public 
	Returns:   	eSortOrder
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eSortOrder CGridColumn::GetSortOrder()
{
	return m_SortOrder;
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
	Method:   	CGridColumn::SetSortOrder
	Access:    	public 
	Parameter: 	const eSortOrder _SortOrder
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridColumn::SetSortOrder(const eSortOrder _SortOrder /*= UNSORTED*/)
{
	m_SortOrder = _SortOrder;
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
/////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CGridColumnList::DoAddColumn
	Access:    	public 
	Parameter: 	int _Width
	Parameter: 	int _Style
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGridColumnList::DoAddColumn(int _Width /*= -1*/, int _Style /*= COL_NORMAL*/)
{
	CGridColumn *pCol = new CGridColumn(_Style, _Width);
	EXT_ASSERT_POINTER(pCol, CGridColumn);
	m_Columns.Add(pCol);
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CGridColumnList::DoAddItem
	Access:    	public
	Parameter: 	CString _Text
	Parameter: 	void * _Pointer
	Parameter: 	ePointerType _PointerType
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridColumnList::DoAddItem(CString _Text, void* _Pointer, ePointerType _PointerType /*= MAX_POINTER*/)
{
	// add new row for all columns.
	for (int LoopCount = 0; LoopCount < m_Columns.GetSize(); LoopCount++)
	{
		CGridColumn* pCol = GetAt(LoopCount);
		CGridRow* pRow = new CGridRow();
		EXT_ASSERT_POINTER(pRow, CGridRow);
		pRow->DoCreate(_Text, _Pointer, _PointerType);
		pCol->m_Rows.Add(pRow);
	}
}


/*------------------------------------------------------------------------------
Method:   	CGridColumnList::DoAddRow
Access:    	public 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CGridColumnList::DoAddRow()
{
	// add new row for all columns.
	for (int LoopCount = 0; LoopCount < m_Columns.GetSize(); LoopCount++)
	{
		CGridColumn* pCol = GetAt(LoopCount);
		CGridRow* pRow = new CGridRow();
		EXT_ASSERT_POINTER(pRow, CGridRow);
		pRow->DoCreate();
		pCol->m_Rows.Add(pRow);
	}
	DEBUG_OUTPUT(("Adding %d rows"), (GetCount()));
}


/*------------------------------------------------------------------------------
Method:   	CGridColumnList::DoAddRows
Access:    	public 
Parameter: 	int _HowMany
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CGridColumnList::DoAddRows(int _HowMany)
{
	EXT_ASSERT(m_Columns.GetSize() > 0);
	for (int RowCount = 0; RowCount < _HowMany; RowCount++)
	{
		// add new row for all columns.
		for (int LoopCount = 0; LoopCount < m_Columns.GetSize(); LoopCount++)
		{
			CGridColumn* pCol = GetAt(LoopCount);
			CGridRow* pRow = new CGridRow();
			EXT_ASSERT_POINTER(pRow, CGridRow);
			pRow->DoCreate();
			pCol->m_Rows.Add(pRow);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGridColumnList::DisableRow
	Access:    	public 
	Parameter: 	int _RowID
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridColumnList::DisableRow(int _RowID, bool _Value)
{
	for (int LoopCount = 0; LoopCount < m_Columns.GetSize(); LoopCount++)
	{
		CGridColumn *pCol = GetAt(LoopCount);
		EXT_ASSERT_POINTER(pCol, CGridColumn);
		pCol->GetRow(_RowID)->SetDisabled(_Value);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGridColumnList::GetCount
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CGridColumnList::GetCount() 
{ 
	return m_Columns.GetCount(); 
}


/*------------------------------------------------------------------------------
	Method:   	CGridColumnList::GetAt
	Access:    	public 
	Parameter: 	int _Element
	Returns:   	CGridColumn*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CGridColumn* CGridColumnList::GetAt(int _Element) 
{ 
	EXT_ASSERT(_Element < m_Columns.GetCount());
	return (CGridColumn*)m_Columns.GetAt(_Element); 
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoRemoveAllData
	Access:    	public 
	Parameter: 	bool _Columns
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGridColumnList::DoRemoveAllData(bool _Columns /*= false*/)
{
	// clean up the data.
	for (int CurrentColumn = 0; CurrentColumn < m_Columns.GetCount(); CurrentColumn++)
	{
		CGridColumn *pCol = GetAt(CurrentColumn);
		if (pCol != NULL)
		{
			pCol->DoRemoveAllData();
			m_Columns.SetAt(CurrentColumn, NULL);
		}
		if (_Columns)
		{
			delete pCol;
		}
	}
	m_Columns.RemoveAll();
}


/*------------------------------------------------------------------------------
Method:   	CGridColumn::GetRowCount
Access:    	public 
Returns:   	int
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
int	CGridColumnList::GetRowCount()
{
	if (GetCount() > 0)
	{
		// All columns should have the same number of rows
		return GetAt(0)->GetRowCount();
	}
	return 0;
}
