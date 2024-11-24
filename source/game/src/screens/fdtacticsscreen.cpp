//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDTacticsscreen.cpp
// Description : FDTacticsScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
//#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdtacticsscreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTacticsScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

static int g_styleStrID[] =
{
  IDS_PLAYSTYLE1, IDS_PLAYSTYLE2, IDS_PLAYSTYLE3, IDS_PLAYSTYLE4,
  IDS_PLAYSTYLE5, IDS_PLAYSTYLE6, IDS_PLAYSTYLE7, IDS_PLAYSTYLE8,
  IDS_PLAYSTYLE9, IDS_PLAYSTYLE10
};

static int g_aggressionStrID[] =
{
  IDS_AGGRESSION1, IDS_AGGRESSION2, IDS_AGGRESSION3, IDS_AGGRESSION4,
  IDS_AGGRESSION5, IDS_AGGRESSION6, IDS_AGGRESSION7, IDS_AGGRESSION8,
  IDS_AGGRESSION9, IDS_AGGRESSION10
};

static int g_markingStrID[] =
{
  IDS_MARKING1, IDS_MARKING2, IDS_MARKING3,
  IDS_MARKING4, IDS_MARKING5
};


FDTacticsScreen::FDTacticsScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDTacticsScreen::~FDTacticsScreen()
{
}

void FDTacticsScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_tacticsScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "tactics_screen" );
  m_tacticsScreen->Hide();

  m_tacticsScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "tactics_screen_sub" );
  m_tacticsScreenSub->Hide();

  // add formation blobs on top screen
  for(int i = 0; i < 10; i++)
  {
    m_playerBlobList.Push( FD_GAME_NEW FDPlayerBlobWidget( m_tacticsScreenSub, 0, "formation",
                                  FDResourceManager::GetSprite("formationblobs","formation12"), i+2, 0,
                                  LUI_CENTER_CENTER, lUIPoint( 0, 0 ) ) );
  }

  // add controls to bottom screen

  // Formation combo box
  FDBackingWidget * formStyleBacking = FD_GAME_NEW FDBackingWidget ( m_tacticsScreen, 0, "abacstyle_hdr", L_ARGB(80, 0, 0, 0), LUI_LEFT_TOP, lUIPoint( 20, 2 ), lUIPoint( 237, 35 ) );
  m_tacticsStripes[0] = FD_GAME_NEW lUISpriteWidget( m_tacticsScreen, 0, "stripes", FDResourceManager::GetSprite( "uiicon", "grad_grey1" ), LUI_CENTER_CENTER, lUIPoint( 138, 10 ), lUIPoint( 237, 12 ) );
  lUITextWidget *formHeader = FD_GAME_NEW lUITextWidget( m_tacticsScreen,0, "astyle_hdr", lUIText("Formation"), LUI_CENTER_CENTER, lUIPoint( 138, 10 ) );
  formHeader->SetTextToUpper();

  m_squadFormationCombo = FD_GAME_NEW FDComboWidget( m_tacticsScreen, 0, "tacticsscreen_formation_combo", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 138, 27), lUIPoint( 120, 19) );
  m_squadFormationCombo->SetAllToUpper( true );

	for(int i = 0; i < WorldData().GetCurrentUser()->GetTacticsList().TotalStandardTactics(); i++)
	{
		CTactic& tactic = WorldData().GetCurrentUser()->GetTacticsList().GetAt(i);
		m_squadFormationCombo->AddItem( lUIText( tactic.TitleStr() ) );
	}

	m_squadFormationCombo->SignalSelection.Connect( this, &FDTacticsScreen::OnFormationSelect );

  // style of play
  FDBackingWidget * styleBacking = FD_GAME_NEW FDBackingWidget ( m_tacticsScreen, 0, "bacstyle_hdr", L_ARGB(80, 0, 0, 0), LUI_LEFT_TOP, lUIPoint( 20, 42 ), lUIPoint( 237, 35 ) );
  m_tacticsStripes[1] = FD_GAME_NEW lUISpriteWidget( m_tacticsScreen, 0, "stripes", FDResourceManager::GetSprite( "uiicon", "grad_grey1" ), LUI_TOP_CENTER, lUIPoint( 138, 44 ), lUIPoint( 237, 12 ) );
  lUITextWidget *styleHeader = FD_GAME_NEW lUITextWidget( m_tacticsScreen,0, "style_hdr", lUIText(IDS_STYLEOFPLAY), LUI_TOP_CENTER, lUIPoint( 138, 44 ) );
  styleHeader->SetTextToUpper();

  m_tacticsStyleOfPlay = FD_GAME_NEW FDComboWidget( m_tacticsScreen, 0, "styleofplay_combo", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 138, 67 ), lUIPoint( 120, 19) );//, lUIPoint( 150, 20 ) );
  m_tacticsStyleOfPlay->SetAllToUpper( true );

  for(int i = 0; i < sizeof(g_styleStrID) / sizeof(g_styleStrID[0]); i++)
  {
    m_tacticsStyleOfPlay->AddItem( lText::Text( g_styleStrID[i] ) );
  }
  m_tacticsStyleOfPlay->SignalSelection.Connect( this, &FDTacticsScreen::OnStyleSelectionChange );

  // marking
  FDBackingWidget * styleBacking2 = FD_GAME_NEW FDBackingWidget ( m_tacticsScreen, 0, "bacstyle_hdr", L_ARGB(80, 0, 0, 0), LUI_LEFT_TOP, lUIPoint( 20, 84 ), lUIPoint( 237, 35 ) );
  m_tacticsStripes[2] = FD_GAME_NEW lUISpriteWidget( m_tacticsScreen, 0, "stripes", FDResourceManager::GetSprite( "uiicon", "grad_grey1" ), LUI_TOP_CENTER, lUIPoint( 138, 86 ), lUIPoint( 237, 12 ) );
  lUITextWidget *markHeader = FD_GAME_NEW lUITextWidget( m_tacticsScreen,0, "mark_hdr", lUIText(IDS_MARKSTYLE), LUI_TOP_CENTER, lUIPoint( 138, 86 ) );
  markHeader->SetTextToUpper();
  m_tacticsMarking     = FD_GAME_NEW FDComboWidget( m_tacticsScreen, 0, "marking_combo", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 138, 109 ), lUIPoint( 120, 19) );//, lUIPoint( 150, 20 ) );
  m_tacticsMarking->SetAllToUpper( true );

  for(int i = 0; i < sizeof(g_markingStrID) / sizeof(g_markingStrID[0]); i++)
  {
    m_tacticsMarking->AddItem( lText::Text( g_markingStrID[i] ) );
  }

  m_tacticsMarking->SignalSelection.Connect( this, &FDTacticsScreen::OnMarkingSelectionChange );

  // aggression
  FDBackingWidget * styleBacking3 = FD_GAME_NEW FDBackingWidget ( m_tacticsScreen, 0, "bacstyle_hdr", L_ARGB(80, 0, 0, 0), LUI_LEFT_TOP, lUIPoint( 20, 124 ), lUIPoint( 237, 30 ) );
  m_tacticsStripes[3] = FD_GAME_NEW lUISpriteWidget( m_tacticsScreen, 0, "stripes", FDResourceManager::GetSprite( "uiicon", "grad_grey1" ), LUI_TOP_CENTER, lUIPoint( 138, 126 ), lUIPoint( 237, 12 ) );
  lUITextWidget *aggrHeader = FD_GAME_NEW lUITextWidget( m_tacticsScreen,0, "aggr_hdr", lUIText(IDS_AGGRESSION), LUI_TOP_CENTER, lUIPoint( 138, 126 ) );
  aggrHeader->SetTextToUpper();
  m_tacticsAggression = FD_GAME_NEW FDSliderWidget( m_tacticsScreen, 0, "aggression_slider", 0, lUIText(""), LUI_CENTER_CENTER, lUIPoint( 138, 145 ) );
  m_tacticsAggression->SetValueInc(0.1f);
  m_tacticsAggression->SignalSliderChange.Connect( this, &FDTacticsScreen::OnAggressionChange );

  // check boxes
  FDBackingWidget * styleBacking4 = FD_GAME_NEW FDBackingWidget ( m_tacticsScreen, 0, "bacstyle_hdr", L_ARGB(80, 0, 0, 0), LUI_LEFT_TOP, lUIPoint( 20, 158 ), lUIPoint( 237, 30 ) );
  m_counterattackCheckBox = FD_GAME_NEW FDCheckBoxWidget( m_tacticsScreen, 0, "counterattack_checkbox",
                                          FDResourceManager::GetSprite("uiicon", "tick_0"), FDResourceManager::GetSprite("uiicon", "tick_1"),
                                          lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,
                                          LUI_TOP_LEFT,
                                          lUIPointPercent( 32, 82 ) );

  m_counterattackCheckBox->SetText( lUIText( IDS_COUNTERATTACK ) );
  m_counterattackCheckBox->SignalControlCommand.Connect( this, &FDTacticsScreen::OnCounterAttackBreak );

  m_offsidetrapCheckBox = FD_GAME_NEW FDCheckBoxWidget( m_tacticsScreen, 0, "offsidetrap_checkbox",
                                          FDResourceManager::GetSprite("uiicon", "tick_0"), FDResourceManager::GetSprite("uiicon", "tick_1"),
                                          lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,
                                          LUI_TOP_LEFT,
                                          lUIPointPercent( 32, 90 ) );

  m_offsidetrapCheckBox->SetText( lUIText( IDS_USE_OFFSIDETRAP ) );
  m_offsidetrapCheckBox->SignalControlCommand.Connect( this, &FDTacticsScreen::OnOffsideTrap );

  m_matchPlay = NULL;
}

