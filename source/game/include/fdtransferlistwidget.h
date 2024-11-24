//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDDumbTablewidget.h
// Description : FDTransferListWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDTRANSFERLISTWIDGETUI_H_INCLUDED
#define FDTRANSFERLISTWIDGETUI_H_INCLUDED

#include <ui/luiwidget.h>
#include <ui/luibuttonwidget.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;
class FDScrollBarWidget;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTransferListWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDTransferListWidget : public lUIWidget, public lHasSlots<>
{
  LUI_RTTI( FDTransferListWidget, lUIWidget );

public:
  static void ClassInitialiser();

  typedef int (*TableSortFunc)(FDTransferListWidget *, luint32_t, luint32_t, luint32_t);
  typedef luint32_t (*CellBkgColourCallback)(FDTransferListWidget *, void *, luint32_t, luint32_t);

  FDTransferListWidget();
  FDTransferListWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                  luint32_t tableFlags,
                  const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                  const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                  const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDTransferListWidget();

  // lUIWidget interface

  virtual void    Tick( float delta);
  virtual void     Render();

  virtual lUIPoint GetIdealSize() const;

  virtual void     OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags );
  virtual bool     OnMouseDown( const lUIPoint &, luint32_t );
  virtual bool     OnMouseUp( const lUIPoint &, luint32_t );

  void                Hide();
  void                Show();

  // FDTransferListWidget interface

  void            ClearAll( bool bClearSelection = true );

  int             AddColumn( const lUICoord &width, lUIAlign align = LUI_CENTER_CENTER );
  int             AddColumn( const lUICoordPercent &pctWidth, lUIAlign align = LUI_CENTER_CENTER  );
  void            RemoveColumn( int index );

  int             AddRows( int numRows );
  void            RemoveRows( int index, int numRows );
  void            SetRows( int numRows );

  void            SetScrollBar(FDScrollBarWidget *pWidget);
  FDScrollBarWidget *GetScrollBar() { return m_scroll; }

  void            SetRowGap( int height );

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
    TABLE_FLAG_WORD_WRAP	  = 0x800, // word wrap cell contents
    TABLE_FLAG_CUT_OFF	  = 0x1000, // remove excess cell contents
    TABLE_FLAG_CAP_FIRSTROW	  = 0x2000, // remove excess cell contents
    TABLE_FLAG_FLAT_CELLBACKGROUND = 0x4000, // use a flat, instead of a gradient background
    TABLE_FLAG_MULTIPLE_SELECTROW = 0x8000, // select multiple rows
  };

  lSignal3<FDTransferListWidget *, int, int> SignalCellSelect;
  lSignal2<FDTransferListWidget *, int>      SignalRowSelect;

  void GetScrollPosition( int & currentTopRow, bool & bDirection);
  void SetScrollPosition( int currentTopRow, bool bDirection );

  lUIText   GetText( int row, int col );
  int       GetNumber( int row, int col );

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

  bool IsFirstRowFixed() const;

  void FlashColumnHeader(const int iColumn);
  void SetCellBkgColourCallback(CellBkgColourCallback cCallback, void *pData);

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

  static int GenericNumericSort(FDTransferListWidget *, luint32_t, luint32_t, luint32_t);
  static int GenericMoneySort(FDTransferListWidget *, luint32_t, luint32_t, luint32_t);
  static int GenericStringSort(FDTransferListWidget *, luint32_t, luint32_t, luint32_t);
  static int GenericFirstInitialLastNameSort(FDTransferListWidget *, luint32_t, luint32_t, luint32_t);

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

  enum
  {
    TABLE_HOT       = 0x01,
    TABLE_DOWN      = 0x02
  };

  luint32_t             m_state;
  luint32_t             m_tableflags;

  FDScrollBarWidget *m_scroll;

  // width / height info
  lArray<float>         m_columnSpecifiedWidth;
  lArray<float>         m_columnWidth;
  lArray<lUIAlign>      m_columnAlign;
  lArray<luint16_t>     m_rowOriginal;
  lArray<luint32_t>     m_rowTagData;
  lArray<TableSortFunc> m_columnSortFunctions;
  lArray<luint32_t>     m_columnSortTiebreakerColumns;

  lArray<lString>       m_playerNames;

  // scroll
  int                   m_currentTopRow;

  // table size
  luint32_t             m_numRows;
  luint32_t             m_numCols;

  int                   m_selectedRow;
  int                   m_selectedCol;

  bool                      m_bSliding;
  bool                      m_bMouseCaptured;
  bool                      m_bSortWhenColumnHeaderClicked;
  int                       m_iLastSortedColumn;
  bool                      m_bLastColumnSortWasAscending;

  CellBkgColourCallback     m_bkgColourCallback;
  void                    * m_bkgColourCallbackData;

  lSharedPtr<lSprite>	m_gradient;

  int     m_rowGap;
  int	  m_iFlashColumnHeader;

  // Used in the column sorting code.
  static int ms_iSortColumn;
  static FDTransferListWidget *ms_pSortTable;
  static bool ms_bAscendingSort;
};




END_L_NAMESPACE
#endif

