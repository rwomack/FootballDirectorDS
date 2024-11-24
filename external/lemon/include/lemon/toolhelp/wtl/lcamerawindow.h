//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lcamerawindow.h
// Description : lCameraWindow WTL implementation
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef WTL_LCAMERAWINDOW_H_INCLUDED
#define WTL_LCAMERAWINDOW_H_INCLUDED

#include "../ltoolhelpapi.h"
#include "./lrenderwindow.h"

#include "../../render/lrenderhelp.h" // for lrCameraOrbit (can be fixed)
#include "../../math/lmray.h"         // lmRay3
#include "../../math/lmaabox.h"       // lmAABox3
#include "../../math/lmplane.h"       // lmPlane3
#include "../../math/lmmatrix.h"      // lmVector3

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lCameraWindow - Generic orbit camera window for WTL
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// WTL render window with a user controllable camera.
// Description:
//   Inherit from lCameraWindow and override RenderSurface to render your
//   surface. The default implementation clears the screen and renders an XZ
//   grid using the DrawGridLines method. There is no requirement to call it
//   in the derived method.
//
//   Don't call buffer swap, that will be done automatically when the
//   window needs to be repainted.

template <class T, class TBase = ATL::CWindow, class TWinTraits = ATL::CFrameWinTraits>
class LTOOLHELP_API lCameraWindow : public lRenderWindow<T,TBase,TWinTraits>
{
public:

  BEGIN_MSG_MAP(lCameraWindow)
    #if ((_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400))
    MESSAGE_HANDLER(WM_MOUSEWHEEL,    OnMouseWheel)
    #endif //((_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400))
    MESSAGE_HANDLER(WM_MOUSEMOVE,     OnMouseMove)
    MESSAGE_HANDLER(WM_LBUTTONDOWN,   OnMouseButtonDown)
    MESSAGE_HANDLER(WM_RBUTTONDOWN,   OnMouseButtonDown)
    MESSAGE_HANDLER(WM_MBUTTONDOWN,   OnMouseButtonDown)
    MESSAGE_HANDLER(WM_LBUTTONUP,     OnMouseButtonUp)
    MESSAGE_HANDLER(WM_RBUTTONUP,     OnMouseButtonUp)
    MESSAGE_HANDLER(WM_MBUTTONUP,     OnMouseButtonUp)
    MESSAGE_HANDLER(WM_KEYDOWN,       OnKeyDown)

    CHAIN_MSG_MAP(lRenderWindow<T>)
  END_MSG_MAP()

  // lCameraWindow

  lCameraWindow();
  ~lCameraWindow();

  virtual void RenderSurface();

  void   Zoom(float mouseMotion);
  void   SetOffsetDistance(float offsetDist);
  void   EnableMouseCamera( bool enable );
  void   EnableKeyboardCamera( bool enable );
  void   SetMouseMoveMode();
  void   SetMouseRotateMode();
  void   SetMouseZoomMode();
  void   SetMouseNoMode();
  lmRay3 GetPickRay(int x, int y) const;
  lmRay3 GetEyeRay() const;
  void   FrameCamera( const lmAABox3 &frameBox );
  void   DrawSelectionBox( lmVector3 &pos, lmVector3 &halfDim, unsigned int  colour=0xffffffff );
  void   DrawGridLines( const lmPlane3 &constructionPlane,
                        int m_gridSpace = 10,
                        unsigned int m_colourMinorGrid  = L_ARGB(255,101,101,101),
                        unsigned int m_colourMajorGrid1 = L_ARGB(255,79,79,79),
                        unsigned int m_colourMajorGrid2 = L_ARGB(255,0,0,0) );

  // Message handlers

  #if ((_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400))
  LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  #endif

  LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
  LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnMouseButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnMouseButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
  bool             m_bMouseCapture;
  int              m_xMouse, m_xMouseMove;
  int              m_yMouse, m_yMouseMove;
  bool             m_bAllowCameraMovement;
  bool             m_bAllowKeyboardCameraMovement;
  bool             m_bDefaultCameraButtons;
  bool             m_bRotate;
  bool             m_bMove;
  bool             m_bZoom;
  lmRay3           m_curRay;
  lrCameraOrbit    m_cam;
  float            m_cameraFOV;
  float            m_cameraNear;
  float            m_cameraFar;
  int              m_cameraWidth;
  int              m_cameraHeight;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lCameraWindow implementation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <class T, class TBase, class TWinTraits>
inline lCameraWindow<T,TBase,TWinTraits>::lCameraWindow()
{
  m_bMouseCapture = false;
  m_xMouse        = 0;
  m_yMouse        = 0;
  m_bAllowCameraMovement = true;
  m_bAllowKeyboardCameraMovement = true;

  m_cam._rotation(1) += lmDeg2Rad(-45.0f);
  m_cam._rotation(0) += lmDeg2Rad(45.0f);

  SetOffsetDistance( 35.0f );

  m_cameraFOV    = 90;
  m_cameraWidth  = 1;
  m_cameraHeight = 1;

  m_bDefaultCameraButtons = true;
  m_bRotate = false;
  m_bMove   = false;
  m_bZoom   = false;
}

template <class T, class TBase, class TWinTraits>
inline lCameraWindow<T,TBase,TWinTraits>::~lCameraWindow()
{
}

template <class T, class TBase, class TWinTraits>
inline void lCameraWindow<T,TBase,TWinTraits>::RenderSurface()
{
  lRender::BufferClear( L_ARGB(255,128,128,128) );

  WTL::CRect rc;
  GetClientRect(rc);
  m_cameraWidth  = rc.Width();
  m_cameraHeight = rc.Height();

  m_cam.SetProjection( m_cameraFOV, float(m_cameraWidth)/float(m_cameraHeight), m_cameraNear, m_cameraFar );
  m_cam.Update(1);
  m_cam.Select();

  lmPlane3 constructionPlane( lmVector3(0,1,0), 0 );
  DrawGridLines(constructionPlane);
}


#if ((_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400))
template <class T, class TBase, class TWinTraits>
inline LRESULT lCameraWindow<T,TBase,TWinTraits>::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  if( m_bAllowCameraMovement && !m_bMouseCapture )
  {
    float wheelDelta = (float)((short)HIWORD(wParam));  //positive for wheel-up; zoom-in

    Zoom( wheelDelta );

    Redraw();
  }
  else
  {
    bHandled = false;
    return 1;
  }
  return 0;
}
#endif

template <class T, class TBase, class TWinTraits>
inline void lCameraWindow<T,TBase,TWinTraits>::Zoom(float mouseMotion)
{
  float newOffsetDistance = m_cam._offsetDist  *  lmPow( 0.999f, mouseMotion );

  SetOffsetDistance( newOffsetDistance );
}

template <class T, class TBase, class TWinTraits>
inline void lCameraWindow<T,TBase,TWinTraits>::SetOffsetDistance(float offsetDist)
{
  m_cam._offsetDist = offsetDist;
  m_cameraNear = offsetDist * 0.1f;
  m_cameraFar = offsetDist * 100.0f;
}

template <class T, class TBase, class TWinTraits>
inline LRESULT lCameraWindow<T,TBase,TWinTraits>::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
  if( m_bAllowCameraMovement && m_bAllowKeyboardCameraMovement )
  {
    bool changed = false;
    lmVector3 vec(0,0,0);
    float val = 10.0f;

    if( GetKeyState( VK_RCONTROL ) & 0x8000 || GetKeyState( VK_LCONTROL ) & 0x8000 )
    {
      val = 50.0f;
    }

    if(wParam == VK_LEFT)     { vec.Init( -val, 0,  0 ); changed = true; }
    if(wParam == VK_RIGHT)    { vec.Init(  val, 0,  0 ); changed = true; }
    if(wParam == VK_UP)       { vec.Init(  0, 0,  val ); changed = true; }
    if(wParam == VK_DOWN)     { vec.Init(  0, 0, -val ); changed = true; }
    if(wParam == VK_HOME)     { vec.Init(  0, val,  0 ); changed = true; }
    if(wParam == VK_END)      { vec.Init(  0, -val, 0 ); changed = true; }

    if(changed)
    {
      vec.Rotate( m_cam._rotation );
      m_cam._player.Translate( vec );
      Redraw();
    }
  }

  return 0;
}

