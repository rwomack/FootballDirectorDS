//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDYouthSquadscreen.cpp
// Description : FDYouthSquadScreen implementation
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

#include <screens/fdyouthsquadscreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDYouthSquadScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

static bool s_lastActionSack = false;
extern bool g_hackLastMsgResponse;

FDYouthSquadScreen::FDYouthSquadScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDYouthSquadScreen::~FDYouthSquadScreen()
{
}

void FDYouthSquadScreen::Init()
{
  m_bComingFromDialogBox = false;

  m_youthSquadScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "youthsquad_screen" );
  m_youthSquadScreen->Hide();

  // Print names above each table

  // * Table for player names

  m_clubTable = FD_GAME_NEW FDTableWidget( m_youthSquadScreen, 0, "ysclubtable",
            /*FDTableWidget::TABLE_FLAG_ROW_LINES|FDTableWidget::TABLE_FLAG_COLUMN_LINES|*/
            FDTableWidget::TABLE_FLAG_BORDER|FDTableWidget::TABLE_FLAG_ROW_SELECT|
			FDTableWidget::TABLE_FLAG_CELLBACKGROUND|FDTableWidget::TABLE_FLAG_BACKGROUND | FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW |
			FDTableWidget::TABLE_FLAG_CAP_FIRSTROW,
            LUI_TOP_LEFT, lUIPoint( 20, 10 ), lUIPoint( 256 - 20 - GetDefaultScrollBarWidth(), 144 ) );

  FDScrollBarWidget *pClubScrollBar = FD_GAME_NEW FDScrollBarWidget(m_youthSquadScreen, 0, "clubtablescroll", LUI_TOP_LEFT, lUIPoint(248, 10), lUIPoint(GetDefaultScrollBarWidth(), 146));
  pClubScrollBar->AlignTo( m_clubTable );
  m_clubTable->SetScrollBar(pClubScrollBar);
  m_clubTable->SignalRowSelect.Connect( this, &FDYouthSquadScreen::OnClubRowSelect );
  m_clubTable->Show();

  //

  m_availableTable = FD_GAME_NEW FDTableWidget( m_youthSquadScreen, 0, "ysavailabletable",
            /*FDTableWidget::TABLE_FLAG_ROW_LINES|FDTableWidget::TABLE_FLAG_COLUMN_LINES|*/
            FDTableWidget::TABLE_FLAG_BORDER|FDTableWidget::TABLE_FLAG_ROW_SELECT|
            FDTableWidget::TABLE_FLAG_CELLBACKGROUND|FDTableWidget::TABLE_FLAG_BACKGROUND|FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW |
			FDTableWidget::TABLE_FLAG_CAP_FIRSTROW,
            LUI_TOP_LEFT, lUIPoint( 20, 10 ), lUIPoint( 256 - 20 - GetDefaultScrollBarWidth(), 144 ) );

  FDScrollBarWidget *pAvailableScrollBar = FD_GAME_NEW FDScrollBarWidget(m_youthSquadScreen, 0, "availabletablescroll", LUI_TOP_LEFT, lUIPoint(248, 10), lUIPoint(GetDefaultScrollBarWidth(), 146));
  pAvailableScrollBar->AlignTo( m_availableTable );
  m_availableTable->SetScrollBar(pAvailableScrollBar);
  m_availableTable->SignalRowSelect.Connect( this, &FDYouthSquadScreen::OnAvailableRowSelect );
  m_availableTable->Hide();

  //

  m_tableSelect = FD_GAME_NEW FDComboWidget( m_youthSquadScreen, 0, "ystable_combo", lUIText(""), LUI_TOP_RIGHT, lUIPointPercent( 89, 85 ) );
  m_tableSelect->SetAllToUpper( true );

  m_tableSelect->AddItem( IDS_CLUB );
  m_tableSelect->AddItem( IDS_AVAILABLE );

  m_tableSelect->SignalSelection.Connect( this, &FDYouthSquadScreen::OnTableSelect );

  // buttons

  m_hireButton = FD_GAME_NEW FDButtonWidget( m_youthSquadScreen, 0, "yshire_button", FDResourceManager::GetSprite("uiicon","hire"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 35, 175 ) );
  m_sackButton = FD_GAME_NEW FDButtonWidget( m_youthSquadScreen, 0, "ysfire_button", FDResourceManager::GetSprite("uiicon","fire"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 35, 175 ) );
  m_promoteButton = FD_GAME_NEW FDButtonWidget( m_youthSquadScreen, 0, "yspromote_button", FDResourceManager::GetSprite("uiicon","offercontract"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_LEFT_CENTER, lUIPoint( 65, 175 ) );

  m_hireButton->Hide();
  m_sackButton->Hide();
  m_promoteButton->Hide();

  m_hireButton->SignalControlCommand.Connect( this, &FDYouthSquadScreen::OnHireButton );
  m_sackButton->SignalControlCommand.Connect( this, &FDYouthSquadScreen::OnSackButton );
  m_promoteButton->SignalControlCommand.Connect( this, &FDYouthSquadScreen::OnPromoteButton );
}

