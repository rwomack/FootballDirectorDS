#include "stdafx.h"

#include "voimage.h"
#include "OffScreenBuffer.h"
#include "DateDisplay.h"
#include "PNGDrawInfo.h"
#include "BasicDisplay.h"
#include "GridRow.h"
#include "TextRegion.h"


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::CTextRegion
	Access:    	public
	Parameter: 	void
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTextRegion::CTextRegion(void)
	: m_Font(NULL)
	, CGridRow()
{
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::~CTextRegion
	Access:    	public
	Parameter: 	void
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTextRegion::~CTextRegion(void)
{
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::DoInitialise
	Access:    	public
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	CString _Text
	Parameter: 	CRect _Rect
	Parameter: 	void * _Pointer
	Parameter: 	int _Style
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTextRegion::DoInitialise(CBasicDisplay* _BasicDisplay, CString _Text, CRect _Rect, void* _Pointer/* = NULL*/, int _Style/* = COL_NORMAL*/)
{
  FDASSERT_POINTER(_BasicDisplay, CBasicDisplay);
	m_BasicDisplay = _BasicDisplay;
	CGridRow::SetText(_Text, _Pointer);
  FDASSERT(_Rect.left < _Rect.right);
  FDASSERT(_Rect.top < _Rect.bottom);
	m_WindowRect = _Rect;
	m_Style = _Style;
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::MoveWindow
	Access:    	public
	Parameter: 	CRect _Rect
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTextRegion::MoveWindow(CRect _Rect)
{
  FDASSERT(_Rect.left < _Rect.right);
  FDASSERT(_Rect.top < _Rect.bottom);
	m_WindowRect = _Rect;
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::GetAlignment
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CTextRegion::GetAlignment()
{
	return m_Style & COL_ALIGNMENT;
}


/*------------------------------------------------------------------------------
	Method:   	CTextRegion::OnPaint
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTextRegion::OnPaint()
{
	COffScreenBuffer* memDC =  m_BasicDisplay->GetOffScreenBuffer();
	CRect DataRect(m_WindowRect);
	if (GetBackgroundColour() != TRANSCOLOUR)
	{
		memDC->FillSolidRect(&m_WindowRect, GetBackgroundColour());
	}
	if (m_Font == null)
	{
		m_BasicDisplay->SetFontNormal();
	}
	else
	{
		m_BasicDisplay->SetFont(*m_Font);
	}

	int LeftX;
	CString Str = CGridRow::GetText();
	switch (GetAlignment())
	{
		case COL_ALIGNR:
			LeftX = DataRect.right - memDC->GetTextExtent(Str).cx;
			break;
		case COL_ALIGNL:
			LeftX = DataRect.left + 3;
			break;
		case COL_ALIGNC:
			LeftX = DataRect.left + (DataRect.right - DataRect.left) / 2 - memDC->GetTextExtent(Str).cx / 2;
			break;
		default :
      FDASSERT(false);
	}
	LeftX = max(LeftX, DataRect.left + 1);
	memDC->ExtTextOut(LeftX, DataRect.top, ETO_CLIPPED, DataRect, Str, Str.GetLength(), null);
}
