//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : dib.h
// Description : DIB container
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef EDITOR_UI_SUPPORT_DIB_H_INCLUDED
#define EDITOR_UI_SUPPORT_DIB_H_INCLUDED

#include <lemon/toolhelp/wtl/latl.h>

namespace WTL {

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// DIB - DIB container
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class DIB
{
private:
  CDCHandle & m_dc;

public:
  DIB( CDCHandle &dc ) : m_dc(dc) { }

  void CopyToScreen( CRect &srcRect, CRect &dstRect, int width, int height, void * data )
  {
    BITMAPINFO bmi[2];

    bmi[0].bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
    bmi[0].bmiHeader.biPlanes        = 1;
    bmi[0].bmiHeader.biBitCount      = 0;
    bmi[0].bmiHeader.biXPelsPerMeter = 72;
    bmi[0].bmiHeader.biYPelsPerMeter = 72;
    bmi[0].bmiHeader.biClrUsed       = 0;
    bmi[0].bmiHeader.biClrImportant  = 0;

    bmi[0].bmiHeader.biCompression   = BI_RGB;
    bmi[0].bmiHeader.biBitCount      = 32;
    bmi[0].bmiHeader.biWidth         = width;
    bmi[0].bmiHeader.biHeight        = -height;

    int srcX = srcRect.left;
    int srcY = srcRect.top;
    int dstX = dstRect.left;
    int dstY = dstRect.top;
    int dstW = dstRect.Width();
    int dstH = dstRect.Height();

    m_dc.SetDIBitsToDevice( dstX, dstY, dstW, dstH, srcX, srcY, 0, dstH, data, bmi, DIB_RGB_COLORS);
  }

  void ScaleToScreen( CRect &srcRect, CRect &dstRect, int width, int height, void * data )
  {
    BITMAPINFO bmi[2];

    bmi[0].bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
    bmi[0].bmiHeader.biPlanes        = 1;
    bmi[0].bmiHeader.biBitCount      = 0;
    bmi[0].bmiHeader.biXPelsPerMeter = 72;
    bmi[0].bmiHeader.biYPelsPerMeter = 72;
    bmi[0].bmiHeader.biClrUsed       = 0;
    bmi[0].bmiHeader.biClrImportant  = 0;

    bmi[0].bmiHeader.biCompression   = BI_RGB;
    bmi[0].bmiHeader.biBitCount      = 32;
    bmi[0].bmiHeader.biWidth         = width;
    bmi[0].bmiHeader.biHeight        = -height;

    int srcX = srcRect.left;
    int srcY = srcRect.top;
    int srcW = srcRect.Width();
    int srcH = srcRect.Height();
    int dstX = dstRect.left;
    int dstY = dstRect.top;
    int dstW = dstRect.Width();
    int dstH = dstRect.Height();

    m_dc.StretchDIBits( dstX, dstY, dstW, dstH, srcX, srcY, srcW, srcH, data, bmi, DIB_RGB_COLORS, SRCCOPY);

  }
};

}
#endif
