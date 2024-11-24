
/*

	OffScreenBuffer.cpp: implementation of the COffScreenBuffer class.


	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "OffScreenBuffer.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW 
#endif



/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::COffScreenBuffer
	Access:    	public 
	Parameter: 	const ushort nMaxX
	Parameter: 	const ushort nMaxY
	Parameter: 	CWnd * pParentWnd
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
COffScreenBuffer::COffScreenBuffer(const ushort nMaxX, const ushort nMaxY, CWnd* pParentWnd)
	: m_hOldBitmap(null)
{
	EXT_ASSERT(nMaxX > 0 && nMaxY > 0);
	EXT_ASSERT_POINTER(pParentWnd, CWnd);
	DoInitialise(nMaxX, nMaxY, pParentWnd);
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::COffScreenBuffer
	Access:    	public 
	Parameter: 	CWnd * pParentWnd
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
COffScreenBuffer::COffScreenBuffer(CWnd* _pParentWnd)
{
	EXT_ASSERT_POINTER(_pParentWnd, CWnd);
	m_bkBrush.CreateStockObject(WHITE_BRUSH);
	CRect rect;
	_pParentWnd->GetClientRect(&rect);
	DoInitialise(rect.right, rect.bottom, _pParentWnd);
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::~COffScreenBuffer
	Access:    	virtual public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
COffScreenBuffer::~COffScreenBuffer()
{
	DestroyBitmap();
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::SetParentWnd
	Access:    	public 
	Parameter: 	CWnd * _pParentWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::SetParentWnd(CWnd* _pParentWnd)
{
	m_pParentWnd = _pParentWnd;
	EXT_ASSERT_POINTER(m_pParentWnd, CWnd);
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DestroyBitmap
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:	Put the old BM back or it will leak!
------------------------------------------------------------------------------*/
void COffScreenBuffer::DestroyBitmap()
{
	if(m_hOldBitmap)
	{
		::SelectObject(m_hDC, m_hOldBitmap);
	}
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DoCreateBitmap
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DoCreateBitmap()
{
	EXT_ASSERT_POINTER(m_pParentWnd, CWnd);
	DestroyBitmap();
	CClientDC clientDC(m_pParentWnd);
	CDC::CreateCompatibleDC(&clientDC);
	m_bmp.CreateCompatibleBitmap(&clientDC, m_BufferWidth, m_BufferHeight);
	m_hOldBitmap = (HBITMAP)::SelectObject(this->m_hDC,m_bmp.m_hObject);
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::Blit
	Access:    	public 
	Parameter: 	const ushort nTopY
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::Blit(const ushort _TopY /*= 0*/)
{
	EXT_ASSERT_POINTER(m_pParentWnd, CWnd);
    CPaintDC dc(m_pParentWnd);
	CRect rectWnd;
	m_pParentWnd->GetClientRect(&rectWnd);
	dc.BitBlt(0, 0/*_TopY*/, rectWnd.Width(), rectWnd.Height() /*- _TopY*/, this, 0, 0/*_TopY*/, SRCCOPY);
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::GetBufferWidth
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int COffScreenBuffer::GetBufferWidth()
{
	return m_BufferWidth;
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::GetBufferHeight
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int COffScreenBuffer::GetBufferHeight()
{
	return m_BufferHeight;
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::Initialise
	Access:    	public 
	Parameter: 	const ushort nMaxX
	Parameter: 	const ushort nMaxY
	Parameter: 	CWnd * pParentWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DoInitialise(const ushort _MaxX, const ushort _MaxY, CWnd* _ParentWnd)
{
	EXT_ASSERT_POINTER(_ParentWnd, CWnd);
	m_pParentWnd = _ParentWnd;
	m_BufferWidth = _MaxX;
	m_BufferHeight = _MaxY;
	DoCreateBitmap();
	CDC::SetBkMode(TRANSPARENT);
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DrawBox
	Access:    	public 
	Parameter: 	CRect & _Rect
	Parameter: 	const COLORREF _Color
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DrawBox(CRect& _Rect, const COLORREF _Color /*= BLACK*/, const int _Thickness /*= 1*/)
{
	DrawLine(CPoint(_Rect.left, _Rect.top), CPoint(_Rect.right, _Rect.top), _Color, _Thickness);
	DrawLine(CPoint(_Rect.left, _Rect.top), CPoint(_Rect.left, _Rect.bottom), _Color, _Thickness);
	DrawLine(CPoint(_Rect.right, _Rect.top), CPoint(_Rect.right, _Rect.bottom), _Color, _Thickness);
	DrawLine(CPoint(_Rect.left, _Rect.bottom), CPoint(_Rect.right, _Rect.bottom), _Color, _Thickness);
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DrawSeperator
	Access:    	public 
	Parameter: 	const int y
	Parameter: 	const COLORREF color
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DrawSeperator(const int y, const COLORREF color/* = BLACK*/)
{
	EXT_ASSERT(y < m_BufferHeight);
	CPen borderpen(PS_SOLID, 1, color);
	CPen* old = SelectObject(&borderpen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(::GetStockObject(NULL_BRUSH));
	MoveTo(0, y);
	LineTo(m_BufferWidth, y);		// Top
	SelectObject(hOldBrush);
	SelectObject(old);
	borderpen.DeleteObject();
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DrawLine
	Access:    	public 
	Parameter: 	CPoint & _PointFrom
	Parameter: 	CPoint & _PointTo
	Parameter: 	const COLORREF _Color
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DrawLine(CPoint& _PointFrom, CPoint& _PointTo, const COLORREF _Color /*= BLACK*/, const int _Thickness /*= 1*/)
{
	CPen borderpen(PS_SOLID, _Thickness, _Color);
	CPen* old = SelectObject(&borderpen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(::GetStockObject(NULL_BRUSH));
	MoveTo(_PointFrom.x, _PointFrom.y);
	LineTo(_PointTo.x, _PointTo.y);		// Top
	SelectObject(hOldBrush);
	SelectObject(old);
	borderpen.DeleteObject();
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::GetParentWnd
	Access:    	public 
	Returns:   	CWnd*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CWnd* COffScreenBuffer::GetParentWnd()
{
	EXT_ASSERT_POINTER(m_pParentWnd, CWnd);
	return m_pParentWnd;
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DrawBar
	Access:    	public 
	Parameter: 	const int _YPos
	Parameter: 	const int _Value1
	Parameter: 	const int _Value2
	Parameter: 	const COLORREF _Colour1
	Parameter: 	const COLORREF _Colour2
	Parameter: 	const int _BarHeight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DrawBar(const int _YPos, const int _Value1, const int _Value2, 
							const COLORREF _Colour1, const COLORREF _Colour2, const int _BarHeight /*= 17*/)
{
	CRect bar;
	CString str;
	const int iBarLength = 220;
	const int iBarLeft = GetBufferWidth() / 2 - iBarLength / 2;

	if (_Value1 > 0 || _Value2 > 0)
	{
		int IncVal = iBarLength / (_Value1 + _Value2);

		bar.SetRect(iBarLeft, _YPos, iBarLeft + (IncVal * _Value1), _YPos + _BarHeight);
		FillSolidRect(bar, _Colour1);

		bar.SetRect(iBarLeft + (IncVal * _Value1), _YPos, iBarLeft + iBarLength, _YPos + _BarHeight);
		FillSolidRect(bar, _Colour2);

	}
	else
	{
		// Both bars equal
		bar.SetRect(iBarLeft, _YPos, iBarLeft + (iBarLength / 2), _YPos + _BarHeight);
		FillSolidRect(bar, _Colour1);

		bar.SetRect(iBarLeft + (iBarLength / 2), _YPos, iBarLeft + iBarLength, _YPos + _BarHeight);
		FillSolidRect(bar, _Colour2);
	}
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DoDrawBlobsRight
	Access:    	public 
	Parameter: 	CRect & _DataRect
	Parameter: 	const int _Num
	Parameter: 	const COLORREF _Colour
	Parameter: 	const int _BlobWidth
	Parameter: 	const int _BlobHeight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DoDrawBlobsRight(CRect& _DataRect, const int _Num, const COLORREF _Colour, const int _BlobWidth /*= 8*/, const int _BlobHeight /*= 8*/)
{
	CRect bar (_DataRect.left, _DataRect.top, _DataRect.left + _BlobWidth, _DataRect.top + _BlobHeight);
	if (_Num > 0)
	{
		for (int LoopCount = 0; LoopCount < _Num; LoopCount++)
		{
			FillSolidRect(bar, _Colour);
			bar.left += (_BlobWidth + 1);
			bar.right += (_BlobWidth + 1);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DoDrawBlobsLeft
	Access:    	public 
	Parameter: 	CRect & _DataRect
	Parameter: 	const int _Num
	Parameter: 	COLORREF _Colour
	Parameter: 	const int _BlobWidth
	Parameter: 	const int _BlobHeight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DoDrawBlobsLeft(CRect& _DataRect, const int _Num, COLORREF _Colour, const int _BlobWidth /*= 8*/, const int _BlobHeight /*= 8*/)
{
	CRect bar(_DataRect.left - _BlobWidth, _DataRect.top, _DataRect.left, _DataRect.top + _BlobHeight);
	if (_Num > 0)
	{
		for (int LoopCount = 0; LoopCount < _Num; LoopCount++)
		{
			FillSolidRect(bar, _Colour);
			bar.left -= (_BlobWidth + 1);
			bar.right -= (_BlobWidth + 1);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DisplayRightTitle
	Access:    	public 
	Parameter: 	CString & _szTitle
	Parameter: 	CRect * _pDataRect
	Parameter: 	const int _x
	Parameter: 	const int _y
	Parameter: 	const COLORREF _Colour
	Parameter: 	const bool _bTransparent
	Parameter: 	int _FontSize
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DisplayRightTitle(CString& _szTitle, CRect* _pDataRect, const int _x, const int _y, 
											const COLORREF _Colour /*= BLACK*/, const bool _bTransparent /*= false*/, int _FontSize /*= 18*/)
{
	_pDataRect->left = _x - GetTextExtent(_szTitle).cx;
	_pDataRect->top = _y;
	DisplayText(_szTitle, _pDataRect, _Colour, _FontSize, _bTransparent);
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DisplayTitle
	Access:    	public 
	Parameter: 	const int _Title
	Parameter: 	CRect * _pDataRect
	Parameter: 	const int _x
	Parameter: 	const COLORREF _Colour
	Parameter: 	const int _FontSize
	Parameter: 	const bool _bTransparent
	Parameter: 	const int _Weight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DisplayTitle(const int _Title, CRect* _pDataRect, const int _x, const COLORREF _Colour,
									const int _FontSize, const bool _bTransparent, const int _Weight)
{
	CString szTitle;
	szTitle.LoadString(_Title);
	DisplayTitle(szTitle, _pDataRect, _x, _Colour, _FontSize, _bTransparent, _Weight);
}

/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DisplayTitle
	Access:    	public 
	Parameter: 	CString & _szTitle
	Parameter: 	CRect * _pDataRect
	Parameter: 	const int _x
	Parameter: 	const COLORREF _Colour
	Parameter: 	const int _FontSize
	Parameter: 	const bool _bTransparent
	Parameter: 	const int _Weight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DisplayTitle(CString& _szTitle, CRect* _pDataRect,  const int _x, const COLORREF _Colour,
									   const int _FontSize, const bool _bTransparent, const int _Weight)
{
	if (_bTransparent == true)
	{
		CDC::SetBkMode(TRANSPARENT);
	}
	CFont* pfontCurrent = null;
	CFont Fontbig;
	if (_FontSize > 0)
	{
		Fontbig.CreateFont(_FontSize, 0, 0, 0, _Weight, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
		pfontCurrent = SelectObject(&Fontbig);
	}
	_pDataRect->left = _x - GetTextExtent(_szTitle).cx / 2;
	SetTextColor(_Colour);
	ExtTextOut(_pDataRect->left, _pDataRect->top, 0, _pDataRect, _szTitle, _szTitle.GetLength(), null);
	if (_bTransparent == true)
	{
		CDC::SetBkMode(OPAQUE);
	}
	if (_FontSize > 0)
	{
		EXT_ASSERT_POINTER(pfontCurrent, CFont);
		SelectObject(pfontCurrent);
		Fontbig.DeleteObject();
	}
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DisplayText
	Access:    	public 
	Parameter: 	const int _Title
	Parameter: 	CRect * _pDataRect
	Parameter: 	const COLORREF _Colour
	Parameter: 	const int _FontSize
	Parameter: 	const bool _bTransparent
	Parameter: 	const int _Weight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DisplayText(const int _Title, CRect* _pDataRect, const COLORREF _Colour, const int _FontSize,
									  const bool _bTransparent,  const int _Weight)
{
	CString szTitle;
	szTitle.LoadString(_Title);
	DisplayText(szTitle, _pDataRect, _Colour, _FontSize, _bTransparent, _Weight);
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DisplayText
	Access:    	public 
	Parameter: 	CString & szTitle
	Parameter: 	CRect * _pDataRect
	Parameter: 	const COLORREF _Colour
	Parameter: 	const int _FontSize
	Parameter: 	const bool _bTransparent
	Parameter: 	const int _Weight
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DisplayText(CString& szTitle, CRect* _pDataRect, 
									  const COLORREF _Colour /*= BLACK*/,  const int _FontSize /*= 18*/, 
									  const bool _bTransparent /*= false*/, const int _Weight /*= FW_MEDIUM*/)
{
	if (_bTransparent == true)
	{
		CDC::SetBkMode(TRANSPARENT);
	}
	CFont* pfontCurrent = null;
	CFont Fontbig;
	if (_FontSize > 0)
	{
		Fontbig.CreateFont(_FontSize, 0, 0, 0, _Weight, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
		pfontCurrent = SelectObject(&Fontbig);
	}
	SetTextColor(_Colour);
	ExtTextOut(_pDataRect->left, _pDataRect->top, 0, _pDataRect, szTitle, szTitle.GetLength(), null);
	if (_bTransparent == true)
	{
		CDC::SetBkMode(OPAQUE);
	}
	if (_FontSize > 0)
	{
		EXT_ASSERT_POINTER(pfontCurrent, CFont);
		SelectObject(pfontCurrent);
		Fontbig.DeleteObject();
	}
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DisplayNumeric
	Access:    	public 
	Parameter: 	int _Value
	Parameter: 	CRect * _pDataRect
	Parameter: 	const int _x
	Parameter: 	const int _y
	Parameter: 	const COLORREF _Colour
	Parameter: 	const bool _bTransparent
	Parameter: 	int _FontSize
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DisplayNumeric(int _Value, CRect* _pDataRect, const int _x, const int _y, 
										 const COLORREF _Colour /*= BLACK*/, const bool _bTransparent /*= false*/, int _FontSize /*= 18*/)
{
	CString Str;
	Str.Format(TEXT("%d"), _Value);
	DisplayRightTitle(Str, _pDataRect, _x, _y, _Colour, _bTransparent, _FontSize);
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DrawNumRight
	Access:    	public 
	Parameter: 	const int _x
	Parameter: 	CRect _DataRect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DrawNumRight(const int _x, CRect _DataRect)
{
	CString strPrint;
	strPrint.Format(_T("%d"), _x);
	_DataRect.left -= GetTextExtent(strPrint).cx;
	CDC::ExtTextOut(_DataRect.left, _DataRect.top, 0, _DataRect, strPrint, strPrint.GetLength(), null);
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::StripedRect
	Access:    	public 
	Parameter: 	const int _HowMany
	Parameter: 	CRect _TheRect
	Parameter: 	const COLORREF _color[2]
	Parameter: 	const eBorderType _bBorder
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::StripedRect(const int _HowMany, CRect _TheRect, const COLORREF _color[2], const eBorderType _bBorder /*= OUTERBORDER*/)
{
	CRect rectBox(_TheRect);
	if (_bBorder == OUTERBORDER)
	{
		rectBox.left--;
		rectBox.top--;
		rectBox.right++;
		rectBox.bottom++;
	}
	if (_bBorder != NOBORDER)
	{
		Rectangle(&rectBox);
	}
	int nStripeHeight = (_TheRect.bottom - _TheRect.top) / _HowMany;
	_TheRect.bottom = _TheRect.top + nStripeHeight;
	for (int LoopCount = 0; LoopCount < _HowMany; LoopCount++)
	{
		FillSolidRect(_TheRect, _color[LoopCount % 2]);
		_TheRect.bottom += nStripeHeight;
		_TheRect.top += nStripeHeight;
	}
}


/*------------------------------------------------------------------------------
	Method:   	COffScreenBuffer::DisplayNoData
	Access:    	public 
	Parameter: 	const int _String
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COffScreenBuffer::DisplayNoData(const int _String)
{
	CRect DataRect;
	CString str;
	str.LoadString(_String);
	CFont BigFont;
	BigFont.CreateFont(28, 0, 0, 0, FW_MEDIUM + 75, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	CFont* pfontCurrent = SelectObject(&BigFont);
	DataRect.left = (GetBufferWidth() / 2) - (GetTextExtent(str).cx / 2);
	SetTextColor(BLACK);
	DataRect.top = GetBufferHeight() / 2 + 14;
	ExtTextOut(DataRect.left, DataRect.top, 0, DataRect, str, str.GetLength(), null);
	SelectObject(pfontCurrent);
	BigFont.DeleteObject();
}