void FDTacticsScreen::Update()
{

  if( m_matchPlay == NULL )
  {
    m_tacticsStyleOfPlay->SetSelectedItem( WorldData().GetCurrentUser()->GetCurrentStyleOfPlay(), false );
    m_tacticsMarking->SetSelectedItem( WorldData().GetCurrentUser()->GetCurrentMarkingStyle(), false );
    m_tacticsAggression->SetValuePct( (float)WorldData().GetCurrentUser()->GetCurrentAggression() / 10.0f );

    m_counterattackCheckBox->SetChecked( WorldData().GetCurrentUser()->GetCounterAttack() );
    m_offsidetrapCheckBox->SetChecked( WorldData().GetCurrentUser()->GetOffsideTrap() );
  } else
  {
    CTeamTactic * teamTactic = NULL;

    if( m_matchPlay->IsUserControlledClub( HOME ) )
      teamTactic = &m_matchPlay->GetTactics( HOME )->GetTeamTactic();
    else
      teamTactic = &m_matchPlay->GetTactics( AWAY )->GetTeamTactic();

    m_tacticsStyleOfPlay->SetSelectedItem( teamTactic->GetStyleOfPlay(), false );
    m_tacticsMarking->SetSelectedItem( teamTactic->GetMarkingStyle(), false );
    m_tacticsAggression->SetValuePct( (float)teamTactic->GetAggression() / 10.0f );

    m_counterattackCheckBox->SetChecked( teamTactic->GetCounterAttack() );
    m_offsidetrapCheckBox->SetChecked( teamTactic->GetOffsideTrap() );
  }

  // setup tactics

  // add formation blobs on top screen
  CTactic curTactic = WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTactics();

  if( m_matchPlay != NULL )
  {
    if( m_matchPlay->IsUserControlledClub( HOME ) )
      curTactic = *m_matchPlay->GetTactics( HOME );
    else
      curTactic = *m_matchPlay->GetTactics( AWAY );

    //NOTE: No way of getting the formation to set the combo box unless we use player
    // positions
  }

  lLogFmt("Current Tactics\n");
  lLogFmt("  %s - %d\n", curTactic.TitleStr(), curTactic.GetFamiliarity());

  lArray<int> formation;
  int curX = 0;

  for(int i = 0; i < 10; i++)
  {
    if( curTactic.GetPlayerPositionX(i) != curX )
    {
      curX = curTactic.GetPlayerPositionX(i);
      formation.Push( 0 );
    }

    formation.Last()++;
  }

  // Sprite indices
	int spriteFrame[] = { 1, 6,	12 };

  curX = 0;
  int nextInc = 0;
	for( int i = 0; i < 3; i++ )
	{
	  for( int x = 0; x < formation(i); x++ )
	  {
	    m_playerBlobList(curX++)->GetSprite()->SetFrame( spriteFrame[i] + x + nextInc );
	  }

	  nextInc += formation(i) - 3;
	}

  int totalPlayers = 0;
  for(int i = 0; i < formation.Size(); i++)
  {
    lLogFmt("%d: %d\n", i, formation(i));
    totalPlayers += formation(i);
  }

  if( totalPlayers > 10 )
  {
    lLogFmt("ERROR: %d players listed in formation (10 max)\n");
  }

  // positions for formations
  lArray<float> formationXPos;
  formationXPos.Push( 0.164f );
  formationXPos.Push( 0.406f );
  formationXPos.Push( 0.500f );
  formationXPos.Push( 0.626f );
  formationXPos.Push( 0.774f );

  lmVector2 xMin( 36, 0 );
  lmVector2 xMax( 218, 0 );

  // up to 5 players per line
  float formationYPos[] = { 0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  // 1 player
                            0.375f,  0.625f,  0.0f,  0.0f,  0.0f,  // 2 players
                            0.25f, 0.5f, 0.75f,  0.0f,  0.0f,  // 3 players
                            0.2f,  0.4f,  0.6f,  0.8f,  0.0f,  // 4 players
                            0.1f,  0.3f,  0.5f,  0.7f,  0.9f,  // 5 players
                          };

  lmVector2 yMin( 0, 43 );
  lmVector2 yMax( 0, 163 );

  // position players based on formation passed in
  int numLines = formation.Size();
  if( numLines > formationXPos.Size() )
  {
    lLogFmt("ERROR: Can't represent formation using the formation lines we have\n");
  }

  lArray<int>     lineIndex;
  lineIndex.SetSize( numLines );

  if( numLines == 1 )
  {
    lineIndex(0) = 2;
  } else
  if( numLines == 2 )
  {
    lineIndex(0) = 0;
    lineIndex(1) = 2;
  } else
  if( numLines == 3 )
  {
    lineIndex(0) = 0;
    lineIndex(1) = 2;
    lineIndex(2) = 4;
  } else
  if( numLines == 4 )
  {
    lineIndex(0) = 0;
    lineIndex(1) = 1;
    lineIndex(2) = 3;
    lineIndex(3) = 4;
  }

  int pitchOffsetY = 13;
  int playerIndex = 0;
  for(int i = 0; i < formation.Size(); i++)
  {
    for(int j = 0; j < formation(i); j++)
    {
      lmVector2 pos( 0, pitchOffsetY );
      pos += xMin + ((xMax - xMin) * formationXPos( lineIndex( i ) ));
      pos += yMin + ((yMax - yMin) * formationYPos[ ((formation(i)-1) * 5) + j ]);

      m_playerBlobList(playerIndex)->Move( lUIPoint( (int)pos(0), (int)pos(1) ) );

      playerIndex++;
    }
  }
}


