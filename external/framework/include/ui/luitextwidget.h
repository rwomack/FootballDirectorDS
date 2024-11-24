//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : luitextwidget.h
// Description : lUITextWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LUITEXTWIDGET_H_INCLUDED
#define LUITEXTWIDGET_H_INCLUDED

#include "./luiwidget.h"

BEGIN_L_NAMESPACE

// Pre-declared classes

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUITextWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lUITextWidget : public lUIWidget
{
  LUI_RTTI( lUITextWidget, lUIWidget );

public:
  static void ClassInitialiser();

  lUITextWidget();
  lUITextWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                 const lUIText & text,
                 const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                 const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                 const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~lUITextWidget();

  // lUIWidget interface

  virtual void Tick(float delta);
  virtual void Render();
  virtual lUIPoint GetIdealSize() const;

  // lUITextWidget interface

  void            SetText( const lUIText & text );
  void            SetText( const lUIText & text, bool bClip );
  const lUIText & GetText();

  void            SetTextToUpper() { m_text.ToUpper(); }

  void            SetTextColour( luint32_t colour );
  luint32_t       GetTextColour();

  void            SetAlign( lUIAlign align );
  lUIAlign        GetAlign() const;


  void            SetShadow( bool bShadow, luint32_t colour );

private:

  lUIText   m_text;
  luint32_t m_colour;
  luint32_t m_shadowColour;
  lUIAlign  m_align;
  bool      m_bShadow;
};


END_L_NAMESPACE
#endif
