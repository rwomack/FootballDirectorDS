//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDMatchDayEventsscreen.cpp
// Description : FDMatchDayEventsScreen implementation
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
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

// Add pixel clipping for bar widget, draw sprite instead of rectangle and clip at correct position

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDMatchDayEventsScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDMatchDayEventsScreen::FDMatchDayEventsScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDMatchDayEventsScreen::~FDMatchDayEventsScreen()
{
}

void FDMatchDayEventsScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them
  int colA = L_ARGB( 255, 29, 9, 208 );
  int colB = L_ARGB( 255, 211, 108, 5 );

  //  - Events screen
  lLogFmt("Init match events screen\n");
  m_matchEventsScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "match_events_screen" );
  m_matchEventsScreen->Hide();

  int barName[]  = { IDS_MDAY_CORNERS, //"CORNERS",
                     IDS_MDAY_FREEKICKS, //"FREE KICKS",
                     IDS_MDAY_SHOTSONTARGET, //"SHOTS ON TARGET",
                     IDS_MDAY_SHOTSOFFTARGET, //"SHOTS OFF TARGET",
                     IDS_MDAY_OFFSIDE, //"OFFSIDE",
                     IDS_MDAY_YELLOWCARDS, //"YELLOW CARDS",
                     IDS_MDAY_REDCARDS }; //"RED CARDS" };

  for( int i = 0; i < 7; i++ )
  {
    m_longStripes[i] = FD_GAME_NEW lUISpriteWidget( m_matchEventsScreen, 0, "longStripe", FDResourceManager::GetSprite("uiicon","grad_grey1"), LUI_LEFT_TOP,
                  lUIPoint( 20, 6 + i * 26), lUIPoint( 236, 12 ) );
  }

  for( int i = 0; i < 7; i++ )
  {
    FDBarWidget * bar = FD_GAME_NEW FDBarWidget( m_matchEventsScreen, 0, "bar", LUI_TOP_CENTER,
        lUIPoint( 138, 18 + i * 26 ), lUIPoint( 160, 12 ) );
    bar->SetColourA( colA );
    bar->SetColourB( colB );

    bar->SetPctA( 0.5f );

    lUITextWidget * tempnumA  = FD_GAME_NEW lUITextWidget( m_matchEventsScreen, 0, "homeplayerlabel", lUIText(""), LUI_RIGHT_TOP,
        lUIPoint( 47, 18 + i * 26));

    lUITextWidget * temptext  = FD_GAME_NEW lUITextWidget( m_matchEventsScreen, 0, "awayplayerlabel", lUIText( barName[i] ), LUI_CENTER_TOP,
        lUIPoint( 138, 6 + i * 26));

    lUITextWidget * tempnumB  = FD_GAME_NEW lUITextWidget( m_matchEventsScreen, 0, "awayplayerlabel", lUIText(""), LUI_LEFT_TOP,
        lUIPoint( 233, 18 + i * 26));

    m_barList.Add(1);
    m_barList.Last().m_bar = bar;
    m_barList.Last().m_barText = temptext;
    m_barList.Last().m_barA = tempnumA;
    m_barList.Last().m_barB = tempnumB;
  }
}

void FDMatchDayEventsScreen::DoShow()
{
  m_matchEventsScreen->Show();
}

void FDMatchDayEventsScreen::DoHide()
{
  m_matchEventsScreen->Hide();
}

