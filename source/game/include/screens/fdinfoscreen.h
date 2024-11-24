//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdinfoscreen.h
// Description : FDInfoScreen interface
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
#include <fdsliderwidget.h>
#include <fdtickerwidget.h>
#include <fdtablewidget.h>
#include <fdfullscreenwidget.h>

#ifndef FDINFOSCREEN_H_INCLUDED
#define FDINFOSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDInfoScreen - Used for displaying win/lose/end of season
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDInfoScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDInfoScreen( FDMainMenuUI *parent );
  virtual ~FDInfoScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void AutoplaySkip();

  // FDInfoScreen interface
  void SetWonMOM( lString divName );
  void SetCupKnockedOut( lString cupName );
  void SetCupFinalResult( lString cupName, bool bWin );

  enum
  {
    ENDSEASON_CHAMPIONS,
    ENDSEASON_PROMOTION,
    ENDSEASON_TOP,
    ENDSEASON_MIDTABLE,
    ENDSEASON_BOTTOM,
    ENDSEASON_RELEGATION,
  };

  void SetEndOfSeason( lString divName, int divResult );

  void OnScreenTap( FDFullScreenWidget * );

 private:

  void Init();

  lUIScreenWidget       * m_infoScreen;
  lUIScreenWidget       * m_infoScreenSub;

  lUISpriteWidget       * m_backSprite;
  lUITextWidget         * m_text;

  FDFullScreenWidget    * m_tapScreen;

  bool                    m_bPlayCheer;
};




END_L_NAMESPACE
#endif

