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

#include <fdbarwidget.h>
#include <fdbuttonwidget.h>
#include <fdcombowidget.h>
#include <fdsliderwidget.h>
#include <fdtickerwidget.h>
#include <fdtablewidget.h>
#include <fdtextwidget.h>

#include "TransferNegotiation.h"
#include <screens/fdplayerinfosubscreen.h>
#include <screens/fdyouthinfoscreen.h>
#include <screens/fdemployeeinfoscreen.h>

#ifndef FDNEGOTIATESCREEN_H_INCLUDED
#define FDNEGOTIATESCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDNegotiateScreen - Negotiation screen
// (22,26)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDNegotiateScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDNegotiateScreen( FDMainMenuUI *parent );
  virtual ~FDNegotiateScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();
  virtual void AutoplaySkip();

  // FDNegotiateScreen interface
  void Update();

  // Employee
  void SetOnHireEmployee( CEmployee * _employee );
  void SetOnNegotiateEmployeeContract( CEmployee * _employee, const CString &_Prompt );

  // Youth
  void SetOnPromoteYouth( CYouth * youth, const CString &prompt );

  // Player Contracting
  void SetOnReceiveOffer( CClubTransferOffer* _TheOffer );
  void SetOnNegotiatePlayerContract( CPlayer * _Player, CClub * _Club, const CString &_Prompt );
  void SetOnNegotiateOutOfContractPlayerContract( CPlayer * _Player, CClub * _Club, bool _Accepted, CContract _PlayerContract );
  void SetNotOwnedPlayerContractNegotiations( CTransferNegotiation* _Negotiations );
  void SetOnMakeOfferForPlayer( CPlayer * _Player );

  void SetOnPlayerRequestTransfer( CPlayer * _Player );

  // Loans
  void SetOnApproachClubLoanInPlayer( CPlayer * _Player );

  void ShowMessageScreen();

  // Status
  eContractOfferResponse GetLastContractOfferResponse() const;
  void ResetLastContractOfferResponse();

  enum
  {
    NEGOTIATE_RECEIVEOFFER,  // Club bid on our player
    //NEGOTIATE_LOAN,
    NEGOTIATE_MAKE_OFFER_PLAYER, // Make an offer for their player
    NEGOTIATE_YOUTH,
    //NEGOTIATE_CLUB_TRANSFER,
    NEGOTIATE_HIRE_EMPLOYEE,
    NEGOTIATE_RENEW_EMPLOYEE,
    NEGOTIATE_PLAYER_CONTRACT,
    NEGOTIATE_OUTOFCONTRACT_PLAYER_CONTRACT,
    NEGOTIATE_PLAYER_CONTRACT_NEGOTIATIONS,
    NEGOTIATE_APPROACH_CLUB_LOAN_IN_PLAYER, //ApproachClubLoanInPlayer
    NEGOTIATE_PLAYER_REQUEST_TRANSFER,
  };

 private:

  void Init();
  void OnAccept( lUIControlWidget * );
  void OnOffer( lUIControlWidget * );
  void OnReject( lUIControlWidget * );
  void OnCancel( lUIControlWidget * );
  void OnCounter( lUIControlWidget * );
  void OnContinue( lUIControlWidget * );

  void OnEndNegotiations(const CString &strMessage);

  void OnSliderChange( lUIWidget *, float );

  void HideAll();

  lUIScreenWidget       * m_negotiateScreen;
  lUIScreenWidget       * m_negotiateScreenSub;

  FDFullScreenWidget * m_topFade;

  lUISpriteWidget       * m_messageBar;

  lUISpriteWidget       * m_backingStripes[6];
  FDBarWidget       * m_backingBars[6];

  lUITextWidget         * m_typeText;
  lUITextWidget         * m_text;

  lUITextWidget         * m_mainTitleText;
  lUITextWidget         * m_mainBidAmount;
  lUITextWidget         * m_mainBiddingClub;

  lUITextWidget			* m_mainSellOnFeeAmount;
  lUITextWidget			* m_mainSellOnFeeAmountValue;
  lUISpriteWidget       * m_mainSellOnFeeAmountStripe;
  lUISpriteWidget       * m_mainSellOnFeeAmountBar;

  lUITextWidget         * m_mainBidAmountValue;
  lUITextWidget         * m_mainBiddingClubValue;


  FDSliderWidget       * m_slider1;
  FDSliderWidget       * m_slider2;
  FDSliderWidget       * m_slider3;


  FDButtonWidget * m_acceptButton;
  FDButtonWidget * m_offerButton;
  FDButtonWidget * m_rejectButton;
  FDButtonWidget * m_counterButton;
  FDButtonWidget * m_continueButton;
  FDButtonWidget * m_cancelButton;

  enum {
    NEG_START_SCREEN,
    NEG_MAIN_SCREEN,
    NEG_END_SCREEN,
  };

  int                     m_numSliders;
  bool m_bShowSellOnFeeStuff;

  //
  int                    m_state; // Screen state
  int                    m_type;
  CTransferNegotiation* m_negotiations;
  CClubTransferOffer   * m_transferOffer;
  CClubTransferOffer   m_bidDetails;
  CEmployee *               m_employee;
  CContract                   m_ContractRequired;
  CYouth *                      m_youth;
  CClub *                       m_club;
  CPlayer *                     m_player;

  eContractOfferResponse m_eLastContractOfferResponse;
};

END_L_NAMESPACE
#endif

