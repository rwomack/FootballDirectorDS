//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdoptions.h
// Description : FDOptions interface
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
#include <fdcheckboxwidget.h>
#include <fdsliderwidget.h>
#include <fdbackingwidget.h>

#ifndef FDOPTIONS_H_INCLUDED
#define FDOPTIONS_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDOptions - Options screen
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDOptions : public FDScreen, public lHasSlots<>
{
 public:
  FDOptions( FDMainMenuUI *parent );
  virtual ~FDOptions();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDOptions interface
  void Update();

  void InGame();

 private:

  void Init();

  void OnAccept( lUIControlWidget * );
  void OnCancel( lUIControlWidget * );

  void OnPageSelection( FDComboWidget *, int index );
  void OnMatchSpeedChange(lUIWidget *pSlider, float fValue);
  void OnSoundVolumeChange(lUIWidget *pSlider, float fValue);
  void OnCrowdVolumeChange(lUIWidget *pSlider, float fValue);
  void OnCheckInMatchAudio(lUIControlWidget *pCheckbox);
  void OnCheckTutorials(lUIControlWidget *pCheckbox);
//  void OnCheckInMatchReplays(lUIControlWidget *pCheckbox);

  // Main screen / underlying tabs
  lUIScreenWidget *m_optionsScreen;

  FDButtonWidget *m_acceptButton;
  FDButtonWidget *m_cancelButton;

  // General options
  FDSliderWidget *m_matchSpeedSlider;

  // Sound options
  FDSliderWidget *m_soundVolumeSlider;
  FDSliderWidget *m_crowdVolumeSlider;
  FDCheckBoxWidget *m_inMatchAudioButton;
  FDCheckBoxWidget *m_tutorialsButton;
//  FDCheckBoxWidget *m_inMatchReplaysButton;

  // Top screen
  lUIScreenWidget *m_optionsScreenSub;

  FDPrefsData m_cPrefs;

  bool m_bInGame : 1;
  bool m_bComingFromDialog : 1;
};

END_L_NAMESPACE
#endif
