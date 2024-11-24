//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDTransferlistscreen.cpp
// Description : FDTransferScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>
#include <fdbackingwidget.h>

#include <screens/fdtransferlistscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

#include <playersearch.h>

using namespace L_NAMESPACE_NAME;

static int MinAge[] = { MINIMUPLAYERAGE, 22, 31 };
static int MaxAge[] = { 21, 30, MAXIMUMPLAYERAGE };
static const int MinSkill [] = { MINPLAYSKILL, 30, 60 };
static const int MaxSkill [] = { 40, 70, MAXPLAYSKILL };
static const int MinPrice[] = { 0, 3, 5, 7 };
static const int MaxPrice[] = { 3, 5, 7, NUMMONEYSTEPS - 1 };

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTransferListEditScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDTransferListEditScreen::FDTransferListEditScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDTransferListEditScreen::~FDTransferListEditScreen()
{
}

void FDTransferListEditScreen::OnCheckBox( lUIControlWidget * )
{
  Search();
}

void FDTransferListEditScreen::OnCombo(FDComboWidget *, int)
{
  Search();
}

void FDTransferListEditScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  // edit filter for transfer list
  m_transfereditScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "transferedit_view" );
  m_transfereditScreen->Hide();

  FDBackingWidget * backing1 = FD_GAME_NEW FDBackingWidget( m_transfereditScreen, 0, "back", L_ARGB( 127, 0, 0, 0 ), LUI_LEFT_TOP, lUIPoint(20, 3 ), lUIPoint( 236, 116 ) );
  FDBackingWidget * backing2 = FD_GAME_NEW FDBackingWidget( m_transfereditScreen, 0, "back", L_ARGB( 127, 0, 0, 0 ), LUI_LEFT_TOP, lUIPoint(20, 121  ), lUIPoint( 236, 40 ) );


  lUISpriteWidget * spriteBar[10];

  for( int i = 0; i < 4; i++ )
  {
    spriteBar[i] = FD_GAME_NEW lUISpriteWidget( m_transfereditScreen, 0, "bars", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT, lUIPoint(20, 5 + i * 29 ), lUIPoint( 236, 12 ));
  }

  for( int i = 0; i < 2; i++ )
  {
    spriteBar[4+i] = FD_GAME_NEW lUISpriteWidget( m_transfereditScreen, 0, "bars", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_LEFT, lUIPoint(138, 5 + i * 29 ), lUIPoint( 100, 12 ));
  }

  for( int i = 0; i < 2; i++ )
  {
    spriteBar[ 6 + i * 2] = FD_GAME_NEW lUISpriteWidget( m_transfereditScreen, 0, "bars", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_LEFT, lUIPoint(116, 5 + (i+2) * 29 ), lUIPoint( 18, 12 ));
    spriteBar[ 6 + i * 2 + 1 ] = FD_GAME_NEW lUISpriteWidget( m_transfereditScreen, 0, "bars", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_LEFT, lUIPoint(234, 5 + (i+2) * 29 ), lUIPoint( 18, 12 ));
  }

  m_editfilterMinCost  = FD_GAME_NEW FDSliderWidget( m_transfereditScreen, 0, "tle_min_cost_slider", FDSliderWidget::SLIDER_FLAG_POUND, lUIText(IDS_MINVALUE), LUI_TOP_CENTER, lUIPointPercent( 58, 10 ), lUIPointPercent(60, 13) );
  m_editfilterMinCost->SetValueRange( (float)MONEYSTEPS[0], (float)MONEYSTEPS[NUMMONEYSTEPS-1] );
  m_editfilterMinCost->SetValuePct( 0.0f );
  m_editfilterMinCost->SetTextToUpper();
  m_editfilterMinCost->SetSliderOffset( 20 );

  m_editfilterMaxCost  = FD_GAME_NEW FDSliderWidget( m_transfereditScreen, 0, "tle_max_cost_slider", FDSliderWidget::SLIDER_FLAG_POUND, lUIText(IDS_MAXVALUE), LUI_TOP_CENTER, lUIPointPercent( 58, 25 ), lUIPointPercent(60, 13) );
  m_editfilterMaxCost->SetValueRange( (float)MONEYSTEPS[0], (float)MONEYSTEPS[NUMMONEYSTEPS-1] );
  m_editfilterMaxCost->SetValuePct( 1.0f );
  m_editfilterMaxCost->SetTextToUpper();
  m_editfilterMaxCost->SetSliderOffset( 20 );

  m_editfilterMinAge   = FD_GAME_NEW FDSliderWidget( m_transfereditScreen, 0, "tle_min_age_slider", 0, lUIText(IDS_MINAGE), LUI_TOP_LEFT, lUIPointPercent( 12, 40 ) );
  m_editfilterMinAge->SetValueRange( (float)MINIMUPLAYERAGE, (float)MAXIMUMPLAYERAGE );
  m_editfilterMinAge->SetValuePct( 0.0f );
  m_editfilterMinAge->SetTextToUpper();

  m_editfilterMaxAge   = FD_GAME_NEW FDSliderWidget( m_transfereditScreen, 0, "tle_max_age_slider", 0, lUIText(IDS_MAXAGE), LUI_TOP_LEFT, lUIPointPercent( 58, 40 ) );
  m_editfilterMaxAge->SetValueRange( (float)MINIMUPLAYERAGE, (float)MAXIMUMPLAYERAGE );
  m_editfilterMaxAge->SetValuePct( 1.0f );
  m_editfilterMaxAge->SetTextToUpper();

  m_editfilterMinSkill = FD_GAME_NEW FDSliderWidget( m_transfereditScreen, 0, "tle_min_skill_slider", 0, lUIText(IDS_MINSKILL), LUI_TOP_LEFT, lUIPointPercent( 12, 55 ) );
  m_editfilterMinSkill->SetValueRange( (float)MINPLAYSKILL, (float)MAXPLAYSKILL );
  m_editfilterMinSkill->SetValuePct( 0.0f );
  m_editfilterMinSkill->SetTextToUpper();

  m_editfilterMaxSkill = FD_GAME_NEW FDSliderWidget( m_transfereditScreen, 0, "tle_max_skill_slider", 0, lUIText(IDS_MAXSKILL), LUI_TOP_LEFT, lUIPointPercent( 58, 55 ) );
  m_editfilterMaxSkill->SetValueRange( (float)MINPLAYSKILL, (float)MAXPLAYSKILL );
  m_editfilterMaxSkill->SetValuePct( 1.0f );
  m_editfilterMaxSkill->SetTextToUpper();

  m_editfilterMinCost->SignalSliderChange.Connect( this, &FDTransferListEditScreen::OnSlider );
  m_editfilterMaxCost->SignalSliderChange.Connect( this, &FDTransferListEditScreen::OnSlider );
  m_editfilterMinAge->SignalSliderChange.Connect( this, &FDTransferListEditScreen::OnSlider );
  m_editfilterMaxAge->SignalSliderChange.Connect( this, &FDTransferListEditScreen::OnSlider );
  m_editfilterMinSkill->SignalSliderChange.Connect( this, &FDTransferListEditScreen::OnSlider );
  m_editfilterMaxSkill->SignalSliderChange.Connect( this, &FDTransferListEditScreen::OnSlider );

  // check boxes

  // Listed players checkbox
  m_editfilterListedPlayers = FD_GAME_NEW FDCheckBoxWidget( m_transfereditScreen, 0, "listed_players",
                                                                      FDResourceManager::GetSprite("uiicon", "tick_0"),
                                                                      FDResourceManager::GetSprite("uiicon", "tick_1"),
                                                                      lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,
                                                                      LUI_TOP_LEFT,
                                                                      lUIPointPercent( 60, 62 ) );
  m_editfilterListedPlayers->SetText( lUIText( IDS_INC_LISTED ) );
  m_editfilterListedPlayers->SetChecked( true );
  m_editfilterListedPlayers->SignalControlCommand.Connect( this, &FDTransferListEditScreen::OnCheckBox );


  // Injured players checkbox
  m_editfilterInjuredPlayers = FD_GAME_NEW FDCheckBoxWidget( m_transfereditScreen, 0, "injured_players",
                                                                      FDResourceManager::GetSprite("uiicon", "tick_0"),
                                                                      FDResourceManager::GetSprite("uiicon", "tick_1"),
                                                                      lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,
                                                                      LUI_TOP_LEFT,
                                                                      lUIPointPercent( 10, 62 ) );
  m_editfilterInjuredPlayers->SetText( lUIText( IDS_INC_INJURED ) );
  m_editfilterInjuredPlayers->SetChecked(false);
  m_editfilterInjuredPlayers->SignalControlCommand.Connect( this, &FDTransferListEditScreen::OnCheckBox );

  // non listed players
  m_editfilterNonListedPlayers = FD_GAME_NEW FDCheckBoxWidget( m_transfereditScreen, 0, "nonlisted_players",
                                                                      FDResourceManager::GetSprite("uiicon", "tick_0"),
                                                                      FDResourceManager::GetSprite("uiicon", "tick_1"),
                                                                      lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,
                                                                      LUI_TOP_LEFT,
                                                                      lUIPointPercent( 60, 69 ) );
  m_editfilterNonListedPlayers->SetText( lUIText( IDS_INC_NONLISTED ) );
  m_editfilterNonListedPlayers->SignalControlCommand.Connect( this, &FDTransferListEditScreen::OnCheckBox );

  // suspended players
  m_editfilterSuspendedPlayers = FD_GAME_NEW FDCheckBoxWidget( m_transfereditScreen, 0, "suspended_players",
                                                                      FDResourceManager::GetSprite("uiicon", "tick_0"),
                                                                      FDResourceManager::GetSprite("uiicon", "tick_1"),
                                                                      lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,
                                                                      LUI_TOP_LEFT,
                                                                      lUIPointPercent( 10, 69 ) );
  m_editfilterSuspendedPlayers->SetText( lUIText( IDS_INC_SUSPENDED ) );
  m_editfilterSuspendedPlayers->SignalControlCommand.Connect( this, &FDTransferListEditScreen::OnCheckBox );

  // loan listed players
  m_editfilterLoanListedPlayers = FD_GAME_NEW FDCheckBoxWidget( m_transfereditScreen, 0, "loanlisted_players",
                                                                      FDResourceManager::GetSprite("uiicon", "tick_0"),
                                                                      FDResourceManager::GetSprite("uiicon", "tick_1"),
                                                                      lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,
                                                                      LUI_TOP_LEFT,
                                                                      lUIPointPercent( 60, 76 ) );
  m_editfilterLoanListedPlayers->SetText( lUIText( IDS_INC_LOANLISTED ) );
  m_editfilterLoanListedPlayers->SetChecked( true );
  m_editfilterLoanListedPlayers->SignalControlCommand.Connect( this, &FDTransferListEditScreen::OnCheckBox );

  // out of contract players
  m_editfilterOutOfContractPlayers = FD_GAME_NEW FDCheckBoxWidget( m_transfereditScreen, 0, "outofcontract_players",
                                                                      FDResourceManager::GetSprite("uiicon", "tick_0"),
                                                                      FDResourceManager::GetSprite("uiicon", "tick_1"),
                                                                      lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK,
                                                                      LUI_TOP_LEFT,
                                                                      lUIPointPercent( 10, 76 ) );
  m_editfilterOutOfContractPlayers->SetText( lUIText( IDS_INC_OUTOFCONTRACT ) );
  m_editfilterOutOfContractPlayers->SignalControlCommand.Connect( this, &FDTransferListEditScreen::OnCheckBox );

  // position combo
  m_editfilterPosition = FD_GAME_NEW FDComboWidget( m_transfereditScreen, 0, "tle_position_combo", lUIText(""), LUI_TOP_LEFT, lUIPointPercent( 50, 85 ) );
  m_editfilterPosition->SetAllToUpper( true );

  m_editfilterPosition->AddItem( IDS_GOALKEEPER );
  m_editfilterPosition->AddItem( IDS_DEFENDER );
  m_editfilterPosition->AddItem( IDS_MIDFIELDER );
  m_editfilterPosition->AddItem( IDS_ATTACKER );
  m_editfilterPosition->AddItem( IDS_ANY );
  m_editfilterPosition->SignalSelection.Connect( this, &FDTransferListEditScreen::OnCombo );

  // Search + Select filter buttons
  //FDButtonWidget * selectButton = FD_GAME_NEW FDButtonWidget( m_transfereditScreen, 0, "select", FDResourceManager::GetSprite("uiicon","search"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPointPercent( 15, 85 ) );
  FDButtonWidget * searchButton = FD_GAME_NEW FDButtonWidget( m_transfereditScreen, 0, "tle_search", FDResourceManager::GetSprite("ui","button45"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_CENTER, lUIPointPercent( 27, 85 ) );
  searchButton->SignalControlCommand.Connect( this, &FDTransferListEditScreen::OnSearchButton );

  // TODO LOCALE
  searchButton->SetText("SAVE");

  // preview on sub screen
  m_transfereditScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "transferedit_view" );
  m_transfereditScreenSub->Hide();

  m_previewtable = FD_GAME_NEW FDTableWidget( m_transfereditScreenSub, 0, "resulttable",
                                              //FDTableWidget::TABLE_FLAG_ROW_LINES|FDTableWidget::TABLE_FLAG_COLUMN_LINES|
                                              FDTableWidget::TABLE_FLAG_BORDER|FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW |
                                              FDTableWidget::TABLE_FLAG_CELLBACKGROUND |FDTableWidget::TABLE_FLAG_BACKGROUND|
			FDTableWidget::TABLE_FLAG_CAP_FIRSTROW | FDTableWidget::TABLE_FLAG_CUT_OFF,
                                              LUI_TOP_LEFT, lUIPoint( 0, 65 ), lUIPoint( 255, 125 ) );

  FDBackingWidget * backing3 = FD_GAME_NEW FDBackingWidget( m_transfereditScreenSub, 0, "back", L_ARGB( 127, 0, 0, 0 ), LUI_LEFT_TOP, lUIPoint( 0, 43 ), lUIPoint( 256, 18 ) );

  m_previewInfoText = FD_GAME_NEW lUITextWidget( m_transfereditScreenSub, 0, "prevtext", "", LUI_CENTER_BOTTOM, lUIPoint( 128, 60 ) );
  m_previewInfoText->SetFont( FDResourceManager::GetFont("font16", "font16") );
  m_previewInfoText->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );
}

