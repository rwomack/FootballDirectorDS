//����������������������������������������������������������������������������
// File        : fdgame.cpp
// Description : FDGame implementation
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdgame.h>
#include <fdgameworld.h>

using namespace L_NAMESPACE_NAME;

//����������������������������������������������������������������������������
// FDGame
//����������������������������������������������������������������������������

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


