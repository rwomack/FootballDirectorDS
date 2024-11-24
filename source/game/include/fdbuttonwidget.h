//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdbuttonwidget.h
// Description : FDButtonWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDBUTTONWIDGETUI_H_INCLUDED
#define FDBUTTONWIDGETUI_H_INCLUDED

#include <ui/luibuttonwidget.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDButtonWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDButtonWidget : public lUIButtonWidget
{
  LUI_RTTI( FDButtonWidget, lUIButtonWidget );

public:
  static void ClassInitialiser();

  FDButtonWidget();
  FDButtonWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                  lSharedPtr<lSprite> sprite,
                  luint32_t buttonFlags = 0,
                  const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                  const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                  const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDButtonWidget();

  // lUIWidget interface

  virtual void     Render();
  virtual lUIPoint GetIdealSize() const;

  // lUIButtonWidget interface

  // FDButtonWidget interface

  void                SetSprite( lSharedPtr<lSprite> sprite );
  lSharedPtr<lSprite> GetSprite();

  void                SetSpriteOn( lSharedPtr<lSprite> sprite );
  lSharedPtr<lSprite> GetSpriteOn();

  void                SetDisabledSprite( lSharedPtr<lSprite> sprite );
  lSharedPtr<lSprite> GetDisabledSprite();

  void                Enable();
  void                Disable();
  bool                IsDisabled();
  void                Highlight( bool hi );

  virtual void     OnPress();
  virtual void     OnClick();


  enum
  {
    //NOTE: lUIButtonWidget flags share the same flag space
    FDBUTTON_FLAG_NO_SOUND       = 0x200,  // disable sound for this button

  };


private:
  lSharedPtr<lSprite> m_sprite;
  lSharedPtr<lSprite> m_spriteOn;
  lSharedPtr<lSprite> m_disabledSprite;
  bool                m_bDisabled;
  bool                m_bHighlight;
};


END_L_NAMESPACE
#endif

