//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdstrategyrolesscreen.cpp
// Description : Screen for setting player roles.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdresourcemanager.h>
#include <fdmainmenuui.h>
#include <ui/luibuttonwidget.h>
#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <framework/lsprite.h>
#include <screens/fdstrategyrolesscreen.h>
#include <screens/fdplayerinfosubscreen.h>
#include <fdbuttonwidget.h>
#include <fddragwidget.h>
#include <fdsquadtablewidget.h>
#include <fdtextwidget.h>

#include <PlayerSquad.h>
#include <matchplay.h>

using namespace L_NAMESPACE_NAME;

FDStrategyRolesScreen::FDStrategyRolesScreen( FDMainMenuUI *parent ) : FDScreen(parent)
{
	Init();
}

FDStrategyRolesScreen::~FDStrategyRolesScreen()
{
}

void FDStrategyRolesScreen::Init()
{
	m_spritePlay = 0;
	m_spriteInterval = 0.0f;
	m_pauseInterval = 0.0f;
	m_matchPlay = NULL;

	m_rolesScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "roles_screen" );
	m_rolesScreen->Hide();

	// top screen
	m_rolesScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "roles_screen_sub" );
	m_rolesScreenSub->Hide();

	m_dragWidget = FD_GAME_NEW FDDragWidget(m_rolesScreen, 0, "rolesdrag");

	// Squad table
	m_table = FD_GAME_NEW FDSquadTableWidget( m_rolesScreen, 0, "playertable",
        LUI_TOP_LEFT, lUIPoint( 20, 0 ), lUIPoint( 136, 144 ) );
  m_table->SetInfoToShow(FDSquadTableWidget::INFOTOSHOW_ROLE, true);
	m_table->SignalPlayerDropped.Connect(this, &FDStrategyRolesScreen::OnPlayerDropped);
	m_table->SignalRowSelect.Connect( this, &FDStrategyRolesScreen::OnSignalSelect );

	m_dragWidget->ConnectToTable(m_table->GetUnderlyingTable());

	m_squadRoles      = FD_GAME_NEW lUISpriteWidget( m_rolesScreen, 0, "squadroles", FDResourceManager::GetSprite("ui","squad_roles"), LUI_TOP_RIGHT, lUIPoint( 256, 0 ) );

	m_captainName = FD_GAME_NEW FDTextWidget( m_rolesScreen, 0, "captain", lUIText(""), LUI_TOP_LEFT, lUIPoint( 173, 121 ), lUIPoint(65, 15) );
	m_penName = FD_GAME_NEW FDTextWidget( m_rolesScreen, 0, "pen", lUIText(""), LUI_TOP_LEFT, lUIPoint( 173, 133 ), lUIPoint(65, 15) );
	m_freekickName = FD_GAME_NEW FDTextWidget( m_rolesScreen, 0, "freekick", lUIText(""), LUI_TOP_LEFT, lUIPoint( 173, 146 ), lUIPoint(65, 15) );
	m_cornerName = FD_GAME_NEW FDTextWidget( m_rolesScreen, 0, "corners", lUIText(""), LUI_TOP_LEFT, lUIPoint( 173, 159 ), lUIPoint(65, 15) );

	m_captainName->SetAlign(LUI_TOP_LEFT);
	m_penName->SetAlign(LUI_TOP_LEFT);
	m_freekickName->SetAlign(LUI_TOP_LEFT);
	m_cornerName->SetAlign(LUI_TOP_LEFT);

	// Role animations
	m_squadRoleCorner = FD_GAME_NEW lUISpriteWidget( m_rolesScreen, 0, "squadrolecorner", FDResourceManager::GetSprite("anims","corner"), LUI_TOP_LEFT, lUIPoint( 150, 7 ) );
	m_squadRoleFreekick= FD_GAME_NEW lUISpriteWidget( m_rolesScreen, 0, "squadrolefreekick", FDResourceManager::GetSprite("anims","freekick2"), LUI_BOTTOM_CENTER, lUIPoint( 230,64 ) );
	m_squadRolePenalty= FD_GAME_NEW lUISpriteWidget( m_rolesScreen, 0, "squadrolepenalty", FDResourceManager::GetSprite("anims","penalty"), LUI_BOTTOM_CENTER, lUIPoint( 200, 48 ) );

	m_squadRoleCorner->GetSprite()->SignalFinished.Connect( this, &FDStrategyRolesScreen::OnAnimationFinished );
	m_squadRoleFreekick->GetSprite()->SignalFinished.Connect( this, &FDStrategyRolesScreen::OnAnimationFinished );
	m_squadRolePenalty->GetSprite()->SignalFinished.Connect( this, &FDStrategyRolesScreen::OnAnimationFinished );

	m_squadRoleCorner->Hide();
	m_squadRoleFreekick->Hide();
	m_squadRolePenalty->Hide();

	// Squad number sprites.
	m_captainNameSprite = FD_GAME_NEW lUISpriteWidget(m_rolesScreen, 0, "captainnamesprite", FDResourceManager::GetSprite("ui", "playerdot"), LUI_TOP_LEFT, lUIPoint(158, 121));
	m_penNameSprite = FD_GAME_NEW lUISpriteWidget(m_rolesScreen, 0, "penaltynamesprite", FDResourceManager::GetSprite("ui", "playerdot"), LUI_TOP_LEFT, lUIPoint(158, 133));
	m_freekickNameSprite = FD_GAME_NEW lUISpriteWidget(m_rolesScreen, 0, "freekicknamesprite", FDResourceManager::GetSprite("ui", "playerdot"), LUI_TOP_LEFT, lUIPoint(158, 146));
	m_cornerNameSprite = FD_GAME_NEW lUISpriteWidget(m_rolesScreen, 0, "cornerkicknamesprite", FDResourceManager::GetSprite("ui", "playerdot"), LUI_TOP_LEFT, lUIPoint(158, 159));

	m_captainPitchSprite = FD_GAME_NEW lUISpriteWidget(m_rolesScreen, 0, "captainpitchsprite", FDResourceManager::GetSprite("ui", "playerdot"), LUI_TOP_LEFT, lUIPoint(196, 84));
	m_penPitchSprite = FD_GAME_NEW lUISpriteWidget(m_rolesScreen, 0, "penaltypitchsprite", FDResourceManager::GetSprite("ui", "playerdot"), LUI_TOP_LEFT, lUIPoint(194, 36));
	m_freekickPitchSprite = FD_GAME_NEW lUISpriteWidget(m_rolesScreen, 0, "freekickpitchsprite", FDResourceManager::GetSprite("ui", "playerdot"), LUI_TOP_LEFT, lUIPoint(240, 51));
	m_cornerPitchSprite = FD_GAME_NEW lUISpriteWidget(m_rolesScreen, 0, "cornerkickpitchsprite", FDResourceManager::GetSprite("ui", "playerdot"), LUI_TOP_LEFT, lUIPoint(150, 7));

	m_dragWidget->PushToBack();

}

