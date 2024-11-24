//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDJobCentrescreen.cpp
// Description : FDJobCentreScreen implementation
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
#include "Footballdirector.h"

#include <screens/fdjobcentrescreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDJobCentreScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDJobCentreScreen::FDJobCentreScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDJobCentreScreen::~FDJobCentreScreen()
{
}

void FDJobCentreScreen::Init()
{
  m_bComingFromDialogBox = false;
  m_currentView = JC_VIEW_CLUB;

  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_jobCentreScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "jobcentre_screen" );
  m_jobCentreScreen->Hide();

  // Print names above each table

  // * Table for player names

  m_table = FD_GAME_NEW FDTableWidget( m_jobCentreScreen, 0, "jobcentretable",
            FDTableWidget::TABLE_FLAG_BORDER|FDTableWidget::TABLE_FLAG_ROW_SELECT|
            FDTableWidget::TABLE_FLAG_CELLBACKGROUND |FDTableWidget::TABLE_FLAG_BACKGROUND|FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW |
			FDTableWidget::TABLE_FLAG_CAP_FIRSTROW,
            LUI_TOP_LEFT, lUIPoint( 20, 10 ), lUIPoint( 256 - 20 - GetDefaultScrollBarWidth(), 144 ) );

  FDScrollBarWidget *pScrollBar = FD_GAME_NEW FDScrollBarWidget(m_jobCentreScreen, 0, "tablescroll", LUI_TOP_LEFT, lUIPoint(256-20, 10), lUIPoint(GetDefaultScrollBarWidth(), 146));
  pScrollBar->AlignTo( m_table );
  m_table->SetScrollBar( pScrollBar );
  m_table->SignalRowSelect.Connect( this, &FDJobCentreScreen::OnRowSelect );

  // Set up the table columns.
  m_table->AddColumn( lUICoord( 127),  LUI_LEFT_CENTER );  // name
  m_table->AddColumn( lUICoord( 10 ),  LUI_LEFT_CENTER );  // gap
  m_table->AddColumn( lUICoord( 30 ), LUI_CENTER_CENTER );   // skill
  m_table->AddColumn( lUICoord( 10 ), LUI_LEFT_CENTER );   // gap
  m_table->AddColumn( lUICoord( 40 ),  LUI_CENTER_CENTER );   // type
  m_table->AddColumn( lUICoord( 10 ),  LUI_LEFT_CENTER );   // gap

  // Set up header.
  m_table->AddRows( 1 );
  m_table->SetText( 0, 0, lUIText( IDS_NAME ) );  // name
  m_table->SetText( 0, 2, lUIText( IDS_SKILL_SINGLE ) );   // skill
  m_table->SetText( 0, 4, lUIText( IDS_ROLE ) );   // type
  m_table->SetRowBkgColour(0, L_ARGB(255, 35, 35, 35));

  m_table->SetSortWhenColumnHeaderClicked(true);
  m_table->SetSortFunctionForColumn(0, FDTableWidget::GenericFirstInitialLastNameSort);
  m_table->SetSortFunctionForColumn(2, FDTableWidget::GenericNumericSort);
  m_table->SetSortFunctionForColumn(4, FDTableWidget::GenericStringSort);
  m_table->ClearTiebreakerColumns();
  m_table->AddTiebreakerColumn(4);
  m_table->AddTiebreakerColumn(0);

  m_table->Show();

  //
  m_tableSelect = FD_GAME_NEW FDComboWidget( m_jobCentreScreen, 0, "jctable_combo", lUIText(""), LUI_TOP_RIGHT, lUIPointPercent( 89, 85 ) );
  m_tableSelect->SetAllToUpper( true );

  m_tableSelect->AddItem( IDS_CLUB );
  m_tableSelect->AddItem( IDS_AVAILABLE );

  m_tableSelect->SignalSelection.Connect( this, &FDJobCentreScreen::OnTableSelect );

  // buttons

  m_hireButton = FD_GAME_NEW FDButtonWidget( m_jobCentreScreen, 0, "jchire_button", FDResourceManager::GetSprite("uiicon","hire"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 35, 175 ) );
  m_sackButton = FD_GAME_NEW FDButtonWidget( m_jobCentreScreen, 0, "jcfire_button", FDResourceManager::GetSprite("uiicon","fire"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 35, 175 ) );
  m_offerContractButton = FD_GAME_NEW FDButtonWidget( m_jobCentreScreen, 0, "jcoffercontract_button", FDResourceManager::GetSprite("uiicon","offercontract"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 65, 175 ) );

  m_hireButton->Hide();
  m_sackButton->Hide();
  m_offerContractButton->Hide();

  m_hireButton->SignalControlCommand.Connect( this, &FDJobCentreScreen::OnHireButton );
  m_sackButton->SignalControlCommand.Connect( this, &FDJobCentreScreen::OnSackButton );
  m_offerContractButton->SignalControlCommand.Connect( this, &FDJobCentreScreen::OnOfferContractButton );

  m_availableEmployeeSelected = -1;
  m_clubEmployeeSelected = -1;
}