void FDMatchDayEventsScreen::Update( CFixture &Fixture )
{
  CMatchEventsStats& HomeEvents =  Fixture.GetClubData(HOME);
  CMatchEventsStats& AwayEvents =  Fixture.GetClubData(AWAY);

  // set team names
  CString str;

  uint TextColour = 0;
  uint colA = Fixture.GetClub(HOME).GetHomeKit().GetPrimaryColour()-1;
  uint colB = Fixture.GetClub(AWAY).GetHomeKit().GetPrimaryColour()-1;

  // check for bright colours
  //TextColour = GetInverseColour( colA, colB );

  for( int i = 0; i < m_barList.Size(); i++ )
  {
    m_barList(i).m_bar->SetColourA( GetKitColour( colA ) );
    m_barList(i).m_bar->SetColourB( GetAwayColour( GetKitColour( colA ) )  );//GetKitColour( colB ) );

    //m_barList(i).m_barA->SetTextColour( GetKitColour( Fixture.GetClub(HOME).GetHomeKit().GetSecondaryColour()-1 ) );
    //m_barList(i).m_barB->SetTextColour( GetKitColour( Fixture.GetClub(HOME).GetHomeKit().GetSecondaryColour()-1 ) );

    //m_barList(i).m_barText->SetTextColour( TextColour );
  }

  // set  bars
  char strnum[8];

  float pct = 0.5f;

  pct = 0.5f;
  if( (HomeEvents.GetCorners() + AwayEvents.GetCorners()) > 0 )
    pct = (float)HomeEvents.GetCorners() / (float)(HomeEvents.GetCorners() + AwayEvents.GetCorners());
  m_barList(0).m_bar->SetPctA( pct );
  sprintf(strnum, "%d", HomeEvents.GetCorners());
  m_barList(0).m_barA->SetText( lUIText( strnum ) );
  sprintf(strnum, "%d", AwayEvents.GetCorners());
  m_barList(0).m_barB->SetText( lUIText( strnum ) );

  pct = 0.5f;
  if( (HomeEvents.GetFreeKicks() + AwayEvents.GetFreeKicks()) > 0 )
    pct = (float)HomeEvents.GetFreeKicks() / (float)(HomeEvents.GetFreeKicks() + AwayEvents.GetFreeKicks());
  m_barList(1).m_bar->SetPctA( pct );
  sprintf(strnum, "%d", HomeEvents.GetFreeKicks());
  m_barList(1).m_barA->SetText( lUIText( strnum ) );
  sprintf(strnum, "%d", AwayEvents.GetFreeKicks());
  m_barList(1).m_barB->SetText( lUIText( strnum ) );

  pct = 0.5f;
  if( (HomeEvents.GetShotsOnTarget() + AwayEvents.GetShotsOnTarget()) > 0 )
    pct = (float)HomeEvents.GetShotsOnTarget() / (float)(HomeEvents.GetShotsOnTarget() + AwayEvents.GetShotsOnTarget());
  m_barList(2).m_bar->SetPctA( pct );
  sprintf(strnum, "%d", HomeEvents.GetShotsOnTarget());
  m_barList(2).m_barA->SetText( lUIText( strnum ) );
  sprintf(strnum, "%d", AwayEvents.GetShotsOnTarget());
  m_barList(2).m_barB->SetText( lUIText( strnum ) );

  pct = 0.5f;
  if( (HomeEvents.GetShotsOffTarget() + AwayEvents.GetShotsOffTarget()) > 0 )
    pct = (float)HomeEvents.GetShotsOffTarget() / (float)(HomeEvents.GetShotsOffTarget() + AwayEvents.GetShotsOffTarget());
  m_barList(3).m_bar->SetPctA( pct );
  sprintf(strnum, "%d", HomeEvents.GetShotsOffTarget());
  m_barList(3).m_barA->SetText( lUIText( strnum ) );
  sprintf(strnum, "%d", AwayEvents.GetShotsOffTarget());
  m_barList(3).m_barB->SetText( lUIText( strnum ) );

  pct = 0.5f;
  if( (HomeEvents.GetOffsides() + AwayEvents.GetOffsides()) > 0 )
    pct = (float)HomeEvents.GetOffsides() / (float)(HomeEvents.GetOffsides() + AwayEvents.GetOffsides());
  m_barList(4).m_bar->SetPctA( pct );
  sprintf(strnum, "%d", HomeEvents.GetOffsides());
  m_barList(4).m_barA->SetText( lUIText( strnum ) );
  sprintf(strnum, "%d", AwayEvents.GetOffsides());
  m_barList(4).m_barB->SetText( lUIText( strnum ) );

  pct = 0.5f;
  if( (Fixture.GetNumberOfPlayersBooked(HOME) + Fixture.GetNumberOfPlayersBooked(AWAY)) > 0 )
    pct = (float)Fixture.GetNumberOfPlayersBooked(HOME) / (float)(Fixture.GetNumberOfPlayersBooked(HOME) + Fixture.GetNumberOfPlayersBooked(AWAY));
  m_barList(5).m_bar->SetPctA( pct );
  sprintf(strnum, "%d", Fixture.GetNumberOfPlayersBooked(HOME));
  m_barList(5).m_barA->SetText( lUIText( strnum ) );
  sprintf(strnum, "%d", Fixture.GetNumberOfPlayersBooked(AWAY));
  m_barList(5).m_barB->SetText( lUIText( strnum ) );

  pct = 0.5f;
  if( (Fixture.GetNumberOfPlayersSentOff(HOME) + Fixture.GetNumberOfPlayersSentOff(AWAY)) > 0 )
    pct = (float)Fixture.GetNumberOfPlayersSentOff(HOME) / (float)(Fixture.GetNumberOfPlayersSentOff(HOME) + Fixture.GetNumberOfPlayersSentOff(AWAY));
  m_barList(6).m_bar->SetPctA( pct );
  sprintf(strnum, "%d", Fixture.GetNumberOfPlayersSentOff(HOME));
  m_barList(6).m_barA->SetText( lUIText( strnum ) );
  sprintf(strnum, "%d", Fixture.GetNumberOfPlayersSentOff(AWAY));
  m_barList(6).m_barB->SetText( lUIText( strnum ) );
}

