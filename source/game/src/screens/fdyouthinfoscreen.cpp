//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsquadscreen.cpp
// Description : FDSquadScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdyouthinfoscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDYouthInfoScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDYouthInfoScreen::FDYouthInfoScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
  m_currentYouth = NULL;
}

FDYouthInfoScreen::~FDYouthInfoScreen()
{
}

void FDYouthInfoScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_youthScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "youth_screen_sub" );
  m_youthScreenSub->Hide();

  // Add background stripes.
  for (int i = 0; i < 7; i++)
  {
	  m_backgroundStripes[i] = FD_GAME_NEW lUISpriteWidget(m_youthScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
		  lUIPoint(0, 91 + (14 * i)), lUIPoint(256, 12));
	  m_backgroundStripes[i]->GetSprite()->SetWidth( 256 );

	  m_separatorStripes[i] = FD_GAME_NEW lUISpriteWidget(m_youthScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_white"), LUI_TOP_LEFT,
		  lUIPoint(126, 91 + (14 * i)), lUIPoint(3, 12));
  }

  // Percentage bars - left side
  for (int i = 0; i < 7; i++)
  {
	  m_youthInfoComboData[i] = FD_GAME_NEW FDBarWidget(m_youthScreenSub, 0, "fieldstripe", LUI_TOP_LEFT,
		  lUIPoint(80, 91 + (14 * i)), lUIPoint(35, 12));

    m_youthInfoComboData[i]->SetPctA(0.0f);
    m_youthInfoComboData[i]->SetColourB(L_ARGB(0xFF, 0x50, 0x50, 0x50));
  }

  // Percentage bars - right side
    for (int i = 0; i < 3; i++)
  {
	  m_youthInfoComboData[7 + i] = FD_GAME_NEW FDBarWidget(m_youthScreenSub, 0, "fieldstripe", LUI_TOP_LEFT,
		  lUIPoint(205, 91 + (14 * i)), lUIPoint(35, 12));

    m_youthInfoComboData[7+i]->SetPctA(0.0f);
	  m_youthInfoComboData[7+i]->SetColourB(L_ARGB(0xFF, 0x50, 0x50, 0x50));
  }

  // Left side - label - value
  for (int i = 0; i < 7; i++)
  {
	  m_youthInfoText[i] = FD_GAME_NEW lUITextWidget( m_youthScreenSub, 0, "label", lUIText(""), LUI_TOP_LEFT,
		  lUIPoint( 0, 91 + (14 * i)), lUIPoint(78, 12) );
	  m_youthInfoData[i] = FD_GAME_NEW lUITextWidget( m_youthScreenSub, 0, "value", lUIText(""), LUI_TOP_LEFT,
		  lUIPoint( 80, 91 + (14 * i)), lUIPoint(35, 12) );
  }

  // Right side - label - value
  for (int i = 0; i < 4; i++)
  {
	  m_youthInfoText[7+ i] = FD_GAME_NEW lUITextWidget( m_youthScreenSub, 0, "label", lUIText(""), LUI_TOP_LEFT,
		  lUIPoint( 125, 91 + (14 * i)), lUIPoint(78, 12) );
	  m_youthInfoData[7+ i] = FD_GAME_NEW lUITextWidget( m_youthScreenSub, 0, "value", lUIText(""), LUI_TOP_LEFT,
		  lUIPoint( 205, 91 + (14 * i)), lUIPoint(35, 12) );
  }

  // Left Labels
  m_youthInfoText[0]->SetText( lUIText( IDS_HANDLING ) );
  m_youthInfoText[1]->SetText( lUIText( IDS_TACKLING ) );
  m_youthInfoText[2]->SetText( lUIText( IDS_PASSING ) );
  m_youthInfoText[3]->SetText( lUIText( IDS_SHOOTING ) );
  m_youthInfoText[4]->SetText( lUIText( IDS_PACE ) );
  m_youthInfoText[5]->SetText( lUIText( IDS_TEMPER ) );
  m_youthInfoText[6]->SetText( lUIText( IDS_FITNESS ) );

  // Right labels
  m_youthInfoText[7]->SetText( lUIText( IDS_AGE ) );
  m_youthInfoText[8]->SetText( lUIText( IDS_MORALE ) );
  m_youthInfoText[9]->SetText( lUIText( IDS_BESTFOOT ) );

  // Left Labels
  m_youthInfoText[0]->SetTextToUpper();
  m_youthInfoText[1]->SetTextToUpper();
  m_youthInfoText[2]->SetTextToUpper();
  m_youthInfoText[3]->SetTextToUpper();
  m_youthInfoText[4]->SetTextToUpper();
  m_youthInfoText[5]->SetTextToUpper();
  m_youthInfoText[6]->SetTextToUpper();

  // Right labels
  m_youthInfoText[7]->SetTextToUpper();
  m_youthInfoText[8]->SetTextToUpper();
  m_youthInfoText[9]->SetTextToUpper();

  //m_youthInfoText[10]->SetText( lUIText( IDS_NATIONALITY ) );

  for (int i = 0; i < 11; i++)
  {
	  m_youthInfoText[i]->SetAlign(LUI_CENTER_RIGHT);
	  m_youthInfoData[i]->SetAlign(LUI_CENTER_CENTER);
  }

  //
  m_youthInfoName = FD_GAME_NEW lUITextWidget( m_youthScreenSub, 0, "name", lUIText(""), LUI_TOP_CENTER, lUIPoint( 127, 45 ), lUIPoint( 127, 16 ) );
  m_youthInfoName->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_youthInfoName->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );
  m_youthInfoPos = FD_GAME_NEW lUITextWidget( m_youthScreenSub, 0, "pos", lUIText(""), LUI_TOP_CENTER, lUIPoint( 127, 68 ) );
  m_youthInfoPos->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_youthInfoPos->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  FDBarWidget * splitterBar = FD_GAME_NEW FDBarWidget( m_youthScreenSub, 0, "line", LUI_CENTER_CENTER, lUIPointPercent( 50, 34 ), lUIPoint( 155, 1 ) );
  splitterBar->SetPctA(0.0f);
  splitterBar->SetColourB(L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));

  m_kitBG = FD_GAME_NEW lUISpriteWidget( m_youthScreenSub, 0, "kitbg",  FDResourceManager::GetSprite("uiicon","shirt_holder"), LUI_TOP_LEFT, lUIPoint( 5, 43 ) );
  m_kitBG->GetSprite()->SetAlphaEnable( true );

  m_kit   = FD_GAME_NEW lUISpriteWidget( m_youthScreenSub, 0, "kit",  FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_TOP_LEFT, lUIPoint( 10, 51 ) );
  m_kit->Hide();

  m_teamBadge = FD_GAME_NEW lUISpriteWidget( m_youthScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );
  m_badgeStripe1 = FD_GAME_NEW lUISpriteWidget( m_youthScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );
  m_badgeStripe2 = FD_GAME_NEW lUISpriteWidget( m_youthScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );
}

