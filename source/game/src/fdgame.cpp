//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdgame.cpp
// Description : FDGame implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdgame.h>
#include <fdgameworld.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDGame
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDGame::FDGame()
{
  m_world = FD_GAME_NEW FDGameWorld;
}

FDGame::~FDGame()
{
}

lGameWorld * FDGame::GetWorld()
{
  return m_world;
}


