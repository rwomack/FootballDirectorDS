//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lgame.h
// Description : lGame interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LGAME_H_INCLUDED
#define LGAME_H_INCLUDED

#include <lemon/lapi.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lGameClient;
class lGameWorld;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lGame
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lGame
{
public:

  lGame();
  virtual ~lGame();

  // lGame interface

  void Tick( float delta );

  // Clients

  void AddClient( lGameClient * );
  void RemoveClient( lGameClient * );

  // lGame interface

  virtual lGameWorld * GetWorld() = 0;

  // lGame implementation

private:
  lArray<lGameClient *>   m_clientList;
};



END_L_NAMESPACE

#endif

