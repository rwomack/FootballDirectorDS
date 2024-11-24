
/*
  ---------------------------------------------------------------------------------------

	File:- PNGDrawInfo.h: interface for the CPNGDrawInfo class.

	  
	(c) Rw Software 1994 - 2008
		
  ---------------------------------------------------------------------------------------
*/


#if !defined(AFX_PNGDRAWINFO_H__209592E0_69A8_4865_8364_D687DEB39325__INCLUDED_)
#define AFX_PNGDRAWINFO_H__209592E0_69A8_4865_8364_D687DEB39325__INCLUDED_



class CPNGDrawInfo : public CVOImage  
{
public:
	CPNGDrawInfo();
	virtual ~CPNGDrawInfo();

	void					DoDraw(const HDC _hDC, const int _XPos, const int _YPos, const int _ImageNumber = 0);
	void					DoDraw(const HDC _hDC, const int _XPos, const int _YPos, const int _Width, const int _Height);
	void					DoDrawTransparent(const HDC _hDC, const int _XPos, const int _YPos, const int _ImageNumber = 0, const COLORREF _TransparentColor = RGB(192, 192, 192));
	void					DoInitialise(const HDC _hDC, const int _GFXID, const int _NumImages = 1);
	CVOImage&				GetImage();
	int						GetHeight() const;
	int						GetWidth() const;
	void					SetOffsetX(const int _OffsetX);
	void					SetOffsetY(const int _OffsetY);

protected:
	int						m_OffsetY;
	int						m_OffsetX;
	int						m_Height;
	int						m_Width;
};

#endif // !defined(AFX_PNGDRAWINFO_H__209592E0_69A8_4865_8364_D687DEB39325__INCLUDED_)
