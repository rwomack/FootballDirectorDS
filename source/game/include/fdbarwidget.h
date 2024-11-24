//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdbarwidget.h
// Description : FDTickerWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDBARWIDGETUI_H_INCLUDED
#define FDBARWIDGETUI_H_INCLUDED

#include <ui/luiwidget.h>
#include <framework/lsigslot.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDBarWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDBarWidget : public lUIWidget
{
  LUI_RTTI( FDBarWidget, lUIWidget );

public:
  static void ClassInitialiser();

  FDBarWidget();
  FDBarWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
               const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
               const lUIWidgetPosition pos = LUI_POS_DEFAULT,
               const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDBarWidget();

  // lUIWidget interface

  virtual void     Render();
  virtual void     Tick( float delta );

  virtual lUIPoint GetIdealSize() const;

  // FDBarWidget interface

  void            SetColourA( luint32_t colourA );
  luint32_t       GetColourA();

  void            SetColourB( luint32_t colourB );
  luint32_t       GetColourB();

  // show pctA% of A part of bar, 100%-pctA of B part of bar
  void            SetPctA( float pctA );

private:
  void Init();

  luint32_t m_colourA;
  luint32_t m_colourB;
  float     m_pctA;
  lSharedPtr<lSprite> m_gradient;
};

END_L_NAMESPACE
#endif

