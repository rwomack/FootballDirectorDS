//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdwagesscreen.cpp
// Description : FDWagesScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdscrollbarwidget.h>
#include <fdgameworld.h>
#include <fdtext.h>

#include <screens/fdwagesscreen.h>
#include <screens/fdplayerinfosubscreen.h>
#include <screens/fdemployeeinfoscreen.h>
#include <screens/fdmanagerinfoscreen.h>
#include <screens/fdyouthinfoscreen.h>
#include <screens/fdnegotiatescreen.h>
#include <screens/fdtransferstatusscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDWagesScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

static bool s_lastActionSack = false;
extern bool g_hackLastMsgResponse;

FDWagesScreen::FDWagesScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDWagesScreen::~FDWagesScreen()
{
}

void FDWagesScreen::Init()
{
  m_bComingFromDialogBox = false;
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_wagesScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "squad_screen" );

  // * Table for player names

  m_wagesTable = FD_GAME_NEW FDTableWidget( m_wagesScreen, 0, "wagesplayertable",
            //FDTableWidget::TABLE_FLAG_ROW_LINES|FDTableWidget::TABLE_FLAG_COLUMN_LINES|
            FDTableWidget::TABLE_FLAG_BORDER|FDTableWidget::TABLE_FLAG_ROW_SELECT|
            FDTableWidget::TABLE_FLAG_CELLBACKGROUND|FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW |
			FDTableWidget::TABLE_FLAG_CAP_FIRSTROW | FDTableWidget::TABLE_FLAG_CUT_OFF,
            LUI_TOP_LEFT, lUIPoint( 20, 10 ), lUIPoint( 256 - 20 - GetDefaultScrollBarWidth(), 144 ) );

  FDScrollBarWidget *pScrollBar = FD_GAME_NEW FDScrollBarWidget(m_wagesScreen, 0, "tablescroll", LUI_TOP_LEFT, lUIPoint(256-20, 10), lUIPoint(GetDefaultScrollBarWidth(), 146));
  pScrollBar->AlignTo( m_wagesTable );
  m_wagesTable->SetScrollBar(pScrollBar);
  m_wagesTable->SignalRowSelect.Connect( this, &FDWagesScreen::OnRowSelect );

  // select information shown by table
  m_wagesTableInfoType = FD_GAME_NEW FDComboWidget( m_wagesScreen, 0, "tableinfo_combo", lUIText(""), LUI_LEFT_CENTER, lUIPoint( 140, 175 ) );
  m_wagesTableInfoType->SetAllToUpper( true );
  m_wagesTableInfoType->AddItem( lUIText( IDS_WAGES ) );
  m_wagesTableInfoType->AddItem( lUIText( IDS_CONTRACT ) );

  m_wagesTableInfoType->SignalSelection.Connect( this, &FDWagesScreen::OnTableInfoCombo );

  m_wagesSackButton = FD_GAME_NEW FDButtonWidget( m_wagesScreen, 0, "wagesfire_button", FDResourceManager::GetSprite("uiicon","fire"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 35, 175 ) );
  m_wagesContractButton = FD_GAME_NEW FDButtonWidget( m_wagesScreen, 0, "wagespromote_button", FDResourceManager::GetSprite("uiicon","offercontract"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 65, 175 ) );
  m_wagesTransferStatusButton = FD_GAME_NEW FDButtonWidget( m_wagesScreen, 0, "wagestransfer_button", FDResourceManager::GetSprite("uiicon","transfer_status"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 95, 175 ) );

  m_wagesSackButton->Hide();
  m_wagesContractButton->Hide();
  m_wagesTransferStatusButton->Hide();

  m_wagesSackButton->SignalControlCommand.Connect( this, &FDWagesScreen::OnSackButton );
  m_wagesContractButton->SignalControlCommand.Connect( this, &FDWagesScreen::OnContractButton );
  m_wagesTransferStatusButton->SignalControlCommand.Connect( this, &FDWagesScreen::OnTransferStatusButton );

  m_WhichEvents = WAGE_DISPLAY_WAGE;
}

