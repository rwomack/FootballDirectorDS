//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdscrollbarwidget.h
// Description : Helper widget that can be hooked up to tables to provide
//				 a scroll bar.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDSCROLLBARWIDGET_H_INCLUDED
#define FDSCROLLBARWIDGET_H_INCLUDED

#include <ui/luiwidget.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

class FDScrollBarWidget : public lUIWidget, public lHasSlots<>
{
	LUI_RTTI( FDScrollBarWidget, lUIWidget );
public:
	static void ClassInitialiser();

	FDScrollBarWidget();
	FDScrollBarWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
             const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
             const lUIWidgetPosition pos = LUI_POS_DEFAULT,
             const lUIWidgetSize size = LUI_SIZE_DEFAULT);

	virtual ~FDScrollBarWidget();

	virtual void Render();

	virtual void OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags );
	virtual bool OnMouseDown( const lUIPoint &, luint32_t );
	virtual bool OnMouseUp( const lUIPoint &, luint32_t );

  void AlignTo( lUIWidget *widget );

	void SetNumberOfItems(const int iItems, const bool bFireEvents);
	void SetItemsViewedAtOnce(const int iItems, const bool bFireEvents);
	void SetCurrentPosition(const int iPosition, const bool bFireEvents);

	int GetNumberOfItems() const;
	int GetItemsViewedAtOnce() const;
	int GetCurrentPosition() const;

	lSignal2<FDScrollBarWidget *, int> SignalPositionChanged;

	void SetNewsScrolling( bool value );

private:
	void Init();

	lUIRect GetThumbVisibleRect() const;
	lUIRect GetThumbRect() const;
	lUIRect GetScrollAreaVisibleRect() const;
	lUIRect GetScrollAreaRect() const;

	int m_iNumberOfItems;			// total items tracked by the scrollbar
	int m_iItemsViewedAtOnce;		// number of items visible at once
	int m_iCurrentPosition;			// current position in the table/view
	lUIPoint m_cDragStartPoint;

	bool m_bDragging		: 1;
	bool m_bDragStartedMove : 1;
	int m_iDragStartItem;
	int m_bNewsScroll;
};

END_L_NAMESPACE

#endif // #ifndef FDSCROLLBARWIDGET_H_INCLUDED
