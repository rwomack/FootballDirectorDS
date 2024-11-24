//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDClubRecordsscreen.cpp
// Description : FDClubRecordsScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>

#include <screens/fdclubrecordsscreen.h>

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
#include <fdfullscreenwidget.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget
#include <lemon/math/lmrand.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDClubRecordsScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDClubRecordsScreen::FDClubRecordsScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDClubRecordsScreen::~FDClubRecordsScreen()
{
}

void FDClubRecordsScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  // *** ClubRecords ***

  m_clubrecordScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "clubrecord_screen" );
  m_clubrecordScreen->Hide();

  for( int i = 0; i < 3; i++ )
  {
    m_compBack[i] = FD_GAME_NEW FDBackingWidget(m_clubrecordScreen, 0, "stripe", L_ARGB( 127, 30, 30, 30 ) , LUI_TOP_LEFT,
     lUIPoint( 20, 16 + i * 56 ), lUIPoint(236, 42));

    m_compBars[i]  = FD_GAME_NEW lUISpriteWidget(m_clubrecordScreen, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
     lUIPoint( 20, 20 + i * 56 ), lUIPoint(236, 12));
    m_compBars[i]->GetSprite()->SetWidth(236 );

    m_compLabel[i] = FD_GAME_NEW lUITextWidget(m_clubrecordScreen, 0, "label", lUIText(""), LUI_TOP_CENTER,
     lUIPoint( 137, 20 + i * 56 ), lUIPoint(122, 12));
  }

  //TODO LOCALE
  m_compLabel[0]->SetText( lUIText( "Record Category" ) );
  m_compLabel[1]->SetText( lUIText( "League" ) );
  m_compLabel[2]->SetText( lUIText( "Team" ) );

  m_compLabel[0]->SetTextToUpper();
  m_compLabel[1]->SetTextToUpper();
  m_compLabel[2]->SetTextToUpper();

  m_recordType = FD_GAME_NEW FDComboWidget( m_clubrecordScreen, 0, "recordtype_combo", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 138, 44 ), lUIPoint( 135, 19 ) );
  m_recordType->SetAllToUpper( true );
  m_recordType->AddItem( lUIText( IDS_OPTIONS_GENERAL ) ); // General
  m_recordType->AddItem( lUIText( IDS_SCREEN09PLAYER ) ); // Player
  m_recordType->AddItem( lUIText( IDS_TAB3601 ) ); // Team

  m_recordType->SignalSelection.Connect( this, &FDClubRecordsScreen::OnTypeCombo );

  m_clubList = FD_GAME_NEW FDComboWidget( m_clubrecordScreen, 0, "clubrecordsclub_combo", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 138, 156 ), lUIPoint( 135, 19 ) );
  m_clubList->SignalSelection.Connect( this, &FDClubRecordsScreen::OnClubCombo );
  //m_clubList->SetAllToUpper( true );

  m_divisionList = FD_GAME_NEW FDComboWidget( m_clubrecordScreen, 0, "clubrecordsdivision_combo", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 138, 100 ), lUIPoint( 135, 19 ) );
  m_divisionList->SignalSelection.Connect( this, &FDClubRecordsScreen::OnDivisionCombo );
  m_divisionList->SetAllToUpper( true );


  // *** ClubRecords Sub ***

  m_clubrecordScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "clubrecord_screen_sub" );
  m_clubrecordScreenSub->Hide();

  // backing stripes
  for( int i = 0; i < 10; i++ )
  {
    m_backingStripes[i] = FD_GAME_NEW lUISpriteWidget(m_clubrecordScreenSub, 0, "stripe", FDResourceManager::GetSprite("uiicon", "grad_grey1"), LUI_TOP_LEFT,
     lUIPoint( 0, 50 + i * 14 ), lUIPoint(256, 12));
	  m_backingStripes[i]->GetSprite()->SetWidth( 256 );
  }

  for( int i = 0; i < 10; i++ )
  {
    m_backingBars[i] = FD_GAME_NEW lUISpriteWidget(m_clubrecordScreenSub, 0, "bar", FDResourceManager::GetSprite("uiicon", "grad_grey2"), LUI_TOP_LEFT,
     lUIPoint( 120,60 + i * 14 ), lUIPoint(104, 12));
  }

  for( int i = 0; i < 10; i++ )
  {
    m_recordLabel[i] = FD_GAME_NEW lUITextWidget(m_clubrecordScreenSub, 0, "stripe", lUIText(""), LUI_TOP_LEFT,
     lUIPoint( 0, 50 + i * 32 ), lUIPoint(130, 12));

    m_recordValue[i] = FD_GAME_NEW lUITextWidget(m_clubrecordScreenSub, 0, "bar", lUIText(""), LUI_TOP_LEFT,
     lUIPoint( 120, 60 + i * 32 ), lUIPoint(104, 12));
  }

  for( int i = 0; i < 10; i++ )
  {
    m_recordLabel[i]->SetAlign( LUI_TOP_RIGHT );
    m_recordValue[i]->SetAlign( LUI_TOP_CENTER );
  }

  // Init
}

