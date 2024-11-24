//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdbuttonrolloutwidget.cpp
// Description : FDButtonRolloutWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdbuttonrolloutwidget.h>
#include <framework/lsprite.h>

#include <fdresourcemanager.h>
#include <fdsound.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDButtonRolloutWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDButtonRolloutWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void FDButtonRolloutWidget::ClassInitialiser()
{
}

FDButtonRolloutWidget::FDButtonRolloutWidget()
  : m_state( STATE_ROLLED_IN )
{
}

FDButtonRolloutWidget::FDButtonRolloutWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                                              lSharedPtr<lSprite> sprite,
                                              const lUIWidgetPosition anchor,
                                              const lUIWidgetPosition pos,
                                              const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags, id, anchor, pos, size ),
    m_sprite(sprite),
    m_state( STATE_ROLLED_IN )
{
  m_rod = FDResourceManager::GetSprite("ui","menu_rod");
  m_selectedIndex = -1;
}

FDButtonRolloutWidget::~FDButtonRolloutWidget()
{
}

bool FDButtonRolloutWidget::IsRolledOrRollingOut() const
{
	return (m_state != STATE_ROLLED_IN);
}

lUIPoint FDButtonRolloutWidget::GetIdealSize() const
{
  lUIPoint size(0,0);

  if( m_sprite )
  {
    size = lUIPoint( m_sprite->GetWidth(), m_sprite->GetHeight() );
  }

  if( m_state == STATE_ROLLED_OUT )
  {
    size.x += 25 + (30 * m_buttonList.Size());
  }

  return size;
}

void FDButtonRolloutWidget::Render()
{
  if( m_sprite )
  {
    lUIRect rect = WidgetToScreen( GetRect() );

    if( m_state == STATE_ROLLING_OUT )
    {
      m_state = STATE_ROLLED_OUT;

      for( int i=0;i<m_buttonList.Size();i++ )
      {
        m_buttonList(i)->SetColour( L_ARGB(255,255,255,255) );
      }
    }

    if( m_state == STATE_ROLLED_OUT )
    {
      lUIPoint pos( rect.x, rect.y );

      m_rod->SetPosition( pos.x + (m_sprite->GetWidth() * 0.5f), pos.y + (m_sprite->GetHeight() * 0.5f) );
      m_rod->Render();

      pos.x += 25;

      for( int i=0;i<m_buttonList.Size();i++ )
      {
        if( i < m_buttonList.Size()-1 )
        {
          m_rod->SetPosition( pos.x + (m_buttonList(i)->GetWidth() * 0.5f) , pos.y + (m_sprite->GetHeight() * 0.5f) );
          m_rod->Render();
        }

        if( m_selectedIndex == i )
        {
          m_buttonList(i)->SetColour( L_ARGB( 255, 255, 255, 255 ) );
        } else
        {
          m_buttonList(i)->SetColour( L_ARGB( 255, 100, 100, 100 ) );
        }

        m_buttonList(i)->SetPosition( pos.x, pos.y + ( (rect.h - m_buttonList(i)->GetHeight()) * 0.5f ) );
        m_buttonList(i)->Render();

        pos.x += 30;
      }

      m_sprite->SetColour( L_ARGB(255,255,0,0) );
    }
    else
    {
      m_sprite->SetColour( L_ARGB(255,255,255,255) );
    }

    m_sprite->SetPosition( rect.x, rect.y );
    m_sprite->Render();
  }
}

bool FDButtonRolloutWidget::OnMouseDown(const lUIPoint &pos, luint32_t buttonFlags)
{
  SuperClass::OnMouseDown(pos,buttonFlags);

  if( !IsEnabled() )
  {
    // disabled
    return true;
  }

  lUIPoint screenPos = WidgetClientToScreen( pos );

  if( m_state == STATE_ROLLED_IN )
  {
    if( m_sprite->MouseHit( screenPos.x, screenPos.y ) )
    {
      // default selection 0
      lLogFmt("Rolling out\n");
      SignalHot( this, 0 );
      m_selectedIndex = 0;
      m_state = STATE_ROLLING_OUT;
      FDSound::PlaySoundEffect( FD_SFX_UI_SIDE_BUTTON );
    }
  } else
  if( m_state == STATE_ROLLED_OUT )
  {
    if( m_sprite->MouseHit( screenPos.x, screenPos.y ) )
    {
      lLogFmt("Click menu icon - roll in\n");
      m_selectedIndex = -1;
      m_state = STATE_ROLLED_IN;
      SignalHot( this, -1 );
      FDSound::PlaySoundEffect( FD_SFX_UI_DISABLE );
      return true;
    }

    for( int i=0;i<m_buttonList.Size();i++ )
    {
      if( m_buttonList(i)->MouseHit( screenPos.x, screenPos.y ) )
      {
        if( m_selectedIndex != i )
        {
          // first click, set hot
          lLogFmt("1st Mouse click on %d\n", i);
          m_selectedIndex = i;
          SignalHot( this, i );
          FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );
        } else
        {
          // second click, activate
          lLogFmt("2nd Mouse click on %d\n", i);
          m_selectedIndex = -1;
          m_state = STATE_ROLLED_IN;
          SignalHot( this, -1 );
          SignalCommand( this, i );
          FDSound::PlaySoundEffect( FD_SFX_UI_SIDE_BUTTON );
        }

        break;
      }
    }
  }

  return true;
}

void FDButtonRolloutWidget::AddButton( lSharedPtr<lSprite> buttonSprite )
{
  m_buttonList.Push( buttonSprite );
}

void FDButtonRolloutWidget::ChangeButton( int index,  lSharedPtr<lSprite> buttonSprite )
{
  m_buttonList( index ) = buttonSprite;
}

void FDButtonRolloutWidget::RollIn()
{
  m_selectedIndex = -1;
  m_state = STATE_ROLLED_IN;
  SignalHot( this, -1 );
}