void FDWagesScreen::SerializeState( FDStateStorage &state )
{
  // Prevent automatic update after a dialog box
  if (m_bComingFromDialogBox == true )
  {
    lLogFmt("  from dialog box\n");
    return;
  }

  if( state.IsSaving() )
  {
    int wagestype = m_wagesTableInfoType->GetSelectedItem();
    int tableidx  = m_wagesTable->GetSelectedRow();

    state << wagestype;
    state << tableidx;

    lLogFmt("Saving type %d tableidx %d\n", wagestype, tableidx);
  } else
  {
    int wagestype;
    int tableidx;

    state << wagestype;
    state << tableidx;

    lLogFmt("Loading type %d tableidx %d\n", wagestype, tableidx);
    m_wagesTableInfoType->SetSelectedItem( wagestype, false );
    m_wagesTable->SelectRow( tableidx );
  }
}


void FDWagesScreen::Update()
{
  lLogFmt("FDWagesScreen::Update\n");

  // Prevent automatic update after a dialog box
  if (m_bComingFromDialogBox == true )
  {
    lLogFmt("  from dialog box\n");
    return;
  }

	CClub& theClub = *WorldData().GetCurrentUserClub();
	CCountry& rCountry = theClub.GetCountry();
	CString strString;
	CString strPrint;
	CString szWage;
	CString szContract;

  m_personList.Clear();

  // Clear List
  m_wagesTable->ClearAll();
  m_wagesTable->AddColumn( lUICoord( 5 ) );// gap
  m_wagesTable->AddColumn( lUICoord( 90 ), LUI_LEFT_TOP );
  m_wagesTable->AddColumn( lUICoord( 5 ) );// gap
  m_wagesTable->AddColumn( lUICoord( 45 ), LUI_LEFT_TOP );
  m_wagesTable->AddColumn( lUICoord( 5 ) ); // gap
  m_wagesTable->AddColumn( lUICoord( 5 ) ); // gap
  m_wagesTable->AddColumn( lUICoord( 88 ), LUI_LEFT_TOP );

  m_wagesTable->SetSortWhenColumnHeaderClicked(true);
  m_wagesTable->SetSortFunctionForColumn(1, FDTableWidget::GenericFirstInitialLastNameSort);
  m_wagesTable->SetSortFunctionForColumn(3, FDTableWidget::GenericStringSort);
  m_wagesTable->ClearTiebreakerColumns();
  m_wagesTable->AddTiebreakerColumn(1);

  m_wagesTable->AddRows(1);
  m_wagesTable->SetText( 0, 1, lUIText( IDS_NAME ) );
  m_wagesTable->SetText( 0, 3, lUIText( IDS_JOB ) );

  m_wagesTable->SetRowBkgColour(0, L_ARGB(255, 35, 35, 35));

  // Set up the text and sorting functions for the columns.
  UpdateColumnText();

	if (theClub.GetManagerID() != NOMANAGER)
	{
		m_wagesTable->AddRows(1);

		// Add Manager
		m_personList.Add(1);
		m_personList.Last().person = (CPerson*) &theClub.GetManager();
		m_personList.Last().type = WAGE_MANAGER;

		const int iManagerRow = m_wagesTable->GetRowCount() - 1;
		m_wagesTable->SetTagDataForRow(iManagerRow,  m_personList.Size() - 1);
		UpdateTableEntryForManager(m_personList.Size() - 1);
	}

	m_dTotalEmpWages = 0;
	if (theClub.TotalStaff () > 0)
	{
		// Add Employees  -  See Job Centre
	    for (int LoopCount = 0 ; LoopCount < theClub.TotalStaff(); LoopCount++)
		{
			CEmployee* pEmployee = theClub.GetEmployee(LoopCount);
			m_dTotalEmpWages +=  pEmployee->GetWage() * 52;		// Calculate Total Employee Wages

			m_wagesTable->AddRows(1);
			m_personList.Add(1);
			m_personList.Last().person = (CPerson*) pEmployee;
			m_personList.Last().type = WAGE_EMPLOYEE;

			const int iEmployeeRow = m_wagesTable->GetRowCount() - 1;
			m_wagesTable->SetTagDataForRow(iEmployeeRow,  m_personList.Size() - 1);
			UpdateTableEntryForEmployee(m_personList.Size() - 1);
		}
	}

	// Add Players
	CPlayerIDList ThePlayers;
	ThePlayers.DoBuildFromSquad(theClub.GetFirstTeamSquad(), WorldData().GetPlayerList());
	m_dTotalPlayerWages = 0;
	for (int LoopCount = 0; LoopCount < ThePlayers.GetSize(); LoopCount++)
	{
		CPlayer* pPlayer = WorldData().GetPlayer(ThePlayers[LoopCount]);
		// If is a player on loan then dont display
		if (pPlayer->GetWeeksOnLoan() != 0)
		{
			continue;
		}
		AddPlayerToTable(pPlayer);
	}

	// Add Youth Header
	m_dTotalYouthWages = 0;
	if (theClub.TotalYouths() > 0)
	{
    //strString.Format(IDS_YOUTHS, theClub.TotalYouths());

		// Add Youths
	    for (int LoopCount = 0; LoopCount < theClub.TotalYouths(); LoopCount++)
		{
			CYouth* pYouth = theClub.GetYouth(LoopCount);
			m_wagesTable->AddRows(1);
			m_personList.Add(1);
			m_personList.Last().person = (CPerson*) pYouth;
			m_personList.Last().type = WAGE_YOUTH;

			const int iYouthRow = m_wagesTable->GetRowCount() - 1;
			m_wagesTable->SetTagDataForRow(iYouthRow,  m_personList.Size() - 1);

			UpdateTableEntryForYouth(m_personList.Size() - 1);
		}
	}
	m_wagesTable->ResetScroll();

  //


}

