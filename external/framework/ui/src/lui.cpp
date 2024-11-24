//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lui.cpp
// Description : lIU implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <ui/lui.h>
#include <framework/lspritemanager.h>

using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUI
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

lUI::lUI()
  : m_uiManager( NULL )
{
}

lUI::~lUI()
{
  for( int i=0;i<m_layerList.Size();i++ )
  {
    lLogFmt("UI %s being deleted and not removed from layer %s, removing now\n", GetId(), m_layerList(i)->GetId() );
    m_layerList(i)->RemoveUI( this );
  }

  if( m_uiManager )
  {
    lLogFmt("UI %s being deleted and not removed from UI manager, removing now\n", GetId() );
    m_uiManager->RemoveUI( GetId() );
  }
}

lUIManager * lUI::GetUIManager()
{
  return m_uiManager;
}

const char * lUI::GetId()
{
  return m_id;
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUILayer
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

lUILayer::lUILayer( lUIManager * uiManager, const char * id )
  : m_uiManager( uiManager ),
    m_id( id )
{
}

lUILayer::~lUILayer()
{
  while( GetTopUI() )
  {
    PopUI();
  }
}

const char * lUILayer::GetId()
{
  return m_id;
}


lUI * lUILayer::PushUI( lUI * ui )
{
  lAssert( ui != NULL );

  if( GetTopUI() )
  {
    lLogSys("lUILayer","PushUI: Deactivating %s\n", GetTopUI()->GetId() );
    GetTopUI()->Deactivate( this );
  }

  m_uiStack.Push( ui );
  ui->m_layerList.Push( this );

  lLogSys("lUILayer","PushUI: Activating %s\n", GetTopUI()->GetId() );
  GetTopUI()->Activate( this );

  return ui;
}

void lUILayer::PopUI()
{
  lAssert( GetTopUI() );

  if( GetTopUI() )
  {
    lLogSys("lUILayer","PopUI: Deactivating %s\n", GetTopUI()->GetId() );
    GetTopUI()->Deactivate( this );
  }

  m_uiStack.Last()->m_layerList.RemoveElement( this );
  m_uiStack.Pop();

  if( GetTopUI() )
  {
    lLogSys("lUILayer","PopUI: Activating %s\n", GetTopUI()->GetId() );
    GetTopUI()->Activate( this );
  }
}

void lUILayer::RemoveUI( lUI * ui )
{
  lAssert( ui != NULL );
  lAssert( GetTopUI() != NULL );

  if( ui == GetTopUI() )
  {
    lLogSys("lUILayer","RemoveUI: %s is on top, popping\n", ui->GetId() );
    PopUI();
    return;
  }

  for( int i=0;i<m_uiStack.Size();i++ )
  {
    if( m_uiStack(i) == ui )
    {
      lAssert( i != (m_uiStack.Size()-1) );

      ui->m_layerList.RemoveElement( this );
      m_uiStack.Remove(i);
      return;
    }
  }

  lLogSys("lUILayer","RemoveUI: %s not in stack\n", ui->GetId() );


  lAssert( 0 );
}

lUI * lUILayer::FindUI( const char * id )
{
  for( int i=0;i<m_uiStack.Size();i++ )
  {
    for( int i=0;i<m_uiStack.Size();i++ )
    {
      if( strcmpi( m_uiStack(i)->GetId(), id ) == 0 )
      {
        return m_uiStack(i);
      }
    }
  }

  return NULL;
}

lUI * lUILayer::GetTopUI()
{
  if( m_uiStack.Size() == 0 ) return NULL;
  return m_uiStack.Last();
}

int lUILayer::GetNumUIs()
{
  return m_uiStack.Size();
}

lUI * lUILayer::GetUI( int index )
{
  return m_uiStack( index );
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIManager
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

lUIManager::lUIManager()
{
}

lUIManager::~lUIManager()
{
}

lUILayer * lUIManager::PushLayer( const char * id )
{
  lLogSys("lUIManager","Pushing Layer %s\n", id );

  lUILayer * newLayer = new lUILayer( this, id );
  lAssert( strcmp( newLayer->GetId(), id ) == 0 );
  m_layerList.Push( newLayer );

  return newLayer;
}

void lUIManager::PopLayer()
{
  RemoveLayer( m_layerList.Last() );
}

void lUIManager::RemoveLayer( lUILayer * layer )
{
  for( int i=0;i<m_layerList.Size();i++ )
  {
    if( m_layerList(i) == layer )
    {
      m_layerList.Remove(i);
      break;
    }
  }

  lLogSys("lUIManager","Removing layer %s\n", layer->GetId() );

  delete layer;
}

lUILayer * lUIManager::FindLayer( const char * id )
{
  for( int i=0;i<m_layerList.Size();i++ )
  {
    for( int i=0;i<m_layerList.Size();i++ )
    {
      if( strcmpi( m_layerList(i)->GetId(), id ) == 0 )
      {
        return m_layerList(i);
      }
    }
  }

  return NULL;
}

int lUIManager::GetNumLayers()
{
  return m_layerList.Size();
}

lUILayer * lUIManager::GetLayer( int index )
{
  return m_layerList(index);
}

void lUIManager::AddUI( const char * id, lUI * ui )
{
//  lLogFmt("\n\n");
  lLogSys("lUIManager","Adding UI %s\n", id );
//  for( int i=0;i<m_uiList.Size();i++ )
//  {
//    lLogFmt("%d: %s\n", i, m_uiList(i)->GetId() );
//  }

  //assert id doesnt not already exist
  for( int i=0;i<m_uiList.Size();i++ )
  {
    if( m_uiList(i)->GetId() != NULL )
    {
      if( strcmpi( m_uiList(i)->GetId(), id ) == 0 )
      {
        lAssert("lUIManager, id already exists");
      }
    } else
    {
      lLogFmt("UIList %d == NULL\n", i);
    }
  }

  ui->m_uiManager = this;
  ui->m_id = id;
  m_uiList.Push( ui );

  lLogSys("lUIManager","Init UI %s\n", ui->GetId() );
  ui->Init();
//  lLogFmt("\n\n");
}

lUI * lUIManager::FindUI( const char * id )
{
  for( int i=0;i<m_uiList.Size();i++ )
  {
    for( int i=0;i<m_uiList.Size();i++ )
    {
      if( strcmpi( m_uiList(i)->GetId(), id ) == 0 )
      {
        return m_uiList(i);
      }
    }
  }

  return NULL;
}

void lUIManager::RemoveUI( const char* id )
{
//  lLogFmt("\n\n");
  lLogFmt("Removing ui '%s' : %d\n", id, m_uiList.Size() );
  //for( int i=0;i<m_uiList.Size();i++ )
//  {
//    lLogFmt("%d: %s\n", i, m_uiList(i)->GetId() );
//  }

  for( int i=0;i<m_uiList.Size();i++ )
  {
    if( m_uiList(i)->GetId() != NULL )
    {
      if( strcmpi( m_uiList(i)->GetId(), id ) == 0 )
      {
        lLogSys("lUIManager","Remove UI %s\n", id );
        m_uiList(i)->m_uiManager = NULL;
        m_uiList.Remove(i);
        return;
      }
    } else
    {
      lLogFmt("UIList %d == NULL\n", i);
    }
  }

  lLogSys("lUIManager","ERROR: Couldn't find UI %s\n", id );
//  lLogFmt("\n\n");
}

int lUIManager::GetNumUIs()
{
  return m_uiList.Size();
}

lUI * lUIManager::GetUI( int index )
{
  return m_uiList( index );
}

int lUIManager::GetScreenWidth()
{
  return lSpriteManager::GetDisplayWidth();
}

int lUIManager::GetScreenHeight()
{
  return lSpriteManager::GetDisplayHeight();
}



