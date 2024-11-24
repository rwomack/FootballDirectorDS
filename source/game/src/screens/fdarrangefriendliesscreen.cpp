//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDArrangeFriendlies.cpp
// Description : FDArrangeFriendlies implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdgameworld.h>
#include <fdscrollbarwidget.h>
#include <fdtext.h>

#include <screens/fdarrangefriendliesscreen.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDArrangeFriendlies
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDArrangeFriendlies::FDArrangeFriendlies( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDArrangeFriendlies::~FDArrangeFriendlies()
{
}

void FDArrangeFriendlies::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_arrangeFriendliesScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "af_screen" );
  m_arrangeFriendliesScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "af_screen_sub" );

  // Sub screen

  m_gradients.Add(12);
  for( int i = 0; i < 2; i++ )
  {
    m_gradients(i) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "grad",  FDResourceManager::GetSprite("uiicon","grad_grey2"), LUI_LEFT_TOP,  lUIPoint( 47, 72 + i * 14), lUIPoint( 163, 12 ) );
    m_gradients(6 + i) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "grad",  FDResourceManager::GetSprite("uiicon","grad_grey1"), LUI_TOP_CENTER,  lUIPoint( 126, 72 + i * 14), lUIPoint( 4, 12 )  );
  }
  m_gradients(2) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "grad",  FDResourceManager::GetSprite("uiicon","grad_grey2"), LUI_LEFT_TOP,  lUIPoint( 6, 72 + 2 * 14), lUIPoint( 244, 12 ) );
  m_gradients(6 + 2) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "grad",  FDResourceManager::GetSprite("uiicon","grad_grey1"), LUI_TOP_CENTER,  lUIPoint( 126, 72 + 2 * 14), lUIPoint( 4, 12 )  );

  for( int i = 3; i < 5; i++ )
  {
    m_gradients(i) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "grad",  FDResourceManager::GetSprite("uiicon","grad_grey2"), LUI_LEFT_TOP,  lUIPoint( 47, 96 + i * 14), lUIPoint( 163, 12 ) );
    m_gradients(6 + i) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "grad",  FDResourceManager::GetSprite("uiicon","grad_grey1"), LUI_TOP_CENTER,  lUIPoint( 126, 96 + i * 14), lUIPoint( 4, 12 )  );
  }

  m_gradients(5) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "grad",  FDResourceManager::GetSprite("uiicon","grad_grey2"), LUI_LEFT_TOP,  lUIPoint( 6, 96 + 5 * 14), lUIPoint( 244, 12 ) );
  m_gradients(6 + 5) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "grad",  FDResourceManager::GetSprite("uiicon","grad_grey1"), LUI_TOP_CENTER,  lUIPoint( 126, 96 + 5 * 14), lUIPoint( 4, 12 )  );

  // Left. Bottom Left, Right, bottom right
  m_summaryText.Add(12);
  for( int i = 0; i < 2; i++ )
  {
    m_summaryText(i) = FD_GAME_NEW lUITextWidget( m_arrangeFriendliesScreenSub, 0, "titletext", lUIText(""), LUI_LEFT_TOP, lUIPoint( 48, 72 + i * 14 ), lUIPoint( 70, 12 ) );
    m_summaryText(3 + i) = FD_GAME_NEW lUITextWidget( m_arrangeFriendliesScreenSub, 0, "titletext", lUIText(""), LUI_LEFT_TOP, lUIPoint( 48, 137 + i * 14 ), lUIPoint( 70, 12 ) );
  m_summaryText(i)->SetAlign(LUI_CENTER_LEFT);
  m_summaryText(3 + i)->SetAlign(LUI_CENTER_LEFT);
  }

  m_summaryText(2) = FD_GAME_NEW lUITextWidget( m_arrangeFriendliesScreenSub, 0, "titletext", lUIText(""), LUI_LEFT_TOP, lUIPoint( 8, 72 + 2 * 14 ), lUIPoint( 110, 12 ) );
  m_summaryText(3 + 2) = FD_GAME_NEW lUITextWidget( m_arrangeFriendliesScreenSub, 0, "titletext", lUIText(""), LUI_LEFT_TOP, lUIPoint( 8, 137 + 2 * 14 ), lUIPoint( 110, 12 ) );
  m_summaryText(2)->SetAlign(LUI_CENTER_LEFT);
  m_summaryText(3 + 2)->SetAlign(LUI_CENTER_LEFT);


  for( int i = 6; i < 8; i++ )
  {
    m_summaryText(i)  = FD_GAME_NEW lUITextWidget( m_arrangeFriendliesScreenSub, 0, "titletext", lUIText(""), LUI_LEFT_TOP, lUIPoint( 138, 72 + (i-6) * 14 ), lUIPoint( 70, 12 ) );
    m_summaryText(3 + i)  = FD_GAME_NEW lUITextWidget( m_arrangeFriendliesScreenSub, 0, "titletext", lUIText(""), LUI_LEFT_TOP, lUIPoint( 138, 137 + (i-6) * 14 ), lUIPoint( 70, 12 ) );
  m_summaryText(i)->SetAlign(LUI_CENTER_RIGHT);
  m_summaryText(3 + i)->SetAlign(LUI_CENTER_RIGHT);
  }

  m_summaryText(8)  = FD_GAME_NEW lUITextWidget( m_arrangeFriendliesScreenSub, 0, "titletext", lUIText(""), LUI_LEFT_TOP, lUIPoint( 138, 72 + (8-6) * 14 ), lUIPoint( 110, 12 ) );
  m_summaryText(3 + 8)  = FD_GAME_NEW lUITextWidget( m_arrangeFriendliesScreenSub, 0, "titletext", lUIText(""), LUI_LEFT_TOP, lUIPoint( 138, 137 + (8-6) * 14 ), lUIPoint( 110, 12 ) );
  m_summaryText(8)->SetAlign(LUI_CENTER_RIGHT);
  m_summaryText(3 + 8)->SetAlign(LUI_CENTER_RIGHT);

  m_backBox.Add(4);
  m_clubKit.Add(4);

  m_backBox(0) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "box",  FDResourceManager::GetSprite("uiicon","shirt_holder"), LUI_LEFT_TOP,  lUIPointPercent( 2, 27 ) );
  m_backBox(0)->GetSprite()->SetAlphaEnable( true );
  m_backBox(1) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "box",  FDResourceManager::GetSprite("uiicon","shirt_holder"), LUI_LEFT_TOP, lUIPointPercent( 2, 61) );
  m_backBox(1)->GetSprite()->SetAlphaEnable( true );
  m_backBox(2) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "box",  FDResourceManager::GetSprite("uiicon","shirt_holder"), LUI_RIGHT_TOP, lUIPointPercent( 98, 27 ) );
  m_backBox(2)->GetSprite()->SetAlphaEnable( true );
  m_backBox(3) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "box",  FDResourceManager::GetSprite("uiicon","shirt_holder"), LUI_RIGHT_TOP, lUIPointPercent( 98, 61 ) );
  m_backBox(3)->GetSprite()->SetAlphaEnable( true );

  lUIPoint pt = m_backBox(0)->WidgetClientToScreen( lUIPoint( 5, 8 ) );
  m_clubKit(0) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "kit",  FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_LEFT_TOP,  pt );
  m_clubKit(0)->Hide();

  pt = m_backBox(1)->WidgetClientToScreen( lUIPoint( 5, 8 ) );
  m_clubKit(1) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "kit",  FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_LEFT_TOP, pt );
  m_clubKit(1)->Hide();

  pt = m_backBox(2)->WidgetClientToScreen( lUIPoint( 5, 8 ) );
  m_clubKit(2) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "kit",  FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_LEFT_TOP, pt );
  m_clubKit(2)->Hide();

  pt = m_backBox(3)->WidgetClientToScreen( lUIPoint( 5, 8 ) );
  m_clubKit(3) = FD_GAME_NEW lUISpriteWidget( m_arrangeFriendliesScreenSub, 0, "kit",  FDResourceManager::GetSprite("uilargekit","shirts_30x30_FX3"), LUI_LEFT_TOP, pt );
  m_clubKit(3)->Hide();

  // Main screen
  m_clubsTable = FD_GAME_NEW FDTableWidget( m_arrangeFriendliesScreen, 0, "ar_clubstable",
            FDTableWidget::TABLE_FLAG_BORDER|FDTableWidget::TABLE_FLAG_ROW_SELECT|FDTableWidget::TABLE_FLAG_BACKGROUND |
            FDTableWidget::TABLE_FLAG_CELLBACKGROUND | FDTableWidget::TABLE_FLAG_CUT_OFF,
            LUI_TOP_LEFT, lUIPoint( 20, 11 ), lUIPoint( 117 - 20, 168 ) );

  FDScrollBarWidget *pScrollBar = FD_GAME_NEW FDScrollBarWidget(m_arrangeFriendliesScreen, 0, "clubstablescroll", LUI_TOP_LEFT,
    lUIPoint(126, 11), lUIPoint(GetDefaultScrollBarWidth(), 168));

  pScrollBar->AlignTo( m_clubsTable );
  m_clubsTable->SetScrollBar(pScrollBar);
  m_clubsTable->SignalRowSelect.Connect( this, &FDArrangeFriendlies::OnRowSelect );

  // select information shown by table
  m_datesInfo = FD_GAME_NEW FDComboWidget( m_arrangeFriendliesScreen, 0, "artableinfo_combo_dates", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 192, 45 ), lUIPoint( 128, 19 ) );
  m_divisions = FD_GAME_NEW FDComboWidget( m_arrangeFriendliesScreen, 0, "artableinfo_combo_divisions", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 192, 20 ), lUIPoint( 128, 19 )  );
  m_datesInfo->SetAllToUpper( true );
  m_divisions->SetAllToUpper( true );

  m_divisions->SignalSelection.Connect( this, &FDArrangeFriendlies::OnTableInfoCombo );

  m_homeButton = FD_GAME_NEW FDButtonWidget( m_arrangeFriendliesScreen, 0, "arhome_button", FDResourceManager::GetSprite("ui","button55"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPoint( 166, 90 ) );
  m_awayButton = FD_GAME_NEW FDButtonWidget( m_arrangeFriendliesScreen, 0, "araway_button", FDResourceManager::GetSprite("ui","button55"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPoint( 220, 90 ) );
  m_homeButton->SignalControlCommand.Connect( this, &FDArrangeFriendlies::OnHomeButton );
  m_awayButton->SignalControlCommand.Connect( this, &FDArrangeFriendlies::OnAwayButton );
  m_homeButton->SetText( lUIText( "HOME" ) ); // Home
  m_awayButton->SetText( lUIText( "AWAY" ) ); // Away

  m_confirmButton = FD_GAME_NEW FDButtonWidget( m_arrangeFriendliesScreen, 0, "arconfirm_button", FDResourceManager::GetSprite("ui","button55"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPoint( 166, 120 ) );
  m_clearButton = FD_GAME_NEW FDButtonWidget( m_arrangeFriendliesScreen, 0, "arclear_button", FDResourceManager::GetSprite("ui","button55"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPoint(220, 120) );
  m_confirmButton->SignalControlCommand.Connect( this, &FDArrangeFriendlies::OnConfirmButton );
  m_clearButton->SignalControlCommand.Connect( this, &FDArrangeFriendlies::OnClearButton );
  m_confirmButton->SetText( lUIText( "CONFIRM" ) );
  m_confirmButton->Disable();
  m_clearButton->SetText( lUIText( "CLEAR" ) );

  m_continueButton = FD_GAME_NEW FDButtonWidget( m_arrangeFriendliesScreen, 0, "arcontinue_button", FDResourceManager::GetSprite("ui","button75"), lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_CENTER_CENTER, lUIPoint( 192, 167 ) );
  m_continueButton->SignalControlCommand.Connect( this, &FDArrangeFriendlies::OnContinueButton );
  m_continueButton->SetText( lUIText( "CONTINUE" ) );
}

//////////////////////////////////////////////////////////////////////////
// Called when the screen is shown. Resets all data on the screen.
//////////////////////////////////////////////////////////////////////////
void FDArrangeFriendlies::ResetScreenData()
{
	m_divisions->ClearAll();
	m_datesInfo->SetSelectedItem(0, true);
	m_divisions->SetSelectedItem(0, true);

	CDivisionList &divisionList = WorldData().GetDivisionList();
	for(int i = 0; i < divisionList.GetSize(); i++)
	{
		CDivision &division = divisionList.GetAt(i);
		m_divisions->AddItem( lUIText( division.GetName() ) );
	}

	m_awayButton->Highlight(true);
	m_homeButton->Highlight(false);

	for( int i = 0; i < 4; i++ )
	{
		m_friendly[i].m_club.id = NOCLUB;
		m_friendly[i].m_home = false;
		m_friendly[i].m_set = false;

    m_summaryText(i * 3 + 0)->SetText( lUIText("") );
    m_summaryText(i * 3 + 1)->SetText( lUIText("") );
    m_summaryText(i * 3 + 2)->SetText( lUIText("") );
		m_clubKit(i)->Hide();
	}

	m_bHome = false;

	if (WorldData().GetCurrentUserClub() != NULL)
		m_divisions->SetSelectedItem(WorldData().GetCurrentUserClub()->GetDivisionID(), true);

	Update();
}

void FDArrangeFriendlies::Update()
{
  lLogFmt("FDArrangeFriendlies::Update\n");

	// Clear List
	m_clubsTable->ClearAll();
  m_clubsTable->AddColumn( lUICoordPercent(1.f) );
  m_clubsTable->SetSortFunctionForColumn(0, FDTableWidget::GenericStringSort);

  //m_clubsTable->AddRows(1);
  //m_clubsTable->SetText( 0, 0, lUIText( IDS_CLUBS ) );
  //m_clubsTable->SetRowBkgColour(0, L_ARGB( 127, 130, 130, 130) );

  m_clubIdxList.Empty();

  CDivision& theDivision = WorldData().GetDivisionList().GetAt( m_divisions->GetSelectedItem() );
  CClub* pClub;
  int curTableIdx = 0;
	for (int LoopCount = 0; LoopCount < theDivision.GetNumberOfClubsInDivision(); LoopCount++)
	{
		pClub = &theDivision.DoFindClubInTablePosition(LoopCount);

		if( pClub != WorldData().GetCurrentUserClub() )
		{
			m_clubIdxList.Push( LoopCount );
			m_clubsTable->AddRows(1);
			m_clubsTable->SetText(curTableIdx, 0, lUIText( FDMakeClubNameLink( pClub ) ) );
			m_clubsTable->SetRowBkgColour(curTableIdx, L_ARGB( 127, 80, 80, 80) );
			m_clubsTable->SetTagDataForRow(curTableIdx, pClub->DoFindID().id);
			curTableIdx++;
		}
	}
	m_clubsTable->ResetScroll();
	m_clubsTable->SelectRow(0);

  lLogFmt("Setting dates\n");
  m_datesInfo->ClearAll();
  for (int LoopCount = 0; LoopCount < 4; LoopCount++)
	{
		m_datesInfo->AddItem( lUIText( WorldData().GetCurrentUserClub()->FriendlyDateStr(LoopCount) ) );
    m_summaryText( LoopCount * 3 + 0)->SetText( lUIText( WorldData().GetCurrentUserClub()->FriendlyDateStr(LoopCount) ) );
    lLogFmt("%d, %s\n", LoopCount, WorldData().GetCurrentUserClub()->FriendlyDateStr(LoopCount) );
	}

  m_clubsTable->SortByColumn(0, true);
  lLogFmt("FDArrangeFriendlies::Update End\n");
}

void FDArrangeFriendlies::DoShow()
{
  m_arrangeFriendliesScreen->Show();
  m_arrangeFriendliesScreenSub->Show();

  ResetScreenData();
  //m_continueButton->Hide();
}

void FDArrangeFriendlies::DoHide()
{
  m_arrangeFriendliesScreen->Hide();
  m_arrangeFriendliesScreenSub->Hide();
}

void FDArrangeFriendlies::OnTableInfoCombo( FDComboWidget *, int idx )
{
  lLogFmt("Info to show %d\n", idx);
  Update();
}

void FDArrangeFriendlies::OnRowSelect( FDTableWidget *, int row )
{
  lLogFmt("%d row selected\n", row);
  m_confirmButton->Enable();
}

void FDArrangeFriendlies::OnHomeButton( lUIControlWidget * )
{
  m_bHome = true;

  m_homeButton->Highlight(true);
  m_awayButton->Highlight(false);
}

void FDArrangeFriendlies::OnAwayButton( lUIControlWidget * )
{
  m_bHome = false;
  m_homeButton->Highlight(false);
  m_awayButton->Highlight(true);
}

void FDArrangeFriendlies::SetDate( bool bHome )
{
  lLogFmt("Set Date\n");
  int row = m_clubsTable->GetSelectedRow();

  const ClubID wClubID = ClubID(m_clubsTable->GetTagDataForRow(row));
  CClub* pClub = &WorldData().GetClubByID(wClubID);

  m_summaryText( (m_datesInfo->GetSelectedItem()) * 3 + 1)->SetText( lUIText( pClub->GetName() ), true );
  m_summaryText( (m_datesInfo->GetSelectedItem()) * 3 + 2)->SetText( lUIText( bHome? WorldData().GetCurrentUserClub()->GetStadium()->GetName() : pClub->GetStadium()->GetName() ) );

  m_clubKit(m_datesInfo->GetSelectedItem())->Show();
  m_clubKit(m_datesInfo->GetSelectedItem())->GetSprite().Get()->SetFrame( pClub->GetHomeKitID() );

  m_friendly[m_datesInfo->GetSelectedItem()].m_club = wClubID;
  m_friendly[m_datesInfo->GetSelectedItem()].m_home = bHome;
  m_friendly[m_datesInfo->GetSelectedItem()].m_set = true;

}

void FDArrangeFriendlies::OnContinueButton( lUIControlWidget * )
{
  // Accept all dates

  for( int i = 0; i < 4; i++ )
  {
    Friendly &friendly = m_friendly[i];
    if( friendly.m_set )
      WorldData().GetCurrentUserClub()->GetFriendlies().CreateFriendlyMatch(i, friendly.m_club, friendly.m_home ? HOME : AWAY);
  }

  GetMainUI()->SetDialogState( FDMainMenuUI::DIALOG_STATUS_OK );
}

void FDArrangeFriendlies::OnConfirmButton( lUIControlWidget * )
{
  SetDate( m_bHome );

  if( m_datesInfo->GetSelectedItem()+1 > 3 )
  {
  }
  else
    m_datesInfo->SetSelectedItem(m_datesInfo->GetSelectedItem()+1);
}

void FDArrangeFriendlies::OnClearButton( lUIControlWidget * )
{
  if( m_datesInfo->GetSelectedItem() == -1 )
    return;

  int i = m_datesInfo->GetSelectedItem();

  m_friendly[i].m_club.id = NOCLUB;
  m_friendly[i].m_home = false;
  m_friendly[i].m_set = false;

  m_summaryText(i * 3 + 1)->SetText( lUIText( "" ) );
  m_summaryText(i * 3 + 2)->SetText( lUIText( "" ) );
  m_clubKit(m_datesInfo->GetSelectedItem())->Hide();
}

void FDArrangeFriendlies::AutoplaySkip()
{
/*  // Randomly select 4 matches
  for( int i = 0; i < 4; i++ )
  {
    CDivision& theDivision = WorldData().GetDivisionList().GetAt( m_divisions->GetSelectedItem() );
    int randomIdx = rand() % m_clubIdxList.Size();
    bool bHome = (rand() % 1000) > 500 ? true:false;

    CClub* pClub = &theDivision.DoFindClubInTablePosition( m_clubIdxList(randomIdx) );

    lLogFmt("  Match %d: %s - %s\n", i, pClub->GetName(), bHome ? IDS_TAB5003 : IDS_TAB5004 );

    ushort wClubID = theDivision.DoFindClubNumberInTablePosition( m_clubIdxList(randomIdx) );
    WorldData().GetCurrentUserClub()->GetFriendlies().CreateFriendlyMatch(i, wClubID, bHome ? HOME : AWAY);
  }*/

  // 'Press' OK
  OnContinueButton(NULL);
}


