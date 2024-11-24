//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : luicontrolwidget.h
// Description : lUIControlWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LUICONTROLWIDGET_H_INCLUDED
#define LUICONTROLWIDGET_H_INCLUDED

#include "./luiwidget.h"
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIControlWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lUIControlWidget : public lUIWidget
{
  LUI_RTTI( lUIControlWidget, lUIWidget );

public:
  static void ClassInitialiser();

  lUIControlWidget();
  lUIControlWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                    const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                    const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                    const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~lUIControlWidget();

  // lUIWidget interface

  // lUIControlWidget interface

  // Signals

  lSignal1<lUIControlWidget *> SignalControlHot;
  lSignal1<lUIControlWidget *> SignalControlCommand;
};


END_L_NAMESPACE
#endif
