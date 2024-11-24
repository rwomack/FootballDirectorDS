//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsquadscreen.h
// Description : FDSquadScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbuttonwidget.h>
#include <fdscrollbarwidget.h>

#ifndef FDNEWSSCREEN_H_INCLUDED
#define FDNEWSSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

class FDTeleprinterWidget;

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDNewsScreen - News screen (03/05)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDNewsScreen : public FDScreen
{
 public:
  FDNewsScreen( FDMainMenuUI *parent );
  virtual ~FDNewsScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDNewsScreen interface
  void Update();

  void ResetTeleprinterDate();
 private:

  void Init();

  lUIScreenWidget       * m_newsScreen;
  FDTeleprinterWidget   * m_statusNews;
  luint32_t m_lastTeleprinterDate;
  luint32_t m_numberOfStoriesOnLastDate;
  luint32_t m_lastStoryDate;
};

END_L_NAMESPACE
#endif

