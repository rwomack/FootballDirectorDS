//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdtablewidget.cpp
// Description : FDTableWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdtablewidget.h>
#include <framework/lsprite.h>
#include <fdmenuwidget.h>
#include <fdresourcemanager.h>
#include <fdscrollbarwidget.h>
#include <fdtext.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDTableWidget, lGUID(0,0,0,0), 0 );

int FDTableWidget::ms_iSortColumn;
FDTableWidget *FDTableWidget::ms_pSortTable;
bool FDTableWidget::ms_bAscendingSort;


//   - Allow selection of single cell
// Don't allow selection of a static top row

#define TABLELOG                        //lLogFmt

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTableWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void FDTableWidget::ClassInitialiser()
{
}

//////////////////////////////////////////////////////////////////////////
// Constructor.
//////////////////////////////////////////////////////////////////////////
FDTableWidget::FDTableWidget()
  : m_state( 0 ),
    m_numRows( 0 ),
    m_numCols( 0 ),
    m_tableflags( 0 ),
    m_currentTopRow( 0 ),
    m_selectedRow( -1 ),
	m_scroll(NULL),
    m_bSliding(true),
	m_bMouseCaptured(false),
	m_bDragging(false),
    m_flashTime(0.0f),
    m_bAutoScrollUp(false),
    m_fAutoScrollTimer(1.0f),
	m_bSortWhenColumnHeaderClicked(false),
	m_bLastColumnSortWasAscending(false),
	m_iLastSortedColumn(-1),
	m_renderCallback(NULL),
	m_renderCallbackData(NULL),
	m_fixedRowHeight(0),
	m_rowGap(0),
	m_iFlashColumnHeader(-1)
{
	Init();
}

//////////////////////////////////////////////////////////////////////////
// Constructor.
//////////////////////////////////////////////////////////////////////////
FDTableWidget::FDTableWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                              luint32_t tableFlags,
                              const lUIWidgetPosition anchor,
                              const lUIWidgetPosition pos,
                              const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags, id, anchor, pos, size ),
    m_state( 0 ),
    m_numRows( 0 ),
    m_numCols( 0 ),
    m_tableflags( tableFlags ),
    m_currentTopRow( 0 ),
    m_selectedRow( -1 ),
	m_scroll(NULL),
    m_bSliding(true),
	m_bMouseCaptured(false),
	m_bDragging(false),
    m_flashTime(0.0f),
    m_bAutoScrollUp(false),
    m_fAutoScrollTimer(1.0f),
	m_bSortWhenColumnHeaderClicked(false),
	m_bLastColumnSortWasAscending(false),
	m_iLastSortedColumn(-1),
	m_renderCallback(NULL),
	m_renderCallbackData(NULL),
	m_fixedRowHeight(0),
	m_rowGap(0),
	m_iFlashColumnHeader(-1)
{
	Init();
}

//////////////////////////////////////////////////////////////////////////
// Destructor.
//////////////////////////////////////////////////////////////////////////
FDTableWidget::~FDTableWidget()
{
}

void FDTableWidget::Init()
{
	m_gradient = FDResourceManager::GetSprite("uiicon", "grad_white");
}

//////////////////////////////////////////////////////////////////////////
// Removes all cells in the table.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::ClearAll( bool bClearSelection  )
{
  m_cells.Empty();
  m_columnSpecifiedWidth.Empty();
  m_columnWidth.Empty();
  m_rowHeight.Empty();
  m_columnAlign.Empty();

  m_numRows = 0;
  m_numCols = 0;
  m_iFlashColumnHeader = -1;

  m_bAutoScrollUp = false;

  m_currentTopRow = 0;
  if (m_scroll != NULL)
  {
	  m_scroll->SetCurrentPosition(0, false);
	  m_scroll->SetNumberOfItems(0, false);
  }

  if( bClearSelection )
  {
    TABLELOG("Clear selection\n");
    m_selectedRow = -1;
  }
}

//////////////////////////////////////////////////////////////////////////
// Adds a column to the table, with the width specified in UI units.
//////////////////////////////////////////////////////////////////////////
int FDTableWidget::AddColumn( const lUICoord &width, lUIAlign align )
{
  TABLELOG("Column added: %.2f width\n", width.coord );
  m_columnSpecifiedWidth.Push( width.coord );
  m_columnAlign.Push( align );
  UpdateDimensions( m_numRows, m_numCols+1 );
  return m_columnSpecifiedWidth.Size()-1;
}

//////////////////////////////////////////////////////////////////////////
// Adds a column to the table, with the width specified as a percentage.
//////////////////////////////////////////////////////////////////////////
int FDTableWidget::AddColumn( const lUICoordPercent &pctWidth, lUIAlign align )
{
  TABLELOG("Column added: %.2f%% width\n", pctWidth.coord );
  m_columnSpecifiedWidth.Push( -pctWidth.coord );
  m_columnAlign.Push( align );
  UpdateDimensions( m_numRows, m_numCols+1 );
  return m_columnSpecifiedWidth.Size()-1;
}

//////////////////////////////////////////////////////////////////////////
// Removes a column from the table.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::RemoveColumn( int index )
{
  TABLELOG("Removed column %d\n", index);
  m_columnSpecifiedWidth.Remove( index );
  m_columnAlign.Remove( index );
  UpdateDimensions( m_numRows, m_numCols-1 );
}

//////////////////////////////////////////////////////////////////////////
// Removes the specified rows from the table.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::RemoveRows( int index, int numRows )
{
	lAssert(numRows <= m_numRows);

	// Move up all cell contents and other tracking information, and then update the
	// dimensions of the table.
	m_rowHeight.Remove(index, numRows);
	m_rowTagData.Remove(index, numRows);
	m_cells.Remove(index * m_numCols, numRows * m_numCols);

  // adjust rows in flash list
  for(int i = 0; i < m_flashRowIdxList.Size(); i++)
  {
    if( m_flashRowIdxList(i) >= index || m_flashRowIdxList(i) < index+numRows )
    {
      // remove flash which was indexed
      m_flashRowIdxList.Remove( i );
      continue;
    }

    if( m_flashRowIdxList(i) >= index+numRows )
    {
      // subtract from index after
      m_flashRowIdxList(i) -= numRows;
    }
  }


	if (m_currentTopRow >= (m_numRows - numRows))
		m_currentTopRow = m_numRows - numRows - 1;

	if (m_selectedRow >= (m_numRows - numRows))
	{
		int iNewSelectedRow = m_numRows - numRows - 1;
		if ((m_tableflags & TABLE_FLAG_FIXED_FIRSTROW) && (iNewSelectedRow == 0))
			iNewSelectedRow = -1;

		SelectRow(iNewSelectedRow);
	}
	else
	{
		SelectRow(m_selectedRow);
	}

	UpdateDimensions( m_numRows - numRows, m_numCols );
	TABLELOG("Removed %d rows\n", numRows);
}

