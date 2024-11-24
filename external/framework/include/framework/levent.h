//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : levent.h
// Description : Common game event handling
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LEVENT_H_INCLUDED
#define LEVENT_H_INCLUDED

//todo
//Seperate lEvent into platforms
//Add event detection for:
//  PS2:
//    Controller disconnect/reconnect (slot 1,2 etc.)
//    Disc eject / invalid disc
//  NDS:
//    Close / open lid
//    Low battery
//  Win32:
//    Alt key combos
//    Lost/got focus
//  WII:
//    Controller communication loss
//    Invalid controller extension
//    Disc eject
//    Power / reset buttons

#include <framework/lsigslot.h>
#include <framework/ldialog.h>

BEGIN_L_NAMESPACE

enum
{
  LEVENT_PSP_UTILITY_TYPE_NONE = 0,
  LEVENT_PSP_UTILITY_TYPE_SAVELOAD,
  LEVENT_PSP_UTILITY_TYPE_MESSAGE,
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lEvent
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lEvent
{
  public:

  static void Init();
  static void Check();
  static void CheckPostRender();

  //Win32
  static lSignal0<> LostFocusSignal;
  static lSignal0<> GotFocusSignal;
  static lSignal0<> AltEnterSignal;
  static lSignal0<> AltEscapeSignal;
  static lSignal0<> AltTabSignal;

  //PS2
  static lSignal2<lDialog*,int> ControllerDisconnectSignal;
  static lSignal2<lDialog*,int> ControllerConnectSignal;

  /*
  //NDS
  static lSignal0<> LidCloseSignal;
  static lSignal0<> LidOpenSignal;
  static lSignal0<> LowBatterySignal;*/

  //WII
  static lSignal0<> PowerButtonSignal;
  static lSignal0<> ResetButtonSignal;
  static lSignal0<> HomeButtonSignal;
  static lSignal2<lDialog*,int> ControllerCommunicationLostSignal;
  static lSignal2<lDialog*,int> ControllerCommunicationEstablishedSignal;
  static lSignal2<lDialog*,int> UnsupportedControllerExtensionSignal;
  static lSignal1<lDialog*> DiscErrorSignal;
  static lSignal1<lDialog*> DiscOkSignal;
  static void ResetButtonCallback();
  static void PowerButtonCallback();
  static void WiiDiscCheck(const char*);
  static void DisableHomeButton( bool );
  static void SetDiscErrorReported(bool);
  #ifdef L_PLATFORM_WII
  static lDialog* GetWiiDiscErrorDialog(int status);
  static void DisableResetSignal();
  static void EnableResetSignal();
  #endif

  //PSP
  static lSignal0<> SaveLoadUtilityCompleteSignal;
  static void SetPSPUtilityMode(int mode);
  static int GetPSPUtilityMode();
  static void OpenPSPErrorMessage(int messageId);
  static void ShowPSPDiscEjectMessage();
  static void HidePSPDiscEjectMessage();
  static void ShowPSPDiscReadingMessage();
  static void HidePSPDiscReadingMessage();

  //tmp
  static void TmpPS2Disconnect();
  static void TmpPS2Reconnect();
};



END_L_NAMESPACE

#endif

