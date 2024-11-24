//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdbuttonwidget.cpp
// Description : FDButtonWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdbuttonwidget.h>
#include <framework/lsprite.h>
#include <fdsound.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDButtonWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDButtonWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void FDButtonWidget::ClassInitialiser()
{
}

FDButtonWidget::FDButtonWidget()
{
  m_bDisabled = false;
}

//BUTTON_FLAG_STYLUS_ONECLICK

FDButtonWidget::FDButtonWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                                lSharedPtr<lSprite> sprite,
                                luint32_t buttonFlags,
                                const lUIWidgetPosition anchor,
                                const lUIWidgetPosition pos,
                                const lUIWidgetSize size )
  : lUIButtonWidget( parent, widgetFlags, id, "", buttonFlags, anchor, pos, size ),
    m_sprite( sprite ),
    m_bDisabled( false ),
    m_bHighlight( false )
{
}

FDButtonWidget::~FDButtonWidget()
{
}

lUIPoint FDButtonWidget::GetIdealSize() const
{
  if( !m_sprite )
  {
    return SuperClass::GetIdealSize();
  }

  return lUIPoint(m_sprite->GetWidth(),m_sprite->GetHeight());
}

void FDButtonWidget::SetSprite( lSharedPtr<lSprite> sprite )
{
  m_sprite = sprite;
}

lSharedPtr<lSprite> FDButtonWidget::GetSprite()
{
  return m_sprite;
}

void FDButtonWidget::SetSpriteOn( lSharedPtr<lSprite> sprite )
{
  m_spriteOn = sprite;
}

lSharedPtr<lSprite> FDButtonWidget::GetSpriteOn()
{
  return m_spriteOn;
}

void FDButtonWidget::SetDisabledSprite( lSharedPtr<lSprite> sprite )
{
  m_disabledSprite = sprite;
}

lSharedPtr<lSprite> FDButtonWidget::GetDisabledSprite()
{
  return m_disabledSprite;
}

void FDButtonWidget::Render()
{
  if( m_sprite )
  {
    lUIRect rect = WidgetToScreen( GetRect() );

    luint32_t colour = L_ARGB(255,255,255,255);

    if( m_bDisabled )
    {
      if( !m_disabledSprite )
      {
        // don't grey if we have a disabled sprite
        colour = L_ARGB(255,128,128,128);
      }
    } else
    {
      if( IsHover() )    colour = L_ARGB(255,150,150,150);
      if( m_bHighlight ) colour = L_ARGB(255,0,255,0);
    }

    if( IsHover() && m_spriteOn )
    {
      m_spriteOn->SetPosition( rect.x, rect.y );
      m_spriteOn->SetWidth( rect.w );
      m_spriteOn->SetHeight( rect.h );
      m_spriteOn->SetColour( colour );
      m_spriteOn->Render();
    } else
    if( m_bDisabled && m_disabledSprite )
    {
      m_disabledSprite->SetPosition( rect.x, rect.y );
      m_disabledSprite->SetWidth( rect.w );
      m_disabledSprite->SetHeight( rect.h );
      m_disabledSprite->SetColour( colour );
      m_disabledSprite->Render();
    } else
    {
      m_sprite->SetPosition( rect.x, rect.y );
      m_sprite->SetWidth( rect.w );
      m_sprite->SetHeight( rect.h );
      m_sprite->SetColour( colour );
      m_sprite->Render();
    }
  }

  if( m_text.str.Size() )
  {
    lSharedPtr<lFont> font = GetFont();
    lString           text = m_text.str;
    lUIRect           rect = WidgetToScreen( GetRect() );

    lUIPoint offset(0,0);

    int width;
    int height;
    font->GetStringBound( width, height, text );

    offset = lUICalcAlign( m_textAlign, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );

    if( m_bDisabled )
    {
      font->SetColour( L_ARGB(255,128,128,128) );
    } else
    {
      if( IsChecked() )
      {
        font->SetColour( L_ARGB(255,0,255,0) );
      }
      else if( IsDown() )
      {
        //font->SetColour( L_ARGB(255,255,255,255) );
        font->SetColour( L_ARGB(255,0,0,0) );
      }
      else
      {
        //font->SetColour( L_ARGB(255,0,0,0) );
        font->SetColour( L_ARGB(255,255,255,255) );
      }
    }

    font->Print( int(rect.x+offset.x),int(rect.y+offset.y), text );
  }
}

void FDButtonWidget::Enable()
{
  m_bDisabled = false;
}

void FDButtonWidget::Disable()
{
  m_bDisabled = true;
}

bool FDButtonWidget::IsDisabled()
{
  return m_bDisabled;
}

void FDButtonWidget::Highlight( bool hi )
{
  m_bHighlight = hi;
}

void     FDButtonWidget::OnPress()
{
  if( !(GetButtonFlags() & FDBUTTON_FLAG_NO_SOUND) )
  {
    //FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );

  }
}

void     FDButtonWidget::OnClick()
{
  if( !(GetButtonFlags() & FDBUTTON_FLAG_NO_SOUND) )
  {
    if( !IsDisabled() )
    {
      FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );
    }
  }
}


