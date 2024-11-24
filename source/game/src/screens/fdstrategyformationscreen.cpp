//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdstrategyformationscreen.cpp
// Description : Screen for setting the formation used by the squad.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdresourcemanager.h>
#include <fdmainmenuui.h>
#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <screens/fdstrategyformationscreen.h>
#include <screens/fdplayerinfosubscreen.h>
#include <screens/fdsquadscreen.h>
#include <fdplayerblobwidget.h>
#include <fdbuttonwidget.h>
#include <fddragwidget.h>
#include <fdcombowidget.h>
#include <fdscrollbarwidget.h>
#include <fdsquadtablewidget.h>
#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

FDStrategyFormationScreen::FDStrategyFormationScreen( FDMainMenuUI *parent ) : FDScreen(parent)
{
	Init();
}

FDStrategyFormationScreen::~FDStrategyFormationScreen()
{
}

void FDStrategyFormationScreen::Init()
{
	m_formationScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "formation_screen" );
	m_formationScreen->Hide();

	// top screen
	m_formationScreenSub = FD_GAME_NEW lUIScreenWidget( GetSubScreenWidget(), 0, "formation_screen_sub" );
	m_formationScreenSub->Hide();

  m_dragWidget = FD_GAME_NEW FDDragWidget(m_formationScreen, 0, "rolesdrag");

	// Squad table
	m_table = FD_GAME_NEW FDSquadTableWidget( m_formationScreen, 0, "playertable",
        LUI_TOP_LEFT, lUIPoint( 20, 0 ), lUIPoint( 109, 168 ) );
	m_table->SetInfoToShow(FDSquadTableWidget::INFOTOSHOW_MINIMAL );
  m_table->SignalRowSelect.Connect( this, &FDStrategyFormationScreen::OnSignalSelect );

  // Uncomment to enable player dragging on table
  //m_table->GetUnderlyingTable()->SignalCellDropped.Connect(this, &FDStrategyFormationScreen::OnPlayerDropped);
	//m_dragWidget->ConnectToTable(m_table->GetUnderlyingTable());

	m_squadFormation  = FD_GAME_NEW lUISpriteWidget( m_formationScreen, 0, "squadform", FDResourceManager::GetSprite("ui","squad_form"), LUI_TOP_RIGHT, lUIPoint( 256, 0 ) );

	m_squadFormationCombo = FD_GAME_NEW FDComboWidget( m_formationScreen, 0, "formation_combo1", lUIText(""), LUI_CENTER_CENTER, lUIPoint( 202, 167 ) );
  m_squadFormationCombo->SetAllToUpper( true );

	for(int i = 0; i < WorldData().GetCurrentUser()->GetTacticsList().TotalStandardTactics(); i++)
	{
		CTactic& tactic = WorldData().GetCurrentUser()->GetTacticsList().GetAt(i);
		m_squadFormationCombo->AddItem( lUIText( tactic.TitleStr() ) );
	}

	m_squadFormationCombo->SignalSelection.Connect( this, &FDStrategyFormationScreen::OnFormationSelect );

	// add formation blobs on top screen
	for(int i = 0; i < 11; i++)
	{
		char cSpriteName[64];
		//::sprintf(cSpriteName, "playerdot%d", (int)(i + 1));
		FDPlayerBlobWidget *pNewBlob = FD_GAME_NEW FDPlayerBlobWidget( m_formationScreen, LUI_WIDGET_MOUSEHOTFOCUS, "formation",
									//FDResourceManager::GetSprite("ui",cSpriteName), i+2, 0,
									FDResourceManager::GetSprite("formationblobs","formation12"), i+2, 0,
									LUI_CENTER_CENTER, lUIPoint( 0, 0 ) );

		pNewBlob->SignalDropped.Connect(this, &FDStrategyFormationScreen::OnBlobDropped);
		m_playerBlobList.Push( pNewBlob );
	}

	//m_dragWidget->PushToBack();

	m_matchPlay = NULL;
}


