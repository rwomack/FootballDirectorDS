
// ExtPaintDC.cpp : implementation file
//

#include "stdafx.h"
#include "ExtPaintDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


 //---------------------------------------------------------------------------------------------------------------------------
CExtPaintDC::CExtPaintDC (CWnd* pWnd) : CPaintDC (pWnd)
{
	m_NormalFont.CreateFont(15, 0, 0, 0, \
							 FW_MEDIUM, \
							 FALSE, FALSE, 0, \
							 ANSI_CHARSET, \
							 OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, \
							 DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
							 _T("Arial")
	);
	m_pfontCurrent = NULL;
}


 //---------------------------------------------------------------------------------------------------------------------------
CExtPaintDC::~CExtPaintDC()
{
	UnSelectFont();
	m_NormalFont.DeleteObject();
}


 //---------------------------------------------------------------------------------------------------------------------------
inline void CExtPaintDC::UnSelectFont()
{
	if(m_pfontCurrent != NULL)
	{
		SelectObject(m_pfontCurrent);
	}
	m_pfontCurrent=NULL;
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::SelectFont(CFont* Font)
{
	UnSelectFont();
    m_pfontCurrent = SelectObject(Font);
}



 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::SelectNormalFont()
{
	UnSelectFont();
    m_pfontCurrent = SelectObject(&m_NormalFont);
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::BevelRect(const int x1, const int y1, const int x2, const int y2, const int BevelLength)
{
	MoveTo(x1 + BevelLength, y1);
	LineTo(x2 - BevelLength, y1);		// Top
	LineTo(x2, y1 + BevelLength);
	LineTo(x2, y2 - BevelLength);		// RH edge
	LineTo(x2 - BevelLength, y2);
	LineTo(x1 + BevelLength, y2);		// Bottom
	LineTo(x1, y2 - BevelLength);
	LineTo(x1, y1 + BevelLength);
	LineTo(x1 + BevelLength, y1);
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::DrawSeperator(const int y, const COLORREF color)
{
	ASSERT(y < 320);
	CPen borderpen(PS_SOLID, 1, color);
	CPen* old = SelectObject(&borderpen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(::GetStockObject(NULL_BRUSH));
	MoveTo(0, y);
	LineTo(240, y);		// Top
	SelectObject(hOldBrush);
	SelectObject(old);
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::StripedRect(const int iHowMany, CRect TheRect, const COLORREF color[2], const bool bBorder)
{
	if (bBorder == true)
	{
		CRect rectBox(TheRect);
		rectBox.left--;
		rectBox.top--;
		rectBox.right++;
		rectBox.bottom++;
		Rectangle(&rectBox);
	}
	int nStripeHeight = (TheRect.bottom - TheRect.top) / iHowMany;
	TheRect.bottom = TheRect.top + nStripeHeight;
	for (int n = 0; n < iHowMany; n++)
	{
		FillSolidRect(TheRect, color[n % 2]);
		TheRect.bottom += nStripeHeight;
		TheRect.top += nStripeHeight;
	}
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::Stripes(const int iHowMany, int iTop, const bool ScrollBarVisible)
{
	SetBkMode(TRANSPARENT);
	int iBarLength = 224;
	if (ScrollBarVisible == false)
	{
		iBarLength = 240;
	}
	for (int n = 0; n < iHowMany; n++)
	{
		FillSolidRect(0, iTop, iBarLength, 16, PALEGREY);
		iTop += 32;
	}
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::DrawNumRight(int x, CRect DataRect)
{
	CString strPrint;
	strPrint.Format(_T("%d"), x);
	DataRect.left -= GetTextExtent(strPrint).cx;
	ExtTextOut(DataRect.left, DataRect.top, 0, DataRect, strPrint, strPrint.GetLength(), NULL);
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::DisplayNoData(const int iString)
{
	CRect DataRect;
	CString str;
	str.LoadString(iString);
	CFont BigFont;
	BigFont.CreateFont(28, 0, 0, 0, \
                          FW_MEDIUM + 75, \
                          FALSE, FALSE, 0, \
                          ANSI_CHARSET, \
                          OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, \
                          DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
                          _T("Arial")
    );
	CFont* pFontOld = SelectObject(&BigFont);
	DataRect.left = 120 - GetTextExtent(str).cx / 2;
	DataRect.top = 140;
    SetTextColor(BLACK);
	ExtTextOut(DataRect.left, DataRect.top, 0, DataRect, str, str.GetLength(), NULL);
	SelectObject(pFontOld);
	BigFont.DeleteObject();
}


//---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::DisplayText(CString& szTitle,
							  CRect* DataRect,
							  const COLORREF Colour,
							  const int iFontSize,
							  const bool bTransparent,
							  const int nWeight)
{
	if (bTransparent == true)
	{
		CDC::SetBkMode(TRANSPARENT);
	}
	CFont* pfontCurrent;;
	CFont Fontbig;
	if (iFontSize > 0)
	{
		Fontbig.CreateFont(iFontSize, 0, 0, 0, \
						   nWeight, FALSE, FALSE, 0, \
						   ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, \
						   DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
		pfontCurrent = SelectObject(&Fontbig);
	}
	SetTextColor(Colour);
	ExtTextOut(DataRect->left, DataRect->top, 0, DataRect, szTitle, szTitle.GetLength(), NULL);
	if (bTransparent == true)
	{
		CDC::SetBkMode(OPAQUE);
	}
	if (iFontSize > 0)
	{
		SelectObject(pfontCurrent);
		Fontbig.DeleteObject();
	}
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::DisplayTitle(const int iTitle,
								CRect* DataRect,
								const int x,
								const COLORREF Colour,
								const int iFontSize,
								const bool bTransparent,
								const int nWeight)
{
	CString szTitle;
	szTitle.LoadString(iTitle);
	DisplayTitle(szTitle, DataRect, x, Colour, iFontSize, bTransparent, nWeight);
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::DisplayTitle(CString& szTitle,
								CRect* DataRect,
								const int x,
								const COLORREF Colour,
								const int iFontSize,
								const bool bTransparent,
								const int nWeight)
{
	DataRect->left = x - GetTextExtent(szTitle).cx / 2;
	DisplayText(szTitle, DataRect, Colour, iFontSize, bTransparent, nWeight);
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::DisplayRightTitle(CString& szTitle,
									 CRect* DataRect,
									 const int x,
									 const int y,
									 const COLORREF Colour,
									 const bool bTransparent)
{
	DataRect->left = x - GetTextExtent(szTitle).cx;
	DataRect->top = y;
	DisplayText(szTitle, DataRect, Colour, 18, bTransparent);
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::DisplayLeftTitle(CString& szTitle,
									CRect* DataRect,
									const int x,
									const int y,
									const COLORREF Colour,
									const bool bTransparent)
{
	DataRect->left = x;
	DataRect->top = y;
	DisplayText(szTitle, DataRect, Colour, 18, bTransparent);
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::DrawBlobsRight(CRect* DataRect, int num, COLORREF col, int iBlobWidth, int iBlobHeight)
{
	CRect bar;
	if (num > 0)
	{
		for (int n = 0; n < num; n++)
		{
			bar.SetRect(DataRect->left,
						DataRect->top,
						DataRect->left + iBlobWidth,
						DataRect->top + iBlobHeight);
			FillSolidRect(bar, col);
			DataRect->left += (iBlobWidth + 1);
		}
	}
	DataRect->left = 8;
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::DrawBlobsLeft(CRect* DataRect, int num, COLORREF col, int iBlobWidth, int iBlobHeight)
{
	CRect bar;
	if (num > 0)
	{
		for (int n = 0; n < num; n++)
		{
			bar.SetRect(DataRect->left - iBlobWidth,
						 DataRect->top,
						 DataRect->left,
						 DataRect->top + iBlobHeight);
			FillSolidRect(bar, col);
			DataRect->left -= (iBlobWidth + 1);
		}
	}
	DataRect->left = 8;
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::DrawBar(int YPos, int Value1, int Value2, COLORREF Color1, COLORREF Color2, int iBarHeight)
{
	CRect bar;
	CString str;
	const int iBarLength = 220;
	const int iBarLeft = 10;

	if ((Value1 > 0) || (Value2 > 0))
	{
		int IncVal = iBarLength / (Value1 + Value2);

		bar.SetRect(iBarLeft, YPos, iBarLeft + (IncVal * Value1), YPos + iBarHeight);
		FillSolidRect(bar, Color1);

		bar.SetRect(iBarLeft + (IncVal * Value1), YPos, iBarLeft + iBarLength, YPos + iBarHeight);
		FillSolidRect(bar, Color2);

	}
	else
	{
		// Both bars equal
		bar.SetRect(iBarLeft, YPos, iBarLeft + (iBarLength / 2), YPos + iBarHeight);
		FillSolidRect(bar, Color1);

		bar.SetRect(iBarLeft + (iBarLength / 2), YPos, iBarLeft + iBarLength, YPos + iBarHeight);
		FillSolidRect(bar, Color2);
	}
}


 //---------------------------------------------------------------------------------------------------------------------------
void CExtPaintDC::DrawDoubleBox(int nLineWidth, int nTopX, int nTopY, int nBottomX, int nBottomY, COLORREF nColor)
{
	CPen Scorepen(PS_SOLID, nLineWidth, nColor);
	CPen* pOldPen = SelectObject(&Scorepen);
	Rectangle(nTopX, nTopY, nBottomX, nBottomY);
	Rectangle(nTopX + (2 * nLineWidth), nTopY + (2 * nLineWidth), nBottomX - (2 * nLineWidth), nBottomY - (2 * nLineWidth));
	SelectObject(pOldPen);
}