void FDClubRecordsScreen::Prepare()
{
  // Set up division combo
  m_divisionList->ClearAll();
  CDivisionList &divisionList = WorldData().GetDivisionList();

  for(int i = 0; i < divisionList.GetSize(); i++)
  {
    CDivision &division = divisionList.GetAt(i);
    m_divisionList->AddItem( lUIText( division.GetName() ) );
  }

  // Set our club as selected
  // NOTE: there must be a better way than this
  CClub * ourClub = WorldData().GetCurrentUserClub();

  CClubList &clubList = WorldData().GetClubList();
  const CDivision &division = WorldData().GetDivisionList().GetAt( ourClub->GetDivisionID() );

  int clubCount = -1;
  int index = -1;
  for(int i = 0; i < clubList.GetSize(); i++)
  {
    CClub & club = clubList[i];

    if( club.IsNonLeagueClub() )
    {
      // skip non-league
      continue;
    }

    if( club.GetDivisionID() == ourClub->GetDivisionID() )
    {
       clubCount++;
      if( ourClub->GetNameID()  == club.GetNameID() )
      {
        index = clubCount;
        break;
      }
    }
  }

  // Refresh Club combo
  m_divisionList->SetSelectedItem( ourClub->GetDivisionID(), false );
  m_clubList->SetSelectedItem( index, false );

  // Refresh info
  OnTypeCombo( m_recordType, m_recordType->GetSelectedItem() );
}

void FDClubRecordsScreen::SerializeState( FDStateStorage &state )
{
  if( state.IsSaving() )
  {
    int selectedDiv  = m_divisionList->GetSelectedItem();
    int selectedClub = m_clubList->GetSelectedItem();
    int selectedType = m_recordType->GetSelectedItem();
    state << selectedDiv;
    state << selectedClub;
    state << selectedType;
    lLogFmt("Saving div %d club %d type %d\n", selectedDiv, selectedClub, selectedType);
  } else
  {
    int selectedDiv;
    int selectedClub;
    int selectedType;
    state << selectedDiv;
    state << selectedClub;
    state << selectedType;
    lLogFmt("Loading div %d club %d type %d\n", selectedDiv, selectedClub, selectedType);
    m_divisionList->SetSelectedItem(selectedDiv, false);
    m_clubList->SetSelectedItem(selectedClub, false);
    m_recordType->SetSelectedItem(selectedType, false);
  }
}


void FDClubRecordsScreen::DoShow()
{
  m_clubrecordScreen->Show();
  m_clubrecordScreenSub->Show();
}

void FDClubRecordsScreen::DoHide()
{
  m_clubrecordScreen->Hide();
  m_clubrecordScreenSub->Hide();
}

