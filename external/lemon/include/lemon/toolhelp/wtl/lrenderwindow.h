//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrenderwindow.h
// Description : lRenderWindow WTL implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef WTL_LRENDERWINDOW_H_INCLUDED
#define WTL_LRENDERWINDOW_H_INCLUDED

#include "../ltoolhelpapi.h"
#include "../../render/lrender.h"

#include "./latl.h"
#include <atlwin.h>     // ATL
#include <atlmisc.h>    // WTL
#include <atlctrls.h>   // WTL
#include <atlframe.h>   // WTL

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lRenderWindow - Generic render window for WTL
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// WTL window for an lRender surface
// Description:
//   Inherit from lRenderWindow and use CHAIN_MSG_MAP() at the *top* of the message
//   map to let lRenderWindow handle managment of the render surface within the
//   window. All messages handled by lRenderWindow will always fall through to the
//   implementation in child class.
//
//   class MyWindow : public lRenderWindow<MyWindow>
//   {
//   public:
//     DECLARE_WND_CLASS_EX(NULL, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, COLOR_BTNFACE);
//
//     BEGIN_MSG_MAP(MyWindow)
//       CHAIN_MSG_MAP(lRenderWindow<MyWindow>)
//       MESSAGE_HANDLER(WM_CREATE,        OnCreate)
//       MESSAGE_HANDLER(WM_CLOSE,         OnClose)
//       MESSAGE_HANDLER(WM_DESTROY,       OnDestroy)
//     END_MSG_MAP()
//   }
//
//   Call the base member Redraw() to trigger a fresh redraw of the window.
//   Override RenderSurface to render your surface. The default implementation
//   simply clears the screen with lRender::BufferClear so there is no requirement
//   to call it.

template <class T, class TBase = ATL::CWindow, class TWinTraits = ATL::CFrameWinTraits>
class LTOOLHELP_API lRenderWindow : public WTL::CFrameWindowImpl<T,TBase,TWinTraits>
{
public:

  BEGIN_MSG_MAP(lRenderWindow)
    MESSAGE_HANDLER(WM_DESTROY,       OnDestroy)
    MESSAGE_HANDLER(WM_SIZE,          OnSize)
    MESSAGE_HANDLER(WM_ERASEBKGND,    OnEraseBackground)
    MESSAGE_HANDLER(WM_PAINT,         OnPaint)
    MESSAGE_HANDLER(WM_SETFOCUS,      OnFocus)
    MESSAGE_HANDLER(WM_KILLFOCUS,     OnFocus)
    MESSAGE_HANDLER(WM_ENTERSIZEMOVE, OnEnterSizeMove)
    MESSAGE_HANDLER(WM_EXITSIZEMOVE,  OnExitSizeMove)
  END_MSG_MAP()

  lRenderWindow();
  ~lRenderWindow();

  // lRenderWindow interface

  virtual void RenderSurface();

  bool Redraw();
  void RenderWindowEnable( bool state );

  // Message handlers

  LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnEnterSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnExitSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
  void DoSizeChange();

  lrSurface  * m_pSurface;
  bool                m_bFocusRect;
  unsigned int        m_focusColour;
  unsigned int        m_fHasFocus:1;
  unsigned int        m_bRenderWindowEnabled;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lRenderWindow implementation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<class T, class TBase, class TWinTraits>
inline lRenderWindow<T,TBase,TWinTraits>::lRenderWindow()
  : m_bFocusRect(false),
    m_focusColour(0xff0000),
    m_bRenderWindowEnabled(true)
{
  m_pSurface = NULL;
}

template<class T, class TBase, class TWinTraits>
inline lRenderWindow<T,TBase,TWinTraits>::~lRenderWindow()
{
  if( m_pSurface ) lRender::SurfaceDestroy(m_pSurface);
  m_pSurface = NULL;
}

template<class T, class TBase, class TWinTraits>
inline void lRenderWindow<T,TBase,TWinTraits>::RenderWindowEnable( bool state )
{
  m_bRenderWindowEnabled = state;
}

template<class T, class TBase, class TWinTraits>
inline LRESULT lRenderWindow<T,TBase,TWinTraits>::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  bHandled = false;
  if( m_pSurface ) lRender::SurfaceDestroy(m_pSurface);
  m_pSurface = NULL;
  return 0;
}

