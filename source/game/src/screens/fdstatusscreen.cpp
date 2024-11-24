//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDStatusscreen.cpp
// Description : FDStatusScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdstatusscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDStatusScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDStatusScreen::FDStatusScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDStatusScreen::~FDStatusScreen()
{
}

void FDStatusScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_statusScreen = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "status_screen" );
  m_statusScreen->Hide();

  // create text
  m_teamName = FD_GAME_NEW lUITextWidget( m_statusScreen, 0, "temp1", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 128, 53 ), lUIPoint( 127, 16 ) );
  m_teamName->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_teamName->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  m_managerName = FD_GAME_NEW lUITextWidget( m_statusScreen, 0, "temp1", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 128, 78 ) );
  m_managerName->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_managerName->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_managerName->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  FDBarWidget * splitterBar = FD_GAME_NEW FDBarWidget( m_statusScreen, 0, "line", LUI_CENTER_CENTER, lUIPointPercent( 50, 34 ), lUIPoint( 155, 1 ) );
  splitterBar->SetPctA(0.0f);
  splitterBar->SetColourB(L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));

  // kit / badge
  lUISpriteWidget *teamKitBG = FD_GAME_NEW lUISpriteWidget( m_statusScreen, LUI_WIDGET_MOUSETRANSPARENT, "kitbg", FDResourceManager::GetSprite("uiicon","shirt_holder"), LUI_TOP_LEFT, lUIPoint( 5, 43 ) );
  teamKitBG->GetSprite()->SetAlphaEnable( true );
  m_teamKit = FD_GAME_NEW lUISpriteWidget( m_statusScreen, LUI_WIDGET_MOUSETRANSPARENT, "kit", FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_TOP_LEFT, lUIPoint( 10, 51 ) );

  lUISpriteWidget *teamBadge = FD_GAME_NEW lUISpriteWidget( m_statusScreen, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );

  m_badgeStripe1 = FD_GAME_NEW lUISpriteWidget( m_statusScreen, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );
  m_badgeStripe2 = FD_GAME_NEW lUISpriteWidget( m_statusScreen, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );


  // fields

  // Add background stripes.
  for (int i = 0; i < SFT_MAX; i++)
  {
    m_backgroundStripes[i] = FD_GAME_NEW lUISpriteWidget(m_statusScreen, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
		  lUIPoint(0, 91 + (14 * i)), lUIPoint(256, 12));
	  m_backgroundStripes[i]->GetSprite()->SetWidth( 256 );

    if( i == SFT_FANCONFIDENCE || i == SFT_BOARDCONFIDENCE )
    {
      m_fieldStripes[i] = FD_GAME_NEW FDBarWidget(m_statusScreen, 0, "fieldstripe", LUI_TOP_LEFT,
        lUIPoint(128, 91 + (14 * i)), lUIPoint(50, 12));

      m_fieldValues[i] = FD_GAME_NEW lUITextWidget(m_statusScreen, 0, "value", lUIText(""), LUI_TOP_LEFT,
        lUIPoint(128, 91 + (14 * i)), lUIPoint(50, 12));

    } else
    {
      m_fieldStripes[i] = FD_GAME_NEW FDBarWidget(m_statusScreen, 0, "fieldstripe", LUI_TOP_LEFT,
        lUIPoint(128, 91 + (14 * i)), lUIPoint(110, 12));

      m_fieldValues[i] = FD_GAME_NEW lUITextWidget(m_statusScreen, 0, "value", lUIText(""), LUI_TOP_LEFT,
        lUIPoint(128, 91 + (14 * i)), lUIPoint(110, 12));

      m_fieldStripes[i]->SetPctA(0.0f);
      m_fieldStripes[i]->SetColourB(L_ARGB(0xFF, 0x50, 0x50, 0x50));
    }

    m_fieldLabels[i] = FD_GAME_NEW lUITextWidget(m_statusScreen, 0, "label", "", LUI_TOP_RIGHT,
      lUIPoint(121, 93 + (14 * i)), lUIPoint(110, 8));

	  m_fieldLabels[i]->SetAlign(LUI_CENTER_RIGHT);
	  m_fieldValues[i]->SetAlign(LUI_CENTER_CENTER);

  }

  // next match text
  lUISpriteWidget *nextMatchOverlay = FD_GAME_NEW lUISpriteWidget( m_statusScreen, 0, "overlay",  FDResourceManager::GetSprite("ui","next_match_overlay"), LUI_TOP_LEFT, lUIPoint( 0, 146 ) );
  nextMatchOverlay->GetSprite()->SetAlphaEnable( false );

  m_nextMatch1 = FD_GAME_NEW lUITextWidget( m_statusScreen, 0, "temp7", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 128, 159 ) );
  m_nextMatch2 = FD_GAME_NEW lUITextWidget( m_statusScreen, 0, "temp8", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 128, 177 ) );

  m_nextteamKit = FD_GAME_NEW lUISpriteWidget( m_statusScreen, LUI_WIDGET_MOUSETRANSPARENT, "kit", FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_TOP_LEFT, lUIPoint( 215, 151 ) );
}

