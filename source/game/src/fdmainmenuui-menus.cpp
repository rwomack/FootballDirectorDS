//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdmainmenuui.cpp
// Description : FDMainMenuUI implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifdef L_PLATFORM_NDS
//NOTE: Increase optimiser memory usage limit
#pragma opt_defuse_mem_limit 180
#endif

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
//#include <fdscript.h>
#include <fdgameworld.h>
#include <fdtext.h>
#include <fdcommandlineoptions.h>

#include <screens/fdsquadscreen.h>
#include <screens/fdnewsscreen.h>
#include <screens/fdcupfixturesscreen.h>
#include <screens/fdloadscreen.h>

#include <fdtextwidget.h>
#include <fdmenuwidget.h>
#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdbuttonrolloutwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdsliderwidget.h>
#include <fdtablewidget.h>
#include <fdplayerblobwidget.h>
#include <fdfullscreenwidget.h>
#include <fdscrollbarwidget.h>


#include <playersearch.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdeditmodedata.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

#include <lemon/math/lmrand.h>

extern void ApplySaveDataChangesToWorldData(FDEditModeData &cData);

extern CWorld *s_WorldData;
extern bool    s_bWorldDataUsed;
extern FDEditModeData s_EditModeData;


struct MenuIcon
{
  const char * iconName;
  ScreenComboType screenIdx;
  bool glowing;
};

const char * s_clubMenuPrefix = "menu_club";
static MenuIcon s_clubIcons[] =
  {
      { "news",          SCREENCOMBO_NEWSSTATUS,    false },
      { "squad",         SCREENCOMBO_SQUAD_CLUB,         false },
      { "tactics",       SCREENCOMBO_TACTICS,       false },
      { "training",      SCREENCOMBO_TRAINING,      false },
      { "fixtures",      SCREENCOMBO_CLUBFIXTURES,  false }
  };


const char * s_compMenuPrefix = "menu_comp";
static MenuIcon s_compIcons[] =
  {
      { "league_fixtures",       SCREENCOMBO_LEAGUEFIXTURES, false },
      { "league_tables",         SCREENCOMBO_LEAGUETABLE,    false },
      { "cup_fixtures",          SCREENCOMBO_CUPFIXTURES,    false },
      { "cup_schedules",         SCREENCOMBO_CUPSCHEDULE,    false },
      { "leading_players",       SCREENCOMBO_LEADINGPLAYERS, false },
      { "motm",                  SCREENCOMBO_MOTM,           false },
  };

const char * s_adminMenuPrefix = "menu_admin";
static MenuIcon s_adminIcons[] =
  {
      { "transfer_list",    SCREENCOMBO_TRANSFERLIST,   false },
      { "records",          SCREENCOMBO_CLUBRECORDS,    false },
      { "accounts",         SCREENCOMBO_ACCOUNTS,       false },
      { "wages",            SCREENCOMBO_WAGESSCREEN,    false },
      { "job_centre",       SCREENCOMBO_JOBCENTRE,      false },
      { "youth_squad",      SCREENCOMBO_YOUTHSQUAD,     false },
  };


const char * s_gameMenuPrefix = "menu_game";
static MenuIcon s_gameIcons[] =
  {
      { "save",              SCREENCOMBO_SAVE,           false },
      { "options",           SCREENCOMBO_OPTIONS_IN_GAME,false },
      { "back",              SCREENCOMBO_MAINMENU,       false },
  };

