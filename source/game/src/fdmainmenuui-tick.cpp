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

//#define AUTOLOAD
//#define TIMING
#ifndef L_PLATFORM_NDS
//#define AUTOPLAY_RANDOM_VIEW
#endif

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
#include <fdscrollbarwidget.h>

#include <fdeditmodedata.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

#include <lemon/math/lmrand.h>

extern bool DoMainLoop();

// NOTE: Grab from here as including linput intereferes with WIN32 redefinitions
bool s_skipButtonPressed = false;
int s_linksEnabledCount = 0;

// NOTE: Disables injuries
bool g_injuriesDisabled = false; // should prevent all training and fixture injuries
bool g_suspensionDisabled = false; // should prevent all injures during a fixture
bool g_managerSackingsDisabled = false;


void FDMainMenuUI::OnMenuClock( lUIControlWidget * )
{
  if( !m_clock->IsEnabled() )
  {
    return;
  }

  if( m_bProcessingActive )
  {
    lLogFmt("Already processing!!\n");
    FDSound::PlaySoundEffect( FD_SFX_UI_DISABLE );
    return;
  }

//NOTE: Testing sack player
//  static int countdown = 3;
//  countdown--;
//  lLogFmt("Sacking player in %d\n", countdown);
//  if( countdown <= 0 )
//  {
//    lLogFmt("Sacking player\n");
//    WorldData().GetCurrentUserClub()->DoSackManager( true );
//    countdown = 3;
//  }
//NOTE: Testing sack player

  // This is needed to ensure that stale data is not shown.
  ForceUpdateForAllScreensOnNextShow();

  // roll in menu
  if( m_lastHotMenu != NULL )
  {
    m_lastHotMenu->RollIn();
  }

  FDSound::PlaySoundEffect( FD_SFX_UI_ADVANCE_TIME );

  if (FDCommandLineOptions::IsAutoPlay() == true)
  {
    lLogFmt("\n\n");
    DumpDlmallocStats();
	  GamedataSave();

      char filename_buffer[128];
      static int memory_dump_filename_counter = 0;

      if (memory_dump_filename_counter % 16 == 0)
      {
        ::sprintf(filename_buffer, "autoplaymemory%04d.xml", memory_dump_filename_counter / 16);
        //DumpDlmallocStatsToFile(filename_buffer);
      }

      memory_dump_filename_counter++;
    lLogFmt("\n\n");
  }

  lLogFmt("Clock button - progressing game\n");

  CString cInvalidityReason;
  bool bWasAbleToProgress = !WorldData().CurrentUserCantProgress(cInvalidityReason);

  if ((s_skipButtonPressed == true) && (bWasAbleToProgress == false))
  {
	  // Try and get the club into a running state for automated tests.
	  CClub *pClub = WorldData().GetCurrentUserClub();

	  if (pClub)
	  {
		  int iTryCount = 0;
		  do
		  {
			CYouthList &cClubYouths = pClub->GetYouthList();
			CYouthList &cAvailableYouths = pClub->GetActiveCountry().GetAvailableYouthList();

			cAvailableYouths.FillWithRandomYouths(pClub->GetActiveCountry().GetCountry(), &pClub->GetPopularity(), pClub->GetDivision().GetDivisionStrengthID());
			// Hire first available youth out of the country's pool.
			if (cAvailableYouths.GetSize() > 0)
			{
				CYouth *pNewYouth = cAvailableYouths.GetAt(0);
				pClub->DoAddYouthToSquad(pNewYouth);
				pClub->GetActiveCountry().SackYouth(pNewYouth);
			}

			// Promote all youths to the club.
			while ((cClubYouths.GetSize() > 0) && (pClub->GetFirstTeamSquad().TotalSquadSize() < PLAYERSINSQUAD))
			{
				CYouth *pNewYouth = cClubYouths.GetAt(0);

				CContract ContractOffered;
				ContractOffered.SetSignOnFee( 1 );
        ContractOffered.SetWage( 1 );
        ContractOffered.SetContractLength( 1 );

				pClub->PromoteYouth(0, &ContractOffered);
			}

			// Fill out the first team.
			CPlayerSquad &cFirstTeam = pClub->GetFirstTeamSquad();
			cFirstTeam.DoEnsureEnoughPlayers(*pClub);
			for (int i = 0; i < PLAYERSINTEAM; i++)
			{
				// Empty slot should be filled with another player.
				if (cFirstTeam.GetSquad(i) == NOPLAYER)
				{
					for (int j = PLAYERSINSQUAD - 1; j > i; j--)
					{
						if (cFirstTeam.GetSquad(j) != NOPLAYER)
						{
							cFirstTeam.DoSwapPlayers(i, j);
							break;
						}
					}

					continue;
				}

				// Injured players should be swapped out for other players.
				if (cFirstTeam.GetPlayer(i).IsAvailableForMatch() == false)
				{
					for (int j = PLAYERSINSQUAD - 1; j > i; j--)
					{
						if (cFirstTeam.GetSquad(j) != NOPLAYER)
						{
							cFirstTeam.DoSwapPlayers(i, j);
							break;
						}
					}
				}
      }

			iTryCount++;
		  }
		  while (((bWasAbleToProgress = !WorldData().CurrentUserCantProgress(cInvalidityReason)) == false) &&
			  (iTryCount < PLAYERSINSQUAD));
	  }
  }

  if( bWasAbleToProgress )
  {
    m_bProcessingActive = true;

    SwitchToScreenCombo( SCREENCOMBO_PROGRESS, false );

#ifdef TIMING
    luint64_t timeStart = lPlatform_TimerStart();
#endif

    WorldData().DoProcessMatches( m_progressScreen, true, true );

#ifdef TIMING
    int processTime = lPlatform_TimerStop( timeStart, L_TIMER_MSECONDS );

    static int numProcessCalls = 0;
    static int numProcessMS  = 0.0f;
    static int minProcessMS =  LM_INT32_MAX;
    static int maxProcessMS = -LM_INT32_MAX;

    if( processTime < minProcessMS ) minProcessMS = processTime;
    if( processTime > maxProcessMS ) maxProcessMS = processTime;

    numProcessCalls++;
    numProcessMS += processTime;

#ifdef L_PLATFORM_NDS
    #define LOG OS_Printf
#else
    #define LOG lLogFmt
#endif

    LOG("Took %dms\n", processTime);
    LOG("%d calls\n", numProcessCalls);
    float avg = (float)numProcessMS/(float)numProcessCalls;
    LOG("%.2fms average (%d -> %.2f -> %d)\n", avg, minProcessMS, avg, maxProcessMS);
#endif

    m_bProcessingActive = false;

    if( m_bGameOver )
    {
      lLogFmt("Game over!\n");
      if( FDCommandLineOptions::IsAutoPlay() && FDCommandLineOptions::ShouldQuitAtEnd() )
      {
        lLogFmt(" - quitting as requested\n");
        exit(0);
      }
      FDCommandLineOptions::ForceAutoPlay(false);
      SwitchToScreenComboInternal( SCREENCOMBO_MAINMENU, false );
      return;
    }

    // switch back to news / status always
    ClearScreenStack();
    SwitchToScreenCombo( SCREENCOMBO_NEWSSTATUS, false );
  }

  {
    CClub * club = WorldData().GetCurrentUserClub();

    if( club != NULL )
    {
      CYouthList &cAvailableYouths = club->GetActiveCountry().GetAvailableYouthList();

      int avail = club->GetFirstTeamSquad().DoCountAvailablePlayers();
      int clubYouth = club->GetYouthList().GetSize();
      int availYouth = cAvailableYouths.GetSize();

      lLogFmt("AfterProcess: %d available, %d club youth, %d avail youth - need %d\n", avail, clubYouth, availYouth, PLAYERSINTEAM + 5);
      if( (avail + availYouth + clubYouth)<= PLAYERSINTEAM + 5 )
      {
        cAvailableYouths.FillWithRandomYouths(club->GetActiveCountry().GetCountry(), &club->GetPopularity(), club->GetDivision().GetDivisionStrengthID());
        lLogFmt("Created %d youths\n", cAvailableYouths.GetSize() - availYouth);
        availYouth = cAvailableYouths.GetSize();
      }
    }
  }


  if (!bWasAbleToProgress || WorldData().CurrentUserCantProgress(cInvalidityReason))
  {
    UserMessageBox(cInvalidityReason, MB_OK | MB_ICONEXCLAMATION | MB_DEFBUTTON1);
    theApp.OnViewSquadDetails();

    lLogFmt("User can't progress!\n");
  }

  // clear current highlight
  UnhighlightAll();

  // set status screen selected in menu
  m_clubMenu->SetSelectedIndex( 0 );
  m_leagueMenu->SetSelectedIndex( -1 );
  m_adminMenu->SetSelectedIndex( -1 );
  m_gameMenu->SetSelectedIndex( -1 );
}