void FDWagesScreen::Prepare()
{
	if ((m_bComingFromDialogBox == false) || (IsForcedUpdateRequested() == true))
	{
		SetForcedUpdateRequested(false);
		Update();
	}
}

void FDWagesScreen::DoShow()
{
	m_wagesScreen->Show();

  if( s_lastActionSack && m_bComingFromDialogBox && g_hackLastMsgResponse )
  {
    lLogFmt("Sack occured\n");
    return;
  }

  if( m_wagesTable->GetSelectedRow() < m_wagesTable->GetRowCount())
  {
    m_wagesTable->SelectRow( m_wagesTable->GetSelectedRow() );
  } else
  {
    m_wagesTable->SelectRow( -1 );
  }
}

void FDWagesScreen::DoHide()
{
  m_wagesScreen->Hide();
}

void FDWagesScreen::OnTableInfoCombo( FDComboWidget *, int idx )
{
  lLogFmt("Info to show %d\n", idx);
  m_WhichEvents = (WageDisplayType)idx;

  for (int i = 0; i < m_personList.Size(); i++)
  {
	  if (m_personList(i).person != NULL)
	  {
		  switch (m_personList(i).type)
		  {
		  case WAGE_MANAGER:
			  UpdateTableEntryForManager(i);
			  break;
		  case WAGE_EMPLOYEE:
			  UpdateTableEntryForEmployee(i);
			  break;
		  case WAGE_PLAYER:
			  UpdateTableEntryForPlayer(i);
			  break;
		  case WAGE_YOUTH:
			  UpdateTableEntryForYouth(i);
			  break;
		  default:
			  lAssert(false && "unknown employee type");
			  break;
		  }
	  }
  }

  UpdateColumnText();
}

