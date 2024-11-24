//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdgraphwidget.cpp
// Description : FDGraphWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdgraphwidget.h>
#include <framework/lsprite.h>
#include <fdresourcemanager.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDGraphWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDGraphWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct GraphColorScheme
{
	luint32_t lineColor;
	luint32_t dotColor;
};

static GraphColorScheme s_graphColors[FDGraphWidget::MAX_DATA_SETS] = 
{
	{ L_ARGB(120, 0, 255, 255), L_ARGB(120, 0, 255, 255) },
	{ L_ARGB(120, 255, 0, 255), L_ARGB(120, 255, 0, 255) },
	{ L_ARGB(120, 0, 0, 255), L_ARGB(120, 0, 0, 255) },
	{ L_ARGB(120, 255, 255, 0), L_ARGB(120, 255, 255, 0) },
};

luint32_t FDGraphWidget::GetColorForDataSet(const int iDataSet)
{
	lAssert((iDataSet >= 0) && (iDataSet < MAX_DATA_SETS));
	return s_graphColors[iDataSet].lineColor;
}

void FDGraphWidget::ClassInitialiser()
{
}

FDGraphWidget::FDGraphWidget()
{
}

FDGraphWidget::FDGraphWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                              const lUIWidgetPosition anchor,
                              const lUIWidgetPosition pos,
                              const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags, id, anchor, pos, size ),
    m_scrollLeft(NULL),
    m_scrollRight(NULL)
{
  for (int i = 0; i < MAX_DATA_SETS; i++)
	m_data[i].Clear();

  m_offsetX = 0;
  m_maxValue = 1;
  m_minValue = 0;
  m_maxData = 1;
  m_minData = 0;

  m_intervalGap = 5;

  m_visibleXTextInterval = 5;
  m_visibleYTextInterval = 2;

  m_bScrollIntervals = true;
  m_flipYAxis = false;

  m_title = "";
  m_xAxisText = "";
  m_yAxisText = "";
}

FDGraphWidget::~FDGraphWidget()
{
}

lUIPoint FDGraphWidget::GetIdealSize() const
{
  lUIPoint size(0,0);


  return size;
}

void FDGraphWidget::Render()
{
  lUIRect rect = WidgetToScreen( GetRect() );

  luint32_t textColour = L_ARGB(255,0,0,0);
  luint32_t colour = L_ARGB(255,255,255,255);

  lSharedPtr<lFont> font = GetFont();
  int shrinkX = 10;
  int shrinkY = 10;

  lSpriteManager::DrawRect2( luint32_t(rect.x), luint32_t(rect.y), luint32_t(rect.w), luint32_t(rect.h), L_ARGB(120, 0, 0, 0) );
  // Draw axis

  // Draw Grid?

  // Render title
  if( m_title.CStr() != "" )
  {
    lUIPoint offset(0,0);
    int width;
    int height;
    font->GetStringBound( width, height, m_title );

    //offset = lUICalcAlign( m_textAlign, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );
    offset.x = rect.x + ( rect.w - width) / 2.0f;
    offset.y = rect.y + 2.0f;

    font->SetColour( L_ARGB(255,255,255,255) );

    font->Print( int(offset.x),int(offset.y), m_title );
  }

  if( m_yAxisText.CStr() != "" )
  {
    lUIPoint offset(0,0);
    int width;
    int height;
    font->GetStringBound( width, height, m_yAxisText );

    //offset = lUICalcAlign( m_textAlign, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );
    offset.x = rect.x + 2.0f;
    offset.y = rect.y + ( rect.h - height) / 2.0f;
    shrinkX = width + 2;

    font->SetColour( L_ARGB(255,255,255,255) );

    // TODO: Vertical lettering
    font->Print( int(offset.x),int(offset.y), m_yAxisText );
  }

  if( m_xAxisText.CStr() != "" )
  {
    lUIPoint offset(0,0);
    int width;
    int height;
    font->GetStringBound( width, height, m_xAxisText );
    shrinkY = height + 2;

    //offset = lUICalcAlign( m_textAlign, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );
    offset.x = rect.x + ( rect.w - width) / 2.0f;
    offset.y = rect.y + rect.h - height - 2.0f;

    font->SetColour( L_ARGB(255,255,255,255) );

    font->Print( int(offset.x),int(offset.y), m_xAxisText );
  }

  // Border
  rect.Shrink( 15, 15 );
  rect.x += shrinkX;
  rect.w -= shrinkX;
  rect.h -= shrinkY;

  lSpriteManager::DrawRect2( luint32_t(rect.x), luint32_t(rect.y), luint32_t(rect.w), luint32_t(rect.h), L_ARGB(120, 255, 255, 255) );

  // Draw data
  RenderData(rect);

  if( m_flipYAxis )
  {
    rect.y += rect.h;
  }

    // top left to top right
  lSpriteManager::DrawLine2( luint32_t(rect.x), luint32_t(rect.y), luint32_t(rect.x + rect.w), luint32_t(rect.y), L_ARGB(200,255,255,255) );

  // top left to bottom left
  lSpriteManager::DrawLine2( luint32_t(rect.x), luint32_t(rect.y), luint32_t(rect.x), luint32_t(rect.y + rect.h), L_ARGB(200,255,255,255) );

  // bottom left to bottom right
  lSpriteManager::DrawLine2( luint32_t(rect.x), luint32_t(rect.y + rect.h), luint32_t(rect.x + rect.w), luint32_t(rect.y + rect.h), L_ARGB(200,255,255,255) );

  // top right to bottom right
  lSpriteManager::DrawLine2( luint32_t(rect.x + rect.w), luint32_t(rect.y), luint32_t(rect.x + rect.w), luint32_t(rect.y + rect.h), L_ARGB(200,255,255,255) );
}

