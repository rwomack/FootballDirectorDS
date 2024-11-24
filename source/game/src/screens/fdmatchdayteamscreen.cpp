//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDMatchDayTeamscreen.cpp
// Description : FDMatchDayTeamScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>
#include <fdtext.h>

#include <screens/fdmatchdayscreen.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDMatchDayTeamScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDMatchDayTeamScreen::FDMatchDayTeamScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDMatchDayTeamScreen::~FDMatchDayTeamScreen()
{
}

void FDMatchDayTeamScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  int colA = L_ARGB( 255, 29, 9, 208 );
  int colB = L_ARGB( 255, 211, 108, 5 );

  //  - Team sheet screen
  m_matchTeamScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "match_team_screen" );
  m_matchTeamScreen->Hide();

  m_teamstable = FD_GAME_NEW FDTableWidget( m_matchTeamScreen, 0, "teamtable",
       FDTableWidget::TABLE_FLAG_CELLBACKGROUND | FDTableWidget::TABLE_FLAG_CUT_OFF,
            LUI_TOP_LEFT, lUIPoint( 20, 20 ), lUIPoint( 256 - 20, 192-20 ) );

  m_teamstable->AddColumn( lUICoord( 92.0f ), LUI_RIGHT_CENTER );
  m_teamstable->AddColumn( lUICoord( 15.0f ) );
  m_teamstable->AddColumn( lUICoord( 24.0f ) );
  m_teamstable->AddColumn( lUICoord( 15.0f ) );
  m_teamstable->AddColumn( lUICoord( 92.0f ), LUI_LEFT_CENTER );

  m_teamstable->AddRows( PLAYERSINTEAM + 1 );
}

void FDMatchDayTeamScreen::DoShow()
{
  m_matchTeamScreen->Show();
}

void FDMatchDayTeamScreen::DoHide()
{
  m_matchTeamScreen->Hide();
}

void FDMatchDayTeamScreen::Update( CMatchPlay* _MatchPlay )
{
  char strnum[12];

  // headers
  m_teamstable->SetRowBkgColour( 0, L_ARGB(255, 35, 35, 35) );
  m_teamstable->SetText( 0, 0, lUIText("HOME") );
  m_teamstable->SetText( 0, 4, lUIText("AWAY") );

  // team
  for(int i = 0; i < PLAYERSINTEAM; i++)
  {
    CPlayer* pPlayer = &_MatchPlay->GetPlayer(HOME, i);

    m_teamstable->SetBkgColour( i+1, 2, L_ARGB(80, 0x50, 0x50, 0x50) );
    m_teamstable->SetBkgColour( i+1, 1, L_ARGB(80, 0x70, 0x70, 0x70) );
    m_teamstable->SetBkgColour( i+1, 3, L_ARGB(80, 0x70, 0x70, 0x70) );

    // team 1
    m_teamstable->SetBkgColour( i+1, 0, PosnColour[pPlayer->GetSkills().GetPosition()] );

    m_teamstable->SetText( i+1, 0, lUIText( FDMakePlayerNameLink(pPlayer) ) );  // Link to player

    sprintf( strnum, "%d", _MatchPlay->GetPlayerSkill(HOME, i));
    m_teamstable->SetText( i+1, 1, lUIText( strnum ) );

    sprintf( strnum, "%d", i+1);
    m_teamstable->SetText( i+1, 2, lUIText( strnum ) );

    // team 2
    pPlayer = &_MatchPlay->GetPlayer(AWAY, i);

    m_teamstable->SetBkgColour( i+1, 4, PosnColour[pPlayer->GetSkills().GetPosition()] );

    sprintf( strnum, "%d", _MatchPlay->GetPlayerSkill(AWAY, i));
    m_teamstable->SetText( i+1, 3, lUIText( strnum ) );

    m_teamstable->SetText( i+1, 4, lUIText( FDMakePlayerNameLink(pPlayer) ) );  // Link to player
  }
}


