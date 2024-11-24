/////////////////////////////////////////////////////////////////////////////
// CLWFormView - generic view constructed from a dialog template
#ifndef __LWFORMVIEW_H__
#define __LWFORMVIEW_H__

class CLWFormView : public CView
{
	DECLARE_DYNAMIC(CLWFormView)

// Construction
protected:      // must derive your own class
	CLWFormView(LPCTSTR lpszTemplateName);
	CLWFormView(UINT nIDTemplate);

// Implementation
public:
	virtual void			OnInitialUpdate();

protected:
	LPCTSTR					m_lpszTemplateName;
	CCreateContext*			m_pCreateContext;
	HWND					m_hWndFocus;   // last window to have focus

	virtual void			OnDraw(CDC* pDC);      // default does nothing
	// special case override of child window creation
	virtual BOOL			Create(LPCTSTR, LPCTSTR, DWORD, const RECT&, CWnd*, UINT, CCreateContext*);
	virtual BOOL			PreTranslateMessage(MSG* pMsg);
	virtual void			OnActivateView(BOOL, CView*, CView*);
	virtual void			OnActivateFrame(UINT, CFrameWnd*);
	BOOL					SaveFocusControl();    // updates m_hWndFocus

	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};

#endif //__LWFORMVIEW_H__
