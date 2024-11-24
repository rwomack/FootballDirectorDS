//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdeditmodescreen.h
// Description : Screen for editing club and player names.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDEDITMODESCREEN_H_INCLUDED
#define FDEDITMODESCREEN_H_INCLUDED

#include <fdscreen.h>
#include <fdeditmodedata.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

class FDMainMenuUI;
class FDButtonWidget;
class FDTableWidget;
class FDComboWidget;
class lUIControlWidget;

//////////////////////////////////////////////////////////////////////////
// FDEditModeScreen - Screen for editing club and player names
//////////////////////////////////////////////////////////////////////////
class FDEditModeScreen : public FDScreen, public lHasSlots<>
{
public:
	FDEditModeScreen( FDMainMenuUI *parent );
	virtual ~FDEditModeScreen();

	// FDScreen interface
	virtual void DoShow();
	virtual void DoHide();

private:
	void Init();

	void RefreshScreen();

	void OnSaveChangesButton(lUIControlWidget *);
	void OnCancelChangesButton(lUIControlWidget *);

	void OnEditClubName(lUIControlWidget *);
	void OnEditPlayerName(lUIControlWidget *);

	void OnResetButton(lUIControlWidget *);

	void OnSelectDivision( FDComboWidget *, int index );
	void OnSelectTeam( FDTableWidget *, int row );
	void OnSelectPlayer(FDTableWidget *, int row);

	void HandleDialogResult();
	void RefreshSelectedNames();

	lString GetClubName(CClub *pClub);
	lString GetPlayerName(CPlayer *pPlayer);
	lString GetPlayerSurname(CPlayer *pPlayer);
	char GetPlayerFirstInitial(CPlayer *pPlayer);

	lUIScreenWidget *m_editScreen;
	lUIScreenWidget *m_editScreenSub;
	FDButtonWidget *m_editClubNameButton;
	FDButtonWidget *m_editPlayerNameButton;
	FDButtonWidget *m_saveChangesButton;
	FDButtonWidget *m_cancelChangesButton;
	FDButtonWidget *m_resetToOriginalButton;
	FDTableWidget *m_teamTable;
	FDTableWidget *m_playerTable;
	FDComboWidget *m_divisionSelector;

	FDEditModeData m_editData;
	bool m_bInEditName : 1;
	bool m_bInResetDialog : 1;
};

END_L_NAMESPACE

#endif // #ifndef FDEDITMODESCREEN_H_INCLUDED

