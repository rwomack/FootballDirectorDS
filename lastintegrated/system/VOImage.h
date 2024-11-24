//-------------------------------------------------------------------
// VOImage Header File
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

#if !defined(AFX_VOIMAGE_H__B83C4202_DB1E_48BE_92A5_21019F9EE6FC__INCLUDED_)
#define AFX_VOIMAGE_H__B83C4202_DB1E_48BE_92A5_21019F9EE6FC__INCLUDED_


#ifdef _WIN32_WCE
#include "\Projects\System\imgdecmp.h"
#endif

#ifdef LOAD_VOIMAGE_FILE
#include "vostring.h"
#endif

class CVOImage  
{
public:
	CVOImage();
	virtual ~CVOImage();
	operator HBITMAP() { return m_hbitmap; }

	static DWORD CALLBACK	GetImageResourceData(LPSTR szBuffer, DWORD dwBufferMax, LPARAM lParam);
	static DWORD CALLBACK	GetImageData(LPSTR, DWORD, LPARAM);

	bool					SetCanvasSize(int x, int y, COLORREF rgbBackground = RGB(1, 1, 1), BOOL fHCenter = TRUE, BOOL fVCenter = TRUE);
	COLORREF				GetPixel(int x, int y);
	BOOL					IsLoaded();
	BOOL SetBitmap(HDC hdc, uint _ResourceID, LPCTSTR pcszClass = TEXT("IMAGE"), HMODULE hModule = 0 );
	uint					GetWidth();
	uint					GetHeight();
	BOOL DoDrawTranslucent(HDC hdc, int x, int y, int nPercent, int cx = -1, int cy = -1, COLORREF TransparentColor = RGB(192, 192, 192));
	BOOL					DrawTransparent(HDC hdc, int x, int y, int cx = -1, int cy = -1, COLORREF TransparentColor = RGB(192, 192, 192));
	BOOL					DoDrawTransparent(HDC hdc, int aDestx, int aDestY, int aWidth = 0, int aHeight = 0, int aSrcOffsetX = 0, int aSrcOffsetY = 0, COLORREF TransparentColor = RGB(192, 192, 192));
	BOOL Draw(HDC _hdc, int _DestX, int _DestY, int _Width = 0, int _Height = 0, int _xOffset = 0, int _yOffset = 0);
	BOOL					Load(HDC hdc, LPCTSTR pcszFileName);
#ifdef LOAD_VOIMAGE_FILE
	const CVOString&		GetFileName() { return m_strFileName; }
#endif
	HBITMAP					GetHBitmap() { return m_hbitmap; }

#ifdef _WIN32_WCE
	static void CALLBACK ImageProgress( IImageRender *, BOOL, LPARAM);
#endif

protected:
#ifdef LOAD_VOIMAGE_FILE
	CVOString	m_strFileName;		// Empty if loaded from resource
#endif
	HMODULE		m_hmoduleResource;	// 0 if loaded from file
	uint		m_dwResourceID;		// 0 if loaded from file
	uint		m_dwHeight;
	uint		m_dwWidth;
	HDC			m_hdc;	
	HBITMAP		m_hbitmapOrig;
	HBITMAP		m_hbitmap;
	static BOOL g_bStretchBlt;
	static int	g_iMaxHeight;
	static int	g_iMaxWidth;
	static int	g_iScale;
};

#endif // !defined(AFX_VOIMAGE_H__B83C4202_DB1E_48BE_92A5_21019F9EE6FC__INCLUDED_)
