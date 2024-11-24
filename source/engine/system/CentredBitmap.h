// CentredBitmap.h: interface for the CCentredBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CENTREDBITMAP_H__7D4A0E49_17A0_4039_8E54_5292932A2B5D__INCLUDED_)
#define AFX_CENTREDBITMAP_H__7D4A0E49_17A0_4039_8E54_5292932A2B5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCentredBitmap : public CPNGDrawInfo  
{
public:
	CCentredBitmap();
	virtual ~CCentredBitmap();

	void				DoDraw(const HDC ahDC);
	void				DoInitialise(const HDC ahDC, const int aBitmapID, const int aScreenWidth, const int aScreenHeight);
	
protected:
};

#endif // !defined(AFX_CENTREDBITMAP_H__7D4A0E49_17A0_4039_8E54_5292932A2B5D__INCLUDED_)