void FDGraphWidget::RenderData(lUIRect &rect)
{
	bool bHasData = false;
	int iMaxData = 0;
	for (int iDataSet = 0; iDataSet < MAX_DATA_SETS; iDataSet++)
	{
		lArray<int> &theData = m_data[iDataSet];

		if( theData.Size() > 0 )
		{
			bHasData = true;
			iMaxData = lMax(iMaxData, theData.Size());
		}
	}

	if (bHasData == false)
		return;

	int numDots = int ( rect.w / m_intervalGap ); // max number of data per screen
	if( iMaxData < numDots )
	{
		numDots = iMaxData;
		m_intervalGap = int( rect.w / numDots );
	}

	int intervalY = int ( rect.h / (m_maxValue - m_minValue)  );
	if( m_flipYAxis )
	{
		intervalY = -intervalY;
		rect.y -= rect.h;
	}
	lSharedPtr<lFont> font = GetFont();

	// render left side number
	for( int i = m_minValue; i < m_maxValue; i+=m_visibleYTextInterval+1 )
	{
		lString text;
		text.Format( "%d", i );

		lUIPoint offset(0,0);

		int width;
		int height;
		font->GetStringBound( width, height, text );

		//offset = lUICalcAlign( m_textAlign, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );
		offset.x =  rect.x - width - 2.0f;
		offset.y = (rect.y + rect.h)  - ( intervalY * ( i - m_minValue ) );

		font->SetColour( L_ARGB(255,255,255,255) );

		font->Print( int(offset.x),int(offset.y), text );
	}

	// render lines/dots
	for (int iDataSet = 0; iDataSet < MAX_DATA_SETS; iDataSet++)
	{
		lArray<int> &theData = m_data[iDataSet];

		for( int i = 0; i < numDots; i++ )
		{
			if( m_offsetX + i >= theData.Size() )
				break;

			int dataHeight = ( theData(m_offsetX + i) - m_minValue ) * intervalY;
			float curx = rect.x + i * m_intervalGap;
			float cury = rect.y + rect.h - dataHeight;

			if( m_offsetX + i + 1 < theData.Size() )
			{
				dataHeight = ( theData(m_offsetX + i + 1) - m_minValue ) * intervalY;
				float nextx = rect.x + ( i+1 ) * m_intervalGap;
				float nexty = rect.y + rect.h - dataHeight;

				lSpriteManager::DrawLine2( luint32_t(curx), luint32_t(cury), luint32_t(nextx), luint32_t(nexty), s_graphColors[iDataSet].lineColor );
			}

			// TODO: add dots and lines
			lSpriteManager::DrawRect2( luint32_t( curx-1 ), luint32_t(cury-1), luint32_t(3), luint32_t(3), s_graphColors[iDataSet].dotColor );

			// render bottom numbers
			{
				// skip some
				if( i % m_visibleXTextInterval )
					continue;

				lString text;
				text.Format( "%d", i+1 );

				lUIPoint offset(0,0);

				int width;
				int height;
				font->GetStringBound( width, height, text );

				//offset = lUICalcAlign( m_textAlign, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );
				offset.x = curx;

				if( m_flipYAxis )
					offset.y = rect.y + (rect.h * 2);
				else
					offset.y = rect.y + rect.h;

				font->SetColour( L_ARGB(255,255,255,255) );

				font->Print( int(offset.x),int(offset.y), text );
			}
		}
	}
}

