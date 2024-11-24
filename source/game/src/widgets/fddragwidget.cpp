//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fddragwidget.h
// Description : Helper widget that can be hooked up to tables to provide rendering
//				 of drag-and-drop items.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fddragwidget.h>
#include <fdtablewidget.h>
#include <fdtext.h>
#include <framework/lspritemanager.h>
#include <fdsound.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI(FDDragWidget, lGUID(0, 0, 0, 0), 0);

void FDDragWidget::ClassInitialiser()
{
}

FDDragWidget::FDDragWidget()
{
	Init();
}

FDDragWidget::FDDragWidget(lUIWidget * parent, luint32_t widgetFlags, const char * id) : lUIWidget(parent, widgetFlags | LUI_WIDGET_MOUSETRANSPARENT, id)
{
	Init();
}

FDDragWidget::~FDDragWidget()
{
}

void FDDragWidget::Init()
{
	m_bRenderString = false;
	m_lastPoint = lUIPoint(0.0f, 0.0f);
}

lUIPoint FDDragWidget::GetIdealSize() const
{
	return lUIPoint(lSpriteManager::GetDisplayWidth(), lSpriteManager::GetDisplayHeight());
}

void FDDragWidget::Render()
{
	if (m_bRenderString)
	{
		lSharedPtr<lFont> font = GetFont();
		font->SetColour(0xFFFFFFFF);
		FDTextPrint(font.Get(), m_lastPoint.x - m_grabPoint.x, m_lastPoint.y - m_grabPoint.y, m_string.CStr());
	}
}

void FDDragWidget::ConnectToTable(FDTableWidget *pTable)
{
	pTable->SignalCellDragging.Connect(this, &FDDragWidget::OnTableDrag);
	pTable->SignalCellDropped.Connect(this, &FDDragWidget::OnTableDrop);
}

void FDDragWidget::DisconnectFromTable(FDTableWidget *pTable)
{
	pTable->SignalCellDragging.Disconnect(this, &FDDragWidget::OnTableDrag);
	pTable->SignalCellDropped.Disconnect(this, &FDDragWidget::OnTableDrop);
}

void FDDragWidget::OnTableDrag(FDTableWidget *pTable, int iRow, int iColumn, lUIPoint cPos)
{
	if ((pTable->IsFirstRowFixed() && (iRow == 0)) || (iColumn > 1) )
		return;

	if (m_bRenderString == false)
	{
		m_bRenderString = true;

		lUIRect cCellRect;
		pTable->GetCellRect(cCellRect, iRow, iColumn);
		m_grabPoint = cPos;
		m_grabPoint.x -= cCellRect.x;
		m_grabPoint.y -= cCellRect.y;
		m_string = pTable->GetText(iRow, iColumn).str;
    FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );

	}

	lUIPoint cScreenPos = pTable->WidgetClientToScreen(cPos);

	m_lastPoint = cScreenPos;
}

void FDDragWidget::OnTableDrop(FDTableWidget *pTable, int iRow, int iColumn, lUIPoint cPos)
{
  FDSound::PlaySoundEffect( FD_SFX_UI_CLICK );

	if (m_bRenderString == true)
	{
		m_bRenderString = false;
		m_lastPoint.x = 0;
		m_lastPoint.y = 0;
	}
}
