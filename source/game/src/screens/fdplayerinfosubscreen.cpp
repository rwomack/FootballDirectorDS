//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdplayerinfoscreen.cpp
// Description : FDPlayerInfoSubScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbarwidget.h>
#include <fdbuttonwidget.h>

#include <screens/fdplayerinfosubscreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDPlayerInfoSubScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDPlayerInfoSubScreen::FDPlayerInfoSubScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDPlayerInfoSubScreen::~FDPlayerInfoSubScreen()
{
}

void FDPlayerInfoSubScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_playerScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "player_screen_sub" );
  m_playerScreenSub->Hide();

  // Kit, Flag and shields

  m_kitBG = FD_GAME_NEW lUISpriteWidget( m_playerScreenSub, 0, "kitbg",  FDResourceManager::GetSprite("uiicon","shirt_holder"), LUI_TOP_LEFT, lUIPoint( 5, 43 ) );
  m_kitBG->GetSprite()->SetAlphaEnable( true );
  m_kit   = FD_GAME_NEW lUISpriteWidget( m_playerScreenSub, 0, "kit",  FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_TOP_LEFT, lUIPoint( 10, 51 ) );

  m_flags = FD_GAME_NEW lUISpriteWidget( m_playerScreenSub, 0, "flags",  FDResourceManager::GetSprite("flags","flags_FX"), LUI_ANCHOR_DEFAULT, lUIPoint( 48, 68 ) );
  m_flags->GetSprite()->SetTransparent(false);

  m_teamBadge = FD_GAME_NEW lUISpriteWidget( m_playerScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );
  m_badgeStripe1 = FD_GAME_NEW lUISpriteWidget( m_playerScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );
  m_badgeStripe2 = FD_GAME_NEW lUISpriteWidget( m_playerScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );

  // Add background stripes.
  for (int i = 0; i < 7; i++)
  {
	  m_backgroundStripes[i] = FD_GAME_NEW lUISpriteWidget(m_playerScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
		  lUIPoint(0, 91 + (14 * i)), lUIPoint(256, 12));
	  m_backgroundStripes[i]->GetSprite()->SetWidth( 256 );

	  m_separatorStripes[i] = FD_GAME_NEW lUISpriteWidget(m_playerScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_white"), LUI_TOP_LEFT,
      lUIPoint(105, 91 + (14 * i)), lUIPoint(3, 12));
  }

  // Add left-hand-side field backgrounds and value text.
  for (int i = 0; i < 7; i++)
  {
	  m_fieldStripes[i] = FD_GAME_NEW FDBarWidget(m_playerScreenSub, 0, "fieldstripe", LUI_TOP_LEFT,
      lUIPoint(65, 91 + (14 * i)), lUIPoint(35, 12));
	  m_fieldValues[i] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "value", lUIText(""), LUI_TOP_LEFT,
      lUIPoint(65, 91 + (14 * i)), lUIPoint(35, 12));
  }

  // Add right-hand-side field backgrounds and value text.
  for (int i = 0; i < 4; i++)
  {
	  m_fieldStripes[7 + i] = FD_GAME_NEW FDBarWidget(m_playerScreenSub, 0, "fieldstripe", LUI_TOP_LEFT,
      lUIPoint(210, 91 + (14 * i)), lUIPoint(41, 12));
	  m_fieldValues[7 + i] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "value", lUIText(""), LUI_TOP_LEFT,
      lUIPoint(210, 91 + (14 * i)), lUIPoint(41, 12));
  }

  // Special cases for wage and value.
  m_fieldStripes[PIFT_CONTRACTEND] = FD_GAME_NEW FDBarWidget(m_playerScreenSub, 0, "fieldstripe", LUI_TOP_LEFT,
    lUIPoint(169, 147), lUIPoint(82, 12));
  m_fieldValues[PIFT_CONTRACTEND] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "value", lUIText(""), LUI_TOP_LEFT,
    lUIPoint(169, 147), lUIPoint(82, 12));
  m_fieldStripes[PIFT_WAGE] = FD_GAME_NEW FDBarWidget(m_playerScreenSub, 0, "fieldstripe", LUI_TOP_LEFT,
    lUIPoint(169, 161), lUIPoint(82, 12));
  m_fieldValues[PIFT_WAGE] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "value", lUIText(""), LUI_TOP_LEFT,
    lUIPoint(169, 161), lUIPoint(82, 12));
  m_fieldStripes[PIFT_VALUE] = FD_GAME_NEW FDBarWidget(m_playerScreenSub, 0, "fieldstripe", LUI_TOP_LEFT,
    lUIPoint(169, 175), lUIPoint(82, 12));
  m_fieldValues[PIFT_VALUE] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "value", lUIText(""), LUI_TOP_LEFT,
    lUIPoint(169, 175), lUIPoint(82, 12));

  // Add labels for left-hand-side fields.
  lUIText temp(IDS_HANDLING);  temp.ToUpper();
  m_fieldLabels[PIFT_HANDLING] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "label_handling", temp, LUI_TOP_LEFT,
    lUIPoint(0, 93 + (14 * 0)), lUIPoint(60, 8));
  temp = lUIText(IDS_TACKLING);  temp.ToUpper();
  m_fieldLabels[PIFT_TACKLING] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "label_tackling", temp, LUI_TOP_LEFT,
    lUIPoint(0, 93 + (14 * 1)), lUIPoint(60, 8));
  temp = lUIText(IDS_PASSING);  temp.ToUpper();
  m_fieldLabels[PIFT_PASSING] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "label_passing", temp, LUI_TOP_LEFT,
    lUIPoint(0, 93 + (14 * 2)), lUIPoint(60, 8));
  temp = lUIText(IDS_SHOOTING);  temp.ToUpper();
  m_fieldLabels[PIFT_SHOOTING] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "label_shooting", temp, LUI_TOP_LEFT,
    lUIPoint(0, 93 + (14 * 3)), lUIPoint(60, 8));
  temp = lUIText(IDS_PACE);  temp.ToUpper();
  m_fieldLabels[PIFT_PACE] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "label_pace", temp, LUI_TOP_LEFT,
    lUIPoint(0, 93 + (14 * 4)), lUIPoint(60, 8));
  temp = lUIText("OVERALL");  temp.ToUpper();
  m_fieldLabels[PIFT_OVERALLSKILL] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "label_overallskill", temp, LUI_TOP_LEFT,
    lUIPoint(0, 93 + (14 * 5)), lUIPoint(60, 8));
  temp = lUIText(IDS_FITNESS);  temp.ToUpper();
  m_fieldLabels[PIFT_FITNESS] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "label_fitness", temp, LUI_TOP_LEFT,
    lUIPoint(0, 93 + (14 * 6)), lUIPoint(60, 8));

  // Add labels for right-hand-side fields.
  temp = lUIText(IDS_MORALE);  temp.ToUpper();
  m_fieldLabels[PIFT_MORALE] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "label_morale", temp, LUI_TOP_LEFT,
	  lUIPoint(131, 93 + (14 * 0)), lUIPoint(74, 8));
  temp = lUIText(IDS_BESTFOOT);  temp.ToUpper();
  m_fieldLabels[PIFT_BEST_FOOT] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "label_best_foot", temp, LUI_TOP_LEFT,
    lUIPoint(131, 93 + (14 * 1)), lUIPoint(74, 8));
  temp = lUIText(IDS_AGE);  temp.ToUpper();
  m_fieldLabels[PIFT_AGE] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "label_age", temp, LUI_TOP_LEFT,
    lUIPoint(131, 93 + (14 * 2)), lUIPoint(74, 8));
  temp = lUIText(IDS_DISCIPLINARYPOINTS);  temp.ToUpper();
  m_fieldLabels[PIFT_DISCIPLINARY_POINTS] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "label_disciplinary_points", temp, LUI_TOP_LEFT,
    lUIPoint(131, 93 + (14 * 3)), lUIPoint(74, 8));
  temp = lUIText(IDS_CONTRACTENDDATE);  temp.ToUpper();
  m_fieldLabels[PIFT_CONTRACTEND] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "label_contractend", temp, LUI_TOP_LEFT,
    lUIPoint(111, 93 + (14 * 4)), lUIPoint(53, 8));
  temp = lUIText(IDS_TAB0801);  temp.ToUpper();
  m_fieldLabels[PIFT_WAGE] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "label_wage", temp, LUI_TOP_LEFT,
    lUIPoint(117, 93 + (14 * 5)), lUIPoint(47, 8));
  temp = lUIText(IDS_CLUBVALUATION);  temp.ToUpper();
  m_fieldLabels[PIFT_VALUE] = FD_GAME_NEW lUITextWidget(m_playerScreenSub, 0, "label_value", temp, LUI_TOP_LEFT,
    lUIPoint(117, 93 + (14 * 6)), lUIPoint(47, 8));

  for (int i = 0; i < PIFT_MAX; i++)
  {
	  //m_fieldLabels[i]->SetFont(FDResourceManager::GetFont("font16", "font16"));
	  //m_fieldValues[i]->SetFont(FDResourceManager::GetFont("font16", "font16"));
	  m_fieldLabels[i]->SetAlign(LUI_CENTER_RIGHT);
	  m_fieldValues[i]->SetAlign(LUI_CENTER_CENTER);
  }

  PlayerInfoFieldType eStripesUnused[] = { PIFT_WAGE, PIFT_VALUE, PIFT_CONTRACTEND, PIFT_BEST_FOOT, PIFT_AGE, PIFT_DISCIPLINARY_POINTS };
  for (int i = 0; i < (sizeof(eStripesUnused) / sizeof(PlayerInfoFieldType)); i++)
  {
	  m_fieldStripes[eStripesUnused[i]]->SetPctA(0.0f);
	  m_fieldStripes[eStripesUnused[i]]->SetColourB(L_ARGB(0xFF, 0x50, 0x50, 0x50));
  }

  m_playerInfoName = FD_GAME_NEW lUITextWidget( m_playerScreenSub, 0, "name", lUIText(""), LUI_TOP_CENTER, lUIPoint( 127, 45 ), lUIPoint( 127, 16 ) );
  m_playerInfoName->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_playerInfoName->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  m_playerInfoPos = FD_GAME_NEW lUITextWidget( m_playerScreenSub, 0, "pos", lUIText(""), LUI_TOP_CENTER, lUIPoint( 127, 68) );
  m_playerInfoPos->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_playerInfoPos->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  FDBarWidget * splitterBar = FD_GAME_NEW FDBarWidget( m_playerScreenSub, 0, "line", LUI_CENTER_CENTER, lUIPointPercent( 50, 34 ), lUIPoint( 155, 1 ) );
  splitterBar->SetPctA(0.0f);
  splitterBar->SetColourB(L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));

  m_type = PIT_NORMAL;
  m_playerScreenSub->Hide();
}

