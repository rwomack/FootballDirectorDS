//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lu.h
// Description : lUI interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LUI_H_INCLUDED
#define LUI_H_INCLUDED

#include <lemon/lapi.h>
#include <lemon/containers/lstring.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lUIWidget;

class lUI;
class lUILayer;
class lUIManager;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUI
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lUI
{
public:

  lUI();
  virtual ~lUI();

  // lUI interface

  virtual void        Init() = 0;
  virtual void        Deinit() = 0;
  virtual void        Activate( lUILayer * ) = 0;
  virtual void        Deactivate( lUILayer * ) = 0;
  virtual lUIWidget * GetScreen() = 0;
  virtual lUIWidget * GetSubScreen() { return NULL; } // HACK: porting aid, should be pure virtual.
  virtual void        Tick(float delta) = 0;

  // lUI implementation

  lUIManager  * GetUIManager();
  const char  * GetId();

private:
  friend class lUIManager;
  friend class lUILayer;
  lString      m_id;
  lUIManager * m_uiManager;
  lArray<lUILayer *> m_layerList;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUILayer
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lUILayer
{
public:
  lUILayer( lUIManager * uiManager, const char * id );
  ~lUILayer();

  const char * GetId();
  lUI        * PushUI( lUI * );
  void         PopUI();
  void         RemoveUI( lUI * );
  lUI        * FindUI( const char * id );
  lUI        * GetTopUI();
  int          GetNumUIs();
  lUI        * GetUI( int index );

private:
  lString         m_id;
  lUIManager    * m_uiManager;
  lArray<lUI *>   m_uiStack;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIManager
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lUIManager
{
public:
  lUIManager();
  ~lUIManager();

  lUILayer * PushLayer( const char * id );
  void       PopLayer();
  void       RemoveLayer( lUILayer * );
  lUILayer * FindLayer( const char * id );
  int        GetNumLayers();
  lUILayer * GetLayer( int index );

  void       AddUI( const char * id, lUI * ui );
  lUI      * FindUI( const char * id );
  void       RemoveUI( const char* id );
  int        GetNumUIs();
  lUI      * GetUI( int index );

  int        GetScreenWidth();
  int        GetScreenHeight();

private:
  lArray<lUILayer *> m_layerList;
  lArray<lUI *>      m_uiList;
};



END_L_NAMESPACE
#endif