//////////////////////////////////////////////////////////////////////////
// Adds the specified number of rows to the table. Returns the highest valid row index.
//////////////////////////////////////////////////////////////////////////
int FDTableWidget::AddRows( int numRows )
{
  lAssert((m_numCols > 0) && "Because of the way the table code is set up, you need to add your columns before adding rows...otherwise weird things will happen with scrolling");
  TABLELOG("Added %d rows\n", numRows);
  UpdateDimensions( m_numRows + numRows, m_numCols );
  return m_numRows-1;
}

//////////////////////////////////////////////////////////////////////////
// Sets the number of rows in the table.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::SetRows( int numRows )
{
  TABLELOG("Modified to %d rows\n", numRows);
  UpdateDimensions( numRows, m_numCols );
}

//////////////////////////////////////////////////////////////////////////
// Sets the "tag data" associated with the row. Tag data has a user-defined
// meaning, but usually represents the game data corresponding to the table row.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::SetTagDataForRow(const int iRow, const luint32_t uNewData)
{
	m_rowTagData(iRow) = uNewData;
}

//////////////////////////////////////////////////////////////////////////
// Returns the tag data associated with the row.
//////////////////////////////////////////////////////////////////////////
luint32_t FDTableWidget::GetTagDataForRow(const int iRow) const
{
	return m_rowTagData(iRow);
}

//////////////////////////////////////////////////////////////////////////
// Returns the row index associated with the specified tag data.
//////////////////////////////////////////////////////////////////////////
int FDTableWidget::FindTagData(luint32_t uiTagData, const bool bSkipHeaderRow) const
{
	const int iStartingIndex = (bSkipHeaderRow ? 1 : 0);
	for (int i = iStartingIndex; i < m_numRows; i++)
	{
		if (m_rowTagData(i) == uiTagData)
			return i;
	}

	return -1;
}


//////////////////////////////////////////////////////////////////////////
// Internal function used to update the size of the table, creating and
// removing data as necessary.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::UpdateDimensions( int rows, int cols )
{
  lSharedPtr<lFont> font = GetFont();
  lUIRect rect = WidgetToScreen( GetRect() );

  /*if( m_tableflags & TABLE_FLAG_BORDER )
  {
    rect.Shrink( 1, 1 );
  }*/

  // Resize the array for tag data.
  const int iOldRowTagSize = m_rowTagData.Size();
  m_rowTagData.SetSize(rows);
  for (int i = iOldRowTagSize; i < rows; i++)
	  m_rowTagData(i) = NULL;

  // adjust rows in flash list
  for(int i = 0; i < m_flashRowIdxList.Size(); i++)
  {
    if( m_flashRowIdxList(i) >= rows )
    {
      // remove flash which was indexed
      m_flashRowIdxList.Remove( i );
      continue;
    }
  }

  // Resize the array for column sorting functions.
  const int iOldColumnSortSize = m_columnSortFunctions.Size();
  m_columnSortFunctions.SetSize(cols);
  for (int i = iOldColumnSortSize; i < cols; i++)
	  m_columnSortFunctions(i) = NULL;

  // work out new column widths
  m_columnSpecifiedWidth.SetSize(cols);
  if( m_columnSpecifiedWidth.Size() != m_columnWidth.Size() )
  {
    float totalWidth = 0.0f;
    m_columnWidth.SetSize( m_columnSpecifiedWidth.Size() );
    for(int i = 0;i < m_columnWidth.Size(); i++)
    {
      float width = m_columnSpecifiedWidth(i);
      if( m_columnSpecifiedWidth(i) < 0.0f )
      {
        // percentage of width of rect
        width = rect.w * -m_columnSpecifiedWidth(i);
      }

      TABLELOG("Column %d: width %.2f\n",i, width);

      m_columnWidth(i) = width;
      totalWidth += width;
    }

    if( totalWidth > rect.w )
    {
      TABLELOG("ERROR: Table width more than size of table widget - no horizontal scrolling\n");
    }
  }

  // setup data for cells
  if( m_numRows != rows || m_numCols != cols )
  {
    TABLELOG("Updating table size from %dx%d to %dx%d\n", m_numRows, m_numCols, rows, cols);
    //lLogFmt("%x: Updating table size from %dx%d to %dx%d = %d bytes (Cell=%d), String=%d\n", this, m_numRows, m_numCols, rows, cols, sizeof(Cell)*rows*cols,sizeof(Cell),sizeof(lString));
    lArray<Cell> curCells = m_cells;
    m_cells.SetSize( rows * cols );

    if( curCells.Size() )
    {
      int rowcopy = lmMin<int>( m_numRows, rows );
      int colcopy = lmMin<int>( m_numCols, cols );

      // copy data from old to new
      for(int r = 0; r < rowcopy; r++)
      {
        for(int c = 0; c < colcopy; c++)
        {
          m_cells( r * cols + c ) = curCells( r * m_numCols + c );
        }
      }
    }

    m_numRows = rows;
    m_numCols = cols;
  }

  int hCount = 0;

  // work out row heights
  if( m_numRows != m_rowHeight.Size() )
  {
    m_rowHeight.SetSize( m_numRows );
    for( int i = 0; i < m_numRows; i++ )
    {
      if( m_fixedRowHeight != 0 )
        m_rowHeight(i) = float( m_fixedRowHeight );
      else
	  {
		RefreshRowHeight(i);
	  }
    }
  }

  // Find extra space at end of table if there is some left and squish to nearest row
  // row height never really changes differs from other rows
  if( m_rowHeight.Size() > 0 )
  {
    int diff = 0;
    if( (int) m_rowHeight(0) > 0 )
     diff = (int) rect.h % (int) m_rowHeight(0);

    {
      rect.h -= diff;
      lUIRect nrect = ScreenToWidget( rect );
      Move( nrect );
    }
  }

  UpdateScrollBarForChangedText();
}

//////////////////////////////////////////////////////////////////////////
// Returns the desired size for the table widget.
//////////////////////////////////////////////////////////////////////////
lUIPoint FDTableWidget::GetIdealSize() const
{
  lSharedPtr<lFont> font = GetFont();

  if( m_fixedRowHeight != 0 )
  {
    return lUIPoint( font->DefaultPixelWidth() * 5 * m_numCols, float( m_fixedRowHeight ) * m_numRows );
  }
  else
  {
    // calculate largest cell based on text + font and multiply up for full size
    return lUIPoint( font->DefaultPixelWidth() * 5 * m_numCols, font->DefaultPixelHeight() * m_numRows );
  }
}

