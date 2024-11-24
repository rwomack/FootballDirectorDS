//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdeditmodescreen.cpp
// Description : Screen for editing club and player names.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdresourcemanager.h>
#include <fdbuttonwidget.h>
#include <fdcombowidget.h>
#include <fdscrollbarwidget.h>
#include <fdtablewidget.h>
#include <fdmainmenuui.h>
#include <screens/fdeditmodescreen.h>
#include <screens/fdeditnamescreen.h>
#include <ui/luiscreenwidget.h>

using namespace L_NAMESPACE_NAME;

//////////////////////////////////////////////////////////////////////////
// Constructor.
//////////////////////////////////////////////////////////////////////////
FDEditModeScreen::FDEditModeScreen( FDMainMenuUI *parent ) : FDScreen(parent), m_bInEditName(false), m_bInResetDialog(false)
{
	Init();
}

//////////////////////////////////////////////////////////////////////////
// Destructor.
//////////////////////////////////////////////////////////////////////////
FDEditModeScreen::~FDEditModeScreen()
{
}

//////////////////////////////////////////////////////////////////////////
// Called when the screen is shown.
//////////////////////////////////////////////////////////////////////////
void FDEditModeScreen::DoShow()
{
	if ((m_bInEditName == false) && (m_bInResetDialog == false))
	{
		// Copy edit data from the current options.
		m_editData = GetMainUI()->GetSavedEditModeData();

		RefreshScreen();
	}
	else if (m_bInEditName == true)
	{
		HandleDialogResult();

		// Refresh the screen data.
		RefreshSelectedNames();
	}

	m_bInEditName = false;
	m_bInResetDialog = false;

	m_editScreen->Show();
	m_editScreenSub->Show();
}

//////////////////////////////////////////////////////////////////////////
// Called when the screen is hidden.
//////////////////////////////////////////////////////////////////////////
void FDEditModeScreen::DoHide()
{
	m_editScreen->Hide();
	m_editScreenSub->Hide();
}