static void AddMenuButton( FDMenuWidget *menu, const char *spritePrefix, const char *spriteName )
{
  char tmp[256];

  sprintf( tmp, "%s_btn-on", spritePrefix);
  lSharedPtr<lSprite> buttonOn = FDResourceManager::GetSprite("uimenu", tmp);
  sprintf( tmp, "%s_btn-off", spritePrefix);
  lSharedPtr<lSprite> buttonOff = FDResourceManager::GetSprite("uimenu", tmp);

  sprintf( tmp, "%s_%s_desc-on", spritePrefix, spriteName);
  lSharedPtr<lSprite> textOn = FDResourceManager::GetSprite("uimenu", tmp);
  sprintf( tmp, "%s_%s_desc-off", spritePrefix, spriteName);
  lSharedPtr<lSprite> textOff = FDResourceManager::GetSprite("uimenu", tmp);

  sprintf( tmp, "%s_%s_icon", spritePrefix, spriteName);
  lSharedPtr<lSprite> icon = FDResourceManager::GetSprite("uimenu", tmp);

  sprintf( tmp, "%s_%s_icon_glow", spritePrefix, spriteName);
  lSharedPtr<lSprite> iconGlow = FDResourceManager::GetSprite("uimenu", tmp);

  menu->AddButton( buttonOn, buttonOff, textOn, textOff, icon, iconGlow );
}


void FDMainMenuUI::SetupMenus()
{
  // setup menus

  for(int i = 0; i < sizeof(s_clubIcons) / sizeof(s_clubIcons[0]); i++)
  {
    AddMenuButton( m_clubMenu, s_clubMenuPrefix, s_clubIcons[i].iconName );
  }

  for(int i = 0; i < sizeof(s_compIcons) / sizeof(s_compIcons[0]); i++)
  {
    AddMenuButton( m_leagueMenu, s_compMenuPrefix, s_compIcons[i].iconName );
  }

  for(int i = 0; i < sizeof(s_adminIcons) / sizeof(s_adminIcons[0]); i++)
  {
    AddMenuButton( m_adminMenu, s_adminMenuPrefix, s_adminIcons[i].iconName );
  }

  for(int i = 0; i < sizeof(s_gameIcons) / sizeof(s_gameIcons[0]); i++)
  {
    AddMenuButton( m_gameMenu, s_gameMenuPrefix, s_gameIcons[i].iconName );
  }

  m_clubMenu->SignalHot.Connect( this, &FDMainMenuUI::OnMenuButtonHot );
  m_clubMenu->SignalCommand.Connect( this, &FDMainMenuUI::OnMenuButtonCommand );

  m_leagueMenu->SignalHot.Connect( this, &FDMainMenuUI::OnMenuButtonHot );
  m_leagueMenu->SignalCommand.Connect( this, &FDMainMenuUI::OnMenuButtonCommand );

  m_adminMenu->SignalHot.Connect( this, &FDMainMenuUI::OnMenuButtonHot );
  m_adminMenu->SignalCommand.Connect( this, &FDMainMenuUI::OnMenuButtonCommand );

  m_gameMenu->SignalHot.Connect( this, &FDMainMenuUI::OnMenuButtonHot );
  m_gameMenu->SignalCommand.Connect( this, &FDMainMenuUI::OnMenuButtonCommand );

  m_lastHotMenu = NULL;
}

void FDMainMenuUI::SelectCorrectMenuItem( ScreenComboType screenIdx )
{
  m_clubMenu->SetSelectedIndex( -1 );
  m_leagueMenu->SetSelectedIndex( -1 );
  m_adminMenu->SetSelectedIndex( -1 );
  m_gameMenu->SetSelectedIndex( -1 );


  for(int i = 0; i < sizeof(s_clubIcons) / sizeof(s_clubIcons[0]); i++)
  {
    if( s_clubIcons[i].screenIdx == screenIdx )
    {
      m_clubMenu->SetSelectedIndex( i );
      return;
    }
  }

  for(int i = 0; i < sizeof(s_compIcons) / sizeof(s_compIcons[0]); i++)
  {
    if( s_compIcons[i].screenIdx == screenIdx )
    {
      m_leagueMenu->SetSelectedIndex( i );
      return;
    }
  }

  for(int i = 0; i < sizeof(s_adminIcons) / sizeof(s_adminIcons[0]); i++)
  {
    if( s_adminIcons[i].screenIdx == screenIdx )
    {
      m_adminMenu->SetSelectedIndex( i );
      return;
    }
  }

  for(int i = 0; i < sizeof(s_gameIcons) / sizeof(s_gameIcons[0]); i++)
  {
    if( s_gameIcons[i].screenIdx == screenIdx )
    {
      m_gameMenu->SetSelectedIndex( i );
      return;
    }
  }
}

