//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdhighlightwidget.cpp
// Description : FDHighlightWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdhighlightwidget.h>
#include <framework/lsprite.h>
#include <fdresourcemanager.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDHighlightWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDHighlightWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void FDHighlightWidget::ClassInitialiser()
{
}

FDHighlightWidget::FDHighlightWidget()
  : m_colour( L_ARGB( 255, 0, 0, 0 ) )
{
}

FDHighlightWidget::FDHighlightWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                                        luint32_t   colour,
                                        const lUIWidgetPosition anchor,
                                        const lUIWidgetPosition pos,
                                        const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags /*| LUI_WIDGET_MOUSETRANSPARENT*/, id, anchor, pos, size ),
    m_colour( colour ),
    m_x(0), m_y(0), m_w(1), m_h(1)
{
}

FDHighlightWidget::~FDHighlightWidget()
{
}

lUIPoint FDHighlightWidget::GetIdealSize() const
{
  return lUIPoint( lSpriteManager::GetDisplayWidth(), lSpriteManager::GetDisplayHeight() );
}

void FDHighlightWidget::Render()
{
  // render full screen rect
  int rectx0 = (m_x);// * lSpriteManager::GetDisplayWidth();
  int recty0 = (m_y);// * lSpriteManager::GetDisplayHeight();
  int rectx1 = (m_x+m_w);// * lSpriteManager::GetDisplayWidth();
  int recty1 = (m_y+m_h);// * lSpriteManager::GetDisplayHeight();

  if( rectx0 < 0 ) rectx0 = 0;
  if( recty0 < 0 ) recty0 = 0;
  if( rectx0 >= 256 ) rectx0 = 255;
  if( recty0 >= 192 ) recty0 = 191;

  if( rectx1 < 0 ) rectx1 = 0;
  if( recty1 < 0 ) recty1 = 0;
  if( rectx1 >= 256 ) rectx1 = 255;
  if( recty1 >= 192 ) recty1 = 191;

  int rectw = rectx1 - rectx0;
  int recth = recty1 - recty0;

  lSpriteManager::DrawRect2( 0, 0, lSpriteManager::GetDisplayWidth(), lSpriteManager::GetDisplayHeight(), L_ARGB(180,0,0,0) );
  lSpriteManager::DrawRect2( rectx0, recty0, rectw, recth, L_ARGB(50,255,255,255) );
}

void FDHighlightWidget::Tick( float delta )
{
  //TODO: Add fades
}

void FDHighlightWidget::SetColour( luint32_t colour )
{
  m_colour = colour;
}

luint32_t FDHighlightWidget::GetColour()
{
  return m_colour;
}

void FDHighlightWidget::SetRect( float x, float y, float w, float h )
{
  m_x = x;
  m_y = y;
  m_w = w;
  m_h = h;
}

