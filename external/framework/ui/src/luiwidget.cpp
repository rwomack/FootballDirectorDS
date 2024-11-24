//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : luiwidget.cpp
// Description : lUIWidget implementation
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <ui/luiwidget.h>
#include <framework/lspritemanager.h>
#include <framework/lfont.h>

// TEMP HACK: - to get mouse state and input.. needs to use framework.
#include <ui/luimouse.h>
#include <lemon/input/linput.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( lUIWidget, lGUID(0xD7F1A20B,0x20AA,0x4CDB,0xA792E892FD32147E), 0 );

bool lUIWidget::ms_bLastMouseClickHandled = false;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Global Variables
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

static lSharedPtr<lFont> s_defaultFont;

// HACK: needs to be managed. if you call ProcessInput on more than one
//       root widget, mouse messages can go missing.
static lUIWidget * s_pMouseCapture = NULL;

#if defined(L_PLATFORM_NDS)
static bool        s_stylusInput = true;
#else
static bool        s_stylusInput = false;
#endif

static unsigned int s_mouseFlags = 0;
static unsigned int s_mouseFlagsPrev = 0;
static unsigned int s_mouseDownFlags = 0;
static unsigned int s_mouseUpFlags = 0;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lPropertyUIRect
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void lPropertyUIRect::XMLSerialize( lObject *o, lPropertyObjectMap &mapper, lXMLNode &ar )
{
  lUIRect rect;

  if( ar.IsSaving() )
  {
    rect = Get(o);
  }

  ar.Attribute( "x", rect.x );
  ar.Attribute( "y", rect.y );
  ar.Attribute( "w", rect.w );
  ar.Attribute( "h", rect.h );

  if( ar.IsLoading() )
  {
    Set(o,rect);
  }
}

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lPropertyUIPoint
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void lPropertyUIPoint::XMLSerialize( lObject *o, lPropertyObjectMap &mapper, lXMLNode &ar )
{
  lUIPoint point;

  if( ar.IsSaving() )
  {
    point = Get(o);
  }

  ar.Attribute( "x", point.x );
  ar.Attribute( "y", point.y );

  if( ar.IsLoading() )
  {
    Set(o,point);
  }
}

lUIPoint L_NAMESPACE_NAME::lUICalcAlign( lUIAlign align, const lUIPoint & parentSize, const lUIPoint & itemSize )
{
  lUIPoint result;

  int xFlags = (align>>4) & 0xF;
  int yFlags = align & 0xF;

  if( xFlags == LUI_LEFT )
  {
    result.x = 0;
  }
  else if( xFlags == LUI_RIGHT )
  {
    result.x = parentSize.x - itemSize.x;
  }
  else if( xFlags == LUI_CENTER )
  {
    result.x = (parentSize.x - itemSize.x) * 0.5f;
  }

  if( yFlags == LUI_LEFT )
  {
    result.y = 0;
  }
  else if( xFlags == LUI_RIGHT )
  {
    result.y = parentSize.y - itemSize.y;
  }
  else if( xFlags == LUI_CENTER )
  {
    result.y = (parentSize.y - itemSize.y) * 0.5f;
  }

  return result;
}

float L_NAMESPACE_NAME::lUICalcAlignAxis( lUIAlignAxis align, float parentSize, float itemSize )
{
  float result;

  if( align == LUI_LEFT )
  {
    result = 0;
  }
  else if( align == LUI_RIGHT )
  {
    result = parentSize - itemSize;
  }
  else if( align == LUI_CENTER )
  {
    result = (parentSize - itemSize) * 0.5f;
  }

  return result;
}


//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lUIFont
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

lUIFont::lUIFont()
  : m_flags( LUI_FONT_DEFAULT )
{

}

lUIFont::lUIFont( lUIFontType type )
  : m_flags( type )
{
}

lUIFont::lUIFont( lSharedPtr<lFont> f )
  : m_flags( -1 ),
    m_font( f )
{
}


//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lUIWidgetPosition
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

lUIWidgetPosition::lUIWidgetPosition()
  : m_positionFlags(PIXEL),
    m_x(0),
    m_y(0)
{
}

lUIWidgetPosition::lUIWidgetPosition( lUIAlign align )
  : m_positionFlags( PERCENT )
{
  int xFlags = (align>>4) & 0xF;
  int yFlags = align & 0xF;

  if( xFlags == LUI_LEFT )
  {
    m_x = 0;
  }
  else if( xFlags == LUI_RIGHT )
  {
    m_x = 100;
  }
  else if( xFlags == LUI_CENTER )
  {
    m_x = 50;
  }

  if( yFlags == LUI_TOP )
  {
    m_y = 0;
  }
  else if( yFlags == LUI_BOTTOM )
  {
    m_y = 100;
  }
  else if( yFlags == LUI_CENTER )
  {
    m_y = 50;
  }
}

