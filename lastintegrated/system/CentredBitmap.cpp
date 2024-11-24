// CentredBitmap.cpp: implementation of the CCentredBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCentredBitmap::CCentredBitmap()
{

}

CCentredBitmap::~CCentredBitmap()
{

}

void CCentredBitmap::DoInitialise(const HDC ahDC, const int aBitmapID, const int aScreenWidth, const int aScreenHeight)
{
	CPNGDrawInfo::DoInitialise(ahDC, aBitmapID);
	m_OffsetX = (getWidth() - aScreenWidth) / 2;
	m_OffsetY = (getHeight() - aScreenHeight) / 2;
}

void CCentredBitmap::DoDraw(const HDC ahDC)
{
	CVOImage::Draw(ahDC, 0, 0, 0, 0, m_OffsetX, m_OffsetY);
}
