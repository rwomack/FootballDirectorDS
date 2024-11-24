//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// File        : fdsliderwidget.h
// Description : FDSliderWidget interface
// Notes       :
//
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDSLIDERWIDGETUI_H_INCLUDED
#define FDSLIDERWIDGETUI_H_INCLUDED

#include <ui/luiwidget.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
// FDSliderWidget
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

class FDSliderWidget : public lUIWidget
{
  LUI_RTTI( FDSliderWidget, lUIWidget );

public:
  static void ClassInitialiser();

  FDSliderWidget();
  FDSliderWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                  luint32_t sliderFlags, const lUIText & label,
                  const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                  const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                  const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDSliderWidget();

  // lUIWidget interface

  virtual void     Render();

  virtual lUIPoint GetIdealSize() const;

  virtual void     OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags );
  virtual void     OnMouseOver();
  virtual void     OnMouseLeave();
  virtual bool     OnMouseDown( const lUIPoint &, luint32_t );
  virtual bool     OnMouseUp( const lUIPoint &, luint32_t );

  // FDSliderWidget interface
  void             SetLabel( lUIText text );
  lUIText          GetLabel();

  void             SetTextToUpper();
  void              SetSliderOffset( int value );
  void             SetValueRange( float min, float max );

  void             SetValueScale( float scale );
  float            GetValueScale();

  void             SetValueOffset( float offset );
  float            GetValueOffset();

  void             SetValuePct( float val );
  float            GetValuePct();

  void             SetValueInc( float val );
  float            GetValueInc();

  void             SetValue( float val, bool bDontSignal=false);
  float            GetValue();

  void              SetSliderSprite(lSharedPtr<lSprite>   slider);

  lSignal2<lUIWidget *, float> SignalSliderChange;

  enum
  {
    SLIDER_FLAG_PERCENTAGE     = 0x001,  // show value as percentage
    SLIDER_FLAG_POUND     = 0x002,  // show value as £
    SLIDER_FLAG_X     = 0x004,  // show value as x1, x2 etc
  };

  luint32_t GetSliderFlags();
  void SetSliderFlags( luint32_t flag );

private:

  enum
  {
    SLIDER_HOT       = 0x01,
    SLIDER_DOWN      = 0x02
  };

  lUIText               m_label;
  luint32_t             m_state;
  lSharedPtr<lSprite>   m_slider;
  lSharedPtr<lSprite>   m_button;
  float                 m_value;
  float                 m_valuescale;
  float                 m_valueoffset;
  float                 m_increment;

  luint32_t             m_sliderflags;
  int                     m_sliderOffset;
};




END_L_NAMESPACE
#endif

