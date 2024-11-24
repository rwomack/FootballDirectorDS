#ifndef __EXTPAINTDC_H__
#define __EXTPAINTDC_H__

#include "COLOURS.h"

class CExtPaintDC : public CPaintDC
{
// Construction
public:
	CExtPaintDC(CWnd* pWnd);
	~CExtPaintDC(void);

	void					BevelRect(const int x1, const int y1, const int x2, const int y2, const int BevelLength);
	void					SelectFont(CFont* Font);
	void					SelectNormalFont(void);
	void					DrawNumRight(int x, CRect DataRect);
	void					DisplayText(CString& szTitle, CRect* DataRect, const COLORREF Colour = BLACK,
										const int iFontSize = 18, const bool bTransparent = false, const int nWeight = FW_MEDIUM);
	void					DisplayTitle(CString& szTitle, CRect* DataRect, const int x, const COLORREF Colour = BLACK,
										 const int iFontSize = 18, const bool bTransparent = false, const int nWeight = FW_MEDIUM);
	void					DisplayTitle(const int iTitle, CRect* DataRect, const int x, const COLORREF Colour = BLACK,
										 const int iFontSize = 18, const bool bTransparent = false, const int nWeight = FW_MEDIUM);
	void					DisplayLeftTitle(CString& szTitle, CRect* DataRect, const int x, const int y, const COLORREF Colour = BLACK,
											 const bool bTransparent = false);
	void					DisplayRightTitle(CString& szTitle, CRect* DataRect, const int x, const int y, const COLORREF Colour = BLACK,
											  const bool bTransparent = false);
	void					DisplayNoData(const int iString);
	void					DrawBlobsLeft(CRect* DataRect, int num, COLORREF col, int iBlobWidth = 8, int iBlobHeight = 8);
	void					DrawBlobsRight(CRect* DataRect, int num, COLORREF col, int iBlobWidth = 8, int iBlobHeight = 8);
	void					DrawBar(int YPos, int Value1, int Value2, COLORREF Color1, COLORREF Color2, int iBarHeight = 17);
	void					DrawDoubleBox(int nLineWidth, int nTopX, int nTopY, int nBottomX, int nBottomY, COLORREF nColor);
	void					DrawSeperator(const int y, const COLORREF color = BLACK);
	void					Stripes(const int iHowMany, int iTop, const bool ScrollBarVisible);
	void					StripedRect(const int iHowMany, CRect TheRect, const COLORREF color[2], const bool bBorder = true);
	inline void				UnSelectFont(void);

protected:
	CString					m_strPrint;
	CFont					m_NormalFont;
    CFont*					m_pfontCurrent;
};
#endif
