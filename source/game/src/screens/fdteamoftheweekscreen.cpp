//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDTeamOfTheWeekscreen.cpp
// Description : FDTeamOfTheWeekScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>
#include <fdtext.h>

#include <screens/fdteamoftheweekscreen.h>
#include <screens/fdplayerinfosubscreen.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTeamOfTheWeekScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDTeamOfTheWeekScreen::FDTeamOfTheWeekScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDTeamOfTheWeekScreen::~FDTeamOfTheWeekScreen()
{
}

void FDTeamOfTheWeekScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  // *** TeamOfTheWeek ***

  m_towScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "tow_screen" );
  m_towScreen->Hide();

  // show division combo box
  m_selectteamDivision = FD_GAME_NEW FDComboWidget( m_towScreen,  0, "tow_selectteam_divcombo", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 138, 174 ));//, lUIPoint( 150, 20 ) );
  m_selectteamDivision->SetAllToUpper( true );

  // hook up signal before we set selected item
  m_selectteamDivision->SignalSelection.Connect( this, &FDTeamOfTheWeekScreen::OnSelectTeamSelection );

  // set up division table
  CDivisionList &divisionList = WorldData().GetDivisionList();

  for(int i = 0; i < divisionList.GetSize(); i++)
  {
    CDivision &division = divisionList.GetAt(i);
    m_selectteamDivision->AddItem( lUIText( division.GetName() ) );
  }

  // set up team of week table
  m_table = FD_GAME_NEW FDTableWidget( m_towScreen, 0, "playertable",
            /*FDTableWidget::TABLE_FLAG_ROW_LINES|FDTableWidget::TABLE_FLAG_COLUMN_LINES|*/
            FDTableWidget::TABLE_FLAG_BORDER |FDTableWidget::TABLE_FLAG_ROW_SELECT|
            FDTableWidget::TABLE_FLAG_CELLBACKGROUND|FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW |
			FDTableWidget::TABLE_FLAG_CAP_FIRSTROW | FDTableWidget::TABLE_FLAG_CUT_OFF,
            LUI_TOP_LEFT, lUIPoint( 20, 10 ), lUIPoint( 256 - 20, 155 )  );

  m_table->SignalRowSelect.Connect( this, &FDTeamOfTheWeekScreen::OnRowSelect );

  FDButtonWidget *okButton = FD_GAME_NEW FDButtonWidget( m_towScreen, 0, "clock", FDResourceManager::GetSprite("uiicon","fast_forward"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,  LUI_TOP_LEFT, lUIPoint(0,2) );
  okButton->SignalControlCommand.Connect( this, &FDTeamOfTheWeekScreen::OnDone );
}


void FDTeamOfTheWeekScreen::DoShow()
{
  m_towScreen->Show();
  GetMainUI()->GetPlayerInfoSubScreen()->Hide();
  GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo(NULL);

  GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE

  if (IsForcedUpdateRequested() == true)
  {
    // select users league by default
    if( m_selectteamDivision != NULL )
    {
      m_selectteamDivision->SetSelectedItem( WorldData().GetCurrentUserClub()->GetDivisionID() );
    }

    SetForcedUpdateRequested(false);
  }
}

void FDTeamOfTheWeekScreen::DoHide()
{
  m_towScreen->Hide();
  GetMainUI()->GetPlayerInfoSubScreen()->Hide();
}

void FDTeamOfTheWeekScreen::OnSelectTeamSelection( FDComboWidget *, int index )
{
  m_table->SelectRow(-1);
  Update();
}

void FDTeamOfTheWeekScreen::OnDone( lUIControlWidget * )
{
  GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_OK );
}

void FDTeamOfTheWeekScreen::OnRowSelect( FDTableWidget *, int row )
{
  row = row-1;
  if(row < 0 || row > PLAYERSINTEAM)
  {
    GetMainUI()->GetPlayerInfoSubScreen()->Hide();
    GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo(NULL);
    GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE
    return;
  }

  int divisionIdx = m_selectteamDivision->GetSelectedItem();
  if( divisionIdx == -1 )
  {
    GetMainUI()->GetPlayerInfoSubScreen()->Hide();
    GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo(NULL);
    GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE
    return;
  }

  CDivisionList &divisionList = WorldData().GetDivisionList();
  CDivision &division = divisionList.GetAt( divisionIdx );

  if( division.GetTeamOfTheWeek().IsAvailable() == true )
  {
    CPlayerOfTheWeek* pPlayerOfTheWeek = division.GetTeamOfTheWeek().GetAt(row);

    // Set player info screen
	GetMainUI()->GetPlayerInfoSubScreen()->Show();
    GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( &pPlayerOfTheWeek->GetPlayer() );
    GetMainUI()->HideSubMessage();
  }
}

void FDTeamOfTheWeekScreen::Update()
{
  // Set up table for selected division
  m_table->ClearAll();

  int divisionIdx = m_selectteamDivision->GetSelectedItem();
  if( divisionIdx == -1 )
  {
    return;
  }

  //
  CDivisionList &divisionList = WorldData().GetDivisionList();
  CDivision &division = divisionList.GetAt( divisionIdx );

  if( division.GetTeamOfTheWeek().IsAvailable() == true )
  {
    m_table->AddColumn( lUICoord( 25.0f ) );  // squad number
    m_table->AddColumn( lUICoord( 5.0f ) );  // squad number
    m_table->AddColumn( lUICoord( 103.0f ), LUI_LEFT_CENTER );   // player name
    m_table->AddColumn( lUICoord( 103.0f ), LUI_LEFT_CENTER );   // club name

    lLogFmt("Team available\n");
    m_table->AddRows( PLAYERSINTEAM+1 );

    m_table->SetRowBkgColour( 0, L_ARGB(255, 35, 35, 35) );
    m_table->SetText( 0, 0, lUIText( "POS" ) );   // name
    m_table->SetText( 0, 2, lUIText( "PLAYER" ) );   // name
    m_table->SetText( 0, 3, lUIText( "CLUB"  ) );   // name

    for (int LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++)
    {
      CPlayerOfTheWeek* pPlayerOfTheWeek = division.GetTeamOfTheWeek().GetAt(LoopCount);
      CString str;
      str.Format("%d", LoopCount+1);
      m_table->SetText( LoopCount+1, 0, lUIText( str ) );

      if( pPlayerOfTheWeek->GetClubID() == WorldData().GetCurrentUserClub()->GetID() )
      {
        // player belongs to player
        m_table->SetRowBkgColour( LoopCount+1, L_ARGB(0xFF, 72, 134, 203) );
      } else
      {
        m_table->SetRowBkgColour( LoopCount+1, L_ARGB(80, 0x50, 0x50, 0x50) );
      }

      m_table->SetBkgColour( LoopCount+1, 0, PosnColour[pPlayerOfTheWeek->GetPlayer().GetSkills().GetPosition()] );

      m_table->SetText( LoopCount+1, 2, lUIText( FDMakePlayerNameLink(&pPlayerOfTheWeek->GetPlayer()) ) );
      m_table->SetText( LoopCount+1, 3, lUIText( FDMakeClubNameLink(&WorldData().GetClubByID(pPlayerOfTheWeek->GetClubID())) ) );
    }
  } else
  {
	  m_table->AddColumn(lUICoordPercent(1.0f));
	  m_table->AddRows(2);

	  // TODO LOCALE
    m_table->SetText(1, 0, lUIText("No Team of the Week for this division."));
    lLogFmt("Team not available\n");
  }
}

void FDTeamOfTheWeekScreen::AutoplaySkip()
{
  OnDone( NULL );
}
