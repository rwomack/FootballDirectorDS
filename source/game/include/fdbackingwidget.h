//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDBackingWidget.h
// Description : FDTickerWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDBACKINGWIDGETUI_H_INCLUDED
#define FDBACKINGWIDGETUI_H_INCLUDED

#include <ui/luiwidget.h>
#include <framework/lsigslot.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDBackingWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDBackingWidget : public lUIWidget
{
  LUI_RTTI( FDBackingWidget, lUIWidget );

public:
  static void ClassInitialiser();

  FDBackingWidget();
  FDBackingWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                      luint32_t   colour,
                      const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                      const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                      const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDBackingWidget();

  // lUIWidget interface

  virtual void     Render();
  virtual void     Tick( float delta );

  virtual lUIPoint GetIdealSize() const;

  // FDBackingWidget interface

  void SetLines( int width, luint32_t colour );

  void            SetColour( luint32_t colour );
  luint32_t       GetColour();

  void            SetLineColour( luint32_t colour );
  luint32_t       GetLineColour();

private:

  luint32_t m_bgColour;
  luint32_t m_lineColour;
  int         m_lineWidth;
};

END_L_NAMESPACE
#endif