template<class T, class TBase, class TWinTraits>
inline LRESULT lRenderWindow<T,TBase,TWinTraits>::OnEnterSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  m_bRenderWindowEnabled = false;
  return 0;
}

template<class T, class TBase, class TWinTraits>
inline LRESULT lRenderWindow<T,TBase,TWinTraits>::OnExitSizeMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  m_bRenderWindowEnabled = true;
  DoSizeChange();
  return 0;
}

template<class T, class TBase, class TWinTraits>
inline LRESULT lRenderWindow<T,TBase,TWinTraits>::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  bHandled = false;
  if( !IsWindowVisible() || !m_bRenderWindowEnabled ) return 0;

  if( m_pSurface != NULL )
  {
    if( wParam==SIZE_MAXHIDE || wParam==SIZE_MINIMIZED )
    {
      lRender::SurfaceEnable(m_pSurface,false);
      return 0;
    }
    else
    {
      lRender::SurfaceEnable(m_pSurface,true);
    }
  }

  if( wParam == SIZE_RESTORED )
  {
    DoSizeChange();
  }

  return 0;
}

template<class T, class TBase, class TWinTraits>
inline void lRenderWindow<T,TBase,TWinTraits>::DoSizeChange()
{
  WTL::CRect rc;
  GetClientRect(rc);

  if( rc.Width() > 0 && rc.Height() > 0 )
  {
    lrSurfaceDesc desc;
    desc.xRes         = rc.Width();
    desc.yRes         = rc.Height();
    desc.colourBits   = 24;
    desc.depthBits    = 16;
    desc.stencilBits  = 1;
    desc.caps         = LR_SURFACE_CAPS_PC_WINDOWED;
    desc.hWnd         = m_hWnd;

    if( m_pSurface == NULL )
    {
      m_pSurface = lRender::SurfaceCreate(desc);
    }
    else
    {
      if( ! lRender::SurfaceChange(m_pSurface,desc) )
      {
        lRender::SurfaceDestroy(m_pSurface);
        m_pSurface = lRender::SurfaceCreate(desc);
      }
    }
  }
  else
  {
    if( m_pSurface != NULL )
    {
      lRender::SurfaceDestroy(m_pSurface);
      m_pSurface = NULL;
    }
  }

  Redraw();
}

template<class T, class TBase, class TWinTraits>
inline LRESULT lRenderWindow<T,TBase,TWinTraits>::OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  return 1; // no background needed
}

template<class T, class TBase, class TWinTraits>
inline LRESULT lRenderWindow<T,TBase,TWinTraits>::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  bHandled = false;
  if( !m_bRenderWindowEnabled ) return 0;

  if( m_pSurface != NULL )
  {
    m_pSurface->Select();
    lRender::BufferSwap();
  }

  if( m_bFocusRect && GetFocus() == m_hWnd )
  {
    WTL::CPaintDC dc(m_hWnd);
    WTL::CRect rect;
    GetClientRect( rect );

    WTL::CBrush  brush;
    brush.CreateSolidBrush( 0xff0000 );
    dc.FrameRect(rect, brush);
    rect.DeflateRect( 1,1,1,1 );
    dc.FrameRect(rect, brush);
  }

  return 0;
}

template<class T, class TBase, class TWinTraits>
inline LRESULT lRenderWindow<T,TBase,TWinTraits>::OnFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  m_fHasFocus = (uMsg == WM_SETFOCUS) ? 1 : 0;
  Invalidate();
  UpdateWindow();
  bHandled = false;
  return 1;
}

template<class T, class TBase, class TWinTraits>
inline bool lRenderWindow<T,TBase,TWinTraits>::Redraw()
{
  if( !m_bRenderWindowEnabled ) return 0;
  if( m_pSurface == NULL ) return false;
  m_pSurface->Select();
  if( !lRender::IsReady() ) return false;

  RenderSurface();

  // cause the window to be be repained
  // used to be a BufferSwap(), however we need to
  // properly repain the focus rect if needed.
  Invalidate();
  RedrawWindow();

  return true;
}

template<class T, class TBase, class TWinTraits>
inline void lRenderWindow<T,TBase,TWinTraits>::RenderSurface()
{
  // default implementation of RenderSurface()

  lRender::BufferClear( L_ARGB(255,0,0,0) );
}

END_L_NAMESPACE

#endif