void FDWagesScreen::OnRowSelect( FDTableWidget *, int row )
{
  lLogFmt("%d row selected\n", row);

  GetMainUI()->GetPlayerInfoSubScreen()->Hide();
  GetMainUI()->GetEmployeeInfoScreen()->Hide();
  GetMainUI()->GetYouthInfoScreen()->Hide();
  GetMainUI()->GetManagerInfoScreen()->Hide();

  m_wagesSackButton->Hide();
  m_wagesContractButton->Hide();
  m_wagesTransferStatusButton->Hide();

  GetMainUI()->DisplaySubMessage("Select Contract"); //TODO LOCALE

  if( row <= 0 )
    return;

  GetMainUI()->HideSubMessage();

  // Remove the header row
  const int iPersonListIndex = (int)m_wagesTable->GetTagDataForRow(row);
  switch( m_personList(iPersonListIndex).type )
  {
    case WAGE_MANAGER:
      lLogFmt("Manager selected\n");
      GetMainUI()->GetManagerInfoScreen()->SetManagerInfo( (CManager*)  m_personList(iPersonListIndex).person );
      GetMainUI()->GetManagerInfoScreen()->Show();
      break;
    case WAGE_EMPLOYEE:
      lLogFmt("Employee selected\n");
      GetMainUI()->GetEmployeeInfoScreen()->SetEmployeeInfo( (CEmployee*)  m_personList(iPersonListIndex).person );
      GetMainUI()->GetEmployeeInfoScreen()->Show();
      m_wagesSackButton->Show();
      m_wagesContractButton->Show();
      break;
    case WAGE_YOUTH:
      lLogFmt("Youth selected\n");
      GetMainUI()->GetYouthInfoScreen()->SetYouthInfo( (CYouth*)  m_personList(iPersonListIndex).person );
      GetMainUI()->GetYouthInfoScreen()->Show();
      m_wagesSackButton->Show();
      m_wagesContractButton->Show();
      break;
    case WAGE_PLAYER:
      lLogFmt("Player selected\n");
      GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( (CPlayer*)  m_personList(iPersonListIndex).person, FDPlayerInfoSubScreen::PIT_WAGES );
      GetMainUI()->GetPlayerInfoSubScreen()->Show();
      m_wagesSackButton->Show();
      m_wagesContractButton->Show();
      m_wagesTransferStatusButton->Show();
      break;
	default:
	  lAssert(false && "Unknown wage earner type");
	  break;
  };
}

void FDWagesScreen::OnTransferStatusButton( lUIControlWidget * )
{
  int row = m_wagesTable->GetSelectedRow();

  const int iPersonListIndex = (int)m_wagesTable->GetTagDataForRow(row);
  m_bComingFromDialogBox = true;
  switch( m_personList(iPersonListIndex).type )
  {
    case WAGE_MANAGER:
    case WAGE_EMPLOYEE:
    case WAGE_YOUTH:
      break;
    case WAGE_PLAYER:
      WorldData().SetCurrentPlayer( (CPlayer*) m_personList(iPersonListIndex).person );
      WorldData().SetCurrentClub( WorldData().GetCurrentUserClub() );
      theApp.OnChangeTransferStatus();

	  // ***TODO: determine if the transfer status actually changed, and take action
	  // accordingly.
	  GetMainUI()->ForceUpdateForAllScreensOnNextShow();
      break;
	default:
	  lAssert(false && "Unknown wage earner type");
	  break;
  };
  m_bComingFromDialogBox = false;
}