void FDStrategyRolesScreen::DoShow()
{
	m_rolesScreen->Show();
	m_rolesScreenSub->Show();

	m_squadRoleCorner->GetSprite()->Play(PLAY_ONCE);
	m_squadRoleCorner->Show();
	//m_squadRoleFreekick->GetSprite()->Play(PLAY_ONCE);
	m_squadRoleFreekick->Show();
	//m_squadRolePenalty->GetSprite()->Play(PLAY_ONCE);
	m_squadRolePenalty->Show();
	RefreshInformation();
  GetMainUI()->GetPlayerInfoSubScreen()->Hide();
	GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE
}

void FDStrategyRolesScreen::DoHide()
{
	m_rolesScreen->Hide();
	m_rolesScreenSub->Hide();

  if( m_matchPlay != NULL )
  {
	  m_matchPlay = NULL;
	  m_table->SetMatchPlay( NULL );
  }
}

void FDStrategyRolesScreen::SetMatchPlay( CMatchPlay * matchplay )
{
  m_matchPlay = matchplay;
  m_table->SetMatchPlay( matchplay );
}

static lUIRect GetActualTextRect(FDTextWidget *pWidget)
{
	lUIRect cReturn = pWidget->GetRect();
	lUIPoint cSize = pWidget->GetIdealSize();

	cReturn.w = cSize.x * (float)lSpriteManager::GetDisplayWidth();
	cReturn.h = cSize.y * (float)lSpriteManager::GetDisplayHeight();

	return cReturn;
}

