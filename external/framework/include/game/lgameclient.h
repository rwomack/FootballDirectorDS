//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lgameclient.h
// Description : lGameClient interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LGAMECLIENT_H_INCLUDED
#define LGAMECLIENT_H_INCLUDED

#include <lemon/lapi.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lGame;
class lGameRenderContext;
class lGameLeaf;
class lGameClientView;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lGameClient
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lGameClient
{
public:

  lGameClient();
  virtual ~lGameClient();

  // lGameClient interface

  virtual void            ClientInit() = 0;
  virtual void            ClientDeinit() = 0;

  virtual void            Tick(float) = 0;
  virtual void            Render() = 0;
  virtual void            RenderLeafs( lGameRenderContext *, lGameLeaf ** leafList, int numLeafs ) = 0;

  lGame                 * GetGame();
  lGameClientView       * GetClientView();

private:
  friend class lGame;
  lGame           * m_game;
  lGameClientView * m_view;
};


END_L_NAMESPACE

#endif

