//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdemployeeinfoscreen.cpp
// Description : FDEmployeeInfoScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdemployeeinfoscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDEmployeeInfoScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDEmployeeInfoScreen::FDEmployeeInfoScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDEmployeeInfoScreen::~FDEmployeeInfoScreen()
{
}

void FDEmployeeInfoScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_employeeScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "employee_screen_sub" );
  m_employeeScreenSub->Hide();

  // Stripes + Bars
  m_employeeStripes[0] = FD_GAME_NEW lUISpriteWidget(m_employeeScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
    lUIPoint( 0, 91 ), lUIPoint(256, 12));
  m_employeeStripes[0]->GetSprite()->SetWidth( 256 );

  m_employeeBars[0] = FD_GAME_NEW FDBarWidget(m_employeeScreenSub, 0, "bar", LUI_TOP_LEFT,
    lUIPoint( 137, 91 ), lUIPoint(35, 12));

  for( int i = 1; i < 5; i++ )
  {
    m_employeeStripes[i] = FD_GAME_NEW lUISpriteWidget(m_employeeScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
     lUIPoint( 0, 91 + i * 14 ), lUIPoint(256, 12));
	  m_employeeStripes[i]->GetSprite()->SetWidth( 256 );

    m_employeeBars[i] = FD_GAME_NEW FDBarWidget(m_employeeScreenSub, 0, "bar", LUI_TOP_LEFT,
     lUIPoint( 137, 91 + i * 14 ), lUIPoint(80, 12));
  }

  // Labels + Data
  for( int i = 0; i < 5; i++ )
  {
    m_employeeInfoLabel[i] = FD_GAME_NEW lUITextWidget( m_employeeScreenSub, 0, "label", lUIText(""), LUI_TOP_LEFT,
      lUIPoint( 0, 91 + i * 14 ), lUIPoint( 127, 12  ) );
    m_employeeInfoData[i] = FD_GAME_NEW lUITextWidget( m_employeeScreenSub, 0, "data", lUIText(""), LUI_TOP_LEFT,
      lUIPoint( 140, 91 + i * 14 ) );
  }

  lUIText temp( IDS_SKILL_SINGLE ) ; temp.ToUpper();
  m_employeeInfoLabel[0]->SetText( temp );
  temp = lUIText( IDS_AGE ) ; temp.ToUpper();
  m_employeeInfoLabel[1]->SetText( temp );
  temp = lUIText( IDS_NATIONALITY ) ; temp.ToUpper();
  m_employeeInfoLabel[2]->SetText( temp );
  temp = lUIText( IDS_WEEKLYWAGE ) ; temp.ToUpper();
  m_employeeInfoLabel[3]->SetText( temp );
  temp = lUIText( IDS_CONTRACTENDDATE2 ) ; temp.ToUpper();
  m_employeeInfoLabel[4]->SetText( temp );

   for( int i = 0; i < 5; i++ )
  {
	  m_employeeInfoLabel[i]->SetAlign(LUI_CENTER_RIGHT);
	  m_employeeInfoData[i]->SetAlign(LUI_CENTER_LEFT);

	  m_employeeBars[i]->SetPctA( 0.0f );
	  m_employeeBars[i]->SetColourB( L_ARGB( 128, 80, 80, 80 )) ;
  }



  //
  m_employeeInfoNamePos = FD_GAME_NEW lUITextWidget( m_employeeScreenSub, 0, "namepos", lUIText(""), LUI_TOP_CENTER, lUIPoint( 127, 45 ), lUIPoint( 127, 16 ) );
  m_employeeInfoNamePos->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_employeeInfoNamePos->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  FDBarWidget * splitterBar = FD_GAME_NEW FDBarWidget( m_employeeScreenSub, 0, "line", LUI_CENTER_CENTER, lUIPointPercent( 50, 34 ), lUIPoint( 155, 1 ) );
  splitterBar->SetPctA(0.0f);
  splitterBar->SetColourB(L_ARGB(0xFF, 0xFF, 0xFF, 0xFF));
  m_employeeInfoRolePos = FD_GAME_NEW lUITextWidget( m_employeeScreenSub, 0, "rolepos", lUIText( IDS_ROLE ), LUI_TOP_CENTER, lUIPoint( 127, 68 ) );
  m_employeeInfoRolePos->SetFont( FDResourceManager::GetFont("fontim", "fontim" ) );
  m_employeeInfoRolePos->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );

  m_kitBG = FD_GAME_NEW lUISpriteWidget( m_employeeScreenSub, 0, "kitbg",  FDResourceManager::GetSprite("uiicon","shirt_holder"), LUI_TOP_LEFT, lUIPoint( 5, 43 ) );
  m_kitBG->GetSprite()->SetAlphaEnable( true );

  m_kit   = FD_GAME_NEW lUISpriteWidget( m_employeeScreenSub, 0, "kit",  FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_TOP_LEFT, lUIPoint( 10, 51 ) );
  m_kit->Hide();

  m_teamBadge = FD_GAME_NEW lUISpriteWidget( m_employeeScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );
  m_badgeStripe1 = FD_GAME_NEW lUISpriteWidget( m_employeeScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );
  m_badgeStripe2 = FD_GAME_NEW lUISpriteWidget( m_employeeScreenSub, 0, "shield",  FDResourceManager::GetSprite("badge","shield_box"), LUI_TOP_RIGHT, lUIPoint( 256-5, 43 ) );
}