void FDTransferListEditScreen::OnSearchButton( lUIControlWidget * )
{
  // set filter
  m_filter.minPrice = m_editfilterMinCost->GetValue();
  m_filter.maxPrice = m_editfilterMaxCost->GetValue();

  m_filter.minAge = m_editfilterMinAge->GetValue();
  m_filter.maxAge = m_editfilterMaxAge->GetValue();

  m_filter.minSkill = m_editfilterMinSkill->GetValue();
  m_filter.maxSkill = m_editfilterMaxSkill->GetValue();

  m_filter.positionType = m_editfilterPosition->GetSelectedItem();

  byte Flags = 0;

  if( m_editfilterListedPlayers->IsChecked() )
  {
    lLogFmt("Inc TransferListed\n");
    Flags |= CPlayerSearch::INCLUDEONTRANSFERLIST;
  }

  if( m_editfilterLoanListedPlayers->IsChecked() )
  {
    lLogFmt("Inc LoanListed\n");
    Flags |= CPlayerSearch::INCLUDEONLOANLIST;
  }

  if( m_editfilterNonListedPlayers->IsChecked() )
  {
    lLogFmt("Inc NonListed\n");
    Flags |= CPlayerSearch::INCLUDENONLISTED;
  }

  if( m_editfilterInjuredPlayers->IsChecked() )
  {
    lLogFmt("Inc Injured\n");
    Flags |= CPlayerSearch::INCLUDEINJURED;
  }

  if( m_editfilterOutOfContractPlayers->IsChecked() )
  {
    lLogFmt("Inc OutOfContract\n");
    Flags |= CPlayerSearch::INCLUDEOUTOFCONTRACT;
  }

  if( m_editfilterSuspendedPlayers->IsChecked() )
  {
    lLogFmt("Inc Suspended\n");
    Flags |= CPlayerSearch::INCLUDESUSPENDED;
  }

  m_filter.flags = Flags;

  GetMainUI()->SetTransferFilter(m_transferFilterNumber, m_filter);

  // Save
  GetMainUI()->OptionsSave();
  GetMainUI()->PopScreen();
}

