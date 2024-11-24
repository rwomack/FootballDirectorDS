
/*

	Grid.h: interface for the Grid class.


	(c) Rw Software 2003-2007

---------------------------------------------------------------
*/


#pragma once


class CGridColumn;
class CGridRow;
class CSortedStringList;
class CBasicDisplay;
class CPNGDrawInfo;


// Grid Styles
#define LC_NORMAL			0x01
#define LC_NOHIGHLIGHT		0x02
#define LC_SHOWCOLUMNS		0x04
#define LC_SWAPPABLECOLUMNS 0x08


/////////////////////////////////////////////////////////////////////////////////////////////////////

class CGrid
{
public:
	CGrid(void);
	virtual ~CGrid(void);

	bool					OnSize(const int _RowsOnScreen, CRect _ListArea);
	void					OnInitialUpdate(CWnd* _ParentWnd, CBasicDisplay* _BasicDisplay, const int _Style = LC_NORMAL, const uint _RowHeight = GAPY);
	CGridRow*				GetCell(int _ColumnID, int _RowID);
	CGridColumn*			GetColumn(int _ColumnID = 0);

	bool					DoAddColumn(int _Width = -1, int _Style = COL_NORMAL/*, CString _BitmapFile*/);
	bool					SetColumnBitmap(int _ColumnID);
	bool					SetColumnWidth(int _ColumnID, int _Width);
	int						GetColumnWidth(int _ColumnID);
	int     				CountRows(void);
	int     				CountItems(void);
	int     				CountColumns(void);
	bool					SetColumnTextColour(int _ColumnID, COLORREF _Colour);
	bool					SetItemBackgroundColour(int _RowID, int _ColumnID, COLORREF _Colour);

	void					SetNumToShow(int numtoshow) { m_NumToShow = numtoshow; }

	void    				SetRowHeight(int Height)		{ m_RowHeight = Height; }
	int     				GetRowHeight();
	void    				SetRowBackgroundColour(int Row,COLORREF Colour);
	void					SetRowTextColour(int _RowID, COLORREF _Colour);

	void					SetRowPointer(int _RowID, void* _Pointer);
	void*					GetRowPointer(int _RowID);
	void					SetRowPointerType(int _RowID, ePointerType _PointerType = MAX_POINTER);
	ePointerType			GetRowPointerType(int _RowId);
	void					SetRowPointerDetails(int _RowID, void* _Pointer, ePointerType _PointerType = MAX_POINTER);

	int						DoAddItem(CString _Text, void*_Pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	int						DoAddItemList(CSortedStringList& _SortedTextList);
	int						DoAddRow();
	int						DoAddRows(int _HowMany);

	bool					SetItemTextColour(int _RowID, int _ColumnID, COLORREF _Colour);
	bool					SetItemText(int _RowID, int _ColumnID, LPCTSTR _Text = NULL, void* _Pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	bool					SetItemStrId(int _RowID, int _ColumnID, uint _StrID, void* pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	bool					SetItemNum(int _RowID, int _ColumnID, int _NumText = 0, void* _Pointer = NULL, ePointerType _PointerType = MAX_POINTER);
	void					SetItemBitmap(int _RowID, int _ColumnID, CPNGDrawInfo* _Bitmap);
	void					SetItemPointerDetails(int _RowID, int _ColumnID, void* _Pointer, ePointerType _PointerType = MAX_POINTER);

	CString					GetItemText(int _RowID, int _ColumnID);
	void*					GetItemPointer(int _RowID, int _ColumnID);

	void					SetScrollPos(int position);
	void					ResetScrollBar();
	int						GetVerticalScroll();
	bool					IsScrollBarVisible();
	void					DoInitialiseScrollBar(bool _AdjustForNoScrollBar);
	int						GetNumRowsOnScreen();

	void					SetTextFont(CFont* _Font);

	void					SetHiliteColour(COLORREF _hilitecol);
	void 					SetClickedColour(COLORREF _ClickedColour);
	void 					SetTextColour(COLORREF _TextColour);
	void 					SetHilite(bool _Hilite = true);
	void					SetBackgroundColours(COLORREF _BackgroundColour1, COLORREF _BackgroundColour2, COLORREF _HiliteBackgroundColour);
	void					SetBackgroundColour1(COLORREF _BackgroundColour1);
	void 					SetBackgroundColour2(COLORREF _BackgroundColour2);
	void 					SetHiliteBackgroundColour(COLORREF _HiliteBackgroundColour);
	void 					SetClickedBackgroundColour(COLORREF _ClickedBackgroundColour);

	bool					RemoveRow(int _RowID);
	void    				RemoveSelection(void);
	void    				RemoveAllData(bool Columns = false, bool scrollbar = false);
	void    				RemoveAll(void);

	int						GetClickedItem(void);
	void					SetClickedItem(int value = -1);
	int						GetScrollBarWidth();
	void					OnPaint();
	void					ForcePaint();	// Force redraw of list control
	void					OnKeyDown(UINT _Char, UINT _RepCnt, UINT _Flags);
	bool					OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* pScrollBar);
	bool					SetSelectedRow(int _RowID);
	int						GetCurrSel(CPoint _Point);
	void					SetStyle(int _Style);

	CBasicDisplay*			GetBasicDisplay();
	CScrollBar*				GetScrollBar();
	void					SetVerticalScroll(int _VerticalScroll);
	void					ShowScrollBar(int _Show);
	void					SetSwap();
	int						GetLastSelectedItem();
	bool					IsSwapRows();
	void					DoSwapRows();
	int GetFirstRowOffBottomScreen() const
	{
		return m_VerticalScroll + m_NumRowsOnScreen;
	}

protected:
	int						HitTestRow(CPoint _Point);
	int						HitTestCol(CPoint _Point);
	void					ReDraw(); 
	bool					IsWindowVisible();
	int						GetColumnStyle(int _ColumnID);
	int						GetColumnAlignment(int _ColumnID);
	COLORREF				GetColumnTextColour(int _ColumnID);
	COLORREF				GetRowBackgroundColour(int _RowID);
	COLORREF				GetItemTextColour(int _RowID, int _ColumnID);
	CPNGDrawInfo*			GetItemBitmap(int _RowID, int _ColumnID);
	void					SetScrollBarPos();

	CBasicDisplay*			m_BasicDisplay;
	int			 			m_Style;		
	int          			m_NumCols;
	int          			m_NumRows;
	CPtrArray    			m_Data;
	int          			m_RowHeight;

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
	int			 			m_TotalRowSize;

	int			 			m_NumToShow;

	bool		 			m_Clicked;
	bool		 			m_NumChanged;

	// Colour Stuff
	COLORREF     			m_HiliteColour;
	COLORREF	 			m_ClickedColour;
	COLORREF	 			m_TextColour;
	COLORREF     			m_BackgroundColour1;
	COLORREF     			m_BackgroundColour2;
	COLORREF     			m_HiliteBackgroundColour;
	COLORREF     			m_ClickedBackgroundColour;
	
	bool					m_Hilite;

	// Scroll Variables
	CKeyScrollBar			m_ScrollBar;
	bool         			m_ScrollBarVisible;
	int						m_ScrollBarWidth;
	int						m_VerticalScroll;
	int						m_MaxScroll;
	int						m_NumRowsOnScreen;
	bool					m_ScrollBarIsVisible;
};
