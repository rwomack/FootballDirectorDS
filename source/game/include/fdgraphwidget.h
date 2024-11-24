//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdgraphwidget.h
// Description : FDGraphWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDGRAPHWIDGETUI_H_INCLUDED
#define FDGRAPHWIDGETUI_H_INCLUDED

#include <ui/luiwidget.h>
#include <ui/luibuttonwidget.h>
#include <framework/lsigslot.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDButtonWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDGraphWidget : public lUIWidget, public lHasSlots<>
{
  LUI_RTTI( FDGraphWidget, lUIWidget );

public:
  static void ClassInitialiser();

  static const int MAX_DATA_SETS = 4;

  FDGraphWidget();
  FDGraphWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                  const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                  const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                  const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDGraphWidget();

  // lUIWidget interface

  virtual void     Render();
  virtual lUIPoint GetIdealSize() const;

  virtual void     OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags );
  virtual bool     OnMouseDown( const lUIPoint &, luint32_t );
  virtual bool     OnMouseUp( const lUIPoint &, luint32_t );

  static luint32_t GetColorForDataSet(const int iDataSet);

  // FDGraphWidget interface

  //void                SetBGSprite( lSharedPtr<lSprite> sprite );
  //lSharedPtr<lSprite> GetBGSprite();
  
  void              AddData(const int iDataSet, const int data);

  void				ClearDataSet(const int iDataSet);
  void              ClearAll();

  void              SetMaxValue( int max );
  void              SetMinValue( int min );
  
  void              SetIntervalGap( int gap );
  
  void              FlipYAxis(bool flip);
  
  void              SetTitle( lString text );
  void              SetXAxisTitle( lString text );
  void              SetYAxisTitle( lString text );

  void              SetScrollButtons( lUIButtonWidget * upButton, lUIButtonWidget * downButton );
  void              OnScrollLeftButton( lUIControlWidget * );
  void              OnScrollRightButton( lUIControlWidget * );

private:
  void RenderData(lUIRect &rect);

  //lSharedPtr<lSprite> m_spriteBg;
  
  lArray<int>      m_data[MAX_DATA_SETS];                  // Only storing integer values though

  bool               m_bScrollIntervals;   // Scroll per pixel or per data

  int                 m_visibleXTextInterval;
  int                 m_visibleYTextInterval;

  int                 m_intervalGap;        // Gap between data to display

  int                 m_maxValue;           // Maximum value on the Y axis
  int                 m_minValue;           //  Minimum value on the Y axis
  
  int                 m_maxData;            // Maximum amout of data on the X axis
  int                 m_minData;            // Maximum amout of data  on the X axis
  
  int                 m_offsetX;
  
  bool              m_flipYAxis;
  
  
  lString           m_title;
  lString           m_yAxisText;
  lString           m_xAxisText;

  lUIButtonWidget * m_scrollLeft;
  lUIButtonWidget * m_scrollRight;

};


END_L_NAMESPACE
#endif