void FDStrategyFormationScreen::DoShow()
{
	m_formationScreen->Show();
	m_formationScreenSub->Show();
  GetMainUI()->GetPlayerInfoSubScreen()->Hide();
	GetMainUI()->DisplaySubMessage("Select Player"); //TODO LOCALE
}

void FDStrategyFormationScreen::DoHide()
{
	m_formationScreen->Hide();
	m_formationScreenSub->Hide();

  if( m_matchPlay != NULL )
  {
	  m_matchPlay = NULL;
	  m_table->SetMatchPlay( NULL );
  }
}

void FDStrategyFormationScreen::SetClub(CClub *pClub)
{
	m_pClub = pClub;
	m_table->SetClub(pClub);

	RefreshInformation();
}

void FDStrategyFormationScreen::RefreshInformation()
{
  // select current formation onformation tab
  int idx = WorldData().GetCurrentUser()->GetTacticsList().GetTacticsInUse();

  if( m_matchPlay != NULL )
  {
    if( m_matchPlay->IsUserControlledClub( HOME ) )
      idx = m_matchPlay->GetTacticsId( HOME );
    else
      idx = m_matchPlay->GetTacticsId( AWAY );
  }

  m_squadFormationCombo->SetSelectedItem( idx );

  RefreshFormationDisplay();

  m_table->RefreshTable();
}

//////////////////////////////////////////////////////////////////////////
// Refreshes only the formation display. Called by the combo box selection handler
// as well as the drag-drop handling.
//////////////////////////////////////////////////////////////////////////
void FDStrategyFormationScreen::RefreshFormationDisplay()
{
	// update formations being rendered
	// add formation blobs on top screen

	CTactic * curTactic = NULL;

	if( m_matchPlay != NULL )
	{
	  curTactic = m_matchPlay->GetTactics( m_matchPlay->IsUserControlledClub( HOME ) ? HOME : AWAY );
	}
	else
	{
	  curTactic = &WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTactics();
	}

	lLogFmt("Current Tactics\n");
	lLogFmt("  %s - %d\n", curTactic->TitleStr(), curTactic->GetFamiliarity());

  // Sprite indices
	int spriteFrame[] = { 1, 6,	12 };


	lArray<int> formation;
	int curX = 0;

	for(int i = 0; i < 10; i++)
	{
		if( curTactic->GetPlayerPositionX(i) != curX )
		{
			curX = curTactic->GetPlayerPositionX(i);
			formation.Push( 0 );
		}

		formation.Last()++;
  }

  m_playerBlobList(0)->GetSprite()->SetFrame( 0 ); // goalie

  curX = 1;
  int nextInc = 0;
	for( int i = 0; i < 3; i++ )
	{
	  for( int x = 0; x < formation(i); x++ )
	  {
	    m_playerBlobList(curX++)->GetSprite()->SetFrame( spriteFrame[i] + x + nextInc );
	  }

	  nextInc += formation(i) - 3;
	}

	int totalPlayers = 0;
	for(int i = 0; i < formation.Size(); i++)
	{
		lLogFmt("%d: %d\n", i, formation(i));
		totalPlayers += formation(i);
	}

	if( totalPlayers > 10 )
	{
		lLogFmt("ERROR: %d players listed in formation (10 max)\n");
	}

	// positions for formations
	lArray<float> formationXPos;
	formationXPos.Push( 0.224f );
	formationXPos.Push( 0.406f );
	formationXPos.Push( 0.500f );
	formationXPos.Push( 0.626f );
	formationXPos.Push( 0.774f );

	lmVector2 xMin( 0, 14 );
	lmVector2 xMax( 0, 145 );

	// up to 5 players per line
	float formationYPos[] = { 0.5f,  0.0f,  0.0f,  0.0f,  0.0f,  // 1 player
								0.2f,  0.8f,  0.0f,  0.0f,  0.0f,  // 2 players
								0.2f, 0.5f, 0.8f,  0.0f,  0.0f,  // 3 players
								0.1f,  0.35f,  0.65f,  0.9f,  0.0f,  // 4 players
								0.1f,  0.3f,  0.5f,  0.7f,  0.9f,  // 5 players
							};

	lmVector2 yMin( 159, 0 );
	lmVector2 yMax( 245, 0 );

	// position players based on formation passed in
	int numLines = formation.Size();
	if( numLines > formationXPos.Size() )
	{
		lLogFmt("ERROR: Can't represent formation using the formation lines we have\n");
	}

	lArray<int>     lineIndex;
	lineIndex.SetSize( numLines );

	if( numLines == 1 )
	{
		lineIndex(0) = 2;
	}
	else if( numLines == 2 )
	{
		lineIndex(0) = 0;
		lineIndex(1) = 2;
	}
	else if( numLines == 3 )
	{
		lineIndex(0) = 0;
		lineIndex(1) = 2;
		lineIndex(2) = 4;
	}
	else if( numLines == 4 )
	{
		lineIndex(0) = 0;
		lineIndex(1) = 1;
		lineIndex(2) = 3;
		lineIndex(3) = 4;
	}

	// 1 Green - Goalkeeper
	// 2 - 5 Purple - Defender
	// 5 - 10 Red - Midfielder
	// 8 - 11 Blue - Attacker
	// s1 - s7 Yellow - Subs

	int playerIndex = 0;

	// goalie
  m_playerBlobList(playerIndex)->Move( lUIPoint( 202, 12 ) );
  playerIndex++;

	for(int i = 0; i < formation.Size(); i++)
	{
		for(int j = 0; j < formation(i); j++)
		{
			lmVector2 pos( 0, 0 );
			pos += xMin + ((xMax - xMin) * formationXPos( lineIndex( i ) ));
			pos += yMin + ((yMax - yMin) * formationYPos[ ((formation(i)-1) * 5) + j ]);

			m_playerBlobList(playerIndex)->Move( lUIPoint( (int)pos(0), (int)pos(1) ) );

			playerIndex++;
		}
	}
}

