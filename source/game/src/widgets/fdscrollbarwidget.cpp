//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdscrollbarwidget.h
// Description : Helper widget that can be hooked up to tables to provide
//				 a scroll bar.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#include <fdmemory.h>
#include <fdscrollbarwidget.h>
#include <framework/lspritemanager.h>
#include <fdsound.h>

using namespace L_NAMESPACE_NAME;

LUI_IMPLEMENT_RTTI(FDScrollBarWidget, lGUID(0, 0, 0, 0), 0);

//////////////////////////////////////////////////////////////////////////
// Static method required by the factory system.
//////////////////////////////////////////////////////////////////////////
void FDScrollBarWidget::ClassInitialiser()
{
}

//////////////////////////////////////////////////////////////////////////
// Constructor.
//////////////////////////////////////////////////////////////////////////
FDScrollBarWidget::FDScrollBarWidget()
{
	Init();
}

//////////////////////////////////////////////////////////////////////////
// Constructor.
//////////////////////////////////////////////////////////////////////////
FDScrollBarWidget::FDScrollBarWidget(lUIWidget * parent, luint32_t widgetFlags, const char * id,
									 const lUIWidgetPosition anchor, const lUIWidgetPosition pos,
									 const lUIWidgetSize size) : lUIWidget(parent, widgetFlags, id, anchor, pos, size)
{
	Init();
}

//////////////////////////////////////////////////////////////////////////
// Destructor.
//////////////////////////////////////////////////////////////////////////
FDScrollBarWidget::~FDScrollBarWidget()
{
}

//////////////////////////////////////////////////////////////////////////
// Initialization method. Called by all constructors.
//////////////////////////////////////////////////////////////////////////
void FDScrollBarWidget::Init()
{
	m_bDragging = false;
	m_bDragStartedMove = false;

	// Some sane default values.
	m_iNumberOfItems = 10;
	m_iItemsViewedAtOnce = 3;
	m_iCurrentPosition = 0;
}

//////////////////////////////////////////////////////////////////////////
// Renders the scroll bar.
//////////////////////////////////////////////////////////////////////////
void FDScrollBarWidget::Render()
{

	if (m_iItemsViewedAtOnce >= m_iNumberOfItems)
	{
    // The outside frame of the scroll bar.
    const lUIRect &cRect = WidgetToScreen(GetRect());
    lSpriteManager::DrawRect2( cRect.x, cRect.y, cRect.w, cRect.h, 0xff606060 );

    // The interior frame of the scroll bar.
    lUIRect cInteriorRect = WidgetToScreen(GetScrollAreaVisibleRect());
    cInteriorRect.x += GetRect().x;
    cInteriorRect.y += GetRect().y;
    lSpriteManager::DrawRect2( cInteriorRect.x, cInteriorRect.y, cInteriorRect.w, cInteriorRect.h, 0xff404040);

    // The thumbwheel component of the scroll bar, representing the actual viewed area.
    lUIRect cThumbRect = WidgetToScreen(GetThumbVisibleRect());
    cThumbRect.x += GetRect().x;
    cThumbRect.y += GetRect().y;
    lSpriteManager::DrawRect2(cThumbRect.x, cThumbRect.y, cThumbRect.w, cThumbRect.h, 0xffdddddd);
		return;
	}

	// The outside frame of the scroll bar.
	const lUIRect &cRect = WidgetToScreen(GetRect());
    lSpriteManager::DrawRect2( cRect.x, cRect.y, cRect.w, cRect.h, 0xff606060 );

	// The interior frame of the scroll bar.
	lUIRect cInteriorRect = WidgetToScreen(GetScrollAreaVisibleRect());
	cInteriorRect.x += GetRect().x;
	cInteriorRect.y += GetRect().y;
	lSpriteManager::DrawRect2( cInteriorRect.x, cInteriorRect.y, cInteriorRect.w, cInteriorRect.h, 0xff404040);

	// The thumbwheel component of the scroll bar, representing the actual viewed area.
	lUIRect cThumbRect = WidgetToScreen(GetThumbVisibleRect());
	cThumbRect.x += GetRect().x;
	cThumbRect.y += GetRect().y;
	lSpriteManager::DrawRect2(cThumbRect.x, cThumbRect.y, cThumbRect.w, cThumbRect.h, 0xffffffff);
}

