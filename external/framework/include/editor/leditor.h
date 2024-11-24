//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : leditor.h
// Description : lEditor
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LEDITOR_H_INCLUDED
#define LEDITOR_H_INCLUDED

#include <lemon/lapi.h>

#include <lemon/object/lobject.h>

#include <lemon/math/lmplane.h>
#include <lemon/math/lmray.h>
#include <lemon/math/lmmatrix.h>

#include <framework/lsigslot.h>

#include "../game/lgameclient.h"
#include "./ui/ui.h"

// Pre-declared classes

BEGIN_L_NAMESPACE
class lGameRenderContext;
class lGameLeaf;
class lGameWorld;
END_L_NAMESPACE

namespace lEditor
{
using namespace L_NAMESPACE_NAME;

class Editor;
class Plugin;
class ViewportPlugin;
class Viewport;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// MousePacket - move movement and click information
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct MousePacket
{
  struct ButtonStatus
  {
    unsigned int left:1;
    unsigned int right:1;
    unsigned int middle:1;
    unsigned int drag:1;
  };

  Editor       * editor;
  Viewport     * viewport;
  ButtonStatus   buttons;
  float          x;
  float          y;
  float          vpWidth;
  float          vpHeight;
  lmRay3         screenRay;
  lmPlane3       constructionPlane;

  bool GetConstructionPoint( lmVector3 & p ) const
  {
    float t;
    if ( screenRay.Intersect( constructionPlane, t ) )
    {
      p = screenRay.End(t);
      return true;
    }
    return false;
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// KeyPacket - Key down info
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct KeyPacket
{
  struct KeyStatus
  {
    union
    {
      unsigned int packed;

      struct
      {
        unsigned int repeat:16;
        unsigned int scancode:8;
        unsigned int ext:1;
        unsigned int reserved:3;
        unsigned int context:1;
        unsigned int prevkey:1;
        unsigned int trans:1;
      } unp;
    };
  };

  Editor     * editor;
  Viewport   * viewport;
  int          vkey;
  KeyStatus    keystatus;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// CommandMode - User command mode
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class CommandMode
{
public:
  virtual ~CommandMode() {}
  virtual void    EnterCommandMode() = 0;
  virtual void    ExitCommandMode() = 0;
  virtual void    CommandModeMousePacket( const MousePacket & ) = 0;
  virtual void    CommandModeKeyPacket( const KeyPacket & ) = 0;
  virtual HCURSOR GetCommandModeCursor() = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// MouseCreator - Mouse based object creator
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class MouseCreator
{
public:
  virtual ~MouseCreator() {}

  enum Status
  {
    CreateAbort,
    CreateContinue,
    CreateComplete,
  };

  virtual Status MouseCreatorMousePacket( const MousePacket & ) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Proxy - Proxy for a leaf
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class Proxy
{
public:
  virtual ~Proxy() {}
  virtual void              ProxyRender( lGameRenderContext *, lGameLeaf * ) = 0;
  virtual bool              ProxyTraceRay( lGameLeaf *, const lmRay3 &ray, float &tnear ) = 0;
  virtual MouseCreator   *  BeginMouseCreate( lGameLeaf * ) = 0;
  virtual void              EndMouseCreate( MouseCreator * ) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Widget - Visible widget for a leaf
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class Widget
{
public:
  virtual ~Widget() {}
  virtual void              WidgetRender( lGameLeaf * ) = 0;
  virtual bool              WidgetTraceRay( lGameLeaf *, const lmRay3 &ray, float &tnear ) = 0;
  virtual MouseCreator   *  WidgetBeginMouseCreate( lGameLeaf * ) = 0;
  virtual void              WidgetEndMouseCreate( MouseCreator * ) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Edit - Edit stack item for a leaf
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class Edit
{
public:
  virtual ~Edit() {};
  virtual void EditBegin( lGameLeaf * ) = 0;
  virtual void EditEnd() = 0;
  virtual void EditChange() = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Editor - Editor interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class Editor : public lGameClient, public lHasSlots<>
{
public:
  Editor( lGame * game, Proxy * defaultProxy, CommandMode * defaultCommandMode );
  virtual ~Editor();

  // lGameClient interface

  virtual void ClientInit();
  virtual void ClientDeinit();
  virtual void Tick(float);
  virtual void Render();
  virtual void RenderLeafs( lGameRenderContext * ctx, lGameLeaf ** leafList, int numLeafs );

  // Editor interface

  UI    * GetUI() const;
  void    RedrawViewport();

  // Plugins

  void  PluginAdd( Plugin * );
  void  PluginRemove( Plugin * );

  // Proxies

  void    ProxyAdd( Proxy *, lClass * );
  void    ProxyRemove( Proxy * );
  Proxy * GetProxy( lGameLeaf * );
  Proxy * GetClassProxy( lClass * );
  Proxy * GetDefaultProxy();

  // Widgets

  void  WidgetAdd( Widget *, lClass * );
  void  WidgetRemove( Widget * );
  void  GetClassWidgetSet( lClass *, lArray<Widget *> & );
  void  GetWidgetSet( lGameLeaf *, lArray<Widget *> & );

  // Edits

  void EditAdd( Edit *, lClass * );
  void EditRemove( Edit * );
  void PostEditChange( lGameLeaf * leaf );

  // Command mode

  void          SetCommandMode( CommandMode * );
  CommandMode * GetCommandMode();
  void          DeleteCommandMode( CommandMode * );

  // Tick

  void TickEnable(bool state);
  void PlayEnable(bool state);
  bool IsTicking();
  bool IsPlaying();

  // Selection

  void                         Select( lGameLeaf *, bool bAppend = false );
  void                         SelectToggle( lGameLeaf *, bool bAppend = false );
  void                         Deselect( lGameLeaf * );
  bool                         IsSelected( lGameLeaf * ) const;
  lGameLeaf                  * GetSelected() const;
  const lArray<lGameLeaf *>  & GetSelectedMultiple() const;

  // Messages

  void SendTick();
  void SendPaint();
  void SendMousePacket( Viewport * vp, int x, int y, MousePacket::ButtonStatus );
  void SendKeyPacket( Viewport * vp, int vKey, int keyInfo );

  // Signals

  lSignal1<MousePacket &>       SignalMouseModify;
  lSignal1<const MousePacket &> SignalMouse;
  lSignal0<>                    SignalPrePaint;
  lSignal0<>                    SignalPostPaint;
  lSignal1<lGameLeaf *>         SignalSelect;
  lSignal0<>                    SignalPreSave;
  lSignal0<>                    SignalWorldChange;

  // World

  lGameWorld * GetWorld();

  // Viewports

  void AddViewport( Viewport * );
  void RemoveViewport( Viewport * );

protected:
  UI                    * m_ui;
  lGame                 * m_game;
  lArray<Plugin *>        m_pluginList;

  CommandMode           * m_commandMode;
  lArray<CommandMode *>   m_commandModeStack;

  lArray<lGameLeaf *>     m_selectionList;

  bool                    m_bIsTicking;
  bool                    m_bIsPlaying;
  double                  m_fLastTime;

  struct ProxyMap
  {
    lClass * objectClass;
    Proxy  * proxy;
  };

  struct WidgetMap
  {
    lClass * objectClass;
    Widget * widget;
  };

  struct EditMap
  {
    lClass * objectClass;
    Edit   * edit;
  };

  lArray<ProxyMap>   m_proxyList;
  lArray<WidgetMap>  m_widgetList;
  lArray<EditMap>    m_editList;
  lArray<Edit *>     m_editStack;

  lArray<Viewport *> m_viewportList;

  // Defaults

  Proxy       * m_defaultProxy;
  CommandMode * m_defaultCommandMode;

  // Slots

  void OnWorldDestroyLeafs(lGameLeaf ** leafsToDestroy, int numLeafs);

  void ProcessSingleSelection(lGameLeaf * leaf);
  void SetUITitle();
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Plugin
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class Plugin : public lHasSlots<>
{
public:
  virtual ~Plugin() {}
  virtual void Init(Editor *) = 0;
  virtual void Deinit()       = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// ViewportPlugin
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class ViewportPlugin : public Plugin
{
public:
  virtual const char * ViewportName() = 0;
  virtual Viewport   * CreateViewport() = 0;
  virtual void         DestroyViewport(Viewport *) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Viewport
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class Viewport : public UITool
{
public:
  virtual ~Viewport() {}
  virtual HWND         GetHWND() = 0;
  virtual unsigned int GetWidth() = 0;
  virtual unsigned int GetHeight() = 0;
  virtual lmMatrix4    GetCameraMatrix() = 0;
  virtual lmMatrix4    GetProjectionMatrix() = 0;
  virtual lmPlane3     GetConstructionPlane() = 0;
  virtual lmRay3       EyeRay() = 0;
  virtual lmRay3       PickRay(int x, int y) = 0;
  virtual void         Paint() = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// PluginRegistrar
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class PluginRegistrar
{
public:
  PluginRegistrar( Plugin * (*pfGetInstance)(), const char * name );

  static PluginRegistrar * GetHead();

  PluginRegistrar        * GetNext();
  Plugin                 * GetInstance();
  const char             * GetName();

private:
  Plugin * (*m_pfGetInstance)();
  static PluginRegistrar * s_head;
  PluginRegistrar * m_next;
  const char * m_name;
};

#define L_EDITOR_REGISTER_PLUGIN(_x) static Plugin * _x##GetInstance() { static _x * plugin = new _x; return plugin; } \
                                     static PluginRegistrar _x##_autoregister( &_x##GetInstance, #_x );

}
#endif