void FDPlayerInfoSubScreen::DoShow()
{
  m_playerScreenSub->Show();
}

void FDPlayerInfoSubScreen::DoHide()
{
  m_playerScreenSub->Hide();
}

void FDPlayerInfoSubScreen::SetValueTextPercentageFromInt(const PlayerInfoFieldType eField, const int iValue, const float fMaxValue, const bool bReverseColors)
{
	char str[64];

	::sprintf(str, "%d", iValue);
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

void FDPlayerInfoSubScreen::SetPlayerInfo( CPlayer *player, PlayerInfoType type )
{
  lLogFmt("SetPlayerInfo\n");

  m_type = type;

  if( player == NULL )
  {
    m_playerScreenSub->Hide();
    return;
  }

  GetMainUI()->HideSubMessage();
  m_playerScreenSub->Show();

  if( player->GetClubPtr() != NULL )
  {
    m_kit->Show();
    m_kitBG->Show();
    m_kit->GetSprite().Get()->SetFrame( player->GetClubPtr()->GetHomeKitID() );

    char tmp[32];

    m_teamBadge->Show();
    sprintf(tmp, "SHIELD_STRIPE1_%s", GetBadgeSpritePostfix( player->GetClubPtr()->GetHomeKit().GetPrimaryColour()-1 ) );
    m_badgeStripe1->SetSprite( FDResourceManager::GetSprite( "badge", tmp ) );

    sprintf(tmp, "SHIELD_STRIPE2_%s", GetBadgeSpritePostfix( player->GetClubPtr()->GetHomeKit().GetSecondaryColour()-1 ) );
    m_badgeStripe2->SetSprite( FDResourceManager::GetSprite( "badge", tmp ) );

    m_badgeStripe1->Show();
    m_badgeStripe2->Show();
  } else
  {
    m_kit->Hide();
    m_kitBG->Hide();
    m_teamBadge->Hide();
    m_badgeStripe1->Hide();
    m_badgeStripe2->Hide();
  }


  char str[32];

  SetValueTextPercentageFromInt(PIFT_HANDLING, player->GetSkills().GetHandling(), 100.0f, false);
  SetValueTextPercentageFromInt(PIFT_TACKLING, player->GetSkills().GetTackling(), 100.0f, false);
  SetValueTextPercentageFromInt(PIFT_PASSING, player->GetSkills().GetPassing(), 100.0f, false);
  SetValueTextPercentageFromInt(PIFT_SHOOTING, player->GetSkills().GetShooting(), 100.0f, false);
  SetValueTextPercentageFromInt(PIFT_PACE, player->GetSkills().GetPace(), 100.0f, false);
  SetValueTextPercentageFromInt(PIFT_DISCIPLINARY_POINTS, player->GetDisciplinaryPointsThisSeason(), 100.0f, true);

  // best foot
  sprintf( str, "%s", player->GetSkills().GetBestFoot() == LEFTFOOT ? "L" : "R" );
  m_fieldValues[PIFT_BEST_FOOT]->SetText( lUIText( str ) );
  m_fieldStripes[PIFT_BEST_FOOT]->SetColourA(L_ARGB(0xFF, 0, 0xFF, 0));

  SetValueTextPercentageFromInt(PIFT_OVERALLSKILL, player->GetSkills().GetOverallSkill(), 100.0f, false);
  SetValueTextPercentageFromInt(PIFT_FITNESS, player->GetSkills().GetFitness(), 100.0f, false);

  SetValueTextPercentageFromInt(PIFT_MORALE, player->GetMorale(), 100.0f, false);

  // disc - contract end
  sprintf( str, "%d", player->GetDisciplinaryPointsThisSeason() );
  m_fieldValues[PIFT_DISCIPLINARY_POINTS]->SetText(lUIText( str ));
  m_fieldStripes[PIFT_DISCIPLINARY_POINTS]->SetColourA(L_ARGB(0xFF, 0, 0xFF, 0));

  // age
  char sAge[64];
  ::sprintf(sAge, "%d", player->GetAge());
  m_fieldValues[PIFT_AGE]->SetText(lUIText(sAge));

  // wage
  CClub* club = &player->GetClub();
  if ((club == NULL) || (club->IsUserControlled() == false))
  {
	  m_fieldValues[PIFT_WAGE]->SetText(lUIText("n/a"));

    // contract end
    m_fieldValues[PIFT_CONTRACTEND]->SetText(lUIText( "n/a" ));
  }
  else
  {
	  m_fieldValues[PIFT_WAGE]->SetText(lUIText(club->GetForeignStr(player->GetWage()).CStr()));

    // contract end
    m_fieldValues[PIFT_CONTRACTEND]->SetText(lUIText( player->ContractStr() ));
  }

  // value
  if( club != NULL )
  {
    if( club->IsNonLeagueClub() )
    {
      m_fieldValues[PIFT_VALUE]->SetText( lUIText( "n/a" ) );
    }
    else
    {
      m_fieldValues[PIFT_VALUE]->SetText( lUIText(club->GetForeignStr(player->GetSellingPrice())));
    }
  }

  // name
  m_playerInfoName->SetText( lUIText(player->GetName()), true );
  m_playerInfoPos->SetText( lUIText(player->GetSkills().PositionStr()) );

  // Set flags, photos and shields
  m_flags->GetSprite()->SetFrame(WorldData().GetCountry(player->GetNationalityID()).GetID());
  //m_photo->SetSprite( FDResourceManager::GetPhoto( player->DoFindID() ) );
  //m_shield->SetSprite( FDResourceManager::GetShield( club->GetID() ) );
}
