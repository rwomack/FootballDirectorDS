//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : luibuttonwidget.h
// Description : lUIButtonWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LUIBUTTONWIDGET_H_INCLUDED
#define LUIBUTTONWIDGET_H_INCLUDED

#include "./luicontrolwidget.h"

BEGIN_L_NAMESPACE

// Pre-declared classes

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIButtonWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lUIButtonWidget : public lUIControlWidget
{
  LUI_RTTI( lUIButtonWidget, lUIControlWidget );

public:
  static void ClassInitialiser();

  lUIButtonWidget();
  lUIButtonWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                   const lUIText & text,
                   luint32_t buttonFlags = 0,
                   const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                   const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                   const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~lUIButtonWidget();

  // lUIWidget interface

  virtual void     Render();

  virtual lUIPoint GetIdealSize() const;

  virtual void     OnMouseOver();
  virtual void     OnMouseLeave();
  virtual bool     OnMouseDown( const lUIPoint &, luint32_t );
  virtual bool     OnMouseUp( const lUIPoint &, luint32_t );
  virtual void     OnGainFocus();
  virtual void     OnLoseFocus();

  // lUIButtonWidget interface

  virtual void     OnPress() {}
  virtual void     OnClick() {}

  lUIAlign        GetTextAlign();
  void            SetTextAlign( lUIAlign );

  const lUIText & GetText();
  void            SetText( const lUIText & );
  void            SetTextUpper() { m_text.ToUpper(); }

  luint32_t       GetState();
  luint32_t       GetButtonFlags();
  void            SetButtonFlags( luint32_t );

  bool            IsChecked();
  bool            IsDown();
  bool            IsClicked();
  bool            IsHover();
  bool            IsActive();

  enum
  {
    BUTTON_STATE_CHECKED  = 0x01,
    BUTTON_STATE_DOWN     = 0x02,
    BUTTON_STATE_CLICKED  = 0x04,
    BUTTON_STATE_HOVER    = 0x08,
    BUTTON_STATE_ACTIVE   = 0x10,
    BUTTON_STATE_DEBOUNCE = 0x20
  };

  enum
  {
    BUTTON_FLAG_STYLUS_ONECLICK = 0x1,
    BUTTON_FLAG_CHECKBOX        = 0x2,
    BUTTON_FLAG_RESERVED        = 0x200,  // reserved flag
  };


  void            SetChecked( bool b )
  {
    if( b )
      m_state |= BUTTON_STATE_CHECKED;
    else
      m_state = m_state & ~BUTTON_STATE_CHECKED;
  }

protected:

  luint32_t m_state;
  luint32_t m_buttonFlags;
  lUIText   m_text;
  lUIAlign  m_textAlign;
};


END_L_NAMESPACE
#endif
