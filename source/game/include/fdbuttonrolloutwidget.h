//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdbuttonrolloutwidget.h
// Description : FDButtonRolloutWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDBUTTONROLLOUTWIDGETUI_H_INCLUDED
#define FDBUTTONROLLOUTWIDGETUI_H_INCLUDED

#include <ui/luiwidget.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDButtonRolloutWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDButtonRolloutWidget : public lUIWidget
{
  LUI_RTTI( FDButtonRolloutWidget, lUIWidget );

public:
  static void ClassInitialiser();

  FDButtonRolloutWidget();
  FDButtonRolloutWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                         lSharedPtr<lSprite> sprite,
                         const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                         const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                         const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDButtonRolloutWidget();

  bool IsRolledOrRollingOut() const;

  // lUIWidget interface

  virtual void     Render();

  virtual lUIPoint GetIdealSize() const;

  virtual bool     OnMouseDown( const lUIPoint &, luint32_t );

  // FDButtonRolloutWidget interface

  void AddButton( lSharedPtr<lSprite> buttonSprite );
  void ChangeButton( int index, lSharedPtr<lSprite> buttonSprite );
  void RollIn();

  lSignal2<FDButtonRolloutWidget *, int > SignalCommand;
  lSignal2<FDButtonRolloutWidget *, int > SignalHot;

private:

  enum
  {
    STATE_ROLLED_IN  = 0,
    STATE_ROLLED_OUT = 1,
    STATE_ROLLING_OUT = 2,
  };

  lSharedPtr<lSprite>           m_sprite;
  luint32_t                     m_state;
  lSharedPtr<lSprite>           m_rod;
  lArray< lSharedPtr<lSprite> > m_buttonList;
  luint32_t                     m_selectedIndex;
};



END_L_NAMESPACE
#endif