void FDWagesScreen::OnSackButton( lUIControlWidget * )
{
  s_lastActionSack = true;

  int row = m_wagesTable->GetSelectedRow();

  const int iPersonListIndex = (int)m_wagesTable->GetTagDataForRow(row);
  m_bComingFromDialogBox = true;
  switch( m_personList(iPersonListIndex).type )
  {
    case WAGE_MANAGER:
      break;
    case WAGE_EMPLOYEE:
      lLogFmt("Sack Employee\n");
      // TODO: prevent sacking a new employee, will cause an assertion
      WorldData().SetCurrentEmployee( (CEmployee*) m_personList(iPersonListIndex).person );
      if (theApp.OnSackEmployee() == true)
	  {
		  // Update the table.
		  m_personList(iPersonListIndex).person = NULL;
		  m_wagesTable->RemoveRows(row, 1);
	  }
      break;
    case WAGE_YOUTH:
      WorldData().SetCurrentYouth( (CYouth*) m_personList(iPersonListIndex).person );
      lLogFmt("Sack Youth\n");
      if (theApp.OnSackYouth() == true)
	  {
		  // Update the table.
		  m_personList(iPersonListIndex).person = NULL;
		  m_wagesTable->RemoveRows(row, 1);

		  // Needs to update the youth screen.
		  GetMainUI()->ForceUpdateForAllScreensOnNextShow();
	  }
      break;
    case WAGE_PLAYER:
      WorldData().SetCurrentPlayer( (CPlayer*) m_personList(iPersonListIndex).person );
      WorldData().SetCurrentClub( WorldData().GetCurrentUserClub() );
      if (theApp.OnSackPlayer() == true)
	  {
		  // Update the table.
		  m_personList(iPersonListIndex).person = NULL;
		  m_wagesTable->RemoveRows(row, 1);

		  // Needs to update multiple screens...
		  GetMainUI()->ForceUpdateForAllScreensOnNextShow();
	  }
      break;
	default:
	  lAssert(false && "Unknown wage earner type");
	  break;
  };

  m_bComingFromDialogBox = false;

  s_lastActionSack = false;
}

void FDWagesScreen::OnContractButton( lUIControlWidget * )
{
  int row = m_wagesTable->GetSelectedRow();

  const int iPersonListIndex = (int)m_wagesTable->GetTagDataForRow(row);

  bool bDoFullUpdate = false;

  m_bComingFromDialogBox = true;
  switch( m_personList(iPersonListIndex).type )
  {
    case WAGE_MANAGER:
      break;
    case WAGE_EMPLOYEE:
      lLogFmt("Offer contract\n");
      WorldData().SetCurrentEmployee( (CEmployee*) m_personList(iPersonListIndex).person );
      if (m_personList(iPersonListIndex).person->WillNegotiateContract() != true)
    	{
        CString strMessage;
        strMessage.Format( GetRandomNoContractResponse(), m_personList(iPersonListIndex).person->GetName());
        UserMessageBox(strMessage, MB_OK + MB_ICONEXCLAMATION + MB_DEFBUTTON1) ;
    	}
    	else
    	{
			GetMainUI()->GetNegotiateScreen()->ResetLastContractOfferResponse();

    		theApp.OnNegotiateEmployeeContract();

			if (GetMainUI()->GetNegotiateScreen()->GetLastContractOfferResponse() == ACCEPTED)
			{
				// Update the table entry.
				UpdateTableEntryForEmployee(iPersonListIndex);
				m_wagesTable->Resort();
				GetMainUI()->GetEmployeeInfoScreen()->SetEmployeeInfo((CEmployee *)m_personList(iPersonListIndex).person);
			}
    	}
      break;
    case WAGE_YOUTH:
      lLogFmt("Promote\n");
      s_lastActionSack = true;
      WorldData().SetCurrentYouth( (CYouth*) m_personList(iPersonListIndex).person );
      theApp.OnPromoteYouth();

      if (GetMainUI()->GetNegotiateScreen()->GetLastContractOfferResponse() == ACCEPTED)
      {
        lLogFmt(" Youth promoted\n");
        // ***TODO: Remove the youth's table entry, and add one for the new player.
        // ***TODO: Right now, this will just do a full update, but this is not ideal.
        bDoFullUpdate = true;
        GetMainUI()->ForceUpdateForAllScreensOnNextShow();
      } else
      {
        lLogFmt(" Youth not promoted\n");
      }

      s_lastActionSack = false;
      break;
    case WAGE_PLAYER:

      WorldData().SetCurrentPlayer( (CPlayer*) m_personList(iPersonListIndex).person );
      WorldData().SetCurrentClub( WorldData().GetCurrentUserClub() );
      GetMainUI()->GetNegotiateScreen()->ResetLastContractOfferResponse();
      theApp.OnNegotiatePlayerContract();

	  if (GetMainUI()->GetNegotiateScreen()->GetLastContractOfferResponse() == ACCEPTED)
	  {
		  // Update the table entry for the player.
		  UpdateTableEntryForPlayer(iPersonListIndex);
		  m_wagesTable->Resort();
		  GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( (CPlayer*)  m_personList(iPersonListIndex).person );
		  GetMainUI()->HideSubMessage();
	  }
      break;
	default:
	  lAssert(false && "Unknown wage earner type");
	  break;
  };

  m_bComingFromDialogBox = false;

  if( bDoFullUpdate )
  {
    Update();
    m_wagesTable->SelectRow( -1 );
  }
}