void FDMainMenuUI::UnhighlightAll()
{
  for(int i = 0; i < sizeof(s_clubIcons) / sizeof(s_clubIcons[0]); i++)
  {
    m_clubMenu->ChangeButton( i, false );
  }

  for(int i = 0; i < sizeof(s_compIcons) / sizeof(s_compIcons[0]); i++)
  {
    m_leagueMenu->ChangeButton( i, false );
  }
}


bool FDMainMenuUI::IsAnyMenuActive() const
{
  //NOTE: Also returns true if tutorial open
  return m_clubMenu->IsRolledOrRollingOut() ||
         m_leagueMenu->IsRolledOrRollingOut() ||
         m_adminMenu->IsRolledOrRollingOut() ||
         m_gameMenu->IsRolledOrRollingOut() ||
         m_bTutorialActive;
}

//
// Show/Hide the rollout
void FDMainMenuUI::ShowMenuMenu( bool b )
{
  if( b )
  {
    m_clubMenu->Show();
    m_leagueMenu->Show();
    m_adminMenu->Show();
    m_gameMenu->Show();
    m_clock->Show();
	m_clubMenuBack->Show();
	m_leagueMenuBack->Show();
	m_adminMenuBack->Show();
	m_gameMenuBack->Show();
  } else
  {
    m_clubMenu->Hide();
    m_leagueMenu->Hide();
    m_adminMenu->Hide();
    m_gameMenu->Hide();
    m_clock->Hide();
	m_clubMenuBack->Hide();
	m_leagueMenuBack->Hide();
	m_adminMenuBack->Hide();
	m_gameMenuBack->Hide();
  }
}

//
// Main menu controls
void FDMainMenuUI::OnMenuButtonHot( FDMenuWidget *menu, int index )
{
  lLogFmt("MenuButtonHot %d\n", index);

  // will receive -2 for when menu activated
  // will receive -1 for when inactive
  // otherwise receive index of one clicked

  if( m_lastHotMenu != menu )
  {
    if( m_lastHotMenu != NULL )
    {
      m_lastHotMenu->RollIn();
    }
  }

  if( index == -1 )
  {
    m_lastHotMenu = NULL;
    m_fade->Hide();
    m_fadeSub->Hide();
  } else
  {
    m_lastHotMenu = menu;
    m_fade->Show();
    m_fadeSub->Show();
  }
}

