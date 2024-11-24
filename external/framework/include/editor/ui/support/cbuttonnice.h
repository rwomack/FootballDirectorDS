//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : cbuttonnice.h
// Description : Nice button
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef EDITOR_UI_SUPPORT_CBUTTONNICE_H_INCLUDED
#define EDITOR_UI_SUPPORT_CBUTTONNICE_H_INCLUDED

#include <lemon/toolhelp/wtl/latl.h>

namespace WTL {

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// CButtonNice
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class CButtonNice : public CWindowImpl<CButtonNice,CButton,CControlWinTraits>
{
public:
  DECLARE_WND_SUPERCLASS(NULL, CButton::GetWndClassName())

  CButtonNice();

  void SetToolTip( const char * str );
  void SetChecked(bool b);
  bool GetChecked();
  bool IsHoverMode();
  void SetSmall( bool state );
  void SetImage( int width, int height, void * data );
  void UpdateSize();
  void GetSize( CRect &rc );

  BEGIN_MSG_MAP(CButtonNice)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
    MESSAGE_HANDLER(WM_SETFOCUS, OnFocus)
    MESSAGE_HANDLER(WM_KILLFOCUS, OnFocus)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
    MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
    MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
    MESSAGE_HANDLER(WM_ENABLE, OnEnable)
    MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
    MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
    MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
    MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
  END_MSG_MAP()

  LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& );
  LRESULT OnMouseMessage(UINT, WPARAM, LPARAM, BOOL& );
  LRESULT OnEraseBackground(UINT, WPARAM, LPARAM , BOOL& );
  LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL& );
  LRESULT OnFocus(UINT, WPARAM, LPARAM, BOOL& );
  LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL& );
  LRESULT OnLButtonDblClk(UINT, WPARAM, LPARAM, BOOL& );
  LRESULT OnLButtonUp(UINT, WPARAM, LPARAM, BOOL& );
  LRESULT OnCaptureChanged(UINT, WPARAM, LPARAM, BOOL& );
  LRESULT OnEnable(UINT, WPARAM, LPARAM, BOOL& );
  LRESULT OnMouseMove(UINT, WPARAM, LPARAM, BOOL& );
  LRESULT OnMouseLeave(UINT, WPARAM, LPARAM, BOOL& );
  LRESULT OnKeyDown(UINT, WPARAM, LPARAM, BOOL& );
  LRESULT OnKeyUp(UINT, WPARAM , LPARAM, BOOL& );

private:

  unsigned m_fMouseOver:1;
	unsigned m_fFocus:1;
  unsigned m_fPressed:1;
  unsigned m_fChecked:1;

  CToolTipCtrl m_tip;
  const char * m_lpstrToolTipText;

  enum
	{
		ID_TIMER_FIRST = 1000,
		ID_TIMER_REPEAT = 1001
  };

  void * m_bitmap;
  int    bitmapSizeX;
  int    bitmapSizeY;
  int    largeButtonClientSizeX;
  int    largeButtonClientSizeY;
  int    largeButtonWindowSizeX;
  int    largeButtonWindowSizeY;
  int    smallButtonClientSizeX;
  int    smallButtonClientSizeY;
  int    smallButtonWindowSizeX;
  int    smallButtonWindowSizeY;

  int    shadowOffsetX;
  int    shadowOffsetY;
  int    windowBorder;
  bool   bSmall;

  unsigned int activeBorderBGR;
  unsigned int activeBackgroundBGR;
  unsigned int checkedBorderBGR;
  unsigned int checkedBackgroundBGR;
  unsigned int textBorderBGR;
  unsigned int textShadowColour;
  unsigned int textColour;
  unsigned int shadowBGR;
  unsigned int shadowBlurRadius;

  void Init();
  void DoPaint(CDCHandle dc);
  unsigned int * RenderBitmap(const unsigned int * bitmapData, const unsigned int backgroundBGR, const bool shadow, bool bSmallIcons, CSize & size, const bool disabled);
  BOOL StartTrackMouseLeave();
};


}
#endif
