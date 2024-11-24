//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDCupSchedulescreen.cpp
// Description : FDCupScheduleScreen implementation
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

#include <screens/fdcupschedulescreen.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

#include <SortedStringList.h>
#include <CupDisplayEvent.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDCupScheduleScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDCupScheduleScreen::FDCupScheduleScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDCupScheduleScreen::~FDCupScheduleScreen()
{
}

void FDCupScheduleScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_cupschScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "cupsch_screen" );
  m_cupschScreen->Hide();

  m_table = FD_GAME_NEW FDTableWidget( m_cupschScreen, 0, "cupschtable", //FDTableWidget::TABLE_FLAG_ROW_LINES |
                               FDTableWidget::TABLE_FLAG_CELLBACKGROUND|FDTableWidget::TABLE_FLAG_FIXED_FIRSTROW |
			FDTableWidget::TABLE_FLAG_CAP_FIRSTROW|FDTableWidget::TABLE_FLAG_BACKGROUND,
                               LUI_TOP_LEFT,  lUIPoint( 20, 12 ), lUIPoint( 256 - 20 - GetDefaultScrollBarWidth(), 144 ) );

  FDScrollBarWidget *pScrollBar = FD_GAME_NEW FDScrollBarWidget(m_cupschScreen, 0, "tablescroll", LUI_TOP_LEFT, lUIPoint(256-20, 12), lUIPoint(GetDefaultScrollBarWidth(), 146));
  pScrollBar->AlignTo( m_table );
  m_table->SetScrollBar(pScrollBar);

  // cup / round
  m_cupSelect = FD_GAME_NEW FDComboWidget( m_cupschScreen, 0, "cscupcombo_combo", lUIText(""), LUI_CENTER_BOTTOM, lUIPoint( 138, 178 ) );
  m_cupSelect->SetAllToUpper( true );

  CStringArray theList;
  WorldData().DoBuildCupNameList(theList);

  for(int i = 0; i < theList.GetSize(); i++)
  {
    m_cupSelect->AddItem( lUIText( theList.GetAt( i ) ) );
    lLogFmt("%d: %s - %d\n", i, theList.GetAt( i ), i);
  }

  m_cupSelect->SignalSelection.Connect( this, &FDCupScheduleScreen::OnCupCombo );

  // top screen
  m_cupschScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "cupsch_screen_sub" );
  m_cupschScreenSub->Hide();

  // top screen cup info
  m_cupName = FD_GAME_NEW lUITextWidget( m_cupschScreenSub, 0, "teams", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 128, 169 ) );
  m_cupName->SetFont( FDResourceManager::GetFont("font16", "font16" ) );
  m_cupName->SetShadow( true, L_ARGB(255, 0, 0, 0 ) );
}

void FDCupScheduleScreen::DoShow()
{
  m_cupschScreen->Show();
  m_cupschScreenSub->Show();
}

void FDCupScheduleScreen::DoHide()
{
  m_cupschScreen->Hide();
  m_cupschScreenSub->Hide();
}

void FDCupScheduleScreen::SerializeState( FDStateStorage &state )
{
  if( state.IsSaving() )
  {
    int selectedCup  = m_cupSelect->GetSelectedItem();
    state << selectedCup;
    lLogFmt("Saving cup %d\n", selectedCup);
  } else
  {
    int selectedCup;
    state << selectedCup;
    lLogFmt("Loading cup %d\n", selectedCup);
    m_cupSelect->SetSelectedItem(selectedCup, false);
  }
}


void FDCupScheduleScreen::Update()
{
  OnCupCombo( m_cupSelect, m_cupSelect->GetSelectedItem() );
}

void FDCupScheduleScreen::OnCupCombo( FDComboWidget *, int idx )
{
  lLogFmt("Cup: %d\n", idx);

  if( idx == -1 )
  {
    return;
  }

  // set title
  m_cupName->SetText( lUIText( WorldData().GetCup(idx)->GetName() ) );

  // get dates
	CCupDisplayEventList	CupEventList;
  WorldData().GetCup(idx)->DoBuildEventsStrList(CupEventList);

  m_table->ClearAll();
  m_table->AddColumn( lUICoord( 100 ), LUI_CENTER_CENTER );
  m_table->AddColumn( lUICoord( 10 ), LUI_CENTER_CENTER ); // gap
  m_table->AddColumn( lUICoord( 125 ), LUI_CENTER_CENTER  );

  m_table->AddRows( CupEventList.GetSize() + 1);

  m_table->SetRowBkgColour( 0, L_ARGB(255, 35, 35, 35) );
  m_table->SetText( 0, 0, lUIText( "Date" ) );
  m_table->SetText( 0, 2, lUIText( "Event" ) );

	for (int LoopCount = 0; LoopCount < CupEventList.GetSize(); LoopCount++)
  {
    CCupRound* pCupRound = CupEventList[LoopCount].GetCupRound();

    m_table->SetBkgColour( LoopCount + 1, 0, L_ARGB(128,80,80,80) );
    m_table->SetBkgColour( LoopCount + 1, 1, L_ARGB(128,130,130,130) );
    m_table->SetBkgColour( LoopCount + 1, 2, L_ARGB(128,80,80,80) );

    switch( CupEventList[LoopCount].GetType() )
		{
			case DRAWDATE:
        m_table->SetText( LoopCount + 1, 0, lUIText( pCupRound->LongDrawDateStr() ) );
				break;
			case MATCHDATE:
        m_table->SetText( LoopCount + 1, 0, lUIText( pCupRound->LongMatchDateStr() ) );
				break;
			case REPLAYDATE:
        m_table->SetText( LoopCount + 1, 0, lUIText( pCupRound->LongReplayDateStr() ) );
				break;
			default:
				EXT_ASSERT(false);
				break;
		}

    m_table->SetText( LoopCount + 1, 2, lUIText( CupEventList[LoopCount].TitleStr() ) );
	}

  m_table->ResetScroll();
}

