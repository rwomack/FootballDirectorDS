//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdplayerblobwidget.h
// Description : FDPlayerBlobWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDPLAYERBLOBWIDGETUI_H_INCLUDED
#define FDPLAYERBLOBWIDGETUI_H_INCLUDED

#include <ui/luibuttonwidget.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDPlayerBlobWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDPlayerBlobWidget : public lUIButtonWidget
{
  LUI_RTTI( FDPlayerBlobWidget, lUIButtonWidget );

public:
  static void ClassInitialiser();

  FDPlayerBlobWidget();
  FDPlayerBlobWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                      lSharedPtr<lSprite> sprite, int squadNumber, luint32_t playerblobFlags = 0,
                      const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                      const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                      const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDPlayerBlobWidget();

  // lUIWidget interface

  virtual void     Render();
  virtual lUIPoint GetIdealSize() const;

  virtual void OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags );
  virtual bool OnMouseDown( const lUIPoint & p, luint32_t mouseDownFlags );
  virtual bool OnMouseUp( const lUIPoint & p, luint32_t mouseUpFlags );

  // lUIPlayerBlobWidget interface

  // FDPlayerBlobWidget interface

  void                SetSprite( lSharedPtr<lSprite> sprite );
  lSharedPtr<lSprite> GetSprite();

  void SetColour(const luint32_t uiColour);

  lSignal2<FDPlayerBlobWidget *, lUIPoint> SignalDropped;
private:
  lSharedPtr<lSprite> m_sprite;
  int                 m_squadNumber;
  bool				  m_bDragging;
  bool				  m_bRenderAtDragPoint;
  lUIPoint			  m_cDragStartPoint;
  luint32_t			  m_colour;
};


END_L_NAMESPACE
#endif

