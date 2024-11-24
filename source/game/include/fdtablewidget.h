//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdtablewidget.h
// Description : FDTableWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDTABLEWIDGETUI_H_INCLUDED
#define FDTABLEWIDGETUI_H_INCLUDED

#include <ui/luiwidget.h>
#include <ui/luibuttonwidget.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;
class FDScrollBarWidget;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTableWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDTableWidget : public lUIWidget, public lHasSlots<>
{
  LUI_RTTI( FDTableWidget, lUIWidget );

public:
  static void ClassInitialiser();

  typedef int (*TableSortFunc)(FDTableWidget *, luint32_t, luint32_t, luint32_t);
  typedef void (*CellRenderCallback)(FDTableWidget *, void *, luint32_t, luint32_t, lUIRect);

  FDTableWidget();
  FDTableWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                  luint32_t tableFlags,
                  const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                  const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                  const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDTableWidget();

  // lUIWidget interface

  virtual void    Tick( float delta);
  virtual void     Render();

  virtual lUIPoint GetIdealSize() const;

  virtual void     OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags );
  virtual bool     OnMouseDown( const lUIPoint &, luint32_t );
  virtual bool     OnMouseUp( const lUIPoint &, luint32_t );

  void                Hide();
  void                Show();

  // FDTableWidget interface

  void            ClearAll( bool bClearSelection = true );

  int             AddColumn( const lUICoord &width, lUIAlign align = LUI_CENTER_CENTER );
  int             AddColumn( const lUICoordPercent &pctWidth, lUIAlign align = LUI_CENTER_CENTER  );
  void            RemoveColumn( int index );

  int             AddRows( int numRows );
  void            RemoveRows( int index, int numRows );
  void            SetRows( int numRows );

  void            SetScrollBar(FDScrollBarWidget *pWidget);
  FDScrollBarWidget *GetScrollBar() { return m_scroll; }

  const lUIText & GetText( int row, int col );
  void            SetText( int row, int col, const lUIText & );
  void            SetTextToUpper( int row, int col );
  void            SetTextNumber( int row, int col, int );
  void            SetTextID( int row, int col, int );

  void            SetFixedRowHeight( int height );
  void            SetRowGap( int height );

  //lUIAlign        GetTextAlign( int row, int col );
  //void            SetTextAlign( int row, int col, lUIAlign );

  void            SetBkgColour( int row, int col, int colour );
  luint32_t	      GetBkgColour(const int row, const int col) const;
  void            SetTextColour( int row, int col, int colour );

  void            SetRowTextColour( int row, int colour );  // note can be overridden by SetTextColour
  void            SetRowBkgColour( int row, int colour );

  enum
  {
    TABLE_FLAG_ROW_LINES      = 0x001,  // show dividing lines between rows
    TABLE_FLAG_COLUMN_LINES   = 0x002,  // show dividing lines between columns
    TABLE_FLAG_CELL_SELECT    = 0x004,  // allow selection of individual cells
    TABLE_FLAG_ROW_SELECT     = 0x008,  // allow selection of rows
    //TABLE_FLAG_ROW_EXPAND     = 0x010,  // expandable rows (with preview text + full text) - for news items
    //TABLE_FLAG_TOP_ROW_LINE   = 0x020,  // draw a line below top row
    TABLE_FLAG_BORDER         = 0x040,  // draw a line border around table
    TABLE_FLAG_BACKGROUND     = 0x080,  // draw a rectangle behind table
    TABLE_FLAG_CELLBACKGROUND = 0x100,  // enable backgroud colour for each cell
    TABLE_FLAG_FIXED_FIRSTROW = 0x200,  // always show top row
    TABLE_FLAG_AUTO_SCROLL    = 0x400, // auto scroll table up and down
    TABLE_FLAG_WORD_WRAP	  = 0x800, // word wrap cell contents
    TABLE_FLAG_CUT_OFF	  = 0x1000, // remove excess cell contents
    TABLE_FLAG_CAP_FIRSTROW	  = 0x2000, // remove excess cell contents
    TABLE_FLAG_FLAT_CELLBACKGROUND = 0x4000, // use a flat, instead of a gradient background
    TABLE_FLAG_MULTIPLE_SELECTROW = 0x8000, // select multiple rows
  };

  lSignal3<FDTableWidget *, int, int> SignalCellSelect;
  lSignal2<FDTableWidget *, int>      SignalRowSelect;
  lSignal4<FDTableWidget *, int, int, lUIPoint> SignalCellDropped;
  lSignal4<FDTableWidget *, int, int, lUIPoint> SignalCellDragging;

  lSignal4<FDTableWidget *, luint32_t, luint32_t, lUIRect> SignalCellRendering;

  void GetScrollPosition( int & currentTopRow, bool & bDirection);
  void SetScrollPosition( int currentTopRow, bool bDirection );

  void    SelectRow( int rowIdx, bool bNoSignal = false );
  void    SelectCell( int rowIdx, int colIdx, bool bNoSignal = false );
  int     GetSelectedRow();
  int     GetSelectedCol();

  void CenterOnRow(const int iRow);

  unsigned int GetRowCount() const;
  unsigned int GetColumnCount() const;

  void    ResetScroll();

  void    AllowSlideSelection( const bool value );
  void	  AllowRowSelection(const bool value);
  void	  AllowCellSelection(const bool value);
  void    SetRowFlash( int row, bool bFlash );

  bool IsFirstRowFixed() const;

  void FlashColumnHeader(const int iColumn);
  void SetCellRenderCallback(CellRenderCallback cCallback, void *pData);

  // Sort-related functions.
  void SetSortWhenColumnHeaderClicked(const bool bSort);
  void SetTagDataForRow(const int iRow, const luint32_t uNewData);
  luint32_t GetTagDataForRow(const int iRow) const;
  int FindTagData(luint32_t uiTagData, const bool bSkipHeaderRow) const;
  void SetSortFunctionForColumn(const int iColumn, TableSortFunc cFunc);
  void Resort();
  void SortByColumn(const luint32_t col, const bool bAscending);

  void AddTiebreakerColumn(const int iColumn);
  void ClearTiebreakerColumns();

  static int GenericNumericSort(FDTableWidget *, luint32_t, luint32_t, luint32_t);
  static int GenericMoneySort(FDTableWidget *, luint32_t, luint32_t, luint32_t);
  static int GenericStringSort(FDTableWidget *, luint32_t, luint32_t, luint32_t);
  static int GenericFirstInitialLastNameSort(FDTableWidget *, luint32_t, luint32_t, luint32_t);
  static int GenericDateSort(FDTableWidget *, luint32_t, luint32_t, luint32_t);

  void GetTableLocationOfMouse(const lUIPoint &pos, int &iMouseRow, int &iMouseCol);
  void GetCellRect(lUIRect &cOutRect, const int iRow, const int iColumn);

  void UpdateScrollBarForChangedText();
