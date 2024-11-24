 // OffScreenBuffer.h: interface for the COffScreenBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OFFSCREENBUFFER_H__0F7274BD_B839_49BF_B21A_DC3A5615C3D3__INCLUDED_)
#define AFX_OFFSCREENBUFFER_H__0F7274BD_B839_49BF_B21A_DC3A5615C3D3__INCLUDED_

#ifdef ORIGINAL_CODE

enum eBorderType
{
	NOBORDER,
	OUTERBORDER,
	INNERBORDER
};


class COffScreenBuffer : public CDC
{
public:
	COffScreenBuffer() {};
	COffScreenBuffer(const ushort nMaxX, const ushort nMaxY, CWnd* pParentWnd);
	COffScreenBuffer(CWnd* pParentWnd);
	virtual ~COffScreenBuffer();

	void					Blit(const ushort _TopY = 0);
	void					DestroyBitmap();
	void					DisplayLeftTitle(CString& _szTitle, CRect* _pDataRect, const int _x, const int _y, const COLORREF _Colour = BLACK,   const bool _bTransparent = false);
	void					DisplayNoData(const int _String);
	void					DisplayNumeric(int _Value, CRect* _pDataRect, const int _x, const int _y, const COLORREF _Colour = BLACK, const bool _bTransparent = false, int _FontSize = 18);
	void					DisplayRightTitle(CString& _szTitle, CRect* _pDataRect, const int _x, const int _y, const COLORREF _Colour = BLACK, const bool _bTransparent = false, int _FontSize = 18);
	void					DisplayText(CString& szTitle, CRect* _pDataRect, const COLORREF _Colour = BLACK, const int _FontSize = 18, const bool _bTransparent = false, const int _Weight = FW_MEDIUM);
	void					DisplayText(const int _Title, CRect* _pDataRect, const COLORREF _Colour = BLACK, const int _FontSize = 18, const bool _bTransparent = false, const int _Weight = FW_MEDIUM);
	void					DisplayTitle(CString& szTitle, CRect* DataRect, const int x, const COLORREF Colour = BLACK, const int iFontSize = 18, const bool bTransparent = false, const int nWeight = FW_MEDIUM);
	void					DisplayTitle(const int iTitle, CRect* DataRect, const int x, const COLORREF Colour = BLACK, const int iFontSize = 18, const bool bTransparent = false, const int nWeight = FW_MEDIUM);
	void					DoDrawBlobsLeft(CRect& _DataRect, const int _Num, COLORREF _Colour, const int _BlobWidth = 8, const int _BlobHeight = 8);
	void					DoDrawBlobsRight(CRect& _DataRect, const int _Num, const COLORREF _Colour, const int _BlobWidth = 8, const int _BlobHeight = 8);
	void					DrawNumRight(const int _x, CRect _DataRect);
	void					DrawBar(const int _YPos, const int _Value1, const int _Value2, const COLORREF _Colour1, const COLORREF _Colour2, const int _BarHeight = 17);
	void					DrawSeperator(const int y, const COLORREF color = BLACK);
	void					DrawLine(CPoint& apointFrom, CPoint& apointTo, const COLORREF color = BLACK);
	void					DoCreateBitmap();
	void					DoInitialise(const ushort _MaxX, const ushort _MaxY, CWnd* _ParentWnd);
	int						GetBufferWidth();
	int						GetBufferHeight();
	CWnd*					GetParentWnd();
	void					SetParentWnd(CWnd* _pParentWnd);
	void					Stripes(const int _HowMany, int _Top, const bool _bScrollBarVisible);
	void					StripedRect(const int _HowMany, CRect _TheRect, const COLORREF _color[2], const eBorderType _bBorder = OUTERBORDER);

protected:
	CWnd*					m_pParentWnd;
	ushort					m_BufferWidth;
	ushort					m_BufferHeight;
	CBitmap					m_bmp;
	CBrush					m_bkBrush;
	HBITMAP					m_hOldBitmap;
};

#endif

#endif // !defined(AFX_OFFSCREENBUFFER_H__0F7274BD_B839_49BF_B21A_DC3A5615C3D3__INCLUDED_)
