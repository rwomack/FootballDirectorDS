
/*
  ---------------------------------------------------------------------------------------

	File:- PNGDrawInfo.h: interface for the CPNGDrawInfo class.

	  
	(c) Rw Software 1994 - 2007
		
  ---------------------------------------------------------------------------------------
*/


#if !defined(AFX_PNGDRAWINFO_H__209592E0_69A8_4865_8364_D687DEB39325__INCLUDED_)
#define AFX_PNGDRAWINFO_H__209592E0_69A8_4865_8364_D687DEB39325__INCLUDED_



class CPNGDrawInfo : public CVOImage  
{
public:
	CPNGDrawInfo();
	virtual ~CPNGDrawInfo();

//	void DoDraw();
	void					DoDraw(const HDC ahDC, const int aXPos, const int aYPos, const int aImageNumber = 0);
	void					DoDrawTransparent(const HDC ahDC, const int aXPos, const int aYPos, const int aImageNumber = 0, const COLORREF aTransparentColor = RGB(192, 192, 192));
	void					DoInitialise(const HDC ahDC, const int aGFXID, const int aNumImages = 1);
	CVOImage&				GetImage();
	int						GetHeight() const;
	int						GetWidth() const;
	void					SetOffsetX(const int aOffsetX);
	void					SetOffsetY(const int aOffsetY);

protected:
	int						m_OffsetY;
	int						m_OffsetX;
	int						m_Height;
	int						m_Width;
};

#endif // !defined(AFX_PNGDRAWINFO_H__209592E0_69A8_4865_8364_D687DEB39325__INCLUDED_)
