
/********************************************************************
	created:	23:7:2003   9:53
	filename: 	D:\PROJECTS\SYSTEM\ExtOffScreenBuffer.h

	author:		R. Womack
	copyright:	Rw Software 2003-2007
*********************************************************************/

#ifdef ORIGINAL_CODE

class CExtOffScreenBuffer : public COffScreenBuffer
{
public:
	CExtOffScreenBuffer(CWnd* pParentWnd) : COffScreenBuffer(pParentWnd) {};

	enum eBorderType
	{
		NOBORDER,
		OUTERBORDER,
		INNERBORDER
	};
	void					Stripes(const int _HowMany, int _Top, const bool _bScrollBarVisible);
	void					StripedRect(const int _HowMany, CRect _TheRect, const COLORREF _color[2], const eBorderType _bBorder = OUTERBORDER);
	void					DisplayNoData(const int _String);
	void					DisplayNumeric(int _Value, CRect* _pDataRect, const int _x, const int _y, const COLORREF _Colour /*= BLACK*/, const bool _bTransparent /*= false*/, int _FontSize /*= 18*/);
	void					DisplayText(CString& szTitle, CRect* _pDataRect, const COLORREF _Colour = BLACK, const int _FontSize = 18, const bool _bTransparent = false, const int _Weight = FW_MEDIUM);
	void					DisplayText(const int _Title, CRect* _pDataRect, const COLORREF _Colour = BLACK, const int _FontSize = 18, const bool _bTransparent = false, const int _Weight = FW_MEDIUM);
	void					DisplayTitle(CString& szTitle, CRect* DataRect, const int x, const COLORREF Colour = BLACK,
										 const int iFontSize = 18, const bool bTransparent = false, const int nWeight = FW_MEDIUM);
	void					DisplayTitle(const int iTitle, CRect* DataRect, const int x, const COLORREF Colour = BLACK,
										 const int iFontSize = 18, const bool bTransparent = false, const int nWeight = FW_MEDIUM);
	void					DisplayLeftTitle(CString& _szTitle, CRect* _pDataRect, const int _x, const int _y, const COLORREF _Colour = BLACK,   const bool _bTransparent = false);
	void					DisplayRightTitle(CString& _szTitle, CRect* _pDataRect, const int _x, const int _y, const COLORREF _Colour = BLACK, const bool _bTransparent = false, int _FontSize = 18);
	void					DoDrawBlobsLeft(CRect& _DataRect, const int _Num, COLORREF _Colour, const int _BlobWidth = 8, const int _BlobHeight = 8);
	void					DoDrawBlobsRight(CRect& _DataRect, const int _Num, const COLORREF _Colour, const int _BlobWidth = 8, const int _BlobHeight = 8);
	void					DrawBar(const int _YPos, const int _Value1, const int _Value2, const COLORREF _Colour1, const COLORREF _Colour2, const int _BarHeight = 17);
	void					DrawNumRight(const int _x, CRect _DataRect);

protected:
};

#endif