//////////////////////////////////////////////////////////////////////////
// Called during construction to initialize the contents of the screen.
//////////////////////////////////////////////////////////////////////////
void FDEditModeScreen::Init()
{
	m_editScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "edit_screen" );
	m_editScreen->Hide();

	// top screen
	m_editScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "edit_screen_sub" );
	m_editScreenSub->Hide();

	// Setup for the team list and division selector.
	m_teamTable = FD_GAME_NEW FDTableWidget( m_editScreen,  0, "teamtable",
		FDTableWidget::TABLE_FLAG_BORDER | FDTableWidget::TABLE_FLAG_BACKGROUND |
		FDTableWidget::TABLE_FLAG_ROW_SELECT  | FDTableWidget::TABLE_FLAG_CELLBACKGROUND | FDTableWidget::TABLE_FLAG_CUT_OFF,
    LUI_TOP_LEFT, lUIPoint( 24, 34 ), lUIPoint( 98 - GetDefaultScrollBarWidth(false) * 1.5f, 72 ) );

  FDScrollBarWidget *pTeamScrollBar = FD_GAME_NEW FDScrollBarWidget(m_editScreen, 0, "teamtablescroll", LUI_TOP_LEFT, lUIPoint(24 + 98, 34), lUIPoint(GetDefaultScrollBarWidth(false) * 1.5f, 75));
	pTeamScrollBar->AlignTo( m_teamTable );
	m_teamTable->SetScrollBar(pTeamScrollBar);

	m_teamTable->AddColumn( lUICoordPercent( 1.0f ), LUI_LEFT_CENTER );
	m_teamTable->SetSortFunctionForColumn(0, FDTableWidget::GenericStringSort);
	m_teamTable->SignalRowSelect.Connect( this, &FDEditModeScreen::OnSelectTeam );

	// show division combo box
	m_divisionSelector = FD_GAME_NEW FDComboWidget( m_editScreen,  0, "selectteam_divcombo", lUIText(""), LUI_TOP_CENTER, lUIPoint( 138, 9 ) );
	m_divisionSelector->SetAllToUpper( true );

	// hook up signal before we set selected item
	m_divisionSelector->SignalSelection.Connect( this, &FDEditModeScreen::OnSelectDivision );

	// Setup for the player list.
	m_playerTable = FD_GAME_NEW FDTableWidget(m_editScreen, 0, "playertable",
		FDTableWidget::TABLE_FLAG_BORDER | FDTableWidget::TABLE_FLAG_BACKGROUND |
		FDTableWidget::TABLE_FLAG_ROW_SELECT | FDTableWidget::TABLE_FLAG_CELLBACKGROUND | FDTableWidget::TABLE_FLAG_CUT_OFF,
		LUI_TOP_LEFT, lUIPoint(142, 34), lUIPoint( 100 - 4, 72 ) );

  FDScrollBarWidget *pPlayerScrollBar = FD_GAME_NEW FDScrollBarWidget(m_editScreen, 0, "playertablescroll", LUI_TOP_LEFT, lUIPoint(147 + 100 - 8, 34), lUIPoint(GetDefaultScrollBarWidth(false) * 1.5f, 75));
	pPlayerScrollBar->AlignTo( m_playerTable );
	m_playerTable->SetScrollBar(pPlayerScrollBar);
	m_playerTable->AddColumn(lUICoordPercent(1.0f), LUI_LEFT_CENTER);
	m_playerTable->SetSortFunctionForColumn(0, FDTableWidget::GenericFirstInitialLastNameSort);
	m_playerTable->SignalRowSelect.Connect(this, &FDEditModeScreen::OnSelectPlayer);

	// buttons
	m_saveChangesButton = FD_GAME_NEW FDButtonWidget(m_editScreen, 0, "save_changes_button", FDResourceManager::GetSprite("ui", "button75"),
		lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(23, 156));
	m_cancelChangesButton = FD_GAME_NEW FDButtonWidget(m_editScreen, 0, "cancel_changes_button", FDResourceManager::GetSprite("ui", "button75"),
		lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(101, 156));
	m_editClubNameButton = FD_GAME_NEW FDButtonWidget(m_editScreen, 0, "edit_club_name_button", FDResourceManager::GetSprite("ui", "button75"),
		lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(35, 113));
	m_editPlayerNameButton = FD_GAME_NEW FDButtonWidget(m_editScreen, 0, "edit_player_name_button", FDResourceManager::GetSprite("ui", "button75"),
		lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(161, 113));
	m_resetToOriginalButton = FD_GAME_NEW FDButtonWidget(m_editScreen, 0, "reset_to_original_button", FDResourceManager::GetSprite("ui", "button75"),
		lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint(178, 156));

  m_saveChangesButton->SetText(lUIText(IDS_EDIT_MODE_SAVE_CHANGES_BUTTON));
  m_saveChangesButton->SetTextUpper();

  m_cancelChangesButton->SetText(lUIText(IDS_EDIT_MODE_CANCEL_CHANGES_BUTTON));
  m_cancelChangesButton->SetTextUpper();

  m_editClubNameButton->SetText(lUIText(IDS_EDIT_MODE_EDIT_CLUB_NAME_BUTTON));
  m_editClubNameButton->SetTextUpper();

  m_editPlayerNameButton->SetText(lUIText(IDS_EDIT_MODE_EDIT_PLAYER_NAME_BUTTON));
  m_editPlayerNameButton->SetTextUpper();

  m_resetToOriginalButton->SetText(lUIText(IDS_EDIT_MODE_RESET_ALL_BUTTON));
  m_resetToOriginalButton->SetTextUpper();

	//m_cancelChangesButton->Disable();

	m_saveChangesButton->SignalControlCommand.Connect(this, &FDEditModeScreen::OnSaveChangesButton);
	m_cancelChangesButton->SignalControlCommand.Connect(this, &FDEditModeScreen::OnCancelChangesButton);
	m_resetToOriginalButton->SignalControlCommand.Connect(this, &FDEditModeScreen::OnResetButton);
	m_editClubNameButton->SignalControlCommand.Connect(this, &FDEditModeScreen::OnEditClubName);
	m_editPlayerNameButton->SignalControlCommand.Connect(this, &FDEditModeScreen::OnEditPlayerName);
}