void FDTransferListEditScreen::Search()
{
  // do search and update table at the top
  CPlayerSearch &pPlayerSearch = WorldData().GetCurrentUser()->GetPlayerSearch();

  pPlayerSearch.SetDefaults();

  lLogFmt("Search\n");

  //pPlayerSearch->SetOutOfContract(bool _Value);
////  pPlayerSearch->SetExchangeRate(float _Value);

  //pPlayerSearch.SetMinPriceStep(MinPrice[m_Value]);
  //pPlayerSearch.SetMaxPriceStep(MaxPrice[m_Value]);

  pPlayerSearch.SetMinPrice( m_editfilterMinCost->GetValue() );
  pPlayerSearch.SetMaxPrice( m_editfilterMaxCost->GetValue() );

  lLogFmt("Cost %.2f -> %.2f\n", m_editfilterMinCost->GetValue(), m_editfilterMaxCost->GetValue());

  pPlayerSearch.SetMinAge( m_editfilterMinAge->GetValue() );
  pPlayerSearch.SetMaxAge( m_editfilterMaxAge->GetValue() );

  lLogFmt("Age %.2f -> %.2f\n", m_editfilterMinAge->GetValue(), m_editfilterMaxAge->GetValue());

  pPlayerSearch.SetMinSkill( m_editfilterMinSkill->GetValue() );
  pPlayerSearch.SetMaxSkill( m_editfilterMaxSkill->GetValue() );

  lLogFmt("Skill %.2f -> %.2f\n", m_editfilterMinSkill->GetValue(), m_editfilterMaxSkill->GetValue());

  switch( m_editfilterPosition->GetSelectedItem() )
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
  byte Flags = 0;

  if( m_editfilterListedPlayers->IsChecked() )
  {
    lLogFmt("Inc TransferListed\n");
    Flags |= CPlayerSearch::INCLUDEONTRANSFERLIST;
  }

  if( m_editfilterLoanListedPlayers->IsChecked() )
  {
    lLogFmt("Inc LoanListed\n");
    Flags |= CPlayerSearch::INCLUDEONLOANLIST;
  }

  if( m_editfilterNonListedPlayers->IsChecked() )
  {
    lLogFmt("Inc NonListed\n");
    Flags |= CPlayerSearch::INCLUDENONLISTED;
  }

  if( m_editfilterInjuredPlayers->IsChecked() )
  {
    lLogFmt("Inc Injured\n");
    Flags |= CPlayerSearch::INCLUDEINJURED;
  }

  if( m_editfilterOutOfContractPlayers->IsChecked() )
  {
    lLogFmt("Inc OutOfContract\n");
    Flags |= CPlayerSearch::INCLUDEOUTOFCONTRACT;
  }

  if( m_editfilterSuspendedPlayers->IsChecked() )
  {
    lLogFmt("Inc Suspended\n");
    Flags |= CPlayerSearch::INCLUDESUSPENDED;
  }
  pPlayerSearch.SetIncludeFlags(Flags);

  // get data
  pPlayerSearch.DoEmpty();
  //m_DivisionSelector.GetCurrentDivision().GetAllTransferListed( WorldData().GetCurrentUserClub(), WorldData().GetCurrentUser()->GetPlayerSearch() );
  //WorldData().GetCurrentUserClub()->GetDivision().GetAllTransferListed( WorldData().GetCurrentUserClub(), pPlayerSearch );

  WorldData().GetCurrentUserClub()->GetActiveCountry().GetAllTransferListed( WorldData().GetCurrentUserClub(), pPlayerSearch );

  if( pPlayerSearch.GetIncludeFlag(CPlayerSearch::INCLUDEOUTOFCONTRACT) == true )
	{
    WorldData().GetOutOfContractPlayerList().DoBuildSuitableList( pPlayerSearch );
	}

  // Set List
  m_previewtable->ClearAll();
  m_previewtable->AddColumn( lUICoord( 5 ), LUI_LEFT_CENTER );// gap
  m_previewtable->AddColumn( lUICoord( 100 ), LUI_LEFT_CENTER );
  m_previewtable->AddColumn( lUICoord( 10 ), LUI_LEFT_CENTER ); //gap
  m_previewtable->AddColumn( lUICoord( 65 ), LUI_CENTER_CENTER );
  m_previewtable->AddColumn( lUICoord( 10 ), LUI_CENTER_CENTER ); // gap
  m_previewtable->AddColumn( lUICoord( 65 ), LUI_CENTER_CENTER );

  int numPlayers = WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList().GetSize();
  int maxPlayers = WorldData().GetCurrentUser()->GetPlayerSearch().GetMaximumPlayers();

  lLogFmt("Num Search: %d\n", numPlayers);

  // show number of players
  CString str;
  str.Format( "Filter %d - %d Results %s", m_transferFilterNumber+1, numPlayers, (numPlayers == maxPlayers) ? "(CAPPED)" : "" ); //TODO Change - IDS_NUM_SEARCH_RESULTS
  m_previewInfoText->SetText( lUIText( str ) );

  // cap as we can't scroll anyway
  if( numPlayers > 30 )
    numPlayers = 30;

  m_previewtable->AddRows( numPlayers + 1 );

  m_previewtable->SetText( 0, 1, lUIText( IDS_NAME ) );
  m_previewtable->SetText( 0, 3, lUIText( IDS_SKILL_SINGLE ) );
  m_previewtable->SetText( 0, 5, lUIText( IDS_CLUBVALUATION ) );

  m_previewtable->SetRowBkgColour( 0, L_ARGB(255, 35, 35, 35) );


  // display players in list
  for (int LoopCount = 0; LoopCount < numPlayers; LoopCount++)
	{
    CPlayer* pPlayer = WorldData().GetPlayer(WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerList()[LoopCount]);

//TODO    m_Grid.SetRowPointerDetails(LoopCount, pPlayer, PLAYERCONTRACT);
//
    m_previewtable->SetBkgColour( LoopCount + 1, 0, PosnColour[pPlayer->GetSkills().GetPosition()] );
    m_previewtable->SetBkgColour( LoopCount + 1, 1, PosnColour[pPlayer->GetSkills().GetPosition()] );
    m_previewtable->SetBkgColour( LoopCount + 1, 2, L_ARGB(128,130,130,130) );
    m_previewtable->SetBkgColour( LoopCount + 1, 3, L_ARGB(128,80,80,80) );
    m_previewtable->SetBkgColour( LoopCount + 1, 4, L_ARGB(128,130,130,130) );
    m_previewtable->SetBkgColour( LoopCount + 1, 5, L_ARGB(128,80,80,80) );

    m_previewtable->SetText( LoopCount + 1, 1, lUIText( pPlayer->GetName() ) );
    //m_previewtable->SetTextColour( LoopCount + 1, 0, PosnColour[pPlayer->GetSkills().GetPosition()]);

    str.Format("%d", pPlayer->GetSkills().GetOverallSkill());
    m_previewtable->SetText( LoopCount + 1, 3, lUIText( str ) );
    //m_previewtable->SetTextColour( LoopCount + 1, 1, PosnColour[pPlayer->GetSkills().GetPosition()]);

    m_previewtable->SetText( LoopCount + 1, 5,
                             lUIText( WorldData().GetCurrentUserClub()->GetForeignStr(
                                                      WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerValueList()[LoopCount] ) ) );


//    lLogFmt("%d: %s : %d : %s\n", LoopCount, pPlayer->GetName(), pPlayer->GetSkills().GetOverallSkill(),
//                                  WorldData().GetCurrentUserClub()->GetForeignStr(WorldData().GetCurrentUser()->GetPlayerSearch().GetPlayerValueList()[LoopCount]) );
	}
}

