//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDMatchDaySkillscreen.cpp
// Description : FDMatchDaySkillScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdmatchdayscreen.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDMatchDaySkillScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDMatchDaySkillScreen::FDMatchDaySkillScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDMatchDaySkillScreen::~FDMatchDaySkillScreen()
{
}

void FDMatchDaySkillScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  int colA = L_ARGB( 255, 29, 9, 208 );
  int colB = L_ARGB( 255, 211, 108, 5 );

  //  - Skill screen

  {
    lLogFmt("Skill\n");
    m_matchSkillScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "match_skill_screen" );
    m_matchSkillScreen->Hide();

    for( int i = 0; i < 2; i++ )
    {
      m_longStripes[i] = FD_GAME_NEW lUISpriteWidget( m_matchSkillScreen, 0, "longStripe", FDResourceManager::GetSprite("uiicon","grad_grey1"), LUI_LEFT_TOP,
                    lUIPoint( 20, 38 + i * 14), lUIPoint( 236, 12 ) );
    }

    for( int i = 2; i < 7; i++ )
    {
      m_longStripes[i] = FD_GAME_NEW lUISpriteWidget( m_matchSkillScreen, 0, "longStripe", FDResourceManager::GetSprite("uiicon","grad_grey1"), LUI_LEFT_TOP,
                    lUIPoint( 20, 66 + i * 14), lUIPoint( 236, 12 ) );
    }

    float playPercentageA = 0.5f;
    float playPercentageB = 0.5f;

    m_possessionBar = FD_GAME_NEW FDBarWidget( m_matchSkillScreen, 0, "bar", LUI_TOP_CENTER, lUIPoint( 138, 52 ), lUIPoint( 138, 12 ) );
    m_possessionBar->SetColourA( colA );
    m_possessionBar->SetColourB( colB );
    m_possessionBar->SetPctA( playPercentageA );

    // draw text over
    m_bartext = FD_GAME_NEW lUITextWidget( m_matchSkillScreen, 0, "bartext", lUIText( IDS_MSTATPERCENT_CAP ), LUI_TOP_CENTER, lUIPoint( 138, 38 ) );
    //m_bartext->SetFont( FDResourceManager::GetFont("font10", "font10" ) );
    m_bartext->SetTextColour( L_ARGB( 255, 0, 0, 0 ) );

    char strnum[8];
    sprintf(strnum, "%d%%", (int)(playPercentageA*100.0f));
    m_possA = FD_GAME_NEW lUITextWidget( m_matchSkillScreen, 0, "bartextA", lUIText(strnum), LUI_CENTER_CENTER, lUIPoint( 138 - 88, 58 ) );
    sprintf(strnum, "%d%%", (int)(playPercentageB*100.0f));
    m_possB = FD_GAME_NEW lUITextWidget( m_matchSkillScreen, 0, "bartextB", lUIText(strnum), LUI_CENTER_CENTER, lUIPoint( 138 + 93, 58 ) );


    lUITextWidget * posstext = FD_GAME_NEW lUITextWidget( m_matchSkillScreen, 0, "skilltext", lUIText( IDS_SKILL_SINGLE_CAP ), LUI_TOP_CENTER, lUIPoint( 138, 94 + 0* 14  ) );
    lUITextWidget * gktext   = FD_GAME_NEW lUITextWidget( m_matchSkillScreen, 0, "gktext",  lUIText( IDS_GOALIE_CAP ),     LUI_TOP_CENTER, lUIPoint( 138,  94 + 1 * 14  ) );
    lUITextWidget * deftext  = FD_GAME_NEW lUITextWidget( m_matchSkillScreen, 0, "deftext", lUIText( IDS_DEFENSE_CAP ),    LUI_TOP_CENTER, lUIPoint( 138, 94 + 2 * 14  ) );
    lUITextWidget * midtext  = FD_GAME_NEW lUITextWidget( m_matchSkillScreen, 0, "midtext", lUIText( IDS_MIDFIELD_CAP ),   LUI_TOP_CENTER, lUIPoint( 138, 94 + 3 * 14  ) );
    lUITextWidget * atktext  = FD_GAME_NEW lUITextWidget( m_matchSkillScreen, 0, "atktext", lUIText( IDS_ATTACK_CAP ),     LUI_TOP_CENTER, lUIPoint( 138, 94 + 4 * 14  ) );

    for(int i = 0; i < 4; i++)
    {
      m_skillBars[i * 2 + 0] = FD_GAME_NEW FDBarWidget( m_matchSkillScreen, 0, "bar", LUI_TOP_RIGHT,
        lUIPoint( 108, 108 + i * 14 ), lUIPoint( 80, 12 ) );
      m_skillBars[i * 2 + 0]->SetColourA( L_ARGB(127, 80, 80, 80 ) );
      m_skillBars[i * 2 + 0]->SetColourB( PosnColour[i] );
      m_skillBars[i * 2 + 0]->SetPctA( 1.0f );

      m_skillBars[i * 2 + 1] = FD_GAME_NEW FDBarWidget( m_matchSkillScreen, 0, "bar", LUI_TOP_LEFT,
        lUIPoint( 164, 108 + i * 14 ), lUIPoint( 80, 12 ) );
      m_skillBars[i * 2 + 1]->SetColourA( PosnColour[i] );
      m_skillBars[i * 2 + 1]->SetColourB( L_ARGB(127, 80, 80, 80 ) );
      m_skillBars[i * 2 + 1]->SetPctA( 0.0f );
    }
  }
}