void FDJobCentreScreen::OnTableSelect(FDComboWidget *, int idx)
{
	m_currentView = (ViewMode)idx;

	Update();
}

void FDJobCentreScreen::Update()
{
  // Prevent an automatic update when returning from a dialog box
  if( m_bComingFromDialogBox == true )
    return;

	switch (m_currentView)
	{
	case JC_VIEW_CLUB:
		PopulateClubTable();
		break;
	case JC_VIEW_AVAILABLE:
		PopulateAvailableTable();
		break;
	default:
		lAssert(false && "unknown view type");
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// Used to set up the table with the employees currently at the club.
//////////////////////////////////////////////////////////////////////////
void FDJobCentreScreen::PopulateClubTable()
{
	// Clear all non-header rows.
	if (m_table->GetRowCount() > 1)
		m_table->RemoveRows(1, m_table->GetRowCount() - 1);

	CEmployeeList &employeeList = WorldData().GetCurrentUserClub()->GetEmployeeList();
	int count = employeeList.GetSize();
	lLogFmt("%d employees in club\n", count);

	m_clubEmployees.Empty();

	// set player info up
	for (int i = 0; i < count; i++)
	{
		if(employeeList[i] == NULL)
		{
			continue;
		}

		m_table->AddRows( 1 );

		CEmployee* pEmployee = (CEmployee*)employeeList.GetAt( i );
		m_clubEmployees.Push( pEmployee );

		const int iEmployeeRow = m_table->GetRowCount() - 1;
		m_table->SetTagDataForRow(iEmployeeRow, m_clubEmployees.Size() - 1);

		UpdateTableRow(iEmployeeRow, pEmployee);
	}

	m_table->Resort();
}

//////////////////////////////////////////////////////////////////////////
// Used to set up the table with the list of available employees.
//////////////////////////////////////////////////////////////////////////
void FDJobCentreScreen::PopulateAvailableTable()
{
	// Clear all non-header rows.
	if (m_table->GetRowCount() > 1)
		m_table->RemoveRows(1, m_table->GetRowCount() - 1);

    CEmployeeList &employeeList = WorldData().GetCurrentUserClub()->GetActiveCountry().GetAvailableEmployeeList();
    int count = employeeList.GetSize();
    lLogFmt("%d employees in available\n", count);

    m_availableEmployees.Empty();

    // set employee info up
    for (int i = 0; i < count; i++)
    {
		if(employeeList[i] == NULL)
		{
			continue;
		}

		m_table->AddRows( 1 );

		CEmployee* pEmployee = (CEmployee*)employeeList.GetAt( i );
		m_availableEmployees.Push( pEmployee );

		const int iEmployeeRow = m_table->GetRowCount() - 1;
		m_table->SetTagDataForRow(iEmployeeRow, m_availableEmployees.Size() - 1);

		UpdateTableRow(iEmployeeRow, pEmployee);
    }

	m_table->Resort();
}

void FDJobCentreScreen::UpdateTableRow(const int iRow, CEmployee *pEmployee)
{
	// set name
	m_table->SetBkgColour(iRow, 0, L_ARGB(128,130,130,130) );
	m_table->SetBkgColour(iRow, 1, L_ARGB(128,130,130,130) );
	m_table->SetBkgColour(iRow, 2, L_ARGB(128,80,80,80) );
	m_table->SetBkgColour(iRow, 3, L_ARGB(128,130,130,130) );
	m_table->SetBkgColour(iRow, 4, L_ARGB(128,80,80,80) );
	m_table->SetBkgColour(iRow, 5, L_ARGB(128,130,130,130) );

	m_table->SetText(iRow, 0, lUIText( pEmployee->GetName() ) );
	m_table->SetText(iRow, 2, lUIText( pEmployee->GetSkillStr() ) );

	if( pEmployee->GetType() == CEmployeeBase::COACH )
	{
    m_table->SetText(iRow, 4, lUIText( IDS_COACH ) );
	}
	else if( pEmployee->GetType() == CEmployeeBase::PHYSIO )
	{
    m_table->SetText(iRow, 4, lUIText( IDS_PHYSIO ) );
	}
}

void FDJobCentreScreen::SerializeState( FDStateStorage &state )
{
  if( state.IsSaving() )
  {
    int tablesel   = m_tableSelect->GetSelectedItem();
    int tableidx   = m_table->GetSelectedRow();

    state << tablesel;
    state << tableidx;

    lLogFmt("Saving table %d idx %d\n", tablesel, tableidx);
  } else
  {
    int tablesel;
    int tableidx;

    state << tablesel;
    state << tableidx;

    lLogFmt("Loading table %d idx %d\n", tablesel, tableidx);

    m_tableSelect->SetSelectedItem(tablesel, false);
    m_table->SelectRow(tableidx);
  }
}


void FDJobCentreScreen::Prepare()
{
	if (m_bComingFromDialogBox == false)
		GetMainUI()->DisplaySubMessage("Select Employee"); //TODO LOCALE
}

void FDJobCentreScreen::DoShow()
{
  m_jobCentreScreen->Show();

  if (m_bComingFromDialogBox == false)
  {
   Update();
  }
}

void FDJobCentreScreen::DoHide()
{
  m_jobCentreScreen->Hide();
}

void FDJobCentreScreen::OnRowSelect(FDTableWidget *pTable, int row)
{
	switch (m_currentView)
	{
	case JC_VIEW_CLUB:
		OnClubRowSelect(pTable, row);
		break;
	case JC_VIEW_AVAILABLE:
		OnAvailableRowSelect(pTable, row);
		break;
	default:
		lAssert(false && "Unknown view type");
		break;
	}
}

void FDJobCentreScreen::OnClubRowSelect( FDTableWidget *, int row )
{
  // clear selection
  m_hireButton->Hide();
  m_sackButton->Hide();
  m_offerContractButton->Hide();

  lLogFmt("Selected club row  %d\n", row);

  if (row < 0)
  {
	  GetMainUI()->GetEmployeeInfoScreen()->Hide();
	  GetMainUI()->DisplaySubMessage("Select Employee"); //TODO LOCALE
	  return;
  }

  GetMainUI()->HideSubMessage();
  GetMainUI()->GetEmployeeInfoScreen()->Show();

  // Set employee info on top screen
  int idx = row-1;

  if( idx != -1 && m_clubEmployees.Size())
  {
    const int iEmployeeListIdx = m_table->GetTagDataForRow(row);
    WorldData().SetCurrentEmployee( m_clubEmployees( iEmployeeListIdx ) );
    GetMainUI()->GetEmployeeInfoScreen()->SetEmployeeInfo( m_clubEmployees( iEmployeeListIdx ) );
    m_sackButton->Show();
    m_offerContractButton->Show();

    m_clubEmployeeSelected = iEmployeeListIdx;

    // Display possible button options for this selection
    // Sack
    // Offer Contract

    //TODO: Return to this screen (and update this screen) after action complete
  } else
  {
    //TODO: What to do with top screen on no selection?
  }

}

void FDJobCentreScreen::OnAvailableRowSelect( FDTableWidget *, int row )
{
  // clear selection
  m_hireButton->Hide();
  m_sackButton->Hide();
  m_offerContractButton->Hide();

  lLogFmt("Selected available row  %d\n", row);

  if (row < 0)
  {
	  GetMainUI()->GetEmployeeInfoScreen()->Hide();
	  GetMainUI()->DisplaySubMessage("Select Employee"); //TODO LOCALE
	  return;
  }

  GetMainUI()->HideSubMessage();
  GetMainUI()->GetEmployeeInfoScreen()->Show();

  // Set employee info on top screen
  int idx = row-1;

  if( idx != -1 && m_availableEmployees.Size() )
  {
    // Display possible button options for this selection
    // Hire

    const int iAvailableEmployeeIdx = m_table->GetTagDataForRow(row);
    WorldData().SetCurrentEmployee( m_availableEmployees( iAvailableEmployeeIdx ) );
    GetMainUI()->GetEmployeeInfoScreen()->SetEmployeeInfo( m_availableEmployees( iAvailableEmployeeIdx ) );
    m_hireButton->Show();

    m_availableEmployeeSelected = iAvailableEmployeeIdx;


    //TODO: Return to this screen (and update this screen) after action complete
  } else
  {
    //TODO: What to do with top screen on no selection?
  }

}

extern CFootballDirectorApp theApp;

void FDJobCentreScreen::OnHireButton( lUIControlWidget * )
{
	lLogFmt("Hire\n");

	if (WorldData().GetCurrentUserClub()->NeedEmployee(m_availableEmployees( m_availableEmployeeSelected )->GetType(), true) == true)
	{
		GetMainUI()->GetNegotiateScreen()->ResetLastContractOfferResponse();
		GetMainUI()->GetNegotiateScreen()->SetOnHireEmployee( m_availableEmployees( m_availableEmployeeSelected ) );
		m_bComingFromDialogBox = true;
		theApp.OnHireEmployee();
		m_bComingFromDialogBox = false;

		if (GetMainUI()->GetNegotiateScreen()->GetLastContractOfferResponse() == ACCEPTED)
		{
			// Remove the employee from the pool.
			if( m_table->GetSelectedRow() != -1 )
				m_table->RemoveRows(m_table->GetSelectedRow(), 1);
		}
		else
			GetMainUI()->GetEmployeeInfoScreen()->Show();
	}
}

void FDJobCentreScreen::OnSackButton( lUIControlWidget * )
{
  lLogFmt("Sack\n");
  m_bComingFromDialogBox = true;
  if (theApp.OnSackEmployee() == true)
  {
	  // Remove the employee from the club.
    if( m_table->GetSelectedRow() != -1 )
      m_table->RemoveRows(m_table->GetSelectedRow(), 1);
  }
  else
	GetMainUI()->GetEmployeeInfoScreen()->Show();

  m_bComingFromDialogBox = false;
}

void FDJobCentreScreen::OnOfferContractButton( lUIControlWidget * )
{
	lLogFmt("Offer contract\n");

	m_bComingFromDialogBox = true;
	if (m_clubEmployees( m_clubEmployeeSelected )->WillNegotiateContract() != true)
	{
		CString strMessage;
		strMessage.Format( GetRandomNoContractResponse(), m_clubEmployees( m_clubEmployeeSelected )->GetName());
		UserMessageBox(strMessage, MB_OK + MB_ICONEXCLAMATION + MB_DEFBUTTON1);
	}
	else
	{
		GetMainUI()->GetNegotiateScreen()->ResetLastContractOfferResponse();
		m_bComingFromDialogBox = true;
		theApp.OnNegotiateEmployeeContract();
		m_bComingFromDialogBox = false;

		if (GetMainUI()->GetNegotiateScreen()->GetLastContractOfferResponse() == ACCEPTED)
		{
			// Update the entry for the employee.
			const int iTableEntry = m_table->FindTagData(m_clubEmployeeSelected, true);
			UpdateTableRow(iTableEntry, m_clubEmployees(m_clubEmployeeSelected));
		}
	}

	m_bComingFromDialogBox = false;
}
