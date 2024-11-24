//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : luispritewidget.h
// Description : lUISpriteWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LUISPRITEWIDGET_H_INCLUDED
#define LUISPRITEWIDGET_H_INCLUDED

#include "./luiwidget.h"
#include <lemon/object/lsharedptr.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class  lSprite;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUISpriteWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lUISpriteWidget : public lUIWidget
{
  LUI_RTTI( lUISpriteWidget, lUIWidget );

  lSharedPtr<lSprite>        m_sprite;

public:
  static void ClassInitialiser();

  lUISpriteWidget();
  lUISpriteWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                   lSharedPtr<lSprite> sprite = lSharedPtr<lSprite>(),
                   const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                   const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                   const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~lUISpriteWidget();

  // lUIWidget interface

  virtual void     Tick(float delta);
  virtual void     Render();

  virtual lUIPoint GetIdealSize() const;

  // lUISpriteWidget interface

  void                SetSprite( lSharedPtr<lSprite> );
  lSharedPtr<lSprite> GetSprite();

  void                ClearSprite()
  {
    SetSprite( lSharedPtr<lSprite>() );
  }

private:
};


END_L_NAMESPACE
#endif