lUIWidgetPosition::lUIWidgetPosition( const lUIPoint & p )
  : m_positionFlags( PIXEL ),
    m_x( p.x ),
    m_y( p.y )
{
}

lUIWidgetPosition::lUIWidgetPosition( const lUIPointPercent & p )
  : m_positionFlags( PERCENT ),
    m_x( p.x ),
    m_y( p.y )
{
}

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lUIWidgetSize
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

lUIWidgetSize::lUIWidgetSize()
  : m_positionFlags(PIXEL),
    m_x(0),
    m_y(0)
{
}

lUIWidgetSize::lUIWidgetSize( const lUIPoint & p )
  : m_positionFlags( PIXEL ),
    m_x( p.x ),
    m_y( p.y )
{
}

lUIWidgetSize::lUIWidgetSize( const lUIPointPercent & p )
  : m_positionFlags( PERCENT ),
    m_x( p.x ),
    m_y( p.y )
{
}

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lUIWidgetPositionCoord
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

lUIWidgetPositionCoord::lUIWidgetPositionCoord()
  : m_positionFlags(PIXEL),
    m_coord(0)
{
}

lUIWidgetPositionCoord::lUIWidgetPositionCoord( lUIAlignAxis align )
  : m_positionFlags( PERCENT )
{
  if( align == LUI_LEFT )
  {
    m_coord = 0;
  }
  else if( align == LUI_RIGHT )
  {
    m_coord = 100;
  }
  else if( align == LUI_CENTER )
  {
    m_coord = 50;
  }
}

lUIWidgetPositionCoord::lUIWidgetPositionCoord( const lUICoord & p )
  : m_positionFlags( PIXEL ),
    m_coord( p.coord )
{
}

lUIWidgetPositionCoord::lUIWidgetPositionCoord( const lUICoordPercent & p )
  : m_positionFlags( PERCENT ),
    m_coord( p.coord )
{
}

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lUIWidgetSizeCoord
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

lUIWidgetSizeCoord::lUIWidgetSizeCoord()
  : m_positionFlags(PIXEL),
    m_coord(0)
{
}

lUIWidgetSizeCoord::lUIWidgetSizeCoord( const lUICoord & p )
  : m_positionFlags( PIXEL ),
    m_coord( p.coord )
{
}

lUIWidgetSizeCoord::lUIWidgetSizeCoord( const lUICoordPercent & p )
  : m_positionFlags( PERCENT ),
    m_coord( p.coord )
{
}


//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// lUIWidget
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lUIWidget ClassInitialiser
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

