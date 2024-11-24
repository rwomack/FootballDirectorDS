//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdkeyboardwidget.h
// Description : A widget that encapsulates an onscreen keyboard.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDKEYBOARDWIDGET_H_INCLUDED
#define FDKEYBOARDWIDGET_H_INCLUDED

#include <lemon/lapi.h>
#include <ui/luiwidget.h>
#include <ui/luibuttonwidget.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

class lUIControlWidget;
class FDButtonWidget;

class FDKeyboardWidget : public lUIWidget, public lHasSlots<>
{
	LUI_RTTI( FDKeyboardWidget, lUIWidget );
public:
	static void ClassInitialiser();

	FDKeyboardWidget();
	FDKeyboardWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
					const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
					const lUIWidgetPosition pos = LUI_POS_DEFAULT,
					const lUIWidgetSize size = LUI_SIZE_DEFAULT );

	virtual ~FDKeyboardWidget();

	// lUIWidget interface

	virtual void     Render();
	virtual lUIPoint GetIdealSize() const;

	void  EnableCapsLock( bool value );

	lSignal1<char> SignalKeyPressed;
private:
	void Init();

	void OnKeyboard( lUIControlWidget * );

	struct Key
	{
		FDButtonWidget * widget;
		char             letter;
	};

	FDButtonWidget * m_capsLock;
  bool  m_bCaps;

	lArray<Key> m_Keys;
	lUIPoint m_DesiredSize;
};

END_L_NAMESPACE

#endif // #ifndef FDKEYBOARDWIDGET_H_INCLUDED
