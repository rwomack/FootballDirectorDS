//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdhighlightwidget.h
// Description : FDTickerWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDHIGHLIGHTWIDGETUI_H_INCLUDED
#define FDHIGHLIGHTWIDGETUI_H_INCLUDED

#include <ui/luiwidget.h>
#include <framework/lsigslot.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDHighlightWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDHighlightWidget : public lUIWidget
{
  LUI_RTTI( FDHighlightWidget, lUIWidget );

public:
  static void ClassInitialiser();

  FDHighlightWidget();
  FDHighlightWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                      luint32_t   colour,
                      const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                      const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                      const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDHighlightWidget();

  // lUIWidget interface

  virtual void     Render();
  virtual void     Tick( float delta );

  virtual lUIPoint GetIdealSize() const;

  // FDHighlightWidget interface

  void            SetColour( luint32_t colour );
  luint32_t       GetColour();

  void            SetRect( float x, float y, float w, float h );


private:
  luint32_t m_colour;
  float     m_x;
  float     m_y;
  float     m_w;
  float     m_h;
};

END_L_NAMESPACE
#endif

