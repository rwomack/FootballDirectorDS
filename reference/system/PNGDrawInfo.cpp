
/*

  Program:- PNGDrawInfo.cpp
  
	
  (c) RW Software 2004 - 2008
		  
  ---------------------------------------------------------------
*/



#include "stdafx.h"

#include "UI.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CPNGDrawInfo::CPNGDrawInfo
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPNGDrawInfo::CPNGDrawInfo()
	: m_Height(0)
	, m_OffsetX(0)
	, m_OffsetY(0)
	, m_Width(0)
{

}


/*------------------------------------------------------------------------------
	Method:   	CPNGDrawInfo::~CPNGDrawInfo
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPNGDrawInfo::~CPNGDrawInfo()
{
}


/*------------------------------------------------------------------------------
	Method:   	CPNGDrawInfo::GetHeight
	Access:    	public 
	Returns:   	int
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
int CPNGDrawInfo::GetHeight() const
{
	return m_Height;
}


/*------------------------------------------------------------------------------
	Method:   	CPNGDrawInfo::GetWidth
	Access:    	public 
	Returns:   	int
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
int CPNGDrawInfo::GetWidth() const
{
	return m_Width;
}


/*------------------------------------------------------------------------------
	Method:   	CPNGDrawInfo::SetOffsetX
	Access:    	public 
	Parameter: 	const int aOffsetX
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPNGDrawInfo::SetOffsetX(const int _OffsetX)
{
	m_OffsetX = _OffsetX;
}


/*------------------------------------------------------------------------------
	Method:   	CPNGDrawInfo::SetOffsetY
	Access:    	public 
	Parameter: 	const int _OffsetY
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPNGDrawInfo::SetOffsetY(const int _OffsetY)
{
	m_OffsetY = _OffsetY;
}


/*------------------------------------------------------------------------------
	Method:   	CPNGDrawInfo::GetImage
	Access:    	public 
	Returns:   	CVOImage&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CVOImage& CPNGDrawInfo::GetImage()
{
	return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CPNGDrawInfo::DoInitialise
	Access:    	public 
	Parameter: 	const HDC _hDC
	Parameter: 	const int _GFXID
	Parameter: 	const int _NumImages
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPNGDrawInfo::DoInitialise(const HDC _hDC, const int _GFXID, const int _NumImages /*= 1*/)
{
	CVOImage::SetBitmap(_hDC, _GFXID, TEXT("PNG"));
	m_Width = CVOImage::GetWidth() / _NumImages;
	m_Height = CVOImage::GetHeight();
}


/*------------------------------------------------------------------------------
	Method:   	CPNGDrawInfo::DoDrawTransparent
	Access:    	public 
	Parameter: 	const HDC _hDC
	Parameter: 	const int _XPos
	Parameter: 	const int _YPos
	Parameter: 	const int _ImageNumber
	Parameter: 	const COLORREF _TransparentColor
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPNGDrawInfo::DoDrawTransparent(const HDC _hDC, const int _XPos, const int _YPos, const int _ImageNumber /*= 0*/, const COLORREF _TransparentColor /*= RGB(192, 192, 192)*/)
{
	CVOImage::DoDrawTransparent(_hDC, _XPos, _YPos, m_Width, 0, m_Width * _ImageNumber, 0, _TransparentColor);
}


/*------------------------------------------------------------------------------
	Method:   	CPNGDrawInfo::DoDraw
	Access:    	public 
	Parameter: 	const HDC _hDC
	Parameter: 	const int _XPos
	Parameter: 	const int _YPos
	Parameter: 	const int _Width
	Parameter: 	const int _Height
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPNGDrawInfo::DoDraw(const HDC _hDC, const int _XPos, const int _YPos, const int _Width, const int _Height)
{
	CVOImage::Draw(_hDC, _XPos, _YPos, _Width, _Height, _XPos, _YPos);
}


/*------------------------------------------------------------------------------
	Method:   	CPNGDrawInfo::DoDraw
	Access:    	public 
	Parameter: 	const HDC _hDC
	Parameter: 	const int _XPos
	Parameter: 	const int _YPos
	Parameter: 	const int _ImageNumber
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPNGDrawInfo::DoDraw(const HDC _hDC, const int _XPos, const int _YPos, const int _ImageNumber /*= 0*/)
{
	CVOImage::Draw(_hDC, _XPos, _YPos, m_Width, 0, m_Width * _ImageNumber);
}

