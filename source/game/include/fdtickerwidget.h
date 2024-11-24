//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdtickerwidget.h
// Description : FDTickerWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDTICKERWIDGETUI_H_INCLUDED
#define FDTICKERWIDGETUI_H_INCLUDED

#include <ui/luiwidget.h>
#include <framework/lsigslot.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTickerWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDTickerWidget : public lUIWidget
{
  LUI_RTTI( FDTickerWidget, lUIWidget );

public:
  static void ClassInitialiser();

  FDTickerWidget();
  FDTickerWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                 const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                 const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                 const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDTickerWidget();

  // lUIWidget interface

  virtual void     Render();
  virtual void     Tick( float delta );

  virtual lUIPoint GetIdealSize() const;

  // FDTickerWidget interface

  void            AddItem( const lUIText & );
  int             GetNumItems();
  const lUIText & GetItem( int index );

  void            SetColour( int colour );

private:

  lSharedPtr<lSprite>   m_dot;

  struct Item
  {
    lUIText text;
  };

  lArray< Item >        m_itemList;
  luint32_t             m_index;
  float                 m_offset;
  luint32_t             m_colour;

};

END_L_NAMESPACE
#endif

