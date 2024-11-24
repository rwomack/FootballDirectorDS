//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : FDTransferListScreen.h
// Description : FDTransferListScreen interface
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdbuttonrolloutwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdsliderwidget.h>
#include <fdtablewidget.h>
#include <fdtransferlistwidget.h>

#include <screens/fdplayerinfosubscreen.h>

#ifndef FDTRANSFERLISTSCREEN_H_INCLUDED
#define FDTRANSFERLISTSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE


//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// FDTransferListScreen - TransferList screen (06,48)
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
class FDTransferListScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDTransferListScreen( FDMainMenuUI *parent );
  virtual ~FDTransferListScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void SerializeState( FDStateStorage &state );
  virtual void Prepare();

  // FDTransferListScreen interface
  void Search( FDTransferFilter &filter);

  static luint32_t TableBkgColourCallback(FDTransferListWidget *, void *pFDTransferList, luint32_t row, luint32_t col);

 private:

  void OnEditButton( lUIControlWidget * );
  void OnLoanButton( lUIControlWidget * );
  void OnBuyButton( lUIControlWidget * );
  void OnRowSelect( FDTransferListWidget *, int row );
  void OnFilterSelect( FDComboWidget *, int );


  void Init();

  lUIScreenWidget       * m_transferlistScreen;

  FDComboWidget * m_filterSelect;
  FDTransferListWidget * m_table;

  FDButtonWidget * m_editButton;
  FDButtonWidget * m_loanButton;
  FDButtonWidget * m_buyButton;
  bool m_bComingFromDialogBox;
};

//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
// FDTransferListEditScreen - TransferListEdit screen (06,48)
//
//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
class FDTransferListEditScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDTransferListEditScreen( FDMainMenuUI *parent );
  virtual ~FDTransferListEditScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDTransferListEditScreen interface
  void SetTransferFilterNumber( int number );

 private:

  void Init();
  void OnSearchButton( lUIControlWidget * );
  void OnSlider( lUIWidget *, float );
  void OnCheckBox( lUIControlWidget * );
  void OnCombo(FDComboWidget *, int);

  void Search();

  lUIScreenWidget       * m_transfereditScreen;

  // options
  FDSliderWidget        * m_editfilterMinCost;
  FDSliderWidget        * m_editfilterMaxCost;

  FDSliderWidget        * m_editfilterMinAge;
  FDSliderWidget        * m_editfilterMaxAge;

  FDSliderWidget        * m_editfilterMinSkill;
  FDSliderWidget        * m_editfilterMaxSkill;

  FDCheckBoxWidget      * m_editfilterListedPlayers;
  FDCheckBoxWidget      * m_editfilterInjuredPlayers;
  FDCheckBoxWidget      * m_editfilterNonListedPlayers;
  FDCheckBoxWidget      * m_editfilterSuspendedPlayers;
  FDCheckBoxWidget      * m_editfilterLoanListedPlayers;
  FDCheckBoxWidget      * m_editfilterOutOfContractPlayers;
  FDComboWidget         * m_editfilterPosition;

  lUITextWidget         * m_previewInfoText;

  // preview screen

  lUIScreenWidget       * m_transfereditScreenSub;

  FDTableWidget         * m_previewtable;

  int                         m_transferFilterNumber;
  FDTransferFilter            m_filter;
};


END_L_NAMESPACE
#endif

