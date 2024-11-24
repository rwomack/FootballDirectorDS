//굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
// File        : fdmainmenuui-screens.cpp
// Description : FDMainMenuUI implementation
// Notes       :
//
//굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇굇
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifdef L_PLATFORM_NDS
//NOTE: Increase optimiser memory usage limit
#pragma opt_defuse_mem_limit 180
#endif

//#define AUTOLOAD
//#define TIMING

#define MAX_SCREENSTACK_SIZE 50
//#define SCREENSWITCH_DELAY

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
//#include <fdscript.h>
#include <fdgameworld.h>
#include <fdtext.h>
#include <fdcommandlineoptions.h>

#include <fdscreen.h>
#include <screens/fdsquadscreen.h>
#include <screens/fdaccountsscreen.h>
#include <screens/fdclubfixturesscreen.h>
#include <screens/fdcupfixturesscreen.h>
#include <screens/fdcupschedulescreen.h>
#include <screens/fdtacticsscreen.h>
#include <screens/fdtrainingscreen.h>
#include <screens/fdmatchdayscreen.h>
#include <screens/fdnewsscreen.h>
#include <screens/fdinfoscreen.h>
#include <screens/fdstatusscreen.h>
#include <screens/fdjobcentrescreen.h>
#include <screens/fdtransferlistscreen.h>
#include <screens/fdplayerinfoscreen.h>
#include <screens/fdplayerinfosubscreen.h>
#include <screens/fdemployeeinfoscreen.h>
#include <screens/fdyouthinfoscreen.h>
#include <screens/fdyouthsquadscreen.h>
#include <screens/fdnewgamescreen.h>
#include <screens/fdselectteamscreen.h>
#include <screens/fdloadscreen.h>
#include <screens/fdsavescreen.h>
#include <screens/fdprogressscreen.h>
#include <screens/fdleaguefixturesscreen.h>
#include <screens/fdmatchintroscreen.h>
#include <screens/fdnegotiatescreen.h>
#include <screens/fdsackplayerscreen.h>
#include <screens/fdmatchsummaryscreen.h>
#include <screens/fdteamoftheweekscreen.h>
#include <screens/fdleaguetablescreen.h>
#include <screens/fdliveresultscreen.h>
#include <screens/fdliveleaguetablescreen.h>
#include <screens/fdtransferstatusscreen.h>
#include <screens/fdwagesscreen.h>
#include <screens/fdarrangefriendliesscreen.h>
#include <screens/fdmanagerinfoscreen.h>
#include <screens/fdoptions.h>
#include <screens/fdmonthlyawardscreen.h>

#include <screens/fdclubrecordsscreen.h>
#include <screens/fdleadingplayerscreen.h>
#include <screens/fdeditmodescreen.h>
#include <screens/fdeditnamescreen.h>
#include <screens/fdstrategyformationscreen.h>
#include <screens/fdstrategyrolesscreen.h>

#include <screens/fdclubinfosubscreen.h>

#include <playersearch.h>

#include <lconfig.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

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
#include <fdhighlightwidget.h>
#include <fdscrollbarwidget.h>

#include <fdeditmodedata.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

#include <lemon/math/lmrand.h>

using namespace L_NAMESPACE_NAME;

