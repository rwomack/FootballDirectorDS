/////////////////////////////////////////////////////////////////////////////
// File name:      STScreenBuffer.h
// Author:         Vassili Philippov (vasja@spbteam.com)
// Created:        June 2001
// Last changed:   07 August 2001
// Version:        2.0
// Description:    Class for pixel working with images. One could get access
//                 to image bits using this library or create custom image.

#if !defined(AFX_STSCREENBUFFER_H__22D62F5D_32E2_4785_B3D9_2341C11F84A3__INCLUDED_)
#define AFX_STSCREENBUFFER_H__22D62F5D_32E2_4785_B3D9_2341C11F84A3__INCLUDED_



/////////////////////////////////////////////////////////////////////////////
// BGRColor structure encapsulates color information about one point. Color
// order is Blue, Green, Red (not RGB).

struct BGRColor
{
	BGRColor() {}
	BGRColor(byte R, byte G, byte B) : m_R(R), m_G(G), m_B(B) {}
	byte m_B;
	byte m_G;
	byte m_R;
};


/////////////////////////////////////////////////////////////////////////////
// CSTScreenBuffer class provides fast drawing methods and offscreen buffer.

class CSTScreenBuffer
{
public:
	// Creates uninitialized buffer. Call Create or CreateRGB to
	// initialize the buffer.
	CSTScreenBuffer();

	virtual ~CSTScreenBuffer();

	// Creates buffer with the given size. 
	void Create(int nWidth, int nHeight);

	// Creates buffer with the given size and fills it with 
	// the given color
	void Create(int nWidth, int nHeight, COLORREF clr);

	// Creates buffer with the same width andheight as
	// the given bitmap and that contains the same picture.
	void Create(HBITMAP hBitmap);

	// Creates buffer that will contain picture from the given
 	// area of the given device context.
	void Create(CDC *pDC, CRect rect);

	// Creates buffer with the given size and uses the given
	// array as a source of pixels color information. The given
	// array should contain 3 bytes per pixel (RGB). To the array
	// size should be 3*nWidth*nHeight
	void CreateRGB(void *pData, int nWidth, int nHeight);

	// Draws buffer into the given device context starting from
	// the given point (top left corner)
	BOOL Draw(CDC* pDC, CPoint ptDest);

	// Sets color of the given point
	inline void SetPoint(int nX, int nY, byte R, byte G, byte B) {
		m_pBuffer[m_nCorrectedWidth*(m_nHeight-nY-1)+nX] = BGRColor(R,G,B);
	}

	// Returns color of the given point
	inline BGRColor GetPoint(int nX, int nY) {
		return m_pBuffer[m_nCorrectedWidth*(m_nHeight-nY-1)+nX];
	}

	// Returns array that contains points color information. Each
	// point is represented by 3 bytes in Blue, Green, Red order.
	// Array contains CorrectedWidth*Height elements (Note! not 
	// Width*Height).
	BGRColor *GetPointsArray() {
		return m_pBuffer;
	}

	// Returns real width of the screen buffer. It could be slightly more then
	// requested width. This paramater is important only when you work with
	// points array directly (using GetPointsArray function).
	int GetCorrectedWidth() {
		return m_nCorrectedWidth;
	}

	// Returns screen buffer width.
	int GetWidth() {
		return m_nWidth;
	}

	// Returns screen buffer height.
	int GetHeight() {
		return m_nHeight;
	}

	// Returns handle of the encapsulated bitmap.
	HBITMAP GetHBitmap() {
		return m_hBitmap;
	}

	// Returns screen buffer device context. After drawing this
	// device context should be closed (call ReleaseDC method). 
	CDC *GetDC();

	// Closes bitmap device context previously opened by GetDC function.
	void ReleaseDC();

public:
	// Creates bitmap with the given sizes and the given array of colors.
	static HBITMAP CreateBitmapByRGBArray(void *pData, int nWidth, int nHeight);

	// Returns minimum width that is greater then the given width and
	// that is acceptable as image width (not all numbers are acceptable)
	static int CorrectedWidth(int nWidth);

protected:
	// Creates internal bitmap and image buffer. Assignes width and
	// height properties
	BOOL CreateBitmap(int nWidth, int nHeight);

	int m_nWidth;
	int m_nHeight;
	int m_nCorrectedWidth;
	BGRColor *m_pBuffer;
	HBITMAP m_hBitmap;

	// Members related to device context
	CDC *m_pDC;
	HBITMAP m_hSaveBitmap;
};

#endif // !defined(AFX_STSCREENBUFFER_H__22D62F5D_32E2_4785_B3D9_2341C11F84A3__INCLUDED_)
