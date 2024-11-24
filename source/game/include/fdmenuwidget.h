//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdmenuwidget.h
// Description : FDMenuWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDMENUWIDGETUI_H_INCLUDED
#define FDMENUWIDGETUI_H_INCLUDED

#include <ui/luiwidget.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDMenuWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDMenuWidget : public lUIWidget
{
  LUI_RTTI( FDMenuWidget, lUIWidget );

public:
  static void ClassInitialiser();

  FDMenuWidget();
  FDMenuWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                         lSharedPtr<lSprite> sprite,
                         lSharedPtr<lSprite> spriteGlow, lUIPoint spritePos,
                         const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                         const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                         const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDMenuWidget();

  bool IsRolledOrRollingOut() const;

  // lUIWidget interface

  virtual void     Render();


  virtual lUIPoint GetIdealSize() const;

  virtual bool     OnMouseDown( const lUIPoint &, luint32_t );
  virtual bool     OnMouseUp( const lUIPoint &, luint32_t );

  // FDMenuWidget interface

  int AddButton( lSharedPtr<lSprite> buttonOnSprite, lSharedPtr<lSprite> buttonOffSprite,
                  lSharedPtr<lSprite> textOnSprite, lSharedPtr<lSprite> textOffSprite,
                  lSharedPtr<lSprite> iconSprite, lSharedPtr<lSprite> iconGlowSprite );
  void ChangeButton( int index, bool bUseGlow );
  void SetButtonEnabled(const int iIndex, const bool bEnabled);
  void RollIn();
  void ChangeButtonText( int index, lSharedPtr<lSprite> textOnSprite, lSharedPtr<lSprite> textOffSprite );
  void SetSelectedIndex( int idx );

  lSignal2<FDMenuWidget *, int > SignalCommand;
  lSignal2<FDMenuWidget *, int > SignalHot;

  static bool ms_bMenuActive;
private:

  enum
  {
    STATE_ROLLED_IN  = 0,
    STATE_ROLLED_OUT = 1,
    STATE_ROLLING_OUT = 2,
  };

  struct menuOption
  {
    lSharedPtr<lSprite> buttonOn;
    lSharedPtr<lSprite> buttonOff;
    lSharedPtr<lSprite> textOn;
    lSharedPtr<lSprite> textOff;
    lSharedPtr<lSprite> icon;
    lSharedPtr<lSprite> iconGlow;
	bool                bUseGlow : 1;
	bool				bEnabled : 1;
  };

  lSharedPtr<lSprite>           m_sprite;
  lSharedPtr<lSprite>           m_spriteGlow;
  bool                               m_bGlow;
  luint32_t                     m_state;
  lSharedPtr<lSprite>           m_rod;
  lArray< menuOption >          m_buttonList;
  luint32_t                     m_selectedIndex;
};



END_L_NAMESPACE
#endif