extern int g_seasonsPlayed;

void FDMainMenuUI::Tick(float delta)
{
  if( !m_bReadyToTick )
    return;

  // check if autoplay has ran for set number of seasons
  if (FDCommandLineOptions::IsAutoPlay() == true)
  {
    if( FDCommandLineOptions::SeasonsToPlay() != -1 )
    {
      if( g_seasonsPlayed >= FDCommandLineOptions::SeasonsToPlay() )
      {
        lLogFmt("%d seasons played - stopping autoplay\n", g_seasonsPlayed);

        if( FDCommandLineOptions::IsAutoPlay() && FDCommandLineOptions::ShouldQuitAtEnd() )
        {
          lLogFmt(" - quitting as requested\n");
          exit(0);
        }

        FDCommandLineOptions::ForceAutoPlay(false);
      }
    }
  }

  // skip screen key
  if( 1 )
  {
    // check current screen and tell it to perform whatever operation is required to skip past it
    if( s_skipButtonPressed )
    {
      g_injuriesDisabled = true;
      g_suspensionDisabled = true;
      g_managerSackingsDisabled = true;

      if( m_currentScreenIdx == SCREENCOMBO_MESSAGEBOX_OK )
      {
        // press OK
        lLogFmt("AUTOPLAY: MessageBox OK: Pressing OK\n");
        m_messageBoxClick = MBOX_CLICK_OK;
      } else
      if( m_currentScreenIdx == SCREENCOMBO_MESSAGEBOX_YESNO )
      {
        // press NO
        lLogFmt("AUTOPLAY: MessageBox YESNO: Pressing NO\n");
        m_messageBoxClick = MBOX_CLICK_NO;
      } else
      {
        lLogFmt("AUTOPLAY: %s\n", GetScreenName( m_currentScreenIdx ));

        if( m_activeInputScreen != NULL )
        {
          lLogFmt("   SKIPPING\n");
          m_activeInputScreen->AutoplaySkip();
        } else
        {
          lLogFmt("   ADVANCING\n");
          OnMenuClock( NULL );
        }
      }

#ifdef AUTOPLAY_RANDOM_VIEW
    if( !m_bProcessingActive )
    {
      /*
      static int numTest = -1;
      numTest++;

      if( numTest == 0  ) theApp.OnKnockedOutCup( "Test Cup" );
      if( numTest == 1  ) theApp.OnCupFinalResult( "Test Cup", true );
      if( numTest == 2  ) theApp.OnCupFinalResult( "Test Cup", false );
      if( numTest == 3  ) theApp.OnEndOfSeason( "Premier Division", true, true, false, 0.0f );
      if( numTest == 4  ) theApp.OnEndOfSeason( "Premier Division", false, true, false, 0.1f );
      if( numTest == 5  ) theApp.OnEndOfSeason( "Premier Division", false, false, false, 0.4f );
      if( numTest == 6  ) theApp.OnEndOfSeason( "Premier Division", false, false, false, 0.6f );
      if( numTest == 7  ) theApp.OnEndOfSeason( "Premier Division", false, false, false, 0.8f );
      if( numTest == 8  ) theApp.OnEndOfSeason( "Premier Division", false, false, true, 0.6f );
        */

      // do random view of another screen
      static ScreenComboType s_randomScreenIdx[] = {
              SCREENCOMBO_NEWSSTATUS,
              SCREENCOMBO_SQUAD,
              SCREENCOMBO_TACTICS,
              SCREENCOMBO_TRAINING,
              SCREENCOMBO_CLUBFIXTURES,
              SCREENCOMBO_LEAGUEFIXTURES,
              SCREENCOMBO_LEAGUETABLE,
              SCREENCOMBO_CUPFIXTURES,
              SCREENCOMBO_CUPSCHEDULE,
              SCREENCOMBO_LEADINGPLAYERS,
              SCREENCOMBO_MOTM,
              SCREENCOMBO_TRANSFERLIST,
              SCREENCOMBO_CLUBRECORDS,
              SCREENCOMBO_ACCOUNTS,
              SCREENCOMBO_WAGESSCREEN,
              SCREENCOMBO_JOBCENTRE,
              SCREENCOMBO_YOUTHSQUAD,
      };

      static lmRandomMersenneTwister s_autoPlayRand;  // separate random gen

      int randomIdx = s_autoPlayRand.rand() % (sizeof(s_randomScreenIdx) / sizeof(s_randomScreenIdx[0]));
      lLogFmt("Randomly viewing %d\n", s_randomScreenIdx[ randomIdx ]);
      SwitchToScreenCombo( s_randomScreenIdx[ randomIdx ], true );
    }
#endif

    } else
    {
      g_injuriesDisabled = false;
      g_suspensionDisabled = false;
	    g_managerSackingsDisabled = false;
    }
  }

  // tick tutorial
  TickTutorial( delta );

  if( m_bTutorialActive )
  {
    return;
  }

  //
  if( m_currentScreenIdx == SCREENCOMBO_MATCH )
  {
    // tick match
    m_matchDayScreen->Tick( delta );
  }
  else if( m_currentScreenIdx == SCREENCOMBO_SQUAD )
  {
    // tick match
    //m_squadScreen->Tick( delta );
  }
  else if (m_currentScreenIdx == SCREENCOMBO_STRATEGY_ROLES)
  {
	  m_strategyRolesScreen->Tick(delta);
  }

  // Menu menu icon glow
  TickMenuHighlight( delta );

  // Set enable state of menu buttons as appropriate.
  m_leagueMenu->SetButtonEnabled(4, m_leadingPlayerScreen->HasDataToDisplay());
  m_leagueMenu->SetButtonEnabled(5, m_monthlyAwardScreen->HasDataToDisplay());

#ifdef SCREENSWITCH_DELAY
  // Frame delay of screen switching, to avoid double mouse/stylus clicks...
  if (m_eSwitchingToScreen != SCREENCOMBO_INVALID)
  {
	  if (m_bSwitchingToScreenModal)
		  SwitchToScreenModalInternal(m_eSwitchingToScreen, m_bSwitchingToScreenInheritMenuState);
	  else
      SwitchToScreenComboInternal(m_eSwitchingToScreen, m_bSwitchingToScreenInheritMenuState, m_bSwitchingToScreenStoreLastState, m_bSwitchingToScreenPopping);

	  m_eSwitchingToScreen = SCREENCOMBO_INVALID;
  }
#endif
}

