 #if !defined(AFX_MAINFRM_H__CCFE8752_FD0F_4792_8A9F_16C336D92DF9__INCLUDED_)
#define AFX_MAINFRM_H__CCFE8752_FD0F_4792_8A9F_16C336D92DF9__INCLUDED_

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

class CMainFrame : public CFrameWnd
{
private:		// create from serialization only
	CMainFrame();
	virtual ~CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:

// Overrides
	virtual BOOL			PreCreateWindow(CREATESTRUCT& cs);
	BOOL					OnHelpInfo(HELPINFO* pHelpInfo);

// Implementation

private:
	// control bar embedded members
#ifdef POCKETPC
	CCommandBar				m_wndCommandBar;
#endif
	eView					m_viewCurrent;

	int						OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_MAINFRM_H__CCFE8752_FD0F_4792_8A9F_16C336D92DF9__INCLUDED_)
