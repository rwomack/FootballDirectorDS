//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdlanguagescreen.h
// Description : FDLanguageScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdcombowidget.h>

#ifndef FDLANGUAGESCREEN_H_INCLUDED
#define FDLANGUAGESCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDLanguageScreen - Negotiation screen
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDLanguageScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDLanguageScreen( FDMainMenuUI *parent );
  virtual ~FDLanguageScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDLanguageScreen interface
  void Update();

 private:

  void Init();

  void OnAccept( lUIControlWidget * );
  void OnLanguageSelection( FDComboWidget *, int index );

  lUIScreenWidget       * m_languageScreen;
  lUISpriteWidget       * m_flag;

  FDComboWidget *     m_languageSelection;
  lUITextWidget *         m_instructions;
  FDButtonWidget *      m_acceptButton;

  lUIScreenWidget       * m_languageScreenSub;

};

END_L_NAMESPACE
#endif