void FDTacticsScreen::DoShow()
{
  m_tacticsScreen->Show();
  m_tacticsScreenSub->Show();


  int idx = WorldData().GetCurrentUser()->GetTacticsList().GetTacticsInUse();

  if( m_matchPlay != NULL )
  {
    if( m_matchPlay->IsUserControlledClub( HOME ) )
      idx = m_matchPlay->GetTacticsId( HOME );
    else
      idx = m_matchPlay->GetTacticsId( AWAY );
  }

  m_squadFormationCombo->SetSelectedItem( idx, true );

}

void FDTacticsScreen::DoHide()
{
  m_tacticsScreen->Hide();
  m_tacticsScreenSub->Hide();

  m_matchPlay = NULL;
}

void FDTacticsScreen::OnStyleSelectionChange( FDComboWidget *, int idx )
{
  lLogFmt("Style %d\n", idx);
  if( idx >= 0 && idx < (sizeof(g_styleStrID) / sizeof(g_styleStrID[0])) )
  {
    if( m_matchPlay != NULL )
    {
      CTeamTactic * curTactic = NULL;
      if( m_matchPlay->IsUserControlledClub( HOME ) )
        m_matchPlay->SetStyleOfPlay( HOME, idx );
      else
        m_matchPlay->SetStyleOfPlay( AWAY, idx );
    }
    else
    {
      WorldData().GetCurrentUser()->SetCurrentStyleOfPlay( idx );
    }
  } else
  {
    lLogFmt("Invalid selection\n");
  }
}

