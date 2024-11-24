//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdcheckboxwidget.cpp
// Description : FDCheckBoxWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdcheckboxwidget.h>
#include <framework/lsprite.h>
#include <fdsound.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDCheckBoxWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDCheckBoxWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void FDCheckBoxWidget::ClassInitialiser()
{
}

FDCheckBoxWidget::FDCheckBoxWidget()
{
  m_bDisabled = false;
}

//BUTTON_FLAG_STYLUS_ONECLICK

FDCheckBoxWidget::FDCheckBoxWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                                lSharedPtr<lSprite> spriteOff, lSharedPtr<lSprite> spriteOn,
                                luint32_t buttonFlags,
                                const lUIWidgetPosition anchor,
                                const lUIWidgetPosition pos,
                                const lUIWidgetSize size )
  : lUIButtonWidget( parent, widgetFlags, id, "", buttonFlags | BUTTON_FLAG_CHECKBOX, anchor, pos, size ),
    m_spriteOn( spriteOn ),
    m_spriteOff( spriteOff ),
    m_bDisabled( false )
{
}

FDCheckBoxWidget::~FDCheckBoxWidget()
{
}

lUIPoint FDCheckBoxWidget::GetIdealSize() const
{
  if( !(m_spriteOn && m_spriteOff) )
  {
    return SuperClass::GetIdealSize();
  }

  //TODO: Include space on right for text

  int maxWidth  = lmMax<int>( m_spriteOn->GetWidth(), m_spriteOff->GetWidth() );
  int maxHeight = lmMax<int>( m_spriteOn->GetHeight(), m_spriteOff->GetHeight() );

  return lUIPoint(maxWidth, maxHeight);
}

void FDCheckBoxWidget::SetSprite( lSharedPtr<lSprite> spriteOn, lSharedPtr<lSprite> spriteOff )
{
  m_spriteOn  = spriteOn;
  m_spriteOff = spriteOff;
}

lSharedPtr<lSprite> FDCheckBoxWidget::GetSpriteOn()
{
  return m_spriteOn;
}

lSharedPtr<lSprite> FDCheckBoxWidget::GetSpriteOff()
{
  return m_spriteOff;
}

void FDCheckBoxWidget::Render()
{
  if( m_spriteOn && m_spriteOff )
  {
    lUIRect rect = WidgetToScreen( GetRect() );

    luint32_t colour;

    if( m_bDisabled )
    {
      colour = L_ARGB(255,128,128,128);
    } else
    {
      if( IsHover() ) colour = L_ARGB(255,255,0,0);
      else colour = L_ARGB(255,255,255,255);
    }

    if( m_state & BUTTON_STATE_CHECKED  )
    {
      m_spriteOn->SetPosition( rect.x, rect.y );
      m_spriteOn->SetWidth( rect.w );
      m_spriteOn->SetHeight( rect.h );
      m_spriteOn->SetColour( colour );
      m_spriteOn->Render();
    } else
    {
      m_spriteOff->SetPosition( rect.x, rect.y );
      m_spriteOff->SetWidth( rect.w );
      m_spriteOff->SetHeight( rect.h );
      m_spriteOff->SetColour( colour );
      m_spriteOff->Render();
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

    offset = lUICalcAlign( /*m_textAlign*/ LUI_BOTTOM_CENTER, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );

    if( m_bDisabled )
    {
      font->SetColour( L_ARGB(255,128,128,128) );
    } else
    {
      if( IsDown() )
      {
        font->SetColour( L_ARGB(255,0,0,255) );
      }
      else
      {
        font->SetColour( L_ARGB(255,255,255,255) );
      }
    }

    // Print to the right of the check box
    font->Print( int(rect.x+rect.w+2),int(rect.y+offset.y + 3), text );
  }
}

void FDCheckBoxWidget::Enable()
{
  m_bDisabled = false;
}

void FDCheckBoxWidget::Disable()
{
  m_bDisabled = true;
}

bool FDCheckBoxWidget::IsDisabled()
{
  return m_bDisabled;
}

void FDCheckBoxWidget::OnClick()
{
  FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );
}

