//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDOptions.cpp
// Description : FDOptions implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdoptions.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDOptions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//////////////////////////////////////////////////////////////////////////
// Constructor.
//////////////////////////////////////////////////////////////////////////
FDOptions::FDOptions( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

//////////////////////////////////////////////////////////////////////////
// Destructor.
//////////////////////////////////////////////////////////////////////////
FDOptions::~FDOptions()
{
}

//////////////////////////////////////////////////////////////////////////
// Initialization function called by the constructor.
//////////////////////////////////////////////////////////////////////////
void FDOptions::Init()
{
  // Bottom screen
  m_optionsScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "optionsscreen" );
  m_optionsScreen->Hide();

  m_acceptButton = FD_GAME_NEW FDButtonWidget( m_optionsScreen, 0, "accept_button", FDResourceManager::GetSprite("ui","button55"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_CENTER, lUIPoint( 98, 166 ) );
  m_acceptButton->SetText( lUIText( "ACCEPT" ) );
  m_acceptButton->SignalControlCommand.Connect( this, &FDOptions::OnAccept );

  m_cancelButton = FD_GAME_NEW FDButtonWidget( m_optionsScreen, 0, "cancel_button", FDResourceManager::GetSprite("ui","button55"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_CENTER, lUIPoint( 178, 166 ) );
  m_cancelButton->SetText( lUIText( "CANCEL" ) );
  m_cancelButton->SignalControlCommand.Connect( this, &FDOptions::OnCancel );

  // Top screen
  m_optionsScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "optionsscreen_sub" );
  m_optionsScreenSub->Hide();

  FDBackingWidget * backing = FD_GAME_NEW FDBackingWidget( m_optionsScreen, 0, "back", L_ARGB( 80, 45, 95, 85 ), LUI_LEFT_TOP, lUIPoint(20, 20 ), lUIPoint( 236, 135 ) );
  backing->SetLines( 3, BLACK );

  lUISpriteWidget * spriteBar[3];

  for( int i = 0; i < 3; i++ )
  {
    spriteBar[i] = FD_GAME_NEW lUISpriteWidget( m_optionsScreen, 0, "bars", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT, lUIPoint(20, 25 + i * 30 ), lUIPoint( 236, 12 ));
  }

  // General options
  m_matchSpeedSlider = FD_GAME_NEW FDSliderWidget( m_optionsScreen, 0, "matchspeed_slider", 0, lUIText( IDS_MATCHSPEED), LUI_CENTER_CENTER, lUIPoint( 138, 45 ) );
  m_matchSpeedSlider->SignalSliderChange.Connect( this, &FDOptions::OnMatchSpeedChange );
  m_matchSpeedSlider->SetValueRange(1.0f, 5.0f);
  m_matchSpeedSlider->SetValueInc(1.0f);
  m_matchSpeedSlider->SetSliderFlags( FDSliderWidget::SLIDER_FLAG_X );
  m_matchSpeedSlider->SetTextToUpper();

  // Sound options
  m_soundVolumeSlider = FD_GAME_NEW FDSliderWidget( m_optionsScreen, 0, "soundvolume_slider", 0, lUIText(IDS_SOUNDVOLUME), LUI_CENTER_CENTER, lUIPoint( 138, 75 ) );
  m_soundVolumeSlider->SignalSliderChange.Connect( this, &FDOptions::OnSoundVolumeChange );
  m_soundVolumeSlider->SetValueInc(0.1f);
  m_soundVolumeSlider->SetTextToUpper();

  m_crowdVolumeSlider = FD_GAME_NEW FDSliderWidget( m_optionsScreen, 0, "crowdvolume_slider", 0, lUIText(IDS_CROWDVOLUME), LUI_CENTER_CENTER, lUIPoint( 138, 105 ) );
  m_crowdVolumeSlider->SignalSliderChange.Connect(this, &FDOptions::OnCrowdVolumeChange);
  m_crowdVolumeSlider->SetValueInc(0.1f);
  m_crowdVolumeSlider->SetTextToUpper();

  m_inMatchAudioButton = FD_GAME_NEW FDCheckBoxWidget( m_optionsScreen, 0, "inmatchaudio_check", FDResourceManager::GetSprite("uiicon","tick_0"), FDResourceManager::GetSprite("uiicon","tick_1"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 90, 125));
  m_inMatchAudioButton->SetText( lUIText( IDS_INMATCH_SOUND ) );
  m_inMatchAudioButton->SetChecked( false );
  m_inMatchAudioButton->SignalControlCommand.Connect( this, &FDOptions::OnCheckInMatchAudio );

  m_tutorialsButton = FD_GAME_NEW FDCheckBoxWidget( m_optionsScreen, 0, "tutorials_check", FDResourceManager::GetSprite("uiicon","tick_0"), FDResourceManager::GetSprite("uiicon","tick_1"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 90, 140));
  m_tutorialsButton->SetText( lUIText( "Tutorials" ) );
  m_tutorialsButton->SetChecked( false );
  m_tutorialsButton->SignalControlCommand.Connect( this, &FDOptions::OnCheckTutorials );

  m_bInGame = false;
  m_bComingFromDialog = false;
}

//////////////////////////////////////////////////////////////////////////
// Called when the options screen is shown.
//////////////////////////////////////////////////////////////////////////
void FDOptions::DoShow()
{
  m_optionsScreen->Show();
  m_optionsScreenSub->Show();
}

//////////////////////////////////////////////////////////////////////////
// Called when the options screen is hidden.
//////////////////////////////////////////////////////////////////////////
void FDOptions::DoHide()
{
  m_optionsScreen->Hide();
  m_optionsScreenSub->Hide();
}

//////////////////////////////////////////////////////////////////////////
// Called by the UI to prepare the screen for viewing.
//////////////////////////////////////////////////////////////////////////
void FDOptions::Update()
{
	if (m_bComingFromDialog == true)
		return;

	m_cPrefs = GetMainUI()->GetSaveData().m_userPrefs;

  m_matchSpeedSlider->SetValue(m_cPrefs.m_matchSpeed, true);
	m_soundVolumeSlider->SetValue(m_cPrefs.m_sfxVolume, true);
	m_crowdVolumeSlider->SetValue(m_cPrefs.m_crowdVolume, true);
  m_inMatchAudioButton->SetChecked(m_cPrefs.m_bInMatchSFX);
  m_tutorialsButton->SetChecked(m_cPrefs.m_bTutorialEnabled);
}

//////////////////////////////////////////////////////////////////////////
// Sets a flag that alters the layout of the screen. Meant to be used when
// the options menu is accessed in-game.
//////////////////////////////////////////////////////////////////////////
void FDOptions::InGame()
{
  m_bInGame = true;
}

//////////////////////////////////////////////////////////////////////////
// Handler called when the Accept button is pressed.
//////////////////////////////////////////////////////////////////////////
void FDOptions::OnAccept( lUIControlWidget * )
{
  m_bInGame = false;

  // Apply changes to saved options data, and apply the changes to the current
  // settings.
  GetMainUI()->GetSaveData().m_userPrefs = m_cPrefs;
  GetMainUI()->OptionsApply();

  // Save options data.
  GetMainUI()->OptionsSave();

  // Return to previous screen
  GetMainUI()->PopScreen();
}

//////////////////////////////////////////////////////////////////////////
// Handler called when the Cancel button is pressed.
//////////////////////////////////////////////////////////////////////////
void FDOptions::OnCancel( lUIControlWidget * )
{
  // check if preferences have changed
  if( GetMainUI()->GetSaveData().m_userPrefs != m_cPrefs )
	{
		// Bring up confirmation dialog.
		m_bComingFromDialog = true;

    if (GetMainUI()->UserMessageBox(IDS_OPTIONS_MENU_CANCEL_CONFIRMATION, MB_YESNO) == IDNO)
		{
      // Reapply sound settings.
      GetMainUI()->OptionsApply();
			m_bComingFromDialog = false;
			return;
		}

		m_bComingFromDialog = false;
	}

	m_bInGame = false;

	// Reapply sound settings.
	GetMainUI()->OptionsApply();

	// Return to previous screen
	GetMainUI()->PopScreen();
}

//////////////////////////////////////////////////////////////////////////
// Handler called when the match speed slider is moved.
//////////////////////////////////////////////////////////////////////////
void FDOptions::OnMatchSpeedChange(lUIWidget *pSlider, float fValue)
{
	m_cPrefs.m_matchSpeed = (byte)fValue;
  if( m_cPrefs.m_matchSpeed < 1 ) m_cPrefs.m_matchSpeed = 1;
  if( m_cPrefs.m_matchSpeed > 5 ) m_cPrefs.m_matchSpeed = 5;
}

//////////////////////////////////////////////////////////////////////////
// Handler called when the sound volume slider is moved.
//////////////////////////////////////////////////////////////////////////
void FDOptions::OnSoundVolumeChange(lUIWidget *pSlider, float fValue)
{
	m_cPrefs.m_sfxVolume = fValue;

	// Applies the current options to the game.
	FDSound::SetSoundEffectVolume((int)(m_cPrefs.m_sfxVolume * 5.0f));
}

//////////////////////////////////////////////////////////////////////////
// Handler called when the crowd volume slider is moved.
//////////////////////////////////////////////////////////////////////////
void FDOptions::OnCrowdVolumeChange(lUIWidget *pSlider, float fValue)
{
	m_cPrefs.m_crowdVolume = fValue;
}

//////////////////////////////////////////////////////////////////////////
// Handler called when the "in-match audio" checkbox is changed.
//////////////////////////////////////////////////////////////////////////
void FDOptions::OnCheckInMatchAudio(lUIControlWidget *pCheckbox)
{
	FDCheckBoxWidget *pWidget = (FDCheckBoxWidget *)pCheckbox;
	m_cPrefs.m_bInMatchSFX = pWidget->IsChecked();
}

//////////////////////////////////////////////////////////////////////////
// Handler called when the "tutorials" checkbox is changed.
//////////////////////////////////////////////////////////////////////////
void FDOptions::OnCheckTutorials(lUIControlWidget *pCheckbox)
{
	FDCheckBoxWidget *pWidget = (FDCheckBoxWidget *)pCheckbox;
  m_cPrefs.m_bTutorialEnabled = pWidget->IsChecked();
}


