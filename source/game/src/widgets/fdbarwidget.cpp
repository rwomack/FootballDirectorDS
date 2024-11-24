//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdbarwidget.cpp
// Description : FDBarWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdbarwidget.h>
#include <framework/lsprite.h>
#include <fdresourcemanager.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDBarWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDBarWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void FDBarWidget::ClassInitialiser()
{
}

FDBarWidget::FDBarWidget()
  : m_colourA( L_ARGB( 255, 255, 0, 0 ) ), m_colourB( L_ARGB( 255, 0, 0, 255 ) ), m_pctA( 0.5f )
{
	Init();
}

FDBarWidget::FDBarWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                          const lUIWidgetPosition anchor,
                          const lUIWidgetPosition pos,
                          const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags | LUI_WIDGET_MOUSETRANSPARENT, id, anchor, pos, size ),
    m_colourA( L_ARGB( 255, 255, 0, 0 ) ),
    m_colourB( L_ARGB( 255, 0, 0, 255 ) ),
    m_pctA( 0.5f )
{
	Init();
}

FDBarWidget::~FDBarWidget()
{
}

void FDBarWidget::Init()
{
	m_gradient = FDResourceManager::GetSprite("uiicon", "grad_white");
}

lUIPoint FDBarWidget::GetIdealSize() const
{
  // Is there an ideal size?
  return lUIPoint( lSpriteManager::GetDisplayWidth()/3, lSpriteManager::GetDisplayHeight()/10 );
}

void FDBarWidget::Render()
{
  //TODO: Support for vertical bars - add a flag
  lUIRect rect = WidgetToScreen( GetRect() );

  // render rects
  int barASize = int(rect.w * m_pctA);

  if( m_pctA > 0.0f )
  {
    m_gradient->SetColour(m_colourA);
	m_gradient->SetX(rect.x);
	m_gradient->SetY(rect.y);
	m_gradient->SetWidth(barASize);
	m_gradient->SetHeight(rect.h);
	m_gradient->Render();
  }

  if( m_pctA < 1.0f )
  {
    m_gradient->SetColour(m_colourB);
	m_gradient->SetX(rect.x + barASize);
	m_gradient->SetY(rect.y);
	m_gradient->SetWidth(rect.w - barASize);
	m_gradient->SetHeight(rect.h);
	m_gradient->Render();
  }

  // Render a black line between bars if they are the same colour and not either black or white
  if( m_colourA == m_colourB && 0xffffffff != m_colourA )
    lSpriteManager::DrawRect2( rect.x + barASize, rect.y, 1, rect.h, 0xffffffff );
  else if( m_colourA == m_colourB && 0xff000000 != m_colourA )
    lSpriteManager::DrawRect2( rect.x + barASize, rect.y, 1, rect.h, 0xff000000);
}

void FDBarWidget::Tick( float delta )
{
}

void FDBarWidget::SetColourA( luint32_t colour )
{
  m_colourA = colour;
}

luint32_t FDBarWidget::GetColourA()
{
  return m_colourA;
}

void FDBarWidget::SetColourB( luint32_t colour )
{
  m_colourB = colour;
}

luint32_t FDBarWidget::GetColourB()
{
  return m_colourB;
}

void FDBarWidget::SetPctA( float pctA )
{
  m_pctA = pctA;
}