template <class T, class TBase, class TWinTraits>
inline LRESULT lCameraWindow<T,TBase,TWinTraits>::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  // if the user is just moving the mouse about

  if( !m_bMouseCapture )
  {
    m_xMouseMove = GET_X_LPARAM(lParam);
    m_yMouseMove = GET_Y_LPARAM(lParam);

    m_curRay = GetPickRay( m_xMouseMove, m_yMouseMove );
  }

  // if the user is dragging the mouse

  else
  {
    int curX = GET_X_LPARAM(lParam);
    int curY = GET_Y_LPARAM(lParam);

    // use the camera plane

    lmPlane3 panningPlane( m_cam._player.GetTranslate(), m_cam.localToWorld.GetRow<2>() );

    // calculate the mouse panning delta

    lmRay3 prevRay = GetPickRay( m_xMouse, m_yMouse );
    lmRay3 curRay = GetPickRay( curX, curY );
    m_curRay = curRay;
    float dist;

    if ( !prevRay.Intersect( panningPlane, dist ) )
    {
      return 0;
    }
    lmVector3 prevFocus = prevRay.End( dist );

    if ( !curRay.Intersect( panningPlane, dist ) )
    {
      return 0;
    }
    lmVector3 curFocus = curRay.End( dist );


    lmVector3 mouseDelta = curFocus - prevFocus;

    // deal with special case middle button

    bool redrawAll = false;

    if( (m_bMove&&(wParam&MK_LBUTTON)) || (wParam & MK_MBUTTON) )
    {
      m_cam._player.Translate( -mouseDelta );
    }
    else if( (m_bRotate&&(wParam&MK_LBUTTON)) || (wParam & MK_RBUTTON) )
    {
      m_cam._rotation(1) += lmDeg2Rad((float)curX-m_xMouse);
      m_cam._rotation(0) += lmDeg2Rad((float)curY-m_yMouse);
    }
    else if( (m_bZoom&&(wParam&MK_LBUTTON)))
    {
      Zoom( -(float)( curY - m_yMouse ) );
    }

    // deal with mouse wrapping

    WTL::CPoint pt;
    WTL::CRect rect;

    GetWindowRect( rect );
    pt.x = curX; pt.y = curY;
    ClientToScreen( &pt );

    if(pt.x <= 0)
    {
      mouse_event( MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, 65000, pt.y * (65535 / GetSystemMetrics(SM_CYSCREEN)), 0, NULL );
      curX = GetSystemMetrics(SM_CXSCREEN) - rect.left;
    }
    else if(pt.x >= GetSystemMetrics(SM_CXSCREEN) - 1)
    {
      mouse_event( MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, 1000, pt.y * (65536 / GetSystemMetrics(SM_CYSCREEN)), 0, NULL );
      curX = -rect.left;
    }
    else if(pt.y <= 0 )
    {
      mouse_event( MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, pt.x * (65535 / GetSystemMetrics(SM_CXSCREEN)), 65000, 0, NULL );
      curY = GetSystemMetrics(SM_CYSCREEN) - rect.top;
    }
    else if(pt.y >= GetSystemMetrics(SM_CYSCREEN) - 1)
    {
      mouse_event( MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, pt.x * (65535 / GetSystemMetrics(SM_CXSCREEN)), 1000, 0, NULL );
      curY = -rect.top;
    }

    m_xMouse = curX;
    m_yMouse = curY;

    Redraw();
  }

  return 0;
}

