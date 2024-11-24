#if !defined(GRID_INCLUDED_)
#define GRID_INCLUDED_

/*

	Grid.h: interface for the Grid class.


	(c) Rw Software 2003 - 2008

  ---------------------------------------------------------------
*/


class CGrid
{
public:
	CGrid(void);
	virtual ~CGrid(void);

	int						CountColumns();
	int     				CountRows(void);

	void					DisableRow(int _RowID, bool _Value);
	bool					DoAddColumn(int _Width = -1, int _Style = COL_NORMAL/*, CString _BitmapFile*/);
	int						DoAddItem(CString _Text, void*_Pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	int						DoAddItemList(CSortedStringList& _SortedTextList);
	int						DoAddRow();
	int						DoAddRows(int _HowMany);
	void					DoAddStyle(int _Style);
	void					DoInitialiseScrollBar(bool _AdjustForNoScrollBar);
	void					DoLoadBitmapBars(HWND _hWnd, const int _BitmapRscID, const int _BitmapRsc2ID);
	void					DoRestoreState();
	void					DoSaveState();
	void					DoSort();
	void					DoSort(int _ColumnNum, const eSortOrder _SortOrder);
	void					DoSortHigh(int _ColumnNum);
	void					DoSortHighWithDifference(int _ColumnNum, int _ColumnNum1, int _ColumnNum2);
	void					DoSortLow(int _ColumnNum);
	void					DoSwapRows(int _Row1, int _Row2);
	void					DoSwapClickedRows();
	CBasicDisplay*			GetBasicDisplay();
	int						GetClickedItem(void);
	CGridRow*				GetCell(int _ColumnID, int _RowID);
	CGridColumn*			GetColumn(int _ColumnID = 0);
	int						GetColumnScreenX(int _ColumnID);
	int						GetColumnWidth(int _ColumnID);
	int						GetCurrentRow(CPoint _Point);
	int						GetCurrentRow();
	void*					GetCurrentRowPointer(int _Column = 0);
	ePointerType			GetCurrentRowPointerType(int _Column = 0);
	COLORREF     			GetHiliteBackgroundColour();
	bool					GetHilite();
	COLORREF				GetItemDisabledTextColour(int _RowID, int _ColumnID);
	CString					GetItemText(int _RowID, int _ColumnID);
	void*					GetItemPointer(int _RowID, int _ColumnID);
	int						GetLastSelectedItem();
	int						GetNumRowsOnScreen();
	int     				GetRowHeight();
	void*					GetRowPointer(int _RowID);
	ePointerType			GetRowPointerType(int _RowId);
	CScrollBar*				GetScrollBar();
	int						GetScrollBarWidth();
	int						GetVerticalScroll();
	int						GetWidth();
	void					HideHilite();
	int						HitTestCol(CPoint _Point);
	bool					IsRowDisabled(int _RowID);
	bool					IsScrollBarVisible();
	bool					IsSwappableRows();
	bool					IsSwapRows();
	void					OnInitialUpdate(CWnd* _ParentWnd, CBasicDisplay* _BasicDisplay, const int _Style = LC_NORMAL, const uint _RowHeight = GAPY - 2);
	void					OnKeyDown(UINT _Char, UINT _RepCnt, UINT _Flags);
	void					OnPaint();
	bool					MoveWindow(CRect _ListArea);
	bool					OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* pScrollBar);
	bool					RemoveRow(int _RowID);
	void    				RemoveSelection(void);
	void					RemoveAllData(bool _Columns = false);
	void    				RemoveAll(void);
	void					ResetScrollBar();
	void					SetBackgroundColours(COLORREF _BackgroundColour1, COLORREF _BackgroundColour2, COLORREF _HiliteBackgroundColour);
	void					SetBackgroundColour1(COLORREF _BackgroundColour1);
	void 					SetBackgroundColour2(COLORREF _BackgroundColour2);
	void					SetCell(int _ColumnID, int _RowID, CGridRow* _pItem);
	void 					SetClickedBackgroundColour(COLORREF _ClickedBackgroundColour);
	void 					SetClickedColour(COLORREF _ClickedColour);
	void					SetClickedItem(int value = -1);
	bool					SetColumnBitmap(int _ColumnID);
	bool					SetColumnTextColour(int _ColumnID, COLORREF _Colour);
	bool					SetColumnWidth(int _ColumnID, int _Width);
	bool					SetColumnWidthPercentage(int _ColumnID, int _Width);
	void					SetCurrentRow(const int _Value);
	void 					SetHilite(bool _Hilite = true);
	void 					SetHiliteBackgroundColour(COLORREF _HiliteBackgroundColour);
	void					SetHiliteColour(COLORREF _hilitecol);
	bool					SetItemBackgroundColour(int _RowID, int _ColumnID, COLORREF _Colour);
	void					SetItemBitmap(int _RowID, int _ColumnID, CPNGDrawInfo* _Bitmap, const int _ImageNumber = 0);
	bool					SetItemDate(int _RowID, int _ColumnID, CDate& _Date);
	bool					SetItemMoney(int _RowID, int _ColumnID, CString& _Str, int _Amount);
	bool					SetItemNum(int _RowID, int _ColumnID, int _NumText = 0, void* _Pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	void					SetItemPointerDetails(int _RowID, int _ColumnID, void* _Pointer, ePointerType _PointerType = MAX_POINTER);
	bool					SetItemStrId(int _RowID, int _ColumnID, uint _StrID, void* pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	bool					SetItemTextColour(int _RowID, int _ColumnID, COLORREF _Colour);
	bool					SetItemText(int _RowID, int _ColumnID, LPCTSTR _Text = NULL, void* _Pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	void					SetNumToShow(int numtoshow) { m_NumToShow = numtoshow; }
	void    				SetRowHeight(int Height)		{ m_RowHeight = Height; }
	void    				SetRowBackgroundColour(int Row,COLORREF Colour);
	void					SetRowPointer(int _RowID, void* _Pointer);
	void					SetRowPointerType(int _RowID, ePointerType _PointerType = MAX_POINTER);
	void					SetRowPointerDetails(int _RowID, void* _Pointer, ePointerType _PointerType = MAX_POINTER);
	void					SetRowTextColour(int _RowID, COLORREF _Colour);
	bool					SetSelectedRow(int _RowID);
	void					SetScrollPos(int position);
	void					SetStyle(int _Style);
	void					SetSwap();
	void 					SetTextColour(COLORREF _TextColour);
	void					SetTextFont(CFont* _Font);
	void					SetVerticalScroll(int _VerticalScroll);
	void					ShowCursorBar(int _Show);
	void					ShowScrollBar(int _Show);
	void					ShowWindow(int _Visible);

protected:
	int						HitTestRow(CPoint _Point);
	void					ReDraw(); 
	bool					IsWindowVisible();
	int						GetColumnStyle(int _ColumnID);
	int						GetColumnAlignment(int _ColumnID);
	COLORREF				GetColumnTextColour(int _ColumnID);
	COLORREF				GetRowBackgroundColour(int _RowID);
	COLORREF				GetItemTextColour(int _RowID, int _ColumnID);
	CPNGDrawInfo*			GetItemBitmap(int _RowID, int _ColumnID);
	int						GetItemBitmapNumber(int _RowID, int _ColumnID);
	void					SetScrollBarPos();

	CBasicDisplay*			m_BasicDisplay;
	int			 			m_Style;		
	CGridColumnList			m_Columns;
	int          			m_RowHeight;
	int						m_Visible;

	int          			m_SelectedItem;
	int          			m_LastSelectedItem;
	int			 			m_ClickedItem;

	CRect		 			m_WindowRect;
	CRect        			m_DataArea;
	CRect		 			m_ScrollRect;
	int          			m_ColumnGap;

	CWnd*					m_ParentWnd;
	CFont*					m_Font;

	CRect		 			m_ClientRect;

	int			 			m_NumToShow;

	bool		 			m_Clicked;
	bool		 			m_NumChanged;

	// Colour Stuff
	COLORREF     			m_HiliteColour;
	COLORREF	 			m_LastClickedColour;
	COLORREF	 			m_TextColour;
	COLORREF     			m_BackgroundColour1;
	COLORREF     			m_BackgroundColour2;
	COLORREF     			m_HiliteBackgroundColour;
	COLORREF     			m_LastClickedBackgroundColour;
	CPNGDrawInfo			m_BitmapBar1;
	CPNGDrawInfo			m_BitmapBar2;

	bool					m_Hilite;

	// Scroll Variables
	CKeyScrollBar			m_ScrollBar;
	bool         			m_ScrollBarVisible;
	int						m_ScrollBarWidth;
	int						m_VerticalScroll;
	int						m_MaxScroll;
	int						m_NumRowsOnScreen;
	bool					m_ScrollBarIsVisible;
	bool					m_ScrollBarIsCreated;

	// Save state
	int						m_SaveVerticalScroll;
	int          			m_SaveSelectedItem;
	int          			m_SaveLastSelectedItem;
	int			 			m_SaveClickedItem;

};
/////////////////////////////////////////////////////////////////////////////////////////////////////


class CInvisibleGrid : public CGrid
{
public:
	void					OnInitialUpdate(CWnd* _ParentWnd, CBasicDisplay* _BasicDisplay, int _Style = LC_NORMAL, const uint _RowHeight = GAPY);
};

#endif