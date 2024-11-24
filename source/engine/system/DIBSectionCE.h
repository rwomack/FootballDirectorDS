#if !defined(AFX_CDIBSECTIONCE_H__35D9F3D4_B960_11D2_A981_2C4476000000__INCLUDED_)
#define AFX_CDIBSECTIONCE_H__35D9F3D4_B960_11D2_A981_2C4476000000__INCLUDED_


// DIBSectionCE.h : header file
//

// Copyright © Dundas Software Ltd. 1999, All Rights Reserved

// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class(does not have any objects)
//	NO	Derived from CWnd
//	NO	Is a CWnd.
//	NO	Two stage creation(constructor & Create())
//	NO	Has a message map
//	NO 	Needs a resource(template)
//	YES	Persistent objects(saveable on disk)
//	YES	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :

// CDIBSectionCE is DIBSection wrapper class for win32 and WinCE platforms.
// This class provides a simple interface to DIBSections including loading,
// saving and displaying DIBsections.
//
// Full palette support is provided for Win32 and CE 2.11 and above.

// Using CDIBSectionCE :

// This class is very simple to use. The bitmap can be set using either SetBitmap()
// (which accepts either a Device dependant or device independant bitmap, or a
// resource ID) or by using Load(), which allows an image to be loaded from disk.
// To display the bitmap simply use Draw or Stretch.
//
// eg.
//
//      CDIBsection dibsection;
//      dibsection.Load(_T("image.bmp"));
//      dibsection.Draw(pDC, CPoint(0,0));  // pDC is of type CDC*
//
//      CDIBsection dibsection;
//      dibsection.SetBitmap(IDB_BITMAP);
//      dibsection.Draw(pDC, CPoint(0,0));  // pDC is of type CDC*
//
// The CDIBsection API includes many methods to extract information about the
// image, as well as palette options for getting and setting the current palette.
//
// Author: Chris Maunder(cmaunder@dundas.com)
// Date  : 12 April 1999


/////////////////////////////////////////////////////////////////////////////
// defines

// Only provide palette support for non-CE platforms, or for CE 2.11 and above
#if defined(_WIN32_WCE) && (_WIN32_WCE < 211)

#define DIBSECTION_NO_PALETTE

// Dummy defn for CPalette if it is not available
#ifndef CPalette
#define CPalette void
#endif

#endif


#define DS_BITMAP_FILEMARKER  ((WORD) ('M' << 8) | 'B')    // is always "BM" = 0x4D42

/////////////////////////////////////////////////////////////////////////////
// BITMAPINFO wrapper

struct DIBINFO : public BITMAPINFO
{
	RGBQUAD	 arColors[255];    // Color table info - adds an extra 255 entries to palette

	operator LPBITMAPINFO()          { return(LPBITMAPINFO) this; }
	operator LPBITMAPINFOHEADER()    { return &bmiHeader;          }
	RGBQUAD* ColorTable()            { return bmiColors;           }
};

/////////////////////////////////////////////////////////////////////////////
// LOGPALETTE wrapper

#ifndef DIBSECTION_NO_PALETTE
struct PALETTEINFO : public LOGPALETTE
{
    PALETTEENTRY arPalEntries[255];               // Palette entries

    PALETTEINFO() { palVersion = (WORD) 0x300; }

    operator LPLOGPALETTE()   { return(LPLOGPALETTE) this;            }
    operator LPPALETTEENTRY() { return(LPPALETTEENTRY) (palPalEntry); }
};
#endif // DIBSECTION_NO_PALETTE


/////////////////////////////////////////////////////////////////////////////
// CDIBSectionCE object

class CDIBSectionCE : public CObject
{
// Construction
public:
	CDIBSectionCE();
	virtual ~CDIBSectionCE();
    void DeleteObject();

// static helpers
public:
    static int BytesPerLine(int nWidth, int nBitsPerPixel);
    static int NumColorEntries(int nBitsPerPixel);

