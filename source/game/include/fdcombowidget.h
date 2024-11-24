//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdcombowidget.h
// Description : FDComboWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDCOMBOWIDGETUI_H_INCLUDED
#define FDCOMBOWIDGETUI_H_INCLUDED

#include <ui/luiwidget.h>
#include <framework/lsigslot.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDComboWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDComboWidget : public lUIWidget
{
  LUI_RTTI( FDComboWidget, lUIWidget );

public:
  static void ClassInitialiser();

  FDComboWidget();
  FDComboWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                 const lUIText & label,
                 const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                 const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                 const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDComboWidget();

  // lUIWidget interface

  virtual void     Render();

  virtual lUIPoint GetIdealSize() const;

  virtual void     OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags );
  virtual void     OnMouseOver();
  virtual void     OnMouseLeave();
  virtual bool     OnMouseDown( const lUIPoint &, luint32_t );
  virtual bool     OnMouseUp( const lUIPoint &, luint32_t );

  // FDComboWidget interface

  void            ClearAll();
  void            AddItem( const lUIText &, bool bClip = false );
  int             GetNumItems();
  const lUIText & GetItem( int index );
  int             GetSelectedItem();
  void            SetSelectedItem( int, bool bDontSignal = false );

  void            SetAllToUpper( bool bValue );

  lSignal2<FDComboWidget *, int>      SignalSelection;

private:

  enum
  {
    COMBO_HOT			= 0x01,
    COMBO_HOT_LEFT		= 0x02,
    COMBO_HOT_RIGHT		= 0x04,
    COMBO_DOWN			= 0x08,
	COMBO_DOWN_LEFT		= 0x10,
	COMBO_DOWN_RIGHT	= 0x20,
  };

  lUIText               m_label;
  luint32_t             m_state;
  lSharedPtr<lSprite>   m_left;
  lSharedPtr<lSprite>   m_right;
  lSharedPtr<lSprite>   m_leftOn;
  lSharedPtr<lSprite>   m_rightOn;
  lSharedPtr<lSprite>   m_middle;
  lArray< lUIText >     m_itemList;
  luint32_t             m_index;
  bool                    m_bUpper;
};

END_L_NAMESPACE
#endif

