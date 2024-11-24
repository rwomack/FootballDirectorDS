//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdfullscreenwidget.cpp
// Description : FDFullScreenWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdfullscreenwidget.h>
#include <framework/lsprite.h>
#include <fdresourcemanager.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDFullScreenWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDFullScreenWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void FDFullScreenWidget::ClassInitialiser()
{
}

FDFullScreenWidget::FDFullScreenWidget()
  : m_colour( L_ARGB( 255, 0, 0, 0 ) ),
    m_bTapped( false ),
    m_tapIgnoreDelay(0.0f)
{
}

FDFullScreenWidget::FDFullScreenWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                                        luint32_t   colour,
                                        const lUIWidgetPosition anchor,
                                        const lUIWidgetPosition pos,
                                        const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags /*| LUI_WIDGET_MOUSETRANSPARENT*/, id, anchor, pos, size ),
    m_colour( colour ),
    m_bTapped( false ),
    m_tapIgnoreDelay(0.0f)
{
}

FDFullScreenWidget::~FDFullScreenWidget()
{
}

lUIPoint FDFullScreenWidget::GetIdealSize() const
{
  return lUIPoint( lSpriteManager::GetDisplayWidth(), lSpriteManager::GetDisplayHeight() );
}

void FDFullScreenWidget::Render()
{
  // render full screen rect
  lSpriteManager::DrawRect2( 0, 0, lSpriteManager::GetDisplayWidth(), lSpriteManager::GetDisplayHeight(), m_colour );
}

bool FDFullScreenWidget::OnMouseUp( const lUIPoint &, luint32_t )
{
  lLogFmt("FullScreen: OnMouseUp\n");
  if( m_tapIgnoreDelay <= 0.0f )
  {
    lLogFmt("Tapped!\n");
    m_bTapped = true;
    SignalTapped.Emit(this);
  } else
  {
    lLogFmt("Ignoring due to m_tapIgnoreDelay\n");
  }
  return true;
}

void FDFullScreenWidget::Tick( float delta )
{
  if( m_tapIgnoreDelay > 0.0f )
  {
    //lLogFmt("FullScreen Tick %.2f\n", m_tapIgnoreDelay);
  }
  m_tapIgnoreDelay -= delta;
}

void FDFullScreenWidget::SetTapIgnoreDelay( float delay )
{
  m_tapIgnoreDelay = delay;
}

void FDFullScreenWidget::SetColour( luint32_t colour )
{
  m_colour = colour;
}

luint32_t FDFullScreenWidget::GetColour()
{
  return m_colour;
}

void FDFullScreenWidget::ClearTapped()
{
  m_bTapped = false;
}

bool FDFullScreenWidget::GetTapped()
{
  return m_bTapped;
}