void FDStrategyRolesScreen::Tick( float delta )
{
	if( m_pauseInterval > 0.0f )
	{
		m_pauseInterval -= delta;
		if( m_pauseInterval <= 0.0f )
		{
			// return to start of animation
			m_squadRoleCorner->GetSprite()->Stop();
			m_squadRoleFreekick->GetSprite()->Stop();
			m_squadRolePenalty->GetSprite()->Stop();
		}
	}
	else if( m_spriteInterval > 0.0f )
	{
		m_spriteInterval -= delta;
		if( m_spriteInterval <= 0.0f )
		{
			// play next anim
			switch( m_spritePlay )
			{
			case 0:
				m_squadRoleCorner->GetSprite()->Play(PLAY_ONCE);
				break;
			case 1:
				m_squadRoleFreekick->GetSprite()->Play(PLAY_ONCE);
				break;
			case 2:
				m_squadRolePenalty->GetSprite()->Play(PLAY_ONCE);
				break;
			}
		}
	}

	const luint32_t uiNormalDotColour = L_ARGB(0xFF, 169, 0, 0);
	m_captainNameSprite->GetSprite()->SetColour(uiNormalDotColour);
	m_penNameSprite->GetSprite()->SetColour(uiNormalDotColour);
	m_freekickNameSprite->GetSprite()->SetColour(uiNormalDotColour);
	m_cornerNameSprite->GetSprite()->SetColour(uiNormalDotColour);
	m_captainPitchSprite->GetSprite()->SetColour(uiNormalDotColour);
	m_penPitchSprite->GetSprite()->SetColour(uiNormalDotColour);
	m_freekickPitchSprite->GetSprite()->SetColour(uiNormalDotColour);
	m_cornerPitchSprite->GetSprite()->SetColour(uiNormalDotColour);

	// Determine if the player is on one of the roles
	const lUIRect cDropRect(m_dragWidget->GetCurrentPoint().x, m_dragWidget->GetCurrentPoint().y, 0, 0);
  const luint32_t uiDotColour = L_ARGB(0xFF, 0, 169, 0);

	if (cDropRect.InRect(GetActualTextRect(m_captainName)) || cDropRect.InRect(m_captainNameSprite->GetRect()) || cDropRect.InRect(m_captainPitchSprite->GetRect()))
	{
		 m_captainNameSprite->GetSprite()->SetColour(uiDotColour);
		 m_captainPitchSprite->GetSprite()->SetColour(uiDotColour);
	}
	else if (cDropRect.InRect(GetActualTextRect(m_penName)) || cDropRect.InRect(m_penNameSprite->GetRect()) || cDropRect.InRect(m_penPitchSprite->GetRect()))
	{
		m_penNameSprite->GetSprite()->SetColour(uiDotColour);
		m_penPitchSprite->GetSprite()->SetColour(uiDotColour);
	}
	else if (cDropRect.InRect(GetActualTextRect(m_freekickName)) || cDropRect.InRect(m_freekickNameSprite->GetRect()) || cDropRect.InRect(m_freekickPitchSprite->GetRect()))
	{
		m_freekickNameSprite->GetSprite()->SetColour(uiDotColour);
		m_freekickPitchSprite->GetSprite()->SetColour(uiDotColour);
	}
	else if (cDropRect.InRect(GetActualTextRect(m_cornerName)) || cDropRect.InRect(m_cornerNameSprite->GetRect()) || cDropRect.InRect(m_cornerPitchSprite->GetRect()))
	{
    m_cornerNameSprite->GetSprite()->SetColour(uiDotColour);
	  m_cornerPitchSprite->GetSprite()->SetColour(uiDotColour);
	}
}


