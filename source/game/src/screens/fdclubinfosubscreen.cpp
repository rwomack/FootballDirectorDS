//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDClubInfoscreen.cpp
// Description : FDClubInfoSubScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdclubinfosubscreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDClubInfoSubScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDClubInfoSubScreen::FDClubInfoSubScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDClubInfoSubScreen::~FDClubInfoSubScreen()
{
}

void FDClubInfoSubScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them
  m_clubScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "player_screen_sub" );
  m_clubScreenSub->Hide();

  m_screenBacking = FD_GAME_NEW FDBackingWidget( m_clubScreenSub, 0, "back", L_ARGB( 127, 80, 80, 80 ), LUI_TOP_LEFT, lUIPoint(0, 40), lUIPoint( 256, 152) );
  m_screenBacking->Hide();

  lUISpriteWidget *teamKitBG1 = FD_GAME_NEW lUISpriteWidget( m_clubScreenSub, LUI_WIDGET_MOUSETRANSPARENT, "kitbg", FDResourceManager::GetSprite("uiicon","shirt_holder"), LUI_TOP_LEFT, lUIPoint( 5, 43 ) );
  teamKitBG1->GetSprite()->SetAlphaEnable( true );

  lUIPoint pt = teamKitBG1->WidgetClientToScreen( lUIPoint( 5, 8 ) );
  m_teamKit = FD_GAME_NEW lUISpriteWidget( m_clubScreenSub, LUI_WIDGET_MOUSETRANSPARENT, "kit", FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_TOP_LEFT, pt );

  lUISpriteWidget *teamBadge = FD_GAME_NEW lUISpriteWidget( m_clubScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );
  m_badgeStripe1 = FD_GAME_NEW lUISpriteWidget( m_clubScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );
  m_badgeStripe2 = FD_GAME_NEW lUISpriteWidget( m_clubScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );

  m_teamName = FD_GAME_NEW lUITextWidget( m_clubScreenSub, 0, "teamname", lUIText(""), LUI_CENTER_CENTER, lUIPointPercent( 50, 27 ) ,lUIPoint( 127, 16 ) );
  m_teamName->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_teamName->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  FDBarWidget * splitterBar = FD_GAME_NEW FDBarWidget( m_clubScreenSub, 0, "line", LUI_CENTER_CENTER, lUIPointPercent( 50, 33 ), lUIPoint( 155, 1 ) );

  splitterBar->SetPctA(0.0f);
  splitterBar->SetColourB(L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));

  m_managerName = FD_GAME_NEW lUITextWidget( m_clubScreenSub, 0, "managername", lUIText(""), LUI_CENTER_CENTER, lUIPointPercent( 50, 39 ) );
  m_managerName->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_managerName->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  lUISpriteWidget* m_bar = FD_GAME_NEW lUISpriteWidget( m_clubScreenSub, 0, "bar", FDResourceManager::GetSprite("uiicon","grad_grey1"), LUI_TOP_LEFT,
    lUIPoint( 0, 90 ), lUIPoint( 255, 12) );
  lUITextWidget * m_teamLineUpLabel = FD_GAME_NEW lUITextWidget( m_clubScreenSub, 0, "label", lUIText("Team Line Up"), LUI_TOP_CENTER,
    lUIPoint( 128, 90 ) );
  m_teamLineUpLabel->SetTextToUpper();

  m_playerTable = FD_GAME_NEW FDTableWidget(m_clubScreenSub, 0, "playertable",
  FDTableWidget::TABLE_FLAG_CELLBACKGROUND | FDTableWidget::TABLE_FLAG_CUT_OFF /*|FDTableWidget::TABLE_FLAG_BACKGROUND*/,
  LUI_TOP_LEFT, lUIPoint(0, 104), lUIPoint(256, 156));
	m_playerTable->SetRowGap( 2 );

}

void FDClubInfoSubScreen::DoShow()
{
  lLogFmt("ClubInfo: DoShow\n");
  m_clubScreenSub->Show();
}

void FDClubInfoSubScreen::DoHide()
{
  lLogFmt("ClubInfo: DoHide\n");
  m_clubScreenSub->Hide();
}

