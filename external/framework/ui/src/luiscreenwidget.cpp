//����������������������������������������������������������������������������
// File        : luiscreenwidget.cpp
// Description : lUIScreenWidget implementation
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <ui/luiscreenwidget.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI( lUIScreenWidget, lGUID(0,0,0,0), 0 );

//����������������������������������������������������������������������������
// lUIScreenWidget
//����������������������������������������������������������������������������

void lUIScreenWidget::ClassInitialiser()
{
}

lUIScreenWidget::lUIScreenWidget()
{
}

lUIScreenWidget::lUIScreenWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id )
  : lUIWidget( parent, widgetFlags, id, LUI_TOP_LEFT, lUIPointPercent( 0, 0 ), lUIPointPercent( 100, 100 ) )
{
}

lUIScreenWidget::~lUIScreenWidget()
{
}

void lUIScreenWidget::Tick(float delta)
{
}

void lUIScreenWidget::Render()
{
}