void FDTransferListEditScreen::OnSlider( lUIWidget *, float )
{
  // cost
  if( m_editfilterMinCost->GetValue() > m_editfilterMaxCost->GetValue() )
  {
    m_editfilterMaxCost->SetValue( m_editfilterMinCost->GetValue() );
  }

  if( m_editfilterMaxCost->GetValue() < m_editfilterMinCost->GetValue() )
  {
    m_editfilterMinCost->SetValue( m_editfilterMaxCost->GetValue() );
  }

  // age
  if( m_editfilterMinAge->GetValue() > m_editfilterMaxAge->GetValue() )
  {
    m_editfilterMaxAge->SetValue( m_editfilterMinAge->GetValue() );
  }

  if( m_editfilterMaxAge->GetValue() < m_editfilterMinAge->GetValue() )
  {
    m_editfilterMinAge->SetValue( m_editfilterMaxAge->GetValue() );
  }

  // skill
  if( m_editfilterMinSkill->GetValue() > m_editfilterMaxSkill->GetValue() )
  {
    m_editfilterMaxSkill->SetValue( m_editfilterMinSkill->GetValue() );
  }

  if( m_editfilterMaxSkill->GetValue() < m_editfilterMinSkill->GetValue() )
  {
    m_editfilterMinSkill->SetValue( m_editfilterMaxSkill->GetValue() );
  }

  Search();
}

