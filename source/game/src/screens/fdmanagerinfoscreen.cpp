//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdmanagerinfoscreen.cpp
// Description : FDManagerInfoScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdmanagerinfoscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDManagerInfoScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDManagerInfoScreen::FDManagerInfoScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
  m_currentManager = NULL;
}

FDManagerInfoScreen::~FDManagerInfoScreen()
{
}

void FDManagerInfoScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_managerScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "manager_screen_sub" );
  m_managerScreenSub->Hide();

  // Manager information
  // Stripes + Bars

  lUISpriteWidget *kitBG = FD_GAME_NEW lUISpriteWidget( m_managerScreenSub, 0, "kitbg",  FDResourceManager::GetSprite("uiicon","shirt_holder"), LUI_TOP_LEFT, lUIPoint( 5, 43 ) );
  kitBG->GetSprite()->SetAlphaEnable( true );

  m_kit   = FD_GAME_NEW lUISpriteWidget( m_managerScreenSub, 0, "kit",  FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_TOP_LEFT, lUIPoint( 10, 51 ) );

  lUISpriteWidget *teamBadge = FD_GAME_NEW lUISpriteWidget( m_managerScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );
  m_badgeStripe1 = FD_GAME_NEW lUISpriteWidget( m_managerScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );
  m_badgeStripe2 = FD_GAME_NEW lUISpriteWidget( m_managerScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );


  for( int i = 0; i < 3; i++ )
  {
    m_managerStripes[i] = FD_GAME_NEW lUISpriteWidget(m_managerScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
     lUIPoint( 0, 120 + i * 14 ), lUIPoint(256, 12));
	  m_managerStripes[i]->GetSprite()->SetWidth( 256 );

    m_managerBars[i] = FD_GAME_NEW FDBarWidget(m_managerScreenSub, 0, "bar", LUI_TOP_LEFT,
     lUIPoint( 137,120 + i * 14 ), lUIPoint(80, 12));
  }

  // Labels + Data
  for( int i = 0; i < 3; i++ )
  {
    m_managerInfoLabel[i] = FD_GAME_NEW lUITextWidget( m_managerScreenSub, 0, "label", lUIText(""), LUI_TOP_LEFT,
      lUIPoint( 0, 120 + i * 14 ), lUIPoint( 127, 12  ) );
    m_managerInfoData[i] = FD_GAME_NEW lUITextWidget( m_managerScreenSub, 0, "data", lUIText(""), LUI_TOP_LEFT,
      lUIPoint( 140,120 + i * 14 ), lUIPoint(80, 12) );
  }

   for( int i = 0; i < 3; i++ )
  {
	  m_managerInfoLabel[i]->SetAlign(LUI_CENTER_RIGHT);
	  m_managerInfoData[i]->SetAlign(LUI_CENTER_CENTER);

	  m_managerBars[i]->SetPctA( 0.0f );
	  m_managerBars[i]->SetColourB( L_ARGB( 128, 80, 80, 80 )) ;
  }

  //
  m_managerInfoName = FD_GAME_NEW lUITextWidget( m_managerScreenSub, 0, "namepos", lUIText(""), LUI_TOP_CENTER, lUIPoint( 127, 47 ), lUIPoint( 127, 16 ) );
  m_managerInfoName->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_managerInfoName->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );
  m_managerInfoPos = FD_GAME_NEW lUITextWidget( m_managerScreenSub, 0, "namepos", lUIText(""), LUI_TOP_CENTER, lUIPoint( 127, 70 ), lUIPoint( 127, 16 ) );
  m_managerInfoPos->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_managerInfoPos->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  FDBarWidget * splitterBar = FD_GAME_NEW FDBarWidget( m_managerScreenSub, 0, "line", LUI_CENTER_CENTER, lUIPointPercent( 50, 34 ), lUIPoint( 155, 1 ) );
  splitterBar->SetPctA(0.0f);
  splitterBar->SetColourB(L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));
}

void FDManagerInfoScreen::DoShow()
{
  m_managerScreenSub->Show();
}

void FDManagerInfoScreen::DoHide()
{
  m_managerScreenSub->Hide();
}
void FDManagerInfoScreen::SetManagerInfo( CManager *manager )
{
  lLogFmt("SetManagerInfo\n");

  m_currentManager = manager;

  if( m_currentManager == NULL )
  {
    m_managerInfoName->SetText( lUIText( IDS_NO_MANAGER_SET ) );
    return;
  }

  if( m_currentManager->IsManagingClub() )
  {
    char str[64];

    // name
    m_managerInfoName->SetText( lUIText( manager->GetName() ), true );
    m_managerInfoPos->SetText( lUIText( manager->GetClubName() ), true );

    lUIText temp( IDS_NUM_GAMES ) ; temp.ToUpper();
    m_managerInfoLabel[0]->SetText( temp );
    temp = lUIText( IDS_BOARDCONFIDENCE_SH ) ; temp.ToUpper();
    m_managerInfoLabel[1]->SetText( temp );
    temp = lUIText( IDS_FANSCONFIDENCE_SH ) ; temp.ToUpper();
    m_managerInfoLabel[2]->SetText( temp );

    //kit
    m_kit->GetSprite().Get()->SetFrame( manager->GetCurrentClub()->GetHomeKitID() );
    m_kit->Show();

    if( manager->GetCurrentClub()->GetID() != NOCLUB )
    {
      m_badgeStripe1->Show();
      m_badgeStripe2->Show();
      sprintf(str, "SHIELD_STRIPE1_%s", GetBadgeSpritePostfix( manager->GetCurrentClub()->GetHomeKit().GetPrimaryColour()-1 ) );
      m_badgeStripe1->SetSprite( FDResourceManager::GetSprite( "badge", str ) );

      sprintf(str, "SHIELD_STRIPE2_%s", GetBadgeSpritePostfix( manager->GetCurrentClub()->GetHomeKit().GetSecondaryColour()-1 ) );
      m_badgeStripe2->SetSprite( FDResourceManager::GetSprite( "badge", str ) );
    }

    // Set these as coloured bars?
    sprintf(str, "%d", manager->GetGamesPlayedClub());
    m_managerInfoData[0]->SetText( lUIText( str ) );            //m_managerInfoTable->SetTextColour( rowidx, 1, L_ARGB( 255, 0, 255, 0 ) );

    sprintf(str, "%d%%", manager->GetBoardConfidence());
    m_managerInfoData[1]->SetText( lUIText( str ) );     //m_managerInfoTable->SetTextColour( rowidx, 1, L_ARGB( 255, 0, 255, 0 ) );

    sprintf(str, "%d%%", manager->GetSupportersConfidence());
    m_managerInfoData[2]->SetText( lUIText( str ) );      //m_managerInfoTable->SetTextColour( rowidx, 1, L_ARGB( 255, 0, 255, 0 ) );
  } else
  {
    char str[32];

    // name
    sprintf(str, "%s", manager->GetName());
    m_managerInfoName->SetText( lUIText(str) );

    m_kit->Hide();
    m_badgeStripe1->Hide();
    m_badgeStripe2->Hide();
  }
}
