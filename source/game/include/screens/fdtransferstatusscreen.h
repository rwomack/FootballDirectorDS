//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : FDTransferStatusScreen.h
// Description : FDTransferStatusScreen interface
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
#include <fdcheckboxwidget.h>

#include "TransferNegotiation.h"
#include <screens/fdplayerinfosubscreen.h>

#ifndef FDTRANSFERSTATUSSCREEN_H_INCLUDED
#define FDTRANSFERSTATUSSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDTransferStatusScreen - Player Transfer Status screen
// (16)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDTransferStatusScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDTransferStatusScreen( FDMainMenuUI *parent );
  virtual ~FDTransferStatusScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDTransferStatusScreen interface
  void SetPlayer( CPlayer * player );
  void Update();

 private:

  void Init();

  void OnAccept( lUIControlWidget * );
  void OnCancel( lUIControlWidget * );

  void OnCheckTransferList( lUIControlWidget * );
  void OnCheckLoanList( lUIControlWidget * );
  void OnCheckRejectAllBids( lUIControlWidget * );
  void OnSlider( lUIWidget *, float );

  lUIScreenWidget       * m_transferStatusScreen;

  FDSliderWidget       * m_slider1;

  FDCheckBoxWidget * m_transferList;
  FDCheckBoxWidget * m_rejectAllBids;
  FDCheckBoxWidget * m_availableForLoan;

  FDButtonWidget * m_acceptButton;
  FDButtonWidget * m_cancelButton;

  enum TransferStatusInfoFieldType
  {
    TSIFT_TRANSFERLIST,
    TSIFT_PRICE,
    TSIFT_REJECTALLBIDS,
    TSIFT_AVAILABLEFORLOAN,
    TSIFT_MAX,
  };


  lUISpriteWidget *m_backingStripes[TSIFT_MAX];
  FDBarWidget     *m_backingBars[TSIFT_MAX];
  //lUITextWidget   *m_fieldLabels[AIFT_MAX];

  CClub *                       m_club;
  CPlayer *                     m_player;
};

END_L_NAMESPACE
#endif
