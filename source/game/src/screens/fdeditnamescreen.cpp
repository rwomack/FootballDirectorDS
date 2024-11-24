//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdeditnamescreen.cpp
// Description : Screen with a keyboard for actually editing club and player names.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdresourcemanager.h>
#include <fdbuttonwidget.h>
#include <fdcombowidget.h>
#include <fdtablewidget.h>
#include <fdkeyboardwidget.h>
#include <fdbackingwidget.h>
#include <ui/luitextwidget.h>
#include <fdmainmenuui.h>
#include <screens/FDEditNameScreen.h>
#include <ui/luiscreenwidget.h>
#include <framework/lsprite.h>

using namespace L_NAMESPACE_NAME;

//////////////////////////////////////////////////////////////////////////
// Constructor.
//////////////////////////////////////////////////////////////////////////
FDEditNameScreen::FDEditNameScreen( FDMainMenuUI *parent ) : FDScreen(parent),
	m_currentMode(FDEM_INVALID), m_bResetButtonEnabled(false), m_eDialogResult(FDRT_INVALID), m_iLengthLimit(-1)
{
	Init();
}

//////////////////////////////////////////////////////////////////////////
// Destructor.
//////////////////////////////////////////////////////////////////////////
FDEditNameScreen::~FDEditNameScreen()
{
}

//////////////////////////////////////////////////////////////////////////
// Called when the screen is shown.
//////////////////////////////////////////////////////////////////////////
void FDEditNameScreen::DoShow()
{
	lAssert(m_currentMode != FDEM_INVALID);

	m_eDialogResult = FDRT_INVALID;
	if (m_bResetButtonEnabled == true)
		m_resetButton->Enable();
	else
		m_resetButton->Disable();

	switch (m_currentMode)
	{
	case FDEM_CLUB_NAME:
		m_initialSelector->Hide();
		break;
	case FDEM_PLAYER_NAME:
		m_initialSelector->Show();
		break;
	default:
		lAssert(false && "invalid edit mode");
		break;
	}

	m_name->SetText(lUIText(m_editedName));

	m_editScreen->Show();
	m_editScreenSub->Show();
}

//////////////////////////////////////////////////////////////////////////
// Called when the screen is hidden.
//////////////////////////////////////////////////////////////////////////
void FDEditNameScreen::DoHide()
{
	m_editScreen->Hide();
	m_editScreenSub->Hide();
}

//////////////////////////////////////////////////////////////////////////
// Called during construction to initialize the contents of the screen.
//////////////////////////////////////////////////////////////////////////
void FDEditNameScreen::Init()
{
	m_editScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "edit_name_screen" );
	m_editScreen->Hide();

	// top screen
	m_editScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "edit_screen_sub" );
	m_editScreenSub->Hide();

	// onscreen keyboard
  FDBackingWidget * keyboardBacking = FD_GAME_NEW FDBackingWidget( m_editScreen, 0, "newgame_keyboard_back", L_ARGB( 80, 45, 95, 85 ), LUI_LEFT_TOP, lUIPoint(20, 40 ), lUIPoint( 236, 110) );
  keyboardBacking->SetLines( 3, BLACK );
	m_keyboard = FD_GAME_NEW FDKeyboardWidget(m_editScreen, 0, "edit_keyboard", LUI_TOP_LEFT, lUIPoint(25, 50));
	m_keyboard->SignalKeyPressed.Connect(this, &FDEditNameScreen::OnKeyboardPressed);

	// First name initial selector
	m_initialSelector = FD_GAME_NEW FDComboWidget(m_editScreen, 0, "edit_initial", lUIText(""), LUI_TOP_CENTER, lUIPoint(138, 10));
	m_initialSelector->SignalSelection.Connect(this, &FDEditNameScreen::OnInitialChange);
	for (int i = 'A'; i <= 'Z'; i++)
	{
		char initial_text[2];
		initial_text[0] = (char)i;
		initial_text[1] = 0;
		m_initialSelector->AddItem(lUIText(initial_text));
	}

	// name display
  FDBackingWidget * nameBacking = FD_GAME_NEW FDBackingWidget( m_editScreenSub, 0, "newgame_keyboard_back", L_ARGB( 80, 45, 95, 85 ), LUI_LEFT_TOP, lUIPoint(0, 90 ), lUIPoint( 256, 35) );
  nameBacking->SetLines( 3, BLACK );
	m_name = FD_GAME_NEW lUITextWidget(m_editScreenSub, 0, "edit_name", lUIText(""), LUI_CENTER_CENTER, lUIPoint((float)lSpriteManager::GetDisplayWidth() / 2.0f, 105.0f), lUIPoint( 256, 30 ) );
	m_name->SetFont(FDResourceManager::GetFont("font26", "font26"));
	m_name->SetText( lUIText( "E.Novales" ));
	m_name->SetShadow( true, L_ARGB( 127, 0, 0, 0 ) );

	// buttons
	m_saveChangesButton = FD_GAME_NEW FDButtonWidget(m_editScreen, 0, "save_changes_button", FDResourceManager::GetSprite("ui", "button75"),
		lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(23, 156));
	m_cancelChangesButton = FD_GAME_NEW FDButtonWidget(m_editScreen, 0, "cancel_changes_button", FDResourceManager::GetSprite("ui", "button75"),
		lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(101, 156));
	m_resetButton = FD_GAME_NEW FDButtonWidget(m_editScreen, 0, "reset_name_button", FDResourceManager::GetSprite("ui", "button75"),
		lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(178, 156));

  m_saveChangesButton->SetText(IDS_EDIT_NAME_SAVE_CHANGES_BUTTON);
  m_saveChangesButton->SetTextUpper();

  m_cancelChangesButton->SetText(IDS_EDIT_NAME_CANCEL_CHANGES_BUTTON);
  m_cancelChangesButton->SetTextUpper();

  m_resetButton->SetText(IDS_EDIT_NAME_RESET_BUTTON);
  m_resetButton->SetTextUpper();

	m_saveChangesButton->SignalControlCommand.Connect(this, &FDEditNameScreen::OnSaveChangesButton);
	m_cancelChangesButton->SignalControlCommand.Connect(this, &FDEditNameScreen::OnCancelChangesButton);
	m_resetButton->SignalControlCommand.Connect(this, &FDEditNameScreen::OnResetButton);
}