void FDStrategyRolesScreen::SetClub(CClub *pClub)
{
	m_pClub = pClub;
	m_table->SetClub(pClub);

	RefreshInformation();
}

void FDStrategyRolesScreen::RefreshInformation()
{

  // Update table
  m_table->RefreshTable();

	// set up roles
	CPlayerSquad& cSquad = m_pClub->GetFirstTeamSquad();

	// TODO LOCALE
	m_captainName->SetText( lUIText( "##404040Captain" ) );
	m_penName->SetText( lUIText( "##404040Penalty" ) );
	m_freekickName->SetText( lUIText( "##404040Free Kick" ) );
	m_cornerName->SetText( lUIText( "##404040Corner Kick" ) );

	// Set the sprites of the widgets next to the names.
	char sSpriteName[32];

	const ushort usCaptainSquadNum = cSquad.GetCaptainSquadNum();
	if ((usCaptainSquadNum != NOPLAYER) && (usCaptainSquadNum < PLAYERSINTEAM))
	{
		::sprintf(sSpriteName, "playerdot%d", (int)usCaptainSquadNum + 1);
		m_captainNameSprite->SetSprite(FDResourceManager::GetSprite("ui", sSpriteName));
		m_captainPitchSprite->SetSprite(FDResourceManager::GetSprite("ui", sSpriteName));
	}
	else
	{
		m_captainNameSprite->SetSprite(FDResourceManager::GetSprite("ui", "playerdot"));
		m_captainPitchSprite->SetSprite(FDResourceManager::GetSprite("ui", "playerdot"));
	}

	const ushort usPenaltyTakerSquadNum = cSquad.GetPenaltyTakerSquadNum();
	if ((usPenaltyTakerSquadNum != NOPLAYER) && (usPenaltyTakerSquadNum < PLAYERSINTEAM))
	{
		::sprintf(sSpriteName, "playerdot%d", (int)usPenaltyTakerSquadNum + 1);
		m_penNameSprite->SetSprite(FDResourceManager::GetSprite("ui", sSpriteName));
		m_penPitchSprite->SetSprite(FDResourceManager::GetSprite("ui", sSpriteName));
	}
	else
	{
		m_penNameSprite->SetSprite(FDResourceManager::GetSprite("ui", "playerdot"));
		m_penPitchSprite->SetSprite(FDResourceManager::GetSprite("ui", "playerdot"));
	}

	const ushort usFreeKickTakerSquadNum = cSquad.GetFreeKickTakerSquadNum();
	if ((usFreeKickTakerSquadNum != NOPLAYER) && (usFreeKickTakerSquadNum < PLAYERSINTEAM))
	{
		::sprintf(sSpriteName, "playerdot%d", (int)usFreeKickTakerSquadNum + 1);
		m_freekickNameSprite->SetSprite(FDResourceManager::GetSprite("ui", sSpriteName));
		m_freekickPitchSprite->SetSprite(FDResourceManager::GetSprite("ui", sSpriteName));
	}
	else
	{
		m_freekickNameSprite->SetSprite(FDResourceManager::GetSprite("ui", "playerdot"));
		m_freekickPitchSprite->SetSprite(FDResourceManager::GetSprite("ui", "playerdot"));
	}

	const ushort usCornerTakerSquadNum = cSquad.GetCornerTakerSquadNum();
	if ((usCornerTakerSquadNum != NOPLAYER) && (usCornerTakerSquadNum < PLAYERSINTEAM))
	{
		::sprintf(sSpriteName, "playerdot%d", (int)usCornerTakerSquadNum + 1);
		m_cornerNameSprite->SetSprite(FDResourceManager::GetSprite("ui", sSpriteName));
		m_cornerPitchSprite->SetSprite(FDResourceManager::GetSprite("ui", sSpriteName));
	}
	else
	{
		m_cornerNameSprite->SetSprite(FDResourceManager::GetSprite("ui", "playerdot"));
		m_cornerPitchSprite->SetSprite(FDResourceManager::GetSprite("ui", "playerdot"));
	}

	// NOTE: this isn't 100% appropriate since the sprites may be shared. However, unless something comes along and uses these same sprites on this
	// screen, this will actually show up correctly...
	const luint32_t uiDotColour = L_ARGB(0xFF, 169, 0, 0);
	m_captainNameSprite->GetSprite()->SetColour(uiDotColour);
	m_penNameSprite->GetSprite()->SetColour(uiDotColour);
	m_freekickNameSprite->GetSprite()->SetColour(uiDotColour);
	m_cornerNameSprite->GetSprite()->SetColour(uiDotColour);
	m_captainPitchSprite->GetSprite()->SetColour(uiDotColour);
	m_penPitchSprite->GetSprite()->SetColour(uiDotColour);
	m_freekickPitchSprite->GetSprite()->SetColour(uiDotColour);
	m_cornerPitchSprite->GetSprite()->SetColour(uiDotColour);
}

