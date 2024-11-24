//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdteleprinterwidget.cpp
// Description : FDTeleprinterWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdteleprinterwidget.h>
#include <framework/lsprite.h>
#include <fdresourcemanager.h>
#include <fdscrollbarwidget.h>
#include <fdtablewidget.h>
#include <fdtext.h>
#include <fdsound.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDTeleprinterWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTeleprinterWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
void FDTeleprinterWidget::ClassInitialiser()
{
}

//////////////////////////////////////////////////////////////////////////
// Constructor.
//////////////////////////////////////////////////////////////////////////
FDTeleprinterWidget::FDTeleprinterWidget()
{
	Init();
}

//////////////////////////////////////////////////////////////////////////
// Constructor.
//////////////////////////////////////////////////////////////////////////
FDTeleprinterWidget::FDTeleprinterWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                              const lUIWidgetPosition anchor,
                              const lUIWidgetPosition pos,
                              const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags, id, anchor, pos, size )
{
	Init();
}

//////////////////////////////////////////////////////////////////////////
// Destructor.
//////////////////////////////////////////////////////////////////////////
FDTeleprinterWidget::~FDTeleprinterWidget()
{
}

//////////////////////////////////////////////////////////////////////////
// Initialization function for the teleprinter.
//////////////////////////////////////////////////////////////////////////
void FDTeleprinterWidget::Init()
{
	m_table = FD_GAME_NEW FDTableWidget(this, 0, "teleprintertable",
		FDTableWidget::TABLE_FLAG_CELLBACKGROUND | FDTableWidget::TABLE_FLAG_FLAT_CELLBACKGROUND,
    LUI_TOP_LEFT, lUIPoint(0, 0), lUIPoint(GetRect().w - 11, GetRect().h));
  m_scroll = FD_GAME_NEW FDScrollBarWidget(this, 0, "scroll", LUI_TOP_LEFT, lUIPoint(GetRect().w - 11, 0), lUIPoint(11, GetRect().h));

	m_table->AddColumn(lUICoordPercent(1.0f), LUI_TOP_LEFT);
	m_table->SetScrollBar(m_scroll);

	const int iScrollPosition = 0;
	const bool bScrollUp = false;
	m_table->SetScrollPosition(iScrollPosition, bScrollUp);

	m_uiCurrentMessageCharacter = 0;
	m_timer = 0.0f;
	m_iTeleprinterCharType = 0;
	m_bPrintImmediately = false;
}

