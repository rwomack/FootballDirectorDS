
/*

	GridColumn.h: interface for the GridColumn class.


	(c) Rw Software 2003 - 2008

  ---------------------------------------------------------------
*/


#ifndef GRIDCOLUMN


class CGridColumnList
{
public:
	void					DisableRow(int _RowID, bool _Value);
	bool					DoAddColumn(int _Width = -1, int _Style = COL_NORMAL);
	void					DoAddItem(CString _Text, void* _Pointer, ePointerType _PointerType = MAX_POINTER);
	void					DoAddRow();
	void					DoAddRows(int _HowMany);
	void					DoRemoveAllData(bool _Columns = false);
	void					DoSortLow(int _ColumnNum);
	void					DoSwapRows(int _Row1, int _Row2);
	CGridColumn*			GetAt(int _Element);
	int						GetCount();
	int						GetRowCount();

protected:
	CPtrArray    			m_Columns;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////


class CGridColumn
{
public:
	CGridColumn();
	CGridColumn(const int _Style, const int _Width, const COLORREF _TextColour = BLACK, const eSortOrder _SortOrder = UNSORTED);
	~CGridColumn();

	void					DoRemoveAllData();
	CGridRow*				GetRow(int _RowID);
	int						GetRowCount();
	eSortOrder				GetSortOrder();
	int						GetStyle();
	COLORREF				GetTextColour();
	int						GetWidth();
	void					SetSortOrder(const eSortOrder _SortOrder = UNSORTED);
	void					SetStyle(int _Style = COL_NORMAL);
	void					SetTextColour(COLORREF _Colour);
	void					SetWidth(int _Width);

	CPointerArray<CGridRow> m_Rows;
protected:
	int						m_Style;
	int						m_Width;
	COLORREF				m_TextColour;
	eSortOrder				m_SortOrder;
};
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////
