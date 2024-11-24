//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : wxlcamerawindow.h
// Description : wxlCameraWindow header
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef WXLCAMERAWINDOW_H_INCLUDED
#define WXLCAMERAWINDOW_H_INCLUDED

// TODO:
// Might consider making DrawGridLines and DrawSelectionBox part of lrDrawHelp
// and pass an eye ray or camera in.
//
// Would also like to isloate this interface more with less include dependancies

#include "./ltoolhelpapi.h"
#include "./wxlrenderwindow.h"

#include "../render/lrenderhelp.h" // for lrCameraOrbit (can be fixed)
#include "../math/lmray.h"         // lmRay3
#include "../math/lmaabox.h"       // lmAABox3
#include "../math/lmplane.h"       // lmPlane3
#include "../math/lmmatrix.h"      // lmVector3

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// wxlCameraWindow
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// wxWidgets render window with a user controllable camera.
// Description:
//   Inherit from wxlCameraWindow and override RenderSurface to render your
//   surface. The default implementation clears the screen and renders an XZ
//   grid using the DrawGridLines method. There is no requirement to call it
//   in the derived method.
//
//   Don't call buffer swap, that will be done automatically when the
//   window needs to be repainted.
//
class LTOOLHELP_API wxlCameraWindow : public wxlRenderWindow
{
  DECLARE_CLASS(wxlCameraWindow);
  DECLARE_EVENT_TABLE()

public:

  wxlCameraWindow( wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize, long style = 0,
                  const wxString& name = wxPanelNameStr);

  ~wxlCameraWindow();

  // wxlCameraWindow interface

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

  // Events

  void OnMouseWheel(wxMouseEvent &evt);
  void OnKeyDown(wxKeyEvent &evt);
  void OnMouseMove(wxMouseEvent &evt);
  void OnMouseButtonDown(wxMouseEvent &evt);
  void OnMouseButtonUp(wxMouseEvent &evt);

private:

  bool          m_bMouseCapture;
  int           m_xMouse, m_xMouseMove;
  int           m_yMouse, m_yMouseMove;
  bool          m_bAllowCameraMovement;
  bool          m_bAllowKeyboardCameraMovement;

  bool          m_bDefaultCameraButtons;
  bool          m_bRotate;
  bool          m_bMove;
  bool          m_bZoom;

  lmRay3        m_curRay;

  lrCameraOrbit m_cam;
  float         m_CameraFOV;
  float         m_CameraNear;
  float         m_CameraFar;
  float         m_CameraWidth;
  float         m_CameraHeight;
};


END_L_NAMESPACE
#endif