void FDStrategyFormationScreen::OnFormationSelect( FDComboWidget *, int idx )
{
	lLogFmt("Formation select %d\n", idx);

	if( m_matchPlay != NULL )
  {
    if( m_matchPlay->IsUserControlledClub( HOME ) )
      m_matchPlay->DoFormationChanged( HOME, idx );
    else
      m_matchPlay->DoFormationChanged( AWAY, idx );
  }
  else
  {
    WorldData().GetCurrentUser()->GetTacticsList().SetTacticsInUse( idx );
  }

	//TODOWorldData().GetCurrentUser()->GetTacticsList().DoBuildTitleAndFamiliarityStrList(pViewMode);
  GetMainUI()->GetSquadScreen()->SetForcedUpdateRequested(true);
	RefreshFormationDisplay();

	// Reset the table, as player assignments may have changed
	m_table->SetClub(m_pClub);
}

void FDStrategyFormationScreen::OnBlobDropped(FDPlayerBlobWidget *pBlob, lUIPoint cDropLocation)
{
	lLogFmt("Blob dropped at %10.6f, %10.6f\n", cDropLocation.x, cDropLocation.y);

  int iRow = -1;
  int newRow = -1;

  lUIRect rect( cDropLocation.x, cDropLocation.y, 1, 1 );

  for( int i = 0; i < 11; i++ )
  {
    if( m_playerBlobList(i) == pBlob )
      iRow = i; // + header


    if( rect.InRect( m_playerBlobList(i)->GetRect() ) )
    {
      newRow = i;
    }
  }


  lLogFmt(" %d Selected Row, %d Dropped onto row\n", iRow, newRow);

  int prevScrollPosition = m_table->GetScrollBar()->GetCurrentPosition();

  // Didn't drop on another blob
  if( iRow != -1 && newRow != -1 )
  {
    // swap players
    if( m_matchPlay != NULL )
      m_matchPlay->SwapPlayersOnPitch( m_matchPlay->IsUserControlledClub(HOME) ? HOME : AWAY, iRow, newRow );
    else
      m_pClub->GetFirstTeamSquad().DoSwapPlayers( iRow, newRow );

    if( m_matchPlay != NULL )
      m_matchPlay->SetTacticsChanged();

    if( iRow != newRow )
      RefreshInformation();

    GetMainUI()->GetSquadScreen()->SetForcedUpdateRequested(true);
    m_table->GetScrollBar()->SetCurrentPosition( prevScrollPosition, true );
    return;
  }



  ePlayerPosition ePlayerDragged;
	if (pBlob->GetRect().y < 58)
		ePlayerDragged = DEFENDER;
	else if (pBlob->GetRect().y <= 100)
		ePlayerDragged = MIDFIELDER;
	else
		ePlayerDragged = ATTACKER;

	ePlayerPosition eDropType;
	if (cDropLocation.y < 58)
		eDropType = DEFENDER;
	else if (cDropLocation.y <= 100)
		eDropType = MIDFIELDER;
	else
		eDropType = ATTACKER;

	// If they're not changing position, we don't need to do anything else.
	if (ePlayerDragged == eDropType || m_playerBlobList(0) == pBlob )
		return;


	CTacticList &cTacticList = WorldData().GetCurrentUser()->GetTacticsList();

	CTactic * curTactic = NULL;

	if( m_matchPlay != NULL )
	{
	  curTactic = m_matchPlay->GetTactics( m_matchPlay->IsUserControlledClub( HOME ) ? HOME : AWAY );
  }
  else
  {
    curTactic = &cTacticList.GetCurrentTactics();
  }

	int iCurrentPlayerTypes[MAXPLAYERPOSITIONS];
	int iNewPlayerTypes[MAXPLAYERPOSITIONS];
	for (int i = 0; i < (int)MAXPLAYERPOSITIONS; i++)
	{
		iCurrentPlayerTypes[i] = curTactic->GetNumberOfPlayersAtPosition((ePlayerPosition)i);
		iNewPlayerTypes[i] = iCurrentPlayerTypes[i];
	}

	iNewPlayerTypes[ePlayerDragged]--;
	iNewPlayerTypes[eDropType]++;

	// Now, find the appropriate tactic, if available, and switch to it.
	for (int i = 0; i < cTacticList.GetSize(); i++)
	{
		CTactic &cCandidate = cTacticList.GetAt(i);
		bool bCorrectTactic = true;

		for (int j = 0; j < (int)MAXPLAYERPOSITIONS; j++)
		{
			if (cCandidate.GetNumberOfPlayersAtPosition((ePlayerPosition)j) != iNewPlayerTypes[j])
			{
				bCorrectTactic = false;
				break;
			}
		}

		if (bCorrectTactic == true)
		{
			m_squadFormationCombo->SetSelectedItem(i);

			if( m_matchPlay != NULL )
      {
        if( m_matchPlay->IsUserControlledClub( HOME ) )
          m_matchPlay->DoFormationChanged( HOME, i );
        else
          m_matchPlay->DoFormationChanged( AWAY, i );
      }
      else
      {
        WorldData().GetCurrentUser()->GetTacticsList().SetTacticsInUse( i );
      }

			break;
		}
	}

	// find out where blob was dropped to determine what to swap it with
	int blobBefore = 0;

  switch( eDropType-1 )
  {
    default:
      blobBefore = 0;
      break;
    case ATTACKER:
      lLogFmt("ATTACKER ");
      blobBefore += iNewPlayerTypes[ATTACKER];
    case MIDFIELDER:
    lLogFmt("MIDFIELDer ");
      blobBefore += iNewPlayerTypes[MIDFIELDER];
    case DEFENDER:
    lLogFmt("DEFENDer ");
      blobBefore += iNewPlayerTypes[DEFENDER];
  }

  int between = -1;
  for( int i = 0; i < iCurrentPlayerTypes[eDropType]; i++ )
  {
    lUIRect rect = m_playerBlobList( blobBefore + i + 1)->GetRect();
    lUIRect rect2 = m_playerBlobList( blobBefore + i )->GetRect();

    if( cDropLocation.x > rect.x &&  cDropLocation.x < rect2.x )
    {
      between = iCurrentPlayerTypes[eDropType] - (i + 1);
      break;
    } else if( cDropLocation.x > rect.x + rect.w )
    {
      between = iCurrentPlayerTypes[eDropType] - i;
      break;
    }

    between = iCurrentPlayerTypes[eDropType] - i - 1;
  }

  lLogFmt("Blob dropped after %d, %d, %d\n", between, blobBefore + between, iRow);

  // swap
  if( blobBefore + between < iRow )
  {
    for(int i = 0; i < ((iRow-1) - (blobBefore + between) ); i++ )
    {
      if( m_matchPlay != NULL )
        m_matchPlay->SwapPlayersOnPitch( m_matchPlay->IsUserControlledClub(HOME) ? HOME : AWAY, (iRow-1) - i, (iRow-1) - (i - 1) );
      else
        m_pClub->GetFirstTeamSquad().DoSwapPlayers( (iRow-1) - i, (iRow-1) - (i - 1) );
    }
  } else
  {
    for(int i = 0; i < (blobBefore + between - iRow) + 1; i++ )
    {
      if( m_matchPlay != NULL )
        m_matchPlay->SwapPlayersOnPitch( m_matchPlay->IsUserControlledClub(HOME) ? HOME : AWAY, iRow + i, iRow + i + 1 );
      else
        m_pClub->GetFirstTeamSquad().DoSwapPlayers( iRow + i, iRow + i + 1 );
    }
  }

  if( m_matchPlay != NULL )
    m_matchPlay->SetTacticsChanged();

  RefreshInformation();

  GetMainUI()->GetSquadScreen()->SetForcedUpdateRequested(true);

	m_table->GetScrollBar()->SetCurrentPosition( prevScrollPosition, true );

}

