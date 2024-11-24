//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdkeyboardwidget.cpp
// Description : A widget that encapsulates an onscreen keyboard.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdkeyboardwidget.h>
#include <fdbuttonwidget.h>
#include <fdresourcemanager.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI(FDKeyboardWidget, lGUID(0, 0, 0, 0), 0);

void FDKeyboardWidget::ClassInitialiser()
{
}

FDKeyboardWidget::FDKeyboardWidget()
{
}

FDKeyboardWidget::FDKeyboardWidget( lUIWidget * parent, luint32_t widgetFlags,
								   const char * id,
								   const lUIWidgetPosition anchor,
								   const lUIWidgetPosition pos,
								   const lUIWidgetSize size) :
	lUIWidget(parent, widgetFlags, id, anchor, pos, size)
{
	Init();
}

FDKeyboardWidget::~FDKeyboardWidget()
{
}

void FDKeyboardWidget::Init()
{
	// Add keyboard
	int row = 0;
	int width = 20;
	int offsetx = 0;
	int offsety = 0;
	int spacex;

	// base keys
	m_Keys.Add(26);

	m_Keys(0).letter = 'Q';
	m_Keys(1).letter = 'W';
	m_Keys(2).letter = 'E';
	m_Keys(3).letter = 'R';
	m_Keys(4).letter = 'T';
	m_Keys(5).letter = 'Y';
	m_Keys(6).letter = 'U';
	m_Keys(7).letter = 'I';
	m_Keys(8).letter = 'O';
	m_Keys(9).letter = 'P';

	for( int i = 0; i < 10; i++ )
	{
		char keystr[4];
		sprintf( keystr, "%c", m_Keys(i).letter);
		m_Keys(i).widget = FD_GAME_NEW FDButtonWidget( this, 0, keystr, FDResourceManager::GetSprite("uiicon","key"),
													lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( offsetx + i * width, offsety + row * width ) );
		m_Keys(i).widget->SetText( lUIText( keystr ) );
	}

	row++;
	m_Keys(10).letter = 'A';
	m_Keys(11).letter = 'S';
	m_Keys(12).letter = 'D';
	m_Keys(13).letter = 'F';
	m_Keys(14).letter = 'G';
	m_Keys(15).letter = 'H';
	m_Keys(16).letter = 'J';
	m_Keys(17).letter = 'K';
	m_Keys(18).letter = 'L';

	for( int i = 10; i < 19; i++ )
	{
		int x = offsetx + 12 + (i-10) * width;

		char keystr[4];
		sprintf( keystr, "%c", m_Keys(i).letter);
		m_Keys(i).widget = FD_GAME_NEW FDButtonWidget( this, 0, keystr, FDResourceManager::GetSprite("uiicon","key"),
													lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( x, offsety + row * width ) );
		m_Keys(i).widget->SetText( lUIText( keystr ) );
	}

	row++;
	m_Keys(19).letter = 'Z';
	m_Keys(20).letter = 'X';
	m_Keys(21).letter = 'C';
	m_Keys(22).letter = 'V';
	m_Keys(23).letter = 'B';
	m_Keys(24).letter = 'N';
	m_Keys(25).letter = 'M';

	for( int i = 19; i < 26; i++ )
	{
		int x = offsetx + 24 + (i-19) * width;

		char keystr[4];
		sprintf( keystr, "%c", m_Keys(i).letter);
		m_Keys(i).widget = FD_GAME_NEW FDButtonWidget( this, 0, keystr, FDResourceManager::GetSprite("uiicon","key"),
													lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( x, offsety + row * width ) );
		m_Keys(i).widget->SetText( lUIText( keystr ) );

		if( i == 20 )
		{
			spacex = x;
		}
	}

	row++;

	// space bar
	{
		char keystr[] = "space";

		m_Keys.Add( 1 );
		m_Keys.Last().letter = ' ';
		m_Keys.Last().widget = FD_GAME_NEW FDButtonWidget( this, 0, keystr, FDResourceManager::GetSprite("ui","spacebar"),
													lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( spacex, offsety + row * width ) );
	}

	// backspace
	{
		char keystr[] = "bs";

		int backspaceIdx = m_Keys.Add( 1 );
		m_Keys.Last().letter = 8;
		m_Keys.Last().widget = FD_GAME_NEW FDButtonWidget( this, 0, keystr, FDResourceManager::GetSprite("ui","backspace"),
													lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( spacex + 130, offsety + (row) * width ) );
	}

	m_DesiredSize.x = 0;
	m_DesiredSize.y = 0;

	for(int i = 0; i < m_Keys.Size(); i++)
	{
		m_Keys(i).widget->SignalControlCommand.Connect( this, &FDKeyboardWidget::OnKeyboard );

		const lUIRect &cRect = m_Keys(i).widget->GetRect();
		m_DesiredSize.x = lmMax(m_DesiredSize.x, cRect.x + cRect.w);
		m_DesiredSize.y = lmMax(m_DesiredSize.y, cRect.y + cRect.h);
	}

	// Caps lock
	m_capsLock = FD_GAME_NEW FDButtonWidget( this, 0, "cl", FDResourceManager::GetSprite("uiicon","key"),
													lUIButtonWidget::BUTTON_FLAG_STYLUS_ONECLICK, LUI_TOP_LEFT, lUIPoint( spacex - 30, offsety + row * width ) );
	m_capsLock->SetText( lUIText( "^" ) );
	m_capsLock->SignalControlCommand.Connect( this, &FDKeyboardWidget::OnKeyboard );

	const lUIRect &cCapsLockRect = m_capsLock->GetRect();
	m_DesiredSize.x = lmMax(m_DesiredSize.x, cCapsLockRect.x + cCapsLockRect.w);
	m_DesiredSize.y = lmMax(m_DesiredSize.y, cCapsLockRect.y + cCapsLockRect.h);

  m_bCaps = true;
}

lUIPoint FDKeyboardWidget::GetIdealSize() const
{
	return m_DesiredSize;
}

void FDKeyboardWidget::Render()
{
	SuperClass::Render();
}

void FDKeyboardWidget::OnKeyboard( lUIControlWidget *widget )
{
	for(int i = 0; i < m_Keys.Size(); i++)
	{
		if( m_Keys(i).widget == widget )
		{
			lLogFmt("Key '%c' pressed\n", m_Keys(i).letter);
			SignalKeyPressed.Emit(m_Keys(i).letter);

			// Auto-drop case.
      if (m_bCaps)
      {
        if( m_Keys(i).letter != ' ' && m_Keys(i).letter != 8 )
        {
          // if not space or backspace
          OnKeyboard(m_capsLock);
        }
      }

			break;
		}
	}

  // Check if it was caps lock that was pressed
	if( m_capsLock == widget )
	{
    m_bCaps = !m_bCaps;

		for( int i = 0; i < 26; i++)
		{
			char letter = m_Keys(i).letter;

      if( m_bCaps )
				letter = letter - 'a' + 'A';
			else
				letter = letter - 'A' + 'a';

			m_Keys(i).letter = letter;
			char keystr[4];
			sprintf( keystr, "%c", m_Keys(i).letter);
			m_Keys(i).widget->SetText(lUIText(keystr));
		}
	}
}

void  FDKeyboardWidget::EnableCapsLock( bool value )
{
  if( value && !m_bCaps )
  {
    OnKeyboard( m_capsLock );
  }
  else if( !value && m_bCaps )
  {
    OnKeyboard( m_capsLock );
  }
}