template <class T, class TBase, class TWinTraits>
inline LRESULT lCameraWindow<T,TBase,TWinTraits>::OnMouseButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  SetFocus();

  if( m_bAllowCameraMovement && (((wParam&MK_RBUTTON) || (wParam & MK_MBUTTON)) || !m_bDefaultCameraButtons) )
  {

    m_bMouseCapture = true;
    SetCapture();
    m_xMouse = GET_X_LPARAM(lParam);
    m_yMouse = GET_Y_LPARAM(lParam);

    if( m_bDefaultCameraButtons )
    {
      m_bMove = m_bRotate = m_bZoom = false;
      if( wParam & MK_MBUTTON ) m_bMove  = true;
      if( wParam & MK_RBUTTON ) m_bRotate = true;
    }

    Redraw();
    return 0;
  }

  bHandled = false;
  return 1;
}

template <class T, class TBase, class TWinTraits>
inline LRESULT lCameraWindow<T,TBase,TWinTraits>::OnMouseButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  if( m_bMouseCapture )
  {
    int curX = GET_X_LPARAM(lParam);
    int curY = GET_Y_LPARAM(lParam);

    m_bMouseCapture = false;
    ReleaseCapture();
  }
  else
  {
    bHandled = false;
    return 1;
  }
  return 0;
}

template <class T, class TBase, class TWinTraits>
inline lmRay3 lCameraWindow<T,TBase,TWinTraits>::GetPickRay(int x, int y) const
{
  WTL::CRect rc;
  GetClientRect( &rc );
  return m_cam.PixelRay( float(x), float(y), float(rc.Width()), float(rc.Height()) );
}

template <class T, class TBase, class TWinTraits>
inline lmRay3 lCameraWindow<T,TBase,TWinTraits>::GetEyeRay() const
{
  return m_cam.EyeRay();
}

template <class T, class TBase, class TWinTraits>
inline void lCameraWindow<T,TBase,TWinTraits>::FrameCamera( const lmAABox3 &frameBox )
{
  if( !frameBox.IsEmpty() )
  {
    float radius = frameBox.GetSize().Length() / 2.0;

    // calculate appropriate distance to have the extent project
    // completely into the camera view

    SetOffsetDistance( radius ); // / (2*(1.0/m_cameraNear));
    m_cam._player.SetTranslate( frameBox.GetCentre() );
  }
}

template <class T, class TBase, class TWinTraits>
inline void lCameraWindow<T,TBase,TWinTraits>::DrawSelectionBox( lmVector3 &pos, lmVector3 &halfDim, unsigned int  colour )
{
  lmVector3 eightDim = halfDim/4;

  lmVector3 dir[8] =
  {
    lmVector3(-1,-1,-1),
    lmVector3( 1,-1,-1),
    lmVector3(-1, 1,-1),
    lmVector3( 1, 1,-1),

    lmVector3(-1,-1, 1),
    lmVector3( 1,-1, 1),
    lmVector3(-1, 1, 1),
    lmVector3( 1, 1, 1),
  };

  for( int i=0;i<8;i++ )
  {
    lmVector3 corner;
    corner = halfDim.Scale( dir[i] );
    corner += pos;

    lrVertex vertList[4] =
    {
      lrVertex( corner,                                                    lmVector3(0,0,0), colour, 0,0 ),
      lrVertex( lmVector3(corner + lmVector3(eightDim(0)*-dir[i](0),0,0)), lmVector3(0,0,0), colour, 0,0 ),
      lrVertex( lmVector3(corner + lmVector3(0,eightDim(1)*-dir[i](1),0)), lmVector3(0,0,0), colour, 0,0 ),
      lrVertex( lmVector3(corner + lmVector3(0,0,eightDim(2)*-dir[i](2))), lmVector3(0,0,0), colour, 0,0 )
    };

    static unsigned short lineList[6] = {0,1, 0,2, 0,3};

    lRender::SetTextureResource( 0, NULL );
    lRender::SetDefaultShader();
    lRender::SetVertexDesc( &lrVertex::GetDesc() );

    lRender::SetVertexStream( 0, vertList, sizeof(lrVertex), sizeof(vertList), NULL );
    lRender::SetIndexStream( lineList, sizeof(lineList[0]), sizeof(lineList), NULL );
    lRender::PostGeometry( LR_PRIM_LINELIST, 0, sizeof(lineList) / sizeof(lineList[0]) );
  }
}