private:
  void Init();

  void OnScrollUpButton( lUIControlWidget * );
  void OnScrollDownButton( lUIControlWidget * );
  void OnScrollBarChangedPosition(FDScrollBarWidget *pScroll, int iPosition);

  void UpdateDimensions( int rows, int cols );
  void CheckMouse( const lUIPoint &pos, luint32_t buttonFlags );

  void ColumnHeaderClicked(const luint32_t col);
  static int ColumnQsortFunc(const void *, const void *);

  void RefreshRowHeight(const int iRow);

  enum
  {
    TABLE_HOT       = 0x01,
    TABLE_DOWN      = 0x02
  };

  luint32_t             m_state;
  luint32_t             m_tableflags;

  FDScrollBarWidget *m_scroll;

  bool  m_bAutoScrollUp; // up/down
  float m_fAutoScrollTimer;

  // cells
  struct Cell
  {
    Cell()
    {
      bkgcolour = 0;
      textcolour = 0xffffffff;
    }

    lUIText   text;
    luint32_t bkgcolour;
    luint32_t textcolour;
  };

  lArray<Cell>          m_cells;

  // width / height info
  lArray<float>         m_columnSpecifiedWidth;
  lArray<float>         m_columnWidth;
  lArray<float>         m_rowHeight;
  lArray<lUIAlign>      m_columnAlign;
  lArray<luint32_t>     m_rowTagData;
  lArray<int>           m_flashRowIdxList;
  lArray<TableSortFunc> m_columnSortFunctions;
  lArray<luint32_t>     m_columnSortTiebreakerColumns;

  // scroll
  int                   m_currentTopRow;

  // table size
  luint32_t             m_numRows;
  luint32_t             m_numCols;

  int                   m_selectedRow;
  int                   m_selectedCol;

  bool                m_bSliding;
  bool                m_bMouseCaptured;
  bool                m_bDragging;
  bool                m_bSortWhenColumnHeaderClicked;
  int                 m_iLastSortedColumn;
  bool                m_bLastColumnSortWasAscending;
  float                 m_flashTime;
  CellRenderCallback	m_renderCallback;
  void       *        m_renderCallbackData;
  lSharedPtr<lSprite>	m_gradient;

  int     m_fixedRowHeight;
  int     m_rowGap;
  int	  m_iFlashColumnHeader;

  // Used in the column sorting code.
  static int ms_iSortColumn;
  static FDTableWidget *ms_pSortTable;
  static bool ms_bAscendingSort;
};




END_L_NAMESPACE
#endif