void FDTacticsScreen::OnMarkingSelectionChange( FDComboWidget *, int idx  )
{
  lLogFmt("Marking %d\n", idx);
  if( idx >= 0 && idx < (sizeof(g_markingStrID) / sizeof(g_markingStrID[0])) )
  {
    if( m_matchPlay != NULL )
    {
      if( m_matchPlay->IsUserControlledClub( HOME ) )
        m_matchPlay->DoSetMarkingStyle( HOME, idx );
      else
        m_matchPlay->DoSetMarkingStyle( AWAY, idx );
    }
    else
    {
      WorldData().GetCurrentUser()->SetCurrentMarkingStyle( idx );
    }
  } else
  {
    lLogFmt("Invalid selection\n");
  }
}

void FDTacticsScreen::OnAggressionChange( lUIWidget *, float val )
{
  int numVals = sizeof(g_aggressionStrID) / sizeof(g_aggressionStrID[0]);
  int selection = (float)val * numVals;

  if( selection < 0 )        selection = 0;
  if( selection >= numVals ) selection = numVals-1;

  lLogFmt("Aggression: %.2f = %d\n", val, selection);

  if( m_matchPlay != NULL )
  {
    if( m_matchPlay->IsUserControlledClub( HOME ) )
      m_matchPlay->DoSetAggression( HOME, selection );
    else
      m_matchPlay->DoSetAggression( AWAY, selection );
  }
  else
  {
    WorldData().GetCurrentUser()->SetCurrentAggression( selection );
  }
}

void FDTacticsScreen::OnCounterAttackBreak( lUIControlWidget *widget )
{
  FDCheckBoxWidget *checkbox = (FDCheckBoxWidget *)widget;

  if( m_matchPlay != NULL )
  {
    CTeamTactic * curTactic = NULL;
    if( m_matchPlay->IsUserControlledClub( HOME ) )
      curTactic = &m_matchPlay->GetTactics( HOME )->GetTeamTactic();
    else
      curTactic = &m_matchPlay->GetTactics( AWAY )->GetTeamTactic();

    if( checkbox->IsChecked() )
    {
      lLogFmt("Enabling Counter attack\n");
      curTactic->SetCounterAttack(true);
    }
    else
    {
      lLogFmt("Disabling Counter attackk\n");
      curTactic->SetCounterAttack(false);
    }

    m_matchPlay->SetTacticsChanged();
  }
  else
  {
    if( checkbox->IsChecked() )
    {
      lLogFmt("Enabling Counter attack\n");
      WorldData().GetCurrentUser()->SetCounterAttack(true);
    }
    else
    {
      lLogFmt("Disabling Counter attackk\n");
      WorldData().GetCurrentUser()->SetCounterAttack(false);
    }
  }
}

void FDTacticsScreen::OnOffsideTrap( lUIControlWidget *widget )
{
  FDCheckBoxWidget *checkbox = (FDCheckBoxWidget *)widget;

  if( m_matchPlay != NULL )
  {
    CTeamTactic * curTactic = NULL;
    if( m_matchPlay->IsUserControlledClub( HOME ) )
      curTactic = &m_matchPlay->GetTactics( HOME )->GetTeamTactic();
    else
      curTactic = &m_matchPlay->GetTactics( AWAY )->GetTeamTactic();

    if( checkbox->IsChecked() )
    {
      lLogFmt("Enabling offside trap\n");
      curTactic->SetOffsideTrap(true);
    }
    else
    {
      lLogFmt("Disabling offside trap\n");
      curTactic->SetOffsideTrap(false);
    }

    m_matchPlay->SetTacticsChanged();
  }
  else
  {
    if( checkbox->IsChecked() )
    {
      lLogFmt("Enabling offside trap\n");
      WorldData().GetCurrentUser()->SetOffsideTrap(true);
    }
    else
    {
      lLogFmt("Disabling offside trap\n");
      WorldData().GetCurrentUser()->SetOffsideTrap(false);
    }
  }
}

void FDTacticsScreen::OnFormationSelect( FDComboWidget *, int idx )
{
	lLogFmt("Formation select %d\n", idx);

  if( m_matchPlay != NULL )
  {
    if( m_matchPlay->IsUserControlledClub( HOME ) )
      m_matchPlay->DoFormationChanged( HOME, idx );
    else
      m_matchPlay->DoFormationChanged( AWAY, idx );
  }
  else
  {
    WorldData().GetCurrentUser()->GetTacticsList().SetTacticsInUse( idx );
  }

	Update();
}

void FDTacticsScreen::SetMatchDetails( CMatchPlay * matchPlay )
{
	lLogFmt("match details setting\n");

	m_matchPlay = matchPlay;
}