void FDYouthSquadScreen::OnTableSelect(FDComboWidget *, int idx)
{
	if (m_bComingFromDialogBox == true)
		return;

  WorldData().SetCurrentYouth( NULL );

  if( idx == 0 )
  {
    m_clubTable->Show();
    if( m_clubTable->GetSelectedRow() < m_clubTable->GetRowCount() )
    {
      m_clubTable->SelectRow(m_clubTable->GetSelectedRow());
    } else
    {
      m_clubTable->SelectRow(-1);
    }
    m_availableTable->Hide();
  } else
  {
    m_clubTable->Hide();
    m_availableTable->Show();
    if( m_availableTable->GetSelectedRow() < m_availableTable->GetRowCount() )
    {
      m_availableTable->SelectRow(m_availableTable->GetSelectedRow());
    } else
    {
      m_availableTable->SelectRow(-1);
    }
  }
}

void FDYouthSquadScreen::SerializeState( FDStateStorage &state )
{
	if (m_bComingFromDialogBox == true)
  {
    lLogFmt("Not serializing after dialog box\n");
		return;
  }

  if( state.IsSaving() )
  {
    int tablesel       = m_tableSelect->GetSelectedItem();
    int clubtableidx   = m_clubTable->GetSelectedRow();
    int availtableidx  = m_availableTable->GetSelectedRow();

    state << tablesel;
    state << clubtableidx;
    state << availtableidx;

    lLogFmt("Saving table %d club %d avail %d\n", tablesel, clubtableidx, availtableidx);
  } else
  {
    int tablesel;
    int clubtableidx;
    int availtableidx;

    state << tablesel;
    state << clubtableidx;
    state << availtableidx;

    lLogFmt("Loading table %d club %d avail %d\n", tablesel, clubtableidx, availtableidx);

    m_tableSelect->SetSelectedItem(tablesel, false);
    m_clubTable->SelectRow(clubtableidx);
    m_availableTable->SelectRow(availtableidx);
  }
}

