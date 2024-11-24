//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : luicontrolwidget.cpp
// Description : lUIControlWidget implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <ui/luicontrolwidget.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( lUIControlWidget, lGUID(0,0,0,0), 0 );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIControlWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void lUIControlWidget::ClassInitialiser()
{
}

lUIControlWidget::lUIControlWidget()
{
}

lUIControlWidget::lUIControlWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                  const lUIWidgetPosition anchor,
                  const lUIWidgetPosition pos,
                  const lUIWidgetSize size )
  : lUIWidget( parent, widgetFlags, id, anchor, pos, size )
{
}

lUIControlWidget::~lUIControlWidget()
{
}
