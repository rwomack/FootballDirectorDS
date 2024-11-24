//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDSelectTeamscreen.cpp
// Description : FDSelectTeamScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdselectteamscreen.h>
#include <screens/fdsquadscreen.h>
#include <screens/fdclubinfosubscreen.h>
#include <screens/fdloadscreen.h>
#include <screens/fdnewsscreen.h>
#include <screens/fdnewgamescreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdtickerwidget.h>
#include <fdtablewidget.h>
#include <fdplayerblobwidget.h>
#include <fdscrollbarwidget.h>
#include <fdfullscreenwidget.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget
#include <lemon/math/lmrand.h>

using namespace L_NAMESPACE_NAME;


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDSelectTeamScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDSelectTeamScreen::FDSelectTeamScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDSelectTeamScreen::~FDSelectTeamScreen()
{
}

void FDSelectTeamScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them
  m_idNonSelectable.id = NOCLUB;
  m_bOKToExit = false;

  // team select

  m_selectteamScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "selectteam_screen" );
  m_selectteamScreen->Hide();

  // show table to select team from
  m_selectteamTable = FD_GAME_NEW FDTableWidget( m_selectteamScreen,  0, "selectteam_teamtable",
            FDTableWidget::TABLE_FLAG_BORDER | FDTableWidget::TABLE_FLAG_BACKGROUND |
            FDTableWidget::TABLE_FLAG_ROW_SELECT  | /*FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW  |
			FDTableWidget::TABLE_FLAG_CAP_FIRSTROW |*/ FDTableWidget::TABLE_FLAG_CELLBACKGROUND | FDTableWidget::TABLE_FLAG_CUT_OFF,
            LUI_TOP_LEFT, lUIPoint( 30, 40 ), lUIPoint( 110, 145 ) );

  FDScrollBarWidget *pScrollBar = FD_GAME_NEW FDScrollBarWidget(m_selectteamScreen, 0, "teamtablescroll", LUI_TOP_LEFT,
    lUIPoint(107, 15), lUIPoint(GetDefaultScrollBarWidth(), 167));

  pScrollBar->AlignTo( m_selectteamTable );
  m_selectteamTable->SetScrollBar(pScrollBar);

  m_selectteamTable->AddColumn( lUICoordPercent( 1.0f ), LUI_TOP_CENTER );
  m_selectteamTable->SignalRowSelect.Connect( this, &FDSelectTeamScreen::OnSelectTeamSelectTeam );

  // show division combo box
  m_selectteamDivision = FD_GAME_NEW FDComboWidget( m_selectteamScreen,  0, "selectteam_divcombo", lUIText(""), LUI_TOP_LEFT, lUIPoint( 25, 15 ), lUIPoint( 216, 20 ) );
  m_selectteamDivision->SetAllToUpper( true );

  // hook up signal before we set selected item
  m_selectteamDivision->SignalSelection.Connect( this, &FDSelectTeamScreen::OnSelectTeamSelection );

  //
  m_selectteamDone = FD_GAME_NEW FDButtonWidget( m_selectteamScreen, 0, "selectteam_donebutton", FDResourceManager::GetSprite("ui","button75"),
                                                  lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_RIGHT, lUIPoint( 238, 160 ) );
  m_selectteamDone->SetText( lUIText( IDS_DONE_CAPS) );
  m_selectteamDone->Disable();

  m_selectteamDone->SignalControlCommand.Connect( this, &FDSelectTeamScreen::OnSelectTeamDone );

  m_returnToMainMenuButton = FD_GAME_NEW FDButtonWidget( m_selectteamScreen, 0, "selectteam_returntomenubutton", FDResourceManager::GetSprite("ui", "button75"),
    lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_RIGHT, lUIPoint(238, 135));

  lUIText backText(IDS_QUIT_TO_MAIN_MENU_BUTTON);
  backText.ToUpper();
  m_returnToMainMenuButton->SetText(backText);
  m_returnToMainMenuButton->Hide();

  m_returnToMainMenuButton->SignalControlCommand.Connect(this, &FDSelectTeamScreen::OnReturnToMainMenuButton);

  //m_textWidget = FD_GAME_NEW lUITextWidget( m_selectteamScreen, 0, "text", lUIText(""), LUI_TOP_LEFT, lUIPoint( 150, 60 ) );

  //

  GetMainUI()->DisplaySubMessage("Select a club to manage"); //TODO LOCALE


  SetModalOK(m_selectteamDone);
  SetModalVerifyOKToExitFunction(FDSelectTeamScreen::IsOKToReturnModal);
}

void FDSelectTeamScreen::DoShow()
{
  m_bOKToExit = false;

  //m_textWidget->SetText( m_text );

 // set up division table
  m_selectteamDivision->ClearAll();

  CDivisionList &divisionList = WorldData().GetDivisionList();

  for(int i = 0; i < divisionList.GetSize(); i++)
  {
    CDivision &division = divisionList.GetAt(i);
    m_selectteamDivision->AddItem( lUIText( division.GetName() ) );
  }

  // select premiership
  m_selectteamDivision->SetSelectedItem( 0 );

  m_selectteamScreen->Show();

  m_returnToMainMenuButton->Show();
}

void FDSelectTeamScreen::DoHide()
{
  m_selectteamScreen->Hide();
}

