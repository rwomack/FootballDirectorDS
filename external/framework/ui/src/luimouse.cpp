//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : luimouse.cpp
// Description : Mouse implementation
// Notes       : MOVE TO FRAMEWORK INPUT SYSTEM
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <ui/luimouse.h>
#include <lemon/input/linput.h>
#include <lemon/render/lrender.h>
#include <framework/lspritemanager.h>

#ifdef L_PLATFORM_NDS
#include <framework/dsrender.h>
#endif

#ifdef L_PLATFORM_WII
#include <revolution/wpad.h>
#endif

#if defined(L_PLATFORM_WINDOWS)
#include <framework/lspritemanager.h>
#endif

using namespace L_NAMESPACE_NAME;

static float s_fMouseX = 0;
static float s_fMouseY = 0;
static int   s_mouseX = 0;
static int   s_mouseY = 0;

static void UpdateMouse()
{
  int mX = int(lInput->GetMouseX());
  int mY = int(lInput->GetMouseY());

  #ifdef L_PLATFORM_NDS

  s_fMouseX = mX / 256.0f;
  s_fMouseY = mY / 192.0f;
  s_mouseX = mX;
  s_mouseY = mY;

  if( DSRender_GetBookMode() == 1 )
  {
    // left handed, rotate 270

    lSwap( s_fMouseX, s_fMouseY );
    s_fMouseX = 1.0f - s_fMouseX;

    lSwap( s_mouseX, s_mouseY );
    s_mouseX = 192 - s_mouseX;
  }
  else if( DSRender_GetBookMode() == 2 )
  {
    // right handed, rotate 90

    lSwap( s_fMouseX, s_fMouseY );
    s_fMouseY = 1.0f - s_fMouseY;

    lSwap( s_mouseX, s_mouseY );
    s_mouseY = 256 - s_mouseY;
  }

  #endif

  #ifdef L_PLATFORM_WII
  float mx = lInput->GetMouseX();
  float my = lInput->GetMouseY();

  float range = 0.5f;//amount of sensor bar range to use for mouse position
                     //(the higher the number the further you need to move your
                     //hand to get from one side of the screen to the other)
                     //I dont recommend going above 0.9f becuase the remote
                     //sometimes has trouble reading the sensor at the edges

  mx = mx + (2.7f*range);
  mx = mx / (5.4f*range);

  if( WPADGetSensorBarPosition() == WPAD_SENSOR_BAR_POS_BOTTOM )
  {
    //-1.2 - 2.0
    my = my + (1.2f*range);
    my = my / (3.2f*range);
  }
  else
  {
    //-2.0 - 1.2
    my = my + (2.0f*range);
    my = my / (3.2f*range);
  }


  if( mx > 1.0f ) mx = 1.0f;
  if( mx < 0.0f ) mx = 0.0f;
  if( my > (1.0f/lSpriteManager::GetHackScaleNTSCValue()) ) my = (1.0f/lSpriteManager::GetHackScaleNTSCValue());
  if( my < 0.0f ) my = 0.0f;

  s_fMouseX = mx;
  s_fMouseY = my;
  s_mouseX = mx * lRender::SurfaceGetCurrent()->GetDesc().xRes;
  s_mouseY = my * lRender::SurfaceGetCurrent()->GetDesc().yRes;
  #endif

  #ifdef L_PLATFORM_WINDOWS

  lrSurface * surface = lRender::SurfaceGetCurrent();
  HWND m_hWnd = (HWND)surface->GetDesc().hWnd;

  if( surface->GetDesc().hWnd == 0 )
  {
    m_hWnd = lRenderPC::GetHWnd();
  }

  POINT point;
  ::GetCursorPos( &point );

  float wndW;
  float wndH;

  if( lRender::SurfaceGetCurrent()->GetDesc().caps & LR_SURFACE_CAPS_PC_WINDOWED )
  {
    ::ScreenToClient( m_hWnd, &point );
    RECT clientRc;
    ::GetClientRect( m_hWnd, &clientRc );

    wndW = float(clientRc.right-clientRc.left);
    wndH = float(clientRc.bottom-clientRc.top);
  }
  else
  {
    wndW = float(lRender::SurfaceGetCurrent()->GetDesc().xRes);
    wndH = float(lRender::SurfaceGetCurrent()->GetDesc().yRes);
  }

  if( point.x >= 0 && point.x <= wndW &&
      point.y >= 0 && point.y <= wndH )
  {
    s_fMouseX = (float(point.x) / wndW);
    s_fMouseY = (float(point.y) / wndH);
  }

  if( lSpriteManager::IsEmulateDS() )
  {
    s_fMouseY -= 0.5f;

    if( s_fMouseY < 0.0f )
    {
      s_fMouseY = 0.0f;
    }

    s_mouseX = int(s_fMouseX * lRender::SurfaceGetCurrent()->GetDesc().xRes);
    s_mouseY = int(s_fMouseY * lRender::SurfaceGetCurrent()->GetDesc().yRes);

    s_fMouseY *= 2.0f;
  }
  else
  {
    s_mouseX = int(s_fMouseX * lRender::SurfaceGetCurrent()->GetDesc().xRes);
    s_mouseY = int(s_fMouseY * lRender::SurfaceGetCurrent()->GetDesc().yRes);
  }

  #endif
}

void lUIMouse::Tick(float delta)
{
  //tick for controller based mouse pointer
  #ifdef L_PLATFORM_PSP
  if( lInput->GetInputState( LI_PAD1_THUMBL_X_AXIS ) > 0.5f )
  {
    s_fMouseX += 0.5f*delta;
  }
  if( lInput->GetInputState( LI_PAD1_THUMBL_X_AXIS ) < -0.5f )
  {
    s_fMouseX -= 0.5f*delta;
  }
  if( lInput->GetInputState( LI_PAD1_THUMBL_Y_AXIS ) > 0.5f )
  {
    s_fMouseY += 0.4f*delta;
  }
  if( lInput->GetInputState( LI_PAD1_THUMBL_Y_AXIS ) < -0.5f )
  {
    s_fMouseY -= 0.4f*delta;
  }

  if( s_fMouseX < 0 ) s_fMouseX = 0;
  if( s_fMouseX > 1.0f ) s_fMouseX = 1.0f;
  if( s_fMouseY < 0 ) s_fMouseY = 0;
  if( s_fMouseY > 1.0f ) s_fMouseY = 1.0f;
  #endif
}

int lUIMouse::GetPixelX()
{
  UpdateMouse();
  return s_mouseX;
}

int lUIMouse::GetPixelY()
{
  UpdateMouse();
  return s_mouseY;
}

float lUIMouse::GetX()
{
  UpdateMouse();
  return s_fMouseX;
}

float lUIMouse::GetY()
{
  UpdateMouse();
  #ifndef L_PLATFORM_NDS
  return s_fMouseY*lSpriteManager::GetHackScaleNTSCValue();
  #else
  return s_fMouseY;
  #endif
}

int lUIMouse::GetIntX()
{
  UpdateMouse();
  return s_mouseX;
}

int lUIMouse::GetIntY()
{
  UpdateMouse();
  return s_mouseY;
}