void FDStrategyFormationScreen::OnSignalSelect( FDTableWidget * table, CPlayer *pPlayer, int row)
{
  GetMainUI()->HideSubMessage();

  if( pPlayer != NULL )
  {
    lLogFmt("Player: %d - %s\n", row, pPlayer->GetName());
    GetMainUI()->GetPlayerInfoSubScreen()->SetPlayerInfo( pPlayer );
    GetMainUI()->GetPlayerInfoSubScreen()->Show();
  }
  else
  {
    lLogFmt("Not a valid playeridx!\n");
    GetMainUI()->GetPlayerInfoSubScreen()->Hide();
    GetMainUI()->DisplaySubMessage("Select Player");
  }
}

void FDStrategyFormationScreen::OnPlayerDropped(FDTableWidget *pTable, int iRow, int iCol, lUIPoint pos)
{
  int prevScrollPosition = pTable->GetScrollBar()->GetCurrentPosition();
  int newRow = ( ( pos.y - pTable->GetRect().y ) / 12 ) + pTable->GetScrollBar()->GetCurrentPosition();

	if ( iRow == 0 || iRow > pTable->GetRowCount()-1  || newRow == 0 || newRow > pTable->GetRowCount()-1 ||
      pos.y > (pTable->GetRect().y + pTable->GetRect().h) )
		return;

  lLogFmt(" %d Selected Row, %d Dropped onto row\n", iRow, newRow);

  // swap players
  m_pClub->GetFirstTeamSquad().DoSwapPlayers( iRow-1, newRow-1 );
  if( iRow != newRow )
    RefreshInformation();

  GetMainUI()->GetSquadScreen()->SetForcedUpdateRequested(true);

  pTable->GetScrollBar()->SetCurrentPosition( prevScrollPosition, true );
}

void FDStrategyFormationScreen::SetMatchPlay( CMatchPlay *pMatchPlay )
{
  m_matchPlay = pMatchPlay;
  m_table->SetMatchPlay( m_matchPlay );
}
