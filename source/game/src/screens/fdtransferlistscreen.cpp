//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDTransferListScreen.cpp
// Description : FDTransferListScreen implementation
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

#include <screens/fdtransferlistscreen.h>
#include <screens/fdnegotiatescreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdscrollbarwidget.h>
#include <fdtickerwidget.h>
#include <fdtablewidget.h>
#include <fdplayerblobwidget.h>
#include <fdfullscreenwidget.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget
#include <lemon/math/lmrand.h>

#include <playersearch.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTransferListScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDTransferListScreen::FDTransferListScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDTransferListScreen::~FDTransferListScreen()
{
}

luint32_t FDTransferListScreen::TableBkgColourCallback(FDTransferListWidget *, void *pFDTransferList, luint32_t row, luint32_t col)
{
  FDTransferListScreen *screen = (FDTransferListScreen *)pFDTransferList;

  if( row > 0 )
  {
    int numPlayers = WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList().GetSize();
    int playerIdx = row - 1;

    if( playerIdx >= numPlayers )
    {
      // search has changed possibly (can occur when switching back after editing filter)
      return 0;
    }

    CPlayer* pPlayer = WorldData().GetPlayer(WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList()[playerIdx]);

    if( pPlayer == NULL )
    {
      return 0;
    }

    switch( col )
    {
      case 0:
      case 1:
      {
        return PosnColour[pPlayer->GetSkills().GetPosition()];
      }

      case 2:
      case 4:
      {
        return L_ARGB(128,130,130,130);
      }

      case 3:
      case 5:
      {
        return L_ARGB(128,80,80,80);
      }
    }
	} else
	{
	  return L_ARGB(255, 35, 35, 35);
	}

  return 0;
}