//////////////////////////////////////////////////////////////////////////
// Handler for the Save Changes button.
//////////////////////////////////////////////////////////////////////////
void FDEditModeScreen::OnSaveChangesButton(lUIControlWidget *pWidget)
{
	if (m_saveChangesButton->IsDisabled() == true)
		return;

	// Save out the edit data so that it will get used for the next new game.
	GetMainUI()->GetSavedEditModeData() = m_editData;

	if (m_cancelChangesButton->IsEnabled() == true)
	{
    //// Save out the changes immediately.
    //GetMainUI()->EditModeSave(this);
    GetMainUI()->SwitchToScreenCombo(SCREENCOMBO_SAVE_EDITMODE, false);
	}

  GetMainUI()->SwitchToScreenCombo(SCREENCOMBO_MAINMENU, false);
}

//////////////////////////////////////////////////////////////////////////
// Handler for the Cancel Changes button.
//////////////////////////////////////////////////////////////////////////
void FDEditModeScreen::OnCancelChangesButton(lUIControlWidget *pWidget)
{
	if (m_cancelChangesButton->IsDisabled() == true)
		return;

  GetMainUI()->SwitchToScreenCombo(SCREENCOMBO_MAINMENU, false);
}

//////////////////////////////////////////////////////////////////////////
// Handler for the Reset button.
//////////////////////////////////////////////////////////////////////////
void FDEditModeScreen::OnResetButton(lUIControlWidget *pWidget)
{
	if (m_resetToOriginalButton->IsDisabled() == true)
		return;

	m_bInResetDialog = true;

  if (GetMainUI()->UserMessageBox(IDS_EDIT_MODE_CONFIRM_RESET_DIALOG, MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		GetMainUI()->ResetEditModeData();
		m_editData.Clear();

    GetMainUI()->UserMessageBox(IDS_EDIT_MODE_RESET_COMPLETED_DIALOG, MB_OK);
		RefreshScreen();
    //m_cancelChangesButton->Disable();
	}

	m_bInResetDialog = false;
}

//////////////////////////////////////////////////////////////////////////
// Handler for when the selected division is changed.
//////////////////////////////////////////////////////////////////////////
void FDEditModeScreen::OnSelectDivision( FDComboWidget *, int index )
{
	//
	lLogFmt("Division %d selected\n", index);

	// deselect
	m_teamTable->SelectRow( -1 );

	// selected division
	CClubList &clubList = WorldData().GetClubList();
	const CDivision &division = WorldData().GetDivisionList().GetAt( index );

	lArray<ClubID> divisionTeamList;

	for(int i = 0; i < clubList.GetSize(); i++)
	{
		CClub & club = clubList[i];

		if( club.IsNonLeagueClub() )
		{
			// skip non-league
			continue;
		}

		if( club.GetDivisionID() == index )
		{
			divisionTeamList.Push( club.DoFindID() );

			lLogFmt("  %d: %s\n", i, club.GetName());
		}
	}

	// set table to represent division
	m_teamTable->SetRows( divisionTeamList.Size() );

	for( int i = 0; i < divisionTeamList.Size(); i++ )
	{
		CClub & club = clubList.GetByID(divisionTeamList(i));

		m_teamTable->SetText( i, 0, lUIText( GetClubName(&club) ) );
		m_teamTable->SetTagDataForRow(i, divisionTeamList(i).id);
		m_teamTable->SetRowBkgColour(i, L_ARGB( 127, 80, 80, 80 ) );
	}

	m_teamTable->SortByColumn(0, true);
}

//////////////////////////////////////////////////////////////////////////
// Handler for when the selected team is changed.
//////////////////////////////////////////////////////////////////////////
void FDEditModeScreen::OnSelectTeam( FDTableWidget *, int row )
{
	m_playerTable->ClearAll();
	m_editClubNameButton->Disable();
	m_editPlayerNameButton->Disable();

	if (row < 0)
		return;

	m_editClubNameButton->Enable();

	CClubList &clubList = WorldData().GetClubList();
	CClub &cClub = clubList.GetByID(ClubID(m_teamTable->GetTagDataForRow(row)));

	m_playerTable->AddColumn(lUICoordPercent(1.0f), LUI_LEFT_CENTER);

	int iRowCount = 0;
	for (int i = 0; i < PLAYERSINSQUAD; i++)
	{
		if (cClub.GetPlayerID(i) == NOPLAYER)
			continue;

		CString cName = GetPlayerName(&cClub.GetPlayer(i)).CStr();

		m_playerTable->AddRows(1);
		m_playerTable->SetText(iRowCount, 0, lUIText(cName));
		m_playerTable->SetTagDataForRow(iRowCount, cClub.GetPlayerID(i));
		m_playerTable->SetRowBkgColour(iRowCount, L_ARGB( 127, 80, 80, 80 ) );
		iRowCount++;
	}

	m_playerTable->SortByColumn(0, true);
}

//////////////////////////////////////////////////////////////////////////
// Handler for when a player is selected in the player table.
//////////////////////////////////////////////////////////////////////////
void FDEditModeScreen::OnSelectPlayer(FDTableWidget *, int row)
{
	// Correct button enable state.
	if (row >= 0)
		m_editPlayerNameButton->Enable();
	else
		m_editPlayerNameButton->Disable();
}

//////////////////////////////////////////////////////////////////////////
// Handler for when the Edit Club button has been pressed.
//////////////////////////////////////////////////////////////////////////
void FDEditModeScreen::OnEditClubName(lUIControlWidget *pWidget)
{
	if (m_editClubNameButton->IsDisabled() == true)
		return;

	CClubList &clubList = WorldData().GetClubList();
	CClub &cClub = clubList.GetByID(ClubID(m_teamTable->GetTagDataForRow(m_teamTable->GetSelectedRow())));

	// Check to see if there are club name slots left (if we need a new one). If not, then throw up
	// a dialog box and return.
	if ((m_editData.IsClubIDModified(cClub.DoFindID()) == false) &&
		(m_editData.GetFreeClubNameIndex() < 0))
	{
    GetMainUI()->UserMessageBox(IDS_EDIT_MODE_TOO_MANY_CLUB_NAMES_DIALOG, MB_OK);
		return;
	}

	GetMainUI()->GetEditNameScreen()->SetClub(GetClubName(&cClub));
	GetMainUI()->GetEditNameScreen()->SetLengthLimit(FDEditModeData::MAX_EDIT_CLUB_NAME_LENGTH);
	GetMainUI()->GetEditNameScreen()->SetResetButtonEnabled(m_editData.IsClubIDModified(cClub.DoFindID()));

  GetMainUI()->SwitchToScreenCombo(SCREENCOMBO_EDITNAME, false);
	m_bInEditName = true;
}

//////////////////////////////////////////////////////////////////////////
// Handler for when the Edit Player button has been pressed.
//////////////////////////////////////////////////////////////////////////
void FDEditModeScreen::OnEditPlayerName(lUIControlWidget *pWidget)
{
	if (m_editPlayerNameButton->IsDisabled() == true)
		return;

	CClubList &clubList = WorldData().GetClubList();
	CClub &cClub = clubList.GetByID(ClubID(m_teamTable->GetTagDataForRow(m_teamTable->GetSelectedRow())));

	CPlayer *pPlayer = WorldData().GetPlayer(m_playerTable->GetTagDataForRow(m_playerTable->GetSelectedRow()));
	if (pPlayer == NULL)
		return;

	// Check to see if there are player name slots left (if we need a new one). If not, then throw up
	// a dialog box and return.
	if ((m_editData.IsPlayerIndexModified(pPlayer->DoFindID()) == false) &&
		(m_editData.GetFreePlayerNameIndex() < 0))
	{
    GetMainUI()->UserMessageBox(IDS_EDIT_MODE_TOO_MANY_PLAYER_NAMES_DIALOG, MB_OK);
		return;
	}

	GetMainUI()->GetEditNameScreen()->SetPlayer(GetPlayerSurname(pPlayer), GetPlayerFirstInitial(pPlayer));
	GetMainUI()->GetEditNameScreen()->SetLengthLimit(FDEditModeData::MAX_EDIT_PLAYER_NAME_LENGTH);
	GetMainUI()->GetEditNameScreen()->SetResetButtonEnabled(m_editData.IsPlayerIndexModified(pPlayer->DoFindID()));
  GetMainUI()->SwitchToScreenCombo(SCREENCOMBO_EDITNAME, false);
	m_bInEditName = true;
}

void FDEditModeScreen::RefreshScreen()
{
	// set up division table
	m_divisionSelector->ClearAll();
	CDivisionList &divisionList = WorldData().GetDivisionList();

	for(int i = 0; i < divisionList.GetSize(); i++)
	{
		CDivision &division = divisionList.GetAt(i);
		m_divisionSelector->AddItem( lUIText( division.GetName() ) );
	}

	// select premiership
	m_divisionSelector->SetSelectedItem( 0 );
}

//////////////////////////////////////////////////////////////////////////
// Used after returning from the name edit screen to update the UI.
//////////////////////////////////////////////////////////////////////////
void FDEditModeScreen::RefreshSelectedNames()
{
	CClubList &clubList = WorldData().GetClubList();
	CClub &cClub = clubList.GetByID(ClubID(m_teamTable->GetTagDataForRow(m_teamTable->GetSelectedRow())));

	m_teamTable->SetText(m_teamTable->GetSelectedRow(), 0, lUIText(GetClubName(&cClub)));

	if (m_playerTable->GetSelectedRow() >= 0)
	{
		CPlayer *pPlayer = WorldData().GetPlayer(m_playerTable->GetTagDataForRow(m_playerTable->GetSelectedRow()));
		m_playerTable->SetText(m_playerTable->GetSelectedRow(), 0, lUIText(GetPlayerName(pPlayer)));
	}
}

//////////////////////////////////////////////////////////////////////////
// Deals with the result from the edit name dialog.
//////////////////////////////////////////////////////////////////////////
void FDEditModeScreen::HandleDialogResult()
{
	FDEditNameScreen *pEdit = GetMainUI()->GetEditNameScreen();
	const FDEditNameScreen::ResultType eResult = pEdit->GetDialogResult();

	if (eResult == FDEditNameScreen::FDRT_CANCELLED)
		return;

	// The user has made some kind of change -- enable the cancel button if it isn't already.
  //m_cancelChangesButton->Enable();

	switch (pEdit->GetEditMode())
	{
	case FDEditNameScreen::FDEM_CLUB_NAME:
		{
			if (eResult == FDEditNameScreen::FDRT_SAVED)
			{
				const ClubID cClubID = ClubID(m_teamTable->GetTagDataForRow(m_teamTable->GetSelectedRow()));
				const int iExistingModificationIndex = m_editData.GetClubEntryIndex(cClubID);

				CClub &cClub = WorldData().GetClubByID(cClubID);

				// Check to see if the edit resulted in the original name. If so, then delete the modification.
				if (pEdit->GetName() == cClub.GetName())
				{
					if (iExistingModificationIndex >= 0)
						m_editData.RemoveClubModification(cClubID);
				}
				else
				{
					if (iExistingModificationIndex >= 0)
						m_editData.ModifyClubModification(iExistingModificationIndex, pEdit->GetName());
					else
						m_editData.AddClubModification(cClubID, pEdit->GetName());
				}
			}
			else if (eResult == FDEditNameScreen::FDRT_RESET)
			{
				m_editData.RemoveClubModification(ClubID(m_teamTable->GetTagDataForRow(m_teamTable->GetSelectedRow())));
			}
			else
			{
				lAssert(false && "invalid result type");
			}
		}
		break;
	case FDEditNameScreen::FDEM_PLAYER_NAME:
		{
			if (eResult == FDEditNameScreen::FDRT_SAVED)
			{
				lString sInitial = pEdit->GetInitial();
				CClubList &clubList = WorldData().GetClubList();
				CClub &cClub = clubList.GetByID(ClubID(m_teamTable->GetTagDataForRow(m_teamTable->GetSelectedRow())));
				CPlayer *pPlayer = WorldData().GetPlayer(m_playerTable->GetTagDataForRow(m_playerTable->GetSelectedRow()));
				const ushort usPlayerID = pPlayer->DoFindID();

				// Check to see if the edit resulted in the original name. If so, then delete the modification.
				CString sOriginalName = pPlayer->GetName();
				const int iExistingModificationIndex = m_editData.GetPlayerEntryIndex(usPlayerID);

				if ((sInitial + "." + pEdit->GetName()) == sOriginalName)
				{
					if (iExistingModificationIndex >= 0)
						m_editData.RemovePlayerModification(usPlayerID);
				}
				else
				{
					// Not the same name -- add/modify the player's edit data.
					if (iExistingModificationIndex >= 0)
						m_editData.ModifyPlayerModification(iExistingModificationIndex, pEdit->GetName(), sInitial[0]);
					else
						m_editData.AddPlayerModification(usPlayerID, pEdit->GetName(), sInitial[0]);
				}
			}
			else if (eResult == FDEditNameScreen::FDRT_RESET)
			{
				m_editData.RemovePlayerModification(m_playerTable->GetTagDataForRow(m_playerTable->GetSelectedRow()));
			}
			else
			{
				lAssert(false && "invalid result type");
			}
		}
		break;
	default:
		lAssert(false && "invalid edit mode");
		break;
	}
}

lString FDEditModeScreen::GetClubName(CClub *pClub)
{
	const ClubID cClubID = pClub->DoFindID();
	if (m_editData.IsClubIDModified(cClubID) == true)
	{
		const ushort usDataIndex = m_editData.GetClubEntryIndex(cClubID);
		return lString(m_editData.m_cClubNames.GetString(usDataIndex));
	}
	else
	{
		return lString(pClub->GetName());
	}
}

lString FDEditModeScreen::GetPlayerName(CPlayer *pPlayer)
{
	const ushort usPlayerID = pPlayer->DoFindID();
	if (m_editData.IsPlayerIndexModified(usPlayerID) == true)
	{
		const ushort usPlayerIndex = m_editData.GetPlayerEntryIndex(usPlayerID);
		return lString(m_editData.m_cPlayerForenames.Get(usPlayerIndex)) + lString('.') + lString(m_editData.m_cPlayerSurnames.GetString(usPlayerIndex));
	}
	else
	{
		return lString(pPlayer->GetName());
	}
}

lString FDEditModeScreen::GetPlayerSurname(CPlayer *pPlayer)
{
	const ushort usPlayerID = pPlayer->DoFindID();
	if (m_editData.IsPlayerIndexModified(usPlayerID) == true)
	{
		const ushort usPlayerIndex = m_editData.GetPlayerEntryIndex(usPlayerID);
		return lString(m_editData.m_cPlayerSurnames.GetString(usPlayerIndex));
	}
	else
	{
		return lString(pPlayer->GetSurname());
	}
}

char FDEditModeScreen::GetPlayerFirstInitial(CPlayer *pPlayer)
{
	const ushort usPlayerID = pPlayer->DoFindID();
	if (m_editData.IsPlayerIndexModified(usPlayerID) == true)
	{
		const ushort usPlayerIndex = m_editData.GetPlayerEntryIndex(usPlayerID);
		return m_editData.m_cPlayerForenames.Get(usPlayerIndex);
	}
	else
	{
		CString sForename = pPlayer->GetForename();
		if (sForename.GetLength() == 0)
			return 'A';
		const char cInitial = ::toupper(sForename[0]);
		if ((cInitial < 'A') || (cInitial > 'Z'))
			return 'A';
		return sForename[0];
	}
}

