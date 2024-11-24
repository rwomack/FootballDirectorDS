//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdteleprinterwidget.h
// Description : FDPrinterWidget interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDTELEPRINTERWIDGETUI_H_INCLUDED
#define FDTELEPRINTERWIDGETUI_H_INCLUDED

#include <ui/luiwidget.h>
#include <framework/lsigslot.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

class lSprite;
class FDScrollBarWidget;
class FDTableWidget;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// FDTelePrinterWidget
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class FDTeleprinterWidget : public lUIWidget, public lHasSlots<>
{
  LUI_RTTI( FDTeleprinterWidget, lUIWidget );

public:
  static void ClassInitialiser();

  FDTeleprinterWidget();
  FDTeleprinterWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
                  const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
                  const lUIWidgetPosition pos = LUI_POS_DEFAULT,
                  const lUIWidgetSize size = LUI_SIZE_DEFAULT );

  virtual ~FDTeleprinterWidget();

  // lUIWidget interface

  virtual void     Tick( float delta );

  // FDTeleprinterWidget interface
  void            Clear();
  int            AddItem(const lUIText &text, const luint32_t uiBackgroundColour );
  int            AddItem(const lUIText &date, const lUIText &text, const luint32_t uiBackgroundColour );

  void SkipToPosition(const int iPosition);
  void SkipToEnd();

  void SetPrintImmediately();
private:
  void Init();
  const char *GetNextTeleprinterChar();

  FDTableWidget *m_table;
  FDScrollBarWidget * m_scroll;

  float			m_timer;
  lArray<lUIText> m_queuedMessages;
  lArray<luint32_t> m_queuedBackgroundColours;
  lUIText		m_sCurrentMessage;
  luint32_t		m_uiCurrentMessageCharacter;

  int m_iTeleprinterCharType;
  bool m_bPrintImmediately : 1;
};


END_L_NAMESPACE
#endif
