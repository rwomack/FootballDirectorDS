#if !defined(AFX_SLIDESHOW_H__B7A1B87E_272F_4CBA_8E3A_092F3F04D0DA__INCLUDED_)
#define AFX_SLIDESHOW_H__B7A1B87E_272F_4CBA_8E3A_092F3F04D0DA__INCLUDED_

// SlideShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSlideShow dialog

class CSlideShow : public CSplashScreen 
{
// Construction
public:
	CSlideShow () : CSplashScreen(NULL)
	{
		m_nCount = 0;
	}

	int						DoModal(DWORD dwDisplayTime, CDWordArray* dwList)
	{
		ASSERT(dwList != NULL);
		m_pList = dwList;
		
		m_bModal = true;
		m_dwDisplayTime = dwDisplayTime;
		HDC dc = ::GetDC(m_hWnd);
		m_bmpLogo.SetBitmap(dc, m_pList->GetAt(0), _T("PNG"));
		::ReleaseDC(m_hWnd, dc);
		return CDialog::DoModal();
	}

protected:
	int						m_nCount;
	CDWordArray*			m_pList;

	virtual void			OnTimer(UINT nIDEvent)
	{
		if (nIDEvent == ID_SPLASHSCREEN)
		{
			m_nCount++;
			if (m_nCount < m_pList->GetSize())
			{
				CSplashScreen::SetBitmap(m_pList->GetAt(m_nCount));
				SetTimer(ID_SPLASHSCREEN, m_dwDisplayTime, NULL);
			}
			else
			{
				CSplashScreen::OnTimer(nIDEvent);
			}
		}
	}

};
#endif