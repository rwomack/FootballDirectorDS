//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdtickerwidget.cpp
// Description : FDTickerWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdtickerwidget.h>
#include <framework/lsprite.h>
#include <fdresourcemanager.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( FDTickerWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTickerWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void FDTickerWidget::ClassInitialiser()
{
}

FDTickerWidget::FDTickerWidget()
  : m_index( 0 ),
    m_offset( 0.0f ),
    m_colour( L_ARGB(255,255,255,255) )
{
}

FDTickerWidget::FDTickerWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                              const lUIWidgetPosition anchor,
                              const lUIWidgetPosition pos,
                              const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags, id, anchor, pos, size ),
    m_index( 0 ),
    m_offset( 0.0f ),
    m_colour( L_ARGB(255,255,255,255) )
{
  // TODO:
  //m_dot = FDResourceManager::GetSprite("ui","ticker_dot");
  // hack.

  AddItem( lUIText(". Ticker Test 1") );
  AddItem( lUIText(". Ticker Test 2") );
  AddItem( lUIText(". Ticker Test 3") );
  AddItem( lUIText(". Ticker Test 4") );

}

FDTickerWidget::~FDTickerWidget()
{
}

lUIPoint FDTickerWidget::GetIdealSize() const
{
  lSharedPtr<lFont> font = GetFont();
  return lUIPoint( lSpriteManager::GetDisplayWidth(), font->DefaultPixelHeight() );
}

void FDTickerWidget::Render()
{
  lUIRect rect = WidgetToScreen( GetRect() );
  lUIPoint pos( rect.x, rect.y );

  pos.x += m_offset;

  lSharedPtr<lFont> font = GetFont();
  font->SetColour( m_colour );

  if( m_itemList.Size() > 0 )
  {
    luint32_t index = m_index;

    while( 1 )
    {
      Item * item = &m_itemList( index );

      int width;
      int height;
      font->GetStringBound( width, height, item->text.str );

      font->Print( int(pos.x), int(pos.y), item->text.str );

      pos.x += width;
      pos.x += (font->DefaultPixelWidth() / 3.0f);

      if( pos.x > (rect.x + rect.w) )
      {
        break;
      }

      index++;
      if( index >= m_itemList.Size() )
      {
        index = 0;
      }

      if( index == m_index )
      {
        break;
      }
    }
  }



  // HACK: label is outside of our rect.

  //int width;
  //int height;
  //font->GetStringBound( width, height, m_label.str );
  //lUIPoint labelOffset = lUICalcAlign( LUI_CENTER_CENTER, lUIPoint( rect.w, rect.h ), lUIPoint( width, height ) );
  //
  //labelOffset.y = -(height+1);
  //
  //font->SetColour( L_ARGB(255,255,255,255) );
  //font->Print( int(rect.x+labelOffset.x), int(rect.y+labelOffset.y), m_label.str );
}

void FDTickerWidget::Tick( float delta )
{
  m_offset -= delta * 20.0f;

  // update ticker index

  lSharedPtr<lFont> font = GetFont();

  lUIRect rect = WidgetToScreen( GetRect() );
  lUIPoint pos( rect.x, rect.y );

  pos.x += m_offset;

  if( m_itemList.Size() > 0 )
  {
    Item * item = &m_itemList( m_index );

    int width;
    int height;
    font->GetStringBound( width, height, item->text.str );

    pos.x += width;
    pos.x += (font->DefaultPixelWidth() / 3.0f);

    if( pos.x <= 0 )
    {
      m_index++;

      if( m_index >= m_itemList.Size() )
      {
        m_index = 0;
      }

      m_offset += float( width + (font->DefaultPixelWidth() / 3.0f) );

      // TODO: handle case where next ticker items don't fit into entire screen,
      // so the offset would cause them to 'pop' in.
    }
  }
}


void FDTickerWidget::AddItem( const lUIText & item )
{
  int index = m_itemList.Add();
  m_itemList(index).text = item;
}

int FDTickerWidget::GetNumItems()
{
  return m_itemList.Size();
}

const lUIText & FDTickerWidget::GetItem( int index )
{
  return m_itemList( index ).text;
}


void FDTickerWidget::SetColour( int colour )
{
  m_colour = colour;
}