//////////////////////////////////////////////////////////////////////////
// Set function for the number of items represented by the scroll bar.
//////////////////////////////////////////////////////////////////////////
void FDScrollBarWidget::SetNumberOfItems(const int iItems, const bool bFireEvents)
{
	const int iDelta = iItems - m_iNumberOfItems;
	m_iNumberOfItems = iItems;

	if (iDelta < 0)
	{
		// Ensure that we aren't looking off the edge.
		if ((m_iCurrentPosition + m_iItemsViewedAtOnce) > m_iNumberOfItems)
		{
			m_iCurrentPosition = lmMax(0, m_iCurrentPosition - ((m_iCurrentPosition + m_iItemsViewedAtOnce) - m_iNumberOfItems));
			if (bFireEvents)
			{
				SignalPositionChanged.Emit(this, m_iCurrentPosition);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Set function for the number of items viewed at once.
//////////////////////////////////////////////////////////////////////////
void FDScrollBarWidget::SetItemsViewedAtOnce(const int iItems, const bool bFireEvents)
{
	m_iItemsViewedAtOnce = iItems;
}

//////////////////////////////////////////////////////////////////////////
// Set function for the current position of the scroll bar.
//////////////////////////////////////////////////////////////////////////
void FDScrollBarWidget::SetCurrentPosition(const int iPosition, const bool bFireEvents)
{
	m_iCurrentPosition = iPosition;

	if (bFireEvents)
	{
		SignalPositionChanged.Emit(this, m_iCurrentPosition);
	}
}

//////////////////////////////////////////////////////////////////////////
// Accessor function for the number of items represented by the scroll bar.
//////////////////////////////////////////////////////////////////////////
int FDScrollBarWidget::GetNumberOfItems() const
{
	return m_iNumberOfItems;
}

//////////////////////////////////////////////////////////////////////////
// Accessor function for the number of items viewed at once.
//////////////////////////////////////////////////////////////////////////
int FDScrollBarWidget::GetItemsViewedAtOnce() const
{
	return m_iItemsViewedAtOnce;
}

//////////////////////////////////////////////////////////////////////////
// Accessor function for the current position of the scroll bar.
//////////////////////////////////////////////////////////////////////////
int FDScrollBarWidget::GetCurrentPosition() const
{
	return m_iCurrentPosition;
}

//////////////////////////////////////////////////////////////////////////
// Returns a rect in widget coordinates representing the thumb area of the
// scroll bar.
//////////////////////////////////////////////////////////////////////////
lUIRect FDScrollBarWidget::GetThumbVisibleRect() const
{
	const lUIRect cInteriorRect = GetScrollAreaVisibleRect();
	lUIRect cThumbRect = cInteriorRect;

	if (m_iNumberOfItems > 0)
	{
		const float fHeight = lmMax(((float)m_iItemsViewedAtOnce / (float)m_iNumberOfItems) * cInteriorRect.h, 6.0f);
		cThumbRect.h = lmMin(fHeight, cInteriorRect.h);

		const float fYStart = cThumbRect.y + (((float)m_iCurrentPosition / (float)m_iNumberOfItems) * cInteriorRect.h);
		cThumbRect.y = fYStart;
	};

	return cThumbRect;
}

lUIRect FDScrollBarWidget::GetThumbRect() const
{
	const lUIRect cInteriorRect = GetScrollAreaRect();
	lUIRect cThumbRect = cInteriorRect;

	if (m_iNumberOfItems > 0)
	{
		const float fHeight = lmMax(((float)m_iItemsViewedAtOnce / (float)m_iNumberOfItems) * cInteriorRect.h, 6.0f);
		cThumbRect.h = lmMin(fHeight, cInteriorRect.h);

		const float fYStart = cThumbRect.y + (((float)m_iCurrentPosition / (float)m_iNumberOfItems) * cInteriorRect.h);
		cThumbRect.y = fYStart;
	};

	return cThumbRect;
}

//////////////////////////////////////////////////////////////////////////
// Returns a rect in widget coordinates representing the interior area of
// the scroll bar.
//////////////////////////////////////////////////////////////////////////
lUIRect FDScrollBarWidget::GetScrollAreaVisibleRect() const
{
	lUIRect cInteriorRect = GetRect();
	cInteriorRect.x = 0.0f;
	cInteriorRect.y = 0.0f;
	cInteriorRect.Shrink(2.0f, 2.0f);

	return cInteriorRect;
}

lUIRect FDScrollBarWidget::GetScrollAreaRect() const
{
	lUIRect cInteriorRect = GetRect();
	cInteriorRect.x = 0.0f;
	cInteriorRect.y = 0.0f;
	cInteriorRect.Shrink(0.0f, 2.0f);

	return cInteriorRect;
}

void FDScrollBarWidget::OnMouse( const lUIPoint & p, luint32_t mouseDownFlags, luint32_t mouseUpFlags )
{
	SuperClass::OnMouse(p, mouseDownFlags, mouseUpFlags);

	// Ensure that we only slide the scroll bar once the user has actually moved the
	// mouse/stylus.
	bool bMoved = (p.x != m_cDragStartPoint.x) || (p.y != m_cDragStartPoint.y);
	if (m_bDragging && (bMoved || m_bDragStartedMove))
	{
		m_bDragStartedMove = true;

		// Update the position of the thumb based on the mouse position, and send
		// view update messages.
		const lUIRect cScrollArea = GetScrollAreaRect();
		float fRelativeY = (p.y - m_cDragStartPoint.y) + (((float)m_iDragStartItem / (float)m_iNumberOfItems) * cScrollArea.h);
		const float fPercentPosition = (fRelativeY / cScrollArea.h);
		int iDesiredPosition = (int)(fPercentPosition * m_iNumberOfItems);

		if ((iDesiredPosition + m_iItemsViewedAtOnce) > m_iNumberOfItems)
			iDesiredPosition = m_iNumberOfItems - m_iItemsViewedAtOnce;

		iDesiredPosition = lmMax(0, iDesiredPosition);

		SetCurrentPosition(iDesiredPosition, true);
	}
}

bool FDScrollBarWidget::OnMouseDown( const lUIPoint &p, luint32_t flags)
{
	SuperClass::OnMouseDown(p, flags);

	if (p.InRect(GetThumbRect()) == true)
	{
    FDSound::PlaySoundEffect( FD_SFX_UI_SCROLL );
		m_bDragging = true;
		m_cDragStartPoint = p;
		m_iDragStartItem = m_iCurrentPosition;
		CaptureMouse();
	}

	return true;
}

bool FDScrollBarWidget::OnMouseUp( const lUIPoint &p, luint32_t flags)
{
	SuperClass::OnMouseUp(p, flags);

	if (m_bDragging)
	{
    FDSound::PlaySoundEffect( FD_SFX_UI_SCROLL );
		ReleaseMouse();
		m_bDragging = false;
		m_bDragStartedMove = false;
	}
	else
	{
		const lUIRect cScrollArea = GetScrollAreaRect();
		if (p.InRect(cScrollArea) == true)
		{
			// Check if the click was above or below the thumb. If so, then
			// page in the appropriate direction.
			const lUIRect cThumbArea = GetThumbRect();

			if (p.y < cThumbArea.y)
			{
				m_iCurrentPosition = lmMax(0, m_iCurrentPosition - m_iItemsViewedAtOnce);
				SignalPositionChanged.Emit(this, m_iCurrentPosition);
        FDSound::PlaySoundEffect( FD_SFX_UI_SCROLL );
			}
			else if (p.y > (cThumbArea.y + cThumbArea.h))
			{
				m_iCurrentPosition = lmMin(m_iNumberOfItems - m_iItemsViewedAtOnce, m_iCurrentPosition + m_iItemsViewedAtOnce);
				SignalPositionChanged.Emit(this, m_iCurrentPosition);
        FDSound::PlaySoundEffect( FD_SFX_UI_SCROLL );
			}
		}
	}

	return true;
}

void FDScrollBarWidget::SetNewsScrolling( bool value )
{
  m_bNewsScroll = value;
}

void FDScrollBarWidget::AlignTo( lUIWidget *widget )
{
  const lUIRect & widgetRect = widget->GetRect();
  Move( lUIWidgetPosition( lUIPoint( widgetRect.x+widgetRect.w, widgetRect.y ) ), lUIWidgetSize( lUIPoint( GetRect().w, widgetRect.h ) ) );
}
