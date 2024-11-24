
/*

	Grid.cpp: implementation for the Grid class.


	(c) Rw Software 2007-2008

  ---------------------------------------------------------------
*/

#include "stdafx.h"

#include "UI.h"




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

	m_Visible = SW_SHOW;
	m_VerticalScroll = 0;
	m_MaxScroll = 0;
	m_NumRowsOnScreen = 0;
	m_RowHeight = 0;
	m_ScrollBarWidth = 12;
	m_ScrollBarIsCreated = false;

	m_Style = LC_NORMAL;
	m_Font = null;
	m_ParentWnd = null;
	m_SelectedItem = 0;
	m_LastSelectedItem  = NOTFOUND;
	m_ClickedItem = NOTFOUND;
	m_ScrollBarVisible  = true;
	m_ColumnGap = 0;
	m_NumToShow	= NOTFOUND;
	m_Clicked = false;
	m_NumChanged = true;

	// default colours
	m_LastClickedColour	= WHITE;
	m_LastClickedBackgroundColour = DARKGREEN;
	SetBackgroundColours(RGB(0xf5, 0xce, 0xaf)/*PASTELORANGE*/, RGB(0xf7, 0xb2, 0x7b)/*PALEORANGE*/, DARKORANGE /*RGB(16, 24, 144)*/);
	SetHiliteColour(WHITE);
	SetTextColour(BLACK);
	m_Hilite = true;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoLoadBitmapBars
	Access:    	public 
	Parameter: 	HWND _hWnd
	Parameter: 	const int _BitmapRscID
	Parameter: 	const int _BitmapRsc2ID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::DoLoadBitmapBars(HWND _hWnd, const int _BitmapRscID, const int _BitmapRsc2ID)
{
	HDC dc = ::GetDC(_hWnd);
	m_BitmapBar1.DoInitialise(dc, _BitmapRscID, 1);
	m_BitmapBar2.DoInitialise(dc, _BitmapRsc2ID, 1);
	::ReleaseDC(_hWnd, dc);
}


/*------------------------------------------------------------------------------
Method:   	CGrid::~CGrid
Access:    	virtual public 
Returns:   		Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CGrid::~CGrid()
{
	RemoveAllData(true);   
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
	return m_Columns.GetRowCount(); 
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
	return m_Columns.GetCount();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DisableRow
	Access:    	public 
	Parameter: 	int _RowID
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::DisableRow(int _RowID, bool _Value)
{
	m_Columns.DisableRow(_RowID, _Value);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoAddColumn
	Access:    	public 
	Parameter: 	int _Width
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::DoAddColumn(int _Width /*= -1*/, int _Style /*= COL_NORMAL*/)
{
	if (_Width == -1)
	{
		_Width = m_WindowRect.right - m_WindowRect.left;
	}
	if (m_Columns.DoAddColumn(_Width, _Style) == true)
	{
		m_NumChanged = true;
	}
	return m_NumChanged;
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
	m_Columns.DoAddItem(_Text, _Pointer, _PointerType);
	return CountRows() - 1;
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
	return CountRows() - 1;
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
	m_Columns.DoAddRow();
	return CountRows() - 1;
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
	DEBUG_OUTPUT(("%d Adding %d rows"), (CountRows(), _HowMany));
	m_NumChanged = true;
	m_Columns.DoAddRows(_HowMany);
	ResetScrollBar();
	return CountRows() - 1;
}


