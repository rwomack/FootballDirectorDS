//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : luispritewidget.cpp
// Description : lUISpriteWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <ui/luispritewidget.h>
#include <framework/lsprite.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( lUISpriteWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUISpriteWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void lUISpriteWidget::ClassInitialiser()
{
}

lUISpriteWidget::lUISpriteWidget()
{
}

lUISpriteWidget::lUISpriteWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                                  lSharedPtr<lSprite> sprite,
                                  const lUIWidgetPosition anchor,
                                  const lUIWidgetPosition pos,
                                  const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags, id, anchor, pos, size ),
    m_sprite( sprite )
{
}

lUISpriteWidget::~lUISpriteWidget()
{
}

void lUISpriteWidget::SetSprite( lSharedPtr<lSprite> sprite )
{
  m_sprite = sprite;
}

lSharedPtr<lSprite> lUISpriteWidget::GetSprite()
{
  return m_sprite;
}

// lUIWidget interface

void lUISpriteWidget::Tick(float delta)
{
  SuperClass::Tick( delta );
  if( m_sprite )
  {
    m_sprite->Tick( delta );
  }
}

void lUISpriteWidget::Render()
{
  if( m_sprite )
  {
    lUIRect rect = WidgetToScreen( GetRect() );

    float w = m_sprite->GetWidth();
    float h = m_sprite->GetHeight();

    m_sprite->SetPosition( rect.x, rect.y );
    m_sprite->SetWidth( rect.w );
    m_sprite->SetHeight( rect.h );
    m_sprite->Render();

    m_sprite->SetWidth( w );
    m_sprite->SetHeight( h );
  }
}

lUIPoint lUISpriteWidget::GetIdealSize() const
{
  if( m_sprite )
  {
    return lUIPoint( m_sprite->GetWidth(), m_sprite->GetHeight() );
  }

  return SuperClass::GetIdealSize();
}