void FDSelectTeamScreen::OnSelectTeamSelection( FDComboWidget *, int index )
{
  //
  lLogFmt("Division %d selected\n", index);

  // deselect
  m_selectteamTable->SelectRow( -1 );
  UpdateClubInfo();

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

      lLogFmt("  %d: ID(%d) %s\n", i, club.DoFindID(), club.GetName());
    }
  }

  // set table to represent division
  m_selectteamTable->SetRows( divisionTeamList.Size() );
  m_selectteamTable->SetSortFunctionForColumn(0, FDTableWidget::GenericStringSort);
  m_selectteamTable->SetRowBkgColour(0, L_ARGB(255, 35, 35, 35));
  for( int i = 0; i < divisionTeamList.Size(); i++ )
  {
    CClub & club = clubList.GetByID(divisionTeamList(i));

    m_selectteamTable->SetText( i, 0, lUIText( club.GetName() ) );
	  m_selectteamTable->SetTagDataForRow(i, divisionTeamList(i).id);
	  m_selectteamTable->SetRowBkgColour(i, L_ARGB( 127, 130, 130, 130) );
  }

  m_selectteamTable->SortByColumn(0, true);
}

void FDSelectTeamScreen::OnSelectTeamSelectTeam( FDTableWidget *, int row )
{
  lLogFmt("Team %d selected\n", row);
  if( row == -1 )
  {
    m_selectteamDone->Disable();
  } else
  {
    const ClubID teamId = ClubID(m_selectteamTable->GetTagDataForRow(row));
    lLogFmt("   Team %d (%s)\n", (int)teamId.id, WorldData().GetClubByID( teamId ).GetName() );

    if ((m_idNonSelectable.id != NOCLUB) && (m_idNonSelectable == teamId))
    {
      UserMessageBox(IDS_CANT_RETURN_TO_CLUB_THAT_SACKED_YOU_DIALOG, MB_OK);
      m_selectteamDone->Disable();
      return;
    }

    m_selectteamDone->Enable();

    UpdateClubInfo();
  }
}

void FDSelectTeamScreen::OnSelectTeamDone( lUIControlWidget *pWidget )
{
  FDButtonWidget *pButton = (FDButtonWidget *)pWidget;
  if (pButton->IsDisabled() == true)
    return;

  lLogFmt("Select team done!\n");

  int rowIdx = m_selectteamTable->GetSelectedRow();

  // get team index
  int divisionIdx = m_selectteamDivision->GetSelectedItem();

  if( divisionIdx == -1 || rowIdx == -1 )
  {
    lLogFmt("Invalid index - not proceeding\n");
    return;
  }

  m_bOKToExit = true;

  const ClubID teamId = ClubID(m_selectteamTable->GetTagDataForRow(m_selectteamTable->GetSelectedRow()));

  // go to loading screen
  GetMainUI()->ShowProcessScreen( true, PROCESS_LOAD );

  // Save manager name to game
  CString strForeName = _T("@");
  CString newGameName( GetMainUI()->GetNewGameScreen()->GetNameStr() );
  WorldData().GetCurrentUser()->SetFullName( strForeName, newGameName );

  // select team
  WorldData().OnInitialiseUser( teamId );

  // welcome human manager
  WorldData().GetCurrentUserClub()->DoHumanManagerWelcome();

  // skip to end of week
  WorldData().GetCountryList().DoEndOfWeek();

  // mark selected team
  GetMainUI()->GetSquadScreen()->SetSquadInfo( WorldData().GetCurrentUserClub() );

  if (m_idNonSelectable.id == NOCLUB)
  {
    GetMainUI()->GetNewsScreen()->ResetTeleprinterDate();
  }

  // Save tutorial data.
  GetMainUI()->OptionsSave();

  // hide process screen
  GetMainUI()->ShowProcessScreen( false, PROCESS_LOAD );

  // show news / status. Ensure that the player can't back up into team selection again.
  GetMainUI()->ClearScreenStack();
  GetMainUI()->SetWorldDataUsed(true);
  GetMainUI()->SwitchToScreenCombo( SCREENCOMBO_NEWSSTATUS, false );
}

void FDSelectTeamScreen::SetNonSelectableTeam(ClubID cClub)
{
	m_idNonSelectable = cClub;
}

void FDSelectTeamScreen::OnReturnToMainMenuButton(lUIControlWidget *)
{
  if (GetMainUI()->UserMessageBox(IDS_QUIT_TO_MAIN_MENU_CONFIRMATION_DIALOG, MB_YESNO) == IDYES)
	{
    GetMainUI()->SwitchToScreenCombo(SCREENCOMBO_MAINMENU, false);
	}
}

bool FDSelectTeamScreen::IsOKToReturnModal(FDScreen *pScreen)
{
	FDSelectTeamScreen *pThis = (FDSelectTeamScreen *)pScreen;
	return pThis->m_bOKToExit;
}

void FDSelectTeamScreen::UpdateClubInfo()
{
  if( m_selectteamTable->GetSelectedRow() != -1 )
  {
    // get club
    ClubID clubId = ClubID(m_selectteamTable->GetTagDataForRow(m_selectteamTable->GetSelectedRow()));

    if( clubId.id != NOCLUB )
    {
      CClub *pClub = &WorldData().GetClubByID(clubId);
      GetMainUI()->HideSubMessage();
      GetMainUI()->GetClubInfoSubScreen()->SetBacking( true );
      GetMainUI()->GetClubInfoSubScreen()->SetClubInfo( pClub );
      GetMainUI()->GetClubInfoSubScreen()->Show();
      return;
    }
  }

  GetMainUI()->GetClubInfoSubScreen()->Hide();
  GetMainUI()->DisplaySubMessage("Select a club to manage"); //TODO LOCALE
}