void FDYouthSquadScreen::Update()
{
  // Prevent automatic update on return from a dialog box
  if ( m_bComingFromDialogBox == true )
    return;

  // Update from tables

  // set up people in club
  {
    m_clubTable->ClearAll();

    m_clubTable->AddColumn( lUICoord( 81),  LUI_LEFT_CENTER );  // name
    m_clubTable->AddColumn( lUICoord( 20 ),  LUI_TOP_CENTER );   // age
    m_clubTable->AddColumn( lUICoord( 5 ),  LUI_LEFT_CENTER );  // gap
    m_clubTable->AddColumn( lUICoord( 20 ),  LUI_TOP_CENTER );   // skill
    m_clubTable->AddColumn( lUICoord( 5),  LUI_LEFT_CENTER );   // gap
    m_clubTable->AddColumn( lUICoord( 20 ),  LUI_TOP_CENTER );  // handling
    m_clubTable->AddColumn( lUICoord( 5 ),  LUI_LEFT_CENTER );   // gap
    m_clubTable->AddColumn( lUICoord( 20),  LUI_TOP_CENTER ); // tackling
    m_clubTable->AddColumn( lUICoord( 5 ),  LUI_LEFT_CENTER );   // gap
    m_clubTable->AddColumn( lUICoord( 20 ),  LUI_TOP_CENTER );  // passing
    m_clubTable->AddColumn( lUICoord( 5 ),  LUI_LEFT_CENTER );   // gap
    m_clubTable->AddColumn( lUICoord( 20 ),  LUI_TOP_CENTER );  // shooting

	m_clubTable->SetSortWhenColumnHeaderClicked(true);
	m_clubTable->SetSortFunctionForColumn(0, FDTableWidget::GenericFirstInitialLastNameSort);
	m_clubTable->SetSortFunctionForColumn(1, FDTableWidget::GenericNumericSort);
	m_clubTable->SetSortFunctionForColumn(3, FDTableWidget::GenericNumericSort);
	m_clubTable->SetSortFunctionForColumn(5, FDTableWidget::GenericNumericSort);
	m_clubTable->SetSortFunctionForColumn(7, FDTableWidget::GenericNumericSort);
	m_clubTable->SetSortFunctionForColumn(9, FDTableWidget::GenericNumericSort);
	m_clubTable->SetSortFunctionForColumn(11, FDTableWidget::GenericNumericSort);
	m_clubTable->ClearTiebreakerColumns();
	m_clubTable->AddTiebreakerColumn(0);

    CYouthList &youthList = WorldData().GetCurrentUserClub()->GetYouthList();
    int count = youthList.GetSize();
    lLogFmt("%d youths in club\n", count);

    m_clubYouths.Empty();

    // setup header
    m_clubTable->AddRows( 1 );
    m_clubTable->SetText( 0, 0, lUIText( IDS_NAME ) );  // name
    m_clubTable->SetText( 0, 1, lUIText( IDS_AGE ) );   // age
    m_clubTable->SetText( 0, 3, lUIText( IDS_SKILL_ABBR ) );   // skill
    m_clubTable->SetText( 0, 5, lUIText( IDS_HANDLING_ABBR ) );   // handling
    m_clubTable->SetText( 0, 7, lUIText( IDS_TACKLING_ABBR ) );   // tackling
    m_clubTable->SetText( 0, 9, lUIText( IDS_PASSING_ABBR ) );   // passing
    m_clubTable->SetText( 0, 11, lUIText( IDS_SHOOTING_ABBR ) );   // shooting

	m_clubTable->SetRowBkgColour(0, L_ARGB(255, 35, 35, 35));

    // set player info up
    for (int i = 0; i < count; i++)
    {
      if(youthList[i] == NULL)
      {
        continue;
      }

      CYouth* pYouth = (CYouth*)youthList.GetAt( i );
      m_clubYouths.Push( pYouth );

	  AddYouthToTable(m_clubTable, pYouth, m_clubYouths.Size() - 1);
    }
  }

  // set up available grid
  {
    m_availableTable->ClearAll();

    m_availableTable->AddColumn( lUICoord( 81),  LUI_LEFT_CENTER );  // name
	m_availableTable->AddColumn( lUICoord( 20 ),  LUI_TOP_CENTER );   // age
    m_availableTable->AddColumn( lUICoord( 5 ),  LUI_LEFT_CENTER );  // gap
    m_availableTable->AddColumn( lUICoord( 20 ),  LUI_TOP_CENTER );   // skill
    m_availableTable->AddColumn( lUICoord( 5),  LUI_LEFT_CENTER );   // gap
	m_availableTable->AddColumn( lUICoord( 20),  LUI_TOP_CENTER );	// handling
    m_availableTable->AddColumn( lUICoord( 5 ),  LUI_LEFT_CENTER );   // gap
	m_availableTable->AddColumn( lUICoord( 20),  LUI_TOP_CENTER );	// tackling
    m_availableTable->AddColumn( lUICoord( 5 ),  LUI_LEFT_CENTER );   // gap
	m_availableTable->AddColumn( lUICoord( 20 ),  LUI_TOP_CENTER );	// passing
    m_availableTable->AddColumn( lUICoord( 5 ),  LUI_LEFT_CENTER );   // gap
	m_availableTable->AddColumn( lUICoord( 20 ),  LUI_TOP_CENTER );	// shooting

	m_availableTable->SetSortWhenColumnHeaderClicked(true);
	m_availableTable->SetSortFunctionForColumn(0, FDTableWidget::GenericFirstInitialLastNameSort);
	m_availableTable->SetSortFunctionForColumn(1, FDTableWidget::GenericNumericSort);
	m_availableTable->SetSortFunctionForColumn(3, FDTableWidget::GenericNumericSort);
	m_availableTable->SetSortFunctionForColumn(5, FDTableWidget::GenericNumericSort);
	m_availableTable->SetSortFunctionForColumn(7, FDTableWidget::GenericNumericSort);
	m_availableTable->SetSortFunctionForColumn(9, FDTableWidget::GenericNumericSort);
	m_availableTable->SetSortFunctionForColumn(11, FDTableWidget::GenericNumericSort);
	m_availableTable->ClearTiebreakerColumns();
	m_availableTable->AddTiebreakerColumn(0);

    CYouthList &youthList = WorldData().GetCurrentUserClub()->GetActiveCountry().GetAvailableYouthList();
    int count = youthList.GetSize();
    lLogFmt("%d youths in available\n", count);

    m_availableYouths.Empty();

	// setup header
    m_availableTable->AddRows( 1 );
    m_availableTable->SetText( 0, 0, lUIText( IDS_NAME ) );  // name
    m_availableTable->SetText( 0, 1, lUIText( IDS_AGE ) );   // age
    m_availableTable->SetText( 0, 3, lUIText( IDS_SKILL_ABBR ) );   // skill
    m_availableTable->SetText( 0, 5, lUIText( IDS_HANDLING_ABBR ) );   // handling
    m_availableTable->SetText( 0, 7, lUIText( IDS_TACKLING_ABBR ) );   // tackling
    m_availableTable->SetText( 0, 9, lUIText( IDS_PASSING_ABBR ) );   // passing
    m_availableTable->SetText( 0, 11, lUIText( IDS_SHOOTING_ABBR ) );   // shooting

	m_availableTable->SetRowBkgColour(0, L_ARGB(255, 35, 35, 35));

	// set player info up
    for (int i = 0; i < count; i++)
    {
      if(youthList[i] == NULL)
      {
        continue;
      }

      CYouth* pYouth = (CYouth*)youthList.GetAt( i );
      m_availableYouths.Push( pYouth );

	  AddYouthToTable(m_availableTable, pYouth, m_availableYouths.Size() - 1);
    }
  }
}