    static RGBQUAD ms_StdColours[];
#ifndef DIBSECTION_NO_PALETTE
    static BOOL UsesPalette(CDC* pDC) { return(pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE); }
    static BOOL CreateHalftonePalette(CPalette& palette, int nNumColours);
#endif // DIBSECTION_NO_PALETTE

// Attributes
public:
    HBITMAP      GetSafeHandle() const       { return(this)? m_hBitmap : NULL;        }
    CSize        GetSize() const             { return CSize(GetWidth(), GetHeight());  }
    int          GetHeight() const           { return m_DIBinfo.bmiHeader.biHeight;    }
    int          GetWidth() const            { return m_DIBinfo.bmiHeader.biWidth;     }
    int          GetPlanes() const           { return m_DIBinfo.bmiHeader.biPlanes;    }
    int          GetBitCount() const         { return m_DIBinfo.bmiHeader.biBitCount;  }
    LPVOID       GetDIBits()                 { return m_ppvBits;                       }
    LPBITMAPINFO GetBitmapInfo()             { return  (BITMAPINFO*) m_DIBinfo;        }
    DWORD        GetImageSize() const        { return m_DIBinfo.bmiHeader.biSizeImage; }
    LPBITMAPINFOHEADER GetBitmapInfoHeader() { return(BITMAPINFOHEADER*) m_DIBinfo;   }
    LPRGBQUAD    GetColorTable()             { return m_DIBinfo.ColorTable();          }

    BOOL SetBitmap(UINT nIDResource);
    BOOL SetBitmap(LPCTSTR lpszResourceName);
    BOOL SetBitmap(HBITMAP hBitmap, CPalette* pPalette = NULL);
    BOOL SetBitmap(LPBITMAPINFO lpBitmapInfo, LPVOID lpBits);

#ifndef DIBSECTION_NO_PALETTE
    CPalette *GetPalette()  { return &m_Palette; }
    BOOL SetPalette(CPalette* pPalette);
    BOOL SetLogPalette(LOGPALETTE* pLogPalette);
#endif // DIBSECTION_NO_PALETTE

// Operations
public:
    BOOL Load(LPCTSTR lpszFileName);
    BOOL Save(LPCTSTR lpszFileName);

    BOOL Draw(CDC* pDC, CPoint ptDest, BOOL bForceBackground = FALSE);
    BOOL Stretch(CDC* pDC, CPoint ptDest, CSize size, BOOL bForceBackground = FALSE);

    CDC* GetMemoryDC(CDC* pDC = NULL, BOOL bSelectPalette = TRUE);
    BOOL ReleaseMemoryDC(BOOL bForceRelease = FALSE);

// Overrideables

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Implementation
protected:
    void _ShowLastError();
#ifndef DIBSECTION_NO_PALETTE
    BOOL CreatePalette();
    BOOL FillDIBColorTable(UINT nNumColours, RGBQUAD *pRGB);
#endif // DIBSECTION_NO_PALETTE
    UINT GetColorTableEntries(HDC hdc, HBITMAP hBitmap);

protected:
    HBITMAP m_hBitmap;          // Handle to DIBSECTION
    DIBINFO m_DIBinfo;          // Bitmap header & color table info
    VOID   *m_ppvBits;          // Pointer to bitmap bits
    UINT    m_iColorDataType;   // color data type(palette or RGB values)
    UINT    m_iColorTableSize;  // Size of color table

    CDC      m_MemDC;           // Memory DC for drawing on bitmap
    BOOL     m_bReuseMemDC;     // Reeuse the memory DC? (Quicker, but maybe unsafe)

#ifndef DIBSECTION_NO_PALETTE
    CPalette m_Palette;         // Color palette
    CPalette *m_pOldPalette;
#endif // DIBSECTION_NO_PALETTE

private:
    HBITMAP  m_hOldBitmap;      // Storage for previous bitmap in Memory DC
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDIBSECTIONCE_H__35D9F3D4_B960_11D2_A981_2C4476000000__INCLUDED_)