//
// Main menu buttons
void FDMainMenuUI::OnMenuButtonCommand( FDMenuWidget *rollout, int index )
{
  lLogFmt("MenuButtonCommand %d\n", index);

  bool bClick = false;

  // Club
  if( rollout == m_clubMenu )
  {
    int numMenuOptions = sizeof(s_clubIcons) / sizeof(s_clubIcons[0]);

    if( index >= 0 && index < numMenuOptions )
    {
      // Glowing?
      if( s_clubIcons[index].glowing )
      {
        s_clubIcons[index].glowing = false;
        m_clubMenu->ChangeButton( index, false );
        //m_fHighlightTimer = 0.0f;
      }

      bClick = true;

      m_leagueMenu->SetSelectedIndex( -1 );
      m_adminMenu->SetSelectedIndex( -1 );
      m_gameMenu->SetSelectedIndex( -1 );

      SwitchToScreenCombo( s_clubIcons[index].screenIdx, false );
    }
  }

  // League
  if( rollout == m_leagueMenu )
  {
    int numMenuOptions = sizeof(s_compIcons) / sizeof(s_compIcons[0]);

    if( index >= 0 && index < numMenuOptions )
    {
      // Glowing?
      if( s_compIcons[index].glowing )
      {
        s_compIcons[index].glowing = false;
        m_leagueMenu->ChangeButton( index, false );
        //m_fHighlightTimer = 0.0f;
      }

      bClick = true;

      m_clubMenu->SetSelectedIndex( -1 );
      m_adminMenu->SetSelectedIndex( -1 );
      m_gameMenu->SetSelectedIndex( -1 );

      SwitchToScreenCombo( s_compIcons[index].screenIdx, false );
    }
  }

  // Admin
  if( rollout == m_adminMenu )
  {
    int numMenuOptions = sizeof(s_adminIcons) / sizeof(s_adminIcons[0]);

    if( index >= 0 && index < numMenuOptions )
    {
      bClick = true;

      m_clubMenu->SetSelectedIndex( -1 );
      m_leagueMenu->SetSelectedIndex( -1 );
      m_gameMenu->SetSelectedIndex( -1 );

      SwitchToScreenCombo( s_adminIcons[index].screenIdx, false );
    }
  }

  // Game
  if( rollout == m_gameMenu )
  {
    int numMenuOptions = sizeof(s_gameIcons) / sizeof(s_gameIcons[0]);

    if( index >= 0 && index < numMenuOptions )
    {
      bClick = true;

	  if (s_gameIcons[index].screenIdx == SCREENCOMBO_MAINMENU)
	  {
		  // Pop up a confirmation dialog first.
      if (UserMessageBox(IDS_GAME_MENU_QUIT_TO_MAIN_MENU_CONFIRMATION_DIALOG, MB_YESNO) == IDNO)
			  return;
	  }

	  if (s_gameIcons[index].screenIdx == SCREENCOMBO_SAVE)
	  {
		  // Pop up a confirmation dialog first.
		  // TODO LOCALE
		  if (UserMessageBox("Saving will overwrite your previous save point. Are you sure?", MB_YESNO) == IDNO)
			  return;
	  }

      m_clubMenu->SetSelectedIndex( -1 );
      m_leagueMenu->SetSelectedIndex( -1 );
      m_adminMenu->SetSelectedIndex( -1 );

      SwitchToScreenCombo( s_gameIcons[index].screenIdx, false );
    }
  }

  if( bClick )
  {
    //FDSound::PlaySoundEffect( FD_SFX_UI_SIDE_BUTTON );
  }
}

void FDMainMenuUI::OnMenuHelp( lUIControlWidget * )
{
  // disable links
  FDTextEnabled( false );
  m_link->SetSprite(FDResourceManager::GetSprite("uiicon","link_off"));

  if( m_lastHotMenu != NULL )
  {
    m_lastHotMenu->RollIn();
    m_lastHotMenu = NULL;
  }


 if( m_bTutorialActive )
  {
    DisableTutorial();
    FDSound::PlaySoundEffect( FD_SFX_UI_BACK_BUTTON );
  } else
  {
    EnableTutorial( GetScreenCombo() );
    FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );
  }
//  if ((m_ScreenStack.GetSize() > 0) && (m_ScreenStack[m_ScreenStack.GetSize() - 1].m_bLinkVisible))
//    m_link->Show();
}

void FDMainMenuUI::OnMenuLink( lUIControlWidget * )
{
  // En/Disable links
  if( FDTextIsEnabled() || FDTextIsEnabledOverride() )
  {
    FDTextEnabled( false );
    m_link->SetSprite(FDResourceManager::GetSprite("uiicon","link_off"));
  } else
  {
    FDTextEnabled( true );
    m_link->SetSprite(FDResourceManager::GetSprite("uiicon","link"));
  }
}

