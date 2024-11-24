//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdcombowidget.cpp
// Description : FDComboWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdcombowidget.h>
#include <framework/lsprite.h>
#include <fdresourcemanager.h>
#include <fdsound.h>
#include <fdtext.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDComboWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDComboWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void FDComboWidget::ClassInitialiser()
{
}

FDComboWidget::FDComboWidget()
  : m_state( 0 ),
    m_index( 0 ),
    m_bUpper(false)
{
}

FDComboWidget::FDComboWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                              const lUIText & label,
                              const lUIWidgetPosition anchor,
                              const lUIWidgetPosition pos,
                              const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags, id, anchor, pos, size ),
    m_label( label ),
    m_state( 0 ),
    m_index( 0 ),
    m_bUpper(false)
{
  m_left = FDResourceManager::GetSprite("uiicon","combo_left");
  m_right = FDResourceManager::GetSprite("uiicon","combo_right");
  m_middle = FDResourceManager::GetSprite("uiicon","combo_middle");

  m_leftOn = FDResourceManager::GetSprite("uiicon","combo_left_on");
  m_rightOn = FDResourceManager::GetSprite("uiicon","combo_right_on");
}

FDComboWidget::~FDComboWidget()
{
}

lUIPoint FDComboWidget::GetIdealSize() const
{
  lSharedPtr<lFont> font = GetFont();

  lUIPoint size(0,0);

  for( int i=0;i<m_itemList.Size(); i++ )
  {
    int width;
    int height;
    font->GetStringBound( width, height, m_itemList(i).str );

    size.x = lmMax( size.x, float(width) );
  }

  size.y = m_middle->GetHeight();
  size.x += m_left->GetWidth();
  size.x += m_right->GetWidth();

  return size;
}

void FDComboWidget::Render()
{
  lUIRect rect = WidgetToScreen( GetRect() );

  m_left->SetPosition( rect.x, rect.y );
  m_right->SetPosition( rect.x + rect.w - m_right->GetWidth(), rect.y );
  m_leftOn->SetPosition( rect.x, rect.y );
  m_rightOn->SetPosition( rect.x + rect.w - m_right->GetWidth(), rect.y );
  m_middle->SetPosition( rect.x + m_left->GetWidth(), rect.y );
  m_middle->SetWidth( rect.w - ( m_left->GetWidth() + m_right->GetWidth() ) );

  luint32_t textColour = L_ARGB(255,255,255,255);
  luint32_t centreColour = L_ARGB(255,255,255,255);
  luint32_t leftColour = L_ARGB(255, 255, 255, 255);
  luint32_t rightColour = L_ARGB(255, 255, 255, 255);

  m_left->SetColour( leftColour );
  m_right->SetColour( rightColour );
  m_middle->SetColour( centreColour );

  if( m_state & COMBO_DOWN_LEFT )
  {
    m_leftOn->Render();
  } else

  {
    m_left->Render();
  }

  if( m_state & COMBO_DOWN_RIGHT )
  {
    m_rightOn->Render();
  } else

  {
    m_right->Render();
  }

  m_middle->Render();

  lSharedPtr<lFont> font = GetFont();

  if( m_itemList.Size() > 0 )
  {
    const char *pcText = m_itemList( m_index ).str.CStr();
    int width;
    int height;
    font->GetStringBound( width, height, pcText );

	const lUIRect cTextRect = lUIRect(m_middle->GetX(), m_middle->GetY(), m_middle->GetWidth(), m_middle->GetHeight());
	//const lUIRect cTextRect = rect;
    const lUIPoint offset = lUICalcAlign( LUI_CENTER_CENTER, lUIPoint( cTextRect.w, cTextRect.h ), lUIPoint( width, height ) );

    font->SetColour( textColour );
    font->Print( int(cTextRect.x + offset.x),int(cTextRect.y + offset.y + 1), pcText );
  }

  // HACK: label is outside of our rect.
  if( m_label.str.Size() )
  {
    int width;
    int height;
    font->GetStringBound( width, height, m_label.str );
    lUIPoint labelOffset = lUICalcAlign( LUI_CENTER_CENTER, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );

    labelOffset.y = -(height+1);

    font->SetColour( L_ARGB(255,255,255,255) );
    font->Print( int(rect.x+labelOffset.x), int(rect.y+labelOffset.y), m_label.str.CStr() );
  }
}