void FDYouthSquadScreen::AddYouthToTable(FDTableWidget *pTable, CYouth *pYouth, const int iTagData)
{
	const unsigned int uiNewRow = pTable->GetRowCount();
	pTable->AddRows(1);

	pTable->SetTagDataForRow(uiNewRow, iTagData);
	pTable->SetText(uiNewRow, 0, lUIText(pYouth->GetName()));
	//pTable->SetTextColour(uiNewRow, 0, PosnColour[pYouth->GetSkills().GetPosition()]);
	pTable->SetBkgColour(uiNewRow, 0, PosnColour[pYouth->GetSkills().GetPosition()]);

	for (unsigned int i = 1; i < pTable->GetColumnCount(); i++)
	{
		if ((i % 2) == 1)
			pTable->SetBkgColour(uiNewRow, i, L_ARGB(128, 50, 50, 50));
		else
			pTable->SetBkgColour(uiNewRow, i, L_ARGB(128, 130, 130, 130));
	}

	CString str;
	str.Format("%d", pYouth->GetAge());
	pTable->SetText(uiNewRow, 1, lUIText(str));

	str.Format("%d", pYouth->GetSkills().GetOverallSkill());
	pTable->SetText(uiNewRow, 3, lUIText(str));

	str.Format("%d", pYouth->GetSkills().GetHandling());
	pTable->SetText(uiNewRow, 5, lUIText(str));

	str.Format("%d", pYouth->GetSkills().GetTackling());
	pTable->SetText(uiNewRow, 7, lUIText(str));

	str.Format("%d", pYouth->GetSkills().GetPassing());
	pTable->SetText(uiNewRow, 9, lUIText(str));

	str.Format("%d", pYouth->GetSkills().GetShooting());
	pTable->SetText(uiNewRow, 11, lUIText(str));
}

