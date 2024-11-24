//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDMenuWidget.cpp
// Description : FDMenuWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmenuwidget.h>
#include <framework/lsprite.h>

#include <fdresourcemanager.h>
#include <fdsound.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDMenuWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDMenuWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

bool FDMenuWidget::ms_bMenuActive = false;

void FDMenuWidget::ClassInitialiser()
{
}

FDMenuWidget::FDMenuWidget()
  : m_state( STATE_ROLLED_IN ),
    m_bGlow(false)
{
}

FDMenuWidget::FDMenuWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                                              lSharedPtr<lSprite> sprite,
                                              lSharedPtr<lSprite> spriteGlow, lUIPoint spritePos,
                                              const lUIWidgetPosition anchor,
                                              const lUIWidgetPosition pos,
                                              const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags, id, anchor, pos, size ),
    m_sprite(sprite),
    m_spriteGlow(spriteGlow),
    m_bGlow(false),
    m_state( STATE_ROLLED_IN )
{
  //m_rod = FDResourceManager::GetSprite("ui","menu_rod");
  m_selectedIndex = -1;
  Move( spritePos );
  m_sprite->SetPosition( spritePos.x, spritePos.y );
  if( m_spriteGlow )
    m_spriteGlow->SetPosition( spritePos.x, spritePos.y );
}

FDMenuWidget::~FDMenuWidget()
{
}

bool FDMenuWidget::IsRolledOrRollingOut() const
{
	return (m_state != STATE_ROLLED_IN);
}

lUIPoint FDMenuWidget::GetIdealSize() const
{
  lUIPoint size(0,0);

  if( m_sprite )
  {
    size = lUIPoint( m_sprite->GetWidth(), m_sprite->GetHeight() );
  }

  if( m_state == STATE_ROLLED_OUT )
  {
    // todo setup properly
    size.x  = 256-50;
    size.y  = 192;
  }

  return size;
}

void FDMenuWidget::Render()
{
  if( m_sprite )
  {
    lUIRect rect = WidgetToScreen( GetRect() );

    if( m_state == STATE_ROLLING_OUT )
    {
      m_state = STATE_ROLLED_OUT;
    }

    if( m_state == STATE_ROLLED_OUT )
    {
      lUIPoint pos( 50, 0 );

//      m_rod->SetPosition( pos.x + (m_sprite->GetWidth() * 0.5f), pos.y + (m_sprite->GetHeight() * 0.5f) );
//      m_rod->Render();

      pos.y = (192/2) - ( ( m_buttonList.Size() * m_buttonList(0).buttonOff->GetHeight() ) / 2 );

      for( int i=0;i<m_buttonList.Size();i++ )
      {
		uint32 uiColour = (m_buttonList(i).bEnabled ? L_ARGB(255, 255, 255, 255) : L_ARGB(255, 100, 100, 100));
		if (m_buttonList(i).buttonOn != NULL)
			m_buttonList(i).buttonOn->SetColour(uiColour);
		if (m_buttonList(i).buttonOff != NULL)
			m_buttonList(i).buttonOff->SetColour(uiColour);
		if (m_buttonList(i).textOn != NULL)
			m_buttonList(i).textOn->SetColour(uiColour);
		if (m_buttonList(i).textOff != NULL)
			m_buttonList(i).textOff->SetColour(uiColour);
		if (m_buttonList(i).icon != NULL)
			m_buttonList(i).icon->SetColour(uiColour);
		if (m_buttonList(i).iconGlow != NULL)
			m_buttonList(i).iconGlow->SetColour(uiColour);

        m_buttonList(i).buttonOn->SetPosition( pos.x, pos.y );
        m_buttonList(i).buttonOff->SetPosition( pos.x, pos.y );

        if( m_selectedIndex == i )
        {
          m_buttonList(i).buttonOn->Render();
          m_buttonList(i).textOn->SetPosition( pos.x, pos.y );
          m_buttonList(i).textOn->Render();
        } else
        {
          m_buttonList(i).buttonOff->Render();
          m_buttonList(i).textOff->SetPosition( pos.x, pos.y );
          m_buttonList(i).textOff->Render();
        }

        if( !m_buttonList(i).bUseGlow )
        {
          m_buttonList(i).icon->SetPosition( pos.x, pos.y );
          m_buttonList(i).icon->Render();
        } else
        {
          m_buttonList(i).iconGlow->SetPosition( pos.x, pos.y );
          m_buttonList(i).iconGlow->Render();
        }

        pos.y += m_buttonList(i).buttonOff->GetHeight();
      }

      m_sprite->SetColour( L_ARGB(255,255,0,0) );
    }
    else
    {
      m_sprite->SetColour( L_ARGB(255,255,255,255) );
    }

    //m_sprite->SetPosition( rect.x, rect.y );

    if( m_bGlow && m_spriteGlow )
    {
      m_spriteGlow->Render();
    }
    else
    {
      m_sprite->Render();
    }
  }
}

