#if !defined(AFX_PNGCLICKRECT_H__D4B7F3C6_2868_40CD_9686_5A4A299203E6__INCLUDED_)
#define AFX_PNGCLICKRECT_H__D4B7F3C6_2868_40CD_9686_5A4A299203E6__INCLUDED_

// PngClickRect.h : header file
//

// (c) RW Software 2003


// Display png graphic from resource and detect if been clicked on

class CPngClickRect
{
public:

	// ---------------------------------------------------------------------------
	void					OnInit(CRect theRect, HDC hdc, DWORD dwResourceID, LPCTSTR pcszClass, HMODULE hModule = 0)
	{
		m_rectPicture = theRect;
		m_pngPicture.SetBitmap(hdc, dwResourceID, pcszClass, hModule);
	}
	
	// ---------------------------------------------------------------------------
	void					SetPNG(HDC hdc, DWORD dwResourceID, LPCTSTR pcszClass, HMODULE hModule = 0)
	{
		m_pngPicture.SetBitmap(hdc, dwResourceID, pcszClass, hModule);
	}

	// ---------------------------------------------------------------------------
	void					OnPaint(HDC hdc)
	{
		m_pngPicture.DrawTransparent(hdc, m_rectPicture.left, m_rectPicture.top, -1, -1, RGB(255, 0, 255));
	}
	
	// ---------------------------------------------------------------------------
	BOOL					PtInRect(POINT point)
	{
		return m_rectPicture.PtInRect(point);
	}

protected:
	CVOImage				m_pngPicture;
	CRect					m_rectPicture;

};
/////////////////////////////////////////////////////////////////////////////
#endif