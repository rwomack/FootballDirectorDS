//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDMatchDayCommentaryscreen.cpp
// Description : FDMatchDayCommentaryScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdgameworld.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdscrollbarwidget.h>
#include <fdteleprinterwidget.h>

#include <screens/fdmatchdayscreen.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDMatchDayCommentaryScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDMatchDayCommentaryScreen::FDMatchDayCommentaryScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDMatchDayCommentaryScreen::~FDMatchDayCommentaryScreen()
{
}

void FDMatchDayCommentaryScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  int colA = L_ARGB( 255, 29, 9, 208 );
  int colB = L_ARGB( 255, 211, 108, 5 );

  lLogFmt("Commentary screen\n");
  m_matchCommentaryScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "match_comm_screen" );
  m_matchCommentaryScreen->Hide();

  m_commtable = FD_GAME_NEW FDTeleprinterWidget( m_matchCommentaryScreen, 0, "commtable", LUI_TOP_LEFT, lUIPoint( 25, 9 ), lUIPoint( 256 - 25, 174 ) );
  m_commtable->SetPrintImmediately();
}

void FDMatchDayCommentaryScreen::Clear()
{
  m_commtable->Clear();
}

void FDMatchDayCommentaryScreen::Update()
{
}

void FDMatchDayCommentaryScreen::AddString( const CString &str, unsigned int colour )
{
  colour = 0x70000000 | (colour ^ 0xFF000000 );
  m_commtable->AddItem( lUIText( str ), colour );
}


void FDMatchDayCommentaryScreen::DoShow()
{
  m_matchCommentaryScreen->Show();
}

void FDMatchDayCommentaryScreen::DoHide()
{
  m_matchCommentaryScreen->Hide();
}

//////////////////////////////////////////////////////////////////////////
// Used to advance the commentary screen to the end, no matter where it is.
// Used when an exciting event happens.
//////////////////////////////////////////////////////////////////////////
void FDMatchDayCommentaryScreen::SkipToEnd()
{
	m_commtable->SkipToEnd();
}