void FDClubRecordsScreen::OnTypeCombo( FDComboWidget *, int idx )
{
  //
  CClubList &clubList = WorldData().GetClubList();
  const CDivision &division = WorldData().GetDivisionList().GetAt( m_divisionList->GetSelectedItem() );

  int clubCount = -1;
  int clubIndex = -1;

  for(int i = 0; i < clubList.GetSize(); i++)
  {
    CClub & club = clubList[i];

    if( club.IsNonLeagueClub() )
    {
      // skip non-league
      continue;
    }

    if( club.GetDivisionID() == m_divisionList->GetSelectedItem() )
    {
      clubCount++;
      if( clubCount == m_clubList->GetSelectedItem() )
      {
        clubIndex = i;
        break;
      }
    }
  }

  lLogFmt("div: %d, club: %d, idx %d\n", m_divisionList->GetSelectedItem(), m_clubList->GetSelectedItem(), clubIndex);

  CClub* pClub = &clubList[ clubIndex ]; //WorldData().GetCurrentUserClub();

  if( pClub == NULL )
  {
    return;
  }

	CClubHistory& rClubHistory = pClub->GetClubHistory();

  for( int i = 0; i < 10; i++ )
  {
    m_recordLabel[i]->SetText( lUIText( "" ) );
    m_recordValue[i]->SetText( lUIText( "" ) );

    m_backingStripes[i]->Hide();
    m_backingBars[i]->Hide();
  }

  switch( idx )
  {
    case 0:
    {
      int rowidx = 0;

      m_recordLabel[rowidx]->SetText( lUIText( IDS_YEARFOUNDED ) );
      m_recordLabel[rowidx]->SetTextToUpper();
      m_recordValue[rowidx]->SetText( lUIText( pClub->GetYearFoundedStr() ) );
      rowidx++;

      m_recordLabel[rowidx]->SetText( lUIText( IDS_MANAGER ) );
      m_recordLabel[rowidx]->SetTextToUpper();
      m_recordValue[rowidx]->SetText( lUIText( pClub->GetManagerName() ) );
      rowidx++;

      m_recordLabel[rowidx]->SetText( lUIText( IDS_STADIUM ) );
      m_recordLabel[rowidx]->SetTextToUpper();
      m_recordValue[rowidx]->SetText( lUIText( pClub->GetStadium()->GetName() ) );
      rowidx++;

      m_recordLabel[rowidx]->SetText( lUIText( IDS_CAPACITY ) );
      m_recordLabel[rowidx]->SetTextToUpper();
      m_recordValue[rowidx]->SetText( lUIText( pClub->GetStadium()->GetCapacityStr() ) );
      rowidx++;

      m_recordLabel[rowidx]->SetText( lUIText( IDS_AVATTEND ) );
      m_recordLabel[rowidx]->SetTextToUpper();
      m_recordValue[rowidx]->SetText( lUIText( pClub->GetSeasonStats().GetAverageAttendanceStr() ) );
      rowidx++;

      m_recordLabel[rowidx]->SetText( lUIText( IDS_RECORDATTENDANCE ) );
      m_recordLabel[rowidx]->SetTextToUpper();
      m_recordValue[rowidx]->SetText( lUIText( rClubHistory.RecordAttendanceStr() ) );
      rowidx++;

      m_recordLabel[rowidx]->SetText( lUIText( IDS_LOWESTATTENDANCE ) );
      m_recordLabel[rowidx]->SetTextToUpper();
      m_recordValue[rowidx]->SetText( lUIText( rClubHistory.LowestAttendanceStr() ) );
      rowidx++;

      m_recordLabel[rowidx]->SetText( lUIText( "Also Known As" ) );
      m_recordLabel[rowidx]->SetTextToUpper();
      m_recordValue[rowidx]->SetText( lUIText( pClub->GetNickname() ) );
      rowidx++;

      for( int i = 0; i < rowidx; i++ )
      {
        m_recordLabel[i]->Move( lUIWidgetPosition( lUIPoint( 0, 50 + i * 14 ) ), lUIWidgetSize( lUIPoint(130, 12) ));
        m_recordLabel[i]->SetAlign( LUI_TOP_RIGHT );

        m_recordValue[i]->Move( lUIWidgetPosition( lUIPoint( 135, 50 + i * 14 ) ), lUIWidgetSize( lUIPoint( 120, 12) ) );
        m_recordValue[i]->SetAlign( LUI_TOP_LEFT );

        m_backingStripes[i]->Move( lUIWidgetPosition( lUIPoint( 0, 50 + i * 14 ) ) );
        m_backingBars[i]->Move( lUIWidgetPosition( lUIPoint( 133, 50 + i * 14 ) ), lUIWidgetSize( lUIPoint( 120, 12 ) ) );

        m_backingStripes[i]->Show();
        m_backingBars[i]->Show();
      }
      break;
    }

    case 1:
    {
      int rowidx = 0;

      m_recordLabel[rowidx]->SetText( lUIText( IDS_RECORDSALE ) );
      m_recordLabel[rowidx]->SetTextToUpper();
      m_recordValue[rowidx]->SetText( lUIText( rClubHistory.RecordSaleInfoStr(pClub->GetCountry()) ) );
      rowidx++;

      m_recordLabel[rowidx]->SetText( lUIText( IDS_RECORDBUY ) );
      m_recordLabel[rowidx]->SetTextToUpper();
      m_recordValue[rowidx]->SetText( lUIText( rClubHistory.RecordBuyInfoStr(pClub->GetCountry()) ) );
      rowidx++;

      m_recordLabel[rowidx]->SetText( lUIText( IDS_RECORDAPPEARANCES ) );
      m_recordLabel[rowidx]->SetTextToUpper();
      m_recordValue[rowidx]->SetText( lUIText( rClubHistory.RecordAppearanceInfoStr() ) );
      rowidx++;

      m_recordLabel[rowidx]->SetText( lUIText( IDS_RECORDCLUBGOALS  ) );
      m_recordLabel[rowidx]->SetTextToUpper();
      m_recordValue[rowidx]->SetText( lUIText( rClubHistory.RecordGoalsInfoStr() ) );
      rowidx++;

//      m_recordLabel[rowidx]->SetText( lUIText( IDS_RECORDSEASONGOALS ) );
//      m_recordValue[rowidx]->SetText( lUIText( rClubHistory.RecordSeasonGoalsInfoStr() ) );
//      rowidx++;

      // Move bars
      for( int i = 0; i < rowidx; i++ )
      {
        m_recordLabel[i]->Move( lUIWidgetPosition( lUIPoint( 0, 50 + i * 28 ) ), lUIWidgetSize( lUIPoint( 256, 12 ) ) );
        m_recordLabel[i]->SetAlign( LUI_TOP_CENTER );
        m_recordValue[i]->Move( lUIWidgetPosition( lUIPoint( 0, 50 + 14 + i* 28) ), lUIWidgetSize( lUIPoint(256, 12) ) );
        m_recordValue[i]->SetAlign( LUI_TOP_CENTER );

        m_backingBars[i]->Move( lUIWidgetPosition( lUIPoint( 5, 50 + 14 + i* 28 ) ), lUIWidgetSize( lUIPoint(240, 12 ) ) );
        m_backingBars[i]->Show();
      }

      for( int i = 0; i < rowidx*2; i++ )
      {
        m_backingStripes[i]->Move( lUIWidgetPosition( lUIPoint( 0, 50 + i * 14 ) ) );
        m_backingStripes[i]->Show();
      }

      break;
    }

    case 2:
    {
      int rowidx = 0;

      if( rClubHistory.GetRecordVictory() != NULL )
      {
        m_recordLabel[rowidx]->SetText( lUIText( IDS_RECORDVICTORY ) );
        m_recordLabel[rowidx]->SetTextToUpper();
        m_recordValue[rowidx++]->SetText( lUIText( rClubHistory.GetRecordVictory()->DetailsStr() ) );
      }

      if( rClubHistory.GetRecordDefeat() != NULL )
      {
        m_recordLabel[rowidx]->SetText( lUIText( IDS_RECORDDEFEAT ) );
        m_recordLabel[rowidx]->SetTextToUpper();
        m_recordValue[rowidx++]->SetText( lUIText( rClubHistory.GetRecordDefeat()->DetailsStr() ) );
      }

      CCountry &country = pClub->GetCountry();

      if( rClubHistory.GetMostPoints() != NULL )
      {
        m_recordLabel[rowidx]->SetText( lUIText( IDS_RECORDPOINTS ) );
        m_recordLabel[rowidx]->SetTextToUpper();
        m_recordValue[rowidx++]->SetText( lUIText( rClubHistory.GetMostPoints()->DetailsStr(country) ) );
      }

      if( rClubHistory.GetLeastPoints() != NULL )
      {
        m_recordLabel[rowidx]->SetText( lUIText( IDS_LEASTPOINTS ) );
        m_recordLabel[rowidx]->SetTextToUpper();
        m_recordValue[rowidx++]->SetText( lUIText( rClubHistory.GetLeastPoints()->DetailsStr(country) ) );
      }

//      if( rClubHistory.GetHighestPosition() != NULL )
//      {
//        m_recordLabel[rowidx]->SetText( lUIText( IDS_RECORDPOSITION ) );
//        m_recordValue[rowidx++]->SetText( lUIText( rClubHistory.GetHighestPosition()->DetailsStr(country) ) );
//      }
//
//      if( rClubHistory.GetLowestPosition() != NULL )
//      {
//        m_recordLabel[rowidx]->SetText( lUIText( IDS_LOWESTPOSITION ) );
//        m_recordValue[rowidx++]->SetText( lUIText( rClubHistory.GetLowestPosition()->DetailsStr(country) ) );
//      }

      for( int i = 0; i < rowidx; i++ )
      {
        m_recordLabel[i]->Move( lUIWidgetPosition( lUIPoint( 0, 50 + i * 28 ) ), lUIWidgetSize( lUIPoint(256, 12) ) );
        m_recordLabel[i]->SetAlign( LUI_TOP_CENTER );
        m_recordValue[i]->Move( lUIWidgetPosition( lUIPoint( 0, 50 + 14 + i* 28) ), lUIWidgetSize( lUIPoint(256, 12) ) );
        m_recordValue[i]->SetAlign( LUI_TOP_CENTER );

        m_backingBars[i]->Move( lUIWidgetPosition( lUIPoint( 5, 50 + 14 + i* 28 ) ), lUIWidgetSize( lUIPoint( 242, 12 ) ) );
        m_backingBars[i]->Show();
      }

      for( int i = 0; i < rowidx*2; i++ )
      {
        m_backingStripes[i]->Move( lUIWidgetPosition( lUIPoint( 0, 50 + i * 14 ) ) );
        m_backingStripes[i]->Show();
      }
      break;
    }
  }


}

void FDClubRecordsScreen::OnClubCombo( FDComboWidget *, int idx )
{
  // Refresh info
  OnTypeCombo( m_recordType, m_recordType->GetSelectedItem() );
}

void FDClubRecordsScreen::OnDivisionCombo( FDComboWidget *, int idx )
{
  // Update clubs
  CClubList &clubList = WorldData().GetClubList();
  const CDivision &division = WorldData().GetDivisionList().GetAt( idx );

  m_clubList->ClearAll();

  for(int i = 0; i < clubList.GetSize(); i++)
  {
    CClub & club = clubList[i];

    if( club.IsNonLeagueClub() )
    {
      // skip non-league
      continue;
    }

    if( club.GetDivisionID() == idx )
    {
      m_clubList->AddItem( lUIText( club.GetName() ), true );

      lLogFmt("  %d: %s\n", i, club.GetName());
    }
  }

  // Refresh Club and info
  m_clubList->SetSelectedItem( 0, false );
}

void FDClubRecordsScreen::Update()
{
  lLogFmt("ClubRecords::Update\n");

}
