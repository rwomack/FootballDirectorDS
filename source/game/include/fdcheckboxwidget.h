//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdcheckboxwidget.h
// Description : FDCheckBoxWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDCHECKBOXWIDGETUI_H_INCLUDED
#define FDCHECKBOXWIDGETUI_H_INCLUDED

#include <ui/luibuttonwidget.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDCheckBoxWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDCheckBoxWidget : public lUIButtonWidget
{
  LUI_RTTI( FDCheckBoxWidget, lUIButtonWidget );

public:
  static void ClassInitialiser();

  FDCheckBoxWidget();
  FDCheckBoxWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                  lSharedPtr<lSprite> spriteOff, lSharedPtr<lSprite> spriteOn,
                  luint32_t buttonFlags = 0,
                  const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                  const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                  const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDCheckBoxWidget();

  // lUIWidget interface

  virtual void     Render();
  virtual lUIPoint GetIdealSize() const;

  // lUIButtonWidget interface

  virtual void OnClick();

  // FDCheckBoxWidget interface

  void                SetSprite( lSharedPtr<lSprite> spriteOn, lSharedPtr<lSprite> spriteOff );
  lSharedPtr<lSprite> GetSpriteOn();
  lSharedPtr<lSprite> GetSpriteOff();

  void                Enable();
  void                Disable();
  bool                IsDisabled();

private:
  lSharedPtr<lSprite> m_spriteOn;
  lSharedPtr<lSprite> m_spriteOff;
  bool                m_bDisabled;
};


END_L_NAMESPACE
#endif

