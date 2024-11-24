//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdyouthsquadscreen.h
// Description : FDyouthSquadScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>
#include "fdyouthinfoscreen.h"
#include "fdnegotiatescreen.h"

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdsliderwidget.h>
#include <fdtablewidget.h>

#ifndef FDYOUTHSQUADSCREEN_H_INCLUDED
#define FDYOUTHSQUADSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDYouthSquadScreen - Youth Squad screen
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDYouthSquadScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDYouthSquadScreen( FDMainMenuUI *parent );
  virtual ~FDYouthSquadScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void SerializeState( FDStateStorage &state );
  virtual void Prepare();

  // FDYouthSquadScreen interface
  void Update();

 private:

  void OnClubRowSelect( FDTableWidget *, int row );
  void OnAvailableRowSelect( FDTableWidget *, int row );

  void OnTableSelect(FDComboWidget *, int);

  void OnHireButton( lUIControlWidget * );
  void OnSackButton( lUIControlWidget * );
  void OnPromoteButton( lUIControlWidget * );

  void Init();

  void AddYouthToTable(FDTableWidget *pTable, CYouth *pYouth, const int iTagData);

  lUIScreenWidget       * m_youthSquadScreen;

  FDTableWidget         * m_clubTable;
  FDTableWidget         * m_availableTable;

  FDComboWidget         * m_tableSelect;

  FDButtonWidget        * m_hireButton;
  FDButtonWidget        * m_sackButton;
  FDButtonWidget        * m_promoteButton;

  lArray<CYouth *>        m_clubYouths;
  lArray<CYouth *>        m_availableYouths;

  bool m_bComingFromDialogBox;
};



END_L_NAMESPACE
#endif

