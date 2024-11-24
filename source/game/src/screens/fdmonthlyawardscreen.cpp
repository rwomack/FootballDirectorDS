//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDMonthlyAwardScreen.cpp
// Description : FDMonthlyAwardScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdtextwidget.h>
#include <fdgameworld.h>

#include <screens/fdmonthlyawardscreen.h>
#include <screens/fdmanagerinfoscreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDMonthlyAwardScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDMonthlyAwardScreen::FDMonthlyAwardScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDMonthlyAwardScreen::~FDMonthlyAwardScreen()
{
}

void FDMonthlyAwardScreen::Init()
{

  // Bottom screen
  m_monthlyawardscreenScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "monthsawardscreen" );
  m_monthlyawardscreenScreen->Hide();

  m_managerBacking[0] = FD_GAME_NEW FDBackingWidget( m_monthlyawardscreenScreen, 0, "backssLeague", L_ARGB( 127, 30, 30, 30 ), LUI_TOP_LEFT, lUIPoint( 20, 116 ), lUIPoint( 237, 42 ) );
  m_managerStripes[0] = FD_GAME_NEW lUISpriteWidget( m_monthlyawardscreenScreen, 0, "stripesLeague", FDResourceManager::GetSprite( "uiicon", "grad_grey1") , LUI_TOP_LEFT, lUIPoint( 20, 121 ), lUIPoint( 237, 12) );
  //TODO LOCALE
  m_managerLabels[0] = FD_GAME_NEW lUITextWidget( m_monthlyawardscreenScreen, 0, "labelsLeague", lUIText("Month") , LUI_TOP_CENTER, lUIPoint( 137, 121 ), lUIPoint( 80, 12));
  m_managerLabels[0]->SetTextToUpper();

  m_monthSelection = FD_GAME_NEW FDComboWidget( m_monthlyawardscreenScreen,  0, "mommonthsselection", lUIText(""), LUI_TOP_CENTER, lUIPoint( 137, 135 ), lUIPoint( 135, 19 ) );
  m_monthSelection->SignalSelection.Connect( this, &FDMonthlyAwardScreen::OnMonthSelection );
  m_monthSelection->SetAllToUpper( true );

  CDivisionList &divisionList = WorldData().GetDivisionList();


  m_managerBacking[1] = FD_GAME_NEW FDBackingWidget( m_monthlyawardscreenScreen, 0, "backssLeague", L_ARGB( 127, 30, 30, 30 ), LUI_TOP_LEFT, lUIPoint( 20, 47 ), lUIPoint( 237, 42 ) );
  m_managerStripes[1] = FD_GAME_NEW lUISpriteWidget( m_monthlyawardscreenScreen, 0, "stripesLeague", FDResourceManager::GetSprite( "uiicon", "grad_grey1") , LUI_TOP_LEFT, lUIPoint( 20, 52 ), lUIPoint( 237, 12) );
  //TODO LOCALE
  m_managerLabels[1] = FD_GAME_NEW lUITextWidget( m_monthlyawardscreenScreen, 0, "labelsLeague", lUIText("League") , LUI_TOP_CENTER, lUIPoint( 137, 52 ), lUIPoint( 80, 12));
  m_managerLabels[1]->SetTextToUpper();

  // Note: the first  index into division-getmanagerofthemonth is august not january
  for( int i = 0; i < 5; i++ )
    m_monthSelection->AddItem( lUIText( IDS_AUG + i ) );
  for( int i = 0; i < 4; i++ )
    m_monthSelection->AddItem( lUIText( IDS_JAN + i ) );

  m_divisionSelection = FD_GAME_NEW FDComboWidget( m_monthlyawardscreenScreen,  0, "momselectdivision_combo", lUIText(""), LUI_TOP_CENTER, lUIPoint( 137, 66 ), lUIPoint( 135, 19 ) );
  m_divisionSelection->SignalSelection.Connect( this, &FDMonthlyAwardScreen::OnDivisionSelection );
  m_divisionSelection->SetAllToUpper( true );

  // set up division table
  for(int i = 0; i < divisionList.GetSize(); i++)
  {
    CDivision &division = divisionList.GetAt(i);
    m_divisionSelection->AddItem( lUIText( division.GetName() ) );
  }

  // Top Screen
  m_monthlyawardscreenScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "monthsawardscreen" );
  m_monthlyawardscreenScreenSub->Hide();

  m_managerBacking[2] = FD_GAME_NEW FDBackingWidget( m_monthlyawardscreenScreenSub, 0, "backssLeague", L_ARGB( 127, 30, 30, 30 ), LUI_TOP_LEFT, lUIPoint( 0, 96 ), lUIPoint( 255, 24 ) );
  m_managerStripes[2] = FD_GAME_NEW lUISpriteWidget( m_monthlyawardscreenScreenSub, 0, "stripesLeague", FDResourceManager::GetSprite( "uiicon", "grad_grey1") , LUI_TOP_LEFT, lUIPoint( 0, 101 ), lUIPoint( 255, 12) );
  m_managerText = FD_GAME_NEW lUITextWidget( m_monthlyawardscreenScreenSub, 0, "nomanager", lUIText( IDS_MOTM_NOT_AWARDED ) , LUI_TOP_CENTER, lUIPoint( 127, 101 ));

}

