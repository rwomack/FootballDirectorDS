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
#include <fdscrollbarwidget.h>

#ifndef FDNEWSSCREEN_H_INCLUDED
#define FDNEWSSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

class FDTeleprinterWidget;

//����������������������������������������������������������������������������
// FDNewsScreen - News screen (03/05)
//
//����������������������������������������������������������������������������
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

