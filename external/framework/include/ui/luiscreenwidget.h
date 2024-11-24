//����������������������������������������������������������������������������
// File        : luiscreenwidget.h
// Description : lUIScreenWidget interface
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LUISCREENWIDGET_H_INCLUDED
#define LUISCREENWIDGET_H_INCLUDED

#include "./luiwidget.h"

BEGIN_L_NAMESPACE

// Pre-declared classes

//����������������������������������������������������������������������������
// lUIScreenWidget
//����������������������������������������������������������������������������

class lUIScreenWidget : public lUIWidget
{
  LUI_RTTI( lUIScreenWidget, lUIWidget );

public:
  static void ClassInitialiser();

  lUIScreenWidget();
  lUIScreenWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id );

  virtual ~lUIScreenWidget();

  // lUIWidget interface

  virtual void Tick(float delta);
  virtual void Render();

  // lUIScreenWidget interface

private:
};



END_L_NAMESPACE
#endif
