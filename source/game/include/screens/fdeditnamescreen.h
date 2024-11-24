//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdeditnamescreen.h
// Description : Screen with a keyboard for actually editing club and player names.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDEDITNAMESCREEN_H_INCLUDED
#define FDEDITNAMESCREEN_H_INCLUDED

#include <fdscreen.h>

class CClub;
class CPlayer;

BEGIN_L_NAMESPACE

class FDMainMenuUI;
class FDButtonWidget;
class FDTableWidget;
class FDComboWidget;
class lUIControlWidget;
class FDKeyboardWidget;

//////////////////////////////////////////////////////////////////////////
// FDEditNameScreen - Screen that actually has the keyboard for editing club and player names.
//////////////////////////////////////////////////////////////////////////
class FDEditNameScreen : public FDScreen, public lHasSlots<>
{
public:
	enum EditMode
	{
		FDEM_CLUB_NAME,
		FDEM_PLAYER_NAME,
		FDEM_INVALID
	};

	enum ResultType
	{
		FDRT_SAVED,
		FDRT_CANCELLED,
		FDRT_RESET,
		FDRT_INVALID
	};

	FDEditNameScreen( FDMainMenuUI *parent );
	virtual ~FDEditNameScreen();

	void SetPlayer(const lString lSurname, const char ucInitial);
	void SetClub(const lString lClubName);
	void SetLengthLimit(const int iLimit);

	void SetResetButtonEnabled(const bool bEnabled);
	ResultType GetDialogResult() const;
	const lString &GetName() const;
	lString GetInitial() const;

	EditMode GetEditMode() const;

	// FDScreen interface
	virtual void DoShow();
	virtual void DoHide();

private:
	void Init();

	void OnSaveChangesButton(lUIControlWidget *);
	void OnCancelChangesButton(lUIControlWidget *);
	void OnResetButton(lUIControlWidget *);

	void OnInitialChange(FDComboWidget *, int index);

	void OnKeyboardPressed(char cPressed);

	lUIScreenWidget *m_editScreen;
	lUIScreenWidget *m_editScreenSub;
	FDButtonWidget *m_saveChangesButton;
	FDButtonWidget *m_cancelChangesButton;
	FDButtonWidget *m_resetButton;

	lUITextWidget *m_name;
	FDComboWidget *m_initialSelector;
	FDKeyboardWidget *m_keyboard;

	bool m_bResetButtonEnabled;
	ResultType m_eDialogResult;
	lString m_editedName;
	EditMode m_currentMode;
	int m_iLengthLimit;
};

END_L_NAMESPACE

#endif // #ifndef FDEDITNAMESCREEN_H_INCLUDED

