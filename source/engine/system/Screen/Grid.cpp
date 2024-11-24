
/*

	Grid.cpp: implementation for the Grid class.


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
	Method:   	CGrid::CGrid
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CGrid::CGrid()
{
	m_BasicDisplay = NULL;
	m_NumCols = 0;
	m_NumRows = 0;

	m_VerticalScroll = 0;
	m_MaxScroll = 0;
	m_NumRowsOnScreen = 0;
	m_RowHeight = 0;
	m_ScrollBarWidth = 12;

	m_Style = LC_NORMAL;
	m_Font = null;
	m_ParentWnd = null;
	m_SelectedItem = NOTFOUND;
	m_LastSelectedItem  = NOTFOUND;
	m_ClickedItem = NOTFOUND;
	m_ScrollBarVisible  = true;
	m_ColumnGap = 0;
	m_TotalRowSize = 0;
	m_NumToShow	= NOTFOUND;
	m_Clicked = false;
	m_NumChanged = true;

	// default colours.
	m_ClickedColour	= RED;
	m_ClickedBackgroundColour = GREEN;
	SetBackgroundColours(RGB(0xf5, 0xce, 0xaf)/*PASTELORANGE*/, RGB(0xf7, 0xb2, 0x7b)/*PALEORANGE*/, VDARKORANGE);
	SetHiliteColour(WHITE);
	SetTextColour(BLACK);
	//DoAddColumn();

	m_Hilite = true;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::OnInitialUpdate
	Access:    	public
	Parameter: 	CWnd * _ParentWnd
	Parameter: 	const uint _RowHeight
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::OnInitialUpdate(CWnd* _ParentWnd, CBasicDisplay* _BasicDisplay, const int _Style /*= LC_NORMAL*/, const uint _RowHeight /*= GAPY*/)
{
  FDASSERT_POINTER(_ParentWnd, CWnd);
  FDASSERT_POINTER(_BasicDisplay, CBasicDisplay);
  FDASSERT(_RowHeight > 0);
	m_ScrollBar.Create(WS_CHILD | SBS_VERT, m_ScrollRect, _ParentWnd, 100);
	m_Style = _Style;
	m_RowHeight = _RowHeight;
	m_ParentWnd = _ParentWnd;
	m_BasicDisplay = _BasicDisplay;
	RemoveAllData(true);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetRowHeight
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::GetRowHeight()
{
	return m_RowHeight;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::CountRows
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::CountRows()
{
	return m_NumRows;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::CountItems
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::CountItems()
{
	return CountRows ();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::CountColumns
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::CountColumns()
{
	return m_NumCols;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetClickedItem
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::GetClickedItem()
{
	return m_ClickedItem;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetClickedItem
	Access:    	public
	Parameter: 	int value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetClickedItem(int value /*= -1*/)
{
	m_ClickedItem = value;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetScrollBarWidth
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::GetScrollBarWidth()
{
	return m_ScrollBarWidth;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::~CGrid
	Access:    	virtual public
	Returns:   		Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CGrid::~CGrid()
{
	RemoveAllData(true, false);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetScrollBar
	Access:    	protected
	Returns:   	CScrollBar*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CScrollBar* CGrid::GetScrollBar()
{
	return &m_ScrollBar;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetBasicDisplay
	Access:    	public
	Returns:   	CBasicDisplay*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CBasicDisplay* CGrid::GetBasicDisplay()
{
  FDASSERT_POINTER(m_BasicDisplay, CBasicDisplay);
	return m_BasicDisplay;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::OnInitialise
	Access:    	public
	Parameter: 	const int _RowsOnScreen
	Parameter: 	CScrollBar * pScrollBar
	Parameter: 	CRect _ListArea
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::OnSize(const int _RowsOnScreen, CRect _ListArea)
{
	/*wchar_t str[256];
	wsprintf(str, _T("Grid Left %d, Right %d\n"), _ListArea.left, _ListArea.right);
	OutputDebugString(str);*/
	m_NumRowsOnScreen = (_ListArea.bottom - _ListArea.top) / GetRowHeight(); //_RowsOnScreen;
	m_WindowRect = _ListArea;
	DoInitialiseScrollBar(true);
	m_VerticalScroll = min(m_VerticalScroll, m_MaxScroll);	// Height may be taller
	if (m_NumCols == 1)
	{
		// Only 1 column so ensure fills width of grid
		SetColumnWidth(0, _ListArea.right);
	}
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetStyle
	Access:    	public
	Parameter: 	int _Style
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetStyle(int _Style)
{
	m_Style = _Style;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::IsScrollBarVisible
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::IsScrollBarVisible()
{
	return m_ScrollBarIsVisible;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetHiliteColour
	Access:    	public
	Parameter: 	COLORREF _hilitecol
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetHiliteColour(COLORREF _hilitecol)
{
	m_HiliteColour = _hilitecol;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetClickedColour
	Access:    	public
	Parameter: 	COLORREF _ClickedColour
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetClickedColour(COLORREF _ClickedColour)
{
	m_ClickedColour = _ClickedColour;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetTextColour
	Access:    	public
	Parameter: 	COLORREF _TextColour
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetTextColour(COLORREF _TextColour)
{
	m_TextColour = _TextColour;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetHilite
	Access:    	public
	Parameter: 	bool _Hilite
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetHilite(bool _Hilite /*= true*/)
{
	m_Hilite = _Hilite;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetBackgroundColours
	Access:    	public
	Parameter: 	COLORREF _BackgroundColour1
	Parameter: 	COLORREF _BackgroundColour2
	Parameter: 	COLORREF _HiliteBackgroundColour
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetBackgroundColours(COLORREF _BackgroundColour1, COLORREF _BackgroundColour2, COLORREF _HiliteBackgroundColour)
{
	m_BackgroundColour1 = _BackgroundColour1;
	m_BackgroundColour2 = _BackgroundColour2;
	m_HiliteBackgroundColour= _HiliteBackgroundColour;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetBackgroundColour1
	Access:    	public
	Parameter: 	COLORREF _BackgroundColour1
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetBackgroundColour1(COLORREF _BackgroundColour1)
{
	m_BackgroundColour1 = _BackgroundColour1;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetBackgroundColour2
	Access:    	public
	Parameter: 	COLORREF _BackgroundColour2
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetBackgroundColour2(COLORREF _BackgroundColour2)
{
	m_BackgroundColour2 = _BackgroundColour2;
};


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetHiliteBackgroundColour
	Access:    	public
	Parameter: 	COLORREF _HiliteBackgroundColour
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetHiliteBackgroundColour(COLORREF _HiliteBackgroundColour)
{
	m_HiliteBackgroundColour = _HiliteBackgroundColour;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetClickedBackgroundColour
	Access:    	public
	Parameter: 	COLORREF _ClickedBackgroundColour
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetClickedBackgroundColour(COLORREF _ClickedBackgroundColour)
{
	m_ClickedBackgroundColour = _ClickedBackgroundColour;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetNumRowsOnScreen
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int	CGrid::GetNumRowsOnScreen()
{
	return m_NumRowsOnScreen;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::IsWindowVisible
	Access:    	protected
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::IsWindowVisible()
{
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoAddColumn
	Access:    	public
	Parameter: 	int _Width
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::DoAddColumn(int _Width /*= -1*/, int _Style /*= COL_NORMAL/*, CString _BitmapFile*/)
{
	if (_Width == -1)
	{
		_Width = m_WindowRect.right - m_WindowRect.left;
	}
/*	int Width = 0;
	for (int LoopCount = 0; LoopCount < m_Data.GetCount(); LoopCount++)
	{
		Width += GetColumn(LoopCount)->GetWidth();
	}
  FDASSERT(Width + _Width < m_WindowRect.right - m_WindowRect.left);*/
	CGridColumn *pCol = FD_ENGINE_NEW CGridColumn();
  FDASSERT_POINTER(pCol, CGridColumn);
	m_Data.Add(pCol);
	pCol->SetWidth(_Width);
	pCol->SetStyle(_Style);
	m_NumCols++;
	m_NumChanged = true;
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetColumn
	Access:    	public
	Parameter: 	int _ColumnID
	Returns:   	CColumnInfo*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CGridColumn* CGrid::GetColumn(int _ColumnID)
{
  FDASSERT(_ColumnID < m_Data.GetCount());
	CGridColumn* pCol = (CGridColumn*)m_Data.GetAt(_ColumnID);
	return pCol;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetColumnWidth
	Access:    	public
	Parameter: 	int _ColumnID
	Parameter: 	int _Width
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::SetColumnWidth(int _ColumnID, int _Width)
{
	GetColumn(_ColumnID)->SetWidth(_Width);
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetColumnWidth
	Access:    	public
	Parameter: 	int _ColumnID
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::GetColumnWidth(int _ColumnID)
{
	// If only 1 column then column is full width
	if (m_Data.GetCount() == 1)
	{
    FDASSERT(m_WindowRect.right > 0);
		return m_WindowRect.right - m_WindowRect.left;
	}
	return GetColumn(_ColumnID)->GetWidth();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetColumnStyle
	Access:    	protected
	Parameter: 	int _ColumnID
	Returns:   	eColumnStyle
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::GetColumnStyle(int _ColumnID)
{
	return GetColumn(_ColumnID)->GetStyle();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetColumnAlignment
	Access:    	protected
	Parameter: 	int _ColumnID
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::GetColumnAlignment(int _ColumnID)
{
	if (GetColumnStyle(_ColumnID) == COL_MONEY)
	{
		return COL_ALIGNR;
	}
	return GetColumn(_ColumnID)->GetStyle() & COL_ALIGNMENT;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetColumnTextColour
	Access:    	public
	Parameter: 	int _ColumnID
	Parameter: 	COLORREF _Colour
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::SetColumnTextColour(int _ColumnID, COLORREF _Colour)
{
	GetColumn(_ColumnID)->SetTextColour(_Colour);
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetColumnTextColour
	Access:    	protected
	Parameter: 	int _ColumnID
	Returns:   	COLORREF
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
COLORREF CGrid::GetColumnTextColour(int _ColumnID)
{
	return GetColumn(_ColumnID)->GetTextColour();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetItemColour
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Parameter: 	COLORREF _Colour
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::SetItemBackgroundColour(int _RowID, int _ColumnID, COLORREF _Colour)
{
	return GetCell(_ColumnID, _RowID)->SetBackgroundColour(_Colour);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetRowColour
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	COLORREF _Colour
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetRowBackgroundColour(int _RowID, COLORREF _Colour)
{
	// get column
	for (int LoopCount = 0; LoopCount < m_NumCols; LoopCount++)
	{
		GetCell(LoopCount, _RowID)->SetBackgroundColour(_Colour);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetRowTextColour
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	COLORREF _Colour
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetRowTextColour(int _RowID, COLORREF _Colour)
{
	// get column
	for (int LoopCount = 0; LoopCount < m_NumCols; LoopCount++)
	{
		GetCell(LoopCount, _RowID)->SetTextColour(_Colour);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetRowColour
	Access:    	protected
	Parameter: 	int _RowID
	Returns:   	COLORREF
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
COLORREF CGrid::GetRowBackgroundColour(int _RowID)
{
	return GetCell(0, _RowID)->GetBackgroundColour();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetRowPointerDetails
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	void * _Pointer
	Parameter: 	int _PointerType
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetRowPointerDetails(int _RowID, void* _Pointer, ePointerType _PointerType /*= MAX_POINTER*/)
{
	GetCell(0, _RowID)->SetPointerDetails(_Pointer, _PointerType);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetRowPointer
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	void * _Pointer
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetRowPointer(int _RowID, void* _Pointer)
{
	GetCell(0, _RowID)->SetPointer(_Pointer);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetRowPointer
	Access:    	public
	Parameter: 	int _RowID
	Returns:   	void*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void* CGrid::GetRowPointer(int _RowID)
{
	return GetCell(0, _RowID)->GetPointer();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetRowPointerType
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	ePointerType _PointerType
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetRowPointerType(int _RowID, ePointerType _PointerType /*= MAX_POINTER*/)
{
	GetCell(0, _RowID)->SetPointerType(_PointerType);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetRow
	Access:    	public
	Parameter: 	int _ColumnID
	Parameter: 	int _RowID
	Returns:   	CRowInfo*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CGridRow* CGrid::GetCell(int _ColumnID, int _RowID)
{
  FDASSERT/*MSG*/(/*_T("RowID out of range"), */_RowID >= 0 && _RowID < m_NumRows);
  FDASSERT/*MSG*/(/*_T("ColumnID out of range"),*/ _ColumnID >= 0 && _ColumnID < m_NumCols);
	CGridColumn *pCol = (CGridColumn*)m_Data.GetAt(_ColumnID);
	CGridRow* pRow = (CGridRow*)pCol->m_Rows.GetAt(_RowID);
	return pRow;
}



/*------------------------------------------------------------------------------
	Method:   	CGrid::GetRowPointerType
	Access:    	public
	Parameter: 	int _RowId
	Returns:   	ePointerType
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ePointerType CGrid::GetRowPointerType(int _RowId)
{
	return GetCell(0, _RowId)->GetPointerType();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoAddItem
	Access:    	public
	Parameter: 	CString _Text
	Parameter: 	void * _Pointer
	Parameter: 	ePointerType _PointerType
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::DoAddItem(CString _Text, void* _Pointer, ePointerType _PointerType /*= MAX_POINTER*/)
{
	m_NumChanged = true;

	// add new row for all columns.
	for (int LoopCount = 0; LoopCount < m_NumCols; LoopCount++)
	{
		CGridColumn* pCol = (CGridColumn*)m_Data.GetAt(LoopCount);

		CGridRow* pRow = FD_ENGINE_NEW CGridRow();
		pRow->DoCreate(_Text, _Pointer, _PointerType);
		pCol->m_Rows.Add(pRow);
	}
	m_NumRows++;
	return m_NumRows - 1;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoAddRow
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::DoAddRow()
{
	m_NumChanged = true;
	CString Str;
	// add new row for all columns.
	for (int LoopCount = 0; LoopCount < m_NumCols; LoopCount++)
	{
		CGridColumn* pCol = (CGridColumn*)m_Data.GetAt(LoopCount);
		CGridRow* pRow = FD_ENGINE_NEW CGridRow();
		pRow->DoCreate(Str);
		pCol->m_Rows.Add(pRow);
	}
	m_NumRows++;
	return m_NumRows - 1;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoAddRows
	Access:    	public
	Parameter: 	int _HowMany
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::DoAddRows(int _HowMany)
{
	m_NumChanged = true;
	CString Str;
	for (int RowCount = 0; RowCount < _HowMany; RowCount++)
	{
		// add new row for all columns.
		for (int LoopCount = 0; LoopCount < m_NumCols; LoopCount++)
		{
			CGridColumn* pCol = (CGridColumn*)m_Data.GetAt(LoopCount);

			CGridRow* pRow = FD_ENGINE_NEW CGridRow();
			pRow->DoCreate(Str);
			pCol->m_Rows.Add(pRow);
		}
		m_NumRows++;
	}
	ResetScrollBar();
	return m_NumRows - 1;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoAddItemList
	Access:    	public
	Parameter: 	CSortedStringList & _SortedTextList
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::DoAddItemList(CSortedStringList& _SortedTextList)
{
	for (int LoopCount = 0; LoopCount < _SortedTextList.GetSize(); LoopCount++)
	{
		DoAddItem(_SortedTextList.GetSortedStr(LoopCount));
	}
	return m_NumRows - 1;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetItemPointerDetails
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Parameter: 	void * _Pointer
	Parameter: 	int _PointerType
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetItemPointerDetails(int _RowID, int _ColumnID, void* _Pointer, ePointerType _PointerType /*= MAX_POINTER*/)
{
	GetCell(_ColumnID, _RowID)->SetPointerDetails(_Pointer, _PointerType);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetCellTextColor
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Parameter: 	COLORREF _Colour
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::SetItemTextColour(int _RowID, int _ColumnID, COLORREF _Colour)
{
	return GetCell(_ColumnID, _RowID)->SetTextColour(_Colour);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetItemText
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Parameter: 	LPCTSTR _Text
	Parameter: 	void * pointer
	Parameter: 	ePointerType pointertype
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::SetItemText(int _RowID, int _ColumnID, LPCTSTR _Text /*= NULL*/, void* _Pointer /*= NULL*/, ePointerType _PointerType /*= MAX_POINTER*/)
{
	return GetCell(_ColumnID, _RowID)->SetText(_Text, _Pointer, _PointerType);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetItemStrId
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Parameter: 	uint _StrID
	Parameter: 	void * _Pointer
	Parameter: 	ePointerType _PointerType
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::SetItemStrId(int _RowID, int _ColumnID, uint _StrID, void* _Pointer /*= NULL*/, ePointerType _PointerType /*= MAX_POINTER*/)
{
	CString str;
  LoadText( str, _StrID );
	return SetItemText(_RowID, _ColumnID, str, _Pointer, _PointerType);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetItemNum
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Parameter: 	int _NumText
	Parameter: 	void * _Pointer
	Parameter: 	ePointerType _PointerType
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::SetItemNum(int _RowID, int _ColumnID, int _NumText /*= 0*/, void* _Pointer /*= NULL*/, ePointerType _PointerType /*= MAXPOINTER*/)
{
	CString str;
	str.Format(_T("%d"),_NumText);
	return SetItemText(_RowID, _ColumnID, str, _Pointer, _PointerType);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetItemBitmap
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Parameter: 	CPNGDrawInfo * _Bitmap
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetItemBitmap(int _RowID, int _ColumnID, CPNGDrawInfo* _Bitmap)
{
	GetCell(_ColumnID, _RowID)->SetBitmap(_Bitmap);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetItemPointer
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Returns:   	void*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void* CGrid::GetItemPointer(int _RowID, int _ColumnID)
{
	return GetCell(_ColumnID, _RowID)->GetPointer();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetItemBitmap
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Returns:   	CPNGDrawInfo*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPNGDrawInfo* CGrid::GetItemBitmap(int _RowID, int _ColumnID)
{
	return GetCell(_ColumnID, _RowID)->GetBitmap();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetItemText
	Access:    	public
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CGrid::GetItemText(int _RowID, int _ColumnID)
{
	return GetCell(_ColumnID, _RowID)->GetText();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetItemTextColour
	Access:    	protected
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Returns:   	COLORREF
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
COLORREF CGrid::GetItemTextColour(int _RowID, int _ColumnID)
{
	return GetCell(_ColumnID, _RowID)->GetTextColour();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetTextFont
	Access:    	public
	Parameter: 	CFont * _Font
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetTextFont(CFont* _Font)
{
	m_Font = _Font;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::RemoveRow
	Access:    	public
	Parameter: 	int _RowID - Remove the row with index from the listcontrol.
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::RemoveRow(int _RowID)
{
	for (int LoopCount = 0; LoopCount < m_NumCols; LoopCount++)
	{
		CGridColumn *pCol = (CGridColumn*)m_Data.GetAt(LoopCount);
		CGridRow *pRow = (CGridRow*)pCol->m_Rows.GetAt(_RowID);
		pCol->m_Rows.RemoveAt(_RowID);
		delete pRow;
	}
	m_NumRows--;
	m_NumChanged = true;
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::RemoveAllData
	Access:    	public
	Parameter: 	bool Columns
	Parameter: 	bool scrollbar
	Returns:   	void
	Qualifier:
	Purpose:	Remove all rows and data within them
------------------------------------------------------------------------------*/
void CGrid::RemoveAllData(bool Columns, bool scrollbar)
{
	// clean up the data.
	for (int CurrentColumn = 0; CurrentColumn < m_NumCols; CurrentColumn++)
	{
		CGridColumn *pCol = (CGridColumn*)m_Data.GetAt(CurrentColumn);
		for (int CurrentRow = 0; CurrentRow < m_NumRows; CurrentRow++)
		{
			CGridRow *pRow = (CGridRow*)pCol->m_Rows.GetAt(CurrentRow);
			delete pRow;
		}

		pCol->m_Rows.RemoveAll();

		if (Columns)
		{
			delete pCol;
		}
	}

	m_Data.RemoveAll();
	m_NumCols = 0;
	m_NumRows = 0;
	m_SelectedItem = m_LastSelectedItem = NOTFOUND;
	m_ClickedItem = NOTFOUND;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::RemoveAll
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::RemoveAll()
{
	RemoveAllData(false,true);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::ResetScrollBar
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::ResetScrollBar()
{
	m_VerticalScroll = 0;
	DoInitialiseScrollBar(false);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoInitialiseScrollBar
	Access:    	protected
	Parameter: 	bool _AdjustForNoScrollBar
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::DoInitialiseScrollBar(bool _AdjustForNoScrollBar)
{
	if (m_ParentWnd != null)
	{
		m_MaxScroll = max(0, m_NumRows - m_NumRowsOnScreen);
		if (m_MaxScroll > 0)
		{
			m_ScrollRect.top = m_WindowRect.top - 1;
			m_ScrollRect.bottom = m_WindowRect.bottom + 1;
			m_ScrollRect.left = m_WindowRect.right;
			m_ScrollRect.right = m_ScrollRect.left + m_ScrollBarWidth;
			m_ScrollBar.SetScrollRange(0, m_MaxScroll);
			m_ScrollBar.SetScrollPos(m_VerticalScroll);
			m_ScrollBar.MoveWindow(&m_ScrollRect, TRUE);
			m_ScrollBar.ShowWindow(SW_SHOW);
			m_ScrollBarIsVisible = true;
		}
		else
		{
			if (_AdjustForNoScrollBar == true)
			{
				//m_WindowRect.left += m_ScrollBarWidth / 2;
				//m_WindowRect.right += m_ScrollBarWidth / 2;
			}
			m_ScrollBar.ShowWindow(SW_HIDE);
			m_ScrollBarIsVisible = false;
		}
		ForcePaint();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetScrollPos
	Access:    	public
	Parameter: 	int position
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetScrollPos(int position)
{
	if (position < m_NumRows)
	{
		m_ScrollBarVisible = false;
	}
	m_NumChanged = true;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetCurrSel
	Access:    	public
	Parameter: 	CPoint _Point
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::GetCurrSel(CPoint _Point)
{
	int SelectedRow = HitTestRow(_Point);
	if (SelectedRow >= 0)
	{
		SetSelectedRow(SelectedRow);
		if (m_Style & LC_SWAPPABLECOLUMNS)
		{
			SetSwap();
		}
		return m_SelectedItem;
	}
	return NOTFOUND;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetSwap
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetSwap()
{
  FDASSERT(m_Style & LC_SWAPPABLECOLUMNS);
	if (m_LastSelectedItem == NOTFOUND)
	{
		// This is first row clicked
		m_LastSelectedItem = m_SelectedItem;                   // First player
	}
	else if (m_LastSelectedItem == m_SelectedItem)
	{
		// Clicked on same row
		m_LastSelectedItem = m_SelectedItem = NOTFOUND;
	}
	else
	{
		// This is second player clicked
		m_ClickedItem = m_SelectedItem;                   // Second player
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetLastSelectedItem
	Access:    	protected
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::GetLastSelectedItem()
{
  FDASSERT(m_Style & LC_SWAPPABLECOLUMNS);
	return m_LastSelectedItem;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::IsSwapRows
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::IsSwapRows()
{
  FDASSERT(m_Style & LC_SWAPPABLECOLUMNS);
	if (m_ClickedItem == m_SelectedItem)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoSwapRows
	Access:    	protected
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::DoSwapRows()
{
  FDASSERT(m_Style & LC_SWAPPABLECOLUMNS);
	m_LastSelectedItem = m_SelectedItem = m_ClickedItem = NOTFOUND;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::HitTestRow
	Access:    	protected
	Parameter: 	CPoint _Point
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::HitTestRow(CPoint _Point)
{
	if (!m_WindowRect.PtInRect(_Point))
	{
		// We're not even in the data area, so forget it.
		return NOTFOUND;
	}

	_Point.y -= m_WindowRect.top;
	// Find out which row we're on.
	int RowNum = (_Point.y / m_RowHeight) + m_VerticalScroll;

	if (RowNum > (m_NumRows - 1))
	{
		return NOTFOUND;
	}
	else
	{
		return RowNum;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::HitTestCol
	Access:    	protected
	Parameter: 	CPoint _Point
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::HitTestCol(CPoint _Point)
{
	CRect DataRect;
	// We're not even in the data area, so forget it.
	DataRect = m_WindowRect;

	if (!DataRect.PtInRect(_Point))
	{
		return NOTFOUND;
	}

	// Check all columns
	int col = 0;
	int offset = 0;
	while (col < m_NumCols)
	{
		CGridColumn *pCol = (CGridColumn*)m_Data.GetAt(col);
		if ( (_Point.x > offset) && (_Point.x < offset + pCol->GetWidth() + m_ColumnGap) )
		{
			return col;
		}
		col++;
		offset += pCol->GetWidth() + m_ColumnGap;
	}
	return NOTFOUND;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetSelectedRow
	Access:    	public
	Parameter: 	int _RowID
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::SetSelectedRow(int _RowID)
{
	if (m_SelectedItem != _RowID)
	{
		m_LastSelectedItem = m_SelectedItem;
		m_SelectedItem = _RowID;
		ReDraw();
		return true;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::RemoveSelection
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::RemoveSelection()
{
	m_SelectedItem = NOTFOUND;
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::ForcePaint
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::ForcePaint()
{
	if ((m_NumCols <= 0) || (m_NumRows <= 0))
	{
		return;
	}

	if (!IsWindowVisible())
	{
		return;
	}

	// Force List Control Redraw.
	ReDraw();
}


/*------------------------------------------------------------------------------
	Method:   	CScreen04::ReDraw
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::ReDraw()
{
  FDASSERT_POINTER(m_ParentWnd, CWnd);
	m_ParentWnd->InvalidateRect(m_WindowRect, FALSE);
};


/*------------------------------------------------------------------------------
	Method:   	CGrid::OnPaint
	Access:    	public
	Parameter: 	COffScreenBuffer & _memDC
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::OnPaint()
{
	if (/*!IsWindowVisible() && !IsWindowEnabled() ||*/ m_NumRows == 0)
	{
		return;
	}

	COffScreenBuffer* memDC =  GetBasicDisplay()->GetOffScreenBuffer();
	// Draw outline around the list
	CPen Scorepen(PS_SOLID, 1, BLACK);
	CPen* old = memDC->SelectObject(&Scorepen);
	memDC->Rectangle(m_WindowRect.left - 1, m_WindowRect.top - 1, m_WindowRect.right + 1, m_WindowRect.bottom + 1);
	memDC->SelectObject(old);

	if (m_Font == null)
	{
		GetBasicDisplay()->SetFontNormal();
	}
	else
	{
		GetBasicDisplay()->SetFont(*m_Font);
	}
	COLORREF ColourStripe;
	CRect DataRect = m_WindowRect;
	CString Str;
	int LeftX;
	for (int CurrentRow = m_VerticalScroll; CurrentRow < m_VerticalScroll + GetNumRowsOnScreen(); CurrentRow++)
	{
		DataRect.bottom = DataRect.top + m_RowHeight;
		if (CurrentRow == m_SelectedItem && m_Style != LC_NOHIGHLIGHT)
		{
			ColourStripe = m_HiliteBackgroundColour;
		}
		else if (CurrentRow < m_NumRows && GetRowBackgroundColour(CurrentRow) != TRANSCOLOUR)
		{
			ColourStripe = GetRowBackgroundColour(CurrentRow);
		}
		else if (CurrentRow % 2 == 1)
		{
			ColourStripe = m_BackgroundColour1;
		}
		else
		{
			ColourStripe = m_BackgroundColour2;
		}
		if (ColourStripe != TRANSCOLOUR)
		{
			memDC->FillSolidRect(DataRect.left, DataRect.top, DataRect.right - DataRect.left, m_RowHeight, ColourStripe);
		}

		if (CurrentRow < m_NumRows)
		{
			int OrigLeftX;
			for (int CurrentColumn = 0; CurrentColumn < m_NumCols; CurrentColumn++)
			{
				OrigLeftX = DataRect.left;
				if (GetItemBitmap(CurrentRow, CurrentColumn) != NULL)
				{
					DataRect.left += GetColumnWidth(CurrentColumn) / 2 - GetItemBitmap(CurrentRow, CurrentColumn)->GetWidth() / 2;
					GetItemBitmap(CurrentRow, CurrentColumn)->DoDrawTransparent(memDC->operator HDC(), DataRect.left + 2, DataRect.top + 2);
				}
				else
				{
					if (CurrentRow == m_SelectedItem && (m_Style & LC_NOHIGHLIGHT) == 0)
					{
						memDC->SetTextColor(m_HiliteColour);
					}
					else if (GetItemTextColour(CurrentRow, CurrentColumn) != TRANSCOLOUR)
					{
						memDC->SetTextColor(GetItemTextColour(CurrentRow, CurrentColumn));
					}
					else if (GetColumnTextColour(CurrentColumn) != TRANSCOLOUR)
					{
						memDC->SetTextColor(GetColumnTextColour(CurrentColumn));
					}
					else
					{
						memDC->SetTextColor(m_TextColour);
					}
					DataRect.right = DataRect.left + GetColumnWidth(CurrentColumn);
					Str = GetItemText(CurrentRow, CurrentColumn);
					if (Str.IsEmpty() != true)
					{
						switch (GetColumnAlignment(CurrentColumn))
						{
							case COL_ALIGNR:
								LeftX = DataRect.right - memDC->GetTextExtent(Str).cx;
								break;
							case COL_ALIGNL:
								LeftX = DataRect.left + 3;
								break;
							case COL_ALIGNC:
								LeftX = DataRect.left + (DataRect.right - DataRect.left) / 2 - memDC->GetTextExtent(Str).cx / 2;
								break;
							default :
                FDASSERTMSG(_T("Unhandled text alignment"), 0);
						}
						LeftX = max(LeftX, DataRect.left + 1);
						memDC->ExtTextOut(LeftX, DataRect.top, ETO_CLIPPED, DataRect, Str, Str.GetLength(), null);
					}
				}
				DataRect.left = OrigLeftX + GetColumnWidth(CurrentColumn);
			} // for (int CurrentColumn = 0; CurrentColumn < m_NumCols; CurrentColumn++)
		}
		DataRect.top += m_RowHeight;
		DataRect.left = m_WindowRect.left;
		DataRect.right = m_WindowRect.right;
	}

	if (m_Style & LC_SHOWCOLUMNS)
	{
		DataRect = m_WindowRect;
		DataRect.right = DataRect.left + 1;
		//wchar_t str[256];
		for (int CurrentColumn = 0; CurrentColumn < m_NumCols - 1; CurrentColumn++)
		{
			DataRect.right += GetColumnWidth(CurrentColumn);
			memDC->DrawLine(CPoint(DataRect.right, m_WindowRect.top), CPoint(DataRect.right, m_WindowRect.bottom));
		//	wsprintf(str, _T("Left %d, Line %d %d to %d %d\n"), DataRect.left, DataRect.right, m_WindowRect.top, DataRect.right, m_WindowRect.bottom);
		//	OutputDebugString(str);
		}
		//wsprintf(str, _T("\n"));
		//OutputDebugString(str);
	}
	GetBasicDisplay()->RestoreFont();
}




/*------------------------------------------------------------------------------
	Method:   	CGrid::OnKeyDown
	Access:    	public
	Parameter: 	UINT _Char
	Parameter: 	UINT _RepCnt
	Parameter: 	UINT _Flags
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::OnKeyDown(UINT _Char, UINT _RepCnt, UINT _Flags)
{
	if (_Char == VK_UP)
	{
		if (m_SelectedItem != 0)
		{
			SetSelectedRow(m_SelectedItem - 1);
		}
	}
	else if (_Char == VK_DOWN)
	{
		if (m_SelectedItem != m_NumRows)
		{
			SetSelectedRow(m_SelectedItem + 1);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::OnVScroll
	Access:    	public
	Parameter: 	UINT _SBCode
	Parameter: 	UINT _Pos
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* pScrollBar)
{
	if (pScrollBar != &m_ScrollBar)
	{
		return false;
	}

	int iOldScrollPos = m_VerticalScroll;
	switch(_SBCode)
	{
	case SB_LINEDOWN:
		if (m_VerticalScroll < m_MaxScroll)
		{
			m_VerticalScroll++;
		}
		break;
	case SB_LINEUP:
		if (m_VerticalScroll > 0)
		{
			m_VerticalScroll--;
		}
		break;
	case SB_BOTTOM:
		// Scroll to bottom.
		m_VerticalScroll = m_MaxScroll;
		break;
	case SB_ENDSCROLL:
		//End scroll.
		break;
	case SB_PAGEDOWN:
		// Scroll one page down.
		m_VerticalScroll = min(m_VerticalScroll + m_NumRowsOnScreen, m_MaxScroll);
		break;
	case SB_PAGEUP:
		// Scroll one page up.
		m_VerticalScroll = max(m_VerticalScroll - m_NumRowsOnScreen, 0);
		break;
	case SB_THUMBPOSITION:
		// Scroll to the absolute position. The current position is provided in nPos.
		m_VerticalScroll = min((int)_Pos, m_MaxScroll);
		break;
	case SB_THUMBTRACK:
		// Drag scroll box to specified position. The current position is provided in nPos.
		m_VerticalScroll = min((int)_Pos, m_MaxScroll);
		break;
	}
	if (iOldScrollPos != m_VerticalScroll)
	{
		SetScrollBarPos();
		ForcePaint();
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetScrollBarPos
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetScrollBarPos()
{
	if (m_ScrollBarIsVisible == true)
	{
		m_ScrollBar.SetScrollPos(m_VerticalScroll);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::ShowScrollBar
	Access:    	public
	Parameter: 	int _Show
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::ShowScrollBar(int _Show)
{
	if (_Show == SW_HIDE || m_ScrollBarIsVisible == true)
	{
		m_ScrollBar.ShowWindow(_Show);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetVerticalScroll
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::GetVerticalScroll()
{
	return m_VerticalScroll;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetVerticalScroll
	Access:    	public
	Parameter: 	int _VerticalScroll
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetVerticalScroll(int _VerticalScroll)
{
	m_VerticalScroll = _VerticalScroll;
	SetScrollBarPos();
}