void FDYouthSquadScreen::Prepare()
{
	if ((m_bComingFromDialogBox == false) || (IsForcedUpdateRequested() == true))
	{
		SetForcedUpdateRequested(false);
		Update();

		GetMainUI()->GetYouthInfoScreen()->Hide();
		GetMainUI()->DisplaySubMessage("Select Youth"); //TODO LOCALE
	}
}

void FDYouthSquadScreen::DoShow()
{
  m_youthSquadScreen->Show();

  // force update
  if( s_lastActionSack && m_bComingFromDialogBox && g_hackLastMsgResponse )
  {
    lLogFmt("Sack occured\n");
    return;
  }


  if( m_clubTable->IsVisible() && m_clubTable->GetSelectedRow() )
  {
    m_clubTable->SelectRow( m_clubTable->GetSelectedRow() );
  }

  if( m_availableTable->IsVisible() && m_availableTable->GetSelectedRow() )
  {
    m_availableTable->SelectRow( m_availableTable->GetSelectedRow() );
  }
}

void FDYouthSquadScreen::DoHide()
{
  m_youthSquadScreen->Hide();
}

void FDYouthSquadScreen::OnClubRowSelect( FDTableWidget *, int row )
{
  // clear selection
  m_hireButton->Hide();
  m_sackButton->Hide();
  m_promoteButton->Hide();

  lLogFmt("Club Row : %d\n", row );

  // Set youth info on top screen

  if( row != -1 && m_clubYouths.Size() )
  {
    const int iYouthListIndex = m_clubTable->GetTagDataForRow(row);

    // Display possible button options for this selection
    // Sack
    // Promote to first team - set wage + contract terms

    lLogFmt("Youth list index: %d\n", iYouthListIndex );

    WorldData().SetCurrentYouth( m_clubYouths( iYouthListIndex ) );
    m_sackButton->Show();
    m_promoteButton->Show();

    GetMainUI()->HideSubMessage();
    GetMainUI()->GetYouthInfoScreen()->Show();
    GetMainUI()->GetYouthInfoScreen()->SetYouthInfo( m_clubYouths( iYouthListIndex ) );

    // On click of buttons go to correct screen
    //TODO: Return to this screen (and update this screen) after action complete
  } else
  {
    //TODO: What to do with top screen on no selection?
    WorldData().SetCurrentYouth( NULL );
    GetMainUI()->GetYouthInfoScreen()->Hide();
    GetMainUI()->DisplaySubMessage("Select Youth"); //TODO LOCALE
  }
}