void FDStatusScreen::SetValueTextPercentageFromInt(const StatusFieldType eField, const int iValue, const float fMaxValue, const bool bReverseColors)
{
	char str[64];

  ::sprintf(str, "%d%%", iValue);
	m_fieldValues[eField]->SetText(lUIText(str));

	float fPercentage = (float)iValue / fMaxValue;
	m_fieldStripes[eField]->SetPctA(fPercentage);

	if (bReverseColors)
		fPercentage = 1.0f - fPercentage;

	if (fPercentage < 0.3f)
		m_fieldStripes[eField]->SetColourA(L_ARGB(0xFF, 0xFF, 0, 0));
	else if (fPercentage < 0.7f)
		m_fieldStripes[eField]->SetColourA(L_ARGB(0xFF, 0xFF, 0xFF, 0));
	else
		m_fieldStripes[eField]->SetColourA(L_ARGB(0xFF, 0, 0xFF, 0));

	m_fieldStripes[eField]->SetColourB(L_ARGB(0xFF, 0x30, 0x30, 0x30));
}


void FDStatusScreen::Update()
{
  char tmp[256];

  // team name
  m_teamName->SetText( lUIText( WorldData().GetCurrentUserClub()->GetName() ), true );

  m_teamKit->GetSprite().Get()->SetFrame( WorldData().GetCurrentUserClub()->GetHomeKitID() );

  sprintf(tmp, "SHIELD_STRIPE1_%s", GetBadgeSpritePostfix( WorldData().GetCurrentUserClub()->GetHomeKit().GetPrimaryColour()-1 ) );
  m_badgeStripe1->SetSprite( FDResourceManager::GetSprite( "badge", tmp ) );

  sprintf(tmp, "SHIELD_STRIPE2_%s", GetBadgeSpritePostfix( WorldData().GetCurrentUserClub()->GetHomeKit().GetSecondaryColour()-1 ) );
  m_badgeStripe2->SetSprite( FDResourceManager::GetSprite( "badge", tmp ) );

  // manager name
  m_managerName->SetText( lUIText(WorldData().GetCurrentUserClub()->GetManagerName()) );

  // division
  //TODO LOCALE
  m_fieldLabels[SFT_LEAGUE]->SetText(lUIText("LEAGUE"));
  if( WorldData().GetCurrentUserClub()->GetTableData().GetNumberGamesPlayed() > 0 )
  {
    sprintf( tmp, "%s (%d)", WorldData().GetCurrentUserClub()->GetDivision().GetName(),
                             WorldData().GetCurrentUserClub()->GetTableData().GetLeaguePosition() + 1);
    m_fieldValues[SFT_LEAGUE]->SetText(lUIText( tmp ));
  }
  else
  {
    sprintf( tmp, "%s", WorldData().GetCurrentUserClub()->GetDivision().GetName());
    m_fieldValues[SFT_LEAGUE]->SetText(lUIText( tmp ));
  }

  // confidence
  SetValueTextPercentageFromInt(SFT_BOARDCONFIDENCE, (int)WorldData().GetCurrentUserClub()->GetManager().GetBoardConfidence(),
                                100.0f, false );
  m_fieldLabels[SFT_BOARDCONFIDENCE]->SetText(lUIText("BOARD CONFIDENCE"));

  SetValueTextPercentageFromInt(SFT_FANCONFIDENCE, (int)WorldData().GetCurrentUserClub()->GetManager().GetSupportersConfidence(),
                                100.0f, false );
  m_fieldLabels[SFT_FANCONFIDENCE]->SetText(lUIText("FAN CONFIDENCE"));

  // club balance
  m_fieldLabels[SFT_CLUBBALANCE]->SetText(lUIText("CLUB BALANCE"));
  m_fieldValues[SFT_CLUBBALANCE]->SetText( lUIText( WorldData().GetCurrentUserClub()->GetCashStr().CStr() ) );

  // next match
  CMatchInfo Match;
	WorldData().GetCurrentUserClub()->GetNextMatchDetails(Match);

	// Display the details of the next match, if known
  if( Match.GetCompetitionType() != NOTAMATCH && Match.GetOpponentClub( WorldData().GetCurrentUserClub() ) )
	{
    sprintf( tmp, "%s - %s", Match.GetTheDate().LongDateStr(), Match.GetCompetitionName() );
    m_nextMatch1->SetText( lUIText( tmp ) );

    CClub *pOpponentClub = Match.GetOpponentClub(WorldData().GetCurrentUserClub());

    lString str = pOpponentClub->GetName().CStr();

    lSharedPtr<lFont> font = m_nextMatch2->GetFont();

    int Width = 0, Height = 0;

	  font->GetStringBound( Width, Height, str.CStr() );

	  if( Width > 70 )
	  {
	    bool bDone = false;

	    while( !bDone )
	    {
	      font->GetStringBound( Width, Height, str.CStr() );

	      if( Width > 70 - 5 )
	      {
	        str = str.Left( str.Size()-1 );
	        if( str.Size()  == 0 ) bDone = true;
	      }
	      else
	      {
	        bDone = true;
        }
	    }
	  }

    sprintf( tmp, "%s - %s", str.CStr(), Match.GetStadiumName() );
    m_nextMatch2->SetText( lUIText( tmp ) );

    m_nextteamKit->GetSprite().Get()->SetFrame( pOpponentClub->GetHomeKitID() );
    m_nextteamKit->Show();
  } else
  {
    m_nextMatch1->SetText(lUIText(""));
    m_nextMatch2->SetText(lUIText(""));
    m_nextteamKit->Hide();
  }
}


void FDStatusScreen::DoShow()
{
  m_statusScreen->Show();
}

void FDStatusScreen::DoHide()
{
  m_statusScreen->Hide();
}


