//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fddragwidget.h
// Description : Helper widget that can be hooked up to tables to provide rendering
//				 of drag-and-drop items.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDDRAGWIDGET_H_INCLUDED
#define FDDRAGWIDGET_H_INCLUDED

#include <ui/luiwidget.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

class FDTableWidget;

class FDDragWidget : public lUIWidget, public lHasSlots<>
{
	LUI_RTTI( FDDragWidget, lUIWidget );
public:
	static void ClassInitialiser();

	FDDragWidget();
	FDDragWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id);

	virtual ~FDDragWidget();

	virtual lUIPoint GetIdealSize() const;

	virtual void Render();

	void ConnectToTable(FDTableWidget *pTable);
	void DisconnectFromTable(FDTableWidget *pTable);

	lUIPoint GetCurrentPoint() { return lUIPoint(m_lastPoint.x/* - m_grabPoint.x*/, m_lastPoint.y /*- m_grabPoint.y*/); };
private:
	void Init();

	void OnTableDrag(FDTableWidget *pTable, int iRow, int iColumn, lUIPoint cPos);
	void OnTableDrop(FDTableWidget *pTable, int iRow, int iColumn, lUIPoint cPos);

	bool m_bRenderString;
	lString m_string;
	lUIPoint m_lastPoint;
	lUIPoint m_grabPoint;
};

END_L_NAMESPACE

#endif // #ifndef FDDRAGWIDGET_H_INCLUDED