void FDStrategyRolesScreen::OnAnimationFinished()
{
	m_spritePlay++;

	if( m_spritePlay >= 3 )
		m_spritePlay = 0;

	m_spriteInterval = 1.0f;
	m_pauseInterval = 1.0f;

	// return to start of animation
	//m_squadRoleCorner->GetSprite()->Stop();
	//m_squadRoleFreekick->GetSprite()->Stop();
	//m_squadRolePenalty->GetSprite()->Stop();
}


void FDStrategyRolesScreen::OnPlayerDropped(CPlayer *pPlayer, lUIPoint cDropLocation)
{
	// Determine if the player was dropped on one of the roles. If so, then set that role for the club.
	CPlayerSquad &cFirstTeam = m_pClub->GetFirstTeamSquad();
	const lUIRect cDropRect(cDropLocation.x + m_table->GetRect().x, cDropLocation.y + m_table->GetRect().y, 0, 0);
	const ushort usPlayerNumInSquad = cFirstTeam.DoFindPlayerIDInSquad(pPlayer->DoFindID());

	if (usPlayerNumInSquad == NOPLAYER || usPlayerNumInSquad > 10 )
		return;

	if (cDropRect.InRect(GetActualTextRect(m_captainName)) || cDropRect.InRect(m_captainNameSprite->GetRect()) || cDropRect.InRect(m_captainPitchSprite->GetRect()))
	{
		cFirstTeam.SetCaptainSquadNum(usPlayerNumInSquad);
		RefreshInformation();
	}
	else if (cDropRect.InRect(GetActualTextRect(m_penName)) || cDropRect.InRect(m_penNameSprite->GetRect()) || cDropRect.InRect(m_penPitchSprite->GetRect()))
	{
		cFirstTeam.SetPenaltyTakerSquadNum(usPlayerNumInSquad);
		RefreshInformation();
	}
	else if (cDropRect.InRect(GetActualTextRect(m_freekickName)) || cDropRect.InRect(m_freekickNameSprite->GetRect()) || cDropRect.InRect(m_freekickPitchSprite->GetRect()))
	{
		cFirstTeam.SetFreeKickTakerSquadNum(usPlayerNumInSquad);
		RefreshInformation();
	}
	else if (cDropRect.InRect(GetActualTextRect(m_cornerName)) || cDropRect.InRect(m_cornerNameSprite->GetRect()) || cDropRect.InRect(m_cornerPitchSprite->GetRect()))
	{
		cFirstTeam.SetCornerTakerSquadNum(usPlayerNumInSquad);
		RefreshInformation();
	}
}

void FDStrategyRolesScreen::OnSignalSelect( FDTableWidget * table, CPlayer *pPlayer, int row)
{
  GetMainUI()->HideSubMessage();
  if( pPlayer != NULL )
  {
    lLogFmt("Player: %d - %s\n", row, pPlayer->GetName());
    GetMainUI()->GetPlayerInfoSubScreen()->Show();
    GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( pPlayer );
  }
  else
  {
    lLogFmt("Not a valid playeridx!\n");
    GetMainUI()->GetPlayerInfoSubScreen()->Hide();
    GetMainUI()->DisplaySubMessage("Select Player"); // TODO LOCALE
  }
}