//////////////////////////////////////////////////////////////////////////
// Tick function used to update the teleprinter.
//////////////////////////////////////////////////////////////////////////
void FDTeleprinterWidget::Tick( float delta )
{
  if( delta > 0.25f )
  {
    delta = 0.25f;
  }

	lUIWidget *pIter = this;
	while (pIter && pIter->IsVisible())
		pIter = pIter->GetParent();
	if (pIter && (pIter->IsVisible() == false))
		return;

	m_timer -= delta;
	if (m_timer <= 0.0f)
	{
		const int CHARACTERS_PER_SECOND = (m_bPrintImmediately ? 9999 : 50);
		const float fOldTimer = m_timer;
		m_timer = (1.0f / CHARACTERS_PER_SECOND);
		bool bPrintedNormalLine = false;

		while ((!m_sCurrentMessage.str.IsEmpty() || m_queuedMessages.Size()) && (m_bPrintImmediately || !bPrintedNormalLine))
		{
			const int iCharactersToRender = lmMax(1, (m_bPrintImmediately ? 9999 : ((int)(1 + (fOldTimer / -(1.0f / CHARACTERS_PER_SECOND))))));

			// Get the next message, if necessary.
			const lString sStrippedOfTags = lString(StripTags(m_sCurrentMessage.str.CStr()));
			if (m_sCurrentMessage.str.IsEmpty() || (m_uiCurrentMessageCharacter >= sStrippedOfTags.Size()))
			{
				if (m_queuedMessages.Size() == 0)
				{
					m_sCurrentMessage = "";
					return;
				}

				m_sCurrentMessage = m_queuedMessages.First();
				m_queuedMessages.Remove(0);

				const luint32_t uiRowColour = m_queuedBackgroundColours.First();
				m_queuedBackgroundColours.Remove(0);

				// Determine if we need to auto-scroll down. (That is, if the current scroll position is at the bottom,
				// and there are enough items to require scrolling.)
				const bool bAutoScroll = ((m_scroll->GetCurrentPosition() + m_scroll->GetItemsViewedAtOnce()) >= m_scroll->GetNumberOfItems());

				m_table->AddRows(1);
				m_table->SetRowBkgColour(m_table->GetRowCount() - 1, uiRowColour);
				m_uiCurrentMessageCharacter = 0;

				if (bAutoScroll)
					m_scroll->SetCurrentPosition(lmMax(0, m_scroll->GetNumberOfItems() - m_scroll->GetItemsViewedAtOnce()), true);
			}

			// Print the next characters of the message.
			lString sCurrentMessage;
			if ((m_uiCurrentMessageCharacter + iCharactersToRender) < sStrippedOfTags.Size())
			{
				if( !m_bPrintImmediately )
				{
					// play every other loop
					static bool bSkip = false;
					if( !bSkip )
						FDSound::PlaySoundEffect( FD_SFX_VIDIPRINTER );
					bSkip = !bSkip;
				}

				sCurrentMessage = sStrippedOfTags.Left(m_uiCurrentMessageCharacter + iCharactersToRender) + GetNextTeleprinterChar();
			}
			else
			{
				sCurrentMessage = m_sCurrentMessage.str;
			}

			if( sCurrentMessage.Right(1) == lString("~") )
			{
				// end of line
				sCurrentMessage = sCurrentMessage.Left( sCurrentMessage.Size() - 1 );

				if( !m_bPrintImmediately )
				{
					m_timer += 0.3f;
				}
			}

			m_table->SetText(m_table->GetRowCount() - 1, 0, lUIText(sCurrentMessage));
			m_uiCurrentMessageCharacter += iCharactersToRender;
			bPrintedNormalLine = true;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Queues up a dated item to be printed.
//////////////////////////////////////////////////////////////////////////
int FDTeleprinterWidget::AddItem(const lUIText &date, const lUIText &text, const luint32_t uiBackgroundColour)
{
	lUIText str(date.str + lString(" - ") + text.str);
	return AddItem(str, uiBackgroundColour);
}

//////////////////////////////////////////////////////////////////////////
// Queues up an item to be printed.
//////////////////////////////////////////////////////////////////////////
int FDTeleprinterWidget::AddItem(const lUIText &text, const luint32_t uiBackgroundColour)
{
	// Line-split the new item.
    lString cSplitText = SplitText(m_table->GetFont().Get(), text.str, m_table->GetRect().w / lSpriteManager::GetDisplayWidth());

    char *pcToken = ::strtok(const_cast<char *>(cSplitText.CStr()), "\n");
	const char *pcPrepend = NULL;
	int iLines = 0;
    while (pcToken != NULL)
    {
      lUIText cTextToAdd;

      if (pcPrepend == NULL)
        cTextToAdd = pcToken;
      else
        cTextToAdd = lUIText(lString(pcPrepend) + lString(pcToken));

      pcPrepend = GetUnclosedTag(cTextToAdd.str.CStr());
      if (pcPrepend != NULL)
      {
        cTextToAdd = lUIText(cTextToAdd.str + "@");
      }

      m_queuedMessages.AddElement(cTextToAdd);
      m_queuedBackgroundColours.AddElement(uiBackgroundColour);

      pcToken = ::strtok(NULL, "\n");

      iLines++;
    }

    if( iLines )
    {
      m_queuedMessages.Last().str += "~";
    }

	return iLines;
}

//////////////////////////////////////////////////////////////////////////
// Clears the teleprinter completely.
//////////////////////////////////////////////////////////////////////////
void FDTeleprinterWidget::Clear()
{
	m_table->RemoveRows(0, m_table->GetRowCount());
	m_queuedMessages.Empty();
	m_queuedBackgroundColours.Empty();
	m_sCurrentMessage = "";

	const int iScrollPosition = 0;
	const bool bScrollUp = false;
	m_table->SetScrollPosition(iScrollPosition, bScrollUp);
}

const char *FDTeleprinterWidget::GetNextTeleprinterChar()
{
	const char *pcReturn;
	switch (m_iTeleprinterCharType)
	{
	case 0:
		pcReturn = ".";
		break;
	case 1:
		pcReturn = "/";
		break;
	case 2:
		pcReturn = "-";
		break;
	case 3:
		pcReturn = "\\";
		break;
	default:
		lAssert(false && "invalid char type");
		break;
	}

	m_iTeleprinterCharType = (m_iTeleprinterCharType + 1) % 4;
	return pcReturn;
}

void FDTeleprinterWidget::SkipToPosition(const int iPosition)
{
	if (m_table->GetRowCount() < iPosition)
	{
		// Put in all table entries up until the requested position.
		int iMessagesToAdd = lmMin((int)(iPosition - m_table->GetRowCount()), m_queuedMessages.Size()) + (m_sCurrentMessage.str.IsEmpty() ? 0 : 1);
		int iRowIndex = lmMax(0, (int)(m_table->GetRowCount() - 1));

		if (m_sCurrentMessage.str.IsEmpty() == false)
		{
			// Finish the current message.
			lString sMessage = m_sCurrentMessage.str;
			if (sMessage[sMessage.Size() - 1] == '~')
				sMessage = sMessage.Left( sMessage.Size() - 1 );

			m_table->SetText(iRowIndex, 0, lUIText(sMessage));
			iRowIndex++;
			iMessagesToAdd--;
			m_sCurrentMessage = "";
		}

		m_table->AddRows(iMessagesToAdd);

		for (int i = 0; i < iMessagesToAdd; i++)
		{
			lString sCurrentMessage = m_queuedMessages(i).str;
			if (sCurrentMessage[sCurrentMessage.Size() - 1] == '~')
				sCurrentMessage = sCurrentMessage.Left( sCurrentMessage.Size() - 1 );

			m_table->SetText(iRowIndex, 0, lUIText(sCurrentMessage));
			m_table->SetRowBkgColour(iRowIndex, m_queuedBackgroundColours(i));
			iRowIndex++;
		}

		m_queuedMessages.Remove(0, iMessagesToAdd);
		m_queuedBackgroundColours.Remove(0, iMessagesToAdd);

		m_table->UpdateScrollBarForChangedText();
	}

	m_scroll->SetCurrentPosition(lmMin(lmMax(0, (int)(m_table->GetRowCount() - m_table->GetScrollBar()->GetItemsViewedAtOnce())), iPosition), true);
}

void FDTeleprinterWidget::SkipToEnd()
{
	SkipToPosition(m_table->GetRowCount() + m_queuedMessages.Size() + (m_sCurrentMessage.str.IsEmpty() ? 0 : 1));

	const int iViewedAtOnce = m_scroll->GetItemsViewedAtOnce();
	const int iNumberOfItems = m_scroll->GetNumberOfItems();

	if (iViewedAtOnce < iNumberOfItems)
		m_scroll->SetCurrentPosition(lmMax(0, iNumberOfItems - iViewedAtOnce), true);
}

void FDTeleprinterWidget::SetPrintImmediately()
{
	m_bPrintImmediately = true;
}