void FDComboWidget::OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags )
{
  SuperClass::OnMouse( p, mouseDownFlags, mouseUpFlags );

  lUIPoint screenPos = WidgetClientToScreen( p );

  if( m_left->MouseHit( screenPos.x, screenPos.y ) )
  {
    m_state |= COMBO_HOT_LEFT;
  }
  else
  {
    m_state &= ~COMBO_HOT_LEFT;
  }

  if( m_right->MouseHit( screenPos.x, screenPos.y ) )
  {
    m_state |= COMBO_HOT_RIGHT;
  }
  else
  {
    m_state &= ~COMBO_HOT_RIGHT;
  }
}

void FDComboWidget::OnMouseOver()
{
  SuperClass::OnMouseOver();
  m_state |= COMBO_HOT;
}

void FDComboWidget::OnMouseLeave()
{
  SuperClass::OnMouseLeave();
  m_state &= ~(COMBO_HOT | COMBO_HOT_LEFT | COMBO_HOT_RIGHT);
}

bool FDComboWidget::OnMouseDown(const lUIPoint &pos, luint32_t buttonFlags)
{
  SuperClass::OnMouseDown(pos,buttonFlags);
  CaptureMouse();

  lUIPoint screenPos = WidgetClientToScreen( pos );
  if (m_left->MouseHit(screenPos.x, screenPos.y))
	m_state |= COMBO_DOWN | COMBO_DOWN_LEFT;
  else if (m_right->MouseHit(screenPos.x, screenPos.y))
	m_state |= COMBO_DOWN | COMBO_DOWN_RIGHT;

  return true;
}

bool FDComboWidget::OnMouseUp(const lUIPoint &pos, luint32_t buttonFlags)
{
  SuperClass::OnMouseUp(pos,buttonFlags);
  ReleaseMouse();
  m_state &= ~(COMBO_DOWN | COMBO_DOWN_LEFT | COMBO_DOWN_RIGHT);

  if( m_state & COMBO_HOT_LEFT )
  {
    if( m_index == 0 )
    {
      if( m_itemList.Size() > 0 )
      {
        m_index = m_itemList.Size() - 1;
      }
    }
    else
    {
      m_index--;
    }
    SignalSelection( this, m_index );
    FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );
  }
  else if (m_state & COMBO_HOT_RIGHT)
  {
    m_index++;
    if( m_index >= m_itemList.Size() )
    {
      m_index = 0;
    }
    SignalSelection( this, m_index );
    FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );
  }

  return true;
}

void FDComboWidget::ClearAll()
{
  m_itemList.Empty();
}

void FDComboWidget::AddItem( const lUIText & item, bool bClip )
{
  lString str = item.str;

  // Note: Will only work if the widget has been set a size before adding new items
  if( bClip )
  {
    lUIRect rect = WidgetToScreen( GetRect() );
    m_middle->SetPosition( rect.x + m_left->GetWidth(), rect.y );
    m_middle->SetWidth( rect.w - ( m_left->GetWidth() + m_right->GetWidth() ) );
    const lUIRect cTextRect = lUIRect(m_middle->GetX(), m_middle->GetY(), m_middle->GetWidth(), m_middle->GetHeight());


    lSharedPtr<lFont> font = GetFont();

    int Width = 0;
	  int Height = 0;

	  font->GetStringBound( Width, Height, str.CStr() );

	  //lLogFmt("Text Width: %d\nTable width: %d\n", (int) Width, (int) m_columnWidth(col) );


	  if( Width > cTextRect.w)
	  {
	    bool bDone = false;

	    while( !bDone )
	    {
	      font->GetStringBound( Width, Height, str.CStr() );

	      if( Width > cTextRect.w - 5 )
	      {
	        str = str.Left( str.Size()-1 );
	        //lLogFmt("%s\n", str.CStr() );
	        if( str.Size()  == 0 )
	          bDone = true;
	      }
	      else
	      {
	        bDone = true;
        }
	    }
	  }
  }

  m_itemList.Push( lUIText(str.CStr() ));
  if( m_bUpper )
    m_itemList.Last().ToUpper();
}

int FDComboWidget::GetNumItems()
{
  return m_itemList.Size();
}

const lUIText & FDComboWidget::GetItem( int index )
{
  return m_itemList( index );
}

int FDComboWidget::GetSelectedItem()
{
  if( m_itemList.Size() == 0 ) return 0;
  return m_index;
}

void FDComboWidget::SetSelectedItem( int index, bool bDontSignal )
{
  m_index = index;
  if( !bDontSignal )
  {
    SignalSelection( this, m_index );
  }
}

void FDComboWidget::SetAllToUpper( bool bValue )
{
  m_bUpper = bValue;
}




