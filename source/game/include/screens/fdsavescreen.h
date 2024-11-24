//����������������������������������������������������������������������������
// File        : fdsquadscreen.h
// Description : FDSquadScreen interface
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbuttonwidget.h>

#ifndef FDSAVESCREEN_H_INCLUDED
#define FDSAVESCREEN_H_INCLUDED

BEGIN_L_NAMESPACE


//����������������������������������������������������������������������������
// FDSaveScreen -
//
//����������������������������������������������������������������������������
class FDSaveScreen : public FDScreen, public FDLoadSaveCallback, public lHasSlots<>
{
 public:
  FDSaveScreen( FDMainMenuUI *parent );
  virtual ~FDSaveScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDSaveScreen interface
  virtual void SaveProgress( float pct );
  void SetProgress( float pct, bool bDoLoop );

  void SetEditMode( bool bEditMode );

 private:

  void Init();
  void OnSaveButton( lUIControlWidget * );

  // New game creation screen

  lUIScreenWidget       * m_saveScreen;
  FDButtonWidget        * m_saveButton;
  lUISpriteWidget       * m_progressText;
  lUISpriteWidget       * m_progressTextSaved;
  lUISpriteWidget       * m_warnText;
  lUISpriteWidget       * m_progressSprite;
  lUISpriteWidget       * m_progressSpriteComplete;
};


END_L_NAMESPACE
#endif

