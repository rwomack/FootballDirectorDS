//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdtextwidget.h
// Description : FDTextWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDTEXTWIDGET_H_INCLUDED
#define FDTEXTWIDGET_H_INCLUDED

#include <ui/luiwidget.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTextWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDTextWidget : public lUIWidget
{
  LUI_RTTI( FDTextWidget, lUIWidget );

public:
  static void ClassInitialiser();

  FDTextWidget();
  FDTextWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                 const lUIText & text,
                 const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                 const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                 const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDTextWidget();

  // lUIWidget interface

  virtual void Tick(float delta);
  virtual void Render();
  virtual lUIPoint GetIdealSize() const;

  // FDTextWidget interface

  void            SetText( const lUIText & text );
  const lUIText & GetText();

  void            SetTextColour( luint32_t colour );
  luint32_t       GetTextColour();

  void            SetAlign( lUIAlign align );
  lUIAlign        GetAlign() const;

private:

  lUIText   m_text;
  luint32_t m_colour;
  lUIAlign  m_align;
};


END_L_NAMESPACE
#endif
