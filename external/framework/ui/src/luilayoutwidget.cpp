//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : luilayoutwidget.cpp
// Description : lUILayoutWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <ui/luilayoutwidget.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( lUILayoutWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUILayoutWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void lUILayoutWidget::ClassInitialiser()
{
}

void lUILayoutWidget::SetSpacer( const lUIWidgetSizeCoord & size )
{
  m_spacer = size;
}

void lUILayoutWidget::SetAlign( lUIAlignAxis align )
{
  m_align = align;
}

lUILayoutWidget::lUILayoutWidget()
{
}

lUILayoutWidget::lUILayoutWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                                  luint32_t layoutFlags,
                                  const lUIWidgetPosition anchor,
                                  const lUIWidgetPosition pos,
                                  const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags, id, anchor, pos, size ),
    m_layoutFlags( layoutFlags ),
    m_spacer( lUICoordPercent( 4 ) ),
    m_align( LUI_CENTER )
{
}

lUILayoutWidget::~lUILayoutWidget()
{
}

lUIPoint lUILayoutWidget::GetIdealSize() const
{
  return PixelSize( lUIPointPercent(100,100) );
}


void lUILayoutWidget::Tick(float delta)
{
  lUIRect rect    = GetRect();
  float   space   = 0.0f;
  lUIPoint pos(0,0);

  if( m_layoutFlags & LUI_LAYOUT_VERTICAL )
  {
    space = PixelSizeY( m_spacer );
  }
  else
  {
    space = PixelSizeX( m_spacer );
  }


  float totalSize = 0;
  int   numItems  = 0;

  if( (m_layoutFlags & LUI_LAYOUT_CENTER) || (m_layoutFlags & LUI_LAYOUT_EVEN) )
  {
    lUIWidget * widget = FirstChild();

    while( widget )
    {
      lUIRect childRect = widget->GetRect();

      if( m_layoutFlags & LUI_LAYOUT_VERTICAL )
      {
        totalSize += childRect.h;
        totalSize += space;
      }
      else
      {
        totalSize += childRect.w;
        totalSize += space;
      }

      widget = widget->NextSibling();
      numItems++;
    }

    if( m_layoutFlags & LUI_LAYOUT_EVEN )
    {
      if( numItems > 1 )
      {
        if( m_layoutFlags & LUI_LAYOUT_VERTICAL )
        {
          if( m_layoutFlags & LUI_LAYOUT_CENTER )
          {
            space = (rect.h - totalSize) / (numItems + 1);
            totalSize += space * (numItems + 1);
            pos.y += space;
          }
          else
          {
            space = (rect.h - totalSize) / (numItems - 1);
            totalSize += space * (numItems - 1);
          }
        }
        else
        {
          if( m_layoutFlags & LUI_LAYOUT_CENTER )
          {
            space = (rect.w - totalSize) / (numItems + 1);
            totalSize += space * (numItems + 1);
            pos.x += space;
          }
          else
          {
            space = (rect.w - totalSize) / (numItems - 1);
            totalSize += space * (numItems - 1);

          }
        }
      }
    }
    else if( m_layoutFlags & LUI_LAYOUT_CENTER )
    {
      if( m_layoutFlags & LUI_LAYOUT_VERTICAL )
      {
        pos.y += (rect.h - totalSize) * 0.5f;
      }
      else
      {
        pos.x += (rect.w - totalSize) * 0.5f;
      }
    }
  }

  lUIWidget * widget = FirstChild();

  while( widget )
  {
    lUIRect childRect = widget->GetRect();

    if( m_layoutFlags & LUI_LAYOUT_VERTICAL )
    {
      if( m_align == LUI_CENTER )
      {
        pos.x = (rect.w - childRect.w) * 0.5f;
      }
      else if( m_align == LUI_LEFT )
      {
        pos.x = 0;
      }
      else
      {
        pos.x = rect.w - childRect.w;
      }

      widget->Move( pos );
      pos.y += childRect.h;
      pos.y += space;
    }
    else
    {
      if( m_align == LUI_CENTER )
      {
        pos.y = (rect.h - childRect.h) * 0.5f;
      }
      else if( m_align == LUI_LEFT )
      {
        pos.y = 0;
      }
      else
      {
        pos.y = rect.h - childRect.h;
      }

      widget->Move( pos );
      pos.x += childRect.w;
      pos.x += space;
    }


    widget = widget->NextSibling();
  }
}


