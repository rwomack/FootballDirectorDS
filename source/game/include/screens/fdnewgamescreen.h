//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsquadscreen.h
// Description : FDSquadScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>

#ifndef FDNEWGAMESCREEN_H_INCLUDED
#define FDNEWGAMESCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

class FDKeyboardWidget;

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDNewGameScreen - NewGame screen (11)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDNewGameScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDNewGameScreen( FDMainMenuUI *parent );
  virtual ~FDNewGameScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDNewGameScreen interface
  lString          GetNameStr() { return m_newgameNameStr; }

 private:
  void Init();

  void OnNewGameKeyboard( char );
  void OnNewGameDone( lUIControlWidget * );
  void OnNewGameBack( lUIControlWidget * );

  // New game creation screen
  lUIScreenWidget       * m_newgameScreen;
  lUIScreenWidget       * m_newgameScreenSub;
  lUITextWidget         * m_newgameName;
  lString                 m_newgameNameStr;
  FDKeyboardWidget		* m_keyboard;
  FDButtonWidget        * m_newgameDone;
  FDButtonWidget		* m_newgameBack;
};


END_L_NAMESPACE
#endif