void FDYouthSquadScreen::OnAvailableRowSelect( FDTableWidget *, int row )
{
  // clear selection
  m_hireButton->Hide();
  m_sackButton->Hide();
  m_promoteButton->Hide();

  lLogFmt("Available Row : %d\n", row );

  // Set youth info on top screen
  if( row != -1 && m_availableYouths.Size() )
  {
    const int iAvailableYouthListIdx = m_availableTable->GetTagDataForRow(row);
    // Display possible button options for this selection
    // Hire
    lLogFmt("Youth list index: %d\n", iAvailableYouthListIdx );

    WorldData().SetCurrentYouth( m_availableYouths( iAvailableYouthListIdx ) );
    m_hireButton->Show();

    GetMainUI()->HideSubMessage();
    GetMainUI()->GetYouthInfoScreen()->Show();
    GetMainUI()->GetYouthInfoScreen()->SetYouthInfo( m_availableYouths( iAvailableYouthListIdx ) );

    //TODO: Return to this screen (and update this screen) after action complete
  } else
  {
    //TODO: What to do with top screen on no selection?
    WorldData().SetCurrentYouth( NULL );
    GetMainUI()->GetYouthInfoScreen()->Hide();
    GetMainUI()->DisplaySubMessage("Select Youth"); //TODO LOCALE
  }
}

void FDYouthSquadScreen::OnHireButton( lUIControlWidget * )
{
  lLogFmt("Hire\n");

  m_bComingFromDialogBox = true;
  theApp.OnHireYouth();
  //Update();
  m_bComingFromDialogBox = false;

  if( m_availableTable->GetSelectedRow() != -1 )
  {
    CYouth *pYouth = m_availableYouths(m_availableTable->GetTagDataForRow(m_availableTable->GetSelectedRow()));

    m_availableTable->RemoveRows(m_availableTable->GetSelectedRow(), 1);
    m_availableTable->SelectRow( -1 );

    // Put the youth in the club table.
    m_clubYouths.Push(pYouth);
    AddYouthToTable(m_clubTable, pYouth, m_clubYouths.Size() - 1);
    m_clubTable->Resort();
  }
  GetMainUI()->GetYouthInfoScreen()->Hide();
}

void FDYouthSquadScreen::OnSackButton( lUIControlWidget * )
{
  lLogFmt("Sack\n");
  s_lastActionSack = true;
  m_bComingFromDialogBox = true;
  const bool bWasSacked = theApp.OnSackYouth();
  m_bComingFromDialogBox = false;
  //Update();

  if (bWasSacked == true)
  {
    lLogFmt("Youth was successfully sacked\n");
    if( m_clubTable->GetSelectedRow() != -1 )
    m_clubTable->RemoveRows(m_clubTable->GetSelectedRow(), 1);

    // Required because of youth wages showing up in the wages screen.
    GetMainUI()->ForceUpdateForAllScreensOnNextShow();

    GetMainUI()->GetYouthInfoScreen()->Hide();
    m_clubTable->SelectRow( -1 );
    WorldData().SetCurrentYouth( NULL );
  }
  else
	  GetMainUI()->GetYouthInfoScreen()->Show();

  s_lastActionSack = false;
}

void FDYouthSquadScreen::OnPromoteButton( lUIControlWidget * )
{
  lLogFmt("Promote\n");
  s_lastActionSack = true;
  m_bComingFromDialogBox = true;
  theApp.OnPromoteYouth();
  m_bComingFromDialogBox = false;

  if (GetMainUI()->GetNegotiateScreen()->GetLastContractOfferResponse() == ACCEPTED)
  {
    lLogFmt("Contract accepted\n");
    if( m_clubTable->GetSelectedRow() != -1 )
      m_clubTable->RemoveRows(m_clubTable->GetSelectedRow(), 1);

    // Promoted youth needs to show up in the squad screen.
    GetMainUI()->ForceUpdateForAllScreensOnNextShow();

    GetMainUI()->GetYouthInfoScreen()->Hide();
    m_clubTable->SelectRow( -1 );
    WorldData().SetCurrentYouth( NULL );
  }
  else
  {
    lLogFmt("Contract not accepted\n");
	  GetMainUI()->GetYouthInfoScreen()->Show();
  }
  s_lastActionSack = false;
}