//////////////////////////////////////////////////////////////////////////
// Function to add a player to the wages table.
//////////////////////////////////////////////////////////////////////////
void FDWagesScreen::AddPlayerToTable(CPlayer *pPlayer)
{
	m_wagesTable->AddRows(1);
	//m_Grid.SetRowPointerDetails(CurrRow, pPlayer, PLAYERCONTRACT);
	m_personList.Add(1);
	m_personList.Last().person = (CPerson*) pPlayer;
	m_personList.Last().type = WAGE_PLAYER;

	if( pPlayer->IsOnLoan() )
		lLogFmt("%s on loan\n", pPlayer->GetName() );

	// Calculate total player wages
	m_dTotalPlayerWages +=  pPlayer->GetWage() * 52;
	m_wagesTable->SetTagDataForRow(m_wagesTable->GetRowCount() - 1,  m_personList.Size() - 1);
	UpdateTableEntryForPlayer(m_personList.Size() - 1);
}

//////////////////////////////////////////////////////////////////////////
// Updates the column headers based on the selection in the combo box.
//////////////////////////////////////////////////////////////////////////
void FDWagesScreen::UpdateColumnText()
{
	switch(m_WhichEvents)
	{
		case WAGE_DISPLAY_WAGE:
      m_wagesTable->SetText( 0, 6, lUIText( IDS_WAGES ) );
      m_wagesTable->SetSortFunctionForColumn(6, FDTableWidget::GenericMoneySort);
			break;
		case WAGE_DISPLAY_CONTRACT:
      m_wagesTable->SetText( 0, 6, lUIText( IDS_CONTRACT ) );
      m_wagesTable->SetSortFunctionForColumn(6, FDTableWidget::GenericDateSort);
			break;
		default:
			lAssert(false && "Unknown display type");
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Function that updates the row text for a manager.
//////////////////////////////////////////////////////////////////////////
void FDWagesScreen::UpdateTableEntryForManager(const int iPersonListIndex)
{
	lAssert(m_personList(iPersonListIndex).type == WAGE_MANAGER);

	const int iManagerRowIndex = m_wagesTable->FindTagData(iPersonListIndex, true);
	if (iManagerRowIndex < 0)
	{
		lAssert(false && "manager not found");
		return;
	}

	CClub& theClub = *WorldData().GetCurrentUserClub();
	CManager *pManager = (CManager *)m_personList(iPersonListIndex).person;

	m_wagesTable->SetBkgColour(iManagerRowIndex, 0, L_ARGB(128,130,130,130) );
	m_wagesTable->SetBkgColour(iManagerRowIndex, 1, L_ARGB(128,130,130,130) );
	m_wagesTable->SetBkgColour(iManagerRowIndex, 2, L_ARGB(128,80,80,80) );
	m_wagesTable->SetBkgColour(iManagerRowIndex, 3, L_ARGB(128,80,80,80) );
	m_wagesTable->SetBkgColour(iManagerRowIndex, 4, L_ARGB(128,130,130,130) );
	m_wagesTable->SetBkgColour(iManagerRowIndex, 5, L_ARGB(128,80,80,80) );
  m_wagesTable->SetBkgColour(iManagerRowIndex, 6, L_ARGB(128,80,80,80) );

  m_wagesTable->SetText(iManagerRowIndex, 1, lUIText( theClub.GetManagerName()));
  m_wagesTable->SetText(iManagerRowIndex, 3, lUIText( IDS_MANAGER ) );
	switch(m_WhichEvents)
	{
		case WAGE_DISPLAY_WAGE:
      m_wagesTable->SetText(iManagerRowIndex, 6, lUIText( theClub.GetForeignStr(theClub.GetManager().GetWage() * 52)) );
			break;
		case WAGE_DISPLAY_CONTRACT:
      m_wagesTable->SetText(iManagerRowIndex, 6, lUIText( theClub.GetManager().ContractMediumStr()) );
			break;
		default:
			lAssert(false && "Unknown display type");
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Function that updates the row text for an employee.
//////////////////////////////////////////////////////////////////////////
void FDWagesScreen::UpdateTableEntryForEmployee(const int iPersonListIndex)
{
	lAssert(m_personList(iPersonListIndex).type == WAGE_EMPLOYEE);

	const int iEmployeeRowIndex = m_wagesTable->FindTagData(iPersonListIndex, true);
	if (iEmployeeRowIndex < 0)
	{
		lAssert(false && "employee not found");
		return;
	}

	CClub& theClub = *WorldData().GetCurrentUserClub();
	CEmployee *pEmployee = (CEmployee *)m_personList(iPersonListIndex).person;

	m_wagesTable->SetBkgColour(iEmployeeRowIndex, 0, L_ARGB(128,130,130,130) );
	m_wagesTable->SetBkgColour(iEmployeeRowIndex, 1, L_ARGB(128,130,130,130) );
	m_wagesTable->SetBkgColour(iEmployeeRowIndex, 2, L_ARGB(128,80,80,80) );
	m_wagesTable->SetBkgColour(iEmployeeRowIndex, 3, L_ARGB(128,80,80,80)  );
	m_wagesTable->SetBkgColour(iEmployeeRowIndex, 4, L_ARGB(128,130,130,130) );
	m_wagesTable->SetBkgColour(iEmployeeRowIndex, 5, L_ARGB(128,80,80,80) );
  m_wagesTable->SetBkgColour(iEmployeeRowIndex, 6, L_ARGB(128,80,80,80) );

	m_wagesTable->SetText( iEmployeeRowIndex, 1, lUIText(pEmployee->GetName() ) );
	m_wagesTable->SetText( iEmployeeRowIndex, 3, lUIText( pEmployee->TypeTextStr() ) );
	switch(m_WhichEvents)
	{
		case WAGE_DISPLAY_WAGE:
      m_wagesTable->SetText( iEmployeeRowIndex, 6, lUIText(theClub.GetForeignStr(pEmployee->GetWage() * 52) ) );
			break;
		case WAGE_DISPLAY_CONTRACT:
      m_wagesTable->SetText( iEmployeeRowIndex, 6, lUIText(pEmployee->ContractMediumStr() ) );
			break;
		default:
			lAssert(false && "Unknown display type");
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Function that updates the row text for a player.
//////////////////////////////////////////////////////////////////////////
void FDWagesScreen::UpdateTableEntryForPlayer(const int iPersonListIndex)
{
	lAssert(m_personList(iPersonListIndex).type == WAGE_PLAYER);

	const int iPlayerRowIndex = m_wagesTable->FindTagData(iPersonListIndex, true);
	if (iPlayerRowIndex < 0)
	{
		lAssert(false && "player not found");
		return;
	}

	CClub& theClub = *WorldData().GetCurrentUserClub();
	CPlayer *pPlayer = (CPlayer *)m_personList(iPersonListIndex).person;

	m_wagesTable->SetBkgColour(iPlayerRowIndex, 0, PosnColour[pPlayer->GetSkills().GetPosition()] );
	m_wagesTable->SetBkgColour(iPlayerRowIndex, 1, PosnColour[pPlayer->GetSkills().GetPosition()] );
	m_wagesTable->SetBkgColour(iPlayerRowIndex, 2, L_ARGB(128,80,80,80) );
	m_wagesTable->SetBkgColour(iPlayerRowIndex, 3, L_ARGB(128,80,80,80) );
	m_wagesTable->SetBkgColour(iPlayerRowIndex, 4, L_ARGB(128,130,130,130) );
	m_wagesTable->SetBkgColour(iPlayerRowIndex, 5, L_ARGB(128,80,80,80)  );
  m_wagesTable->SetBkgColour(iPlayerRowIndex, 6, L_ARGB(128,80,80,80)  );

	m_wagesTable->SetText(iPlayerRowIndex, 1, lUIText( FDMakePlayerNameLink(pPlayer) ));
	//m_wagesTable->SetTextColour(iPlayerRowIndex, 1, PosnColour[pPlayer->GetSkills().GetPosition()]);
	m_wagesTable->SetText(iPlayerRowIndex, 3, lUIText( pPlayer->GetSkills().PosnLongAbbr()));
	//m_wagesTable->SetTextColour(iPlayerRowIndex, 3, PosnColour[pPlayer->GetSkills().GetPosition()]);
	switch(m_WhichEvents)
	{
		case WAGE_DISPLAY_WAGE:
      m_wagesTable->SetText(iPlayerRowIndex, 6, lUIText( theClub.GetForeignStr(pPlayer->GetWage() * 52) ) );
			break;
		case WAGE_DISPLAY_CONTRACT:
      m_wagesTable->SetText(iPlayerRowIndex, 6, lUIText( pPlayer->ContractMediumStr() ) );
			break;
		default:
			lAssert(false && "Unknown display type");
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Function that updates the row text for a youth.
//////////////////////////////////////////////////////////////////////////
void FDWagesScreen::UpdateTableEntryForYouth(const int iPersonListIndex)
{
	lAssert(m_personList(iPersonListIndex).type == WAGE_YOUTH);

	const int iYouthRowIndex = m_wagesTable->FindTagData(iPersonListIndex, true);
	if (iYouthRowIndex < 0)
	{
		lAssert(false && "youth not found");
		return;
	}

	CClub& theClub = *WorldData().GetCurrentUserClub();
	CYouth *pYouth = (CYouth *)m_personList(iPersonListIndex).person;

	m_wagesTable->SetBkgColour(iYouthRowIndex, 0, PosnColour[pYouth->GetSkills().GetPosition()] );
	m_wagesTable->SetBkgColour(iYouthRowIndex, 1, PosnColour[pYouth->GetSkills().GetPosition()] );
	m_wagesTable->SetBkgColour(iYouthRowIndex, 2, L_ARGB(128,80,80,80) );
	m_wagesTable->SetBkgColour(iYouthRowIndex, 3, L_ARGB(128,80,80,80) );
	m_wagesTable->SetBkgColour(iYouthRowIndex, 4, L_ARGB(128,130,130,130) );
	m_wagesTable->SetBkgColour(iYouthRowIndex, 5, L_ARGB(128,80,80,80)  );
  m_wagesTable->SetBkgColour(iYouthRowIndex, 6, L_ARGB(128,80,80,80)  );

	m_wagesTable->SetText(iYouthRowIndex, 1, lUIText( pYouth->GetName() ) );
	//m_wagesTable->SetTextColour(iYouthRowIndex, 1, PosnColour[pYouth->GetSkills().GetPosition()]);

  lString str;
  str.Format("%s-Yth", lText::Text(pYouth->GetSkills().PosnLongAbbr()));
  m_wagesTable->SetText(iYouthRowIndex, 3, lUIText( str ) );
	//m_wagesTable->SetTextColour(iYouthRowIndex, 3, PosnColour[pYouth->GetSkills().GetPosition()]);
	switch(m_WhichEvents)
	{
		case WAGE_DISPLAY_WAGE:
      m_wagesTable->SetText(iYouthRowIndex, 6, lUIText( theClub.GetForeignStr(pYouth->GetWage() * 52) ) );
			break;
		case WAGE_DISPLAY_CONTRACT:
      m_wagesTable->SetText(iYouthRowIndex, 6, lUIText( pYouth->ContractMediumStr() ) );
			break;
		default:
			lAssert(false && "Unknown display type");
			break;
	}
}