void lUIWidget::ClassInitialiser()
{
  //lPropertyFlag::Create( StaticClass(), lProperty::PERSISTENT, "Visible", &lUIWidget::m_flags, 1 );
  //lPropertyUIRect::Create( StaticClass(), lProperty::PERSISTENT, "Rect", &lUIWidget::m_rect );
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lUIWidget constructor
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
lUIWidget::lUIWidget()
  : m_widgetFlags(0),
    m_mouseState(0),
    m_prevMouseState(0),
    m_focusChild(NULL)
{
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lUIWidget constructor
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
lUIWidget::lUIWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                      const lUIWidgetPosition anchor,
                      const lUIWidgetPosition pos,
                      const lUIWidgetSize size )
  : m_widgetFlags(widgetFlags),
    m_id(id),
    m_anchor( anchor ),
    m_pos( pos ),
    m_size( size ),
    m_mouseState(0),
    m_prevMouseState(0),
    m_focusChild(NULL)
{
  if( parent )
  {
    lUIWidget * lastChild = parent->LastChild();
    if( lastChild )
    {
      InsertAfter( lastChild );
    }
    else
    {
      SetParent( parent );
    }
  }
}


//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// lUIWidget destructor
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
lUIWidget::~lUIWidget()
{
  if( GetParent() )
  {
    if( GetParent()->m_focusChild == this )
    {
      GetParent()->m_focusChild = NULL;
    }
  }
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Tick the widget
// Tick
//
// In:
//   delta - seconds passed since last tick
//
// Out:
//   NONE
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void lUIWidget::Tick( float delta )
{
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Render the widget
// Render
//
// In:
//   NONE
//
// Out:
//   NONE
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void lUIWidget::Render()
{
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Get the ideal size for the widget
// GetIdealSize
//
// In:
//   NONE
//
// Out:
//   Returns a lUIPoint containing the ideal widget size
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
lUIPoint lUIWidget::GetIdealSize() const
{
  return lUIPoint(0,0);
}

bool lUIWidget::HitMask( const lUIPoint & point )
{
  return true;
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Get the id for the widget
// GetId
//
// In:
//   NONE
//
// Out:
//   Returns the widget id
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
const char * lUIWidget::GetId() const
{
  return m_id;
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Get the rect for the widget
// GetRect
//
// In:
//   NONE
//
// Out:
//   Returns the widget rect
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
const lUIRect & lUIWidget::GetRect() const
{
  UpdateSize();
  UpdatePos();
  return m_rect;
}

// Update m_rect.w and m_rect.h with
// the widget pixel size based on m_size.
// Note: Calls UpdateSize() on the parent widget if required.
void lUIWidget::UpdateSize() const
{
  float x;
  float y;

  if( m_size.m_x < 0 || m_size.m_y < 0 )
  {
    lUIPoint size = GetIdealSize();
    x = size.x;
    y = size.y;
  }
  else
  {
    x = m_size.m_x;
    y = m_size.m_y;
  }

  if( m_size.m_positionFlags == lUIWidgetSize::PIXEL )
  {
    m_rect.w = x;
    m_rect.h = y;
    return;
  }

  float w;
  float h;

  if( GetParent() )
  {
    GetParent()->UpdateSize();
    const lUIRect & widgetRect = GetParent()->m_rect;
    w = widgetRect.w;
    h = widgetRect.h;
  }
  else
  {
    w = float(lSpriteManager::GetDisplayWidth());
    h = float(lSpriteManager::GetDisplayHeight());
  }

  lAssert( m_size.m_positionFlags == lUIWidgetSize::PERCENT );

  m_rect.w = (x * 0.01f) * w;
  m_rect.h = (y * 0.01f) * h;
}

// Update m_rect.x and m_rect.x with
// the widget pixel position based on m_anchor and m_pos
void lUIWidget::UpdatePos() const
{
  UpdateSize();

  lUIPoint pixelAnchor;
  lUIPoint pixelPos;

  CalcPos( pixelAnchor, m_anchor, this );
  CalcPos( pixelPos, m_pos, GetParent() );

  m_rect.x = pixelPos.x - pixelAnchor.x;
  m_rect.y = pixelPos.y - pixelAnchor.y;
}

// Calculate a pixel position relative to the specified widget.
// Note: calls UpdateSize() on the widget if required.
void lUIWidget::CalcPos( lUIPoint & result, const lUIWidgetPosition & pos, const lUIWidget * widget ) const
{
  if( pos.m_positionFlags == lUIWidgetPosition::PIXEL )
  {
    result.x = pos.m_x;
    result.y = pos.m_y;
    return;
  }

  float w;
  float h;

  if( widget )
  {
    widget->UpdateSize();
    const lUIRect & widgetRect = widget->m_rect;
    w = widgetRect.w;
    h = widgetRect.h;
  }
  else
  {
    w = float(lSpriteManager::GetDisplayWidth());
    h = float(lSpriteManager::GetDisplayHeight());
  }

  lAssert( pos.m_positionFlags == lUIWidgetPosition::PERCENT );

  result.x = (pos.m_x * 0.01f) * w;
  result.y = (pos.m_y * 0.01f) * h;
}

lSharedPtr<lFont> lUIWidget::GetFont() const
{
  if( m_font.m_flags == LUI_FONT_DEFAULT )
  {
    if( GetParent() )
    {
      return GetParent()->GetFont();
    }
    else
    {
      return s_defaultFont;
    }
  }

  return m_font.m_font;
}

void lUIWidget::SetFont( const lUIFont & font )
{
  m_font = font;
}



//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Move the widget to the specified position
// Move
//
// In:
//   point - new position for widget
//
// Out:
//   NONE
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void lUIWidget::Move(const lUIPoint & point)
{
  m_pos = lUIWidgetPosition( point );
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Move and resize the widget to the specified rect
// Move
//
// In:
//   rect - new rect for the widget
//
// Out:
//   NONE
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void lUIWidget::Move(const lUIRect & rect)
{
  m_pos = lUIWidgetPosition( lUIPoint(rect.x, rect.y) );
  m_size = lUIWidgetSize( lUIPoint(rect.w, rect.h) );
}

void lUIWidget::Move( const lUIWidgetPosition & pos )
{
  m_pos = pos;
}

void lUIWidget::Move( const lUIWidgetPosition & pos, const lUIWidgetSize & size )
{
  m_pos  = pos;
  m_size = size;
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Show the widget
// Show
//
// In:
//   NONE
//
// Out:
//   NONE
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void lUIWidget::Show()
{
  m_widgetFlags &= ~LUI_WIDGET_HIDDEN;
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Hide the widget
// Hide
//
// In:
//   NONE
//
// Out:
//   NONE
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void lUIWidget::Hide()
{
  m_widgetFlags |= LUI_WIDGET_HIDDEN;
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Check if the widget is visible
// IsVisible
//
// In:
//   NONE
//
// Out:
//   Returns true if the widget is visible
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
bool lUIWidget::IsVisible() const
{
  return !(m_widgetFlags & LUI_WIDGET_HIDDEN);
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Show the widget
// Show
//
// In:
//   NONE
//
// Out:
//   NONE
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void lUIWidget::Enable()
{
  m_widgetFlags &= ~LUI_WIDGET_DISABLED;
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Hide the widget
// Hide
//
// In:
//   NONE
//
// Out:
//   NONE
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void lUIWidget::Disable()
{
  m_widgetFlags |= LUI_WIDGET_DISABLED;
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Check if the widget is visible
// IsVisible
//
// In:
//   NONE
//
// Out:
//   Returns true if the widget is visible
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
bool lUIWidget::IsEnabled() const
{
  return !(m_widgetFlags & LUI_WIDGET_DISABLED);
}


//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Bring the widget to the front of its parent child list
// BringToFront
//
// In:
//   NONE
//
// Out:
//   NONE
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void lUIWidget::BringToFront()
{
  if( GetParent() )
  {
    InsertBefore( GetParent()->FirstChild() );
  }
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Push the widget to the back of its parent child list
// PushToBack
//
// In:
//   NONE
//
// Out:
//   NONE
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void lUIWidget::PushToBack()
{
  if( GetParent() )
  {
    InsertAfter( GetParent()->LastChild() );
  }
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Transform a point from screen to widget coordinate space
// ScreenToWidget
//
// In:
//   in - input point
//
// Out:
//   Returns the point in widget coordinate space
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
lUIPoint lUIWidget::ScreenToWidget( const lUIPoint &in )
{
  lUIPoint out = in;

  lUIWidget * parent = GetParent();
  while( parent )
  {
    parent->UpdatePos();

    out.x -= parent->m_rect.x;
    out.y -= parent->m_rect.y;
    parent = parent->GetParent();
  }

  return out;
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Transform a point from screen to widget coordinate space
// ScreenToWidget
//
// In:
//   in - input point
//
// Out:
//   Returns the point in widget coordinate space
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
lUIPoint lUIWidget::ScreenToWidgetClient( const lUIPoint &in )
{
  lUIPoint out = in;

  lUIWidget * parent = this;
  while( parent )
  {
    parent->UpdatePos();

    out.x -= parent->m_rect.x;
    out.y -= parent->m_rect.y;
    parent = parent->GetParent();
  }

  return out;
}


//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Transform a rect from screen to widget coordinate space
// ScreenToWidget
//
// In:
//   in - input rect
//
// Out:
//   Returns the rect in widget coordinate space
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
lUIRect lUIWidget::ScreenToWidget( const lUIRect &in )
{
  lUIRect out = in;

  lUIWidget * parent = GetParent();
  while( parent )
  {
    parent->UpdatePos();

    out.x -= parent->m_rect.x;
    out.y -= parent->m_rect.y;
    parent = parent->GetParent();
  }

  return out;
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Transform a point from widget to screen coordinate space
// ScreenToWidget
//
// In:
//   in - input point
//
// Out:
//   Returns the point in widget coordinate space
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
lUIPoint lUIWidget::WidgetToScreen( const lUIPoint &in )
{
  lUIPoint out = in;

  lUIWidget * parent = GetParent();
  while( parent )
  {
    parent->UpdatePos();

    out.x += parent->m_rect.x;
    out.y += parent->m_rect.y;

    parent = parent->GetParent();
  }

  return out;
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Transform a point from widget to screen coordinate space
// ScreenToWidget
//
// In:
//   in - input point
//
// Out:
//   Returns the point in widget coordinate space
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
lUIPoint lUIWidget::WidgetClientToScreen( const lUIPoint &in )
{
  lUIPoint out = in;

  lUIWidget * parent = this;
  while( parent )
  {
    parent->UpdatePos();

    out.x += parent->m_rect.x;
    out.y += parent->m_rect.y;

    parent = parent->GetParent();
  }

  return out;
}


//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Transform a rect from widget to screen coordinate space
// ScreenToWidget
//
// In:
//   in - input rect
//
// Out:
//   Returns the rect in widget coordinate space
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
lUIRect lUIWidget::WidgetToScreen( const lUIRect &in )
{
  lUIRect out = in;

  lUIWidget * parent = GetParent();
  while( parent )
  {
    parent->UpdatePos();

    out.x += parent->m_rect.x;
    out.y += parent->m_rect.y;

    parent = parent->GetParent();
  }

  return out;
}

lUIPoint lUIWidget::PixelSize( const lUIWidgetSize & size ) const
{
  float x;
  float y;

  if( size.m_x < 0 || size.m_y < 0 )
  {
    lUIPoint size = GetIdealSize();
    x = size.x;
    y = size.y;
  }
  else
  {
    x = size.m_x;
    y = size.m_y;
  }

  if( size.m_positionFlags == lUIWidgetSize::PIXEL )
  {
    return lUIPoint( x, y );
  }

  float w;
  float h;

  if( GetParent() )
  {
    GetParent()->UpdateSize();
    const lUIRect & widgetRect = GetParent()->m_rect;
    w = widgetRect.w;
    h = widgetRect.h;
  }
  else
  {
    w = float(lSpriteManager::GetDisplayWidth());
    h = float(lSpriteManager::GetDisplayHeight());
  }

  lAssert( size.m_positionFlags == lUIWidgetSize::PERCENT );

  return lUIPoint( (x * 0.01f) * w, (y * 0.01f) * h );
}

float lUIWidget::PixelSizeX( const lUIWidgetSizeCoord & size ) const
{
  float x;

  if( size.m_coord < 0 )
  {
    lUIPoint size = GetIdealSize();
    x = size.x;
  }
  else
  {
    x = size.m_coord;
  }

  if( size.m_positionFlags == lUIWidgetSize::PIXEL )
  {
    return x;
  }

  float w;

  if( GetParent() )
  {
    GetParent()->UpdateSize();
    const lUIRect & widgetRect = GetParent()->m_rect;
    w = widgetRect.w;
  }
  else
  {
    w = float(lSpriteManager::GetDisplayWidth());
  }

  lAssert( size.m_positionFlags == lUIWidgetSize::PERCENT );

  return (x * 0.01f) * w;
}

float lUIWidget::PixelSizeY( const lUIWidgetSizeCoord & size ) const
{
  float y;

  if( size.m_coord < 0 )
  {
    lUIPoint size = GetIdealSize();
    y = size.y;
  }
  else
  {
    y = size.m_coord;
  }

  if( size.m_positionFlags == lUIWidgetSize::PIXEL )
  {
    return y;
  }

  float h;

  if( GetParent() )
  {
    GetParent()->UpdateSize();
    const lUIRect & widgetRect = GetParent()->m_rect;
    h = widgetRect.h;
  }
  else
  {
    h = float(lSpriteManager::GetDisplayHeight());
  }

  lAssert( size.m_positionFlags == lUIWidgetSize::PERCENT );

  return (y * 0.01f) * h;
}


lUIPoint lUIWidget::PixelPos( const lUIWidgetPosition & pos ) const
{
  lUIPoint result;
  CalcPos( result, pos, this );
  return result;
}

float lUIWidget::PixelPosX( const lUIWidgetPositionCoord & pos ) const
{
  lUIPoint result;
  CalcPos( result, lUIPoint(pos.m_coord,0), this );
  return result.x;
}

float lUIWidget::PixelPosY( const lUIWidgetPositionCoord & pos ) const
{
  lUIPoint result;
  CalcPos( result, lUIPoint(0,pos.m_coord), this );
  return result.y;
}


lUIPoint lUIWidget::CalcAlign( lUIAlign align, const lUIPoint & itemSize ) const
{
  UpdateSize();
  return lUICalcAlign( align, lUIPoint( m_rect.w, m_rect.h ), itemSize );
}

float lUIWidget::CalcAlignX( lUIAlignAxis align, float itemSizeX ) const
{
  UpdateSize();
  return lUICalcAlignAxis( align, m_rect.w, itemSizeX );
}

float lUIWidget::CalcAlignY( lUIAlignAxis align, float itemSizeY ) const
{
  UpdateSize();
  return lUICalcAlignAxis( align, m_rect.h, itemSizeY );
}

void lUIWidget::InternalSetParentFocus()
{
  if( GetParent() == NULL )
  {
    // we have no parent.
    return;
  }

  if( GetParent()->m_focusChild == this )
  {
    // we're already the focus of our parent.
    return;
  }

  // make ourselves the focus of our parent

  if( GetParent()->m_focusChild == NULL )
  {
    // our parent needs focus

    GetParent()->InternalSetParentFocus();
  }
  else
  {
    // another child of our parent already have focus,
    // undo it.

    lUIWidget * widget = GetParent()->m_focusChild;

    while( widget )
    {
      lUIWidget * next = widget->m_focusChild;

      if( widget->m_focusChild == NULL )
      {
        widget->OnLoseFocus();
      }

      widget->m_focusChild = NULL;

      widget = next;
    }
  }

  GetParent()->m_focusChild = this;
}


void lUIWidget::SetFocus()
{
  if( GetParent() == NULL && m_focusChild == NULL )
  {
    // we already have focus
    return;
  }

  if( (GetParent() != NULL && GetParent()->m_focusChild == this) && m_focusChild == NULL )
  {
    // we already have focus
    return;
  }

  InternalSetParentFocus();

  if( m_focusChild )
  {
    // if one of our children has focus, remove it.

    lUIWidget * widget = m_focusChild;

    while( widget )
    {
      lUIWidget * next = widget->m_focusChild;

      if( widget->m_focusChild == NULL )
      {
        widget->OnLoseFocus();
      }

      widget->m_focusChild = NULL;

      widget = next;
    }
  }

  OnGainFocus();
}

bool lUIWidget::HasFocus()
{
  if( GetParent() == NULL || GetParent()->m_focusChild == this )
  {
    if( m_focusChild == NULL )
    {
      return true;
    }
  }

  return false;
}

bool lUIWidget::IsHot()
{
  return (m_prevMouseState & MOUSE_STATE_HOT);
}

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

bool lUIWidget::ProcessInput()
{
  bool bResult = false;
  // dispatch top-level messages

  if( GetParent() == NULL && IsVisible() && IsEnabled() )
  {
    lUIPoint mouse;

    mouse.x = float(lUIMouse::GetPixelX());
    mouse.y = float(lUIMouse::GetPixelY());

    s_mouseFlags = 0;

    if( lInput->GetInputState( LI_MOUSE_BUTTON1 ) ) s_mouseFlags |= LUI_MOUSE_LEFT;
    if( lInput->GetInputState( LI_MOUSE_BUTTON2 ) ) s_mouseFlags |= LUI_MOUSE_RIGHT;
    
    #ifdef L_PLATFORM_WII
    if( lInput->GetInputState( LI_WII_PAD1_BUTTON_A  ) ) s_mouseFlags |= LUI_MOUSE_LEFT;
    #endif

    s_mouseDownFlags = s_mouseFlags & (~s_mouseFlagsPrev);
    s_mouseUpFlags   = (~s_mouseFlags) & (s_mouseFlagsPrev);
    s_mouseFlagsPrev = s_mouseFlags;

    // For stylus input, don't send mouse messages unless the stylus state has changed.

    if( !s_stylusInput || (s_mouseFlags || s_mouseUpFlags || s_mouseDownFlags)  )
    {
      if( s_pMouseCapture )
      {
        lUIPoint mouseWindow = s_pMouseCapture->ScreenToWidget(mouse);
        bResult = s_pMouseCapture->ProcessMouseEvent( mouseWindow, s_mouseDownFlags, s_mouseUpFlags );
      }
      else
      {
        lUIPoint mouseWindow = ScreenToWidget(mouse);
        bResult = ProcessMouseEvent( mouseWindow, s_mouseDownFlags, s_mouseUpFlags );
      }
    }

    // deal with keyboard input

    #if defined( L_PLATFORM_WINDOWS )
    // HACK: key input can be invalid on other platforms, check lInput.

    int          size;
    const char * buf;

    buf = lInput->GetKeyAsciiBuffer(&size);

    if( size > 0 )
    {
      while( size-- )
      {
        if( (*buf) & 0x80 )
        {
          ProcessKeyEvent( *buf, *(buf+1) );
          buf++;
          size--;
        }
        else
        {
          ProcessKeyEvent( *buf, 0 );
        }

        buf++;
      }
    }

    #endif
  }

  // dispatch mouse leave message

  if( GetParent() == NULL )
  {
    if( !(m_mouseState & MOUSE_STATE_OVER) && (m_prevMouseState & MOUSE_STATE_OVER) )
    {
      OnMouseLeave();
    }

    if( !(m_mouseState & MOUSE_STATE_HOT) && (m_prevMouseState & MOUSE_STATE_HOT) )
    {
      OnMouseLeaveHot();
    }

    m_prevMouseState = m_mouseState;
    m_mouseState     = 0;
  }

  // dispatch update message

  if( IsVisible() && IsEnabled() )
  {
    lUIWidget * child = LastChild();
    while( child )
    {
      child->ProcessInput();
      child = child->PrevSibling();
    }

    // dispatch all child leave messages first

    child = LastChild();
    while( child )
    {
      if( !(child->m_mouseState & MOUSE_STATE_OVER) && (child->m_prevMouseState & MOUSE_STATE_OVER) )
      {
        child->OnMouseLeave();
      }

      if( !(child->m_mouseState & MOUSE_STATE_HOT) && (child->m_prevMouseState & MOUSE_STATE_HOT) )
      {
        child->OnMouseLeaveHot();
      }

      child = child->PrevSibling();
    }

    // dispatch all child over messages

    child = LastChild();
    while( child )
    {
      if( (child->m_mouseState & MOUSE_STATE_OVER) )
      {
        if( !(child->m_prevMouseState & MOUSE_STATE_OVER) )
        {
          child->OnMouseOver();
        }

        child->OnMouse( child->m_mouseOverPos, s_mouseDownFlags, s_mouseUpFlags );
      }

      if( (child->m_mouseState & MOUSE_STATE_HOT) )
      {
        if( !(child->m_prevMouseState & MOUSE_STATE_HOT) )
        {
          child->OnMouseOverHot();
        }

        child->OnMouseHot( child->m_mouseHotPos, s_mouseDownFlags, s_mouseUpFlags );
      }

      child = child->PrevSibling();
    }

    // update flags

    child = LastChild();
    while( child )
    {
      child->m_prevMouseState = child->m_mouseState;
      child->m_mouseState     = 0;

      child = child->PrevSibling();
    }
  }

  ms_bLastMouseClickHandled = bResult;

  return bResult;
}


bool lUIWidget::ProcessMouseEvent( const lUIPoint &pos, luint32_t mouseDownFlags, luint32_t mouseUpFlags )
{
  const lUIRect & widgetRect = m_rect;
  const lUIRect & clientRect = m_rect;

  if( !IsVisible() )
  {
    return false;
  }

  bool bMessageHandled = false;

  UpdateSize();
  UpdatePos();

  if( pos.InRect( widgetRect ) )
  {
    if( pos.InRect( clientRect ) )
    {
	  // Disabled controls eat mouse events.
	  if (!IsEnabled())
		return true;

      m_mouseState |= MOUSE_STATE_OVER;
      m_mouseOverPos = pos;
      m_mouseOverPos.x -= clientRect.x;
      m_mouseOverPos.y -= clientRect.y;

      if( GetParent() == NULL )
      {
        if( !(m_prevMouseState & MOUSE_STATE_OVER) ) OnMouseOver();
        OnMouse( m_mouseOverPos, mouseDownFlags, mouseUpFlags );
      }

      lUIPoint childPos = pos;
      childPos.x -= clientRect.x;
      childPos.y -= clientRect.y;

      // don't recurse into children if we've captured the mouse
      if( s_pMouseCapture != this )
      {
        lUIWidget * child = LastChild();
        while( child )
        {
          bMessageHandled = child->ProcessMouseEvent( childPos, mouseDownFlags, mouseUpFlags );
          if( bMessageHandled )
			  break;

          child = child->PrevSibling();
        }
      }

      if( !bMessageHandled )
      {
        // not in a child, but in our client area,
        // send mouse up and down messages.

        if( mouseDownFlags )
        {
          lLogFmt("OnMouseDown: %s\n", GetId() );
          bMessageHandled |= OnMouseDown( childPos, mouseDownFlags );
        }

        if( mouseUpFlags )
        {
          bMessageHandled |= OnMouseUp( childPos, mouseUpFlags );
        }
      }
    }

    if( !bMessageHandled )
    {
      // not in a child, but in our widget area.
      // deal with focus flags.

      if( m_widgetFlags & LUI_WIDGET_MOUSEHOTFOCUS )
      {
        SetFocus();
      }
      else if( m_widgetFlags & LUI_WIDGET_MOUSEDOWNFOCUS )
      {
        if( mouseDownFlags & LUI_MOUSE_LEFT )
        {
          SetFocus();
        }
      }

      // deal with being hot.

      m_mouseState |= MOUSE_STATE_HOT;
      m_mouseHotPos = pos;
      m_mouseHotPos.x -= clientRect.x;
      m_mouseHotPos.y -= clientRect.y;

      if( GetParent() == NULL )
      {
        if( !(m_prevMouseState & MOUSE_STATE_HOT) )
        {
          OnMouseOverHot();
        }

        OnMouseHot( m_mouseHotPos, mouseDownFlags, mouseUpFlags );
      }
    }

    // HACK: should HitMask affect all mouse messages sent to this widget?
    // HACK: not sure about the semantics of the transparent flag.. should widget still get mouse events?

    if( (m_widgetFlags & LUI_WIDGET_MOUSETRANSPARENT) )
    {
      return bMessageHandled;
    }

    return bMessageHandled | HitMask( m_mouseHotPos );
  }
  else
  {
    if( s_pMouseCapture == this )
    {
      lUIPoint childPos = pos;
      childPos.x -= clientRect.x;
      childPos.y -= clientRect.y;

      OnMouse( childPos, mouseDownFlags, mouseUpFlags );

      if( mouseDownFlags )
      {
        lLogFmt("OnMouseDown2: %s\n", GetId() );
        bMessageHandled |= OnMouseDown( childPos, mouseDownFlags );
      }

      if( mouseUpFlags )
      {
        bMessageHandled |= OnMouseUp( childPos, mouseUpFlags );
      }

      // HACK: should HitMask affect all mouse messages sent to this widget?
      // HACK: not sure about the semantics of the transparent flag.. should widget still get mouse events?

      if( (m_widgetFlags & LUI_WIDGET_MOUSETRANSPARENT) )
      {
        return bMessageHandled;
      }

      return bMessageHandled | HitMask( m_mouseHotPos );
    }

    return false;
  }
}

bool lUIWidget::ProcessKeyEvent( luint8_t c, luint8_t ext )
{
  if( !IsVisible() || !IsEnabled() )
  {
    return false;
  }

  bool bProcessed = false;

  // if we have a child focus, pass the message to it

  if( m_focusChild )
  {
    bProcessed = m_focusChild->ProcessKeyEvent( c, ext );
  }

  // dispatch message to root window

  if( GetParent() == NULL )
  {
    OnKey(c,ext);
  }

  // dispatch message to this window

  if( !bProcessed )
  {
    OnKey( c, ext );
    return true;
  }


  return bProcessed;
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Capture mouse input
// CaptureMouse
//
// In:
//   NONE
//
// Out:
//   Returns the previous widget that captured the mouse, or NULL if none.
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
lUIWidget * lUIWidget::CaptureMouse()
{
  lUIWidget * old = s_pMouseCapture;
  s_pMouseCapture = this;
  return old;
}

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// Release mouse input capture
// ReleaseMouse
//
// In:
//   NONE
//
// Out:
//   NONE
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
void lUIWidget::ReleaseMouse()
{
  s_pMouseCapture = NULL;
}



//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Events
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void lUIWidget::OnMouseLeave()
{
}

void lUIWidget::OnMouseLeaveHot()
{
}

void lUIWidget::OnMouseOver()
{
}

void lUIWidget::OnMouseOverHot()
{
}

void lUIWidget::OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags )
{
  L_UNUSED_PARAM( p );
  L_UNUSED_PARAM( mouseDownFlags );
  L_UNUSED_PARAM( mouseUpFlags );
}

bool lUIWidget::OnMouseDown( const lUIPoint & p, luint32_t mouseDownFlags )
{
  L_UNUSED_PARAM( p );
  L_UNUSED_PARAM( mouseDownFlags );
  return false;
}

bool lUIWidget::OnMouseUp( const lUIPoint & p, luint32_t mouseUpFlags )
{
  L_UNUSED_PARAM( p );
  L_UNUSED_PARAM( mouseUpFlags );
  return false;
}

void lUIWidget::OnMouseHot( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags )
{
  L_UNUSED_PARAM( p );
  L_UNUSED_PARAM( mouseDownFlags );
  L_UNUSED_PARAM( mouseUpFlags );
}

void lUIWidget::OnGainFocus()
{
}

void lUIWidget::OnLoseFocus()
{
}

void lUIWidget::OnKey( luint8_t c, luint8_t ext )
{
  L_UNUSED_PARAM( c );
  L_UNUSED_PARAM( ext );
}

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void lUIWidget::SetStylusInput( bool b )
{
  s_stylusInput = b;
}

bool lUIWidget::GetStylusInput()
{
  return s_stylusInput;
}

void lUIWidget::SetDefaultFont(lSharedPtr<lFont> font)
{
  s_defaultFont = font;
}

lSharedPtr<lFont> lUIWidget::GetDefaultFont()
{
  return s_defaultFont;
}

luint32_t lUIWidget::GetWidgetFlags()
{
  return m_widgetFlags;
}

void lUIWidget::SetWidgetFlags(luint32_t flags)
{
  m_widgetFlags = flags;
}





