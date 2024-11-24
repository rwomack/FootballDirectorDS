//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDNewsscreen.cpp
// Description : FDNewsScreen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.


#include <fdmemory.h>
#include <screens/fdnewsscreen.h>

#include <fdgameworld.h>
#include <fdmainmenuui.h>
#include <fdresourcemanager.h>
#include <fdscript.h>
#include <fdteleprinterwidget.h>

#include <framework/lsprite.h> // hack: to talk to lSprite of lUISpriteWidget

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDNewsScreen
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDNewsScreen::FDNewsScreen( FDMainMenuUI *parent ) : FDScreen( parent )
{
  Init();
}

FDNewsScreen::~FDNewsScreen()
{
}

void FDNewsScreen::Init()
{
  //NOTE: Only intended for each of these classes to be created once so no function set up for freeing them

  m_newsScreen = FD_GAME_NEW lUIScreenWidget( GetMainScreenWidget(), 0, "news_screen" );
  m_newsScreen->Hide();

  m_statusNews = FD_GAME_NEW FDTeleprinterWidget( m_newsScreen, 0, "news",
                      LUI_TOP_LEFT, lUIPoint( 25, 9 ), lUIPoint( 256 - 25, 174 ) );
  m_lastTeleprinterDate = CDate::MIN_DATE;
  m_numberOfStoriesOnLastDate = 0;
  m_lastStoryDate = CDate::MIN_DATE;
}

void FDNewsScreen::DoShow()
{
  m_newsScreen->Show();
}

void FDNewsScreen::DoHide()
{
  m_newsScreen->Hide();
}

void FDNewsScreen::Update()
{
	if (IsForcedUpdateRequested() == false)
		return;

	SetForcedUpdateRequested(false);

	// Get data
	CStringArray strlistNews;
	lArray<bool> userRelatedList;

	CString Date = TEXT("");

	WorldData().GetCurrentUser()->GetNewsList().DoBuildDisplayList(strlistNews, false, Date, true);
	WorldData().GetCurrentUser()->GetNewsList().DoBuildUserRelatedList(userRelatedList, Date);

	m_statusNews->Clear();

	const int iNewsCompleteItemsCount = strlistNews.GetSize() / 2;
	int *pRowsAdded = FD_GAME_NEW int[iNewsCompleteItemsCount];
	for(int i = 0; i < iNewsCompleteItemsCount; i++)
	{
		lLogFmt(" %d: %s\n", i, strlistNews[i]);
		const luint32_t uiRowColour = (userRelatedList(i)) ? L_ARGB( 70, 200,0,0 ) : 0;

		// Skip empty news items.
		if (strlistNews[(i*2)+0].IsEmpty() == false)
			pRowsAdded[i] = m_statusNews->AddItem( lUIText( strlistNews[(i*2)+1] ), lUIText( strlistNews[(i*2)+0] ), uiRowColour );
		else
			pRowsAdded[i] = 0;
	}

	// Find the news item that starts on today, if there is one, and scroll to it.
	CNewsList &cNews = WorldData().GetCurrentUser()->GetNewsList();
	bool bFoundStartDate = false;
	int iRowAccumulator = 0;
	CCalendar cLastDate;
	const CCalendar cCurrentDate = WorldData().GetCurrentDate();
	cLastDate.SetDate(m_lastTeleprinterDate);

	for (int i = 0; i < cNews.GetSize(); i++)
	{
		CNewsItem *pItem = cNews.GetAt(i);
		const CCalendar cNewsItemDate = pItem->GetDate();
		if (cNewsItemDate >= cLastDate)
		{
			if ((cNewsItemDate > cLastDate) || (m_numberOfStoriesOnLastDate == 0) || (cNewsItemDate.GetDate() > m_lastStoryDate))
			{
				m_statusNews->SkipToPosition(iRowAccumulator);
				bFoundStartDate = true;
				break;
			}
			else if (cNewsItemDate == cLastDate)
			{
				m_numberOfStoriesOnLastDate--;
			}
		}

		iRowAccumulator += pRowsAdded[i];
	}

	m_numberOfStoriesOnLastDate = 0;
	for (int i = 0; i < cNews.GetSize(); i++)
	{
		CNewsItem *pItem = cNews.GetAt(i);
		const CCalendar cNewsItemDate = pItem->GetDate();
		if (cNewsItemDate >= cCurrentDate)
		{
			m_numberOfStoriesOnLastDate = cNews.GetSize() - i;
			m_lastStoryDate = cNewsItemDate.GetDate();
			break;
		}
	}

	if (bFoundStartDate == false)
	{
		m_statusNews->SkipToEnd();
	}

	m_lastTeleprinterDate = cCurrentDate.GetDate();
	delete[] pRowsAdded;
}

void FDNewsScreen::ResetTeleprinterDate()
{
	CCalendar cDate = WorldData().GetCurrentDate();
	cDate.PreviousMatchDay();
	m_lastTeleprinterDate = cDate.GetDate();
	m_numberOfStoriesOnLastDate = 0;
	m_lastStoryDate = CDate::MIN_DATE;
}