extern bool DoMainLoop();

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// SetLayout - setup graphical elements for a particular screen combo
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void FDMainMenuUI::SetLayout( bool bMainScreen, bool bSubScreen, ScreenComboType screenIdx )
{
  // set top and bottom backgrounds based on screen index
  // NOTE: Special case main menu etc
  // NOTE: Events screen needs to be able to get a widget pointer before background so it can make
  //       its bars elements of this
  bool bNewLayout = false;
  if( screenIdx != m_currentLayoutScreenIdx )
  {
    bNewLayout = true;
    m_currentLayoutScreenIdx = screenIdx;
  }

  m_mainScreenBg->Hide();
  m_mainScreenSubBg->Hide();
  m_subScreenTitleBar->Hide();
  m_subScreenTitleText->Hide();
  m_subScreenSpecial->Hide();

  m_subScreenCalendar->Hide();
  m_subScreenCalendarYear->Hide();
  m_subScreenCalendarMonth->Hide();
  m_subScreenCalendarDay->Hide();

  const char *bar      = NULL;

  bool bCalendar = true;

  // look up bar type
  switch( screenIdx )
  {
    default:
    case SCREENCOMBO_SQUAD:
    case SCREENCOMBO_SQUAD_CLUB:
    case SCREENCOMBO_NEWSSTATUS:
    case SCREENCOMBO_TRAINING:
    case SCREENCOMBO_CLUBFIXTURES:
    case SCREENCOMBO_TACTICS:
    case SCREENCOMBO_ARRANGEFRIENDLIES:
    case SCREENCOMBO_PLAYERPROFILE:
    //case SCREENCOMBO_YOUTHPROFILE:
    //case SCREENCOMBO_EMPLOYEEPROFILE:
    //case SCREENCOMBO_MANAGERPROFILE:
    case SCREENCOMBO_TRANSFERSTATUS:
    case SCREENCOMBO_STRATEGY_FORMATION:
    case SCREENCOMBO_STRATEGY_ROLES:
    case SCREENCOMBO_INFO:
      bar = "red_bar";
      break;

    case SCREENCOMBO_CUPFIXTURES:
    case SCREENCOMBO_CUPSCHEDULE:
    case SCREENCOMBO_LEAGUEFIXTURES:
    case SCREENCOMBO_LEAGUETABLE:
    case SCREENCOMBO_LEADINGPLAYERS:
    case SCREENCOMBO_MOTM:
      bar = "blue_bar";
      break;

    case SCREENCOMBO_TRANSFERLIST:
    case SCREENCOMBO_TRANSFERLIST_EDIT:
    case SCREENCOMBO_ACCOUNTS:
    case SCREENCOMBO_JOBCENTRE:
    case SCREENCOMBO_WAGESSCREEN:
    case SCREENCOMBO_YOUTHSQUAD:
    case SCREENCOMBO_NEGOTIATION:
    case SCREENCOMBO_SACKPLAYER:
    case SCREENCOMBO_CLUBRECORDS:
      bar = "green_bar";
      break;

    case SCREENCOMBO_MATCH_INTRO:
    case SCREENCOMBO_MATCH_SUMMARY:
    case SCREENCOMBO_MATCH:
    case SCREENCOMBO_TEAMOFTHEWEEK:
      bar = "purple_bar";
      break;

    case SCREENCOMBO_MESSAGEBOX_OK:
    case SCREENCOMBO_MESSAGEBOX_YESNO:
    case SCREENCOMBO_SAVE:
    case SCREENCOMBO_SAVE_EDITMODE:
    case SCREENCOMBO_SAVEDONE:
    case SCREENCOMBO_MAINMENU:
    case SCREENCOMBO_PROGRESS:
    case SCREENCOMBO_NEWGAME:
    case SCREENCOMBO_NEWGAME_SELECTTEAM:
    case SCREENCOMBO_SELECTNEWTEAM:
    case SCREENCOMBO_EDITMODE:
    case SCREENCOMBO_EDITNAME:
    case SCREENCOMBO_LANGUAGE:
    case SCREENCOMBO_OPTIONS:
    case SCREENCOMBO_OPTIONS_IN_GAME:
      bar = "yellow_bar";
      break;
  }

  // look up background
  const char *topBg    = "background1";

  switch( screenIdx )
  {
    default:
    case SCREENCOMBO_SQUAD:
    case SCREENCOMBO_SQUAD_CLUB:
    case SCREENCOMBO_NEWSSTATUS:
    case SCREENCOMBO_TRAINING:
    case SCREENCOMBO_CLUBFIXTURES:
    case SCREENCOMBO_TACTICS:
    case SCREENCOMBO_ARRANGEFRIENDLIES:
    case SCREENCOMBO_PLAYERPROFILE:
    //case SCREENCOMBO_YOUTHPROFILE:
    //case SCREENCOMBO_EMPLOYEEPROFILE:
    //case SCREENCOMBO_MANAGERPROFILE:
    case SCREENCOMBO_TRANSFERSTATUS:
    case SCREENCOMBO_STRATEGY_FORMATION:
    case SCREENCOMBO_STRATEGY_ROLES:
      topBg = "background4";
      break;

    case SCREENCOMBO_CUPFIXTURES:
    case SCREENCOMBO_LEAGUEFIXTURES:
    case SCREENCOMBO_LEAGUETABLE:
    case SCREENCOMBO_LEADINGPLAYERS:
    case SCREENCOMBO_MOTM:
      topBg = "background2";
      break;

    case SCREENCOMBO_TRANSFERLIST:
    case SCREENCOMBO_TRANSFERLIST_EDIT:
    case SCREENCOMBO_ACCOUNTS:
    case SCREENCOMBO_JOBCENTRE:
    case SCREENCOMBO_WAGESSCREEN:
    case SCREENCOMBO_YOUTHSQUAD:
    case SCREENCOMBO_NEGOTIATION:
    case SCREENCOMBO_SACKPLAYER:
    case SCREENCOMBO_CLUBRECORDS:
      topBg = "background3";
      break;

    case SCREENCOMBO_MATCH_INTRO:
    case SCREENCOMBO_MATCH_SUMMARY:
    case SCREENCOMBO_MATCH:
    case SCREENCOMBO_TEAMOFTHEWEEK:
      topBg = "background5";
      break;

    case SCREENCOMBO_MESSAGEBOX_OK:
    case SCREENCOMBO_MESSAGEBOX_YESNO:
    case SCREENCOMBO_SAVE:
    case SCREENCOMBO_SAVE_EDITMODE:
    case SCREENCOMBO_SAVEDONE:
    case SCREENCOMBO_MAINMENU:
    case SCREENCOMBO_PROGRESS:
    case SCREENCOMBO_NEWGAME:
    case SCREENCOMBO_NEWGAME_SELECTTEAM:
    case SCREENCOMBO_SELECTNEWTEAM:
    case SCREENCOMBO_EDITMODE:
    case SCREENCOMBO_EDITNAME:
    case SCREENCOMBO_LANGUAGE:
    case SCREENCOMBO_OPTIONS:
    case SCREENCOMBO_OPTIONS_IN_GAME:
    case SCREENCOMBO_INFO:
      topBg = "background1";
      break;

    case SCREENCOMBO_CUPSCHEDULE:
      topBg = "cup1";
      break;
  }

  const char *bottomBg = topBg;         // use same background

  switch( screenIdx )
  {
    default:
      break;

    case SCREENCOMBO_SAVE:
    case SCREENCOMBO_SAVE_EDITMODE:
    case SCREENCOMBO_SAVEDONE:
    case SCREENCOMBO_PROGRESS:
      bottomBg = "activity_background";
      break;

    case SCREENCOMBO_INFO:
      bottomBg = "wl_back";
      break;

    case SCREENCOMBO_CUPSCHEDULE:
      bottomBg = "background2";
      break;
  }

  // set title text sprite
  const char *textsprite = NULL;

  switch( screenIdx )
  {
    default: break;
    case SCREENCOMBO_SQUAD_CLUB:
    case SCREENCOMBO_SQUAD:               textsprite = "en_squad";              break;
    case SCREENCOMBO_NEWSSTATUS:          textsprite = "en_newsstatus";         break;
    case SCREENCOMBO_TRAINING:            textsprite = "en_training";           break;
    case SCREENCOMBO_CLUBFIXTURES:        textsprite = "en_clubfixtures";       break;
    case SCREENCOMBO_TACTICS:             textsprite = "en_tactics";            break;
    case SCREENCOMBO_ARRANGEFRIENDLIES:   textsprite = "en_arrangefriendlies";  break;
    case SCREENCOMBO_PLAYERPROFILE:       textsprite = "en_playerprofile";      break;
    //case SCREENCOMBO_YOUTHPROFILE:        textsprite = "en_youthprofile";       break;
    //case SCREENCOMBO_EMPLOYEEPROFILE:     textsprite = "en_employeeprofile";    break;
    //case SCREENCOMBO_MANAGERPROFILE:      textsprite = "en_managerprofile";     break;
    case SCREENCOMBO_TRANSFERSTATUS:      textsprite = "en_transferstatus";     break;
    case SCREENCOMBO_CUPFIXTURES:         textsprite = "en_cupfixtures";        break;
    case SCREENCOMBO_CUPSCHEDULE:         textsprite = "en_cupschedule";        break;
    case SCREENCOMBO_LEAGUEFIXTURES:      textsprite = "en_leaguefixtures";     break;
    case SCREENCOMBO_LEAGUETABLE:         textsprite = "en_leaguetables";       break;
    case SCREENCOMBO_LEADINGPLAYERS:      textsprite = "en_leadingplayers";     break;
    case SCREENCOMBO_TEAMOFTHEWEEK:       textsprite = "en_teamoftheweek";      break;
    case SCREENCOMBO_MOTM:                textsprite = "en_motm";               break;
    case SCREENCOMBO_TRANSFERLIST:        textsprite = "en_transferlist";       break;
    case SCREENCOMBO_TRANSFERLIST_EDIT:   textsprite = "en_transferlist_edit";  break;
    case SCREENCOMBO_ACCOUNTS:            textsprite = "en_accounts";           break;
    case SCREENCOMBO_JOBCENTRE:           textsprite = "en_jobcentre";          break;
    case SCREENCOMBO_WAGESSCREEN:         textsprite = "en_contracts";        break;
    case SCREENCOMBO_YOUTHSQUAD:          textsprite = "en_youthsquad";         break;
    case SCREENCOMBO_NEGOTIATION:         textsprite = "en_negotiation";        break;
    case SCREENCOMBO_SACKPLAYER:          textsprite = "en_sackplayer";         break;
    case SCREENCOMBO_CLUBRECORDS:         textsprite = "en_clubrecords";        break;
    case SCREENCOMBO_MATCH_INTRO:         textsprite = "en_match_intro";        break;
    case SCREENCOMBO_MATCH_SUMMARY:       textsprite = "en_match";      break;
    case SCREENCOMBO_MATCH:               textsprite = "en_match";              break;
    case SCREENCOMBO_SAVE:                textsprite = "en_save";               break;
    case SCREENCOMBO_SAVE_EDITMODE:       textsprite = "en_editmode";               break;
    case SCREENCOMBO_SAVEDONE:            textsprite = "en_save";               break;
    case SCREENCOMBO_NEWGAME:             textsprite = "en_newgame";            break;
    case SCREENCOMBO_NEWGAME_SELECTTEAM:  textsprite = "en_newgame_selectteam"; break;
    case SCREENCOMBO_SELECTNEWTEAM:       textsprite = "en_selectnewteam";            break;
    case SCREENCOMBO_LANGUAGE:            textsprite = "en_language";           break;
    case SCREENCOMBO_OPTIONS:             textsprite = "en_options";            break;
    case SCREENCOMBO_OPTIONS_IN_GAME:     textsprite = "en_options";            break;
    //case SCREENCOMBO_PROGRESS:            textsprite = "en_progress";           break;
    case SCREENCOMBO_EDITMODE:        textsprite = "en_editmode";   break;
    case SCREENCOMBO_EDITNAME:        textsprite = "en_editname";   break;
    case SCREENCOMBO_STRATEGY_FORMATION:  textsprite = "en_formation"; break;
    case SCREENCOMBO_STRATEGY_ROLES:    textsprite = "en_roles"; break;
  }

  //
  // do settings

  switch( screenIdx )
  {
    case SCREENCOMBO_NEWGAME:
    case SCREENCOMBO_NEWGAME_SELECTTEAM:
    case SCREENCOMBO_SELECTNEWTEAM:
    case SCREENCOMBO_LANGUAGE:
    case SCREENCOMBO_OPTIONS_IN_GAME:
    case SCREENCOMBO_MAINMENU:
    case SCREENCOMBO_EDITMODE:
    case SCREENCOMBO_EDITNAME:
    case SCREENCOMBO_STRATEGY_FORMATION:
    case SCREENCOMBO_STRATEGY_ROLES:
    case SCREENCOMBO_INFO:
    {
      bCalendar = false;
      break;
    }

    case SCREENCOMBO_SAVE_EDITMODE:
    case SCREENCOMBO_OPTIONS:
    case SCREENCOMBO_PROGRESS:
    {
      bCalendar = false;
      break;
    }

    case SCREENCOMBO_MATCH:
    case SCREENCOMBO_MATCH_INTRO:
    {
      bCalendar = false;
      break;
    }

    default:
      break;
  }

  switch( screenIdx )
  {
    case SCREENCOMBO_TACTICS:
    case SCREENCOMBO_TACTICS_IN_MATCH:
    {
      m_subScreenSpecial->SetSprite( FDResourceManager::GetSpriteBG("pitch_top") );
      m_subScreenSpecial->Move( lUIPoint(0, 13) );
      m_subScreenSpecial->Show();
      break;
    }

    case SCREENCOMBO_OPTIONS:
    {
      bar = NULL;                       // show no title text
      break;
    }

    case SCREENCOMBO_LOADING:
    {
      bar = NULL;
      bCalendar = false;
      break;
    }

    case SCREENCOMBO_INFO:
    case SCREENCOMBO_MAINMENU:
    case SCREENCOMBO_PROGRESS:
    {
      bar = NULL;                       // show no title text
      topBg = "title";
      break;
    }

    case SCREENCOMBO_MATCH_SUMMARY:
    case SCREENCOMBO_MATCH:
    {
      break;
    }

    default:
    {
      break;
    }
  };

  // calendar
  if( bCalendar )
  {
    m_subScreenCalendarYear->GetSprite().Get()->SetFrame( WorldData().GetCurrentDate().GetYear() - 12 );
    m_subScreenCalendarMonth->GetSprite().Get()->SetFrame( WorldData().GetCurrentDate().GetMonth() - 1 );
    m_subScreenCalendarDay->GetSprite().Get()->SetFrame( WorldData().GetCurrentDate().GetDay() - 1 );

    m_subScreenCalendar->Show();
    m_subScreenCalendarYear->Show();
    m_subScreenCalendarMonth->Show();
    m_subScreenCalendarDay->Show();
  }
  else
  {
	  m_subScreenCalendar->Hide();
	  m_subScreenCalendarYear->Hide();
	  m_subScreenCalendarMonth->Hide();
	  m_subScreenCalendarDay->Hide();
  }

  // set up backgrounds
  if( bMainScreen )
  {
    // set random background
    if( bNewLayout )
    {
      m_mainScreenBg->SetSprite( FDResourceManager::GetSpriteBG( bottomBg ) );
      m_mainScreenBg->GetSprite()->SetTransparent( false ); //HACK: this shouldn't really be needed. should be a flag in the sprite bank.
    }

    m_mainScreenBg->Show();
  }

  if( bSubScreen )
  {
    // set random background
    if( bNewLayout )
    {
      m_mainScreenSubBg->SetSprite( FDResourceManager::GetSpriteBG( topBg ) );
      m_mainScreenSubBg->GetSprite()->SetTransparent( false ); //HACK: this shouldn't really be needed. should be a flag in the sprite bank.
    }

    m_mainScreenSubBg->Show();

    // set title bar
    if( bar != NULL )
    {
      m_subScreenTitleBar->SetSprite( FDResourceManager::GetSprite( bar, bar ) );
      m_subScreenTitleBar->Show();

      if( textsprite != NULL )
      {
        SetTitleText( textsprite );
      } else
      {
        lLogFmt("Error couldn't find text for title text %d\n", screenIdx);
      }
    }
  }
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// SetTitleText - alter the title text for the current screen
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void FDMainMenuUI::SetTitleText( const char * textsprite )
{
  if( textsprite != NULL )
  {
    // position top right in top right corner
    m_subScreenTitleText->SetSprite( FDResourceManager::GetSprite( "en_titles", textsprite ) );

    if( m_subScreenTitleText->GetSprite() != NULL )
    {
      m_subScreenTitleText->Show();
    }
  }
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// SwitchToScreenCombo - switch to the specified screen combo
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void FDMainMenuUI::SwitchToScreenCombo( ScreenComboType screenIdx, const bool bInheritMenuState, bool bStoreLastState, bool bPopping )
{
  lLogBreak();
  lLogFmt("SwitchToScreenCombo  %d\n", screenIdx);

	m_eSwitchingToScreen = screenIdx;
	m_bSwitchingToScreenInheritMenuState = bInheritMenuState;
  m_bSwitchingToScreenStoreLastState     = bStoreLastState;
  m_bSwitchingToScreenPopping            = bPopping;
	m_bSwitchingToScreenModal = false;
#ifndef SCREENSWITCH_DELAY
  SwitchToScreenComboInternal(m_eSwitchingToScreen, m_bSwitchingToScreenInheritMenuState, m_bSwitchingToScreenStoreLastState, m_bSwitchingToScreenPopping);
  m_eSwitchingToScreen = SCREENCOMBO_INVALID;
#endif
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// SwitchToScreenCombo - switch to the specified screen modal
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void FDMainMenuUI::SwitchToScreenModal( ScreenComboType screenIdx, const bool bInheritMenuState )
{
	m_eSwitchingToScreen = screenIdx;
	m_bSwitchingToScreenInheritMenuState = bInheritMenuState;
	m_bSwitchingToScreenModal = true;

#ifndef SCREENSWITCH_DELAY
  SwitchToScreenModalInternal(m_eSwitchingToScreen, m_bSwitchingToScreenInheritMenuState);
  m_eSwitchingToScreen = SCREENCOMBO_INVALID;
#endif
}

//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
// SwitchToScreenComboInternal - Do actual screen init / display work
//
//같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같
void FDMainMenuUI::SwitchToScreenComboInternal( ScreenComboType screenIdx, const bool bInheritMenuState, bool bStoreLastState, bool bPopping )
{
  bool bAddToScreenStack = false;

  if( m_bTutorialActive )
  {
    lLogFmt("SwitchToScreen: disabling tutorial\n");
    DisableTutorial();

  }

  if ((m_ScreenStack.GetSize() == 0) || (m_ScreenStack.GetAtUpperBound().m_eScreen != screenIdx))
  {
    bAddToScreenStack = true;
  }

  //Note: Shouldn't really be able to return to a message box
  if( screenIdx == SCREENCOMBO_MESSAGEBOX_YESNO || screenIdx == SCREENCOMBO_MESSAGEBOX_OK )
  {
    bAddToScreenStack = false;
  }

  // Don't allow returning to progress / save screens
  if( screenIdx == SCREENCOMBO_SAVE || screenIdx == SCREENCOMBO_SAVE_EDITMODE || screenIdx == SCREENCOMBO_SAVEDONE || screenIdx == SCREENCOMBO_LOADING )
  {
    bAddToScreenStack = false;
  }

  /*
  if( screenIdx == SCREENCOMBO_TRANSFERLIST )
  {
    // remove transfer list edit filter screen from stack if we go back to transferlist
    for(int i = 0; i < m_ScreenStack.GetSize(); i++)
    {
      if( m_ScreenStack[i].m_eScreen == SCREENCOMBO_TRANSFERLIST_EDIT )
      {
        lLogFmt("Removing transferlist edit %d\n", i);
        m_ScreenStack.RemoveAt(i);
        break;
      }
    }
  }

  // remove two transfer list screens in a row as a result of our removal
  for(int i = 0; i < m_ScreenStack.GetSize()-1; i++)
  {
    if( m_ScreenStack[i].m_eScreen == SCREENCOMBO_TRANSFERLIST &&
        m_ScreenStack[i+1].m_eScreen == SCREENCOMBO_TRANSFERLIST )
    {
      lLogFmt("Removing multiple transferlist %d\n", i);
      m_ScreenStack.RemoveAt(i+1);
      break;
    }
  }            */



  // turn off all screens
  m_fade->Hide();
  m_fadeSub->Hide();

  // hide menus (rollouts, timers, ticker)
  m_menuScreen->Hide();
  m_menuScreenSub->Hide();

  // hide other screens
  m_mainmenuScreen->Hide();
  m_mainmenuScreenSub->Hide();

  m_newgameScreen->Hide();
  m_selectteamScreen->Hide();
  m_playerInfoSubScreen->Hide();
  m_playerInfoMainScreen->Hide();
  m_squadScreen->Hide();
  m_infoScreen->Hide();
  m_accountsScreen->Hide();
  m_leadingPlayerScreen->Hide();
  m_sackPlayerScreen->Hide();
  m_clubRecordsScreen->Hide();
  m_leagueFixturesScreen->Hide();
  m_matchIntroScreen->Hide();
  m_clubFixturesScreen->Hide();
  m_cupFixturesScreen->Hide();
  m_transferlistScreen->Hide();
  m_transferlisteditScreen->Hide();
  m_teamoftheweekScreen->Hide();
  m_leagueTableScreen->Hide();
  m_trainingScreen->Hide();
  m_jobCentreScreen->Hide();
  m_employeeInfoScreen->Hide();
  m_youthSquadScreen->Hide();
  m_youthInfoScreen->Hide();
  m_managerInfoScreen->Hide();
  m_tacticsScreen->Hide();
  m_negotiateScreen->Hide();
  m_matchSummaryScreen->Hide();
  m_matchDayScreen->Hide();
  m_cupScheduleScreen->Hide();
  m_saveScreen->Hide();
  m_loadScreen->Hide();
  m_statusScreen->Hide();
  m_newsScreen->Hide();
  m_progressScreen->Hide();
  m_transferStatusScreen->Hide();
  m_wagesScreen->Hide();
  m_arrangeFriendliesScreen->Hide();
  m_options->Hide();
  m_monthlyAwardScreen->Hide();
  m_editModeScreen->Hide();
  m_editNameScreen->Hide();
  m_strategyFormationScreen->Hide();
  m_strategyRolesScreen->Hide();
  m_clubInfoSubScreen->Hide();

  // hide elements
  m_messageBoxFade->Hide();
  m_link->Hide();
  m_splitter->Show();

  // Return menubar back to whence it came
  GetMenuBar4()->InsertAfter(m_menuBar);
  GetMenuBar2()->InsertAfter(m_menuBar);

  // hide sub message text
  if( !bPopping )
  {
    HideSubMessage();
  }

  // turn on screens
  bool bMenu = true;
  bool bLink = false;
  bool bPlayMusic = false;
  int  menuBar    = 4;                  // 4 slot menu bar

  m_bBackShouldBeVisible = false;

  // clear input screen
  m_activeInputScreen = NULL;

  SetLayout( true, true, screenIdx );

  if( DoesTutorialExist( screenIdx ) )
  {
    m_help->Show();
  } else
  {
    m_help->Hide();
  }

  switch( screenIdx )
  {
    case SCREENCOMBO_LOADING:
      bMenu = false;
      menuBar = 0;
      bPlayMusic = false;
      m_splitter->Hide();
      break;

    case SCREENCOMBO_MAINMENU:
      bMenu = false;
      menuBar = 0;
      m_mainmenuScreen->Show();
      m_mainmenuScreenSub->Show();
      bPlayMusic = true;
      m_splitter->Hide();

      // Ensure that we don't get stuck if the user exits to the main menu after being sacked.
      m_bProcessingActive = false;
      break;
    case SCREENCOMBO_NEWGAME:
      bMenu = false;
      menuBar = 0;
      m_newgameScreen->Show();
      m_mainmenuScreenSub->Show();
      m_activeInputScreen = m_newgameScreen;
      m_splitter->Show();
      bPlayMusic = true;
      break;
    case SCREENCOMBO_NEWGAME_SELECTTEAM:
    case SCREENCOMBO_SELECTNEWTEAM:
      if( screenIdx == SCREENCOMBO_SELECTNEWTEAM )
      {
        // TODO LOCALE
        //lUIText selectTeam("Select a new team\nto manage.");
        //m_selectteamScreen->SetInfoText( selectTeam );
      } else
      {
        // TODO LOCALE
        //lUIText selectTeam("Select which team you\nwould like to manage.");
        //m_selectteamScreen->SetInfoText( selectTeam );
        bPlayMusic = true;                // music only on front end
      }

      bMenu = false;
      menuBar = 0;
      m_selectteamScreen->Show();
      m_mainmenuScreenSub->Show();
      m_activeInputScreen = m_selectteamScreen;
      m_splitter->Show();
      break;
    case SCREENCOMBO_SQUAD_CLUB:
    case SCREENCOMBO_SQUAD:
      m_playerInfoSubScreen->Show();
      m_squadScreen->Update();
      m_squadScreen->Show();
      m_activeInputScreen = m_squadScreen;
      m_bBackShouldBeVisible = true;
      bLink = true;
      break;
    case SCREENCOMBO_NEWSSTATUS:
      m_statusScreen->Update();
      m_statusScreen->Show();
      m_newsScreen->Update();
      m_newsScreen->Show();
	    m_bBackShouldBeVisible = true;
      bLink = true;
      break;
    case SCREENCOMBO_ACCOUNTS:
      m_accountsScreen->Update();
      m_accountsScreen->Show();
	    m_bBackShouldBeVisible = true;
      break;
    case SCREENCOMBO_INFO:
      m_infoScreen->Show();
      m_activeInputScreen = m_infoScreen;
      m_bBackShouldBeVisible = false;
      bMenu = false;
      menuBar = 0;
      m_splitter->Hide();
      break;
    case SCREENCOMBO_LEADINGPLAYERS:
      m_playerInfoSubScreen->Show();
      m_leadingPlayerScreen->Update();
      m_leadingPlayerScreen->Show();
	    m_bBackShouldBeVisible = true;
      bLink = true;
      break;
    case SCREENCOMBO_CLUBFIXTURES:
      m_clubFixturesScreen->Update();
      m_clubFixturesScreen->Show();
	    m_bBackShouldBeVisible = true;
      bLink = true;
      break;
    case SCREENCOMBO_LEAGUEFIXTURES:
      m_leagueFixturesScreen->Update();
      m_leagueFixturesScreen->Show();
	    m_bBackShouldBeVisible = true;
      bLink = true;
      break;
    case SCREENCOMBO_CUPSCHEDULE:
      m_cupScheduleScreen->Update();
      m_cupScheduleScreen->Show();
	    m_bBackShouldBeVisible = true;
      bLink = true;
      break;
    case SCREENCOMBO_CUPFIXTURES:
      m_cupFixturesScreen->Update();
      m_cupFixturesScreen->Show();
	    m_bBackShouldBeVisible = true;
      bLink = true;
      break;
    case SCREENCOMBO_PLAYERPROFILE:
      m_playerInfoSubScreen->Show();
      m_playerInfoMainScreen->Show();
	    m_bBackShouldBeVisible = true;
      break;
    case SCREENCOMBO_TRANSFERLIST:
      m_transferlistScreen->Show();
	    m_bBackShouldBeVisible = true;
      bLink = true;
      break;
    case SCREENCOMBO_TRANSFERLIST_EDIT:
      m_playerInfoSubScreen->SetPlayerInfo(NULL);
      m_playerInfoSubScreen->Hide();
      m_transferlisteditScreen->Show();
      m_bBackShouldBeVisible = true;
      break;
    case SCREENCOMBO_TRANSFERSTATUS:
      m_playerInfoSubScreen->Show();
      m_transferStatusScreen->Show();
      m_bBackShouldBeVisible = false;
      bMenu = false;
      menuBar = 0;
      bLink = true;
      break;
    case SCREENCOMBO_TRAINING:
      m_trainingScreen->Update();
      m_trainingScreen->Show();
	    m_bBackShouldBeVisible = true;
      break;
    case SCREENCOMBO_JOBCENTRE:
      m_jobCentreScreen->Show();
      //m_employeeInfoScreen->Show();
      m_bBackShouldBeVisible = true;
      bLink = true;
      break;
    case SCREENCOMBO_YOUTHSQUAD:
      m_youthSquadScreen->Show();
      //m_youthInfoScreen->Show();
      m_bBackShouldBeVisible = true;
      bLink = false;
      break;
    case SCREENCOMBO_NEGOTIATION:
      bMenu = false;
      m_bBackShouldBeVisible = false;
      m_negotiateScreen->Update();
      m_negotiateScreen->Show();
      m_activeInputScreen = m_negotiateScreen;
      m_splitter->Hide();
	    bLink = false;
      break;
    case SCREENCOMBO_SACKPLAYER:
      bMenu = false;
      m_bBackShouldBeVisible = false;
      m_sackPlayerScreen->Update();
      m_sackPlayerScreen->Show();
      m_splitter->Hide();
      break;
    case SCREENCOMBO_TACTICS:
      m_tacticsScreen->SetMatchDetails( NULL );
      m_tacticsScreen->Update();
      m_tacticsScreen->Show();
	    m_bBackShouldBeVisible = true;
      break;
    case SCREENCOMBO_TACTICS_IN_MATCH:
      m_tacticsScreen->Update();
      m_tacticsScreen->Show();
	    m_bBackShouldBeVisible = true;
      bMenu = false;
      menuBar = 0;
      break;
    case SCREENCOMBO_CLUBRECORDS:
      m_clubRecordsScreen->Update();
      m_clubRecordsScreen->Show();
	    m_bBackShouldBeVisible = true;
      break;
    case SCREENCOMBO_MATCH:
      m_matchDayScreen->Update();
      m_matchDayScreen->Show();
      bMenu = false;
      m_menuScreen->Show();
      m_menuScreenSub->Show();
      m_splitter->Show();
      menuBar = 0;
      m_activeInputScreen = m_matchDayScreen;
      bLink = true;
      break;
    case SCREENCOMBO_MATCH_INTRO:
      m_matchIntroScreen->Update();
      m_matchIntroScreen->Show();
      m_activeInputScreen = m_matchIntroScreen;
      bMenu = false;
      bLink = true;
      m_menuScreen->Show();
      m_menuScreenSub->Show();
      m_splitter->Show();
      break;
    case SCREENCOMBO_MATCH_SUMMARY:
      bMenu = false;
      menuBar = 0;
      m_menuScreen->Show();
      m_menuScreenSub->Show();
      m_matchSummaryScreen->Update();
      m_matchSummaryScreen->Show();
      m_splitter->Show();
      m_activeInputScreen = m_matchSummaryScreen;
      break;
    case SCREENCOMBO_TEAMOFTHEWEEK:
      m_teamoftheweekScreen->Update();
      m_teamoftheweekScreen->Show();
      m_activeInputScreen = m_teamoftheweekScreen;
      bMenu = false;
      menuBar = 0;
      bLink = true;
      break;
    case  SCREENCOMBO_LEAGUETABLE:
      m_leagueTableScreen->Update();
      m_leagueTableScreen->Show();
      m_bBackShouldBeVisible = true;
      bLink = true;
      break;
    case SCREENCOMBO_WAGESSCREEN:
      m_wagesScreen->Show();
      m_bBackShouldBeVisible = true;
      bLink = true;
      break;
    case SCREENCOMBO_ARRANGEFRIENDLIES:
      m_arrangeFriendliesScreen->Update();
      m_arrangeFriendliesScreen->Show();
      m_activeInputScreen = m_arrangeFriendliesScreen;
      bMenu = false;
      bLink = true;
      break;
    case SCREENCOMBO_MOTM:
      m_monthlyAwardScreen->Update();
      m_monthlyAwardScreen->Show();
      m_bBackShouldBeVisible = true;
      bLink = false;
      break;
    //case SCREENCOMBO_LANGUAGE:
    //  m_languageScreen->Update();
    //  m_languageScreen->Show();
    //  bPlayMusic = true;
    //  break;
    case SCREENCOMBO_OPTIONS:
      bMenu = false;
      menuBar = 0;
      m_options->Update();
      m_options->Show();
      bPlayMusic = false;
      break;
    case SCREENCOMBO_OPTIONS_IN_GAME:
      m_options->InGame();
      m_options->Update();
      m_options->Show();
      m_bBackShouldBeVisible = true;
      bMenu = false;
      menuBar = 0;
      break;
    case SCREENCOMBO_MESSAGEBOX_OK:
    case SCREENCOMBO_MESSAGEBOX_YESNO:
      bMenu = false;
      menuBar = 0;
      m_messageBoxFade->Show();
      m_fadeSub->Show();
      m_splitter->Hide();
      break;
    case SCREENCOMBO_PROGRESS:
      bMenu = false;
      menuBar = 0;
      m_progressScreen->Show();
      m_splitter->Hide();
      break;
    case SCREENCOMBO_SAVE:
    case SCREENCOMBO_SAVE_EDITMODE:
      lLogFmt("Going to save screen\n");
      bMenu = false;
      m_saveScreen->Show();
      m_splitter->Hide();
      m_mainmenuScreenSub->Show();
      m_bBackShouldBeVisible = false;
      break;
    case SCREENCOMBO_SAVEDONE:
      lLogFmt("Going to save done screen\n");
      m_saveScreen->Show();
      m_mainmenuScreenSub->Show();
      m_bBackShouldBeVisible = true;
      break;
    case SCREENCOMBO_EDITMODE:
      m_editModeScreen->Show();
      //m_mainmenuScreenSub->Show();
      m_bBackShouldBeVisible = false;
      bMenu = false;
      bPlayMusic = false;
      menuBar = 0;
      m_splitter->Show();
      break;
    case SCREENCOMBO_EDITNAME:
      m_editNameScreen->Show();
      //m_mainmenuScreenSub->Show();
      m_bBackShouldBeVisible = false;
      bPlayMusic = false;
      bMenu = false;
      menuBar = 0;
      m_splitter->Show();
      break;
    case SCREENCOMBO_STRATEGY_FORMATION:
      m_playerInfoSubScreen->Show();
      m_strategyFormationScreen->Show();
      m_bBackShouldBeVisible = true;
      bMenu = false;
      menuBar = 0;
      bLink = true;
      break;
    case SCREENCOMBO_STRATEGY_ROLES:
      m_playerInfoSubScreen->Show();
      m_strategyRolesScreen->Show();
      m_bBackShouldBeVisible = true;
      bMenu = false;
      menuBar = 0;
      bLink = true;
      break;
    default:
      lLogFmt("Screen combo not found\n");
      break;
  };

  // setup whether to play music
  if( m_currentScreenIdx == SCREENCOMBO_MAINMENU || m_currentScreenIdx == SCREENCOMBO_NEWGAME )
  {
    if( screenIdx == SCREENCOMBO_MESSAGEBOX_YESNO || screenIdx == SCREENCOMBO_LOADING )
    {
      //lLogFmt("Special case message box / loading (on front end)\n");
      bPlayMusic = true;
    }
  }

  if( m_currentScreenIdx == SCREENCOMBO_EDITMODE || m_currentScreenIdx == SCREENCOMBO_EDITNAME )
  {
    if( screenIdx == SCREENCOMBO_MESSAGEBOX_YESNO || screenIdx == SCREENCOMBO_LOADING )
    {
      //lLogFmt("Special case message box / loading (on front end)\n");
      bPlayMusic = false;
    }
  }

  // inherit menu state from last element in stack
  if ((bInheritMenuState == true) && (m_ScreenStack.GetSize() > 0))
  {
    bMenu = m_ScreenStack.GetAtUpperBound().m_bMenuVisible;
  }

  lLogBreak();
  lLogFmt("SwitchToScreenCombo %d (%s)\n", screenIdx, GetScreenName(screenIdx));
  for(int i = 0; i < m_ScreenStack.GetSize(); i++)
	{
    lLogFmt("  %d: screenidx %d (data %d)\n", i, m_ScreenStack.GetAt(i).m_eScreen, m_ScreenStack.GetAt(i).m_stateData.Length());
  }
  lLogFmt("\n");
  lLogBreak();

  // popping don't do prepare step
  if( !bPopping )
  {
    FDScreen *curScreen = GetScreenClass( screenIdx );
    if( curScreen != NULL )
    {
      curScreen->Prepare();
    }
  }

  // Store previous screens state if it is present on the stack (i.e. we could return to it)
  if( bStoreLastState )
  {
    FDScreen *curScreen = GetScreenClass( m_currentScreenIdx );

    if( curScreen != NULL )
    {
      lLogBreak();
      lLogFmt("Serializing last screen (%d) state data to stack idx %d\n", m_currentScreenIdx, m_ScreenStack.GetSize()-1);

      // check if this is a squad screen whether it's the users club
      if( m_currentScreenIdx == SCREENCOMBO_SQUAD )
      {
        if( m_squadScreen->GetSquadInfo() == WorldData().GetCurrentUserClub() )
        {
          lLogFmt("Setting squad to be squad club in stack\n");
          m_currentScreenIdx = SCREENCOMBO_SQUAD_CLUB;
          m_ScreenStack.GetAtUpperBound().m_eScreen = SCREENCOMBO_SQUAD_CLUB;
        }
      }

      if( m_ScreenStack.GetSize() )
      {
        lLogFmt("  - Valid screen stack\n");
        if( m_ScreenStack.GetAtUpperBound().m_eScreen == m_currentScreenIdx )
        {
          lLogFmt("  - Correct screen\n");
          m_ScreenStack.GetAtUpperBound().m_stateData.InitSave();
          curScreen->SerializeState( m_ScreenStack.GetAtUpperBound().m_stateData );
          lLogFmt("  length %d\n", m_ScreenStack.GetAtUpperBound().m_stateData.Length());
        }
      }
      lLogBreak();
    }
  }

  // add new screen to stack
  if (bAddToScreenStack == true)
  {
    ScreenComboInterface cNewScreenInformation;
    cNewScreenInformation.m_eScreen = screenIdx;
    cNewScreenInformation.m_bMenuVisible = bMenu;
    cNewScreenInformation.m_bPlayMusic = bPlayMusic;
    cNewScreenInformation.m_bBackVisible = m_bBackShouldBeVisible;
    cNewScreenInformation.m_bMenuInherited = bInheritMenuState;
    cNewScreenInformation.m_bLinkVisible = bLink;

    // TODO store pointer for this screen to ease save process

    m_ScreenStack.Add(cNewScreenInformation);
  }

  lLogBreak();
  lLogFmt("SwitchToScreenCombo Post-Add %d\n", screenIdx);
  for(int i = 0; i < m_ScreenStack.GetSize(); i++)
	{
    lLogFmt("  %d: screenidx %d (data %d)\n", i, m_ScreenStack.GetAt(i).m_eScreen, m_ScreenStack.GetAt(i).m_stateData.Length());
  }
  lLogFmt("\n");
  lLogBreak();

  // Check if screen stack has grown too big
  if( m_ScreenStack.GetSize() > MAX_SCREENSTACK_SIZE )
  {
    lAssert( MAX_SCREENSTACK_SIZE >= 2 );
    int toRemove = m_ScreenStack.GetSize() - MAX_SCREENSTACK_SIZE;

    // Remove from the 2nd entry (i.e. never remove the status screen which will be first)
    lLogFmt("Removing %d screens from screen stack\n", toRemove);
    for(int i = 0; i < toRemove; i++)
    {
      m_ScreenStack.RemoveAt(1);
    }
  }

  //////////////////////////////////////////////////////////////////////////
  // Do not modify any of the screen options below this point -- otherwise,
  // they won't be saved correctly on the stack.
  if( bPlayMusic )
  {
    if( FDSound::GetMusicPlayingIdx() != FD_MENU_MUSIC || FDSound::IsMusicFading() )
    {
      //lLogFmt("\nPlaying music\n");
      FDSound::PlayMusic( FD_MENU_MUSIC, ( m_currentScreenIdx != SCREENCOMBO_MAINMENU ));
    }
  } else
  {
    // stop if not already
    //lLogFmt("\nFading music\n");
    FDSound::FadeMusic();
  }

  if( bMenu )
  {
    ShowMenuMenu( true );

    m_menuScreen->Show();
    m_menuScreenSub->Show();
  } else
  {
    ShowMenuMenu( false );
  }

  m_menuBar->Hide();
  m_menuBar2->Hide();
  m_menuBar4->Hide();

  if( menuBar == 4 )
  {
    m_menuBar4->Show();
  } else
  if( menuBar == 2 )
  {
    m_menuBar2->Show();
  } else if (menuBar != 0)
  {
    m_menuBar->Show();
  }

  if( m_bBackShouldBeVisible == true && m_ScreenStack.GetSize() > 1 )
	  m_back->Show();
  else
	  m_back->Hide();

  if (bLink)
  {
    // En/Disable links
    if( FDTextIsEnabled() )
    {
      m_link->SetSprite(FDResourceManager::GetSprite("uiicon","link"));
    }
    else
    {
      m_link->SetSprite(FDResourceManager::GetSprite("uiicon","link_off"));
    }
    m_link->Show();
  }

  // Check whether gamedata exists
  if( screenIdx == SCREENCOMBO_MAINMENU )
  {
    // Continue will be disabled unless a save game is found
    if( !GamedataExist() )
    {
      lLogFmt("No save data found\n");
      m_contgameButton->Disable();
    } else
    {
      lLogFmt("Save data found\n");
      m_contgameButton->Enable();
    }
  }

  //TODO: Restore previous screen state here

  m_currentScreenIdx = screenIdx;

  if( screenIdx == SCREENCOMBO_SQUAD_CLUB )
  {
    // Make sure we force users club to be viewed
    if( GetSquadScreen()->GetSquadInfo() != WorldData().GetCurrentUserClub() )
    {
      GetSquadScreen()->SetSquadInfo( WorldData().GetCurrentUserClub() );
    }
  }

  if( screenIdx == SCREENCOMBO_SAVE )
  {
    lLogFmt("Commencing save\n");
    m_saveScreen->SetEditMode( false );
    GamedataSave( m_saveScreen );

    // set to done
    SwitchToScreenComboInternal( SCREENCOMBO_SAVEDONE, false, false );
    m_saveScreen->SetProgress( 1.0f, false );
  }

  if( screenIdx == SCREENCOMBO_SAVE_EDITMODE )
  {
    lLogFmt("Commencing edit mode save\n");
    m_saveScreen->SetEditMode( true );
    EditModeSave( m_saveScreen );

    // set to done
    SwitchToScreenComboInternal( SCREENCOMBO_MAINMENU, false, false );
    m_saveScreen->SetProgress( 1.0f, false );
  }

  // tutorial
  if( GetSaveData().m_userPrefs.m_bTutorialEnabled )
  {
    if( DoesTutorialExist( screenIdx ) )
    {
      // tutorials enabled, check if we've viewed this screen
      if( !GetSaveData().m_bTutorialViewed[ screenIdx ] )
      {
        lLogFmt("Enabling tutorial for %s\n", GetScreenName( screenIdx ));

        // mark screen as viewed
        bool bViewIntro = !GetSaveData().m_bTutorialIntroViewed;
        GetSaveData().m_bTutorialIntroViewed = true;
        GetSaveData().m_bTutorialViewed[ screenIdx ] = true;

        // Save options
        OptionsSave();

        // start tutorial
        EnableTutorial( screenIdx, bViewIntro );
      }
    }
  }
}

struct TutorialPage
{
  TutorialPage()
  {
    textPos = TEXTPOS_TOP;
    textScreen = SCREEN_TOP;

    bDisplaySprite = false;
    bHighlight = false;
  }

  enum
  {
    SCREEN_TOP,
    SCREEN_BOTTOM,
  };

  enum
  {
    TEXTPOS_TOP,
    TEXTPOS_CENTRE,
    TEXTPOS_BOTTOM,
  };

  lString   text;
  int       textPos;
  int       textScreen;

  // sprite to show
  bool      bDisplaySprite;
  lString   spriteBankName;
  lString   spriteName;

  enum
  {
    SPRITEPOS_CENTRE,
    SPRITEPOS_LEFTTEXT,
    SPRITEPOS_RIGHTTEXT,
    SPRITEPOS_SPECIFIC,
  };

  int       spritePos;
  int       spriteScreen;
  float     spriteSpecificX;
  float     spriteSpecificY;

  // highlight to show
  bool      bHighlight;
  int       highlightScreen;
  lString   highlightWidgetName;
  lString   highlightWidgetName2;
  float     highlightX;
  float     highlightY;
  float     highlightW;
  float     highlightH;

  void Log()
  {
    lLogFmt("Page\n");
    lLogFmt("  Text: %s\n", text.CStr());
    if( textPos == TEXTPOS_TOP )       lLogFmt("  TextPos: TOP\n");
    if( textPos == TEXTPOS_CENTRE )    lLogFmt("  TextPos: CENTRE\n");
    if( textPos == TEXTPOS_BOTTOM )    lLogFmt("  TextPos: BOTTOM\n");
    if( textScreen == SCREEN_TOP )     lLogFmt("  TextScreen: TOP\n");
    if( textScreen == SCREEN_BOTTOM )  lLogFmt("  TextScreen: BOTTOM\n");

    if( bDisplaySprite )
    {
      lLogFmt("  Sprite: %s / %s\n", spriteBankName.CStr(), spriteName.CStr());
      if( spritePos == SPRITEPOS_CENTRE )       lLogFmt("  SpritePos: CENTRE\n");
      if( spritePos == SPRITEPOS_LEFTTEXT )     lLogFmt("  SpritePos: LEFTTEXT\n");
      if( spritePos == SPRITEPOS_RIGHTTEXT )     lLogFmt("  SpritePos: RIGHTTEXT\n");
      if( spritePos == SPRITEPOS_SPECIFIC )
      {
        lLogFmt("  SpritePos: SPECIFIC\n");
        lLogFmt("     %.3f, %.3f\n", spriteSpecificX, spriteSpecificY);
      }
      if( spriteScreen == SCREEN_TOP )          lLogFmt("  SpriteScreen: TOP\n");
      if( spriteScreen == SCREEN_BOTTOM )       lLogFmt("  SpriteScreen: BOTTOM\n");
    }

    if( bHighlight )
    {
      if( highlightWidgetName.Size() )
      {
        lLogFmt("  Highlight: %s screen - Widget %s\n", highlightScreen == SCREEN_TOP ? "TOP" : "BOTTOM", highlightWidgetName.CStr());

        if( highlightWidgetName2.Size() )
        {
          lLogFmt("  Highlight: %s screen - Widget2 %s\n", highlightScreen == SCREEN_TOP ? "TOP" : "BOTTOM", highlightWidgetName2.CStr());
        }

      } else
      {
        lLogFmt("  Highlight: %s screen - %.2f, %.2f -> %.2f, %.2f\n", highlightScreen == SCREEN_TOP ? "TOP" : "BOTTOM",
                                                                       highlightX, highlightY,
                                                                       highlightX + highlightW,
                                                                       highlightY + highlightH );
      }

    }
  }
};

// move into class
#define PRE_CLICKTOSKIP_DELAY 0.25f

static lArray<TutorialPage> s_tutorialPages;

static lString ParseLineBreaks( lString str )
{
  lString strOut = str;

//  lLogFmt("ParseLineBreaks:\n%s\n", str.CStr());
  int idx = strOut.FindFirst("\\n", 0);
  while( idx != -1 )
  {
    strOut.Remove( idx, 1 );
    strOut(idx) = '\n';

    idx = strOut.FindFirst("\\n", idx);
  }
//  lLogFmt("PostParseLineBreaks:\n%s\n", strOut.CStr());

  return strOut;
}


void FDMainMenuUI::DisableTutorial()
{
  m_tutorialBoxBg->Hide();
  m_tutorialText->Hide();
  m_tutorialSprite->Hide();
  m_tutorialBoxBgSub->Hide();
  m_tutorialTextSub->Hide();
  m_tutorialSpriteSub->Hide();
  m_tutorialTapScreen->Hide();
  m_tutorialHighlight->Hide();
  m_tutorialHighlightSub->Hide();

  m_tutorialCurrentPage = -1;
  m_bTutorialActive = false;
}

bool FDMainMenuUI::DoesTutorialExist( ScreenComboType screenIdx )
{
  char filename[128];

  sprintf( filename, "tut_%s.ini", GetScreenName( screenIdx ) );

  lIOStreamSeekable*stream = FDResourceManager::GetFile( filename );

  if( stream != NULL )
  {
    delete stream;
    return true;
  }

  return false;
}

void FDMainMenuUI::LoadTutorial( const char *filename )
{
  lConfigFile file;

  lIOStreamSeekable *stream = FDResourceManager::GetFile( filename );

  if( stream == NULL )
  {
    lLogFmt("Error opening %s\n", filename);
    return;
  }

  if( !file.Read( stream ) )
  {
    lLogFmt("Error reading %s\n", filename);
    delete stream;
    return;
  }

  file.SetDefaultSectionName( "default" );

  lLogFmt("Read %s\n", filename);

  // parse information into pages

  const char *curSectionName = file.GetNextSection( NULL );

  while( curSectionName != NULL )
  {
    if( strcmpi( curSectionName, "default" ) )
    {
      lLogFmt("Adding section %s\n", curSectionName);
      s_tutorialPages.Add(1);
      TutorialPage &page = s_tutorialPages.Last();

      // text for this page
      page.text = ParseLineBreaks( file.GetString( curSectionName, "text", "" ) );

      // position on screen the text goes
      const char *textPosStr = file.GetString( curSectionName, "textPos", "TOP" );
      if( !strcmpi(textPosStr, "TOP") )
      {
        page.textPos = TutorialPage::TEXTPOS_TOP;
      } else
      if( !strcmpi(textPosStr, "BOTTOM") )
      {
        page.textPos = TutorialPage::TEXTPOS_BOTTOM;
      } else
      {
        page.textPos = TutorialPage::TEXTPOS_CENTRE;
      }

      // which screen the text goes on
      const char *textScreenStr = file.GetString( curSectionName, "textScreen", "TOP" );
      if( !strcmpi(textScreenStr, "TOP") )
      {
        page.textScreen = TutorialPage::SCREEN_TOP;
      } else
      {
        page.textScreen = TutorialPage::SCREEN_BOTTOM;
      }

      // sprite info
      page.spriteBankName = file.GetString( curSectionName, "spriteBank", "" );
      page.spriteName     = file.GetString( curSectionName, "spriteName", "" );

      if( page.spriteName != "" )
      {
        page.bDisplaySprite = true;
      } else
      {
        page.bDisplaySprite = false;
      }

      // which screen the sprite goes on
      const char *spritePosStr = file.GetString( curSectionName, "spritePos", "CENTRE" );
      if( !strcmpi(spritePosStr, "CENTRE") )
      {
        page.spritePos = TutorialPage::SPRITEPOS_CENTRE;
      } else
      if( !strcmpi(spritePosStr, "SPECIFIC") )
      {
        page.spritePos = TutorialPage::SPRITEPOS_SPECIFIC;
      } else
      if( !strcmpi(spritePosStr, "RIGHTTEXT") )
      {
        page.spritePos = TutorialPage::SPRITEPOS_RIGHTTEXT;
      } else
      {
        page.spritePos = TutorialPage::SPRITEPOS_LEFTTEXT;
      }

      const char *spriteScreenStr = file.GetString( curSectionName, "spriteScreen", "TOP" );
      if( !strcmpi(spriteScreenStr, "TOP") )
      {
        page.spriteScreen = TutorialPage::SCREEN_TOP;
      } else
      {
        page.spriteScreen = TutorialPage::SCREEN_BOTTOM;
      }

      if( page.spritePos == TutorialPage::SPRITEPOS_LEFTTEXT ||
          page.spritePos == TutorialPage::SPRITEPOS_RIGHTTEXT )
      {
        // override screen
        page.spriteScreen = page.textScreen;
      }

      page.spriteSpecificX = file.GetFloat( curSectionName, "spritePosX", 0.0f );
      page.spriteSpecificY = file.GetFloat( curSectionName, "spritePosY", 0.0f );

      // highlight to show
      const char *highlightScreenStr = file.GetString( curSectionName, "highlightScreen", "TOP" );
      if( !strcmpi(highlightScreenStr, "TOP") )
      {
        page.highlightScreen = TutorialPage::SCREEN_TOP;
      } else
      {
        page.highlightScreen = TutorialPage::SCREEN_BOTTOM;
      }

      const char *highlightWidgetName = file.GetString( curSectionName, "highlightWidgetName", "" );
      page.highlightWidgetName = lString(highlightWidgetName);

      const char *highlightWidgetName2 = file.GetString( curSectionName, "highlightWidgetName2", "" );
      page.highlightWidgetName2 = lString(highlightWidgetName2);

      page.highlightX = file.GetFloat( curSectionName, "highlightX", 0.0f );
      page.highlightY = file.GetFloat( curSectionName, "highlightY", 0.0f );
      page.highlightW = file.GetFloat( curSectionName, "highlightW", 0.0f );
      page.highlightH = file.GetFloat( curSectionName, "highlightH", 0.0f );

      if( (page.highlightW != 0.0f && page.highlightH != 0.0f) || page.highlightWidgetName.Size() || page.highlightWidgetName2.Size() )
      {
        page.bHighlight = true;
      } else
      {
        page.bHighlight = false;
      }

      page.Log();
    }

    // get next section
    curSectionName = file.GetNextSection( curSectionName );
  }

  delete stream;
}

void FDMainMenuUI::EnableTutorial( ScreenComboType screenIdx, bool bViewIntro )
{
  s_tutorialPages.Empty();

  // load in tutorial data for this screen from INI
  char filename[128];

  // load intro tutorial beginning
  if( bViewIntro )
  {
    sprintf( filename, "tut_intro.ini" );
    LoadTutorial( filename );
  }

  // load screen tutorial
  sprintf( filename, "tut_%s.ini", GetScreenName( screenIdx ) );
  LoadTutorial( filename );

  // tag end of intro on the end
  if( bViewIntro )
  {
    sprintf( filename, "tut_intro_end.ini" );
    LoadTutorial( filename );
  }

  // Enable tutorial
  lLogFmt("Tutorial active\n");
  m_bTutorialActive = true;
  m_tutorialCurrentPage = -1;

  TutorialNextPage();
}

static lUIWidget *FindWidgetID( lUIWidget *root, const char *str )
{
  lUIWidget * widget = root;

  while( widget )
  {
    if( widget->IsVisible() )
    {
      if( !strcmpi( str, widget->GetId() ) )
      {
        return widget;
      }
    }
    widget = widget->NextNode();
  }

  return NULL;
}

void FDMainMenuUI::TutorialNextPage()
{
  lAssert( m_bTutorialActive == true );

  // hide all the elements of the tutorials
  m_tutorialBoxBg->Hide();
  m_tutorialText->Hide();
  m_tutorialSprite->Hide();
  m_tutorialBoxBgSub->Hide();
  m_tutorialTextSub->Hide();
  m_tutorialSpriteSub->Hide();
  m_tutorialTapScreen->Hide();
  m_tutorialHighlight->Hide();
  m_tutorialHighlightSub->Hide();

  // advance the page
  m_tutorialCurrentPage++;

  if( m_tutorialCurrentPage >= s_tutorialPages.Size() )
  {
    lLogFmt("Reached end of tutorial\n");
    DisableTutorial();
    return;
  }

  // current page
  const TutorialPage &page = s_tutorialPages( m_tutorialCurrentPage );

  // show the required UI elements in the correct location
  lUITextWidget *textWidget = NULL;
  lUISpriteWidget *bgWidget = NULL;
  lUISpriteWidget *spriteWidget = NULL;

  if( page.textScreen == TutorialPage::SCREEN_TOP )
  {
    bgWidget = m_tutorialBoxBgSub;
    textWidget = m_tutorialTextSub;
  } else
  {
    bgWidget = m_tutorialBoxBg;
    textWidget = m_tutorialText;
  }

  if( page.spriteScreen == TutorialPage::SCREEN_TOP )
  {
    spriteWidget = m_tutorialSpriteSub;
  } else
  {
    spriteWidget = m_tutorialSprite;
  }

  // set text position
  if( page.textPos == TutorialPage::TEXTPOS_TOP )
  {
    // top of screen
    bgWidget->Move( lUIWidgetPosition( lUIPointPercent( 50, 15 ) ) );
    textWidget->Move( lUIWidgetPosition( lUIPointPercent( 50, 15 ) ), lUIWidgetSize( lUIPointPercent( 95, 35 ) ) );
  } else
  if( page.textPos == TutorialPage::TEXTPOS_BOTTOM )
  {
    // bottom of screen
    bgWidget->Move( lUIWidgetPosition( lUIPointPercent( 50, 100-15 ) ) );
    textWidget->Move( lUIWidgetPosition( lUIPointPercent( 50, 100-15 ) ), lUIWidgetSize( lUIPointPercent( 95, 35 ) ) );
  } else
  {
    // centre
    bgWidget->Move( lUIWidgetPosition( lUIPointPercent( 50, 50 ) ) );
    textWidget->Move( lUIWidgetPosition( lUIPointPercent( 50, 50 ) ), lUIWidgetSize( lUIPointPercent( 95, 35 ) ) );
  }

  textWidget->SetText( lUIText( page.text ) );
  textWidget->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  // display the sprite
  if( page.bDisplaySprite )
  {
    spriteWidget->SetSprite( FDResourceManager::GetSprite(page.spriteBankName.CStr(), page.spriteName.CStr()) );

    // set sprite position
    if( page.spritePos == TutorialPage::SPRITEPOS_CENTRE )
    {
      // centre screen
      spriteWidget->Move( lUIWidgetPosition( lUIPointPercent( 50, 50 ) ) );
    } else
    if( page.spritePos == TutorialPage::SPRITEPOS_LEFTTEXT )
    {
      float sprWidthPct = 100.0f * (spriteWidget->GetSprite()->GetWidth() / 256.0f);
      float remainX = (100.0f - sprWidthPct - 5.0f);

      // bottom of screen
      if( page.textPos == TutorialPage::TEXTPOS_TOP )
      {
        // top of screen
        spriteWidget->Move( lUIWidgetPosition( lUIPointPercent( 5.0f + sprWidthPct * 0.5f, 15 ) ) );
        textWidget->Move( lUIWidgetPosition( lUIPointPercent( 100 - (remainX*0.5f), 15 ) ), lUIWidgetSize( lUIPointPercent( remainX * 0.9f, 35 ) ) );
      } else
      if( page.textPos == TutorialPage::TEXTPOS_BOTTOM )
      {
        // bottom of screen
        spriteWidget->Move( lUIWidgetPosition( lUIPointPercent( 5.0f + sprWidthPct * 0.5f, 100-15 ) ) );
        textWidget->Move( lUIWidgetPosition( lUIPointPercent( 100 - (remainX*0.5f), 100-15 ) ), lUIWidgetSize( lUIPointPercent( remainX * 0.9f, 35 ) ) );
      } else
      {
        // centre
        spriteWidget->Move( lUIWidgetPosition( lUIPointPercent( 5.0f + sprWidthPct * 0.5f, 50 ) ) );
        textWidget->Move( lUIWidgetPosition( lUIPointPercent( 100 - (remainX*0.5f), 50 ) ), lUIWidgetSize( lUIPointPercent( remainX * 0.9f, 35 ) ) );
      }
    } else
    if( page.spritePos == TutorialPage::SPRITEPOS_RIGHTTEXT )
    {
      float sprWidthPct = 100.0f * (spriteWidget->GetSprite()->GetWidth() / 256.0f);
      float remainX = (100.0f - sprWidthPct - 5.0f);

      // bottom of screen
      if( page.textPos == TutorialPage::TEXTPOS_TOP )
      {
        // top of screen
        spriteWidget->Move( lUIWidgetPosition( lUIPointPercent( 100 - (5.0f + sprWidthPct * 0.5f), 10 ) ) );
        textWidget->Move( lUIWidgetPosition( lUIPointPercent( (remainX*0.5f), 10 ) ), lUIWidgetSize( lUIPointPercent( remainX * 0.9f, 35 ) ) );
      } else
      if( page.textPos == TutorialPage::TEXTPOS_BOTTOM )
      {
        // bottom of screen
        spriteWidget->Move( lUIWidgetPosition( lUIPointPercent( 100 - (5.0f + sprWidthPct * 0.5f), 100-10 ) ) );
        textWidget->Move( lUIWidgetPosition( lUIPointPercent( (remainX*0.5f), 100-10 ) ), lUIWidgetSize( lUIPointPercent( remainX * 0.9f, 35 ) ) );
      } else
      {
        // centre
        spriteWidget->Move( lUIWidgetPosition( lUIPointPercent( 100 - (5.0f + sprWidthPct * 0.5f), 50 ) ) );
        textWidget->Move( lUIWidgetPosition( lUIPointPercent( (remainX*0.5f), 50 ) ), lUIWidgetSize( lUIPointPercent( remainX * 0.9f, 35 ) ) );
      }
    } else
    {
      // centre
      spriteWidget->Move( lUIWidgetPosition( lUIPoint( page.spriteSpecificX, page.spriteSpecificY ) ) );
    }
  }

  // display the elements
  lLogFmt("Showing page %d - %s\n", m_tutorialCurrentPage, page.text.CStr());

  bgWidget->Show();
  textWidget->Show();

  // Fade screens
  m_tutorialHighlightSub->Show();
  m_tutorialHighlightSub->SetRect( 0,0,0,0 );  // dim full screen
  m_tutorialHighlight->Show();
  m_tutorialHighlight->SetRect( 0,0,0,0 );  // dim full screen

  if( page.bHighlight )
  {
    FDHighlightWidget *highlightWidget = (page.highlightScreen == TutorialPage::SCREEN_TOP) ? m_tutorialHighlightSub : m_tutorialHighlight;

    // Search for highlight widget

    if( page.highlightWidgetName.Size() || page.highlightWidgetName2.Size() )
    {
      lUIWidget *highlightSearchRoot = (page.highlightScreen == TutorialPage::SCREEN_TOP) ? m_rootScreenSub : m_rootScreen;
      lUIWidget *widgetToHighlight = FindWidgetID( highlightSearchRoot, page.highlightWidgetName.CStr() );

      if( widgetToHighlight )
      {
        lLogFmt("Found '%s' widget\n", page.highlightWidgetName.CStr());
        const lUIRect &rect = widgetToHighlight->GetRect();
        highlightWidget->SetRect( rect.x,rect.y,rect.w,rect.h );
      } else
      {
        lLogFmt("Couldn't find '%s' widget\n", page.highlightWidgetName.CStr());

        if( page.highlightWidgetName2.Size() )
        {
          lLogFmt("Trying backup\n");
          widgetToHighlight = FindWidgetID( highlightSearchRoot, page.highlightWidgetName2.CStr() );

          if( widgetToHighlight )
          {
            lLogFmt("Found '%s' widget\n", page.highlightWidgetName2.CStr());
            const lUIRect &rect = widgetToHighlight->GetRect();
            highlightWidget->SetRect( rect.x,rect.y,rect.w,rect.h );
          }
        }
      }
    } else
    {
      highlightWidget->SetRect( page.highlightX,
                                page.highlightY,
                                page.highlightW,
                                page.highlightH );
    }
  }

  if( page.bDisplaySprite )
    spriteWidget->Show();

  m_tutorialTapScreen->Show();
  m_tutorialTapScreen->ClearTapped();
  m_tutorialTapScreen->SetTapIgnoreDelay( 0.05f );
}

void FDMainMenuUI::TickTutorial( float delta )
{
  if( m_bTutorialActive == true )
  {
    // on click of anywhere on screen skip to next message if after minimal delay
    if( m_tutorialTapScreen->GetTapped() )
    {
      // if click detected them skip to next page
      lLogFmt("Tutorial screen tapped\n");
      TutorialNextPage();
    }

    // clear tapped state regardless of whether taps are being paid attention to
    m_tutorialTapScreen->ClearTapped();

    // if done with tutorial then hide
  }
}



void FDMainMenuUI::SwitchToScreenModalInternal( ScreenComboType screenIdx, const bool bInheritMenuState )
{
	// This is only supported for certain screens at the moment.
	FDScreen *pScreen = NULL;
	switch (screenIdx)
	{
  case SCREENCOMBO_SELECTNEWTEAM:
	case SCREENCOMBO_NEWGAME_SELECTTEAM:
		pScreen = m_selectteamScreen;
		break;
	default:
		EXT_ASSERT(false && "Unsupported screen type");
		break;
	}

	EXT_ASSERT(pScreen->GetModalOK() != NULL);

	m_pCurrentModalScreen = pScreen;
	pScreen->GetModalOK()->SignalControlCommand.Connect(this, &FDMainMenuUI::OnModalOK);
	m_bExitModal = false;

	SwitchToScreenComboInternal(screenIdx, bInheritMenuState);

	while (DoMainLoop() && !m_bExitModal)
	{
	}

	pScreen->GetModalOK()->SignalControlCommand.Disconnect(this, &FDMainMenuUI::OnModalOK);
	m_pCurrentModalScreen = NULL;
}

void FDMainMenuUI::OnModalOK(lUIControlWidget *)
{
	if (m_pCurrentModalScreen != NULL)
	{
		if (m_pCurrentModalScreen->RunModalVerifyOKToExitFunction() == false)
			return;
	}

	m_bExitModal = true;
}

FDScreen *FDMainMenuUI::GetScreenClass( ScreenComboType screenIdx )
{
  FDScreen *curScreen = NULL;
  switch( screenIdx )
  {
    case SCREENCOMBO_SQUAD_CLUB:
    case SCREENCOMBO_SQUAD:               curScreen = m_squadScreen; break;
    case SCREENCOMBO_CLUBFIXTURES:        curScreen = m_clubFixturesScreen; break;
    case SCREENCOMBO_CUPFIXTURES:         curScreen = m_cupFixturesScreen; break;
    case SCREENCOMBO_CUPSCHEDULE:         curScreen = m_cupScheduleScreen; break;
    case SCREENCOMBO_LEAGUEFIXTURES:      curScreen = m_leagueFixturesScreen; break;
    case SCREENCOMBO_LEAGUETABLE:         curScreen = m_leagueTableScreen; break;
    case SCREENCOMBO_TRANSFERLIST:        curScreen = m_transferlistScreen; break;
    case SCREENCOMBO_ACCOUNTS:            curScreen = m_accountsScreen; break;
    //case SCREENCOMBO_TRANSFERLIST_EDIT:   curScreen = break;
    case SCREENCOMBO_TRAINING:            curScreen = m_trainingScreen; break;
    case SCREENCOMBO_JOBCENTRE:           curScreen = m_jobCentreScreen; break;
    case SCREENCOMBO_CLUBRECORDS:         curScreen = m_clubRecordsScreen; break;
    case SCREENCOMBO_YOUTHSQUAD:          curScreen = m_youthSquadScreen; break;
    case SCREENCOMBO_LEADINGPLAYERS:      curScreen = m_leadingPlayerScreen; break;
    case SCREENCOMBO_TRANSFERSTATUS:      curScreen = m_transferStatusScreen; break;
    case SCREENCOMBO_WAGESSCREEN:         curScreen = m_wagesScreen; break;
    case SCREENCOMBO_MOTM:                curScreen = m_monthlyAwardScreen; break;
    case SCREENCOMBO_PLAYERPROFILE:       curScreen = m_playerInfoMainScreen; break;
    default: break;
  }

  return curScreen;
}

void FDMainMenuUI::PopScreen()
{
  lLogBreak();
  lLogFmt("PopScreen\n");
  for(int i = 0; i < m_ScreenStack.GetSize(); i++)
	{
    lLogFmt("  %d: screenidx %d (data %d)\n", i, m_ScreenStack.GetAt(i).m_eScreen, m_ScreenStack.GetAt(i).m_stateData.Length());
  }
  lLogFmt("\n");
  lLogBreak();


	if (m_ScreenStack.GetSize() > 1)
	{
		// Make a copy of this, because we are going to remove it from the screen stack.
    ScreenComboInterface cScreenData = m_ScreenStack.GetAt(m_ScreenStack.GetSize() - 2);
		m_ScreenStack.RemoveFromEnd(2);

    lLogFmt("Popping screen %d (%s) currentscreen: %d (%s)\n", cScreenData.m_eScreen, GetScreenName(cScreenData.m_eScreen),
                                    m_currentScreenIdx, GetScreenName(m_currentScreenIdx));
    bool bDoSerialize = true;
    // Pass in state information which we stored on the screen stack
    if( cScreenData.m_stateData.Length() && bDoSerialize )
    {
      lLogFmt("Serializing state data %d\n", cScreenData.m_stateData.Length());

      FDScreen *curScreen = GetScreenClass( cScreenData.m_eScreen );

      if( curScreen != NULL )
      {
        // preparing
        curScreen->Prepare();

        // serializing
        lLogFmt("Serializing popped screens state data\n");
        cScreenData.m_stateData.InitLoad();
        curScreen->SerializeState( cScreenData.m_stateData );
      }

      // reset data
      cScreenData.m_stateData.Reset();
    }

    // select correct menu index
    SelectCorrectMenuItem( cScreenData.m_eScreen );

    // switch screen
    SwitchToScreenCombo(cScreenData.m_eScreen, cScreenData.m_bMenuInherited, false, true);
	}
}

void FDMainMenuUI::ClearScreenStack()
{
	m_ScreenStack.RemoveAll();
}


static float s_showTime = 0.0f;

void FDMainMenuUI::ShowProcessScreen( bool bShow, int processType, int processPct )
{
  if( bShow )
  {
    s_showTime = lPlatform_GetSeconds();

    if( m_bReadyToTick )
    {
      // Hide current menu (if we're ok to tick - i.e. not during initialisation)
      SwitchToScreenComboInternal( SCREENCOMBO_LOADING, true );
    }

    //
    m_mainScreenBg->SetSprite( FDResourceManager::GetSpriteBG( "activity_background" ) );
    m_mainScreenBg->GetSprite()->SetTransparent( false ); //HACK: this shouldn't really be needed. should be a flag in the sprite bank.
    m_mainScreenBg->Show();

    m_mainScreenSubBg->SetSprite( FDResourceManager::GetSpriteBG( "title" ) );
    m_mainScreenSubBg->GetSprite()->SetTransparent( false ); //HACK: this shouldn't really be needed. should be a flag in the sprite bank.
    m_mainScreenSubBg->Show();

    m_loadScreen->Show();
    m_loadScreen->DoShow();
    m_loadScreen->SetType( processType );
    m_loadScreen->LoadProgress( (float)processPct );
    m_bReadyToTick = false;
    lLogFmt("ShowProcessScreen\n");

    // do first render
    ProcessingCallback( "ShowProcessInitialCall" );
  } else
  {
    float displayTime = lPlatform_GetSeconds() - s_showTime;

    while( displayTime < 1.0f )
    {
      ProcessingCallback( "ShowFor1Second" );
      displayTime = lPlatform_GetSeconds() -  s_showTime;
    }

    lLogFmt("HideProcessScreen\n");
    m_loadScreen->Hide();
    m_bReadyToTick = true;
  }
}

void FDMainMenuUI::DisplaySubMessage( char * text )
{
  m_messageBoxTextSub->Show();
  m_messageBoxBgSub->Show();
  m_messageBoxTextSub->SetText( lUIText(text) );
}

void FDMainMenuUI::HideSubMessage()
{
  m_messageBoxTextSub->Hide();
  m_messageBoxBgSub->Hide();
  m_messageBoxTextSub->SetText( lUIText("") );
}

const char *FDMainMenuUI::GetScreenName( ScreenComboType screenIdx )
{
  switch( screenIdx )
  {
    case SCREENCOMBO_MAINMENU:              return "MAINMENU";
    case SCREENCOMBO_PROGRESS:              return "PROGRESS";
    case SCREENCOMBO_NEWGAME:               return "NEWGAME";
    case SCREENCOMBO_NEWGAME_SELECTTEAM:    return "NEWGAME_SELECTTEAM";
    case SCREENCOMBO_SELECTNEWTEAM:         return "SELECTNEWTEAM ";
    case SCREENCOMBO_SQUAD_CLUB:            return "SQUAD_CLUB";
    case SCREENCOMBO_SQUAD:                 return "SQUAD";
    case SCREENCOMBO_NEWSSTATUS:            return "NEWSSTATUS";
    case SCREENCOMBO_ACCOUNTS:              return "ACCOUNTS";
    case SCREENCOMBO_CLUBFIXTURES:          return "CLUBFIXTURES";
    case SCREENCOMBO_CUPFIXTURES:           return "CUPFIXTURES";
    case SCREENCOMBO_CUPSCHEDULE:           return "CUPSCHEDULE";
    case SCREENCOMBO_LEAGUEFIXTURES:        return "LEAGUEFIXTURES";
    case SCREENCOMBO_LEAGUETABLE:           return "LEAGUETABLE";
    case SCREENCOMBO_TRANSFERLIST:          return "TRANSFERLIST";
    case SCREENCOMBO_TRANSFERLIST_EDIT:     return "TRANSFERLIST_EDIT";
    case SCREENCOMBO_TRAINING:              return "TRAINING";
    case SCREENCOMBO_JOBCENTRE:             return "JOBCENTRE";
    case SCREENCOMBO_TACTICS_IN_MATCH:
    case SCREENCOMBO_TACTICS:               return "TACTICS";
    case SCREENCOMBO_MATCH_INTRO:
	{
		switch (m_matchIntroScreen->GetViewType())
		{
		case FDMatchIntroScreen::VIEW_INTRO:
			return "MATCHINTRO";
		case FDMatchIntroScreen::VIEW_OPPINFO:
		case FDMatchIntroScreen::VIEW_REPORT:
			return "MATCHINTRO_OPPOSITION";
		case FDMatchIntroScreen::VIEW_SUBS:
		case FDMatchIntroScreen::VIEW_TEAMS:
			return "MATCHINTRO_TEAMS";
		default:
			return "MATCHINTRO";
		}
	}
    case SCREENCOMBO_MATCH_SUMMARY:         return "MATCHREPORT";
    case SCREENCOMBO_MATCH:                 return "MATCHSCREEN";
    case SCREENCOMBO_CLUBRECORDS:           return "CLUBRECORDS";
    case SCREENCOMBO_MESSAGEBOX_OK:         return "MESSAGEBOX_OK";
    case SCREENCOMBO_MESSAGEBOX_YESNO:      return "MESSAGEBOX_YESNO";
    case SCREENCOMBO_SAVE:                  return "SAVE";
    case SCREENCOMBO_SAVE_EDITMODE:         return "SAVE_EDITMODE";
    case SCREENCOMBO_YOUTHSQUAD:            return "YOUTHSQUAD";
    case SCREENCOMBO_NEGOTIATION:           return "NEGOTIATION";
    case SCREENCOMBO_SACKPLAYER:            return "SACKPLAYER";
    case SCREENCOMBO_TEAMOFTHEWEEK:         return "TEAMOFWEEK";
    case SCREENCOMBO_LEADINGPLAYERS:        return "LEADINGPLAYERS";
    case SCREENCOMBO_TRANSFERSTATUS:        return "TRANSFERSTATUS";
    case SCREENCOMBO_WAGESSCREEN:           return "WAGES";
    case SCREENCOMBO_ARRANGEFRIENDLIES:     return "ARRANGEFRIENDLIES";
    case SCREENCOMBO_LANGUAGE:              return "LANGUAGE";
    case SCREENCOMBO_OPTIONS:               return "OPTIONS";
    case SCREENCOMBO_OPTIONS_IN_GAME:       return "OPTIONS";
    case SCREENCOMBO_PLAYERPROFILE:         return "PLAYERPROFILE";
	case SCREENCOMBO_MOTM:					return "MONTHLYAWARDS";

    case SCREENCOMBO_EDITMODE:       return "EDITMODE";
    case SCREENCOMBO_EDITNAME:       return "EDITNAME";

    case SCREENCOMBO_STRATEGY_FORMATION:  return "FORMATIONS";
    case SCREENCOMBO_STRATEGY_ROLES:      return "ROLES";


    //case SCREENCOMBO_YOUTHPROFILE:          return "YOUTHPROFILE";
    //case SCREENCOMBO_EMPLOYEEPROFILE:       return "EMPLOYEEPROFILE";
    //case SCREENCOMBO_MANAGERPROFILE:        return "MANAGERPROFILE";
    default: break;
  }

  return "UNKNOWN";
}

void FDMainMenuUI::ForceUpdateForAllScreensOnNextShow()
{
	m_teamoftheweekScreen->SetForcedUpdateRequested(true);
	m_leadingPlayerScreen->SetForcedUpdateRequested(true);
	m_playerInfoMainScreen->SetForcedUpdateRequested(true);
	m_playerInfoSubScreen->SetForcedUpdateRequested(true);

	// Are these necessary?
	m_youthInfoScreen->SetForcedUpdateRequested(true);
	m_managerInfoScreen->SetForcedUpdateRequested(true);
	m_employeeInfoScreen->SetForcedUpdateRequested(true);

	m_squadScreen->SetForcedUpdateRequested(true);
	m_negotiateScreen->SetForcedUpdateRequested(true);
	m_sackPlayerScreen->SetForcedUpdateRequested(true);
	m_accountsScreen->SetForcedUpdateRequested(true);

	// Should not be necessary.
	m_clubFixturesScreen->SetForcedUpdateRequested(true);
	m_cupFixturesScreen->SetForcedUpdateRequested(true);
	m_cupScheduleScreen->SetForcedUpdateRequested(true);
	m_leagueFixturesScreen->SetForcedUpdateRequested(true);
	m_leagueTableScreen->SetForcedUpdateRequested(true);

	m_transferlistScreen->SetForcedUpdateRequested(true);
	m_transferlisteditScreen->SetForcedUpdateRequested(true);
	m_trainingScreen->SetForcedUpdateRequested(true);
	m_jobCentreScreen->SetForcedUpdateRequested(true);
	m_youthSquadScreen->SetForcedUpdateRequested(true);
	m_tacticsScreen->SetForcedUpdateRequested(true);
	m_clubRecordsScreen->SetForcedUpdateRequested(true);
	m_statusScreen->SetForcedUpdateRequested(true);
	m_newsScreen->SetForcedUpdateRequested(true);
	m_transferStatusScreen->SetForcedUpdateRequested(true);
	m_wagesScreen->SetForcedUpdateRequested(true);
	m_monthlyAwardScreen->SetForcedUpdateRequested(true);
}


// message box
bool g_hackLastMsgResponse = false;

int FDMainMenuUI::UserMessageBox( const CString& Message, unsigned int style )
{
  // put a message box screen on screen and wait on it to be clicked

  ScreenComboType lastScreen = GetScreenCombo();
  const bool bHideCalendar = (lastScreen == SCREENCOMBO_MAINMENU) || (lastScreen == SCREENCOMBO_EDITMODE) ||
	  (lastScreen == SCREENCOMBO_EDITNAME) || (lastScreen == SCREENCOMBO_OPTIONS) || (lastScreen == SCREENCOMBO_SELECTNEWTEAM);

  // switch to message box
  if( style & MB_YESNO )
  {
    SwitchToScreenCombo( SCREENCOMBO_MESSAGEBOX_YESNO, false );
  } else
  {
    SwitchToScreenCombo( SCREENCOMBO_MESSAGEBOX_OK, false );
  }

  // We only want to hide the calendar for message boxes in some fairly specific circumstances, and
  // the calendar is normally shown in the call to SwitchToScreenCombo() shown above.
  if (bHideCalendar)
  {
	  m_subScreenCalendar->Hide();
	  m_subScreenCalendarDay->Hide();
	  m_subScreenCalendarMonth->Hide();
	  m_subScreenCalendarYear->Hide();
  }

  // set text
  CString sTrimmedMessage = Message;
  sTrimmedMessage.Trim();
  m_messageBoxText->SetText( lUIText( sTrimmedMessage ) );
  m_messageBoxClick = MBOX_CLICK_NONE;

  //m_messageBoxBg->Show();

  if( style & MB_YESNO )
  {
    m_messageBoxOK->Hide();
    m_messageBoxYes->Show();
    m_messageBoxNo->Show();
  } else
  {
    m_messageBoxOK->Show();
    m_messageBoxYes->Hide();
    m_messageBoxNo->Hide();
  }

  // wait for button to be pressed
  int ret = 0;

  while( DoMainLoop() )
  {
    // Check for when message box button is clicked
    if( m_messageBoxClick == MBOX_CLICK_OK )
    {
      lLogFmt("Click OK received on message box button\n");
      break;
    }

    // Check for when message box button is clicked
    if( m_messageBoxClick == MBOX_CLICK_YES )
    {
      lLogFmt("Click YES received on message box button\n");
      ret = IDYES;
      g_hackLastMsgResponse = true;
      break;
    }

    // Check for when message box button is clicked
    if( m_messageBoxClick == MBOX_CLICK_NO )
    {
      lLogFmt("Click NO received on message box button\n");
      ret = IDNO;
      g_hackLastMsgResponse = false;
      break;
    }
  }

  if (bHideCalendar)
  {
	  m_subScreenCalendar->Show();
	  m_subScreenCalendarDay->Show();
	  m_subScreenCalendarMonth->Show();
	  m_subScreenCalendarYear->Show();
  }

  if( style & MB_ENDOFGAME )
  {
    lLogFmt("End of game\n");
    m_bGameOver = true;
    return IDYES;
  }

  // switch back to previous screen
  SwitchToScreenComboInternal( lastScreen, false, false );

  return ret;
}

void FDMainMenuUI::OnMessageBoxButton( lUIControlWidget *button )
{
  if( button == m_messageBoxOK )
  {
    FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );
    m_messageBoxClick = MBOX_CLICK_OK;
  }

  if( button == m_messageBoxYes )
  {
    FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );
    m_messageBoxClick = MBOX_CLICK_YES;
  }

  if( button == m_messageBoxNo )
  {
    FDSound::PlaySoundEffect( FD_SFX_UI_BACK_BUTTON );
    m_messageBoxClick = MBOX_CLICK_NO;
  }
}


