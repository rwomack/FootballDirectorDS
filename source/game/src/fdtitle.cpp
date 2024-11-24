//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdtitle.cpp
// Description : FDTitle title screen implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <lemon/object/lsharedptr.h>
#include <framework/lsprite.h>
#include <framework/lspritebank.h>
#include <fdtitle.h>
#include <lemon/input/linput.h>
#include <fdresourcemanager.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTitle
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

static lSharedPtr<lSprite> s_titleSprite;
static lSharedPtr<lSprite> s_beginSprite;
static bool s_open = false;
static bool s_debounce = true;
static float s_initialTime = 0.5f;

void FDTitle::Init()
{
  //load sprites
  lSharedPtr<lSpriteBank> sb = FDResourceManager::GetSpriteBank("title" );

  s_titleSprite = lSharedPtr<lSprite>(lSprite::Create( sb, "title" ));
  s_titleSprite->SetNormalCoordinates(true);
  s_titleSprite->SetWidth(1.0f);
  s_titleSprite->SetHeight(1.0f);
  s_beginSprite = lSharedPtr<lSprite>(lSprite::Create( sb, "begin" ));
  s_beginSprite->SetNormalCoordinates(true);
  s_beginSprite->SetWidth(1.0f);
  s_beginSprite->SetHeight(1.0f);

  s_open = true;
}

void FDTitle::Deinit()
{
  s_titleSprite.Reset();
  s_beginSprite.Reset();
}


void FDTitle::Tick(float delta)
{
  // Prevent skipping for half a second
  if( s_initialTime > 0 )
  {
    s_initialTime -= delta;
    return;
  }

  if( !lInput->GetInputState( LI_MOUSE_BUTTON1 ) && s_debounce ) s_debounce = false;
  //check for taps
  #ifdef L_PLATFORM_WII
  if( lInput->GetInputState( LI_WII_PAD1_BUTTON_A ) )
  {
    lInput->SetInputState( LI_WII_PAD1_BUTTON_A, 0 );
  #else
  if( lInput->GetInputState( LI_MOUSE_BUTTON1 ) && !s_debounce )
  {
  #endif
    s_open = false;
  }

  //tmp
  #ifdef L_PLATFORM_PSP
  if( lInput->GetInputState( LI_PSP_PAD1_BUTTON_CROSS ) )
  {
    lInput->SetInputState( LI_PSP_PAD1_BUTTON_CROSS, 0 );
    s_open=false;
    return;
  }
  #endif
}

bool FDTitle::IsOpen()
{
  return s_open;
}

void FDTitle::ForceClose()
{
	s_open = false;
}

void FDTitle::Render()
{
  //draw sprites
  #ifdef L_PLATFORM_NDS
  if( lSpriteManager::GetDisplayIndex() == 0 )
  {
    s_titleSprite->Render();
  }
  else
  {
    s_beginSprite->Render();
  }
  #else
  s_titleSprite->Render();
  #endif
}


