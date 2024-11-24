//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : ui.h
// Description : Lemon Editor UI
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef EDITOR_UI_H_INCLUDED
#define EDITOR_UI_H_INCLUDED

#include <lemon/lapi.h>
#include <framework/lsigslot.h>
#include <lemon/containers/lstring.h>
#include <wtypes.h> // for HWND

namespace lEditor
{
using namespace L_NAMESPACE_NAME;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Classes
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class UI;
class UIMenu;
class UIButton;
class UIButtonGroup;
class UITool;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// UI - User interface class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

typedef void UITimerFunc( unsigned int timerId, void * pUserData );

class UI
{
public:

  UI();
  ~UI();

  HWND                GetHWND();

  void                SetTitle( const char * );
  void                SetStatus( const char *, bool bViewport = false );

  UIButtonGroup  *    CreateButtonGroup( bool insert = false );
  UIMenu         *    CreateMenu(const char *, unsigned int = 0);

  enum DockSide
  {
    DockSideLeft,
    DockSideRight,
    DockSideTop,
    DockSideBottom,
    DockHide,
    DockFloat,
    DockLastKnown,
  };

  enum DockFlag
  {
    DockFlag_NoLeft      = 1<<0,
    DockFlag_NoRight     = 1<<1,
    DockFlag_NoTop       = 1<<2,
    DockFlag_NoBottom    = 1<<3,
    DockFlag_NoSplitter  = 1<<4,
    DockFlag_NoFloat     = 1<<5,
    DockFlag_NoHide      = 1<<6,
    DockFlag_NoFloatSize = 1<<7
  };

  void                AddClientTool(UITool * );
  void                RemoveClientTool(UITool *);

  void                AddTool(UITool *, const char * caption, DockSide = DockFloat, unsigned int flags = 0 );
  void                RemoveTool(UITool *);
  void                DockTool( UITool *, DockSide = DockFloat );
  void                ShowTool( UITool * );

  void                ShowRolloutToolBox();
  void                HideRolloutToolBox();
  void                AddRollout(UITool *, const char * caption );
  void                RemoveRollout(UITool *);
  static int          GetRolloutWidth();

  unsigned int        AddTimer( int timeMS, UITimerFunc * timerCallback, void * pUserData );
  void                RemoveTimer( unsigned int id );

  void                MessageOk( const char * title, const char * statement );
  bool                MessageYesNo( const char * title, const char * question );
  bool                OpenFile(lString &fileName, const char * ext, const char * filter);
  bool                SaveFile(lString &fileName, const char * ext, const char * filter);


private:
  class UIMainFrame;
  UIMainFrame * m_frame;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// UIMenu
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class UIMenu
{
public:
  virtual ~UIMenu() {}
  virtual void RemoveAllItems() = 0;
  virtual void AddItem( int id, const char * name, bool checkable = false, bool checked = false ) = 0;
  virtual void Activate() = 0;
  virtual void Deactivate() = 0;
  virtual bool GetChecked( int id ) = 0;
  virtual bool SetChecked( int id, bool state ) = 0;
  lSignal1<int> SignalItemSelect;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// UIButton
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class UIButton
{
public:
  virtual ~UIButton() {}
  virtual void SetChecked( bool ) = 0;
  virtual bool GetChecked( ) = 0;
  virtual void SetEnabled( bool ) = 0;
  virtual bool GetEnabled( ) = 0;
  lSignal0<> SignalPressed;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// UIButtonGroup
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class UIButtonGroup
{
public:
  virtual ~UIButtonGroup() {}
  virtual UIButton * CreateButton(const char * name, int order = 0, const char * help = NULL, const char * image = NULL) = 0;
  virtual void       DestroyButton( UIButton * ) = 0;
  virtual void       Activate() = 0;
  virtual void       Deactivate() = 0;
  virtual void       ToggleChecked( UIButton * ) = 0;
  virtual void       DisableAll() = 0;
  virtual void       EnableAll() = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// UIButtonBar
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class UIButtonBar
{
public:
  virtual ~UIButtonBar() {}
  virtual UIButtonGroup * CreateButtonGroup( bool insert = false ) = 0;
  virtual void            DestroyButtonGroup( UIButtonGroup * ) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// UITool
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class UITool
{
public:
  virtual ~UITool() {}
  virtual HWND GetHWND() = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// UIToolBox
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class UIToolBox
{
public:
  virtual ~UIToolBox() {}
  virtual void AddTool( UITool *, const char * caption ) = 0;
  virtual void RemoveTool( UITool * ) = 0;
};



}
#endif

