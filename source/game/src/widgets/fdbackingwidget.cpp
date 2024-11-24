//����������������������������������������������������������������������������
// File        : fdfullscreenwidget.cpp
// Description : FDBackingWidget implementation
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdbackingwidget.h>
#include <framework/lsprite.h>
#include <fdresourcemanager.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDBackingWidget, lGUID(0,0,0,0), 0 );

//����������������������������������������������������������������������������
// FDBackingWidget
//����������������������������������������������������������������������������

void FDBackingWidget::ClassInitialiser()
{
}

FDBackingWidget::FDBackingWidget()
  : m_bgColour( L_ARGB( 128, 0, 0, 0 ) ),
    m_lineColour( L_ARGB( 128, 0, 0, 0 ) ),
    m_lineWidth(0)
{
}

FDBackingWidget::FDBackingWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                                        luint32_t   colour,
                                        const lUIWidgetPosition anchor,
                                        const lUIWidgetPosition pos,
                                        const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags /*| LUI_WIDGET_MOUSETRANSPARENT*/, id, anchor, pos, size ),
    m_bgColour( colour ),
    m_lineColour( colour ),
    m_lineWidth(0)
{
}

FDBackingWidget::~FDBackingWidget()
{
}

lUIPoint FDBackingWidget::GetIdealSize() const
{
  lUIRect rect = GetRect();

  return lUIPoint( rect.x, rect.y );
}

void FDBackingWidget::Render()
{
  if(!IsVisible())
    return;

  lUIRect rect = GetRect();
  // render full screen rect
  lSpriteManager::DrawRect2( luint32_t(rect.x), luint32_t(rect.y), luint32_t(rect.w), luint32_t(rect.h), m_bgColour );

  if( m_lineWidth != 0 )
  {
    float halfWidth = m_lineWidth * 0.5f;

    // top left to top right
    lSpriteManager::DrawRect2( luint32_t(rect.x), luint32_t(rect.y - halfWidth), luint32_t(rect.w), luint32_t( m_lineWidth ), m_lineColour );

    // bottom left to bottom right
    lSpriteManager::DrawRect2( luint32_t(rect.x), luint32_t(rect.y + rect.h - halfWidth), luint32_t(rect.w), luint32_t( m_lineWidth ), m_lineColour );
  }
}

void FDBackingWidget::Tick( float delta )
{
}

void FDBackingWidget::SetColour( luint32_t colour )
{
  m_bgColour = colour;
}

luint32_t FDBackingWidget::GetColour()
{
  return m_bgColour;
}

void FDBackingWidget::SetLineColour( luint32_t colour )
{
  m_lineColour = colour;
}

luint32_t FDBackingWidget::GetLineColour()
{
  return m_lineColour;
}

void FDBackingWidget::SetLines( int width, luint32_t colour )
{
  m_lineColour = colour;
  m_lineWidth = width;
}

