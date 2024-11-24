#if !defined(AFX_SPLASHSCREEN_H__B7A1B87E_272F_4CBA_8E3A_092F3F04D0DA__INCLUDED_)
#define AFX_SPLASHSCREEN_H__B7A1B87E_272F_4CBA_8E3A_092F3F04D0DA__INCLUDED_

// SplashScreen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplashScreen dialog

class CSplashScreen : public CDialog
{
// Construction
public:
	CSplashScreen(CWnd* pParent = null);   // standard constructor

// Dialog Data
	enum 
	{ 
		ID_SPLASHSCREEN = 423456,
			
		IDD = IDD_SPLASH 
	};

	BOOL					ShowSplashScreen(CPoint BitmapCoords, DWORD dwDisplayTime = 2500, DWORD dwResourceID = IDR_PNGLOGO);
	int						DoModal(CPoint BitmapCoords, DWORD dwDisplayTime, DWORD dwResourceID = IDR_PNGLOGO);
	void					TurnOff();

// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:

// Implementation
	DWORD					m_dwDisplayTime;
    CVOImage				m_bmpLogo;
	CPoint					m_BitmapCoords;
	bool					m_bModal;
	bool					m_bKill;
	
	// Generated message map functions
	BOOL					OnEraseBkgnd(CDC* pDC);
	void					SetBitmap(DWORD dwResourceID);
	afx_msg int				OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL			OnInitDialog();
	virtual void			OnTimer(UINT nIDEvent);
	afx_msg void			OnPaint();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SPLASHSCREEN_H__B7A1B87E_272F_4CBA_8E3A_092F3F04D0DA__INCLUDED_)