void FDTransferListScreen::Init()
{
  m_bComingFromDialogBox = false;
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  // *** Transfer List ***

  m_transferlistScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "transferlist_view" );
  m_transferlistScreen->Hide();

  m_table = FD_GAME_NEW FDTransferListWidget( m_transferlistScreen, 0, "resulttable",
                                              //FDTransferListWidget::TABLE_FLAG_ROW_LINES|FDTransferListWidget::TABLE_FLAG_COLUMN_LINES|
                                              FDTransferListWidget::TABLE_FLAG_BORDER|FDTransferListWidget::TABLE_FLAG_ROW_SELECT|FDTransferListWidget::TABLE_FLAG_BACKGROUND|
                                              FDTransferListWidget::TABLE_FLAG_CELLBACKGROUND|FDTransferListWidget::TABLE_FLAG_FIXED_FIRSTROW |
      FDTransferListWidget::TABLE_FLAG_CAP_FIRSTROW | FDTransferListWidget::TABLE_FLAG_CUT_OFF,
                                              LUI_TOP_LEFT, lUIPoint( 20, 10 ), lUIPoint( 256 - 20 - GetDefaultScrollBarWidth(), 144 ) );

  FDScrollBarWidget *pScrollBar = FD_GAME_NEW FDScrollBarWidget(m_transferlistScreen, 0, "tablescroll", LUI_TOP_LEFT, lUIPoint(256-20, 10), lUIPoint(GetDefaultScrollBarWidth(), 146));
  pScrollBar->AlignTo( m_table );
  m_table->SetScrollBar(pScrollBar);
  m_table->SignalRowSelect.Connect( this, &FDTransferListScreen::OnRowSelect );

  m_table->SetCellBkgColourCallback(&FDTransferListScreen::TableBkgColourCallback, this);

  // Set up the table columns.
  m_table->AddColumn( lUICoord( 5 ), LUI_CENTER_CENTER );// gap
  m_table->AddColumn( lUICoord( 96 ), LUI_LEFT_TOP );
  m_table->AddColumn( lUICoord( 10 ), LUI_CENTER_CENTER );// gap
  m_table->AddColumn( lUICoord( 30 ), LUI_CENTER_CENTER );
  m_table->AddColumn( lUICoord( 10 ), LUI_CENTER_CENTER ); //gap
  m_table->AddColumn( lUICoord( 88 ), LUI_CENTER_CENTER );
  m_table->AddRows(1);

  //m_table->SetText( 0, 1, lUIText( IDS_NAME ) );
  //m_table->SetText( 0, 3, lUIText( IDS_SKILL_SINGLE ) );
  //m_table->SetText( 0, 5, lUIText( IDS_CLUBVALUATION ) );

  m_table->SetSortWhenColumnHeaderClicked(true);
  m_table->SetSortFunctionForColumn(1, &FDTransferListWidget::GenericFirstInitialLastNameSort);
  m_table->SetSortFunctionForColumn(3, &FDTransferListWidget::GenericNumericSort);
  m_table->SetSortFunctionForColumn(5, &FDTransferListWidget::GenericMoneySort);
  m_table->ClearTiebreakerColumns();
  m_table->AddTiebreakerColumn(1);

  // Set up the buttons for the screen.
  m_editButton = FD_GAME_NEW FDButtonWidget( m_transferlistScreen, 0, "tleditfilter", FDResourceManager::GetSprite("uiicon","editfilter"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPointPercent( 37, 85 ) );
  m_editButton->SignalControlCommand.Connect( this, &FDTransferListScreen::OnEditButton );

  m_loanButton = FD_GAME_NEW FDButtonWidget( m_transferlistScreen, 0, "tlloan", FDResourceManager::GetSprite("uiicon","loanplayer"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPointPercent( 15, 85 ) );
  m_loanButton->SignalControlCommand.Connect( this, &FDTransferListScreen::OnLoanButton );

  m_buyButton  = FD_GAME_NEW FDButtonWidget( m_transferlistScreen, 0, "tlbuy", FDResourceManager::GetSprite("uiicon","buyplayer"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPointPercent( 26, 85 ) );
  m_buyButton->SignalControlCommand.Connect( this, &FDTransferListScreen::OnBuyButton );

  // filter combo
  m_filterSelect = FD_GAME_NEW FDComboWidget( m_transferlistScreen, 0, "tlfilter_combo", lUIText(""), LUI_TOP_RIGHT, lUIPointPercent( 89, 85 ) );
  m_filterSelect->SetAllToUpper( true );

  m_filterSelect->AddItem( IDS_FILTER_1 );
  m_filterSelect->AddItem( IDS_FILTER_2 );
  m_filterSelect->AddItem( IDS_FILTER_3 );
  m_filterSelect->AddItem( IDS_FILTER_4 );

  m_filterSelect->SignalSelection.Connect( this, &FDTransferListScreen::OnFilterSelect );
}

void FDTransferListScreen::Prepare()
{
  CClub *pUserClub = WorldData().GetCurrentUserClub();
  if (pUserClub != NULL)
  {
	  if (pUserClub->AreBuyTransfersAllowed())
		  m_buyButton->Enable();
	  else
		  m_buyButton->Disable();
	  if (pUserClub->AreLoanTransfersAllowed())
		  m_loanButton->Enable();
	  else
		  m_loanButton->Disable();
  }
  else
  {
	  m_buyButton->Disable();
	  m_loanButton->Disable();
  }

  if (m_bComingFromDialogBox == false)
  {
    Search( GetMainUI()->GetTransferFilter( m_filterSelect->GetSelectedItem() ) );
    WorldData().SetCurrentPlayer( NULL );
    GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo(NULL);
  }
  else
  {
	  // Deselect things just in case the player is no longer available.
	  m_table->SelectRow(-1);
  }

  GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE
}

void FDTransferListScreen::DoShow()
{
  m_transferlistScreen->Show();
}

void FDTransferListScreen::DoHide()
{
  m_transferlistScreen->Hide();
  GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo(NULL);
}

void FDTransferListScreen::SerializeState( FDStateStorage &state )
{
  if( state.IsSaving() )
  {
    int filter   = m_filterSelect->GetSelectedItem();
    int tableidx = m_table->GetSelectedRow();
    int scroll = m_table->GetScrollBar()->GetCurrentPosition();

    state << filter;
    state << tableidx;
    state << scroll;

    lLogFmt("Saving filter %d tableidx %d\n", filter, tableidx);
  } else
  {
    WorldData().SetCurrentPlayer( NULL );

    int filter;
    int tableidx;
    int scroll;

    state << filter;
    state << tableidx;
    state << scroll;

    lLogFmt("Loading filter %d tableidx %d\n", filter, tableidx);

    m_filterSelect->SetSelectedItem( filter, false );

    if( tableidx < m_table->GetRowCount() && scroll < m_table->GetRowCount() )
    {
      m_table->SelectRow( tableidx );
      m_table->GetScrollBar()->SetCurrentPosition( scroll, true );
    } else
    {
      m_table->SelectRow( 0 );
    }
  }
}


void FDTransferListScreen::OnRowSelect( FDTransferListWidget *pTable, int row )
{
  lLogFmt("Row %d selected\n", row);
  int idx = row - 1;
  if( idx < 0 || idx >= WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList().GetSize() )
  {
    lLogFmt("Not selecting row\n");
    WorldData().SetCurrentPlayer(NULL);
    GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo(NULL);
    GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE
    return;
  }

  lLogFmt("Displaying player %d\n", idx);

  GetMainUI()->HideSubMessage();

  const int iPlayerID = (int)pTable->GetTagDataForRow(row);
  CPlayer* pPlayer = WorldData().GetPlayer(iPlayerID);
  WorldData().SetCurrentPlayer( pPlayer );
  WorldData().SetCurrentClub( &pPlayer->GetClub() );

  GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( pPlayer );

  // update button status
  m_loanButton->Disable();
  m_buyButton->Disable();

  // not owned by us
  if( WorldData().GetCurrentUserClub()->AreBuyTransfersAllowed() == false )
  {
    if( WorldData().GetCurrentUserClub()->AreLoanTransfersAllowed() == false )
    {
      // no purchase options
    }
    else
    {
      // not owned by us - loan available
      if( !WorldData().GetTransferListManager().IsOutOfContract(WorldData().GetCurrentPlayer()->DoFindID()) )
        m_loanButton->Enable();
    }
  }
  else
  {
    // not owned by us - can be bought / loaned
    m_buyButton->Enable();
    if( !WorldData().GetTransferListManager().IsOutOfContract(WorldData().GetCurrentPlayer()->DoFindID()) )
      m_loanButton->Enable();
  }
}


void FDTransferListScreen::OnEditButton( lUIControlWidget * )
{
  // modal spawn edit filter mode
  lLogFmt("Edit filter\n");

  GetMainUI()->GetTransferListEditScreen()->SetTransferFilterNumber(m_filterSelect->GetSelectedItem());
  GetMainUI()->SwitchToScreenCombo(SCREENCOMBO_TRANSFERLIST_EDIT, false);
}

void FDTransferListScreen::OnLoanButton( lUIControlWidget *button )
{
  if( m_loanButton->IsDisabled() )
    return;

  int selected = m_table->GetSelectedRow();
  int scroll = m_table->GetScrollBar()->GetCurrentPosition();

  lLogFmt("Loan\n");
  if( WorldData().GetCurrentPlayer() != NULL )
  {
    m_bComingFromDialogBox = true;
    GetMainUI()->GetNegotiateScreen()->ResetLastContractOfferResponse();

    const int iTableRow = m_table->FindTagData(WorldData().GetCurrentPlayer()->DoFindID(), true);
    // Out of contract players needn't be loaned?
    // Loaned players can't be loaned while on loan?
    if( !WorldData().GetTransferListManager().IsOutOfContract(WorldData().GetCurrentPlayer()->DoFindID()) &&
        !WorldData().GetCurrentPlayer()->IsOnLoan() )
    {
      theApp.OnApproachClubLoanInPlayer();
    }

    m_bComingFromDialogBox = false;

    if (GetMainUI()->GetNegotiateScreen()->GetLastContractOfferResponse() == ACCEPTED)
    {
      const int scroll = m_table->GetScrollBar()->GetCurrentPosition();
      Search( GetMainUI()->GetTransferFilter( m_filterSelect->GetSelectedItem() ) );
      m_table->SelectRow(-1);
      m_table->GetScrollBar()->SetCurrentPosition(scroll, true);
    } else
    {
      m_table->SelectRow( selected );
      m_table->GetScrollBar()->SetCurrentPosition( scroll, true );
    }
  }
}

void FDTransferListScreen::OnBuyButton( lUIControlWidget * )
{
  if( m_buyButton->IsDisabled() )
    return;

  int selected = m_table->GetSelectedRow();
  int scroll = m_table->GetScrollBar()->GetCurrentPosition();

  lLogFmt("Buy\n");
  if( WorldData().GetCurrentPlayer() != NULL )
  {
    GetMainUI()->GetNegotiateScreen()->ResetLastContractOfferResponse();

    m_bComingFromDialogBox = true;
    const int iTableRow = m_table->FindTagData(WorldData().GetCurrentPlayer()->DoFindID(), true);

    if( !WorldData().GetTransferListManager().IsOutOfContract(WorldData().GetCurrentPlayer()->DoFindID()) )
    {
      theApp.OnMakeOfferForPlayer();
    }
    else
    {  int scroll = m_table->GetScrollBar()->GetCurrentPosition();
      theApp.OnNegotiateOutOfContractPlayerContract();
    }

    m_bComingFromDialogBox = false;

    if (GetMainUI()->GetNegotiateScreen()->GetLastContractOfferResponse() == ACCEPTED)
    {
	  const int scroll = m_table->GetScrollBar()->GetCurrentPosition();
	  Search( GetMainUI()->GetTransferFilter( m_filterSelect->GetSelectedItem() ) );
	  m_table->SelectRow(-1);
	  m_table->GetScrollBar()->SetCurrentPosition(scroll, true);
    }
    else
    {
      m_table->SelectRow( selected );
      m_table->GetScrollBar()->SetCurrentPosition( scroll, true );
    }
  }

}

void FDTransferListScreen::OnFilterSelect( FDComboWidget * widget, int selection )
{
  int number = selection;
  if( number < 0 || number > 4 )
    number = 0;

  Search( GetMainUI()->GetTransferFilter( number) );
  m_table->SelectRow(-1);
}

void FDTransferListScreen::Search( FDTransferFilter &filter)
{
  // do search and update table at the top
  CPlayerSearch &pPlayerSearch = WorldData().GetCurrentUser()->GetPlayerSearch();

  pPlayerSearch.SetDefaults();

  lLogFmt("Search\n");

  pPlayerSearch.SetMinPrice( filter.minPrice );
  pPlayerSearch.SetMaxPrice( filter.maxPrice );

  lLogFmt("Cost %.2f -> %.2f\n", (float) filter.minPrice, (float) filter.maxPrice);

  pPlayerSearch.SetMinAge( filter.minAge);
  pPlayerSearch.SetMaxAge( filter.maxAge );

  lLogFmt("Age %.2f -> %.2f\n", (float) filter.minAge, (float) filter.maxAge );

  pPlayerSearch.SetMinSkill( filter.minSkill );
  pPlayerSearch.SetMaxSkill( filter.maxSkill );

  lLogFmt("Skill %.2f -> %.2f\n", (float) filter.minSkill, (float) filter.maxSkill);

  switch( filter.positionType )
  {
    case 0:
      lLogFmt("Goalkeeper\n");
      pPlayerSearch.SetFindPosition( GOALKEEPER );
      break;
    case 1:
      lLogFmt("Defender\n");
      pPlayerSearch.SetFindPosition( DEFENDER );
      break;
    case 2:
      lLogFmt("Midfielder\n");
      pPlayerSearch.SetFindPosition( MIDFIELDER );
      break;
    case 3:
      lLogFmt("Attacker\n");
      pPlayerSearch.SetFindPosition( ATTACKER );
      break;
    default:
      lLogFmt("Any\n");
      pPlayerSearch.SetFindPosition( MAXPLAYERPOSITIONS );
      break;
  }

  pPlayerSearch.SetIncludeFlags(filter.flags);

  // get data
  pPlayerSearch.DoEmpty();
  //m_DivisionSelector.GetCurrentDivision().GetAllTransferListed( WorldData().GetCurrentUserClub(), WorldData().GetCurrentUser()->GetPlayerSearch() );
  ///WorldData().GetCurrentUserClub()->GetDivision().GetAllTransferListed( WorldData().GetCurrentUserClub(), pPlayerSearch );

  // search whole country
  //TODO: Is this right?
  WorldData().GetCurrentUserClub()->GetActiveCountry().GetAllTransferListed( WorldData().GetCurrentUserClub(), pPlayerSearch );

  if( pPlayerSearch.GetIncludeFlag(CPlayerSearch::INCLUDEOUTOFCONTRACT) == true )
	{
    WorldData().GetOutOfContractPlayerList().DoBuildSuitableList( pPlayerSearch );
	}

  // Set List
  if (m_table->GetRowCount() > 1)
  {
    m_table->RemoveRows(1, m_table->GetRowCount() - 1);
  }

  int numPlayers = WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList().GetSize();
  lLogFmt("Num Search: %d\n", numPlayers);

  if( numPlayers > 1000 )
  {
    //NOTE: HACK: Cap to 1000 just in case
    numPlayers = 1000;
  }

  m_table->AddRows( numPlayers );

  CString str;
  //str.Format("%d results", numPlayers);

  //m_previewInfoText->SetText( lUIText( str ) );

  for (int LoopCount = 0; LoopCount < numPlayers; LoopCount++)
	{
    CPlayer* pPlayer = WorldData().GetPlayer(WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList()[LoopCount]);

    // Player name
    //m_table->SetText( LoopCount + 1, 1, lUIText( FDMakePlayerNameLink(pPlayer) ) );
    m_table->SetTagDataForRow(LoopCount + 1, pPlayer->DoFindID());

    // Skill
    //str.Format("%d", pPlayer->GetSkills().GetOverallSkill());
    //m_table->SetText( LoopCount + 1, 3, lUIText( str ) );

    // Value
    //m_table->SetText( LoopCount + 1, 5, lUIText( WorldData().GetCurrentUserClub()->GetForeignStr(
    //                                                  WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerValueList()[LoopCount] ) ) );
	}

  m_table->Resort();


  m_table->SelectRow( -1 );
  m_table->GetScrollBar()->SetCurrentPosition( 0, true );
}

// player selected
//        WorldData().SetCurrentClub(WorldData().GetCurrentUserClub());
//        CPlayer* pPlayer = WorldData().GetPlayer(WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList()[iColumn]);
//        CPopupPlayerMenu TheMenu;
//        TheMenu.DoModal(pPlayer, _Point);
//        DoBuildPlayerList();
//        m_ClubBalanceText.DoUpdate();