void FDEmployeeInfoScreen::DoShow()
{
  m_employeeScreenSub->Show();
}

void FDEmployeeInfoScreen::DoHide()
{
  m_employeeScreenSub->Hide();
}

void FDEmployeeInfoScreen::SetEmployeeInfo( CEmployee *employee )
{
  lLogFmt("SetEmployeeInfo\n");

  m_currentEmployee = employee;
  WorldData().SetCurrentEmployee( m_currentEmployee );

  //TODO: Add current club name o side of name
  char str[32];

  // name
  m_employeeInfoNamePos->SetText( lUIText( m_currentEmployee->GetName() ), true );

  // role
  m_employeeInfoRolePos->SetText( lUIText( m_currentEmployee->TypeTextStr() ) );
  //m_employeeInfoRolePos->SetTextColour( PosnColour[m_currentEmployee->GetType()] );

  // Set params
	CString strPrint;

  // skill
  m_employeeInfoData[0]->SetText( lUIText( m_currentEmployee->GetSkillStr() ) );

  float fPercentage = (float) m_currentEmployee->GetSkill() / 100.0f;
  m_employeeBars[0]->SetPctA( fPercentage );
	if (fPercentage < 0.3f)
		m_employeeBars[0]->SetColourA(L_ARGB(0xFF, 0xFF, 0, 0));
	else if (fPercentage < 0.7f)
		m_employeeBars[0]->SetColourA(L_ARGB(0xFF, 0xFF, 0xFF, 0));
	else
		m_employeeBars[0]->SetColourA(L_ARGB(0xFF, 0, 0xFF, 0));

	m_employeeBars[0]->SetColourB(L_ARGB(0xFF, 0x30, 0x30, 0x30));

  // age
  strPrint.Format("%d", m_currentEmployee->GetAge());
  m_employeeInfoData[1]->SetText( lUIText( strPrint ) );

  // nationality
  m_employeeInfoData[2]->SetText( lUIText( m_currentEmployee->NationalityStr() ) );

  if( !WorldData().GetCurrentUserClub()->IsEmployeeAtClub( m_currentEmployee ) )
  {
    // wage
    m_employeeInfoData[3]->SetText( lUIText( "n/a" ) );

    // contract end date
    m_employeeInfoData[4]->SetText( lUIText( "n/a" ) );
  } else
  {
    // wage
    m_employeeInfoData[3]->SetText( lUIText( WorldData().GetCurrentUserClub()->GetForeignStr(m_currentEmployee->GetWage()) ) );

    // contract end date
    m_employeeInfoData[4]->SetText( lUIText( m_currentEmployee->ContractStr() ) );
  }

  //CClub &club = m_currentEmployee->GetClub() ;

  //if( club.GetID() != NOCLUB )
  if( WorldData().GetCurrentUserClub()->IsEmployeeAtClub( m_currentEmployee ) ) // Assuming we never get to see other clubs employees
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