//////////////////////////////////////////////////////////////////////////
// Handler for the Save Changes button.
//////////////////////////////////////////////////////////////////////////
void FDEditNameScreen::OnSaveChangesButton(lUIControlWidget *pWidget)
{
	if (m_saveChangesButton->IsDisabled() == true)
		return;

	m_eDialogResult = FDRT_SAVED;

	GetMainUI()->PopScreen();
}

FDEditNameScreen::ResultType FDEditNameScreen::GetDialogResult() const
{
	return m_eDialogResult;
}

const lString &FDEditNameScreen::GetName() const
{
	return m_editedName;
}

lString FDEditNameScreen::GetInitial() const
{
	return m_initialSelector->GetItem(m_initialSelector->GetSelectedItem()).str;
}


//////////////////////////////////////////////////////////////////////////
// Handler for the Cancel Changes button.
//////////////////////////////////////////////////////////////////////////
void FDEditNameScreen::OnCancelChangesButton(lUIControlWidget *pWidget)
{
	if (m_cancelChangesButton->IsDisabled() == true)
		return;

	m_eDialogResult = FDRT_CANCELLED;

	GetMainUI()->PopScreen();
}

//////////////////////////////////////////////////////////////////////////
// Handler for the reset button.
//////////////////////////////////////////////////////////////////////////
void FDEditNameScreen::OnResetButton(lUIControlWidget *pWidget)
{
	if (m_resetButton->IsDisabled() == true)
		return;

	m_eDialogResult = FDRT_RESET;

	GetMainUI()->PopScreen();
}

//////////////////////////////////////////////////////////////////////////
// Handler for when the selected initial changes.
//////////////////////////////////////////////////////////////////////////
void FDEditNameScreen::OnInitialChange(FDComboWidget *, int index)
{
}

void FDEditNameScreen::SetPlayer(const lString lSurname, const char ucInitial)
{
	m_currentMode = FDEM_PLAYER_NAME;
	m_editedName = lSurname;
	m_initialSelector->SetSelectedItem(ucInitial - 'A');
}

void FDEditNameScreen::SetClub(const lString lClubName)
{
	m_currentMode = FDEM_CLUB_NAME;
	m_editedName = lClubName;
}

void FDEditNameScreen::OnKeyboardPressed(char cPressed)
{
    if( cPressed == 8 )
    {
		// backspace
		if( m_editedName.Size() )
		{
			FDSound::PlaySoundEffect( FD_SFX_UI_KEYBOARD_BACKSPACE );
			m_editedName = m_editedName.Left( m_editedName.Size() - 1 );
		}

		if( m_editedName.Size() )
		{
			// set to new text
			m_name->SetText( lUIText( m_editedName.CStr() ) );
		}
		else
		{
			// set to _ to represent empty
			m_name->SetText( lUIText( "_" ) );
		}
    }
	else
    {
		// set to current
		m_editedName += lString( cPressed );

		if((m_iLengthLimit >= 0) && (m_editedName.Size() > m_iLengthLimit ))
		{
      lLogFmt("Editedname: %s (%d) - capping to %d\n", m_editedName.CStr(), m_editedName.Size(), m_iLengthLimit);
			m_editedName = m_editedName.Left( m_iLengthLimit );
		}

    // check that length of text on screen isnt too large

    lSharedPtr<lFont> font = m_name->GetFont();
    int Width = 0, Height = 0;
    font->GetStringBound( Width, Height, m_editedName.CStr() );

    if( Width > 240 )
    {
      m_editedName = m_editedName.Left( m_editedName.Size()-1 );
    }

		m_name->SetText( lUIText( m_editedName.CStr() ) );

		FDSound::PlaySoundEffect( FD_SFX_UI_KEYBOARD_CLICK );
    }

	if (m_editedName.IsEmpty())
		m_saveChangesButton->Disable();
  else
		m_saveChangesButton->Enable();
}

//////////////////////////////////////////////////////////////////////////
// Used to set whether or not the reset button is enabled for a particular name.
//////////////////////////////////////////////////////////////////////////
void FDEditNameScreen::SetResetButtonEnabled(const bool bEnabled)
{
	m_bResetButtonEnabled = bEnabled;
}

FDEditNameScreen::EditMode FDEditNameScreen::GetEditMode() const
{
	return m_currentMode;
}

void FDEditNameScreen::SetLengthLimit(const int iLimit)
{
	m_iLengthLimit = iLimit;
}
