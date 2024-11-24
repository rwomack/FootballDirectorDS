//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdfullscreenwidget.h
// Description : FDTickerWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDFULLSCREENWIDGETUI_H_INCLUDED
#define FDFULLSCREENWIDGETUI_H_INCLUDED

#include <ui/luiwidget.h>
#include <framework/lsigslot.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDFullScreenWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDFullScreenWidget : public lUIWidget, public lHasSlots<>
{
  LUI_RTTI( FDFullScreenWidget, lUIWidget );

public:
  static void ClassInitialiser();

  FDFullScreenWidget();
  FDFullScreenWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                      luint32_t   colour,
                      const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                      const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                      const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDFullScreenWidget();

  // lUIWidget interface

  virtual void     Render();
  virtual void     Tick( float delta );
  virtual bool     OnMouseUp( const lUIPoint &, luint32_t );

  virtual lUIPoint GetIdealSize() const;

  // FDFullScreenWidget interface

  void            SetColour( luint32_t colour );
  luint32_t       GetColour();

  void            ClearTapped();
  bool            GetTapped();

  void            SetTapIgnoreDelay( float );

  lSignal1<FDFullScreenWidget *> SignalTapped;

private:

  luint32_t m_colour;
  bool      m_bTapped;
  float     m_tapIgnoreDelay;
};

END_L_NAMESPACE
#endif

