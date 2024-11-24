//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdstrategyrolesscreen.h
// Description : Screen for setting player roles.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDSTRATEGYROLESSCREEN_H_INCLUDED
#define FDSTRATEGYROLESSCREEN_H_INCLUDED

#include <fdscreen.h>
#include <framework/lsigslot.h>

class CClub;

BEGIN_L_NAMESPACE

class FDMainMenuUI;
class FDButtonWidget;
class lUISpriteWidget;
class FDTextWidget;
class FDSquadTableWidget;
class FDDragWidget;


//////////////////////////////////////////////////////////////////////////
// FDStrategyRolesScreen - Screen for setting the player roles
//////////////////////////////////////////////////////////////////////////
class FDStrategyRolesScreen : public FDScreen, public lHasSlots<>
{
public:
	FDStrategyRolesScreen( FDMainMenuUI *parent );
	virtual ~FDStrategyRolesScreen();

	// FDScreen interface
	virtual void DoShow();
	virtual void DoHide();

	virtual void Tick(float delta);

	void SetClub(CClub *pClub);
	void SetMatchPlay( CMatchPlay * matchplay );

private:
	void Init();
	void RefreshInformation();

	void OnAnimationFinished();
	void OnPlayerDropped(CPlayer *pPlayer, lUIPoint cDropLocation);
  void OnSignalSelect( FDTableWidget * table, CPlayer *pPlayer, int row);

	CClub *m_pClub;

	lUIScreenWidget *m_rolesScreen;
	lUIScreenWidget *m_rolesScreenSub;

	FDSquadTableWidget *m_table;

	lUISpriteWidget       * m_squadRoles;

	lUISpriteWidget       * m_squadRoleCorner;
	lUISpriteWidget       * m_squadRoleFreekick;
	lUISpriteWidget       * m_squadRolePenalty;

	FDTextWidget          * m_captainName;
	FDTextWidget          * m_penName;
	FDTextWidget          * m_freekickName;
	FDTextWidget          * m_cornerName;

	lUISpriteWidget		  * m_captainNameSprite;
	lUISpriteWidget		  * m_penNameSprite;
	lUISpriteWidget		  * m_freekickNameSprite;
	lUISpriteWidget		  * m_cornerNameSprite;

	lUISpriteWidget		  * m_captainPitchSprite;
	lUISpriteWidget		  * m_penPitchSprite;
	lUISpriteWidget		  * m_freekickPitchSprite;
	lUISpriteWidget		  * m_cornerPitchSprite;

	int                   m_spritePlay;
	float                 m_spriteInterval;
	float                 m_pauseInterval;
	FDDragWidget *		  m_dragWidget;
	CMatchPlay * m_matchPlay;
};

END_L_NAMESPACE

#endif // #ifndef FDSTRATEGYROLESSCREEN_H_INCLUDED
