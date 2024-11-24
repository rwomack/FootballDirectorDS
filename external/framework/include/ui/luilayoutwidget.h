//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : luilayoutwidget.h
// Description : lUILayoutWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LUILAYOUTWIDGET_H_INCLUDED
#define LUILAYOUTWIDGET_H_INCLUDED

#include "./luiwidget.h"

BEGIN_L_NAMESPACE

// Pre-declared classes

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Layout flags
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum
{
  LUI_LAYOUT_VERTICAL   = 0x1,
  LUI_LAYOUT_HORIZONTAL = 0x2,
  LUI_LAYOUT_CENTER     = 0x4,
  LUI_LAYOUT_EVEN       = 0x8,
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUILayoutWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lUILayoutWidget : public lUIWidget
{
  LUI_RTTI( lUILayoutWidget, lUIWidget );

public:
  static void ClassInitialiser();

  lUILayoutWidget();
  lUILayoutWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                   luint32_t layoutFlags,
                   const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                   const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                   const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~lUILayoutWidget();

  // lUIWidget interface

  virtual void Tick(float delta);
  virtual lUIPoint GetIdealSize() const;

  // lUILayoutWidget interface

  void SetSpacer( const lUIWidgetSizeCoord & size );
  void SetAlign( lUIAlignAxis align );

private:

  luint32_t          m_layoutFlags;
  lUIAlignAxis       m_align;
  lUIWidgetSizeCoord m_spacer;

};


END_L_NAMESPACE
#endif