template <class T, class TBase, class TWinTraits>
inline void lCameraWindow<T,TBase,TWinTraits>::DrawGridLines( const lmPlane3 &constructionPlane,
                                                       int m_gridSpace,
                                                       unsigned int m_colourMinorGrid,
                                                       unsigned int m_colourMajorGrid1,
                                                       unsigned int m_colourMajorGrid2 )
{
  // get the camera transform
  lmRay3 eyeRay = m_cam.EyeRay();

  float dist = 0;

  eyeRay.Intersect( constructionPlane, dist );

  // calculate the focus point

  lmVector3 focusPoint = eyeRay.End( dist );

  // calculate the transformation for the construction plane,
  // we could special case grid drawing for certain axes, however, this is more general.

  int major = constructionPlane.m_normal.GetMajorAxis();

  lmVector3 up;
  if( major == 0 ) up = lmVector3( 0,1,0 );
  if( major == 1 ) up = lmVector3( 0,0,1 );
  if( major == 2 ) up = lmVector3( 0,1,0 );

  lmMatrix4 constructionTransform; // look from <0,0,0> along the construction normal, using the up vector
  constructionTransform.LookAt( lmVector3(0,0,0), up, lmVector3(constructionPlane.m_normal) );

  // calculate the view extents on the construction plane

  //lmVector3 extentPoint[4];
  //float      extentDist[4];
  //
  //lmMatrix3 invView = lmMatrix4::Get3x3(camMatrix);
  //
  //for( int i=0;i<4;i++ )
  //{
  //  lmVector3 extentDir( ((i&1)?1:-1)/m_cam.projectionMatrix(0,0), ((i&2)?-1:1)/m_cam.projectionMatrix(1,1), 1.0f );
  //  extentDir = invView * extentDir;
  //  extentDist[i] = 0;
  //  ctRayIsectPlane( ctSeg(eyeOrigin, extentDir, 10000), constructionPlane, &extentDist[i] );
  //  extentPoint[i] = eyeOrigin + (extentDir * extentDist[i]);
  //}
  //
  //m_font->Print3D( extentPoint[0], lrFont::AlignRight, lrFont::AlignBottom, "Top Left!" );
  //m_font->Print3D( extentPoint[1], lrFont::AlignLeft,  lrFont::AlignBottom, "Top Right!" );
  //m_font->Print3D( extentPoint[2], lrFont::AlignRight, lrFont::AlignTop, "Bottom Left!" );
  //m_font->Print3D( extentPoint[3], lrFont::AlignLeft,  lrFont::AlignTop, "Bottom Right!" );
  //dist = lmMax( lmVector3::Distance( extentPoint[0], extentPoint[1] ), lmVector3::Distance( extentPoint[0], extentPoint[2] ) );

  // calculate the grid spacing based on the camera distance

  if( dist == 0 )
  {
    m_gridSpace = 10;
  }
  else
  {
    for( m_gridSpace = 10; m_gridSpace<1000 ; m_gridSpace *= m_gridSpace )
    {
      if( m_gridSpace * 80 < dist )
      {
      }
      else
      {
        break;
      }
    }
  }

  // draw the grid

  static lrVertex vertList[2] =
  {
    lrVertex( lmVector3(0,0,0), lmVector3(0,0,0), m_colourMinorGrid, 0,0 ),
    lrVertex( lmVector3(0,0,0), lmVector3(0,0,0), m_colourMinorGrid, 0,0 ),
  };

  static unsigned short lineList[2] = {0,1};

  int xGridSpace = m_gridSpace;
  int yGridSpace = m_gridSpace;
  int yMax       = yGridSpace*30;
  int yMin       = -yMax;
  int xMax       = xGridSpace*30;
  int xMin       = -xMax;

  lRender::SetTextureResource( 0, NULL );
  lRender::SetDefaultShader();
  lRender::SetVertexDesc( &lrVertex::GetDesc() );

  #define RND( _x, _y ) ( ( (_x>0 ? (int)((_x+(_y/2))) : (int)((_x-(_y/2))) )  / _y ) * (int)_y )

  xMin += RND( focusPoint(0), (m_gridSpace * 20) );
  xMax += RND( focusPoint(0), (m_gridSpace * 20) );
  yMin += RND( focusPoint(2), (m_gridSpace * 20) );
  yMax += RND( focusPoint(2), (m_gridSpace * 20) );

  for( int y=yMin;y<=yMax;y+=yGridSpace )
  {
    if( y % (yGridSpace * yGridSpace * yGridSpace) == 0 )
    {
      vertList[0].colour = vertList[1].colour = m_colourMajorGrid2;
    }
    else if( y % (yGridSpace * yGridSpace) == 0 )
    {
      vertList[0].colour = vertList[1].colour = m_colourMajorGrid1;
    }
    else
    {
      vertList[0].colour = vertList[1].colour = m_colourMinorGrid;
    }

    vertList[0].p = lmVector3( float(xMin), float(y), 0.0f ) * constructionTransform;
    vertList[1].p = lmVector3( float(xMax), float(y), 0.0f ) * constructionTransform;

    lRender::SetVertexStream( 0, vertList, sizeof(lrVertex), sizeof(vertList), NULL );
    lRender::SetIndexStream( lineList, sizeof(lineList[0]), sizeof(lineList), NULL );
    lRender::PostGeometry( LR_PRIM_LINELIST, 0, sizeof(lineList) / sizeof(lineList[0]) );
  }

  for( int x=xMin;x<=xMax;x+=xGridSpace )
  {
    if( x % (xGridSpace * xGridSpace * xGridSpace) == 0 )
    {
      vertList[0].colour = vertList[1].colour = m_colourMajorGrid2;
    }
    else if( x % (xGridSpace * xGridSpace) == 0 )
    {
      vertList[0].colour = vertList[1].colour = m_colourMajorGrid1;
    }
    else
    {
      vertList[0].colour = vertList[1].colour = m_colourMinorGrid;
    }

    vertList[0].p = lmVector3( float(x), float(yMin), 0.0f ) * constructionTransform;
    vertList[1].p = lmVector3( float(x), float(yMax), 0.0f ) * constructionTransform;

    lRender::SetVertexStream( 0, vertList, sizeof(lrVertex), sizeof(vertList), NULL );
    lRender::SetIndexStream( lineList, sizeof(lineList[0]), sizeof(lineList), NULL );
    lRender::PostGeometry( LR_PRIM_LINELIST, 0, sizeof(lineList) / sizeof(lineList[0]) );
  }
}