void FDTransferListEditScreen::DoShow()
{
  m_transfereditScreen->Show();
  m_transfereditScreenSub->Show();
}

void FDTransferListEditScreen::DoHide()
{
  m_transfereditScreen->Hide();
  m_transfereditScreenSub->Hide();
}


void FDTransferListEditScreen::SetTransferFilterNumber( int number )
{
  m_transferFilterNumber = number;
  m_filter = GetMainUI()->GetTransferFilter(m_transferFilterNumber);

  m_editfilterMinCost->SetValue( m_filter.minPrice, true );
  m_editfilterMaxCost->SetValue( m_filter.maxPrice, true );

  m_editfilterMinAge->SetValue( m_filter.minAge, true );
  m_editfilterMaxAge->SetValue( m_filter.maxAge, true );

  m_editfilterMinSkill->SetValue( m_filter.minSkill, true );
  m_editfilterMaxSkill->SetValue( m_filter.maxSkill, true );

  m_editfilterPosition->SetSelectedItem( m_filter.positionType, true );

  if( m_filter.flags & CPlayerSearch::INCLUDEONTRANSFERLIST )
  {
    lLogFmt("Inc TransferListed\n");
    m_editfilterListedPlayers->SetChecked( true );
  }

  if( m_filter.flags & CPlayerSearch::INCLUDEONLOANLIST )
  {
    lLogFmt("Inc LoanListed\n");
    m_editfilterLoanListedPlayers->SetChecked( true );
  }

  if( m_filter.flags & CPlayerSearch::INCLUDENONLISTED )
  {
    lLogFmt("Inc NonListed\n");
    m_editfilterNonListedPlayers->SetChecked( true );
  }

  if( m_filter.flags & CPlayerSearch::INCLUDEINJURED )
  {
    lLogFmt("Inc Injured\n");
    m_editfilterInjuredPlayers->SetChecked( true );
  }

  if( m_filter.flags & CPlayerSearch::INCLUDEOUTOFCONTRACT )
  {
    lLogFmt("Inc OutOfContract\n");
    m_editfilterOutOfContractPlayers->SetChecked( true );
  }

  if(  m_filter.flags & CPlayerSearch::INCLUDESUSPENDED )
  {
    lLogFmt("Inc Suspended\n");
    m_editfilterSuspendedPlayers->SetChecked( true );
  }

  Search();
}