void FDMonthlyAwardScreen::DoShow()
{
  m_monthlyawardscreenScreen->Show();
  GetMainUI()->GetManagerInfoScreen()->Show();
}

void FDMonthlyAwardScreen::DoHide()
{
  m_monthlyawardscreenScreen->Hide();
  m_monthlyawardscreenScreenSub->Hide();
  GetMainUI()->GetManagerInfoScreen()->Hide();
}

bool FDMonthlyAwardScreen::HasDataToDisplay() const
{
  CDivisionList &divisionList = WorldData().GetDivisionList();
  CManagerOfTheMonth * motm = &divisionList.GetAt( m_divisionSelection->GetSelectedItem() ).GetManagerOfMonth( 0 );
  return !motm->IsEmpty();
}

void FDMonthlyAwardScreen::SerializeState( FDStateStorage &state )
{
  if( state.IsSaving() )
  {
    int division = m_divisionSelection->GetSelectedItem();
    int month    = m_monthSelection->GetSelectedItem();

    state << division;
    state << month;

    lLogFmt("Saving division %d month %d\n", division, month);
  } else
  {
    int division;
    int month;

    state << division;
    state << month;

    lLogFmt("Loading division %d month %d\n", division, month);
    m_divisionSelection->SetSelectedItem( division, false );
    m_monthSelection->SetSelectedItem( month, false );
  }
}


void FDMonthlyAwardScreen::Update()
{
  if (IsForcedUpdateRequested() == false)
	  return;

  SetForcedUpdateRequested(false);

  m_monthlyawardscreenScreenSub->Show();
  GetMainUI()->GetManagerInfoScreen()->Hide();

  // Get Manager of the month
  CDivisionList &divisionList = WorldData().GetDivisionList();
  CManagerOfTheMonth * motm = &divisionList.GetAt( m_divisionSelection->GetSelectedItem() ).GetManagerOfMonth( m_monthSelection->GetSelectedItem() );
  GetMainUI()->GetManagerInfoScreen()->SetManagerInfo( NULL );
  if( !motm->IsEmpty() )
  {
    for( int i = 0; i < divisionList.GetAt( m_divisionSelection->GetSelectedItem() ).GetNumberOfClubsInDivision(); i++ )
    {
      CClub * club = &WorldData().GetClubByID(divisionList.GetAt( m_divisionSelection->GetSelectedItem() ).GetClubs()[i]);
      if( club->DoFindID()  == motm->GetClubID() )
      {
        CManager * manager =  &club->GetManager();
        GetMainUI()->GetManagerInfoScreen()->SetManagerInfo( manager );
        GetMainUI()->GetManagerInfoScreen()->Show();
        m_monthlyawardscreenScreenSub->Hide();
      }
    }

  }
}

void FDMonthlyAwardScreen::OnMonthSelection( FDComboWidget *, int index )
{
  lLogFmt("OnMonthSelection : %d\n", index );
  SetForcedUpdateRequested(true);
  Update();
}

void FDMonthlyAwardScreen::OnDivisionSelection( FDComboWidget *, int index )
{
  lLogFmt("OnDivisionSelection : %d\n", index );
  SetForcedUpdateRequested(true);
  Update();
}
