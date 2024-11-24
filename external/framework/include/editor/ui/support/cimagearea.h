//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : cimagearea.h
// Description : Image area control
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef EDITOR_UI_SUPPORT_CIMAGEAREA_H_INCLUDED
#define EDITOR_UI_SUPPORT_CIMAGEAREA_H_INCLUDED

#include <lemon/lapi.h>
#include <lemon/toolhelp/wtl/latl.h>

// Predeclared classes

BEGIN_L_NAMESPACE
class lImage;
END_L_NAMESPACE

namespace WTL {

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// CImageArea
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class CImageArea : public CWindowImpl<CImageArea>
{
public:
  DECLARE_WND_CLASS_EX(NULL, 0, -1)

  CImageArea();

  void SetImage( const Lemon::lImage * img );
  void SetImage( const char * fileName );

  BEGIN_MSG_MAP(CImageArea)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
  END_MSG_MAP()


  LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
  Lemon::lImage * m_img;
  void DoPaint(CDCHandle dc);
};


}

#endif
