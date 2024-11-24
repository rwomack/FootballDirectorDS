//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdtextwidget.cpp
// Description : FDTextWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdtextwidget.h>
#include <framework/lsprite.h>
#include <fdresourcemanager.h>
#include <fdtext.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDTextWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTextWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void FDTextWidget::ClassInitialiser()
{
}


FDTextWidget::FDTextWidget()
  : m_colour( L_ARGB(255,255,255,255) ),
    m_align( LUI_CENTER_CENTER )
{
}

FDTextWidget::FDTextWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                              const lUIText & text,
                              const lUIWidgetPosition anchor,
                              const lUIWidgetPosition pos,
                              const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags, id, anchor, pos, size ),
    m_text( text ),
    m_colour( L_ARGB(255,255,255,255) ),
    m_align( LUI_CENTER_CENTER )
{
}

FDTextWidget::~FDTextWidget()
{
}

void FDTextWidget::Tick(float delta)
{
}

lUIPoint FDTextWidget::GetIdealSize() const
{
  lSharedPtr<lFont> font = GetFont();

  float maxWidth = PixelSizeX( lUICoordPercent(100) );
  lString text = SplitText( font.Get(), m_text.str, maxWidth / (float)lSpriteManager::GetDisplayWidth() );

  int width;
  int height;
  font->GetStringBound( width, height, text );

  return lUIPoint( width / (float)lSpriteManager::GetDisplayWidth(), height / (float)lSpriteManager::GetDisplayHeight() );
}

void FDTextWidget::Render()
{
  lSharedPtr<lFont> font = GetFont();

  lUIRect rect = WidgetToScreen( GetRect() );

  float maxWidth = PixelSizeX( lUICoord(rect.w) );
  lString text = SplitText( font.Get(), m_text.str, maxWidth / (float)lSpriteManager::GetDisplayWidth() );

  font->SetColour( m_colour );

  lUIAlignAxis xAlign = lUIAlignAxis( (m_align>>4) & 0xF );
  lUIAlignAxis yAlign = lUIAlignAxis( m_align & 0xF );

  // count lines

  int numLines = 1;
  const char * line = text.CStr();

  while( *line )
  {
    if( *line == '\n' )
    {
      numLines++;
    }

    line++;
  }

  float yOffset = lUICalcAlignAxis( yAlign, rect.h, font->DefaultPixelHeight() * numLines );

  // process each line.

  lUIPoint pos = lUIPoint( rect.x, rect.y + yOffset );

  char * s = const_cast<char *>(text.CStr()); // HACK
  line = s;

  while( 1 )
  {
    if( *s == '\n' || *s == '\0' )
    {
      bool bDone = false;
      if( *s == '\0' ) bDone = true;

      char tmp = *s;
      *s = '\0';

      int width;
      int height;
      FDTextGetStringBound( font.Get(), width, height, line );

      float xOffset = lUICalcAlignAxis( xAlign, rect.w, width );

      FDTextPrint( font.Get(), int(pos.x+xOffset), int(pos.y), line );
      pos.y += font->DefaultPixelHeight();
      pos.x = rect.x;

      if( bDone ) break;

      *s = tmp;
      s++;
      line = s;
    }
    else
    {
      s++;
    }
  }
}

void FDTextWidget::SetText( const lUIText & text )
{
  m_text = text;
}

const lUIText & FDTextWidget::GetText()
{
  return m_text;
}

void FDTextWidget::SetTextColour( luint32_t colour )
{
  m_colour = colour;
}

luint32_t FDTextWidget::GetTextColour()
{
  return m_colour;
}

void FDTextWidget::SetAlign( lUIAlign align )
{
  m_align = align;
}

lUIAlign FDTextWidget::GetAlign() const
{
  return m_align;
}