void FDMainMenuUI::OnMenuBack( lUIControlWidget * )
{
  if( !m_back->IsEnabled() )
  {
    return;
  }

  if( m_lastHotMenu != NULL )
  {
    m_lastHotMenu->RollIn();
    m_lastHotMenu = NULL;
  }

  PopScreen();
}

void FDMainMenuUI::OnMainMenuNewButton( lUIControlWidget * )
{
  lLogFmt("New game\n");

  FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );

  ForceUpdateForAllScreensOnNextShow();

  m_bGameOver = false;
  m_newsScreen->ResetTeleprinterDate();

  // if game exists throw up a dialog box yes no?
  bool bStartingNewGame = false;
  if( GamedataExist() )
  {
    lLogFmt("Save data found\n");
    CString strText;
    strText.Format(IDS_GAME_EXISTS);
    if(UserMessageBox(strText, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDYES )
    {
	  bStartingNewGame = true;
    }
  } else
  {
    lLogFmt("No save data found\n");

	bStartingNewGame = true;
  }

  if (bStartingNewGame == true)
  {
	  // Ask the user if they wish to use tutorials during the game.
	  ResetTutorialData();

	  // TODO LOCALE
      CString strText = "Do you want to enable tutorials?";
	  if (UserMessageBox(strText, MB_YESNO) == IDYES)
	  {
		  m_saveData.m_userPrefs.m_bTutorialEnabled = true;
	  }
	  else
	  {
		  m_saveData.m_userPrefs.m_bTutorialEnabled = false;
	  }

	  OptionsSave();
      SwitchToScreenCombo( SCREENCOMBO_NEWGAME, false );
  }

  // set news status as selected menu item
  m_clubMenu->SetSelectedIndex( 0 );
  m_leagueMenu->SetSelectedIndex( -1 );
  m_adminMenu->SetSelectedIndex( -1 );
  m_gameMenu->SetSelectedIndex( -1 );
}

void FDMainMenuUI::OnMainMenuContinueButton( lUIControlWidget *button )
{
  if( !button->IsEnabled() )
  {
    FDSound::PlaySoundEffect( FD_SFX_UI_DISABLE );
    return;
  }

  if( !GamedataExist() )
  {
    lLogFmt("Button not disabled?\n");
    FDSound::PlaySoundEffect( FD_SFX_UI_DISABLE );
    return;
  }

  m_bGameOver = false;

  FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );
  ForceUpdateForAllScreensOnNextShow();

  lLogFmt("Continue game\n");

  // Show game load screen
  ShowProcessScreen( true, PROCESS_LOAD );

  // fade music (we need all space for gamedata load)
  FDSound::FadeMusicWait();

  // Reinit current world data if needed
  ReinitWorldData(false);

  // set world data as used (even if load fails we're potentially overwriting it)
  s_bWorldDataUsed = true;

  // Would load game normally
  if( !GamedataLoad() )
  {
    // load fail - disable button
    lLogFmt("Load failed\n");
    m_contgameButton->Disable();
    m_bReadyToTick = true;
    SwitchToScreenComboInternal( SCREENCOMBO_MAINMENU, false );
    return;
  } else
  {
    lLogFmt("Load success\n");
  }

  m_newsScreen->ResetTeleprinterDate();

//  // HACK: select random team
//  WorldData().OnInitialiseUser( 21 );
//
//  // welcome human manager
//  WorldData().GetCurrentUserClub()->DoHumanManagerWelcome();
//
//  // skip to end of week
//  WorldData().GetCountryList().DoEndOfWeek();
//

  ApplySaveDataChangesToWorldData(s_EditModeData);

  // mark selected team
  m_squadScreen->SetSquadInfo( WorldData().GetCurrentUserClub() );

  // set news status as selected menu item
  m_clubMenu->SetSelectedIndex( 0 );
  m_leagueMenu->SetSelectedIndex( -1 );
  m_adminMenu->SetSelectedIndex( -1 );
  m_gameMenu->SetSelectedIndex( -1 );

  // Hide game load screen
  ShowProcessScreen( false, PROCESS_LOAD );

  // switch to news / status screen
  ClearScreenStack();
  SwitchToScreenCombo( SCREENCOMBO_NEWSSTATUS, false );
}

