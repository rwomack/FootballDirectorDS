//����������������������������������������������������������������������������
// File        : fdgame.h
// Description : FDGame interface
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDGAME_H_INCLUDED
#define FDGAME_H_INCLUDED

#include <game/lgame.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class FDGameWorld;

//����������������������������������������������������������������������������
// FDGame
//����������������������������������������������������������������������������

class FDGame : public lGame
{
public:
  FDGame();
  virtual ~FDGame();

  // lGame interface

  virtual lGameWorld * GetWorld();

private:
  FDGameWorld * m_world;
};

END_L_NAMESPACE

#endif