bool FDMenuWidget::OnMouseDown(const lUIPoint &pos, luint32_t buttonFlags)
{
  SuperClass::OnMouseDown(pos,buttonFlags);

  if( !IsEnabled() )
  {
    // disabled
    return true;
  }

  lUIPoint screenPos = WidgetClientToScreen( pos );

  if( m_state == STATE_ROLLED_OUT )
  {
    if( m_sprite->MouseHit( screenPos.x, screenPos.y ) )
    {
      lLogFmt("Click menu icon - roll in\n");
      RollIn();
      FDSound::PlaySoundEffect( FD_SFX_UI_DISABLE );
      return true;
    }

    for( int i=0;i<m_buttonList.Size();i++ )
    {
      if( m_buttonList(i).bEnabled && m_buttonList(i).buttonOn->MouseHit( screenPos.x, screenPos.y ) )
      {
//        if( m_selectedIndex != i )
//        {
//          // first click, set hot
//          lLogFmt("1st Mouse click on %d\n", i);
//          m_selectedIndex = i;
//          SignalHot( this, i );
//          FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );
//        } else
        {
          // second click, activate
          lLogFmt("2nd Mouse click on %d\n", i);
          RollIn();
          m_selectedIndex = i;
          SignalCommand( this, i );
          FDSound::PlaySoundEffect( FD_SFX_UI_SIDE_BUTTON );
        }

        break;
      }
    }
  }

  return true;
}

bool FDMenuWidget::OnMouseUp(const lUIPoint &pos, luint32_t buttonFlags)
{
  SuperClass::OnMouseUp(pos,buttonFlags);

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
      m_state = STATE_ROLLING_OUT;
      FDSound::PlaySoundEffect( FD_SFX_UI_SIDE_BUTTON );
      Move( lUIPoint( 50, 0 ) );
	  ms_bMenuActive = true;
    }
  }

  return true;
}

void FDMenuWidget::SetSelectedIndex( int idx )
{
  m_selectedIndex = idx;
}

int FDMenuWidget::AddButton( lSharedPtr<lSprite> buttonOnSprite, lSharedPtr<lSprite> buttonOffSprite,
                              lSharedPtr<lSprite> textOnSprite, lSharedPtr<lSprite> textOffSprite,
                              lSharedPtr<lSprite> iconSprite, lSharedPtr<lSprite> iconGlowSprite )
{
  m_buttonList.Add(1);
  m_buttonList.Last().buttonOn  = buttonOnSprite;
  m_buttonList.Last().buttonOff = buttonOffSprite;
  m_buttonList.Last().textOn    = textOnSprite;
  m_buttonList.Last().textOff   = textOffSprite;
  m_buttonList.Last().icon      = iconSprite;
  m_buttonList.Last().iconGlow  = iconGlowSprite;
  m_buttonList.Last().bUseGlow  = false;
  m_buttonList.Last().bEnabled	= true;

  return m_buttonList.Size()-1;
}

void FDMenuWidget::ChangeButton( int index, bool bUseGlow )
{
  m_buttonList( index ).bUseGlow = bUseGlow;
  m_bGlow = bUseGlow;
}

void FDMenuWidget::SetButtonEnabled(const int iIndex, const bool bEnabled)
{
	m_buttonList(iIndex).bEnabled = bEnabled;
}

void FDMenuWidget::RollIn()
{
  m_state = STATE_ROLLED_IN;
  Move( lUIPoint( m_sprite->GetX(), m_sprite->GetY() ) );
  SignalHot( this, -1 );
  ms_bMenuActive = false;
}

void FDMenuWidget::ChangeButtonText( int index, lSharedPtr<lSprite> textOnSprite, lSharedPtr<lSprite> textOffSprite )
{
  m_buttonList( index ).textOn  = textOnSprite;
  m_buttonList( index ).textOff = textOffSprite;
}