//////////////////////////////////////////////////////////////////////////
// Update function, used to flash cells when necessary.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::Tick( float delta )
{
  // flash...
  float interval = 1.0f;

  if( m_flashTime < interval )
  {
    m_flashTime += delta;
  } else
  {
    m_flashTime -= interval;

    // toggle colours for selected items
    int curRow = 0;
    int curCol = 0;
    for( int i = 0; i < m_cells.Size(); i++ )
    {
      // is row in flash list
      if( m_flashRowIdxList.Find( curRow ) != -1 )
      {
        //switch foreground and background colours
        luint32_t temp = m_cells(i).bkgcolour;
        m_cells(i).bkgcolour = m_cells(i).textcolour;
        m_cells(i).textcolour = temp;
      }

      // increment row/col
      curCol++;
      if( curCol >= m_numCols )
      {
        curRow++;
        curCol = 0;
      }
    }
  }

  if( (m_tableflags & TABLE_FLAG_AUTO_SCROLL) && m_cells.Size() > 0 )
  {
    if( m_fAutoScrollTimer > 0.0f )
    {
      m_fAutoScrollTimer -= delta;
    } else
    {
      m_fAutoScrollTimer += 2.0f;

      lSharedPtr<lFont> font = GetFont();
      lUIRect rect = WidgetToScreen( GetRect() );
      lUIPoint point = GetIdealSize();

      if( rect.h <= point.y )
      {
        float diff = point.y - rect.h;

        int numLines = int( diff / font->DefaultPixelHeight());

        if( m_fixedRowHeight != 0 )
          numLines = int( diff / m_fixedRowHeight );

        // TODO: Make smoother/ pixel scrolling

        if( !m_bAutoScrollUp && m_currentTopRow > numLines)
        {
          lLogFmt("Auto Scroll Up\n");
          m_bAutoScrollUp = true;
        }
        else if( m_bAutoScrollUp && m_currentTopRow == 0 )
        {
          lLogFmt("Auto Scroll Down\n");
          m_bAutoScrollUp = false;
        }

        if( !m_bAutoScrollUp )
        {
          m_currentTopRow++;
        }
        else
        {
          m_currentTopRow--;
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// Rendering function.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::Render()
{
  lUIRect rect = WidgetToScreen( GetRect() );

  /*if( m_tableflags & TABLE_FLAG_BORDER )
  {
    rect.Shrink( 1, 1 );
  }*/

  // draw background
  if( m_tableflags & TABLE_FLAG_BACKGROUND )
  {
    lSpriteManager::DrawRect2( luint32_t(rect.x), luint32_t(rect.y), luint32_t(rect.w), luint32_t(rect.h), L_ARGB(120, 0, 0, 0) );
  }

  // draw dividing lines

//  if( m_tableflags & TABLE_FLAG_ROW_LINES )
//  {
//    // draw a line between every two rows
//    if( m_numRows )
//    {
//      float cury = rect.y + m_rowHeight(m_currentTopRow);
//      int startidx = m_currentTopRow+1;
//
//      if( m_tableflags & TABLE_FLAG_FIXED_FIRSTROW )
//      {
//        startidx = m_currentTopRow;
//        cury = rect.y + m_rowHeight(0);
//      }
//
//      for(int i = startidx; i < m_numRows; i++)
//      {
//        if( cury > rect.y + rect.h )
//          break;
//        lSpriteManager::DrawLine2( luint32_t(rect.x), luint32_t(cury), luint32_t(rect.x + rect.w), luint32_t(cury), L_ARGB(200,255,255,255) );
//        cury += m_rowHeight(i);
//      }
//    }
//  }
//
//  if( m_tableflags & TABLE_FLAG_COLUMN_LINES )
//  {
//    // draw a line between every two columns
//    if( m_numCols )
//    {
//      float curx = rect.x + m_columnWidth(0);
//
//      for(int i = 1; i < m_numCols; i++)
//      {
//        lSpriteManager::DrawLine2( luint32_t(curx), luint32_t(rect.y), luint32_t(curx), luint32_t(rect.y + rect.h), L_ARGB(200,255,255,255) );
//        curx += m_columnWidth(i);
//      }
//    }
//  }
//
//  if( m_tableflags & TABLE_FLAG_BORDER )
//  {
//    //rect.Expand( 1, 1 );
//
//    // top left to top right
//    lSpriteManager::DrawLine2( luint32_t(rect.x), luint32_t(rect.y), luint32_t(rect.x + rect.w), luint32_t(rect.y), L_ARGB(200,255,255,255) );
//
//    // top left to bottom left
//    lSpriteManager::DrawLine2( luint32_t(rect.x), luint32_t(rect.y), luint32_t(rect.x), luint32_t(rect.y + rect.h), L_ARGB(200,255,255,255) );
//
//    // bottom left to bottom right
//    lSpriteManager::DrawLine2( luint32_t(rect.x), luint32_t(rect.y + rect.h), luint32_t(rect.x + rect.w), luint32_t(rect.y + rect.h), L_ARGB(200,255,255,255) );
//
//    // top right to bottom right
//    lSpriteManager::DrawLine2( luint32_t(rect.x + rect.w), luint32_t(rect.y), luint32_t(rect.x + rect.w), luint32_t(rect.y + rect.h), L_ARGB(200,255,255,255) );
//    //rect.Shrink( 1, 1 );
//  }

//  if( m_tableflags & TABLE_FLAG_CELLBACKGROUND )
  {
    //TODO: Don't draw cell background if selected as we'll draw another colour soon
    float cury = rect.y;
    int   curidx = 0;

    if( !(m_tableflags & TABLE_FLAG_FIXED_FIRSTROW) )
    {
      // start at top of scroll list
      curidx = m_currentTopRow;
    }

    for(int row = curidx; row < m_numRows; row++)
    {
      float curx = rect.x;
      float rowheight = m_rowHeight(row);

      if( cury + rowheight > rect.y + rect.h )
      {
        // can't fit row in
        break;
      }

      if( (m_tableflags & TABLE_FLAG_CELLBACKGROUND) )
      {
        // draw cell backgrounds if set
        for(int col = 0; col < m_numCols; col++)
        {
          float colwidth = m_columnWidth(col);
		  luint32_t uiBkgColour = 0;

		  if ((row == 0) && (m_iFlashColumnHeader == col) && (m_tableflags & TABLE_FLAG_FIXED_FIRSTROW))
		  {
			  uiBkgColour = L_ARGB(0xFF, 0x50, 0x50, 0xA0);
		  }
		  else
			  uiBkgColour = m_cells(row * m_numCols + col).bkgcolour;

          if( uiBkgColour != 0 )
          {
            //TODO: Group up all columns on row if all are set to same (if we run short on CPU / Polys)
            //TODO: Accuracy wrong somewhere in pipeline? Line between cell background and line

			if (m_tableflags & TABLE_FLAG_FLAT_CELLBACKGROUND)
			{
	            lSpriteManager::DrawRect2( luint32_t(curx), luint32_t(cury), luint32_t(colwidth), luint32_t(rowheight),
		                                   uiBkgColour );
			}
			else
			{
				m_gradient->SetX(curx);
				m_gradient->SetY(cury);
				m_gradient->SetWidth(colwidth);
				m_gradient->SetHeight(rowheight);
				m_gradient->SetColour(uiBkgColour);
				// Note: Alpha doesnt seem to work too well on the DS
				//m_gradient->SetAlphaEnable(true);
				//m_gradient->SetAlpha(m_cells(row * m_numCols + col).bkgcolour >> 24);
				m_gradient->Render();
			}
          }

          curx += colwidth;
        }
      }

      if (( m_selectedRow == row ) &&
		  (FDMenuWidget::ms_bMenuActive == false) &&
		  ((m_selectedRow != 0) || ((m_tableflags & TABLE_FLAG_FIXED_FIRSTROW) == 0)))
      {
        float fullwidth = 0;
        for(int col = 0; col < m_numCols; col++)
        {
          fullwidth += m_columnWidth(col);
        }

        // draw selection bar
        lSpriteManager::DrawRect2( luint32_t(rect.x), luint32_t(cury), luint32_t(fullwidth), luint32_t(rowheight),
                                    L_ARGB( 110, 220, 240, 220 ) );
      }

      cury += rowheight + m_rowGap;

      if( m_tableflags & TABLE_FLAG_FIXED_FIRSTROW && row == 0)
      {
        // have displayed first row, now skip to scroll position
        row = m_currentTopRow;
      }
    }
  }

  // write text
  {
    lSharedPtr<lFont> font = GetFont();
    float cury = rect.y;
    int   curidx = 0;

    if( !(m_tableflags & TABLE_FLAG_FIXED_FIRSTROW) )
    {
      // start at top of scroll list
      curidx = m_currentTopRow;
    }

    for(int row = curidx; row < m_numRows; row++)
    {
      float curx = rect.x;
      float rowheight = m_rowHeight(row);

      if( cury + rowheight > rect.y + rect.h )
      {
        // can't fit row in
        break;
      }

      for(int col = 0; col < m_numCols; col++)
      {
        float colwidth = m_columnWidth(col);

        if( m_cells(row * m_numCols + col).text.str.Size() )
        {
          int strwidth, strheight;

		  if (m_tableflags & TABLE_FLAG_WORD_WRAP)
		  {
			lString sSplitText = SplitText(font.Get(), m_cells(row * m_numCols + col).text.str, m_columnWidth(col) / lSpriteManager::GetDisplayWidth());
			FDTextGetStringBound( font.Get(), strwidth, strheight, sSplitText );

			lUIPoint cellOffset = lUICalcAlign( m_columnAlign(col), lUIPoint( colwidth, rowheight ), lUIPoint( strwidth, strheight ) );

			font->SetColour( m_cells(row * m_numCols + col).textcolour );

			FDTextPrint(font.Get(), int(curx + cellOffset.x), int(cury + cellOffset.y),
						sSplitText.CStr());
		  }
		  else
		  {
			FDTextGetStringBound( font.Get(), strwidth, strheight, m_cells(row * m_numCols + col).text.str );

			lUIPoint cellOffset = lUICalcAlign( m_columnAlign(col), lUIPoint( colwidth, rowheight ), lUIPoint( strwidth, strheight ) );

			font->SetColour( m_cells(row * m_numCols + col).textcolour );

			FDTextPrint(font.Get(), int(curx + cellOffset.x), int(cury + cellOffset.y),
						m_cells(row * m_numCols + col).text.str);
		  }
        }

		// If we have a render callback, call it for this cell.
		lUIRect cCellRect = lUIRect(curx, cury, colwidth, rowheight);
		if (m_renderCallback)
		{
			(*m_renderCallback)(this, m_renderCallbackData, row, col, cCellRect);
		} else
		{
		  SignalCellRendering(this, row, col, cCellRect);
		}

        curx += colwidth;
      }

      cury += rowheight + m_rowGap;

      if( m_tableflags & TABLE_FLAG_FIXED_FIRSTROW && row == 0)
      {
        // have displayed first row, now skip to scroll position
        row = m_currentTopRow;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
// Mouse handler for when the mouse is moved inside the widget.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags )
{
  SuperClass::OnMouse( p, mouseDownFlags, mouseUpFlags );

  if( m_bSliding && !m_bDragging )
  {
    CheckMouse( p, 0 );
  }

  if (m_bMouseCaptured && (m_selectedRow >= 0))
  {
	  if ((m_tableflags & TABLE_FLAG_ROW_SELECT) == 0)
	  {
	    if (m_selectedCol >= 0)
		{
	      m_bDragging = true;
		  SignalCellDragging.Emit(this, m_selectedRow, m_selectedCol, p);
		}
	  }
	  else
	  {
	    m_bDragging = true;
	    SignalCellDragging.Emit(this, m_selectedRow, 1, p);
	  }
  }
}

//////////////////////////////////////////////////////////////////////////
// Given a mouse position, returns the row and column that the mouse
// occupies.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::GetTableLocationOfMouse(const lUIPoint &pos, int &iMouseRow, int &iMouseCol)
{
	iMouseRow = -1;
	iMouseCol = -1;

    lUIPoint mpos = WidgetClientToScreen( pos );
    lUIRect rect = WidgetToScreen( GetRect() );
    lSharedPtr<lFont> font = GetFont();
    float cury = rect.y;
    int   curidx = 0;

    if( !(m_tableflags & TABLE_FLAG_FIXED_FIRSTROW) )
    {
      // start at top of scroll list
      curidx = m_currentTopRow;
    }

    for(int row = curidx; row < m_numRows; row++)
    {
		float curx = rect.x;
		float rowheight = m_rowHeight(row);

		for( int col = 0; col < m_numCols; col++ )
		{
			float colwidth = m_columnWidth(col);

			if( mpos.x >= curx && mpos.x <= curx + colwidth )
			{
				if( mpos.y >= cury && mpos.y <= (cury+rowheight) )
				{
					// mouse over row
					iMouseRow = row;
					iMouseCol = col;
					return;
				}
			}
			curx += colwidth;
		}

		cury += rowheight;

		if( m_tableflags & TABLE_FLAG_FIXED_FIRSTROW && row == 0)
		{
			// have displayed first row, now skip to scroll position
			row = m_currentTopRow;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Called to determine the selection during mouse operations.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::CheckMouse( const lUIPoint &pos, luint32_t buttonFlags )
{
  if( m_tableflags & TABLE_FLAG_ROW_SELECT )
  {
    lUIPoint mpos = WidgetClientToScreen( pos );
    lUIRect rect = WidgetToScreen( GetRect() );
    lSharedPtr<lFont> font = GetFont();
    float cury = rect.y;
    int   curidx = 0;

    if( !(m_tableflags & TABLE_FLAG_FIXED_FIRSTROW) )
    {
      // start at top of scroll list
      curidx = m_currentTopRow;
    }

    for(int row = curidx; row < m_numRows; row++)
    {
      float curx = rect.x;
      float rowheight = m_rowHeight(row);

      if( mpos.x >= curx && mpos.x <= (rect.x+rect.w) )
      {
        if( mpos.y >= cury && mpos.y <= (cury+rowheight) )
        {
          // mouse over row
          TABLELOG("Mouse over row %d\n", row);

          if( (m_tableflags & TABLE_FLAG_FIXED_FIRSTROW) && row == 0 )
          {
            // don't allow selection of first row
          } else
          {
            m_selectedRow = row;
          }
        }
      }

      cury += rowheight + m_rowGap;

      if( m_tableflags & TABLE_FLAG_FIXED_FIRSTROW && row == 0)
      {
        // have displayed first row, now skip to scroll position
        row = m_currentTopRow;
      }
    }
  }
  else if( m_tableflags & TABLE_FLAG_CELL_SELECT )
  {
	  int iMouseRow, iMouseCol;
	  GetTableLocationOfMouse(pos, iMouseRow, iMouseCol);

	  if (iMouseRow != -1)
		  m_selectedRow = iMouseRow;
	  if (iMouseCol != -1)
		  m_selectedCol = iMouseCol;
  }
}

//////////////////////////////////////////////////////////////////////////
// Called when a mouse button is pressed in the table.
//////////////////////////////////////////////////////////////////////////
bool FDTableWidget::OnMouseDown(const lUIPoint &pos, luint32_t buttonFlags)
{
  SuperClass::OnMouseDown(pos,buttonFlags);

  if( !m_bSliding )
  {
    CheckMouse( pos, 0 );
  }

  CaptureMouse();
  m_bMouseCaptured = true;

//  if( m_tableflags & TABLE_FLAG_ROW_SELECT )
//  {
//    // do signal
//    TABLELOG("Mouse down %d\n", m_selectedRow);
//    SignalRowSelect( this, m_selectedRow );
//  }

//  m_state |= SLIDER_DOWN;

  return true;
}

//////////////////////////////////////////////////////////////////////////
// Called when a mouse button is released in the table.
//////////////////////////////////////////////////////////////////////////
bool FDTableWidget::OnMouseUp(const lUIPoint &pos, luint32_t buttonFlags)
{
  SuperClass::OnMouseUp(pos,buttonFlags);

  if( m_bMouseCaptured )
  {
    ReleaseMouse();
	m_bMouseCaptured = false;
  }

  if (m_bDragging == true)
  {
	  if ((m_tableflags & TABLE_FLAG_ROW_SELECT) && (m_selectedRow >= 0))
		SignalCellDropped(this, m_selectedRow, m_selectedCol, pos);
	  else if ((m_selectedRow >= 0) && (m_selectedCol >= 0))
	    SignalCellDropped(this, m_selectedRow, m_selectedCol, pos);

	  m_bDragging = false;
  }

  CheckMouse(pos, 0);
  if( m_tableflags & TABLE_FLAG_ROW_SELECT )
  {
    // do signal
    TABLELOG("Mouse up %d\n", m_selectedRow);
	SignalRowSelect( this, m_selectedRow );
  }
  else if( m_tableflags & TABLE_FLAG_CELL_SELECT )
  {
    SignalCellSelect( this, m_selectedRow, m_selectedCol );
  }

  if (m_bSortWhenColumnHeaderClicked)
  {
	  int iMouseRow, iMouseCol;
	  GetTableLocationOfMouse(pos, iMouseRow, iMouseCol);

	  if ((iMouseRow == 0) && (iMouseCol >= 0))
		ColumnHeaderClicked(iMouseCol);
  }

//  m_state &= ~SLIDER_DOWN;

  return true;
}

//////////////////////////////////////////////////////////////////////////
// Called when the widget is hidden.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::Hide()
{
  SuperClass::Hide();

  if (m_scroll != NULL)
	  m_scroll->Hide();
}

//////////////////////////////////////////////////////////////////////////
// Called when the widget is shown.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::Show()
{
  SuperClass::Show();

  if (m_scroll != NULL)
	  m_scroll->Show();
}

// set all cells background colour for this row
void FDTableWidget::SetRowBkgColour( int row, int colour )
{
  for(int i = 0; i < m_numCols; i++)
  {
    m_cells( row * m_numCols + i ).bkgcolour = colour;
  }
}

void FDTableWidget::SetRowTextColour( int row, int colour )
{
  for(int i = 0; i < m_numCols; i++)
  {
    m_cells( row * m_numCols + i ).textcolour = colour;
  }
}


void FDTableWidget::SetBkgColour( int row, int col, int colour )
{
  m_cells( row * m_numCols + col ).bkgcolour = colour;
}

luint32_t FDTableWidget::GetBkgColour(const int row, const int col) const
{
	return m_cells(row * m_numCols + col).bkgcolour;
}

void FDTableWidget::SetTextColour( int row, int col, int colour )
{
  m_cells( row * m_numCols + col ).textcolour = colour;
}

const lUIText & FDTableWidget::GetText( int row, int col )
{
  return m_cells( row * m_numCols + col ).text;
}

void FDTableWidget::SetTextToUpper( int row, int col )
{
  m_cells( row * m_numCols + col ).text.ToUpper();
}

void FDTableWidget::SetText( int row, int col, const lUIText &text )
{
  m_cells( row * m_numCols + col ).text = text;

  // Caps first row
  if( m_tableflags & TABLE_FLAG_CAP_FIRSTROW && row == 0 )
  {
    m_cells( row * m_numCols + col ).text.ToUpper();
  }

  // If this is a word wrapping table, then determine if we need to update the row height.
  if (m_tableflags & TABLE_FLAG_WORD_WRAP)
  {
	  float fWidth = m_columnWidth(col);
	  float fHeight = 0.0f;
	  lString sSplit = SplitText(GetFont().Get(), text.str, fWidth / lSpriteManager::GetDisplayWidth());
	  FDTextGetStringBound(GetFont().Get(), fWidth, fHeight, sSplit.CStr());

	  m_rowHeight(row) = lmMax(fHeight * lSpriteManager::GetDisplayHeight(), m_rowHeight(row));
  }
  else if (m_tableflags & TABLE_FLAG_CUT_OFF)
  {
    lString str = text.str;

    int Width = 0;
	  int Height = 0;
	  FDTextGetStringBound(GetFont().Get(), Width, Height, str.CStr() );

	  //lLogFmt("Text Width: %d\nTable width: %d\n", (int) Width, (int) m_columnWidth(col) );

    if( m_columnWidth == 0.0f ) return;

	  if( Width > m_columnWidth(col)  )
	  {
	    bool bIsLink = false;
	    bool bDone = false;

	    for(int i = 0; i < str.Size()-1; i++)
	    {
	      if( str(i) == '#' && str(i+1) == '@')
	      {
	        bIsLink = true;
	        break;
        }
	    }

	    while( !bDone )
	    {
	      FDTextGetStringBound(GetFont().Get(), Width, Height, str.CStr() );
	      if( Width > m_columnWidth(col) - 5 )
	      {
	        str = str.Left( str.Size()-1 );
	        //lLogFmt("%s\n", str.CStr() );
	        if( str.Size()  == 2 )
	          bDone = true;
	      }
	      else
	      {
	        bDone = true;
        }
	    }

	    if( bIsLink )
	      str += "@";
	    m_cells( row * m_numCols + col ).text = lUIText( str );
	  }
  }
}

void FDTableWidget::SetTextNumber( int row, int col, int num )
{
  char str[32];
  sprintf( str, "%d", num );
  m_cells( row * m_numCols + col ).text = lUIText( str );
}

void FDTableWidget::SetTextID( int row, int col, int id )
{
  m_cells( row * m_numCols + col ).text = lUIText( id );
}


//lUIAlign FDTableWidget::GetTextAlign( int row, int col )
//{
//  return m_cells( row * m_numCols + col ).align;
//}
//
//void FDTableWidget::SetTextAlign( int row, int col, lUIAlign align )
//{
//  m_cells( row * m_numCols + col ).align = align;
//}

void FDTableWidget::SetScrollBar(FDScrollBarWidget *pWidget)
{
	if (m_scroll != NULL)
	{
		m_scroll->SignalPositionChanged.Disconnect(this, &FDTableWidget::OnScrollBarChangedPosition);
	}

	m_scroll = pWidget;

	if (m_scroll != NULL)
	{
		m_scroll->SetNumberOfItems(m_numRows, false);
		m_scroll->SetItemsViewedAtOnce((int)(GetRect().h / GetFont()->DefaultPixelHeight()), false);
		m_scroll->SetCurrentPosition(m_currentTopRow, false);
		m_scroll->SignalPositionChanged.Connect(this, &FDTableWidget::OnScrollBarChangedPosition);
	}
}


void FDTableWidget::OnScrollUpButton( lUIControlWidget * )
{
  TABLELOG("Scroll up\n");
  m_currentTopRow--;
  if( m_currentTopRow < 0 ) m_currentTopRow = 0;
}

void FDTableWidget::OnScrollDownButton( lUIControlWidget * )
{
  TABLELOG("Scroll down\n");
  m_currentTopRow++;
  if( m_currentTopRow >= m_numRows ) m_currentTopRow = m_numRows-1;
  if( m_numRows == 0 ) m_currentTopRow = 0;
}

void FDTableWidget::OnScrollBarChangedPosition(FDScrollBarWidget *pScroll, int iPosition)
{
	m_currentTopRow = iPosition;
}

void FDTableWidget::GetScrollPosition( int & currentTopRow, bool & bDirection )
{
  currentTopRow = m_currentTopRow;
  bDirection = m_bAutoScrollUp;
}

void FDTableWidget::SetScrollPosition( int currentTopRow, bool bDirection )
{
  m_currentTopRow = currentTopRow;
  m_bAutoScrollUp = bDirection;

  if ( m_currentTopRow >= m_numRows ) m_currentTopRow = m_numRows-1;
  if (m_numRows == 0)
	  m_currentTopRow = 0;
}

void    FDTableWidget::ResetScroll()
{
  m_currentTopRow = 0;
  if (m_scroll != NULL)
  {
	  m_scroll->SetCurrentPosition(0, false);
  }
}

void    FDTableWidget::SelectRow( int rowIdx, bool bNoSignal )
{
  m_selectedRow = rowIdx;
  TABLELOG("Select row %d\n", m_selectedRow);

  if( !bNoSignal )
  {
    SignalRowSelect( this, m_selectedRow );
  }
}

void    FDTableWidget::SelectCell( int rowIdx, int colIdx, bool bNoSignal )
{
  m_selectedRow = rowIdx;
  m_selectedCol = colIdx;
  TABLELOG("Select col %d,%d\n", m_selectedRow, m_selectedCol);

  if( !bNoSignal )
  {
    SignalCellSelect( this, m_selectedRow, m_selectedCol );
  }
}


//////////////////////////////////////////////////////////////////////////
// Sets the specified row as the selection, and centers the table view
// on it.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::CenterOnRow(const int iRow)
{
	const float fHeight = GetFont()->DefaultPixelHeight();
	const int iRowsPerPage = (int)(GetRect().h / fHeight);
	m_currentTopRow = (iRow - (iRowsPerPage / 2));

	m_currentTopRow = lmMax(0, lmMin(m_currentTopRow, (int)m_numRows - iRowsPerPage));

	if (m_scroll != NULL)
	{
		m_scroll->SetCurrentPosition(m_currentTopRow, false);
	}
}

int     FDTableWidget::GetSelectedRow()
{
  return m_selectedRow;
}

int     FDTableWidget::GetSelectedCol()
{
  return m_selectedCol;
}

void    FDTableWidget::AllowSlideSelection(const bool value)
{
  m_bSliding = value;
}

void	FDTableWidget::AllowRowSelection(const bool value)
{
	if (value)
		m_tableflags |= TABLE_FLAG_ROW_SELECT;
	else
		m_tableflags &= ~TABLE_FLAG_ROW_SELECT;
}

void FDTableWidget::AllowCellSelection(const bool value)
{
	if (value)
		m_tableflags |= TABLE_FLAG_CELL_SELECT;
	else
		m_tableflags &= ~TABLE_FLAG_CELL_SELECT;
}

void    FDTableWidget::SetRowFlash( int row, bool bFlash )
{
  if( bFlash )
  {
    m_flashRowIdxList.AddUniqueElement( row );
  } else
  {
    m_flashRowIdxList.RemoveElement( row );
  }
}

unsigned int FDTableWidget::GetRowCount() const
{
	return m_numRows;
}

unsigned int FDTableWidget::GetColumnCount() const
{
	return m_numCols;
}

//////////////////////////////////////////////////////////////////////////
// Utility function to sort the table by a particular column, and in
// ascending or descending order.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::SortByColumn(const luint32_t col, const bool bAscending)
{
	unsigned int *pRows = FD_GAME_NEW unsigned int[GetRowCount()];

	for (unsigned int i = 0; i < GetRowCount(); i++)
		pRows[i] = i;

	ms_iSortColumn = col;
	ms_pSortTable = this;
	ms_bAscendingSort = bAscending;

	unsigned int *pRowsToSort = ((m_tableflags & TABLE_FLAG_FIXED_FIRSTROW) != 0) ? &pRows[1] : pRows;
	const int iRowsToSort = ((m_tableflags & TABLE_FLAG_FIXED_FIRSTROW) != 0) ? GetRowCount() - 1 : GetRowCount();
	::qsort(pRowsToSort, iRowsToSort, sizeof(unsigned int), ColumnQsortFunc);

	// Now, resort the actual table cells based on this.
	lArray<Cell> cNewCells;
  lArray<luint32_t> cNewTagData;
	cNewCells.SetSize(m_cells.Size());
	cNewTagData.SetSize(GetRowCount());

	for (unsigned int i = 0; i < GetRowCount(); i++)
	{
		for (unsigned int j = 0; j < GetColumnCount(); j++)
		{
			cNewCells((i * GetColumnCount()) + j) = m_cells((pRows[i] * GetColumnCount()) + j);
		}
		cNewTagData(i) = m_rowTagData(pRows[i]);
	}

	m_cells = cNewCells;
	m_rowTagData = cNewTagData;

	delete[] pRows;
}

//////////////////////////////////////////////////////////////////////////
// Function passed to qsort() for sorting the table. Calls user-specified
// functions to actually compute the sort.
//////////////////////////////////////////////////////////////////////////
int FDTableWidget::ColumnQsortFunc(const void *p1, const void *p2)
{
	// Check on the column sorting function for the column.
	TableSortFunc pFunc = ms_pSortTable->m_columnSortFunctions(ms_iSortColumn);
	unsigned int uiRow1 = *((unsigned int *)p1);
	unsigned int uiRow2 = *((unsigned int *)p2);
	if (pFunc != NULL)
	{
		const int iSort = (*pFunc)(ms_pSortTable, uiRow1, uiRow2, ms_iSortColumn) * (ms_bAscendingSort ? 1 : -1);
		if (iSort != 0)
			return iSort;
	}

	// Check for tiebreaker sorts.
	for (int i = 0; i < ms_pSortTable->m_columnSortTiebreakerColumns.Size(); i++)
	{
		const int iCandidateColumn = ms_pSortTable->m_columnSortTiebreakerColumns(i);
		if ((iCandidateColumn == ms_iSortColumn) ||
			(ms_pSortTable->m_columnSortFunctions(iCandidateColumn) == NULL))
			continue;

		pFunc = ms_pSortTable->m_columnSortFunctions(iCandidateColumn);
		const int iSort = (*pFunc)(ms_pSortTable, uiRow1, uiRow2, iCandidateColumn) * (ms_bAscendingSort ? 1 : -1);
		if (iSort != 0)
			return iSort;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Generic sorting function, for sorting by numeric integer values in a column.
//////////////////////////////////////////////////////////////////////////
int FDTableWidget::GenericNumericSort(FDTableWidget *pTable, luint32_t uiRow1, luint32_t uiRow2, luint32_t uiColumn)
{
	int iVal1 = atoi(pTable->GetText(uiRow1, uiColumn).str.CStr());
	int iVal2 = atoi(pTable->GetText(uiRow2, uiColumn).str.CStr());

	if (iVal1 < iVal2)
		return -1;
	if (iVal1 > iVal2)
		return 1;
	return 0;
}

static int GetMoneyValueFromString(const char *pcString)
{
	int iValue = 0;

	while (*pcString)
	{
		if ((*pcString >= '0') && (*pcString <= '9'))
		{
			iValue = (iValue * 10) + (*pcString - '0');
		}
		pcString++;
	}

	return iValue;
}

//////////////////////////////////////////////////////////////////////////
// Generic sorting function, for sorting a table by a column representing a monetary value.
//////////////////////////////////////////////////////////////////////////
int FDTableWidget::GenericMoneySort(FDTableWidget *pTable, luint32_t uiRow1, luint32_t uiRow2, luint32_t uiColumn)
{
	const lUIText &t1 = pTable->GetText(uiRow1, uiColumn);
	const lUIText &t2 = pTable->GetText(uiRow2, uiColumn);

	int iValue1 = GetMoneyValueFromString(t1.str.CStr());
	int iValue2 = GetMoneyValueFromString(t2.str.CStr());

	if (iValue1 < iValue2)
		return -1;
	if (iValue1 > iValue2)
		return 1;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Generic table sorting function -- sorts by string values. Ignores
// markup tags in the string.
//////////////////////////////////////////////////////////////////////////
int FDTableWidget::GenericStringSort(FDTableWidget *pTable, luint32_t uiRow1, luint32_t uiRow2, luint32_t uiColumn)
{
	// This uses a ghetto implementation of stricmp() so we can avoid tags without having to strip them.
	const char *iter1 = pTable->GetText(uiRow1, uiColumn).str.CStr();
	const char *iter2 = pTable->GetText(uiRow2, uiColumn).str.CStr();

	int i = 0;
	do
	{
		SkipTag(iter1);
		SkipTag(iter2);
		if ((*iter1 == 0) && (*iter2 != 0))
			return -1;
		if ((*iter2 == 0) && (*iter1 != 0))
			return 1;
		if ((*iter1 == 0) && (*iter2 == 0))
			return 0;

		if (::toupper(*iter1) > ::toupper(*iter2))
			return 1;
		if (::toupper(*iter2) > ::toupper(*iter1))
			return -1;

		iter1++;
		iter2++;
	} while (true);
}

//////////////////////////////////////////////////////////////////////////
// Sort for names in the format "first initial, period, last name". Also
// works for names that are simply a last name.
//////////////////////////////////////////////////////////////////////////
int FDTableWidget::GenericFirstInitialLastNameSort(FDTableWidget *pTable, luint32_t uiRow1, luint32_t uiRow2, luint32_t uiColumn)
{
	// This uses a ghetto implementation of stricmp() so we can avoid tags without having to strip them.
	const char *iter1 = pTable->GetText(uiRow1, uiColumn).str.CStr();
	const char *iter2 = pTable->GetText(uiRow2, uiColumn).str.CStr();

	int i = 0;
	bool bFirstInitialCheck = true;
	char cFI1 = 0;
	char cFI2 = 0;
	do
	{
		SkipTag(iter1);
		SkipTag(iter2);
		if ((*iter1 == 0) && (*iter2 != 0))
			return -1;
		if ((*iter2 == 0) && (*iter1 != 0))
			return 1;
		if ((*iter1 == 0) && (*iter2 == 0))
		{
			if (cFI1 < cFI2)
				return -1;
			if (cFI1 > cFI2)
				return 1;

			return 0;
		}

		// If we haven't processed the first initial yet, do so.
		if (bFirstInitialCheck == true)
		{
			if (*(iter1 + 1) == '.')
			{
				cFI1 = *iter1;
				iter1++;
				iter1++;
			}
			if (*(iter2 + 1) == '.')
			{
				cFI2 = *iter2;
				iter2++;
				iter2++;
			}

			bFirstInitialCheck = false;
			continue;
		}

		if (::toupper(*iter1) > ::toupper(*iter2))
			return 1;
		if (::toupper(*iter2) > ::toupper(*iter1))
			return -1;

		iter1++;
		iter2++;
	} while (true);
}

//////////////////////////////////////////////////////////////////////////
// Sort function for dealing with dates of the format DD.MM.YY.
//////////////////////////////////////////////////////////////////////////
int FDTableWidget::GenericDateSort(FDTableWidget *pTable, luint32_t uiRow1, luint32_t uiRow2, luint32_t uiColumn)
{
	const char *iter1 = pTable->GetText(uiRow1, uiColumn).str.CStr();
	const char *iter2 = pTable->GetText(uiRow2, uiColumn).str.CStr();

	const int day1 = ((iter1[0] - '0') * 10) + (iter1[1] - '0');
	const int day2 = ((iter2[0] - '0') * 10) + (iter2[1] - '0');
	const int month1 = ((iter1[3] - '0') * 10) + (iter1[4] - '0');
	const int month2 = ((iter2[3] - '0') * 10) + (iter2[4] - '0');
	const int year1 = ((iter1[6] - '0') * 10) + (iter1[7] - '0');
	const int year2 = ((iter2[6] - '0') * 10) + (iter2[7] - '0');

	if (year1 < year2)
		return -1;
	if (year1 > year2)
		return 1;

	if (month1 < month2)
		return -1;
	if (month1 > month2)
		return 1;

	if (day1 < day2)
		return -1;
	if (day1 > day2)
		return 1;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Sets the flag controlling whether or not sorts occur when a table header
// is clicked.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::SetSortWhenColumnHeaderClicked(const bool bSort)
{
	lAssert(((m_tableflags & TABLE_FLAG_FIXED_FIRSTROW) != 0) && "This should only be used for tables that always show the first row");
	m_bSortWhenColumnHeaderClicked = bSort;
	m_iLastSortedColumn = -1;
	m_bLastColumnSortWasAscending = false;
}

void FDTableWidget::Resort()
{
	if (m_iLastSortedColumn != -1)
	{
		SortByColumn(m_iLastSortedColumn, m_bLastColumnSortWasAscending);
	}
}


//////////////////////////////////////////////////////////////////////////
// Sets the sort function to be used by a particular column.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::SetSortFunctionForColumn(const int iColumn, TableSortFunc cFunc)
{
	m_columnSortFunctions(iColumn) = cFunc;
}

void FDTableWidget::AddTiebreakerColumn(const int iColumn)
{
	m_columnSortTiebreakerColumns.Add(iColumn, 1);
}

void FDTableWidget::ClearTiebreakerColumns()
{
	m_columnSortTiebreakerColumns.Empty();
}

//////////////////////////////////////////////////////////////////////////
// Called by the mouse handling. Updates flags and column tracking information,
// and calls the function that actually kicks off the column sorting.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::ColumnHeaderClicked(const luint32_t col)
{
	if (m_columnSortFunctions(col) == NULL)
		return;

	if (m_iLastSortedColumn != col)
		m_bLastColumnSortWasAscending = false;

	m_iFlashColumnHeader = col;
	SortByColumn(col, !m_bLastColumnSortWasAscending);
	m_bLastColumnSortWasAscending = !m_bLastColumnSortWasAscending;
	m_iLastSortedColumn = col;
}

//////////////////////////////////////////////////////////////////////////
// Set function for the cell rendering callback.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::SetCellRenderCallback(CellRenderCallback cCallback, void *pData)
{
	m_renderCallback = cCallback;
	m_renderCallbackData = pData;
}


void FDTableWidget::SetFixedRowHeight( int height )
{
  m_fixedRowHeight = height;
}

void FDTableWidget::SetRowGap( int height )
{
  m_rowGap = height;
}

//////////////////////////////////////////////////////////////////////////
// Returns whether or not the first row of the table is a fixed row.
//////////////////////////////////////////////////////////////////////////
bool FDTableWidget::IsFirstRowFixed() const
{
	return ((m_tableflags & TABLE_FLAG_FIXED_FIRSTROW) != 0);
}

//////////////////////////////////////////////////////////////////////////
// Returns the rect (in widget coordinates) of the specified cell.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::GetCellRect(lUIRect &cOutRect, const int iRow, const int iColumn)
{
	// Determine the x-coordinate and width of the cell. This is straightforward.
	cOutRect.x = 0;
	for( int col = 0; col < iColumn; col++ )
	{
		float colwidth = m_columnWidth(col);
		cOutRect.x += m_columnWidth(col);
	}

	cOutRect.w = m_columnWidth(iColumn);

	// Now, determine the current y-coordinate of the cell.
    lUIRect rect = WidgetToScreen( GetRect() );
    lSharedPtr<lFont> font = GetFont();
    float cury = rect.y;
    int   curidx = 0;

    if( !(m_tableflags & TABLE_FLAG_FIXED_FIRSTROW) )
    {
      // start at top of scroll list
      curidx = m_currentTopRow;
    }

	for(int row = curidx; row < iRow; row++)
    {
		float curx = rect.x;
		cury += m_rowHeight(row);

		if( m_tableflags & TABLE_FLAG_FIXED_FIRSTROW && row == 0)
		{
			// have displayed first row, now skip to scroll position
			row = m_currentTopRow;
		}
	}

	cOutRect.y = cury;
	cOutRect.h = m_rowHeight(iRow);
}

void FDTableWidget::RefreshRowHeight(const int iRow)
{
	if( m_fixedRowHeight != 0 )
	    m_rowHeight(iRow) = float( m_fixedRowHeight );
	else if ((m_tableflags & TABLE_FLAG_WORD_WRAP) == 0)
	{
		m_rowHeight(iRow) = GetFont()->DefaultPixelHeight();
	}
	else
	{
		m_rowHeight(iRow) = 0;
		for (int i = 0; i < m_numCols; i++)
		{
			float fWidth = m_columnWidth(i);
			float fHeight = 0.0f;
			const lUIText &text = GetText(iRow, i);
			lString sSplit = SplitText(GetFont().Get(), text.str, fWidth / lSpriteManager::GetDisplayWidth());
			FDTextGetStringBound(GetFont().Get(), fWidth, fHeight, sSplit.CStr());

			m_rowHeight(iRow) = lmMax(fHeight * lSpriteManager::GetDisplayHeight(), m_rowHeight(iRow));
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Call this when the scroll bar extents need to be updated. Normally called
// at the right times by the table itself -- the only time you might need to
// call this manually is when updating the text of word wrapped tables.
//////////////////////////////////////////////////////////////////////////
void FDTableWidget::UpdateScrollBarForChangedText()
{
  // Update the scroll bar, if there is one.
  if (m_scroll != NULL)
  {
	  m_scroll->SetNumberOfItems(m_numRows, true);

	  if (m_fixedRowHeight != 0)
	  {
		m_scroll->SetItemsViewedAtOnce((int)(GetRect().h / m_fixedRowHeight), false);
	  }
	  else if (m_tableflags & TABLE_FLAG_WORD_WRAP)
	  {
		float fMaxRowHeight = 0.0f;
		for( int i = 0; i < m_numRows; i++ )
		{
			fMaxRowHeight = lMax(m_rowHeight(i), fMaxRowHeight);
		}
		m_scroll->SetItemsViewedAtOnce((int)(GetRect().h / fMaxRowHeight), false);
	  }
	  else
	  {
		m_scroll->SetItemsViewedAtOnce((int)(GetRect().h / GetFont()->DefaultPixelHeight()), false);
	  }
  }
}

void FDTableWidget::FlashColumnHeader(const int iColumn)
{
	m_iFlashColumnHeader = iColumn;
}

