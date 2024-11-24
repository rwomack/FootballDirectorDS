//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : luiwidget.h
// Description : lUIWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LUIWIDGET_H_INCLUDED
#define LUIWIDGET_H_INCLUDED

#include <lemon/lapi.h>
#include <framework/lproperty.h>
#include <game/lgameleaf.h>
#include <lemon/containers/ltreenode.h>
#include <lemon/containers/lstring.h>
#include "./luirtti.h"

#include <lemon/object/lsharedptr.h>
#include <framework/lfont.h>

// TEMP
#include <framework/ltext.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lUIWidget;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIText
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// TEMP!!

class lUIText
{
public:
  lUIText()
  {
  }

  lUIText( int scriptId, const char * postfix = NULL )
  {
    str = lText::Text(scriptId);
    if( postfix ) str += postfix;
  }

  lUIText( const char * text, const char * postfix = NULL )
  {
    str = text;
    if( postfix ) str += postfix;
  }

  void ToUpper()
  {
    str.ToUpper();
  }

  lString str;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIRect
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct lUIRect
{
  float x,y,w,h;

  lUIRect()
  {
  }

  lUIRect( float x_, float y_, float w_, float h_ )
    : x(x_),
      y(y_),
      w(w_),
      h(h_)
  {
  }

  void Shrink( float sw, float sh )
  {
    x+=sw;
    y+=sh;
    w-=sw*2;
    h-=sh*2;
  }

  void Expand( float sw, float sh )
  {
    x-=sw;
    y-=sh;
    w+=sw*2;
    h+=sh*2;
  }

  bool InRect( const lUIRect &rect ) const
  {
    if( x >= rect.x && (x+w) <= (rect.x+rect.w) &&
        y >= rect.y && (y+h) <= (rect.y+rect.h) )
    {
      return true;
    }

    return false;
  }

  bool OverlapsRect( const lUIRect &rect ) const
  {
    if( ( (x >= rect.x && x <= (rect.x+rect.w)) || ((x+w) >= rect.x && (x+w) <= (rect.x+rect.w)) ) &&
        ( (y >= rect.y && y <= (rect.y+rect.h)) || ((y+h) >= rect.y && (y+h) <= (rect.y+rect.h)) ) )
    {
      return true;
    }

    return false;
  }

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIPoint
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct lUIPoint
{
  float x,y;

  lUIPoint()
    : x(0),
      y(0)
  {
  }

  lUIPoint( float x_, float y_ )
    : x(x_),
      y(y_)
  {
  }

  bool InRect( const lUIRect &rect ) const
  {
    if( x >= rect.x && x <= (rect.x+rect.w) &&
        y >= rect.y && y <= (rect.y+rect.h) )
    {
      return true;
    }

    return false;
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIPointPercent
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct lUIPointPercent
{
  float x,y;

  lUIPointPercent()
    : x(0),
      y(0)
  {
  }

  lUIPointPercent( float x_, float y_ )
    : x(x_),
      y(y_)
  {
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUICoord
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct lUICoord
{
  float coord;

  lUICoord()
    : coord(0)
  {
  }

  lUICoord( float coord_ )
    : coord(coord_)
  {
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUICoordPercent
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct lUICoordPercent
{
  float coord;

  lUICoordPercent()
    : coord(0)
  {
  }

  lUICoordPercent( float coord_ )
    : coord(coord_)
  {
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyUIRect
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lPropertyUIRect : public lValuePropertyRTTI< lPropertyUIRect, lUIRect >
{
public:
  virtual void XMLSerialize( lObject *o, lPropertyObjectMap &mapper, lXMLNode &ar );
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyUIPoint
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lPropertyUIPoint : public lValuePropertyRTTI< lPropertyUIPoint, lUIPoint >
{
public:
  virtual void XMLSerialize( lObject *o, lPropertyObjectMap &mapper, lXMLNode &ar );
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIFontType
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum lUIFontType
{
  LUI_FONT_DEFAULT = 0
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIFont
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lUIFont
{
public:

  lUIFont();

  lUIFont( lUIFontType );
  lUIFont( lSharedPtr<lFont> );

private:

  friend class lUIWidget;

  luint32_t         m_flags;
  lSharedPtr<lFont> m_font;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIAlignAxis
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum lUIAlignAxis
{
  LUI_LEFT   = 0x0,
  LUI_RIGHT  = 0x1,
  LUI_CENTER = 0x2,
  LUI_TOP    = LUI_LEFT,
  LUI_BOTTOM = LUI_RIGHT
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIAlign
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum lUIAlign
{
  LUI_LEFT_TOP        = ( LUI_LEFT << 4 ) + LUI_TOP,
  LUI_LEFT_BOTTOM     = ( LUI_LEFT << 4 ) + LUI_BOTTOM,
  LUI_LEFT_CENTER     = ( LUI_LEFT << 4 ) + LUI_CENTER,

  LUI_RIGHT_TOP       = ( LUI_RIGHT << 4 ) + LUI_TOP,
  LUI_RIGHT_BOTTOM    = ( LUI_RIGHT << 4 ) + LUI_BOTTOM,
  LUI_RIGHT_CENTER    = ( LUI_RIGHT << 4 ) + LUI_CENTER,

  LUI_CENTER_TOP      = ( LUI_CENTER << 4 ) + LUI_TOP,
  LUI_CENTER_BOTTOM   = ( LUI_CENTER << 4 ) + LUI_BOTTOM,
  LUI_CENTER_CENTER   = ( LUI_CENTER << 4 ) + LUI_CENTER,

  LUI_TOP_LEFT        = LUI_LEFT_TOP,
  LUI_TOP_RIGHT       = LUI_RIGHT_TOP,
  LUI_TOP_CENTER      = LUI_CENTER_TOP,

  LUI_BOTTOM_LEFT     = LUI_LEFT_BOTTOM,
  LUI_BOTTOM_RIGHT    = LUI_RIGHT_BOTTOM,
  LUI_BOTTOM_CENTER   = LUI_CENTER_BOTTOM,

  LUI_CENTER_LEFT     = LUI_LEFT_CENTER,
  LUI_CENTER_RIGHT    = LUI_RIGHT_CENTER,
};

lUIPoint lUICalcAlign( lUIAlign align, const lUIPoint & parentSize, const lUIPoint & itemSize );
float    lUICalcAlignAxis( lUIAlignAxis align, float parentSize, float itemSize );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Defaults
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define LUI_ANCHOR_DEFAULT LUI_TOP_LEFT
#define LUI_POS_DEFAULT    lUIPoint(0,0)
#define LUI_SIZE_DEFAULT   lUIPoint(-1,-1)

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIWidgetPosition
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lUIWidgetPosition
{
public:

  lUIWidgetPosition();

  lUIWidgetPosition( lUIAlign );
  lUIWidgetPosition( const lUIPoint & );
  lUIWidgetPosition( const lUIPointPercent & );

private:

  friend class lUIWidget;

  enum
  {
    PIXEL   = 0,
    PERCENT = 1
  };

  luint32_t m_positionFlags;
  float     m_x;
  float     m_y;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIWidgetSize
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lUIWidgetSize
{
public:
  lUIWidgetSize();

  lUIWidgetSize( const lUIPoint & );
  lUIWidgetSize( const lUIPointPercent & );

private:

  friend class lUIWidget;

  enum
  {
    PIXEL   = 0,
    PERCENT = 1
  };

  luint32_t m_positionFlags;
  float     m_x;
  float     m_y;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIWidgetPositionCoord
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lUIWidgetPositionCoord
{
public:

  lUIWidgetPositionCoord();

  lUIWidgetPositionCoord( lUIAlignAxis );
  lUIWidgetPositionCoord( const lUICoord & );
  lUIWidgetPositionCoord( const lUICoordPercent & );

private:

  friend class lUIWidget;

  enum
  {
    PIXEL   = 0,
    PERCENT = 1
  };

  luint32_t m_positionFlags;
  float     m_coord;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIWidgetSize
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lUIWidgetSizeCoord
{
public:
  lUIWidgetSizeCoord();

  lUIWidgetSizeCoord( const lUICoord & );
  lUIWidgetSizeCoord( const lUICoordPercent & );

private:

  friend class lUIWidget;

  enum
  {
    PIXEL   = 0,
    PERCENT = 1
  };

  luint32_t m_positionFlags;
  float     m_coord;
};




//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lUIWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum
{
  LUI_MOUSE_LEFT  = 0x1,
  LUI_MOUSE_RIGHT = 0x2,
};

enum
{
  LUI_WIDGET_MOUSEHOTFOCUS    = 0x1, // widget will gain focus when mouse over
  LUI_WIDGET_MOUSEDOWNFOCUS   = 0x2, // widget will gain focus on mouse click
  LUI_WIDGET_MOUSETRANSPARENT = 0x3, // widget mouse hits fall through this widget

  LUI_WIDGET_HIDDEN          = 0x100,
  LUI_WIDGET_DISABLED        = 0x200,
};

class lUIWidget : public lTreeNode<lUIWidget>, public lGameLeaf
{
  LUI_RTTI( lUIWidget, lGameLeaf );

public:
  static void ClassInitialiser();

  static bool ms_bLastMouseClickHandled;

  lUIWidget();
  virtual ~lUIWidget();

  lUIWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
             const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
             const lUIWidgetPosition pos = LUI_POS_DEFAULT,
             const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  // lUIWidget interface

  virtual void     Tick( float );
  virtual void     Render();

  // Layout

  virtual lUIPoint GetIdealSize() const;

  // Mouse mask

  virtual bool     HitMask( const lUIPoint & point );


  // Mouse events

  virtual void        OnMouseLeave();
  virtual void        OnMouseLeaveHot();

  virtual void        OnMouseOver();
  virtual void        OnMouseOverHot();
  virtual void        OnMouseHot( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags );

  virtual bool        OnMouseDown( const lUIPoint & p, luint32_t mouseDownFlags );
  virtual bool        OnMouseUp( const lUIPoint & p, luint32_t mouseUpFlags );
  virtual void        OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags );

  virtual void        OnGainFocus();
  virtual void        OnLoseFocus();

  // Key events

  virtual void        OnKey( luint8_t c, luint8_t ext );

  // lUIWidget implementation

  const char        * GetId() const;

  const lUIRect     & GetRect() const;

  lSharedPtr<lFont>   GetFont() const;
  void                SetFont( const lUIFont & );

  void                Move(const lUIPoint &);
  void                Move(const lUIRect &);

  void                Move( const lUIWidgetPosition & );
  void                Move( const lUIWidgetPosition &, const lUIWidgetSize & );

  void                Show();
  void                Hide();
  bool                IsVisible() const;

  void                Enable();
  void                Disable();
  bool                IsEnabled() const;

  void                BringToFront();
  void                PushToBack();

  lUIPoint            ScreenToWidget( const lUIPoint & );
  lUIPoint            ScreenToWidgetClient( const lUIPoint & );
  lUIRect             ScreenToWidget( const lUIRect & );
  lUIPoint            WidgetToScreen( const lUIPoint & );
  lUIPoint            WidgetClientToScreen( const lUIPoint & );
  lUIRect             WidgetToScreen( const lUIRect & );

  lUIPoint            PixelSize( const lUIWidgetSize & ) const;
  float               PixelSizeX( const lUIWidgetSizeCoord & ) const;
  float               PixelSizeY( const lUIWidgetSizeCoord & ) const;

  lUIPoint            PixelPos( const lUIWidgetPosition & ) const;
  float               PixelPosX( const lUIWidgetPositionCoord & ) const;
  float               PixelPosY( const lUIWidgetPositionCoord & ) const;


  lUIPoint            CalcAlign( lUIAlign align, const lUIPoint & itemSize ) const;
  float               CalcAlignX( lUIAlignAxis align, float itemSizeX ) const;
  float               CalcAlignY( lUIAlignAxis align, float itemSizeY ) const;

  void                SetFocus();
  bool                HasFocus();
  bool                IsHot();

  lUIWidget         * CaptureMouse();
  void                ReleaseMouse();
  
  luint32_t           GetWidgetFlags();
  void                SetWidgetFlags(luint32_t);

  // Config

  static void               SetStylusInput( bool );
  static bool               GetStylusInput();

  static void               SetDefaultFont(lSharedPtr<lFont> font);
  static lSharedPtr<lFont>  GetDefaultFont();


  // Private

  bool                ProcessInput();

private:
  luint32_t         m_widgetFlags;
  lString           m_id;
  lUIWidgetPosition m_anchor;
  lUIWidgetPosition m_pos;
  lUIWidgetSize     m_size;
  lUIFont           m_font;

  mutable lUIRect   m_rect;

  enum
  {
    MOUSE_STATE_HOT   = 0x01,
    MOUSE_STATE_OVER  = 0x02,
  };

  luint32_t         m_mouseState;
  luint32_t         m_prevMouseState;

  void UpdateSize() const;
  void UpdatePos() const;
  void CalcPos( lUIPoint &, const lUIWidgetPosition &, const lUIWidget * ) const;

  lUIWidget *             m_focusChild;
  lUIPoint                m_mouseOverPos;
  lUIPoint                m_mouseHotPos;

  bool ProcessMouseEvent( const lUIPoint &pos, unsigned int mouseDownFlags, unsigned int mouseUpFlags );
  bool ProcessKeyEvent( luint8_t c, luint8_t ext );
  void InternalSetParentFocus();
};


END_L_NAMESPACE
#endif