void FDMatchDaySkillScreen::DoShow()
{
  m_matchSkillScreen->Show();
}

void FDMatchDaySkillScreen::DoHide()
{
  m_matchSkillScreen->Hide();
}

void FDMatchDaySkillScreen::Update( CFixture &Fixture )
{
  int colA = Fixture.GetClub(HOME).GetHomeKit().GetPrimaryColour()-1;
  int colB = Fixture.GetClub(AWAY).GetHomeKit().GetPrimaryColour()-1;

  // set skill bars
  float skillA[4];
  float skillB[4];

  skillA[0] = Fixture.GetGoalKeeperSkill(HOME) / 100.0f;
  skillB[0] = Fixture.GetGoalKeeperSkill(AWAY) / 100.0f;

  skillA[1] = Fixture.GetDefenderSkill(HOME) / 100.0f;
  skillB[1] = Fixture.GetDefenderSkill(AWAY) / 100.0f;

  skillA[2] = Fixture.GetMidfielderSkill(HOME) / 100.0f;
  skillB[2] = Fixture.GetMidfielderSkill(AWAY) / 100.0f;

  skillA[3] = Fixture.GetAttackerSkill(HOME) / 100.0f;
  skillB[3] = Fixture.GetAttackerSkill(AWAY) / 100.0f;

  for(int i = 0; i < 4; i++)
  {
    m_skillBars[i * 2 + 0]->SetPctA( 1.0f - skillA[i] );

    m_skillBars[i * 2 + 1]->SetPctA( skillB[i] );
  }

  // set bar percentage
  float playPercentageA = (float)Fixture.GetPercentageOfPlay(HOME) / 100.0f;
  float playPercentageB = (float)Fixture.GetPercentageOfPlay(AWAY) / 100.0f;

  //lLogFmt("\nKit Colour Home %d\n", Fixture.GetClub(HOME).GetHomeKit().GetPrimaryColour() );
  //lLogFmt("\nKit Colour Away %d\n", Fixture.GetClub(AWAY).GetHomeKit().GetPrimaryColour() );

  m_possessionBar->SetColourA( GetKitColour( colA ) );
  m_possessionBar->SetColourB( GetAwayColour( GetKitColour( colA ) ) );//GetKitColour( colB ) );
  m_possessionBar->SetPctA( playPercentageA );

  uint TextColour = GetInverseColour( colA, colA );
  m_bartext->SetTextColour( TextColour ) ;

  // draw text over bar
  char strnum[8];
  sprintf(strnum, "%d%%", (int)(playPercentageA*100.0f));
  m_possA->SetText( lUIText( strnum ) );
  sprintf(strnum, "%d%%", (int)(playPercentageB*100.0f));
  m_possB->SetText( lUIText( strnum ) );

}
