//-------------------------------------------------------------------
// VOImage implementation
//-------------------------------------------------------------------
//
// Copyright ©2000 Virtual Office Systems Incorporated
// All Rights Reserved
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name is included.
//
// This code can be compiled, modified and distributed freely, providing
// that this copyright information remains intact in the distribution.
//
// This code may be compiled in original or modified form in any private
// or commercial application.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage, in any form, caused
// by this code. Use it at your own risk.
//-------------------------------------------------------------------

#include "stdafx.h"

#include "VOImage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef _WIN32_WCE
#pragma comment(lib, "imgdecmp.lib")
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int		CVOImage::g_iScale = 100;
int		CVOImage::g_iMaxWidth = 10000;
int		CVOImage::g_iMaxHeight = 10000;
BOOL	CVOImage::g_bStretchBlt = FALSE;


/*------------------------------------------------------------------------------
	Method:   	CVOImage::CVOImage
	Access:    	public 
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CVOImage::CVOImage() 
	: m_hdc(0), 
	m_hbitmap(0), 
	m_dwWidth(0), 
	m_dwHeight(0), 
	m_hmoduleResource(0), 
	m_dwResourceID(0), 
	m_hbitmapOrig(0)
{
}


/*------------------------------------------------------------------------------
	Method:   	CVOImage::~CVOImage
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CVOImage::~CVOImage()
{
	if (m_hbitmap)
	{
		// You can't delete the bitmap while it's selected into a DC, so make sure it's not!
		if(m_hdc)
		{
			SelectObject(m_hdc,m_hbitmapOrig);
		}

		DeleteObject(m_hbitmap);
	}
	if(m_hdc)
	{
		DeleteDC(m_hdc);
	}
}


#ifdef LOAD_VOIMAGE_FILE
//---------------------------------------------------------------------------------------
BOOL CVOImage::Load(HDC hdc, LPCTSTR pcszFileName)
{
	if(m_hbitmap)
	{
		if(GetFileName().GetLength() && GetFileName() == pcszFileName)
		{
			return TRUE;	// Already Loaded
		}

		//DSR you can't delete the bitmap while it's selected into a DC, so make sure it's not!
		if(m_hdc)
		{
			SelectObject(m_hdc,m_hbitmapOrig);
		}

		DeleteObject(m_hbitmap);
	}

	if(!m_hdc)
	{
		m_hdc = CreateCompatibleDC(hdc);

		HBITMAP	hbitmap = CreateCompatibleBitmap(hdc, GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES));

		//DSR get the original bitmap out of the DC so we can put it back later
		m_hbitmapOrig=(HBITMAP)SelectObject(m_hdc, (HBITMAP)hbitmap);
	}

	BYTE    szBuffer[1024] = {0};
	HANDLE hFile = INVALID_HANDLE_VALUE;

#ifdef _WIN32_WCE
	HRESULT hr;

	DecompressImageInfo	dii;
#endif

	hFile = CreateFile(pcszFileName, GENERIC_READ, FILE_SHARE_READ, null, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, null );
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

#ifdef _WIN32_WCE
	// Fill in the 'DecompressImageInfo' structure
	dii.dwSize = sizeof( DecompressImageInfo );		// Size of this structure
	dii.pbBuffer = szBuffer;						// Pointer to the buffer to use for data
	dii.dwBufferMax = 1024;							// Size of the buffer
	dii.dwBufferCurrent = 0;						// The amount of data which is current in the buffer
	dii.phBM = &m_hbitmap;							// Pointer to the bitmap returned (can be null)
	dii.ppImageRender = null;						// Pointer to an IImageRender object (can be null)
	dii.iBitDepth = GetDeviceCaps(hdc,BITSPIXEL);	// Bit depth of the output image
	dii.lParam = ( LPARAM ) hFile;					// User parameter for callback functions
	dii.hdc = m_hdc;								// HDC to use for retrieving palettes
	dii.iScale = g_iScale;							// Scale factor (1 - 100)
	dii.iMaxWidth = g_iMaxWidth;					// Maximum width of the output image
	dii.iMaxHeight = g_iMaxHeight;					// Maximum height of the output image
	dii.pfnGetData = GetImageData;					// Callback function to get image data
	dii.pfnImageProgress = ImageProgress;			// Callback function to notify caller of progress decoding the image
	dii.crTransparentOverride = ( UINT ) -1;		// If this color is not (UINT)-1, it will override the
													// transparent color in the image with this color. (GIF ONLY)

	// Process and decompress the image data
	hr = DecompressImageIndirect( &dii );
#endif

	// Clean up
	CloseHandle( hFile );

	HBITMAP hbitmapOld = (HBITMAP)SelectObject(m_hdc, m_hbitmap);

	DeleteObject(hbitmapOld);

	BITMAP	bmp;

	GetObject(m_hbitmap, sizeof(BITMAP), &bmp);

	m_dwWidth = bmp.bmWidth;
	m_dwHeight = bmp.bmHeight;

	m_strFileName = pcszFileName;
	m_dwResourceID = 0;
	m_hmoduleResource = 0;

	return TRUE;
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CVOImage::Draw
	Access:    	public 
	Parameter: 	HDC _hdc
	Parameter: 	int _DestX
	Parameter: 	int _DestY
	Parameter: 	int _Width
	Parameter: 	int _Height
	Parameter: 	int _xOffset
	Parameter: 	int _yOffset
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CVOImage::Draw(HDC _hdc, int _DestX, int _DestY, int _Width /*= 0*/, int _Height /*= 0*/, int _xOffset /*= 0*/, int _yOffset /*= 0*/)
{
	BITMAP bmp;
	GetObject(m_hbitmap, sizeof(BITMAP), &bmp);
	EXT_ASSERT(_yOffset < bmp.bmHeight);
	EXT_ASSERT(_xOffset < bmp.bmWidth);
	if (_Width == 0)
	{
		_Width = bmp.bmWidth;
	}
	if (_Height == 0)
	{
		_Height = bmp.bmHeight;
	}
	BitBlt(_hdc, _DestX, _DestY, _Width, _Height, m_hdc, _xOffset, _yOffset, SRCCOPY );
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CVOImage::DoDrawTransparent
	Access:    	public 
	Parameter: 	HDC hdc
	Parameter: 	int aDestX
	Parameter: 	int aDestY
	Parameter: 	int aWidth
	Parameter: 	int aHeight
	Parameter: 	int aSrcOffsetX
	Parameter: 	int aSrcOffsetY
	Parameter: 	COLORREF TransparentColor
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CVOImage::DoDrawTransparent(HDC hdc, int aDestX, int aDestY, int aWidth, int aHeight, int aSrcOffsetX, int aSrcOffsetY, COLORREF TransparentColor)
{
	BITMAP	bmp;
	GetObject(m_hbitmap, sizeof(BITMAP), &bmp);
	if (aWidth == 0)
	{
		aWidth = GetWidth();
	}
	if (aHeight == 0)
	{
		aHeight = GetHeight();
	}
	if (TransparentColor == RGB(192, 192, 192))
	{
		TransparentColor = GetPixel(0, 0);	// If none of the above conditions are met, use the pixel at 0,0 (Upper Left)
	}
#ifdef _WIN32_WCE
	return TransparentImage(hdc, aDestX, aDestY, aWidth, aHeight, m_hdc, aSrcOffsetX, aSrcOffsetY, aWidth, aHeight, TransparentColor);
#else
	return FALSE;
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CVOImage::DoDrawTranslucent
	Access:    	public 
	Parameter: 	HDC hdc
	Parameter: 	int x
	Parameter: 	int y
	Parameter: 	int nPercent
	Parameter: 	int cx
	Parameter: 	int cy
	Parameter: 	COLORREF TransparentColor
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CVOImage::DoDrawTranslucent(HDC hdc, int x, int y, int nPercent, int cx /*= -1*/, int cy /*= -1*/, COLORREF TransparentColor /*= RGB(192, 192, 192)*/)
{
	BITMAP	bmpBG, bmpImage;
	HDC		hdcBG = CreateCompatibleDC(hdc);
	HDC		hdcImage = CreateCompatibleDC(hdc);

	if(cx == -1)
		cx = GetWidth();
	if(cy == -1)
		cy = GetHeight();

	BITMAPINFO i;

	memset( &i.bmiHeader, 0, sizeof(BITMAPINFOHEADER) );
	i.bmiHeader.biWidth=cx;
	i.bmiHeader.biHeight=cy;
	i.bmiHeader.biPlanes=1;
	i.bmiHeader.biBitCount=24;
	i.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);

	PBYTE	pBitsBG;
	HBITMAP hbitmapBG = CreateDIBSection(hdc, &i, DIB_RGB_COLORS, (PVOID*)&pBitsBG, null, 0 );

	if(hbitmapBG == 0)
	{
		uint dwError = GetLastError();

		DebugBreak();
		return FALSE;
	}

	// Copy the background into the 24bit Background DIB
	HBITMAP hbitmapOld = (HBITMAP)SelectObject(hdcBG, hbitmapBG);
	BitBlt(hdcBG, 0, 0, cx, cy, hdc, x, y, SRCCOPY);
	SelectObject(hdcBG, hbitmapOld);


	// Create a matching 24bit DIB Section to hold the image data
	PBYTE	pBitsImage;
	HBITMAP hbitmapImage = CreateDIBSection(hdc, &i, DIB_RGB_COLORS, (PVOID*)&pBitsImage, null, 0 );

	if(hbitmapImage == 0)
	{
		uint dwError = GetLastError();

		DebugBreak();
		return FALSE;
	}

	// Copy the bitmap into the 24bit DIB
	hbitmapOld = (HBITMAP)SelectObject(hdcImage, hbitmapImage);

	if(TransparentColor == RGB(192, 192, 192))
		StretchBlt(hdcImage, 0, 0, cx, cy, m_hdc, 0, 0, GetWidth(), GetHeight(), SRCCOPY);
	else
	{
#ifdef _WIN32_WCE
		BitBlt(hdcImage, 0, 0, cx, cy, hdc, x, y, SRCCOPY);
		TransparentImage(hdcImage, 0, 0, cx, cy, m_hdc, 0, 0, GetWidth(), GetHeight(), TransparentColor);
#else
		StretchBlt(hdcImage, 0, 0, cx, cy, m_hdc, 0, 0, GetWidth(), GetHeight(), SRCCOPY);
#endif
	}

	SelectObject(hdcImage, hbitmapOld);

	GetObject(hbitmapImage, sizeof(BITMAP), &bmpImage);
	GetObject(hbitmapBG, sizeof(BITMAP), &bmpBG);

	PBYTE	pPixelBG;
	PBYTE	pPixelImage;
	int		r, g, b;
	int		nPercentBG = 100 - nPercent;

	for(int yOffset = 0; yOffset < cy; yOffset++)
	{
		for(int xOffset = 0; xOffset < cx; xOffset++)
		{
			pPixelBG = (PBYTE)bmpBG.bmBits + (cx * yOffset + xOffset) * 3;
			pPixelImage = (PBYTE)bmpImage.bmBits + (cx * yOffset + xOffset) * 3;

			r = ((pPixelImage[2] * nPercent / 100) + ((int)pPixelBG[2] * nPercentBG / 100));
			g = ((pPixelImage[1] * nPercent / 100) + ((int)pPixelBG[1] * nPercentBG / 100));
			b = ((pPixelImage[0] * nPercent / 100) + ((int)pPixelBG[0] * nPercentBG / 100));

			pPixelBG[2] = r;
			pPixelBG[1] = g;
			pPixelBG[0] = b;
		}
	}

	hbitmapOld = (HBITMAP)SelectObject(hdcBG, hbitmapBG);

	BitBlt(hdc, x, y, cx, cy, hdcBG, 0, 0,SRCCOPY );

	SelectObject(hdcBG, hbitmapOld);

	DeleteObject(hbitmapImage);
	DeleteObject(hbitmapBG);

	DeleteDC(hdcImage);
	DeleteDC(hdcBG);
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CVOImage::DrawTransparent
	Access:    	public 
	Parameter: 	HDC hdc
	Parameter: 	int x
	Parameter: 	int y
	Parameter: 	int cx
	Parameter: 	int cy
	Parameter: 	COLORREF TransparentColor
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CVOImage::DrawTransparent(HDC hdc, int x, int y, int cx, int cy, COLORREF TransparentColor)
{
	if (cx == -1)
	{
		cx = GetWidth();
	}
	if (cy == -1)
	{
		cy = GetHeight();
	}

	if (TransparentColor == RGB(192, 192, 192))
	{
		TransparentColor = GetPixel(0, 0);	// If none of the above conditions are met, use the pixel at 0,0 (Uppler Left)
	}

#ifdef _WIN32_WCE
	return TransparentImage(hdc, x, y, cx, cy, m_hdc, 0, 0, GetWidth(), GetHeight(), TransparentColor);
#else
	return FALSE;
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CVOImage::GetHeight
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CVOImage::GetHeight()
{
	return m_dwHeight;
}


/*------------------------------------------------------------------------------
	Method:   	CVOImage::GetWidth
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CVOImage::GetWidth()
{
	return m_dwWidth;
}


/*------------------------------------------------------------------------------
	Method:   	CVOImage::SetBitmap
	Access:    	public 
	Parameter: 	HDC hdc
	Parameter: 	uint dwResourceID
	Parameter: 	LPCTSTR pcszClass
	Parameter: 	HMODULE hModule
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CVOImage::SetBitmap(HDC hdc, uint _ResourceID, LPCTSTR pcszClass /*= TEXT("IMAGE")*/, HMODULE hModule /*= 0 */)
{
	if (m_hbitmap)
	{
		if (m_hmoduleResource == hModule && m_dwResourceID == _ResourceID)
		{
			return TRUE;	// Already loaded
		}

		if (m_hdc)
		{
			SelectObject(m_hdc, m_hbitmapOrig);
		}
		DeleteObject(m_hbitmap);
		m_hbitmap = null;
	}

	if (!m_hdc)
	{
		m_hdc = CreateCompatibleDC(hdc);
		HBITMAP	hbitmap = CreateCompatibleBitmap(hdc, GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES));
		m_hbitmapOrig = (HBITMAP)SelectObject(m_hdc, hbitmap);
	}

	CBinaryReader res(hModule, _ResourceID, pcszClass);

	if (!res.IsLoaded())
    {
		HBITMAP hBMOld=(HBITMAP)SelectObject(m_hdc,m_hbitmapOrig);

		//delete the bitmap we selected in above...
		if(hBMOld && hBMOld != m_hbitmapOrig)
		{
			DeleteObject(hBMOld);
		}
		return FALSE;
	}
	res.SetUserData(0);	// Use this for the current resource offset

#ifdef _WIN32_WCE
	BYTE szBuffer[1024] = {0};
	HRESULT hr;
	DecompressImageInfo	dii;
	// Fill in the 'DecompressImageInfo' structure
	dii.dwSize = sizeof( DecompressImageInfo );		// Size of this structure
	dii.pbBuffer = szBuffer;						// Pointer to the buffer to use for data
	dii.dwBufferMax = 1024;							// Size of the buffer
	dii.dwBufferCurrent = 0;						// The amount of data which is current in the buffer
	dii.phBM = &m_hbitmap;							// Pointer to the bitmap returned (can be null)
	dii.ppImageRender = null;						// Pointer to an IImageRender object (can be null)
	dii.iBitDepth = GetDeviceCaps(hdc,BITSPIXEL);	// Bit depth of the output image
	dii.lParam = ( LPARAM ) &res;					// User parameter for callback functions
	dii.hdc = m_hdc;								// HDC to use for retrieving palettes
	dii.iScale = g_iScale;							// Scale factor (1 - 100)
	dii.iMaxWidth = g_iMaxWidth;					// Maximum width of the output image
	dii.iMaxHeight = g_iMaxHeight;					// Maximum height of the output image
	dii.pfnGetData = GetImageResourceData;			// Callback function to get image data
	dii.pfnImageProgress = ImageProgress;			// Callback function to notify caller of progress decoding the image
	dii.crTransparentOverride = ( UINT ) -1;		// If this color is not (UINT)-1, it will override the
																								// transparent color in the image with this color. (GIF ONLY)
	// Process and decompress the image data
	hr = DecompressImageIndirect( &dii );
#endif

	// Get rid of the bitmap we selected into the dc when we created it, and select in m_hBitmap
	HBITMAP hbitmapOld = (HBITMAP)SelectObject(m_hdc, m_hbitmap);

	if(hbitmapOld && hbitmapOld != m_hbitmapOrig)
	{
		DeleteObject(hbitmapOld);
	}

	BITMAP	bmp;

	GetObject(m_hbitmap, sizeof(BITMAP), &bmp);

	m_dwWidth = bmp.bmWidth;
	m_dwHeight = bmp.bmHeight;

#ifdef LOAD_VOIMAGE_FILE
	m_strFileName = TEXT("");
#endif
	m_hmoduleResource = hModule;
	m_dwResourceID = _ResourceID;

	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CVOImage::GetImageData
	Access:    	public static 
	Parameter: 	LPSTR szBuffer
	Parameter: 	DWORD dwBufferMax
	Parameter: 	LPARAM lParam
	Returns:   	DWORD CALLBACK
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
DWORD CALLBACK CVOImage::GetImageData(LPSTR szBuffer, DWORD dwBufferMax, LPARAM lParam )
{
	DWORD dwNumberOfBytesRead;

	if ( (HANDLE)lParam == INVALID_HANDLE_VALUE )
	{
		return 0;
	}

	ReadFile( (HANDLE)lParam, szBuffer, dwBufferMax, &dwNumberOfBytesRead, null );

	// Return number of bytes read
	return dwNumberOfBytesRead;
}


/*------------------------------------------------------------------------------
	Method:   	CVOImage::GetImageResourceData
	Access:    	public 
	Parameter: 	LPSTR szBuffer
	Parameter: 	DWORD dwBufferMax
	Parameter: 	LPARAM lParam
	Returns:   	DWORD CALLBACK
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
DWORD CALLBACK CVOImage::GetImageResourceData(LPSTR szBuffer, DWORD dwBufferMax, LPARAM lParam)
{
	DWORD dwNumberOfBytesToRead = dwBufferMax;
	CBinaryReader* pRes = (CBinaryReader*) lParam;

	if(!pRes)
		return 0;

	uint dwResourceOffset = pRes->GetUserData();

	if(dwResourceOffset + dwNumberOfBytesToRead > pRes->GetSize() )
		dwNumberOfBytesToRead = pRes->GetSize() - dwResourceOffset;

	memmove(szBuffer, pRes->GetData() + dwResourceOffset, dwNumberOfBytesToRead);

	pRes->SetUserData(dwResourceOffset + dwNumberOfBytesToRead);
	return dwNumberOfBytesToRead;	// return amount read
}


#ifdef _WIN32_WCE
/*------------------------------------------------------------------------------
	Method:   	CVOImage::ImageProgress
	Access:    	public static 
	Parameter: 	IImageRender * pRender
	Parameter: 	BOOL bComplete
	Parameter: 	LPARAM lParam
	Returns:   	void CALLBACK
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CALLBACK CVOImage::ImageProgress(IImageRender *pRender, BOOL bComplete, LPARAM lParam )
{
	if (bComplete)
	{
		;// (Optional) add code here for completion processing
	}
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CVOImage::IsLoaded
	Access:    	public 
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CVOImage::IsLoaded()
{
	return (m_hbitmap != 0);
}


/*------------------------------------------------------------------------------
	Method:   	CVOImage::GetPixel
	Access:    	public 
	Parameter: 	int x
	Parameter: 	int y
	Returns:   	COLORREF
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
COLORREF CVOImage::GetPixel(int x, int y)
{
	if(m_hdc == 0)
		return RGB(0, 0, 0);

	return ::GetPixel(m_hdc, x, y);
}


/*------------------------------------------------------------------------------
	Method:   	CVOImage::SetCanvasSize
	Access:    	public 
	Parameter: 	int x
	Parameter: 	int y
	Parameter: 	COLORREF rgbBackground
	Parameter: 	BOOL fHCenter
	Parameter: 	BOOL fVCenter
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CVOImage::SetCanvasSize(int x, int y, COLORREF rgbBackground, BOOL fHCenter, BOOL fVCenter)
{
	if(rgbBackground == RGB(1, 1, 1))
	{
		rgbBackground = GetPixel(0, 0);
	}
	if(x < (int)GetWidth() || y < (int)GetHeight())
	{
		return false;	// Cropping may be implemented at some point in the future
	}
	int nOffsetX = 0;
	int nOffsetY = 0;

	if(fHCenter)
		nOffsetX = (x - GetWidth()) / 2;

	if(fVCenter)
		nOffsetY = (y - GetHeight()) / 2;

	BITMAP	bm, bmNew;
	HBITMAP hbitmapNew;

	::GetObject(m_hbitmap, sizeof(BITMAP), &bm);

	HDC hdc = CreateCompatibleDC(m_hdc);

	hbitmapNew = CreateCompatibleBitmap(m_hdc, x, y);
	SelectObject(hdc, hbitmapNew);

	RECT rect;

	rect.left = 0;
	rect.top = 0;
	rect.right = x;
	rect.bottom = y;

	FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

	if(!BitBlt(hdc, nOffsetX, nOffsetY, GetWidth(), GetHeight(), m_hdc, 0, 0, SRCCOPY))
	{
		DeleteDC(hdc);
		DeleteObject(hbitmapNew);

		return FALSE;
	}

	HBITMAP hPrev = (HBITMAP) ::GetObject(hbitmapNew, sizeof(BITMAP), &bmNew);

	::SelectObject(hdc, hPrev);

	DeleteDC(m_hdc);
	DeleteObject(m_hbitmap);

	m_hdc = hdc;
	m_hbitmap = hbitmapNew;

	return TRUE;
}
