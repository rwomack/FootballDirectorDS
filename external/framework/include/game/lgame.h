//����������������������������������������������������������������������������
// File        : lgame.h
// Description : lGame interface
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LGAME_H_INCLUDED
#define LGAME_H_INCLUDED

#include <lemon/lapi.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lGameClient;
class lGameWorld;

//����������������������������������������������������������������������������
// lGame
//����������������������������������������������������������������������������

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

