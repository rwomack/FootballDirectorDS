//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdgameworld.cpp
// Description : FDGameWorld implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdgameworld.h>
#include <fdresourcemanager.h>
#include <fdscript.h>

#include <game/lgameclient.h>
#include <game/lgameclientview.h>

#include <ui/lui.h>
#include <ui/luiwidget.h>
#include <ui/luimouse.h>
#include <lemon/input/linput.h>

#include <framework/lspritemanager.h> // for display index

#include <fdfrontend.h>

using namespace L_NAMESPACE_NAME;

static bool s_debounce = true;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDSubGame
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDSubGame::FDSubGame()
  : m_world(NULL)
{
}

FDSubGame::~FDSubGame()
{
}

FDGameWorld * FDSubGame::GetWorld()
{
  return m_world;
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDGameWorld
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

void FDGameWorld::ClassInitialiser()
{
}

FDGameWorld::FDGameWorld()
{
  m_subGameIndex = -1;
  m_bPlaying = false;
  m_time = 0.0f;
  m_tick = 0.0f;
}

FDGameWorld::~FDGameWorld()
{
}

void FDGameWorld::Init()
{
  lUIWidget::SetDefaultFont( FDResourceManager::GetDefaultFont() );

  m_uiManager = FD_GAME_NEW lUIManager();

  AddSubGame( FD_GAME_NEW FDFrontEnd );         // FD_GAMEMODE_MAIN_MENU
}

void FDGameWorld::Deinit()
{
  FDResourceManager::Deinit();
}

int FDGameWorld::AddSubGame( FDSubGame * subGame )
{
  int index = m_subGameList.Add();
  m_subGameList(index) = subGame;
  subGame->m_world = this;

  subGame->Init();

  return index;
}

int FDGameWorld::GetNumSubGames()
{
  return m_subGameList.Size();
}

FDSubGame * FDGameWorld::GetSubGame( int index )
{
  if( m_subGameIndex == -1 )
  {
    return NULL;
  }

  return m_subGameList(index);
}

void FDGameWorld::SetCurrentSubGame( int index )
{
  // TODO: we might want to delay the sub-game swap till the next tick.
  //       we can then clean up any ui's and layers in the manager automatically.

  if( index != m_subGameIndex )
  {
    if( m_subGameIndex != -1 )
    {
      lLogFmt("Deactivating sub game %d\n", m_subGameIndex );
      m_subGameList(m_subGameIndex)->Deactivate();
    }

    m_subGameIndex = index;

    if( m_subGameIndex != -1 )
    {
      lLogFmt("Activating sub game %d\n", m_subGameIndex );
      m_subGameList(m_subGameIndex)->Activate();
    }
  }
}

FDSubGame * FDGameWorld::GetCurrentSubGame()
{
  if( m_subGameIndex != -1 )
  {
    return m_subGameList(m_subGameIndex);
  }

  return NULL;
}

void FDGameWorld::BeginPlay()
{
  lLogSys("FDGame","Game play started\n");
  m_bPlaying = true;
  m_time = 0.0f;

  SetCurrentSubGame( FD_GAMEMODE_MAIN_MENU );
}

void FDGameWorld::EndPlay()
{
  lLogSys("FDGame","Game play ended\n");
  m_bPlaying = false;
  m_time = 0.0f;

  SetCurrentSubGame( -1 );
}

void FDGameWorld::Tick(float delta)
{
  if( m_bPlaying )
  {
    m_time += delta;
  }

  m_tick += delta;

  if( m_subGameIndex != -1 )
  {
    m_subGameList( m_subGameIndex )->Tick( delta );
  }

  // Get the active input layer

  lUILayer * inputLayer = NULL;

  if( m_uiManager->GetNumLayers() > 0 )
  {
    inputLayer = m_uiManager->GetLayer( m_uiManager->GetNumLayers()-1 );
  }

  // Process input

  if( inputLayer )
  {
    lUI * ui = inputLayer->GetTopUI();

    if( ui )
    {
      lUIWidget * mainWidget = ui->GetScreen();

      if( mainWidget )
      {

        if( !s_debounce )
        {
          mainWidget->ProcessInput();
        } else if( s_debounce && !(lInput->GetInputState( LI_MOUSE_BUTTON1 ) || lInput->GetInputState( LI_MOUSE_BUTTON2 ) ) )
        {
          lLogFmt("Debounce menu\n");
          s_debounce = false;
        }
      }
    }
  }

  // Tick

  for( int i=0;i<m_uiManager->GetNumLayers();i++ )
  {
    lUILayer  * layer  = m_uiManager->GetLayer( i );
    lUI       * ui     = layer->GetTopUI();

    if( ui )
    {
      ui->Tick( delta );

      lUIWidget * mainWidget = ui->GetScreen();
      lUIWidget * subWidget  = ui->GetSubScreen();

      // Tick the main screen widget, and tick it.

      if( mainWidget )
      {
        lUIWidget * widget = mainWidget;

        while( widget )
        {
          widget->Tick(delta);
          widget = widget->NextNode();
        }
      }

      // Tick sub-screen, if it's a different widget.

      if( subWidget != mainWidget )
      {
        lUIWidget * widget = subWidget;

        while( widget )
        {
          widget->Tick(delta);
          widget = widget->NextNode();
        }
      }
    }
  }
}

// HACK:
// instead, use the tree recursion to skip nodes and their children
static bool IsVisible( lUIWidget * widget )
{
  while( widget )
  {
    if( !widget->IsVisible() ) return false;
    widget = widget->GetParent();
  }

  return true;
}

void FDGameWorld::RenderClientView( lGameClientView * view, unsigned int flags, lrCamera * cullCamera )
{
  if( view == NULL ) return;

  for( int i=0;i<m_uiManager->GetNumLayers();i++ )
  {
    lUILayer  * layer  = m_uiManager->GetLayer( i );
    lUI       * ui     = layer->GetTopUI();

    if( ui )
    {
      lUIWidget * mainWidget = ui->GetScreen();
      lUIWidget * subWidget  = ui->GetSubScreen();

      // Render main screen

      if( lSpriteManager::GetNumDisplays() == 1 || lSpriteManager::GetDisplayIndex() == 1 )
      {
        lUIWidget * widget = mainWidget;

        while( widget )
        {
          if( IsVisible( widget ) )
          {
            widget->Render();
          }

          widget = widget->NextNode();
        }
      }

      // Render sub screen

      if( lSpriteManager::GetNumDisplays() == 1 || lSpriteManager::GetDisplayIndex() == 0 )
      {
        lUIWidget * widget = subWidget;

        while( widget )
        {
          if( IsVisible( widget ) )
          {
            widget->Render();
          }

          widget = widget->NextNode();
        }
      }
    }
  }


  // depending on what sub-game we're in, we'll render leafs through that.
  //
  // to actually render stuff, call:
  view->GetClient()->RenderLeafs( NULL, NULL, 0 );
  //this allows the editor to intercept the leaf renders to attach widgets and edits to them.
}

lUIWidget * FDGameWorld::GetScreen()
{
  // TODO: for editing.
  return NULL;
}

lUIManager * FDGameWorld::GetUIManager()
{
  return m_uiManager;
}

void FDGameWorld::Debounce()
{
  s_debounce = true;
}