template <class T, class TBase, class TWinTraits>
inline void lCameraWindow<T,TBase,TWinTraits>::EnableMouseCamera( bool enable )
{
  m_bAllowCameraMovement = enable;
}

template <class T, class TBase, class TWinTraits>
inline void lCameraWindow<T,TBase,TWinTraits>::EnableKeyboardCamera( bool enable )
{
  m_bAllowKeyboardCameraMovement = enable;
}

template <class T, class TBase, class TWinTraits>
inline void lCameraWindow<T,TBase,TWinTraits>::SetMouseMoveMode()
{
  m_bMove = true;
  m_bRotate = false;
  m_bZoom = false;
}

template <class T, class TBase, class TWinTraits>
inline void lCameraWindow<T,TBase,TWinTraits>::SetMouseRotateMode()
{
  m_bMove = false;
  m_bRotate = true;
  m_bZoom = false;
}

template <class T, class TBase, class TWinTraits>
inline void lCameraWindow<T,TBase,TWinTraits>::SetMouseZoomMode()
{
  m_bMove = false;
  m_bRotate = false;
  m_bZoom = true;
}

template <class T, class TBase, class TWinTraits>
inline void lCameraWindow<T,TBase,TWinTraits>::SetMouseNoMode()
{
  m_bMove = false;
  m_bRotate = false;
  m_bZoom = false;
}


END_L_NAMESPACE
#endif