void FDMainMenuUI::OnMainMenuOptionsButton( lUIControlWidget * )
{
  lLogFmt("Options\n");
  FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );

  SwitchToScreenCombo( SCREENCOMBO_OPTIONS, false );

  FDSound::FadeMusicWait();
}

void FDMainMenuUI::OnMainMenuEditModeButton(lUIControlWidget *)
{
	lLogFmt("Edit Mode\n");


	FDSound::PlaySoundEffect(FD_SFX_UI_CLICK);

	// If there is existing game data, warn the user that the changes will only be applicable
	// to the new game.
	if (GamedataExist() == true)
	{
    if (UserMessageBox(IDS_EDIT_MODE_ONLY_AFFECTS_NEW_GAME_DIALOG, MB_YESNO) == IDNO)
			return;
	}

	// If the world data has been used, then ask the user if they
	// are sure that they want to enter edit mode.
	if (s_bWorldDataUsed)
	{
		// Show game load screen
		ShowProcessScreen( true, PROCESS_LOAD );

		ReinitWorldData(false);

		// Hide game load
		ShowProcessScreen( false, PROCESS_LOAD );
	}

	SwitchToScreenCombo(SCREENCOMBO_EDITMODE, false);
}

void FDMainMenuUI::HighlightMenuIcon( int index, int subIndex )
{
  switch( index )
  {
    //NOTE: Menu highlighting disabled
    //case HIGHLIGHT_NEWS:
    //{
    //  m_clubMenu->ChangeButton( 0, true );
    //  s_clubIcons[ 0 ].glowing = true;
    //  break;
    //}

    case HIGHLIGHT_SQUAD:
    {
      m_clubMenu->ChangeButton( 1, true );
      s_clubIcons[ 1 ].glowing = true;
      break;
    }

    case HIGHLIGHT_FIXTURES:
    {
      m_clubMenu->ChangeButton( 4, true );
      s_clubIcons[ 4 ].glowing = true;
      break;
    }

    case HIGHLIGHT_CUP_FIXTURES:
    {
      m_leagueMenu->ChangeButton( 2, true );
      s_compIcons[ 2 ].glowing = true;

      if( subIndex != -1 )
      {
        m_cupFixturesScreen->SetInitialCup( subIndex );
      }

      break;
    }

    default:
      break;
  }

  m_fHighlightTimer = 1.0f;
}


void FDMainMenuUI::TickMenuHighlight( float delta )
{
#ifdef L_PLATFORM_NDS
  if( delta < 0.01f ) delta = 0.05f;
  if( delta > 0.1f )  delta = 0.1f;
#endif

  if( m_fHighlightTimer > 0.0f )
  {
    m_fHighlightTimer -= delta;
    if( m_fHighlightTimer <= 0.0f )
    {
      m_fHighlightTimer += 0.5f;

      if( m_bGlow )
      {
        for( int i = 0; i < 5; i++ )
        {
          if( s_clubIcons[ i ].glowing ) m_clubMenu->ChangeButton( i, false );
        }

        for( int i = 0; i < 6; i++ )
        {
          if( s_compIcons[ i ].glowing ) m_leagueMenu->ChangeButton( i, false );
        }

        m_bGlow = false;
      } else
      {
        for( int i = 0; i < 5; i++ )
        {
          if( s_clubIcons[ i ].glowing ) m_clubMenu->ChangeButton( i, true );
        }

        for( int i = 0; i < 6; i++ )
        {
          if( s_compIcons[ i ].glowing ) m_leagueMenu->ChangeButton( i, true );
        }
        m_bGlow = true;
      }
    }
  }
}
