//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsquadscreen.h
// Description : FDSquadScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDSCREEN_H_INCLUDED
#define FDSCREEN_H_INCLUDED

#include <lemon/lapi.h>

BEGIN_L_NAMESPACE

class FDMainMenuUI;
class lUIScreenWidget;
class lUIButtonWidget;
class FDStateStorage;

class FDScreen
{
 public:
  FDScreen( FDMainMenuUI *parent );
  virtual ~FDScreen();

  // Get main interfaces here
  FDMainMenuUI    * GetMainUI();
  lUIScreenWidget * GetMainScreenWidget();
  lUIScreenWidget * GetSubScreenWidget();

  // FDScreen interface
  virtual void DoShow() = 0;
  virtual void DoHide() = 0;

  // prepare screen to be shown
  virtual void Prepare()
  {
  }

  virtual void AutoplaySkip()
  {
  }

  virtual void SerializeState( FDStateStorage &state )
  {
  }

  void Show();
  void Hide();

  // Modal functions
  typedef bool (*ModalVerifyOKToExitFunction)(FDScreen *);
  void SetModalOK(lUIButtonWidget *pWidget);
  void SetModalVerifyOKToExitFunction(ModalVerifyOKToExitFunction pFunc);
  lUIButtonWidget *GetModalOK();
  bool RunModalVerifyOKToExitFunction();

  // Update handling
  bool IsForcedUpdateRequested() const;
  void SetForcedUpdateRequested(const bool bUpdate);

  bool IsVisible() { return !m_bHidden; };

 private:
  FDMainMenuUI                  *m_parent;
  lUIButtonWidget               *m_pModalOK;
  ModalVerifyOKToExitFunction    m_pModalVerifyFunc;

  bool      m_bHidden : 1;
  bool			m_bForcedUpdateRequested : 1;
};


END_L_NAMESPACE
#endif