void FDClubInfoSubScreen::SetClubInfo( CClub * club )
{
  lLogFmt("SetClubInfo %x\n", club);

  if( club == NULL )
  {
    m_teamName->SetText(lUIText(""));
		m_teamKit->Hide();
    m_badgeStripe1->Hide();
    m_badgeStripe2->Hide();
		m_managerName->SetText(lUIText(""));
    return;
  }

  m_teamName->Show();
  m_managerName->Show();

  m_playerTable->ClearAll();
	m_playerTable->AddColumn(lUICoord(5), LUI_CENTER_LEFT); //gap
	m_playerTable->AddColumn(lUICoord(15), LUI_CENTER_CENTER); // number
	m_playerTable->AddColumn(lUICoord(5), LUI_CENTER_LEFT);  //gap
	m_playerTable->AddColumn(lUICoord(98), LUI_CENTER_LEFT); // name
	m_playerTable->AddColumn(lUICoord(5), LUI_CENTER_LEFT);  //gap
	m_playerTable->AddColumn(lUICoord(15), LUI_CENTER_CENTER); // number
	m_playerTable->AddColumn(lUICoord(5), LUI_CENTER_LEFT);  //gap
	m_playerTable->AddColumn(lUICoord(98), LUI_CENTER_LEFT); // name
	m_playerTable->AddColumn(lUICoord(15), LUI_CENTER_LEFT);  //gap

  // team name
  m_teamName->SetText( lUIText( club->GetName() ), true );

  m_teamKit->GetSprite().Get()->SetFrame( club->GetHomeKitID() );

  char tmp[32];

  sprintf(tmp, "SHIELD_STRIPE1_%s", GetBadgeSpritePostfix( club->GetHomeKit().GetPrimaryColour()-1 ) );
  m_badgeStripe1->SetSprite( FDResourceManager::GetSprite( "badge", tmp ) );

  sprintf(tmp, "SHIELD_STRIPE2_%s", GetBadgeSpritePostfix( club->GetHomeKit().GetSecondaryColour()-1 ) );
  m_badgeStripe2->SetSprite( FDResourceManager::GetSprite( "badge", tmp ) );

  m_teamKit->Show();
  m_badgeStripe1->Show();
  m_badgeStripe2->Show();

  // manager name
  m_managerName->SetText( lUIText( club->GetManagerName() ) );

  // Starting players.
  const int iPlayerCount = lmMin(club->GetFirstTeamSquad().TotalSquadSize(), (ushort)PLAYERSINTEAM);
  const int iNumPlayerRows = (iPlayerCount + 1) / 2;
  m_playerTable->AddRows(iNumPlayerRows);

  for( int i = 0; i < iNumPlayerRows; i++ )
  {
    m_playerTable->SetBkgColour( i, 0, L_ARGB( 127, 130, 130, 130 ) );
    m_playerTable->SetBkgColour( i, 1, L_ARGB( 127, 80, 80, 80 ) );
    m_playerTable->SetBkgColour( i, 2, L_ARGB( 127, 130, 130, 130 ) );
    m_playerTable->SetBkgColour( i, 3, L_ARGB( 127, 80, 80, 80 ) );
    m_playerTable->SetBkgColour( i, 4, L_ARGB( 127, 130, 130, 130 ) );
    m_playerTable->SetBkgColour( i, 5, L_ARGB( 127, 80, 80, 80) );
    m_playerTable->SetBkgColour( i, 6, L_ARGB( 127, 130, 130, 130 ) );
    m_playerTable->SetBkgColour( i, 7, L_ARGB( 127, 80, 80, 80 ) );
    m_playerTable->SetBkgColour( i, 8, L_ARGB( 127, 130, 130, 130 ) );
  }

  for (int i = 0; i < iPlayerCount; i++)
  {
    if( club->GetPlayerID(i) == NOPLAYER )
      continue;

    CPlayer* pPlayer = &club->GetPlayer(i);
    char pcBuffer[128];
    ::sprintf(pcBuffer, "%d", i + 1);

    const int iRowToUse = (i >= iNumPlayerRows) ? (i - iNumPlayerRows) : i;
    const int iCol1ToUse = (i >= iNumPlayerRows) ? 5 : 1;
    const int iCol2ToUse = (i >= iNumPlayerRows) ? 7 : 3;

    m_playerTable->SetText(iRowToUse, iCol1ToUse, lUIText(pcBuffer));
    m_playerTable->SetText(iRowToUse, iCol2ToUse, lUIText(pPlayer->GetName()));
  }
}

void FDClubInfoSubScreen::SetBacking( bool show )
{
  if( show )
  {
    m_screenBacking->Show();
  }
  else
  {
    m_screenBacking->Hide();
  }
}