void FDYouthInfoScreen::DoShow()
{
  m_youthScreenSub->Show();
}

void FDYouthInfoScreen::DoHide()
{
  m_youthScreenSub->Hide();
}

void FDYouthInfoScreen::SetValueTextPercentageFromInt(const int eField, const int iValue, const float fMaxValue, const bool bReverseColors)
{
	char str[64];

	::sprintf(str, "%d", iValue);
	m_youthInfoData[eField]->SetText(lUIText(str));

	float fPercentage = (float)iValue / fMaxValue;
	m_youthInfoComboData[eField]->SetPctA(fPercentage);

	if (bReverseColors)
		fPercentage = 1.0f - fPercentage;

	if (fPercentage < 0.3f)
		m_youthInfoComboData[eField]->SetColourA(L_ARGB(0xFF, 0xFF, 0, 0));
	else if (fPercentage < 0.7f)
		m_youthInfoComboData[eField]->SetColourA(L_ARGB(0xFF, 0xFF, 0xFF, 0));
	else
		m_youthInfoComboData[eField]->SetColourA(L_ARGB(0xFF, 0, 0xFF, 0));

	m_youthInfoComboData[eField]->SetColourB(L_ARGB(0xFF, 0x30, 0x30, 0x30));
}


void FDYouthInfoScreen::SetYouthInfo( CYouth *youth )
{
  lLogFmt("SetYouthInfo\n");

  m_currentYouth = youth;

  char str[32];

  // handling
  SetValueTextPercentageFromInt( 0, youth->GetSkills().GetHandling(), 100.0f, false );
  // tackling
  SetValueTextPercentageFromInt( 1, youth->GetSkills().GetTackling(), 100.0f, false );
  // passing
  SetValueTextPercentageFromInt( 2, youth->GetSkills().GetPassing(), 100.0f, false );
  // shooting
  SetValueTextPercentageFromInt( 3, youth->GetSkills().GetShooting(), 100.0f, false );
  // pace
  SetValueTextPercentageFromInt( 4, youth->GetSkills().GetPace(), 100.0f,false );

    // temp
  SetValueTextPercentageFromInt( 5, youth->GetTemperament(), 9.0f, true );
  //TODO m_pCurrentYouth->TemperamentStr()

  // fitness
  SetValueTextPercentageFromInt( 6, youth->GetSkills().GetFitness(), 100.0f, false );

  // age
  sprintf( str, "%d", youth->GetAge());
  m_youthInfoData[7]->SetText( lUIText( str ) );

  // morale
  sprintf( str, "%d", youth->GetMorale());
  m_youthInfoData[8]->SetText( lUIText( str ) );

  // best foot
  sprintf( str, "%s", youth->GetSkills().GetBestFoot() == LEFTFOOT ? "L" : "R" );
  m_youthInfoData[9]->SetText( lUIText( str ) );                  //m_youthInfoTable->SetTextColour( 5, 1, L_ARGB( 255, 0, 255, 0 ) );

  // nationality
  //m_youthInfoData[10]->SetText( lUIText( WorldData().GetCountry(youth->GetNationalityID()).NationalityStr() ) );   //m_youthInfoTable->SetTextColour( 1, 4, L_ARGB( 255, 0, 255, 0 ) );

  // name
  m_youthInfoName->SetText( lUIText( youth->GetName() ), true  );
  m_youthInfoPos->SetText( lUIText( youth->GetSkills().PositionStr() ) );

  //CClub &club = youth->GetClub() ;

  //if( club.GetID() != NOCLUB )
  if( WorldData().GetCurrentUserClub()->IsYouthAtClub( youth ) )
  {
    m_kit->GetSprite().Get()->SetFrame( WorldData().GetCurrentUserClub()->GetHomeKitID() );

    m_kit->Show();
    m_kitBG->Show();

    m_teamBadge->Show();
    m_badgeStripe1->Show();
    m_badgeStripe2->Show();

    sprintf(str, "SHIELD_STRIPE1_%s", GetBadgeSpritePostfix( WorldData().GetCurrentUserClub()->GetHomeKit().GetPrimaryColour()-1 ) );
    m_badgeStripe1->SetSprite( FDResourceManager::GetSprite( "badge", str ) );

    sprintf(str, "SHIELD_STRIPE2_%s", GetBadgeSpritePostfix( WorldData().GetCurrentUserClub()->GetHomeKit().GetSecondaryColour()-1 ) );
    m_badgeStripe2->SetSprite( FDResourceManager::GetSprite( "badge", str ) );
  } else
  {
    m_kit->Hide();
    m_kitBG->Hide();
    m_teamBadge->Hide();
    m_badgeStripe1->Hide();
    m_badgeStripe2->Hide();
  }
}

/*

----------
 Actions

	WorldData().OnHireYouth();

	theApp.OnPromoteYouth(TEXT(""));

  if (WorldData().OnSackYouth() == true)
  */
