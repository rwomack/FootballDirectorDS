//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdfrontend.cpp
// Description : FDFrontEnd implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdfrontend.h>

#include <ui/lui.h>
#include <ui/luiwidget.h>

#include <fdmainmenuui.h>

#include <fdresourcemanager.h>
#include <fdscript.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDFrontEnd
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

FDFrontEnd::FDFrontEnd()
  : m_bFirstTime(true)
{

}

FDFrontEnd::~FDFrontEnd()
{
}

void FDFrontEnd::Init()
{
  lLogFmt("FDFrontEnd::Init\n");

  lUIManager * uiManager = GetWorld()->GetUIManager();

  // Create the menu UI's

  lUI * m_mainMenu      = FD_GAME_NEW FDMainMenuUI();

  // Register the UI's

  uiManager->AddUI( "main_menu",  m_mainMenu );
}

void FDFrontEnd::Activate()
{
  lUIManager * uiManager = GetWorld()->GetUIManager();

  // Add menu layer

  m_layer = uiManager->PushLayer( "main" );

  if( m_bFirstTime )
  {
    m_bFirstTime = false;
    m_layer->PushUI( uiManager->FindUI( "main_menu" ) );
  }
  else
  {
    m_layer->PushUI( uiManager->FindUI( "main_menu" ) );
  }
}

void FDFrontEnd::Deactivate()
{
  lUIManager * uiManager = GetWorld()->GetUIManager();

  uiManager->RemoveLayer(m_layer);
}

void FDFrontEnd::Deinit()
{
  lLogFmt("FDFrontEnd::Deinit\n");
}

void FDFrontEnd::Tick( float delta )
{
}