/*------------------------------------------------------------------------------
Method:   	CGrid::DoAddStyle
Access:    	public 
Parameter: 	int _Style
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CGrid::DoAddStyle(int _Style)
{
	m_Style |= _Style;
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
		DEBUG_OUTPUT(("Moving %d rows"), (CountRows()));
		m_MaxScroll = max(0, CountRows() - m_NumRowsOnScreen);
		if (m_MaxScroll > 0)
		{
			m_ScrollRect.top = m_WindowRect.top - 1;
			m_ScrollRect.bottom = m_WindowRect.bottom + 1;
			m_ScrollRect.left = m_WindowRect.right;
			m_ScrollRect.right = m_ScrollRect.left + m_ScrollBarWidth;
			m_ScrollBar.SetScrollRange(0, m_MaxScroll);
			m_ScrollBar.SetScrollPos(m_VerticalScroll);
			m_ScrollBar.MoveWindow(&m_ScrollRect, TRUE);
			if (m_Visible == SW_SHOW)
			{
				m_ScrollBar.ShowWindow(SW_SHOW);
			}
			m_ScrollBarIsVisible = true;
		}
		else
		{
			m_ScrollBar.ShowWindow(SW_HIDE);
			m_ScrollBarIsVisible = false;
		}
		ReDraw();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoSaveState
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::DoSaveState()
{
	m_SaveVerticalScroll = m_VerticalScroll;
	m_SaveSelectedItem = m_SelectedItem;
	m_SaveLastSelectedItem = m_LastSelectedItem;
	m_SaveClickedItem = m_ClickedItem;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoRestoreState
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::DoRestoreState()
{
	m_VerticalScroll = m_SaveVerticalScroll;
	m_SelectedItem = m_SaveSelectedItem;
	m_LastSelectedItem = m_SaveLastSelectedItem;
	m_ClickedItem = m_SaveClickedItem;
}


/*------------------------------------------------------------------------------
Method:   	CGrid::DoSortLow
Access:    	public 
Parameter: 	int _ColumnNum
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CGrid::DoSortLow(int _ColumnNum)
{
	bool swap = true;
	while(swap == true)
	{
		swap = false;
		for (int LoopCount = 0; LoopCount < CountRows() - 1; LoopCount++)
		{
			if (GetColumnStyle(_ColumnNum) & COL_TEXT)
			{
				if (GetCell(_ColumnNum, LoopCount)->GetText() > GetCell(_ColumnNum, LoopCount + 1)->GetText())
				{
					DoSwapRows(LoopCount, LoopCount + 1);
					swap = true;
				}
			}
			else if (GetColumnStyle(_ColumnNum) & COL_NUMERIC || (GetColumnStyle(_ColumnNum) & COL_DATE) || (GetColumnStyle(_ColumnNum) & COL_MONEY))
			{
				if (GetCell(_ColumnNum, LoopCount)->GetValue() > GetCell(_ColumnNum, LoopCount + 1)->GetValue())
				{
					DoSwapRows(LoopCount, LoopCount + 1);
					swap = true;
				}
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoSortHigh
	Access:    	public 
	Parameter: 	int _ColumnNum
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::DoSortHigh(int _ColumnNum)
{
	bool swap = true;
	while(swap == true)
	{
		swap = false;
		for (int y = 0; y < CountRows() - 1; y++)
		{
			if (GetColumnStyle(_ColumnNum) & COL_TEXT)
			{
				if (GetCell(_ColumnNum, y)->GetText() < GetCell(_ColumnNum, y + 1)->GetText())
				{
					DoSwapRows(y, y + 1);
					swap = true;
				}
			}
			else if (GetColumnStyle(_ColumnNum) & COL_NUMERIC || (GetColumnStyle(_ColumnNum) & COL_DATE) || (GetColumnStyle(_ColumnNum) & COL_MONEY))
			{
				if (GetCell(_ColumnNum, y)->GetValue() < GetCell(_ColumnNum, y + 1)->GetValue())
				{
					DoSwapRows(y, y + 1);
					swap = true;
				}
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoSort
	Access:    	protected 
	Parameter: 	int _ColumnNum
	Parameter: 	const eSortOrder _SortOrder
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::DoSort(int _ColumnNum, const eSortOrder _SortOrder)
{
	GetColumn(_ColumnNum)->SetSortOrder(_SortOrder);
	DoSort();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoSort
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::DoSort()
{
	for (int LoopCount = 0; LoopCount < CountColumns(); LoopCount++)
	{
		if (GetColumn(LoopCount)->GetSortOrder() == SORT_LOW)
		{
			DoSortLow(LoopCount);
			break;
		}
		if (GetColumn(LoopCount)->GetSortOrder() == SORT_HIGH)
		{
			DoSortHigh(LoopCount);
			break;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoSortHighWithDifference
	Access:    	public 
	Parameter: 	int _ColumnNum
	Parameter: 	int _ColumnNum1
	Parameter: 	int _ColumnNum2
	Returns:   	void
	Qualifier: 	
	Purpose:	Used for league table, supports goal difference
------------------------------------------------------------------------------*/
void CGrid::DoSortHighWithDifference(int _ColumnNum, int _ColumnNum1, int _ColumnNum2)
{
	EXT_ASSERT(GetColumnStyle(_ColumnNum) & COL_NUMERIC);
	EXT_ASSERT(_ColumnNum1 != _ColumnNum2);
	bool swap = true;
	while(swap == true)
	{
		swap = false;
		for (int LoopCount = 0; LoopCount < CountRows() - 1; LoopCount++)
		{
			if (GetCell(_ColumnNum, LoopCount)->GetValue() < GetCell(_ColumnNum, LoopCount + 1)->GetValue() || 
				(GetCell(_ColumnNum, LoopCount)->GetValue() == GetCell(_ColumnNum, LoopCount + 1)->GetValue() &&
				GetCell(_ColumnNum1, LoopCount)->GetValue() - GetCell(_ColumnNum2, LoopCount)->GetValue() < 
				GetCell(_ColumnNum1, LoopCount + 1)->GetValue() - GetCell(_ColumnNum2, LoopCount + 1)->GetValue())
				)
			{
				DoSwapRows(LoopCount, LoopCount + 1);
				swap = true;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoSwapRows
	Access:    	public 
	Parameter: 	int _Row1
	Parameter: 	int _Row2
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::DoSwapRows(int _Row1, int _Row2)
{
	CGridRow* pRow;
	COLORREF TempBackgroundColour;
	for (int LoopCount = 0; LoopCount < CountColumns(); LoopCount++)
	{
		pRow = GetCell(LoopCount, _Row1);
		SetCell(LoopCount, _Row1, GetCell(LoopCount, _Row2));
		SetCell(LoopCount, _Row2, pRow);
		TempBackgroundColour = GetCell(LoopCount, _Row1)->GetBackgroundColour();
		GetCell(LoopCount, _Row1)->SetBackgroundColour(GetCell(LoopCount, _Row2)->GetBackgroundColour());
		GetCell(LoopCount, _Row2)->SetBackgroundColour(TempBackgroundColour);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::DoSwapRows
	Access:    	protected 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::DoSwapClickedRows()
{
	EXT_ASSERT(m_Style & LC_SWAPPABLECOLUMNS);
	m_LastSelectedItem = m_ClickedItem = NOTFOUND;
	m_SelectedItem = 0;
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
	EXT_ASSERT_POINTER(m_BasicDisplay, CBasicDisplay);
	return m_BasicDisplay;
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
	EXT_ASSERT(_RowID >= 0 && _RowID < CountRows());
	EXT_ASSERT(_ColumnID >= 0 && _ColumnID < CountColumns());
	CGridColumn* pCol = m_Columns.GetAt(_ColumnID);
	return (CGridRow*)pCol->m_Rows.GetAt(_RowID);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetCell
	Access:    	protected 
	Parameter: 	int _ColumnID
	Parameter: 	int _RowID
	Parameter: 	CGridRow * _pItem
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetCell(int _ColumnID, int _RowID, CGridRow* _pItem)
{
	EXT_ASSERT(_RowID >= 0 && _RowID < CountRows());
	EXT_ASSERT(_ColumnID >= 0 && _ColumnID < CountColumns());
	CGridColumn* pCol = m_Columns.GetAt(_ColumnID);
	pCol->m_Rows.SetAt(_RowID, _pItem);
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
	if (m_Columns.GetCount() == 1)
	{
		EXT_ASSERT(m_WindowRect.right > 0);
		return m_WindowRect.right - m_WindowRect.left;
	}
	return GetColumn(_ColumnID)->GetWidth();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetColumnScreenX
	Access:    	protected 
	Parameter: 	int _ColumnID
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::GetColumnScreenX(int _ColumnID)
{
	int XPos = m_WindowRect.left;
	for (int LoopCount = 0; LoopCount <_ColumnID; LoopCount++)
	{
		XPos += GetColumn(LoopCount)->GetWidth();
	}
	return XPos;
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
	Method:   	CGrid::GetHiliteBackgroundColour
	Access:    	public 
	Returns:   	COLORREF
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
COLORREF CGrid::GetHiliteBackgroundColour()
{
	return m_HiliteBackgroundColour;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetHilite
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::GetHilite()
{
	return m_Hilite;
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
	Method:   	CGrid::GetCurrentRowPointer
	Access:    	public 
	Returns:   	void*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void* CGrid::GetCurrentRowPointer(int _Column /*= 0*/)
{
	return GetCell(_Column, GetCurrentRow())->GetPointer();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetCurrentRowPointerType
	Access:    	public 
	Parameter: 	int _Column
	Returns:   	ePointerType
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ePointerType CGrid::GetCurrentRowPointerType(int _Column /*= 0*/)
{
	return GetCell(_Column, GetCurrentRow())->GetPointerType();
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
	Method:   	CGrid::GetColumn
	Access:    	public 
	Parameter: 	int _ColumnID
	Returns:   	CColumnInfo*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CGridColumn* CGrid::GetColumn(int _ColumnID)
{
	return m_Columns.GetAt(_ColumnID);
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
	Method:   	CGrid::GetWidth
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::GetWidth()
{
	EXT_ASSERT(m_WindowRect.right > 0);
	return m_WindowRect.right - m_WindowRect.left;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::HideHilite
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::HideHilite()
{
	SetHiliteBackgroundColour(TRANSCOLOUR);
	SetHiliteColour(TRANSCOLOUR);
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

	if (RowNum > (CountRows() - 1))
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
	Access:    	public
	Parameter: 	CPoint _Point
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::HitTestCol(CPoint _Point)
{
	// If we're in the data area
	if (m_WindowRect.PtInRect(_Point) == TRUE)
	{
		// Check all columns
		int col = 0;
		int offset = m_WindowRect.left;
		CGridColumn* pCol;
		while (col < CountColumns())
		{
			pCol = m_Columns.GetAt(col);
			EXT_ASSERT_POINTER(pCol, CGridColumn);
			if ( (_Point.x > offset) && (_Point.x < offset + pCol->GetWidth() + m_ColumnGap) )
			{
				return col;
			}
			col++;
			offset += pCol->GetWidth() + m_ColumnGap;
		}
	}
	return NOTFOUND;
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
	EXT_ASSERT_POINTER(_ParentWnd, CWnd);
	EXT_ASSERT_POINTER(_BasicDisplay, CBasicDisplay);
	EXT_ASSERT(_RowHeight > 0);
	if (m_ScrollBarIsCreated == false)
	{
		m_ScrollBar.Create(WS_CHILD | SBS_VERT, m_ScrollRect, _ParentWnd, 100);
		m_ScrollBarIsCreated = true;
	}
	if (_Style & LC_NOHIGHLIGHT)
	{
		HideHilite();
	}
	m_Style = _Style;
	m_RowHeight = _RowHeight;
	m_ParentWnd = _ParentWnd;
	m_BasicDisplay = _BasicDisplay;
	RemoveAllData(true);
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
	if (m_Visible == SW_SHOW && CountRows() > 0)
	{
		switch (_Char)
		{
			case  VK_UP:
				{
					int CurrentItem = m_SelectedItem - 1;
					while (CurrentItem >= 0)
					{ 
						if (IsRowDisabled(CurrentItem) == false)
						{
							SetSelectedRow(CurrentItem);
							m_SelectedItem = CurrentItem;
							break;
						}
						CurrentItem--;
					}
				}
				break;
				
			case VK_DOWN:
				{
					int CurrentItem = m_SelectedItem + 1;
					while (CurrentItem < CountRows() /*- 1*/)
					{ 
						if (IsRowDisabled(CurrentItem) == false)
						{
							SetSelectedRow(CurrentItem);
							m_SelectedItem = CurrentItem;
							break;
						}
						CurrentItem++;
					}
				}
				break;

			case VK_RETURN:
			case VK_SELECT:
			case VK_F23:
				if (m_Style & LC_SWAPPABLECOLUMNS)
				{
					SetSwap();
				}

		}
	}
}


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
	DEBUG_ONLY(g_DebugStr.Format(_T("Grid Visibility %d, %d rows, %d columns to draw."), m_Visible, CountRows(), CountColumns())); 
	DEBUG_ONLY(g_DebugStr.OutputDebug());	
	if (m_Visible == SW_HIDE || CountRows() == 0)
	{
		return;
	}

	COffScreenBuffer* memDC =  GetBasicDisplay()->GetOffScreenBuffer();
	EXT_ASSERT_POINTER(memDC, COffScreenBuffer);

	// Draw outline around the list
	if (!(m_Style & LC_NOBORDER))
	{
		CPen Scorepen(PS_SOLID, 1, BLACK);
		CPen* old = memDC->SelectObject(&Scorepen);
		memDC->Rectangle(m_WindowRect.left - 1, m_WindowRect.top - 1, m_WindowRect.right + 1, m_WindowRect.bottom + 1);
		memDC->SelectObject(old);
	}

	if (m_Font == null)
	{
		GetBasicDisplay()->SetFontSmall();
	}
	else
	{
		GetBasicDisplay()->SetFont(*m_Font);
	}
	COLORREF ColourStripe;
	CRect DataRect = m_WindowRect;
	CString Str;
	int LeftX, TopY;
	for (int CurrentRow = m_VerticalScroll; CurrentRow < m_VerticalScroll + m_NumRowsOnScreen; CurrentRow++)
	{
		DataRect.bottom = DataRect.top + m_RowHeight;
		if (!(m_Style & LC_NOBACKGROUND))
		{
			if ((m_Style & LC_SWAPPABLECOLUMNS) && CurrentRow == m_LastSelectedItem)
			{
				ColourStripe = m_LastClickedBackgroundColour;
			}
			else if (CurrentRow == m_SelectedItem && m_HiliteBackgroundColour != TRANSCOLOUR && m_Hilite == true)
			{
				ColourStripe = m_HiliteBackgroundColour;
			}
			else if (CurrentRow < CountRows() && GetRowBackgroundColour(CurrentRow) != TRANSCOLOUR)
			{
				ColourStripe = GetRowBackgroundColour(CurrentRow);
			}
			else if (CurrentRow % 2 == 1)
			{
				m_BitmapBar1.DoDraw(memDC->operator HDC(), DataRect.left, DataRect.top, DataRect.right - DataRect.left, m_RowHeight);
				ColourStripe = TRANSCOLOUR;
			}
			else
			{
				m_BitmapBar2.DoDraw(memDC->operator HDC(), DataRect.left, DataRect.top, DataRect.right - DataRect.left, m_RowHeight);
				ColourStripe = TRANSCOLOUR;
			}
			if (ColourStripe != TRANSCOLOUR)
			{
				memDC->FillSolidRect(DataRect.left, DataRect.top, DataRect.right - DataRect.left, m_RowHeight, ColourStripe);
			}
		}

		if (CurrentRow < CountRows())
		{
			int OrigLeftX;
			for (int CurrentColumn = 0; CurrentColumn < CountColumns(); CurrentColumn++)
			{
				OrigLeftX = DataRect.left;
				if (GetItemBitmap(CurrentRow, CurrentColumn) != NULL)
				{
					if (GetItemBitmapNumber(CurrentRow, CurrentColumn) != NOTFOUND)
					{
						// Draw the bitmap
						DataRect.left += GetColumnWidth(CurrentColumn) / 2 - GetItemBitmap(CurrentRow, CurrentColumn)->GetWidth() / 2;
						DataRect.top += GetRowHeight() / 2 - GetItemBitmap(CurrentRow, CurrentColumn)->GetHeight() / 2;
						GetItemBitmap(CurrentRow, CurrentColumn)->DoDrawTransparent(memDC->operator HDC(), DataRect.left, DataRect.top, GetItemBitmapNumber(CurrentRow, CurrentColumn));
						DataRect.top -= GetRowHeight() / 2 - GetItemBitmap(CurrentRow, CurrentColumn)->GetHeight() / 2;
					}
				}
				else
				{
					// Draw the text
					Str = GetItemText(CurrentRow, CurrentColumn);
					if (Str.IsEmpty() != true)
					{
						if (IsRowDisabled(CurrentRow) == true)
						{
							memDC->SetTextColor(GetItemDisabledTextColour(CurrentRow, CurrentColumn));
						}
						else if (CurrentRow == m_SelectedItem && m_HiliteColour != TRANSCOLOUR && m_Hilite == true)
						{
							memDC->SetTextColor(m_HiliteColour);
						}
						else if (GetColumnTextColour(CurrentColumn) != TRANSCOLOUR)
						{
							memDC->SetTextColor(GetColumnTextColour(CurrentColumn));
						}
						else if (GetItemTextColour(CurrentRow, CurrentColumn) != TRANSCOLOUR)
						{
							memDC->SetTextColor(GetItemTextColour(CurrentRow, CurrentColumn));
						}
						else
						{
							memDC->SetTextColor(m_TextColour);
						}
						DataRect.right = DataRect.left + GetColumnWidth(CurrentColumn);
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
								EXT_ASSERT(false);
						}
						LeftX = max(LeftX, DataRect.left + 1);
						TopY = DataRect.top + (GetRowHeight() - memDC->GetTextExtent(Str).cy) / 2;
						memDC->ExtTextOut(LeftX, TopY, ETO_CLIPPED, DataRect, Str, Str.GetLength(), null);
					}
				}
				DataRect.left = OrigLeftX + GetColumnWidth(CurrentColumn);
			} // for (int CurrentColumn = 0; CurrentColumn < CountColumns(); CurrentColumn++)
		}
		DataRect.top += m_RowHeight;
		DataRect.left = m_WindowRect.left;
		DataRect.right = m_WindowRect.right;
	}

	if (m_Style & LC_SHOWCOLUMNS)
	{
		DataRect = m_WindowRect;
		DataRect.right = DataRect.left + 1;
		for (int CurrentColumn = 0; CurrentColumn < CountColumns() - 1; CurrentColumn++)
		{
			DataRect.right += GetColumnWidth(CurrentColumn);
			memDC->DrawLine(CPoint(DataRect.right, m_WindowRect.top), CPoint(DataRect.right, m_WindowRect.bottom));
		}
	}
	GetBasicDisplay()->RestoreFont();
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
	if (pScrollBar != &m_ScrollBar || m_Visible == SW_HIDE || CountRows() == 0)
	{
		return false;
	}

	int iOldScrollPos = m_VerticalScroll;
	switch (_SBCode)
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
		ReDraw();
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::MoveWindow
	Access:    	public 
	Parameter: 	const int _RowsOnScreen
	Parameter: 	CRect _ListArea
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::MoveWindow(CRect _ListArea)
{
	m_WindowRect = _ListArea;
	m_NumRowsOnScreen = (_ListArea.bottom - _ListArea.top) / GetRowHeight(); //_RowsOnScreen;
	DoInitialiseScrollBar(true);
	m_VerticalScroll = min(m_VerticalScroll, m_MaxScroll);	// Height may be taller
	if (CountColumns() == 1)
	{
		// Only 1 column so ensure fills width of grid
		SetColumnWidth(0, _ListArea.right);
	}
	return TRUE;
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
	m_HiliteBackgroundColour = _HiliteBackgroundColour;
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
	Method:   	CGrid::SetCurrentRow
	Access:    	public 
	Parameter: 	const int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetCurrentRow(const int _Value) 
{ 
	m_SelectedItem = _Value; 
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
	m_LastClickedColour = _ClickedColour; 
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
	m_LastClickedBackgroundColour = _ClickedBackgroundColour; 
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
	Method:   	CGrid::SetColumnWidthPercentage
	Access:    	public 
	Parameter: 	int _ColumnID
	Parameter: 	int _Width
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::SetColumnWidthPercentage(int _ColumnID, int _Width)
{
	return SetColumnWidth(_ColumnID, (GetWidth() / 100) * _Width);
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
	for (int LoopCount = 0; LoopCount < CountColumns(); LoopCount++) 
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
	for (int LoopCount = 0; LoopCount < CountColumns(); LoopCount++) 
	{
		GetCell(LoopCount, _RowID)->SetTextColour(_Colour);
	}
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
	return GetCell(_ColumnID, _RowID)->SetStrId(_StrID, _Pointer, _PointerType);
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
	return GetCell(_ColumnID, _RowID)->SetItemNum(_NumText, _Pointer, _PointerType);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetItemDate
	Access:    	public 
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Parameter: 	CDate & _Date
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::SetItemDate(int _RowID, int _ColumnID, CDate& _Date)
{
	return GetCell(_ColumnID, _RowID)->SetItemDate(_Date);
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::SetItemMoney
	Access:    	public 
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Parameter: 	CString & _Str
	Parameter: 	int _Amount
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::SetItemMoney(int _RowID, int _ColumnID, CString& _Str, int _Amount)
{
	return GetCell(_ColumnID, _RowID)->SetItemMoney(_Str, _Amount);
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
void CGrid::SetItemBitmap(int _RowID, int _ColumnID, CPNGDrawInfo* _Bitmap, const int _ImageNumber /*= 0*/)
{
	GetCell(_ColumnID, _RowID)->SetBitmap(_Bitmap, _ImageNumber);
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
	Method:   	CGrid::GetItemBitmapNumber
	Access:    	protected 
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::GetItemBitmapNumber(int _RowID, int _ColumnID)
{
	return GetCell(_ColumnID, _RowID)->GetBitmapImageNumber();
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
	Method:   	CGrid::GetItemDisabledTextColour
	Access:    	protected 
	Parameter: 	int _RowID
	Parameter: 	int _ColumnID
	Returns:   	COLORREF
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
COLORREF CGrid::GetItemDisabledTextColour(int _RowID, int _ColumnID)
{
	return GetCell(_ColumnID, _RowID)->GetDisabledTextColour();
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::IsRowDisabled
	Access:    	public 
	Parameter: 	int _RowID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::IsRowDisabled(int _RowID)
{
	return GetCell(0, _RowID)->IsDisabled();
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
	for (int LoopCount = 0; LoopCount < CountColumns(); LoopCount++)
	{
		CGridColumn *pCol = m_Columns.GetAt(LoopCount);
		CGridRow *pRow = (CGridRow*)pCol->m_Rows.GetAt(_RowID);
		pCol->m_Rows.RemoveAt(_RowID);
		delete pRow;
	}
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
void CGrid::RemoveAllData(bool _Columns /*= false*/)
{
	if (m_Columns.GetCount())
	{
		m_Columns.DoRemoveAllData(_Columns);
	}
	m_SelectedItem = 0;
	m_LastSelectedItem = NOTFOUND;
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
	RemoveAllData(false);
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
	Method:   	CGrid::SetScrollPos
	Access:    	public 
	Parameter: 	int position
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::SetScrollPos(int position)
{
	if (position < CountRows())
	{
		m_ScrollBarVisible = false;		
	}
	m_NumChanged = true;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::GetCurrentRow
	Access:    	public 
	Parameter: 	CPoint _Point
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::GetCurrentRow(CPoint _Point) 
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
	Method:   	CGrid::GetCurrSel
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CGrid::GetCurrentRow() 
{ 
	return m_SelectedItem; 
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
	EXT_ASSERT(m_Style & LC_SWAPPABLECOLUMNS);

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
	EXT_ASSERT(m_Style & LC_SWAPPABLECOLUMNS);
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
	EXT_ASSERT(m_Style & LC_SWAPPABLECOLUMNS);
	if (m_ClickedItem != NOTFOUND && m_LastSelectedItem != NOTFOUND)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CGrid::IsSwappableRows
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CGrid::IsSwappableRows()
{
	if (m_Style & LC_SWAPPABLECOLUMNS)
	{
		return true;
	}
	return false;
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
	EXT_ASSERT(_RowID < CountRows());
	if (m_SelectedItem != _RowID)
	{
		m_SelectedItem = _RowID;        
		if (m_SelectedItem < m_VerticalScroll)
		{
			m_VerticalScroll = m_SelectedItem;
		}
		while (m_SelectedItem >= m_VerticalScroll + m_NumRowsOnScreen)
		{
			m_VerticalScroll++;
		}
		SetScrollBarPos();
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
	m_SelectedItem = 0;    
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
	EXT_ASSERT_POINTER(m_ParentWnd, CWnd);
	m_ParentWnd->InvalidateRect(m_WindowRect, FALSE);
};


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
	Method:   	CGrid::ShowWindow
	Access:    	public 
	Parameter: 	int _Visible
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::ShowWindow(int _Visible)
{
	EXT_ASSERT(_Visible == SW_SHOW || _Visible == SW_HIDE);
	m_Visible = _Visible;
	ShowScrollBar(_Visible);
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
	Method:   	CGrid::ShowCursorBar
	Access:    	public 
	Parameter: 	int _Show
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CGrid::ShowCursorBar(int _Show)
{
	if (_Show == SW_HIDE)
	{
		m_Hilite = false;
	}
	else
	{
		m_Hilite = true;
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


/*------------------------------------------------------------------------------
	Method:   	CInvisibleGrid::OnInitialUpdate
	Access:    	public 
	Parameter: 	CWnd * _ParentWnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	int _Style
	Parameter: 	const uint _RowHeight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInvisibleGrid::OnInitialUpdate(CWnd* _ParentWnd, CBasicDisplay* _BasicDisplay, int _Style /*= LC_NORMAL*/, const uint _RowHeight /*= GAPY*/)
{
	_Style |= LC_NOBORDER | LC_NOBACKGROUND;
	CGrid::OnInitialUpdate(_ParentWnd, _BasicDisplay, _Style , _RowHeight); 
	CGrid::HideHilite();
}
