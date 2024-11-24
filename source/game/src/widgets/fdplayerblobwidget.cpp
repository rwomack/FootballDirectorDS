//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdplayerblobwidget.cpp
// Description : FDPlayerBlobWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdplayerblobwidget.h>
#include <framework/lsprite.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDPlayerBlobWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDPlayerBlobWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void FDPlayerBlobWidget::ClassInitialiser()
{
}

FDPlayerBlobWidget::FDPlayerBlobWidget() : m_bDragging(false), m_colour(0xFFFFFFFF)
{
}

//PLAYERBLOB_FLAG_STYLUS_ONECLICK

FDPlayerBlobWidget::FDPlayerBlobWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                                        lSharedPtr<lSprite> sprite, int squadNumber, luint32_t playerblobFlags,
                                        const lUIWidgetPosition anchor,
                                        const lUIWidgetPosition pos,
                                        const lUIWidgetSize size )
  : lUIButtonWidget( parent, widgetFlags, id, "", playerblobFlags, anchor, pos, size ),
    m_sprite( sprite ),
    m_squadNumber( squadNumber ),
	m_bDragging(false),
	m_colour(0xFFFFFFFF)
{
}

FDPlayerBlobWidget::~FDPlayerBlobWidget()
{
}

lUIPoint FDPlayerBlobWidget::GetIdealSize() const
{
  if( !m_sprite )
  {
    return SuperClass::GetIdealSize();
  }

  return lUIPoint(m_sprite->GetWidth(),m_sprite->GetHeight());
}

void FDPlayerBlobWidget::SetSprite( lSharedPtr<lSprite> sprite )
{
  m_sprite = sprite;
}

lSharedPtr<lSprite> FDPlayerBlobWidget::GetSprite()
{
  return m_sprite;
}

void FDPlayerBlobWidget::Render()
{
  lUIRect rect = WidgetToScreen( GetRect() );
  lUIPoint cPositionToUse;
  if (m_bDragging && m_bRenderAtDragPoint)
  {
	cPositionToUse.x = m_cDragStartPoint.x - rect.w * 0.5f;
	cPositionToUse.y = m_cDragStartPoint.y - rect.h * 0.5f;
  }
  else
  {
	cPositionToUse.x = rect.x;
	cPositionToUse.y = rect.y;
  }

  if( m_sprite )
  {
    m_sprite->SetPosition( cPositionToUse.x, cPositionToUse.y );
    m_sprite->SetWidth( rect.w );
    m_sprite->SetHeight( rect.h );
    m_sprite->SetColour( m_colour );
    m_sprite->Render();
  }

  // write squad number
  /*
  lSharedPtr<lFont> font = GetFont();
  lString           text;

  text.Format("%d", m_squadNumber);

  lUIPoint offset(0,0);

  int width;
  int height;
  font->GetStringBound( width, height, text );

  offset = lUICalcAlign( m_textAlign, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );

  font->SetColour( L_ARGB(255,255,255,255) );
  font->Print( int(cPositionToUse.x+offset.x),int(cPositionToUse.y+offset.y), text );
  */
}

void FDPlayerBlobWidget::OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags )
{
	// Ensure that we only slide the scroll bar once the user has actually moved the
	// mouse/stylus.
	bool bMoved = (p.x != m_cDragStartPoint.x) || (p.y != m_cDragStartPoint.y);
	if (m_bDragging && bMoved)
	{
		m_bRenderAtDragPoint = true;
		m_cDragStartPoint = WidgetClientToScreen(p);
	}
}

bool FDPlayerBlobWidget::OnMouseDown( const lUIPoint & p, luint32_t mouseDownFlags )
{
	lUIWidget *pWidget = CaptureMouse();
	m_bDragging = true;
	m_bRenderAtDragPoint = false;
	m_cDragStartPoint = WidgetClientToScreen(p);

	return true;
}

bool FDPlayerBlobWidget::OnMouseUp( const lUIPoint & p, luint32_t mouseUpFlags )
{
	if (m_bDragging == true)
	{
		ReleaseMouse();
		m_bDragging = false;

		// Send the "dropped" event.

		// This is not technically correct but since the blobs are only used in one
		// case it will work...and the WidgetToScreen() stuff doesn't appear to work.
		const float fX = GetRect().x + p.x;
		const float fY = GetRect().y + p.y;
		SignalDropped.Emit(this, lUIPoint(fX, fY));
	}

	return true;
}

void FDPlayerBlobWidget::SetColour(const luint32_t uiColour)
{
	m_colour = uiColour;
}