void FDGraphWidget::OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags )
{
  SuperClass::OnMouse( p, mouseDownFlags, mouseUpFlags );

  lUIPoint screenPos = WidgetClientToScreen( p );

}

bool FDGraphWidget::OnMouseDown(const lUIPoint &pos, luint32_t buttonFlags)
{
  SuperClass::OnMouseDown(pos,buttonFlags);
  CaptureMouse();

  return true;
}

bool FDGraphWidget::OnMouseUp(const lUIPoint &pos, luint32_t buttonFlags)
{
  SuperClass::OnMouseUp(pos,buttonFlags);
  ReleaseMouse();

  return true;
}

void FDGraphWidget::AddData(const int iDataSet, const int data )
{
  m_data[iDataSet].Add(1);
  m_data[iDataSet].Last() = data;

  if( m_maxValue < data )
    m_maxValue = data;
  if( m_minValue > data )
    m_minValue = data;

  // Set visible interval basic for now
  if( m_data[iDataSet].Size() < 10 )
    m_visibleXTextInterval = 1;
  else
    m_visibleXTextInterval = 5;
}

void FDGraphWidget::SetMaxValue( int max )
{
  m_maxValue = max;
}

void FDGraphWidget::SetMinValue( int min )
{
  m_minValue = min;
}

void FDGraphWidget::SetIntervalGap( int gap )
{
  m_intervalGap = gap;
}

void FDGraphWidget::ClearDataSet(const int iDataSet)
{
	m_data[iDataSet].Clear();
}

void FDGraphWidget::ClearAll()
{
  for (int i = 0; i < MAX_DATA_SETS; i++)
    m_data[i].Clear();

  m_maxValue = 1;
  m_minValue = 0;
  m_maxData = 1;
  m_minData = 0;

  m_intervalGap = 5;
}

void FDGraphWidget::FlipYAxis( bool flip )
{
  m_flipYAxis = flip;
}

void FDGraphWidget::SetTitle( lString text )
{
  m_title = text;
}

void FDGraphWidget::SetXAxisTitle( lString text )
{
  m_xAxisText = text;
}

void FDGraphWidget::SetYAxisTitle( lString text )
{
  m_yAxisText = text;
}

void FDGraphWidget::SetScrollButtons( lUIButtonWidget * leftButton, lUIButtonWidget * rightButton )
{
  m_scrollLeft   = leftButton;
  m_scrollRight = rightButton;

  m_scrollLeft->SignalControlCommand.Connect( this, &FDGraphWidget::OnScrollLeftButton );
  m_scrollRight->SignalControlCommand.Connect( this, &FDGraphWidget::OnScrollRightButton );
}

void FDGraphWidget::OnScrollLeftButton( lUIControlWidget * )
{
  lLogFmt("Scroll Left\n");
  m_offsetX--;
  if( m_offsetX < 0 ) m_offsetX = 0;
}

void FDGraphWidget::OnScrollRightButton( lUIControlWidget * )
{
  lLogFmt("Scroll down\n");
  m_offsetX++;
  if( m_offsetX >= m_maxData ) m_offsetX = m_maxData-1; //  Note: should also minus visible space...
}
