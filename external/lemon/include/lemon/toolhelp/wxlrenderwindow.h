//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : wxlrenderwindow.h
// Description : wxlRenderWindow header
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef WXLRENDERWINDOW_H_INCLUDED
#define WXLRENDERWINDOW_H_INCLUDED

#include "./ltoolhelpapi.h"

BEGIN_L_NAMESPACE

class lrSurface;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// wxlRenderWindow
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// wxWidgets window for an lRender surface
// Description:
//   Inherit from wxlRenderWindow and override RenderSurface to render your
//   surface. The default implementation simply clears the screen with
//   lRender::BufferClear so there is no requirement to call it.
//
//   Don't call buffer swap, that will be done automatically when the
//   window needs to be repainted.
//
class LTOOLHELP_API wxlRenderWindow : public wxWindow
{
  DECLARE_CLASS(wxlRenderWindow)
  DECLARE_EVENT_TABLE()

public:

  wxlRenderWindow( wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize, long style = 0,
                  const wxString& name = wxPanelNameStr);

  ~wxlRenderWindow();

  // wxlRenderWindow interface

  virtual void RenderSurface();

  bool Redraw();

  // Events

  void OnPaint(wxPaintEvent& evt);
  void OnEraseBackground(wxEraseEvent& evt);
  void OnSize(wxSizeEvent& evt);

  // MSW window proc

  #if defined L_PLATFORM_WINDOWS
  bool      Create( wxWindow *parent, wxWindowID id, const wxPoint& pos,
                    const wxSize& size, long style, const wxString& name );
  WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
  #endif


protected:

  lrSurface * m_pSurface;
};

END_L_NAMESPACE
#endif

